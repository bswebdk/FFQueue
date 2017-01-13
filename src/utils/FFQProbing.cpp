/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQProbing.cpp                                                  *
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

#include "FFQProbing.h"
#include "../utils/FFQLang.h"
#include "FFQBuildCmd.h"
#include "FFQMisc.h"
#include "FFQConsole.h"
#include "FFQConfig.h"

//Probe constants
const wxString PROBE_BIT_RATE = "bit_rate";
const wxString PROBE_DURATION = "duration";
const wxString PROBE_FILENAME = "filename";
const wxString PROBE_FILESIZE = "file_size";
const wxString PROBE_FORMAT_NAME = "format_name";
const wxString PROBE_FORMAT_LONG_NAME = "format_long_name";
const wxString PROBE_NB_STREAMS = "nb_streams";
const wxString PROBE_START_TIME = "start_time";
const wxString PROBE_CODEC_NAME = "codec_name";
const wxString PROBE_CODEC_LONG_NAME = "codec_long_name";
const wxString PROBE_CODEC_TYPE = "codec_type";
const wxString PROBE_WIDTH = "width";
const wxString PROBE_HEIGHT = "height";
const wxString PROBE_INDEX = "index";
const wxString PROBE_PIXEL_FMT = "pixel_fmt";
const wxString PROBE_FORMAT = "[format]";

//---------------------------------------------------------------------------------------

FFPROBE_STREAM_INFO::FFPROBE_STREAM_INFO()
{

    //Default constructor
    index = 0; //Stream index
    width = 0; //Video width
    height = 0; //Video height
    codec_name = ""; //Codec name ("h264", "mpeg4" and so on)
    codec_long_name = ""; //Full codec name ("H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10" or alike)
    codec_type = ""; //Codec type (audio, video or subtitle)
    duration = ""; //Duration of stream (Could be changed to a TIME_VALUE field)
    pixel_fmt = ""; //The pixel format used in video
    bit_rate = ""; //The bit rate of the stream
    items = NULL; //Complete set of NVP's for display when stream is double-clicked in the job editor

}

//---------------------------------------------------------------------------------------

FFPROBE_STREAM_INFO::FFPROBE_STREAM_INFO(FFPROBE_STREAM_INFO &copy_from)
{

    //Copy constructor - copies another stream info
    index = copy_from.index;
    width = copy_from.width;
    height = copy_from.height;
    codec_name = copy_from.codec_name;
    codec_long_name = copy_from.codec_long_name;
    codec_type = copy_from.codec_type;
    duration = copy_from.duration;
    pixel_fmt = copy_from.pixel_fmt;
    bit_rate = copy_from.bit_rate;

    items = NULL; //Must be set to NULL first
    LPFFQ_NVP_LIST ptr = copy_from.items;
    while (ptr)
    {

        NVPListAppend(items, ptr->name, ptr->value);
        ptr = ptr->GetNext();

    }

}

//---------------------------------------------------------------------------------------

FFPROBE_STREAM_INFO::~FFPROBE_STREAM_INFO()
{

    //Destructor, release any nvp items
    if (items) delete items;
    items = NULL;

}

//---------------------------------------------------------------------------------------

