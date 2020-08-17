/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQStreamMapping.h                                              *
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

#ifndef FFQSTREAMMAPPING_H
#define FFQSTREAMMAPPING_H

#include <wx/string.h>

//Char used to separate multiple stream mappings
const wxUniChar STREAM_MAPPING_SEPERATOR = '|';

//The STREAM_MAPPINT structure
typedef struct STREAM_MAPPING
{

    bool checked; //True if the stream is "checked" in the job editor and thus must be included in the output file
    int file_id, //The index of the file used with -map (starting from 1, must be decremented when used in the ffmpeg command)
        stream_id; //The index of the stream in the file (starting from 0 - unlike file_id)
    wxString codec_type, //The codec type of the stream (video, audio or subtitle)
             preset_list; //A list of space-separated preset id's to be used with this stream

    STREAM_MAPPING();
    STREAM_MAPPING(bool chk, int f_id, int s_id, wxString c_type, wxString pst_lst);
    STREAM_MAPPING(wxString &from);

    bool Parse(wxString &from);
    wxString ToString(wxString long_codec_name = "");

} STREAM_MAPPING, *LPSTREAM_MAPPING;

#endif // FFQSTREAMMAPPING_H
