/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQFilter.cpp                                                   *
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

#include "FFQFilter.h"
#include "FFQMisc.h"
#include <wx/filefn.h>

//---------------------------------------------------------------------------------------

//extern const's

const wxString FILTER_SEPARATOR = "[|]";

const wxString FILTER_VIDEO_IN = "<VIDIN>";
const wxString FILTER_VIDEO_OUT = "<VIDOUT>";
const wxString FILTER_AUDIO_IN = "<AUDIN>";
const wxString FILTER_AUDIO_OUT = "<AUDOUT>";
const wxString FILTER_SUBTITLE_IN = "<SUBIN>";
const wxString FILTER_SUBTITLE_OUT = "<SUBOUT>";
const wxString FILTER_REQUIRED_IN = "<REQIN>";
const wxString FILTER_UNIQUE_ID = "<UID%u>";

//---------------------------------------------------------------------------------------

const unsigned int FILTER_COUNT = 46;

const wxString FILTER_NAMES[FILTER_COUNT] = {

    "overlay", "yadif,kerndeint", "crop", "scale", "subtitle,overlay",
    "delogo,removelogo", "overlay", "hflip,vflip", "dejudder", "deshake",
    "boxblur", "colorbalance", "pad", "rotate", "unsharp",
    "vignette", "volume,bass,treble", "aecho", "aphaser", "flanger",
    "highpass", "lowpass", "bandpass", "bandreject", "equalizer",
    "colormatrix", "curves", "channelmap",
    "-", "aresample", "earwax", "negate", "elbg", "adelay", "atempo",
    "decimate", "interlace,tinterlace", "framestep", "hqdn3d", "hue",
    "lenscorrection", "noise", "swapuv", "perspective", "transpose",
    "dynaudnorm"

};

//---------------------------------------------------------------------------------------

const unsigned int FILTER_ORDER_COUNT = FILTER_COUNT + 1;

const int FILTER_ORDER[FILTER_ORDER_COUNT] = {

    ftBOXBLUR, ftCOLORBALANCE, ftCOLOROVERLAY, ftCURVES, ftCOLORMATRIX, ftCROP, ftDEINTERLACE, ftDEJUDDER,
    ftDESHAKE, ftDECIMATE, ftFLIP, ftFRAMESTEP, ftHUE, ftHQDN3D, ftINTERLACE, ftLENSCORRECTION,
    ftNEGATE, ftNOISE, ftPAD, ftPERSPECTIVE, ftELBG, ftDELOGO, ftROTATE, ftSCALE, ftUNSHARP, ftSWAPUV,
    ftTRANSPOSE, ftVIGNETTE, ftWATERMARK,
    -1, //Separator
    ftBANDPASS, ftBANDREJECT, ftATEMPO, ftCHANNELMAP, ftADELAY, ftECHO, ftEQUALIZER, ftFLANGER,
    ftEARWAX, ftHIGHPASS, ftLOWPASS, ftDYNAUDNORM, ftPHASER, ftRESAMPLE, ftVOLUME_AND_TONE,
    -1, //Separator
    ftCUSTOM

};

//---------------------------------------------------------------------------------------

REQUIRED_FILE::REQUIRED_FILE()
{

    //Default constructor
    Reset();

}

//---------------------------------------------------------------------------------------

REQUIRED_FILE::REQUIRED_FILE(wxString from)
{

    //Parse values from a string
    tag = GetToken(from, ':');
    loop = GetToken(from, ':');
    path = from;
}

//---------------------------------------------------------------------------------------

bool REQUIRED_FILE::IsValid()
{

    //Returns true is the file is valid
    return (path.Len() > 0) && wxFileExists(path);

}

//---------------------------------------------------------------------------------------

void REQUIRED_FILE::Reset()
{

    //Reset values
    path="";
    tag="";
    loop="";

}

//---------------------------------------------------------------------------------------

wxString REQUIRED_FILE::ToString()
{

    //Pack values to a string
    return tag + ":" + loop + ":" + path;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFMPEG_FILTER::FFMPEG_FILTER()
{

    //Default constructor
     Reset();

}

//---------------------------------------------------------------------------------------

FFMPEG_FILTER::FFMPEG_FILTER(wxString fltr)
{

    //Parse values from string

    //Type of the filter
    type = (FILTER_TYPE)Str2Long(GetToken(fltr, FILTER_SEPARATOR));

    //User friendly (readable)
    friendly = GetToken(fltr, FILTER_SEPARATOR);

    //FF-compatible filter
    ff_filter = GetToken(fltr, FILTER_SEPARATOR);

    //Editable values (used by the GUI)
    editable = GetToken(fltr, FILTER_SEPARATOR);

    //File required by the filter
    required_file = REQUIRED_FILE(fltr);

}

//---------------------------------------------------------------------------------------

bool FFMPEG_FILTER::IsAudio()
{

    //Test if filter is an audio filter
    return ff_filter.Find(FILTER_AUDIO_IN) != wxNOT_FOUND;

}

//---------------------------------------------------------------------------------------

bool FFMPEG_FILTER::IsComplex()
{

    //Test if filter is complex
    return required_file.path.Len() > 0;

}

//---------------------------------------------------------------------------------------

bool FFMPEG_FILTER::IsEditable()
{

    //Test if filter is editable (does it have a GUI?)
    return (type != ftEARWAX) && (type != ftSWAPUV);

}

//---------------------------------------------------------------------------------------

bool FFMPEG_FILTER::IsSubtitle()
{

    //Test if filter is for subtitles
    return ff_filter.Find(FILTER_SUBTITLE_IN) != wxNOT_FOUND;

}

//---------------------------------------------------------------------------------------

bool FFMPEG_FILTER::IsVideo()
{

    //Test if the filter is for video
    return ff_filter.Find(FILTER_VIDEO_IN) != wxNOT_FOUND;

}
//---------------------------------------------------------------------------------------

void FFMPEG_FILTER::Reset()
{

    //Reset all filter values
    type = ftUNKNOWN;
    friendly = "";
    ff_filter = "";
    editable = "";
    required_file.Reset();

}
//---------------------------------------------------------------------------------------

wxString FFMPEG_FILTER::ToString()
{

    //Pack filter to a string
    return ToStr(type) + FILTER_SEPARATOR +
           friendly + FILTER_SEPARATOR +
           ff_filter + FILTER_SEPARATOR +
           editable + FILTER_SEPARATOR +
           required_file.ToString();

}
