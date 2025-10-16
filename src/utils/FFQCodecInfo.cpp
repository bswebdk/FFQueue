/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQCodecInfo.cpp                                                *
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

#include "FFQCodecInfo.h"
#include "FFQMisc.h"
#include "FFQConfig.h"
#include "FFQConst.h"

//---------------------------------------------------------------------------------------

const int QUANTIZER_BEST_QUALITY = 0;
const int QUANTIZER_WORST_QUALITY = 63;

const wxString DEFAULT_VIDEO_CODEC_INFO = "*v";
const wxString DEFAULT_AUDIO_CODEC_INFO = "*a";

//---------------------------------------------------------------------------------------


//This is the default codec information used to create
//the default list of CODEC_INFO's. The list can be overwritten
//or extended by adding a section the the configuration
//file named [codec_info] where each line is conform with
//the default list:

//[codec identifyer]=[friendly(0|1)],[minimum qscale quality],[maximum qscale quality],[minimum crf quality],[maximum crf quality]

//If minimum value (qscale or crf) equals maximum the feature is not
//supported and will be grayed out in the preset editor

const wxString DEFAULT_CODEC_INFO = "*v=0,31,1,0,0\n" //Values used for any video codecs without info
                                    "*a=0,31,1,0,0\n" //Values used for any audio codecs without info
                                    "libx264=1,0,0,63,0\n"  //Might be 51..0 for 8bit
                                    "libopenh264=1,0,0,63,0\n"
                                    "libx264rgb=1,0,0,63,0\n"  //Untested
                                    "mpeg4=1,31,1,0,0\n"
                                    "flv=1,31,1,0,0\n"
                                    "libxvid=1,31,1,0,0\n"
                                    "libx265=1,0,0,51,0\n"  //Untested
                                    "mjpeg=1,31,1,0,0\n"
                                    "libvpx=1,0,0,63,0\n" //Was before V1.7.62: "libvpx=1,0,0,63,4\n"
                                    "libvpx-vp9=1,0,0,63,0\n" //As above "libvpx-vp9=1,0,0,63,4\n"
                                    "libaom-av1=1,0,0,63,0\n"
                                    //"librav1e=1,0,0,255,0\n"
                                    "libsvtav1=1,0,0,63,0\n"
                                    "wmv1=1,31,1,0,0\n"
                                    "wmv2=1,31,1,0,0\n"
                                    "libmp3lame=1,9,0,0,0\n"
                                    "aac=1,0.1f,2,0,0\n"
                                    "mp3=1,9,0,0,0\n"
                                    "flac=1,0,0,0,0\n"
                                    "pcm_s16le=1,0,0,0,0\n"
                                    "pcm_s24le=1,0,0,0,0\n"
                                    "ac3=1,0,0,0,0\n"
                                    "ac3_fixed=1,0,0,0,0\n"
                                    "libvo_aacenc=1,0,0,0,0\n"
                                    "alac=1,0,0,0,0\n"
                                    "libvorbis=1,-1,10,0,0\n"
                                    "wmav1=1,0,0,0,0\n"
                                    "wmav2=1,0,0,0,0\n";

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

float GetNumericPart(wxString &from, bool &is_float, float def)
{

    //Returns the value of a numeric part and examines
    //if it is an integral value or a floating point

    wxString s = GetToken(from, ",", true);
    if (s.Right(1) == "f")
    {
        is_float = true;
        s.RemoveLast();
    }
    return Str2Float(s, def);

}

//---------------------------------------------------------------------------------------

bool GetPctValue(int &pct, float &low, float &high, bool &as_float, float *result)
{
    //Return false if values are equal
    if (CmpFloats(low, high)) return false;

    //If a result is wanted, convert a percentage (0..100) to the range of low..high
    if (result) *result = as_float ? ConvertPctToMinMaxFloat(pct, low, high) : ConvertPctToMinMaxInt(pct, low, high);

    //Return values differ
    return true;
}

//---------------------------------------------------------------------------------------

