/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQProbing.h                                                    *
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

#ifndef FFQPROBING_H
#define FFQPROBING_H

#include <vector>
#include <wx/string.h>
#include "FFQNvpList.h"
#include "FFQTimeValue.h"
#include "FFQProcess.h"

//---------------------------------------------------------------------------------------
//Stream info
typedef struct FFPROBE_STREAM_INFO
{

    int index, width, height;
    wxString codec_name, codec_long_name, codec_type;
    wxString duration, pixel_fmt, bit_rate;
    LPFFQ_NVP_LIST items;

    FFPROBE_STREAM_INFO();
    FFPROBE_STREAM_INFO(FFPROBE_STREAM_INFO &copy_from);
    ~FFPROBE_STREAM_INFO();
    bool IsTeleText();

} FFPROBE_STREAM_INFO, *LPFFPROBE_STREAM_INFO;


//---------------------------------------------------------------------------------------
//File format info
typedef struct FFPROBE_FILE_FORMAT
{

    wxString filename;
    int nb_streams;
    wxString format_name, format_long_name;
    wxString start_time, duration;
    wxString file_size, bit_rate;
    LPFFQ_NVP_LIST items;

    FFPROBE_FILE_FORMAT();

} FFPROBE_FILE_FORMAT, *LPFFPROBE_FILE_FORMAT;


//---------------------------------------------------------------------------------------
//FFProbeInfoParser
class FFProbeInfoParser
{

    public:

        FFProbeInfoParser();
        ~FFProbeInfoParser();
        LPFFPROBE_STREAM_INFO FindStreamByCodecType(wxString codec_type);
        LPFFPROBE_FILE_FORMAT GetFormat();
        LPFFPROBE_STREAM_INFO GetStreamInfo(size_t StreamIndex);
        size_t GetStreamCount();
        bool GetDuration(TIME_VALUE &Time);
        bool GetVideoDimension(unsigned int &Width, unsigned int &Height);
        bool GetVideoRate(double &rate);
        long IndexOf(LPFFPROBE_STREAM_INFO StreamInfo);
        bool RunFFProbe(wxString ForFile, wxString *ErrorMsg = NULL, bool ErrorToConsole = false);
        bool SetProbeOutput(wxString ProbeOutput);

    private:

        FFQProcess *m_Process;
        FFPROBE_FILE_FORMAT m_Format;
        std::vector<LPFFPROBE_STREAM_INFO> m_StreamList;
        bool m_IsValid;

        void CheckValid();
        void Clear();
        void ParseAVProbe(wxString &pout);
        void ParseFFProbe(wxString &pout);

};

#endif // FFQPROBING_H
