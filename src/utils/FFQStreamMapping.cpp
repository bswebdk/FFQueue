/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQStreamMapping.cpp                                            *
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

#include "FFQStreamMapping.h"
#include "FFQConfig.h"
#include "FFQMisc.h"
#include "FFQConst.h"
#include "../utils/FFQLang.h"

STREAM_MAPPING::STREAM_MAPPING()
{

    //Default constructor - set default values
    checked = true;
    file_id = -1;
    stream_id = -1;
    codec_type = "";
    preset_list = "";

}

//---------------------------------------------------------------------------------------

STREAM_MAPPING::STREAM_MAPPING(bool chk, int f_id, int s_id, wxString c_type, wxString pst_lst)
{

    //Constructor that defines values
    checked = chk;
    file_id = f_id;
    stream_id = s_id;
    codec_type = c_type;
    preset_list = pst_lst;

}

//---------------------------------------------------------------------------------------

STREAM_MAPPING::STREAM_MAPPING(wxString &from)
{

    //Constructor that parses values from a string and throws an error if values are invalid
    if (!Parse(from)) ThrowError(FFQS(SID_INVALID_STREAM_MAP_ENDODING));

}

//---------------------------------------------------------------------------------------

bool STREAM_MAPPING::Parse(wxString &from)
{

    //Parse values from a string

    wxString chk;

    if (from.StartsWith(HASH))
    {

        //The format used in the job editor: #file_id:stream_id codec_type { codec_long_name }
        chk = STR_YES; //Always checked by default
        from = from.AfterFirst(HASH);
        file_id = Str2Long(GetToken(from, COLON), -1);
        stream_id = Str2Long(GetToken(from, SPACE), -1);
        codec_type = GetToken(from, SPACE, false);
        preset_list = "";

    }

    else //checked,file_id,stream_id,codec_type\t....
    {

        //The format used in FFQ_JOB::stream_map: checked,file_id,stream_id,codec_type[,preset][STREAM_MAPPING_SEPERATOR | end of string]
        chk = GetToken(from, COMMA);
        file_id = Str2Long(GetToken(from, COMMA), -1);
        stream_id = Str2Long(GetToken(from, COMMA), -1);
        codec_type = GetToken(from, STREAM_MAPPING_SEPERATOR);
        if (codec_type.Find(COMMA) > 0)
        {
            preset_list = codec_type.After(COMMA);
            codec_type = codec_type.Before(COMMA);
        }

    }

    //Validate the values and return the status of the validation
    if ((chk != STR_YES) && (chk != STR_NO)) return false;
    checked = (chk == STR_YES);
    return (file_id >= 0) && (stream_id >= 0) && (codec_type.Len() > 0);

}

//---------------------------------------------------------------------------------------

wxString STREAM_MAPPING::ToString(wxString long_codec_name)
{

    //Pack the values of the mapping to a string
    wxString res;

    if (long_codec_name.Len() == 0) //Pack for use in FFQ_JOB::stream_map
        //res << BOOLSTR(checked) << "," << file_id << "," << stream_id << "," << codec_type << "," << preset_id;
        res.Printf("%s,%d,%d,%s,%s", BOOLSTR(checked), file_id, stream_id, codec_type, preset_list);

    else //Pack for visual display (in the job editor etc.)
        //res << "#" << file_id << ":" << stream_id << " " << codec_type << " {" << long_codec_name << "}";
        res.Printf("#%d:%d %s {%s}", file_id, stream_id, codec_type, long_codec_name);

    //Return the result
    return res;

}