bool FFPROBE_STREAM_INFO::IsTeleText()
{

    //Checks if the stream info is for teletext (and not subtitles)
    return codec_long_name.Lower().Find("teletext") >= 0;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFPROBE_FILE_FORMAT::FFPROBE_FILE_FORMAT()
{

    //Default constructor
    filename = ""; //Full path and name of the file
    nb_streams = 0; //Number of streams in the file
    format_name = ""; //Short name of the file format ("mpegts" or alike)
    format_long_name = ""; //Full name of the file format ("MPEG-TS (MPEG-2 Transport Stream)" or alike)
    start_time = ""; //If available it is (??) the time the file was recorded / created
    duration = ""; //The duration of the file (Could be changed to a TIME_VALUE field)
    file_size = ""; //The size of the file in (10.23 Gibyte or alike)
    bit_rate = ""; //The total bitrate (aud/vid/subs combined; 11.488038 Mbit/s or alike)
    items = NULL; //Complete list of NVP's returned by ffprobe

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFProbeInfoParser::FFProbeInfoParser()
{

     //Default constructor, clear values
     m_Process = NULL;
     Clear();

}

//---------------------------------------------------------------------------------------

FFProbeInfoParser::~FFProbeInfoParser()
{

    //Release any memory used
    if (m_Process) delete m_Process;
    m_Process = NULL;
    Clear();

}

//---------------------------------------------------------------------------------------

LPFFPROBE_STREAM_INFO FFProbeInfoParser::FindStreamByCodecType(wxString codec_type)
{

    for (std::vector<LPFFPROBE_STREAM_INFO>::const_iterator ite = m_StreamList.begin(); ite != m_StreamList.end(); ite++)
    {

        LPFFPROBE_STREAM_INFO si = *ite;
        if (si->codec_type == codec_type) return si;

    }

    return NULL;

}

//---------------------------------------------------------------------------------------

LPFFPROBE_FILE_FORMAT FFProbeInfoParser::GetFormat()
{

    //Check if valid and return pointer to the file format
    CheckValid();
    return &m_Format;

}

//---------------------------------------------------------------------------------------

LPFFPROBE_STREAM_INFO FFProbeInfoParser::GetStreamInfo(size_t StreamIndex)
{

    //Check if valid
    CheckValid();

    //Verify stream index
    if (StreamIndex >= m_StreamList.size())
        ThrowError(FFQSF(SID_STREAM_INDEX_OUT_OF_BOUNDS, StreamIndex));

    //Return stream info
    return (LPFFPROBE_STREAM_INFO)m_StreamList[StreamIndex];

}

//---------------------------------------------------------------------------------------

size_t FFProbeInfoParser::GetStreamCount()
{

    //Return the number of streams held
    return m_StreamList.size();

}

//---------------------------------------------------------------------------------------

#define shortest_time(T1, T2) (((T2) < (T1)) && ((T2) > 0) ? (T2) : (T1))

//---------------------------------------------------------------------------------------

bool FFProbeInfoParser::GetDuration(TIME_VALUE &Time)
{

    //Time code of "[format]" may be other than for the streams.
    //This function finds the shortest time value since this is
    //usually the correct one

    wxString vid = "", aud = "";

    for (std::vector<LPFFPROBE_STREAM_INFO>::const_iterator ite = m_StreamList.begin(); ite != m_StreamList.end(); ite++)
    {

        LPFFPROBE_STREAM_INFO si = *ite;
        if (si->codec_type == CODEC_TYPE_VIDEO) vid = si->duration; //Video duration
        else if (si->codec_type == CODEC_TYPE_AUDIO) aud = si->duration; //Audio duration

        //ALERT: This will get duration of the LAST stream only!
        //If multiple audio streams are available only the last is used

    }

    //Get duration of the file (from format)
    Time = TIME_VALUE(m_Format.duration);

    //Get duration of video
    TIME_VALUE tmp = TIME_VALUE(vid);

    //If video duration is valid - get which ever is shortest
    if (tmp.IsValid()) Time = shortest_time(Time.ToMilliseconds(), tmp.ToMilliseconds());

    //Get duration of audio
    tmp = TIME_VALUE(aud);

    //If audio duration is valid - get which ever is shortest
    if (tmp.IsValid()) Time = shortest_time(Time.ToMilliseconds(), tmp.ToMilliseconds());

    //If time is not valid return 0
    if (!Time.IsValid()) Time = TIME_VALUE(0);

    //Return wheter or not we have a success
    return !Time.IsUndefined();

}

//---------------------------------------------------------------------------------------

bool FFProbeInfoParser::GetVideoDimension(unsigned int &Width, unsigned int &Height)
{

    //Return the dimension of the video stream - if available
    LPFFPROBE_STREAM_INFO si = FindStreamByCodecType(CODEC_TYPE_VIDEO);
    if (si)
    {
        //Success!
        Width = si->width;
        Height = si->height;
        return true;
    }

    //Bummer..
    Width = 0;
    Height = 0;
    return false;

}

//---------------------------------------------------------------------------------------

bool FFProbeInfoParser::GetVideoRate(double &rate)
{

    //Get the frame rate for the first video stream
    rate = 0;
    LPFFPROBE_STREAM_INFO si = FindStreamByCodecType(CODEC_TYPE_VIDEO);
    if (si)
    {

        LPFFQ_NVP_LIST vr = si->items->Find("r_frame_rate"); //Real frame rate
        if (vr == NULL) vr = si->items->Find("avg_frame_rate"); //Average frame rate

        if (vr)
        {

            wxString r = vr->value.Lower();

            //Check for a numeric value
            rate = Str2Float(r, 0);
            if (rate > 0) return true;

            //Check for a nominator / denominator value
            if (r.Find('/') >= 0)
            {

                double n = Str2Long(r.Before('/'), 0);
                double d = Str2Long(r.After('/'), 0);
                if ((n <= 0) || (d <= 0)) return false;
                rate = n / d;
                return true;

            }

            //Check for a abbreviation
            if ((r == "ntsc") || (r == "qntsc") || (r == "sntsc")) rate = 30000.0 / 1001.0;
            else if ((r == "pal") || (r == "qpal") || (r == "spal")) rate = 25.0;
            else if (r == "film") rate = 24.0;
            else if (r == "ntsc-film") rate = 24000.0 / 1001.0;

        }

    }

    return rate > 0;

}

//---------------------------------------------------------------------------------------

long FFProbeInfoParser::IndexOf(LPFFPROBE_STREAM_INFO StreamInfo)
{

    //Returns the index of the stream info pointer
    for (size_t i = 0; i < GetStreamCount(); i++) if (GetStreamInfo(i) == StreamInfo) return i;

    //Not found, return -1
    return -1;

}

//---------------------------------------------------------------------------------------

bool FFProbeInfoParser::RunFFProbe(wxString ForFile, wxString *ErrorMsg, bool ErrorToConsole)
{

    //Runs ffprobe on a specific file

    //Create process if not already created
    if (m_Process == NULL) m_Process = new FFQProcess();

    bool ok;
    wxString err_msg;

    try
    {

        //Run ffprobe on the file
        m_Process->FFProbe(ForFile);

        //Parse the returned output and throw error if not successful
        ok = SetProbeOutput(m_Process->GetProcessOutput(false, true));

        //Set error message
        if (!ok) err_msg = FFQS(SID_FFPROBE_BAD_OUTPUT);

    }
    catch (std::exception &err)
    {

        //Set error message and result
        ok = false;
        err_msg = err.what();

    }

    //Return error message if required
    if (ErrorMsg) *ErrorMsg = err_msg;

    //Report error to console if required
    if ((!ok) && ErrorToConsole) FFQConsole::Get()->AppendLine(FFQSF(SID_LOG_FFPROBE_ERROR, ForFile, err_msg), COLOR_RED);

    //Return result
    return ok;

}

//---------------------------------------------------------------------------------------

bool FFProbeInfoParser::SetProbeOutput(wxString ProbeOutput)
{

    //Parses output from ffprobe or avprobe

    //Clear first
    Clear();

    if (FFQCFG()->use_libav) ParseAVProbe(ProbeOutput);
    else ParseFFProbe(ProbeOutput);

    //Check if parsing was a success and the content is valid
    m_IsValid = (m_Format.format_name != "") && (GetStreamCount() > 0);

    //Return if succeeded
    return m_IsValid;

}

//---------------------------------------------------------------------------------------

void FFProbeInfoParser::CheckValid()
{

    //Throws an exception if attempting to access invalid info
    if (!m_IsValid) ThrowError(FFQS(SID_PROBE_INFO_IS_INVALID));

}

//---------------------------------------------------------------------------------------

void FFProbeInfoParser::Clear()
{

    //Clear the info held by the parser
    m_IsValid = false; //Invalidate


    //Format
    if (m_Format.items) delete m_Format.items;
    m_Format = FFPROBE_FILE_FORMAT();
    /*m_Format.bit_rate = "";
    m_Format.duration = "";
    m_Format.filename = "";
    m_Format.file_size = "";
    m_Format.format_long_name = "";
    m_Format.format_name = "";
    m_Format.items = NULL;
    m_Format.nb_streams = 0;
    m_Format.start_time = "";*/

    //Stream info list
    for (std::vector<LPFFPROBE_STREAM_INFO>::const_iterator ite = m_StreamList.begin(); ite != m_StreamList.end(); ite++)
        delete (LPFFPROBE_STREAM_INFO)*ite;
    m_StreamList.clear();

}

//---------------------------------------------------------------------------------------

void FFProbeInfoParser::ParseAVProbe(wxString &pout)
{

    //Parses output from avprobe

    //Handy variables
    wxString cl = "", ll, ln, lv;
    bool fmt = false, tags = false;
    LPFFPROBE_STREAM_INFO si = NULL;

    while (pout.Len() > 0)
    {

        cl = StrTrim(GetLine(pout, true)); //Actual line
        ll = cl.Lower(); //Lowercase line

        if ((ll.Len() > 0) && (ll.at(0) != '#')) //Skip blanks lines and comments
        {

            if (ll.at(0) == '[')
            {

                //Test if it is a tags section
                tags = ll.AfterLast('.') == "tags]";

                if (!tags)
                {

                    //Not a tags section
                    if (si)
                    {

                        //Add current stream info and set to NULL
                        m_StreamList.push_back(si);
                        si = NULL;

                    }

                    //Check if this is a format section
                    fmt = (ll == PROBE_FORMAT);

                    //If not format, check stream section
                    if ((!fmt) && (ll.BeforeFirst('.') == "[streams")) si = new FFPROBE_STREAM_INFO();

                }

            }

            else
            {

                //This is a name=value line
                ln = ll.BeforeFirst('='); //Lowercase name
                lv = Unescape(cl.AfterFirst('=')); //Actual value

                if (tags) ln = "tag:" + ln; //Prepend tag

                if (fmt)
                {

                    //Add to formats NVP list
                    NVPListAppend(m_Format.items, ln, lv);

                    //Set value to format
                    if (ln == PROBE_BIT_RATE) m_Format.bit_rate = lv;
                    else if (ln == PROBE_DURATION) m_Format.duration = lv;
                    else if (ln == PROBE_FILENAME) m_Format.filename = lv;
                    else if (ln == PROBE_FILESIZE) m_Format.file_size = lv;
                    else if (ln == PROBE_FORMAT_LONG_NAME) m_Format.format_long_name = lv;
                    else if (ln == PROBE_FORMAT_NAME) m_Format.format_name = lv;
                    else if (ln == PROBE_NB_STREAMS) m_Format.nb_streams = Str2Long(lv, -1);
                    else if (ln == PROBE_START_TIME) m_Format.start_time = lv;

                }

                else if (si)
                {

                    //Append to stream info's NVP list
                    NVPListAppend(si->items, ln, lv);

                    //Set value to stream info
                    if (ln == PROBE_BIT_RATE) si->bit_rate = lv;
                    else if (ln == PROBE_CODEC_LONG_NAME) si->codec_long_name = lv;
                    else if (ln == PROBE_CODEC_NAME) si->codec_name = lv;
                    else if (ln == PROBE_CODEC_TYPE) si->codec_type = lv;
                    else if (ln == PROBE_DURATION) si->duration = lv;
                    else if (ln == PROBE_HEIGHT) si->height = Str2Long(lv, -1);
                    else if (ln == PROBE_INDEX) si->index = Str2Long(lv, -1);
                    else if (ln == PROBE_PIXEL_FMT) si->pixel_fmt = lv;
                    else if (ln == PROBE_WIDTH) si->width = Str2Long(lv, -1);

                }

            }

        }

    }

    //If any stream info is open, add it
    if (si) m_StreamList.push_back(si);

}

//---------------------------------------------------------------------------------------

void FFProbeInfoParser::ParseFFProbe(wxString &pout)
{

    //Parses output from ffprobe

    //Handy variables
    wxString cl = "", ll, ln, lv;
    bool fmt = false;
    LPFFPROBE_STREAM_INFO si = NULL;

    while (pout.Len() > 0)
    {

        cl = GetLine(pout, true); //Actual line
        ll = cl.Lower(); //Lowercase line

        if (fmt)
        {

            //Values added to format
            if (ll == "[/format]") fmt = false; //Format ended
            else
            {

                ln = ll.BeforeFirst('='); //Lowercase name
                lv = cl.AfterFirst('='); //Actual value

                //Add to formats NVP list
                NVPListAppend(m_Format.items, ln, lv);

                //Set value to format
                if (ln == PROBE_BIT_RATE) m_Format.bit_rate = lv;
                else if (ln == PROBE_DURATION) m_Format.duration = lv;
                else if (ln == PROBE_FILENAME) m_Format.filename = lv;
                else if (ln == PROBE_FILESIZE) m_Format.file_size = lv;
                else if (ln == PROBE_FORMAT_LONG_NAME) m_Format.format_long_name = lv;
                else if (ln == PROBE_FORMAT_NAME) m_Format.format_name = lv;
                else if (ln == PROBE_NB_STREAMS) m_Format.nb_streams = Str2Long(lv, -1);
                else if (ln == PROBE_START_TIME) m_Format.start_time = lv;

            }

        }

        else if ((si == NULL) && (ll == PROBE_FORMAT)) fmt = true; //Format tag has started

        else if ((si != NULL) && (ll == "[/stream]"))
        {

            //Stream tag has closed - add stream info to list
            m_StreamList.push_back(si);

            //Set stream info to null to indicate that no stream tag is open
            si = NULL;

        }

        else if ((si == NULL) && (ll == "[stream]"))
        {

            //Stream tag opened - create new stream info
            si = new FFPROBE_STREAM_INFO();

        }

        else if (si != NULL) //Stream tag is open
        {

            //Split into name and value
            ln = ll.BeforeFirst('=');
            lv = cl.AfterFirst('=');

            //Append to stream info's NVP list
            NVPListAppend(si->items, ln, lv);

            //Set value to stream info
            if (ln == PROBE_BIT_RATE) si->bit_rate = lv;
            else if (ln == PROBE_CODEC_LONG_NAME) si->codec_long_name = lv;
            else if (ln == PROBE_CODEC_NAME) si->codec_name = lv;
            else if (ln == PROBE_CODEC_TYPE) si->codec_type = lv;
            else if (ln == PROBE_DURATION) si->duration = lv;
            else if (ln == PROBE_HEIGHT) si->height = Str2Long(lv, -1);
            else if (ln == PROBE_INDEX) si->index = Str2Long(lv, -1);
            else if (ln == PROBE_PIXEL_FMT) si->pixel_fmt = lv;
            else if (ln == PROBE_WIDTH) si->width = Str2Long(lv, -1);

        }

    }

    if (si != NULL)
    {

        //A stream tag has not been closed properly, discard it
        delete si;

    }

    if (fmt)
    {

        //Format tag has not been closed properly, discard it
        if (m_Format.items) delete m_Format.items;
        m_Format = FFPROBE_FILE_FORMAT();

    }

}
