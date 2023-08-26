/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConsole.h                                                    *
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

#ifndef FFQCONSOLE_H
#define FFQCONSOLE_H

#include "FFQTimeValue.h"
#include <wx/textctrl.h>
#include <wx/string.h>

//Maximum lines of consecutive statistics
#define MAX_STATISTIC_LINES 5

//---------------------------------------------------------------------------------------

//The message types that are recognized by the console handler
enum FF_MSG_TYPE { mtUNDEF, mtSTATS, mtCLIPPING, mtMSG_REPEAT, mtLOW_CONTRAST };

//---------------------------------------------------------------------------------------

//Const array of the colors above for easier search
/*const uint32_t CONSOLE_COLORS[ CONSOLE_COLOR_COUNT ] = {
    COLOR_GRAY, COLOR_BLUE, COLOR_RED, COLOR_BLACK, COLOR_GREEN, COLOR_ORANGE
};*/

//---------------------------------------------------------------------------------------

#ifdef DEBUG
const wxString FF_MSG_NAME[] = {
    "UNDEF", "STATS", "CLIPPING", "MSG_REPEAT", "LOW_CONTRAST"
};
#endif // DEBUG

//---------------------------------------------------------------------------------------

//The console handler interface
class FFQConsole
{
    public:

        FFQConsole();
        ~FFQConsole();

        void AppendBlankLine(int count = 1);
        FF_MSG_TYPE AppendFFOutput(wxString &Output, bool IsStdOut, bool ClearCtrl = false);
        void AppendLine(const wxString &Line, uint32_t Color, bool ClearCtrl = false);
        void AppendStatistics(const wxString &Prog);
        void AppendWithTime(const wxString &Line, uint32_t Color, bool ClearCtrl = false);
        void Clear(bool Ctrl = true);
        void FlushMessages(bool Finalize = true);
        unsigned int GetEncodedFrames();
        TIME_VALUE GetEncodedTime();
        uint64_t GetStatisticsTotal();
        wxTextCtrl* GetTextCtrl();
        void ReplaceLine(int LineNo, const wxString &NewLine);
        bool SaveAsHtml(const wxString &FileName);
        void SetTextCtrl(wxTextCtrl* ctrl);

        //Access to the singleton reference
        static FFQConsole* Get() { return FFQConsole::m_Instance; }

        //Static helper functions (used to be part of the horror named FFQueueUtils.*)
        static bool IsLineClipping(const wxString &Line);
        static bool IsLineMessageRepeat(wxString Line, unsigned int &Repetitions);
        static bool IsLineStatistics(wxString Line, TIME_VALUE &Time, unsigned int &Frame);
        static bool IsLineVidStabLowContrast(wxString Line, unsigned int &Frame);

    protected:

    private:

        static FFQConsole* m_Instance; //Used as a singleton for access outside FFQMain

        wxTextCtrl *m_Ctrl; //Control used as console
        int m_StatsCount; //Number of consecutive status lines
        uint64_t m_StatsTotal; //Total number of status lines
        uint64_t m_ClippingsTotal; //Total number of clipping warnings received
        TIME_VALUE m_EncTime; //The time that has been processed
        unsigned int m_EncFrames; //Number of frames processed
        unsigned int m_LowContrastFirst; //First frame reported with low contrast from vidstabdetect
        unsigned int m_LowContrastLast; //Last frame of above
        FF_MSG_TYPE m_LastMsgType; //The last message type added

        void BringToFront();
        bool ClearStatsAnim();
        void ConsoleUpdated();

};

#endif // FFQCONSOLE_H
