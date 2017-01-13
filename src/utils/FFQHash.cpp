/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQHash.cpp                                                     *
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

#include "FFQHash.h"
#include <wx/file.h>

//---------------------------------------------------------------------------------------

//Helper macros
#define Shl32(w32,bits) (((w32) << (bits)) | ((w32) >> (32-(bits))))
#define ReverseBytes(w32) (((w32) >> 24) | (((w32) >> 8) & 0xFF00) | (((w32) << 8) & 0xFF0000) | ((w32) << 24))

//---------------------------------------------------------------------------------------

void SHA1_Compress(SHA1_Context* ctxt)
{
    uint32_t a, b, c, d, e, t, i, w[80];

    for (i = 0; i < 16; i++) w[i] = ReverseBytes(*((uint32_t*)ctxt->buffer + i));

    for (i = 16; i < 80; i++) w[i] = Shl32(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

    a = ctxt->hash[0];
    b = ctxt->hash[1];
    c = ctxt->hash[2];
    d = ctxt->hash[3];
    e = ctxt->hash[4];

    for (i = 0; i < 20; i++) {

        t = Shl32(a, 5) + ((b & c) | ((~b) & d)) + e + w[i] + 0x5A827999;
        e = d;
        d = c;
        c = Shl32(b, 30);
        b = a;
        a = t;

    }

    for (i = 20; i < 40; i++) {

        t = Shl32(a, 5) + (b ^ c ^ d) + e + w[i] + 0x6ED9EBA1;
        e = d;
        d = c;
        c = Shl32(b, 30);
        b = a;
        a = t;

    }

    for (i = 40; i < 60; i++) {

        t = Shl32(a, 5) + ((b & c) | (b & d) | (c & d)) + e + w[i] + 0x8F1BBCDC;
        e = d;
        d = c;
        c = Shl32(b, 30);
        b = a;
        a = t;

    }

    for (i = 60; i < 80; i++) {

        t = Shl32(a, 5) + (b ^ c ^ d) + e + w[i] + 0xCA62C1D6;
        e = d;
        d = c;
        c = Shl32(b, 30);
        b = a;
        a = t;

    }

    ctxt->hash[0]+= a;
    ctxt->hash[1]+= b;
    ctxt->hash[2]+= c;
    ctxt->hash[3]+= d;
    ctxt->hash[4]+= e;

    ctxt->index = 0;

}

//---------------------------------------------------------------------------------------

bool SHA1_SelfTest()
{

    char res[20] = {'\xA9','\x99','\x3E','\x36',
                    '\x47','\x06','\x81','\x6A',
                    '\xBA','\x3E','\x25','\x71',
                    '\x78','\x50','\xC2','\x6C',
                    '\x9C','\xD0','\xD8','\x9D'};
    char buf[3]  = {'a','b','c'};
    SHA1_Context sha1;
    SHA1_Init(&sha1);
    SHA1_Append(&sha1, &buf, sizeof(buf));
    SHA1_Finish(&sha1);
    return memcmp(&sha1.hash, &res, sizeof(res));

}

//---------------------------------------------------------------------------------------

void SHA1_Init(SHA1_Context* ctxt)
{

    ctxt->hi      = 0;
    ctxt->lo      = 0;
    ctxt->index   = 0;
    ctxt->hash[0] = 0x67452301;
    ctxt->hash[1] = 0xEFCDAB89;
    ctxt->hash[2] = 0x98BADCFE;
    ctxt->hash[3] = 0x10325476;
    ctxt->hash[4] = 0xC3D2E1F0;

}

//---------------------------------------------------------------------------------------

void SHA1_Append(SHA1_Context* ctxt, void* buf, uint32_t len)
{

    uint32_t i;
    char* ba = (char*)buf;

    for (i = 0; i < len; i++) {

        ctxt->lo+= 8;
        if (ctxt->lo == 0) ctxt->hi++;

        ctxt->buffer[ctxt->index++] = *ba;
        ba++;

        if (ctxt->index == 64) SHA1_Compress(ctxt);

    }

}

//---------------------------------------------------------------------------------------

void SHA1_Finish(SHA1_Context* ctxt)
{

    ctxt->buffer[ctxt->index++] = 0x80;

    if (ctxt->index > 56) {

       while (ctxt->index < 64) ctxt->buffer[ctxt->index++] = 0;
       SHA1_Compress(ctxt);

    } else {

       while (ctxt->index < 56) ctxt->buffer[ctxt->index++] = 0;

    }

    uint32_t* sw = (uint32_t*)&ctxt->buffer[56];
    *sw = ReverseBytes(ctxt->hi);
    sw++;
    *sw = ReverseBytes(ctxt->lo);

    SHA1_Compress(ctxt);

    for (int i = 0; i < 5; i++) ctxt->hash[i] = ReverseBytes(ctxt->hash[i]);

}

//---------------------------------------------------------------------------------------

bool SHA1_File(wxString filename, SHA1_Context* sha1)
{

    wxFile *f = new wxFile(filename, wxFile::read);
    char* buf = (char*)malloc(10240);
    bool ok = false;

    try {

        SHA1_Init(sha1);

        while (!f->Eof()) {

            wxFileOffset rd = f->Length() - f->Tell();
            if (rd > 10240) rd = 10240;
            rd = f->Read(buf, rd);
            SHA1_Append(sha1, buf, rd);

        }

        SHA1_Finish(sha1);
        ok = true;

    } catch (...) { }

    free(buf);
    f->Close();
    delete f;

    return ok;

}

//---------------------------------------------------------------------------------------

void SHA1_Buffer(SHA1_Context* sha1, void* buf, uint32_t len)
{

    SHA1_Init(sha1);
    SHA1_Append(sha1, buf, len);
    SHA1_Finish(sha1);

}

//---------------------------------------------------------------------------------------

void SHA1_ToHex(SHA1_Context* sha1, wxString &hex)
{

    hex.clear();
    wxString s;
    unsigned char* hash = (unsigned char*)sha1->hash;
    for (size_t i = 0; i < sizeof(sha1->hash); i++) hex += wxString::Format("%02x", hash[i]);

}
