/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQCompress.cpp                                                 *
*                                                                        *
*  This file is part of FFQueue.                                         *
*                                                                        *
*  FFQueue is free software: you can redistribute it and/or modify       *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
*  FFQueue is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*  GNU General Public License for more details.                          *
*                                                                        *
*  You should have received a copy of the GNU General Public License     *
*  along with FFQueue.  If not, see <http://www.gnu.org/licenses/>.      *
*                                                                        *
*************************************************************************/

#include "FFQCompress.h"
#include "FFQMisc.h"
#include <zlib.h>
#include <wx/base64.h>

//The pack pragma's are important! Do not remove or change them!
//They ensure that the BASE64_DATA_HEADER is not being 32bit or
//64bit aligned and thus padded. The structure MUST remain packed
//with one-byte alignment.

#pragma pack(push)
#pragma pack(1)
typedef struct BASE64_DATA_HEADER
{

    uint16_t FFFF; //Validation header has the value of 0xFFFF if valid.
    uint32_t size; //The size of the decompressed data.

    BASE64_DATA_HEADER() { FFFF=0xFFFF; size=0; };

} BASE64_DATA_HEADER;
#pragma pack(pop)

//---------------------------------------------------------------------------------------

void CompressAndBase64(const void* src_buf, const size_t src_len, wxString &base64, size_t wrap_len)
{

    if (src_len == 0)
    {
        //Never mind empty buffers
        base64 = "";
        return;
    }

    //The header for the data, 6 bytes = 0xFFFF + unsigned 32bit integer for size
    BASE64_DATA_HEADER hdr;
    hdr.size = src_len;

    //The size of the data in a zlib compatible value
    long unsigned int len = src_len;

    //A buffer to hold the data
    unsigned char* buf = new unsigned char[hdr.size + sizeof(hdr)];

    //Compress the data
    if (compress2(&buf[sizeof(hdr)], &len, (unsigned char*)src_buf, len, Z_BEST_COMPRESSION) != Z_OK)
    {

        //In case of an error we store the original data
        memcpy(&buf[sizeof(hdr)], src_buf, src_len);

        //Set data size to "0" to indicate no compression
        hdr.size = 0;

    }

    //Move the header to the buffer
    memcpy(buf, &hdr, sizeof(hdr));

    //Encode the buffer to base64
    base64 = wxBase64Encode(buf, len + sizeof(hdr));

    //Wrap if necessary
    if (wrap_len > 0) Base64Wrap(base64, wrap_len);

    //Release buffer
    delete[] buf;

}

//---------------------------------------------------------------------------------------

void CompressAndBase64(wxString &src_and_dst, size_t wrap_len)
{

    //Get UTF8 data buffer
    wxCharBuffer scb = src_and_dst.ToUTF8();

    //Call buffer overload
    CompressAndBase64((void*)scb.data(), scb.length(), src_and_dst, wrap_len);

}

//---------------------------------------------------------------------------------------

bool DecompressFromBase64(wxString &base64, void* dest_buf, size_t* dest_len)
{

    //Unwrap base64 (not required when using wxBase64DecodeMode_Relaxed but we need the actual base64 length)
    Base64Unwrap(base64);

    //We do not care about empty base64
    if (base64.Len() == 0) return false;

    //Get the header from the first 8 characters in base64
    BASE64_DATA_HEADER hdr;

    //Base64 is too short to contain a header - set control bytes to 0
    if (base64.Len() < 8) hdr.FFFF = 0;

    //Decode the header and return false if not possible
    else if (wxBase64Decode(&hdr, sizeof(hdr), base64.SubString(0, 7)) != sizeof(hdr)) return false;

    //Get the total size of the encoded data
    long unsigned int datalen = Base64DataLen(base64);

    //The required size of the buffer
    size_t required = datalen;

    if (hdr.FFFF == 0xFFFF) //Header assumed to be valid
    {

        //The data is uncompressed, subtract size of header
        if (hdr.size == 0) required -= sizeof(hdr);

        //The data is compressed, get uncompressed size from header
        else required = hdr.size;

    }

    //Check if destBuf is large enough
    if (*dest_len < required)
    {

        //Not big enough - return requirements
        *dest_len = required;
        return false;

    }

    //Get pointer to base64 chars
    char *cb = (char*)base64.c_str().AsChar();

    if (hdr.FFFF == 0xFFFF) //Header assumed to be valid
    {

        //Skip the first 8 base64 chars containing the header
        cb += 8;

        //Decrease the datalen with the size of the header
        datalen -= sizeof(hdr);

        if (hdr.size > 0) //Compressed data
        {

            //Make a buffer for decoded data
            unsigned char *buf = new unsigned char[datalen];

            //Decode data
            bool ok = (wxBase64Decode(buf, datalen, cb) == datalen);

            if (ok) //Decode succeeded
            {

                //Decompress decoded data to destBuf
                long unsigned int dl = *dest_len;
                ok = (uncompress((unsigned char*)dest_buf, &dl, buf, datalen) == Z_OK);

                //If succeeded adjust the destLen
                if (ok) *dest_len = dl;


            }

            //Release decoding buffer
            delete[] buf;

            //Return result
            return ok;

        }

    }

    //The data is uncompressed - fetch it from cb to destBuf
    //"cb" has been adjusted to skip header (if present) - do not use "base64" as source
    if (wxBase64Decode(dest_buf, datalen, cb) == datalen)
    {

        //Success - adjust destLen
        *dest_len = datalen;
        return true;

    }

    //Error
    return false;

}

//---------------------------------------------------------------------------------------

bool DecompressFromBase64(wxString &base64)
{

    //Variable for size of required buffer
    size_t len = 0;

    //Get the needed buffer size
    DecompressFromBase64(base64, NULL, &len);

    //Are we green?
    bool ok = (len > 0);

    if (ok)
    {

        //Make a buffer
        unsigned char *buf = new unsigned char[len];

        //Decompress the data to the buffer
        ok = DecompressFromBase64(base64, buf, &len);

        if (ok)
        {

            //Check for double byte characters / unicode
            if ((len > 1) && ((buf[0] == 0) || (buf[1] == 0))) base64 = wxString((wchar_t*)buf, len / sizeof(wchar_t));

            //Otherwise UTF-8 is assumed
            else base64 = wxString::FromUTF8((char*)buf, len);

        }

        //Release buffer
        delete[] buf;

    }

    //Clear base64 if no success
    if (!ok) base64.Clear();

    //Return result
    return ok;

}