wxString GetPctString(int &pct, float &low, float &high, bool &as_float)
{
    //Returns a string representation of GetPctValue
    float f = 0;
    if (!GetPctValue(pct, low, high, as_float, &f)) return wxEmptyString;
    return as_float ? wxString::Format("%.2f", f) : wxString::Format("%i", (int)f);
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

CODEC_INFO::CODEC_INFO()
{

    //Default constructor - reset values
    Reset();

}

//---------------------------------------------------------------------------------------

CODEC_INFO::CODEC_INFO(wxString &from)
{

    //Constructor that parses values from a string
    Reset();
    Parse(from);

}

//---------------------------------------------------------------------------------------

CODEC_INFO::~CODEC_INFO()
{

    //Destructor that releases memory used
    if (next) delete next;
    next = nullptr;

}

//---------------------------------------------------------------------------------------

void CODEC_INFO::Append(wxString &from)
{

    //Append the info in the string to the list of CODEC_INFO's

    if (from.Find(codec + "=") == 0) Parse(from); //No doublets set values to this
    else if (next) next->Append(from); //Append to next
    else next = new CODEC_INFO(from); //Set as next

}

//---------------------------------------------------------------------------------------

void CODEC_INFO::CopyTo(CODEC_INFO &dst)
{

    //Copy the values of this structure to the argument
    dst.friendly = friendly;
    dst.codec = codec;
    dst.min_qscale = min_qscale;
    dst.max_qscale = max_qscale;
    dst.min_crf = min_crf;
    dst.max_crf = max_crf;
    dst.crf_float = crf_float;
    dst.qscale_float = qscale_float;

}

//---------------------------------------------------------------------------------------

CODEC_INFO* CODEC_INFO::Find(const wxString find_codec)
{

    //Find CODEC_INFO - first check this
    if (codec == find_codec) return this;

    //Then check next (which will nest the call until NULL is reached)
    if (next) return next->Find(find_codec);

    //Not found - return NULL
    return nullptr;

}

//---------------------------------------------------------------------------------------

bool CODEC_INFO::GetConstRate(int pct, float *result)
{
    return GetPctValue(pct, min_crf, max_crf, crf_float, result);
}

//---------------------------------------------------------------------------------------

wxString CODEC_INFO::GetConstRateStr(int pct)
{
    return GetPctString(pct, min_crf, max_crf, crf_float);
}

//---------------------------------------------------------------------------------------

bool CODEC_INFO::GetQScale(int pct, float *result)
{
    return GetPctValue(pct, min_qscale, max_qscale, qscale_float, result);
}

//---------------------------------------------------------------------------------------

wxString CODEC_INFO::GetQScaleStr(int pct)
{
    return GetPctString(pct, min_qscale, max_qscale, qscale_float);
}

//---------------------------------------------------------------------------------------

bool CODEC_INFO::IsDefault(wxString *packed)
{

    //Returns true if the info is default (and thus does not need to be saved to config)
    wxString p = ToString();
    if (packed) *packed = p;
    return DEFAULT_CODEC_INFO.Find(p) >= 0;

}

//---------------------------------------------------------------------------------------

void CODEC_INFO::Parse(wxString &from)
{

    //Parse values from a string
    codec = GetToken(from, "=", true);
    friendly = (GetToken(from, ",", true) == STR_YES);
    min_qscale = GetNumericPart(from, qscale_float, min_qscale);
    max_qscale = GetNumericPart(from, qscale_float, max_qscale);
    min_crf = GetNumericPart(from, crf_float, min_crf);
    max_crf = GetNumericPart(from, crf_float, max_crf);

    /*min_qscale = Str2Float(GetToken(from, ",", true), min_qscale);
    max_qscale = Str2Float(GetToken(from, ",", true), max_qscale);
    min_crf = Str2Float(GetToken(from, ",", true), min_crf);
    max_crf = Str2Float(GetToken(from, ",", true), max_crf);*/

}

//---------------------------------------------------------------------------------------

void CODEC_INFO::Reset()
{

    //Reset values to default values
    friendly = true;
    codec = "";
    min_qscale = 0;
    max_qscale = 0;
    min_crf = 0;
    max_crf = 0;
    next = nullptr;
    crf_float = false;
    qscale_float = false;

}

//---------------------------------------------------------------------------------------

#define FFMT ",%gf,%g"
#define IFMT ",%i,%i"

wxString CODEC_INFO::ToString()
{

    //Pack the values to a string
    wxString res = wxString::Format("%s=%s", codec, BOOLSTR(friendly));
    res += qscale_float ? wxString::Format(FFMT, min_qscale, max_qscale) : wxString::Format(IFMT, (int)min_qscale, (int)max_qscale);
    res += crf_float ? wxString::Format(FFMT, min_crf, max_crf) : wxString::Format(IFMT, (int)min_crf, (int)max_crf);
    return res;

    /*return wxString::Format(
        "%s=%s,%.2f,%.2f,%.2f,%.2f",
        codec, BOOLSTR(friendly), min_qscale, max_qscale, min_crf, max_crf
    );*/

}

//---------------------------------------------------------------------------------------

LPCODEC_INFO DeleteCodecInfo(LPCODEC_INFO root, LPCODEC_INFO del)
{
    LPCODEC_INFO ci;
    if (del && (root == del))
    {
       ci = root->next;
       root->next = nullptr;
       delete root;
       root = ci;
    }
    else
    {
        ci = root;
        while (ci && (ci->next != del)) ci = ci->next;
        if (ci)
        {
            ci->next = del->next;
            del->next = nullptr;
            delete del;
        }
    }
    return root;
}

//---------------------------------------------------------------------------------------

LPCODEC_INFO MakeDefaultCodecInfo(LPCODEC_INFO root)
{

    //Create a list of CODEC_INFO's based on DEFAULT_CODEC_INFO above.
    //If "root" is defined it will be reset to the default values

    wxString dci = DEFAULT_CODEC_INFO, cur;

    while (dci.Len() > 0)
    {

        //Get a line
        cur = GetToken(dci, "\n", true);

        //Append or create the structure
        if (root) root->Append(cur);
        else root = new CODEC_INFO(cur);

    }

    //Return the list
    return root;

}
