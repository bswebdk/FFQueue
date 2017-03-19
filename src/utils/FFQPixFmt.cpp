/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPixFmt.cpp                                                   *
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

#include "FFQPixFmt.h"
#include "FFQMisc.h"

PIXEL_FORMAT::PIXEL_FORMAT(wxString pix_fmt)
{

    //Parse values from a line of pixels formats like:
    //IOHPB name comp bits
    //WARNING: NO VALIDATION!!!

    //Get pixel format line
    wxString fmt = GetLine(pix_fmt, true);//, fmts = fmt;

    //Get flag token and parse values
    wxString s = GetToken(fmt, " ", true).Upper();
    in = s.Find('I') == 0;
    out = s.Find('O') == 1;
    hwaccl = s.Find('H') == 2;

    //Get name of pixel format
    fmt.Trim(false);
    name = GetToken(fmt, " ", true);

    //Number of components
    fmt.Trim(false);
    comp = Str2Long(GetToken(fmt, " ", true), -1);

    //Number of bits per pixel
    fmt.Trim(false);
    bits = Str2Long(GetToken(fmt, " ", true), -1);

    //Possible validation
    //if ( ((!in) && (!out)) || (comp <= 0) || (bits <= 0) || (name.Len() == 0) ) ThrowError("Invalid pixel format: " + fmts);

    //Parse next, if available
    pix_fmt.Trim(false);
    lpNext = (pix_fmt.Len() > 0) ? new PIXEL_FORMAT(pix_fmt) : NULL;

}

//---------------------------------------------------------------------------------------

PIXEL_FORMAT::~PIXEL_FORMAT()
{

    //Destroy next in list and nullify
    if (lpNext) delete lpNext;
    lpNext = NULL;

}

//---------------------------------------------------------------------------------------

PIXEL_FORMAT* PIXEL_FORMAT::Find(wxString fmt)
{

    //Find the suggested pixel format
    if (name == fmt) return this;
    if (lpNext) return lpNext->Find(fmt);
    return NULL;

}

//---------------------------------------------------------------------------------------

unsigned int PIXEL_FORMAT::GetCount()
{

    //Returns the number of pixel formats in the list
    return (lpNext == NULL) ? 1 : lpNext->GetCount() + 1;

}

//---------------------------------------------------------------------------------------

PIXEL_FORMAT* PIXEL_FORMAT::GetNext()
{

    //Return next format in list
    return lpNext;

}

//---------------------------------------------------------------------------------------

bool PIXEL_FORMAT::Is10BitQuantizer()
{

    //Return true is pixel format uses 10bit quantizer scale as mentioned under
    //"Choose CRF value" here: https://trac.ffmpeg.org/wiki/Encode/H.264

    return (name.Find("p10") > 0) || (name.Find("p12") > 0) || (name.Find("p14") > 0) || (name.Find("p16") > 0);

}

//---------------------------------------------------------------------------------------

LPPIXEL_FORMAT ParsePixelFormats(wxString pix_fmt)
{

    //This function strips and trims the list of pixel formats
    //to contain only the actual pixel formats which is then
    //parsed by the PIXEL_FORMAT structure's constructor

    while ((pix_fmt.Len() > 0) && (GetLine(pix_fmt, true) != "-----"));
    pix_fmt.Trim().Trim(false);
    return (pix_fmt.Len() == 0) ? NULL : new PIXEL_FORMAT(pix_fmt);

}
