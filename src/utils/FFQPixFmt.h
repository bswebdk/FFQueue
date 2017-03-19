/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPixFmt.h                                                     *
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

#ifndef FFQPIXFMT_H
#define FFQPIXFMT_H

#include <wx/string.h>

//Structure to hold a pixel format
typedef struct PIXEL_FORMAT
{

    int comp; //Components
    int bits; //Bits per pixel
    bool in, out; //Input and/or output format
    bool hwaccl; //Hardware accelerated
    wxString name; //The name of the pixel format

    PIXEL_FORMAT(wxString pix_fmt);
    ~PIXEL_FORMAT();

    PIXEL_FORMAT* Find(wxString fmt);
    unsigned int  GetCount();
    PIXEL_FORMAT* GetNext();
    bool          Is10BitQuantizer();

private:

    PIXEL_FORMAT* lpNext;


} PIXEL_FORMAT, *LPPIXEL_FORMAT;

//Helper function used to make a list of PIXEL_FORMAT
LPPIXEL_FORMAT ParsePixelFormats(wxString pix_fmt);

#endif // FFQPIXFMT_H
