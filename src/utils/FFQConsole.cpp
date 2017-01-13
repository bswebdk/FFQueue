/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConsole.cpp                                                  *
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

#include "FFQConsole.h"
#include "FFQConfig.h"
#include "FFQConst.h"
#include "FFQMisc.h"
#include "../utils/FFQLang.h"
#include <wx/file.h>

//---------------------------------------------------------------------------------------

//Nullify the singleton instance
FFQConsole* FFQConsole::m_Instance = NULL;

//---------------------------------------------------------------------------------------

FFQConsole::FFQConsole(wxTextCtrl *Ctrl)
{

    //Set singleton to the first instance created; Only one
    //instance should be created anyhow
    if (FFQConsole::m_Instance == NULL) FFQConsole::m_Instance = this;

    //Set control and clear everything
    m_Ctrl = Ctrl;
    Clear();

}

//---------------------------------------------------------------------------------------

FFQConsole::~FFQConsole()
{

    //Clear singleton
    if (FFQConsole::m_Instance == this) FFQConsole::m_Instance = NULL;

}

//---------------------------------------------------------------------------------------

void FFQConsole::AppendBlankLine(int count)
{

    //Make sure that the last "count" lines are blank if the control has content

    if (m_Ctrl->GetNumberOfLines() - 1 <= 0) return;

    //Clear statistics animation
    ClearStatsAnim();

    while (count > 0)
    {

        m_Ctrl->AppendText(CRLF);
        count--;

    }

}

//---------------------------------------------------------------------------------------

FF_MSG_TYPE FFQConsole::AppendFFOutput(wxString &Output, bool IsStdOut, bool ClearCtrl)
{

    //Handle a raw line(s) of output from ffmpeg
    size_t val;

    wxString all = StrTrim(Output);
    while (all.Len() > 0)
    {

        Output = GetLine(all, true);

        if ( IsStdOut )
        {

            //Stdout is always black; nothing comes from there anyway
            AppendLine(Output, COLOR_BLACK);
            return mtUNDEF;

        }

        else if ( FFQConsole::IsLineClipping(Output) )
        {

            //Audio clipping performed
            m_ClippingsTotal++;
            m_LastMsgType = mtCLIPPING;

        }

        else if ( (m_LastMsgType == mtCLIPPING) && FFQConsole::IsLineMessageRepeat(Output, val) )
        {

            //Audio clipping warning repeated
            m_ClippingsTotal += val;
            m_LastMsgType = mtMSG_REPEAT;

        }

        else if ( FFQConsole::IsLineStatistics(Output, m_EncTime, m_EncFrames) )
        {

            //Statistics
            AppendStatistics(Output);
            m_LastMsgType = mtSTATS;

        }

        else if ( FFQConsole::IsLineVidStabLowContrast(Output, val)  )
        {

            //Low contrast reported from vidstabdetect.
            //Adjust the proper variable and message type
            if (m_LastMsgType == mtLOW_CONTRAST) m_LowContrastLast = val;
            else
            {

                m_LowContrastFirst = val;
                m_LastMsgType = mtLOW_CONTRAST;

            }

        }

        else
        {

            //Add a normal line of output
            AppendLine(Output, COLOR_BLUE);
            m_LastMsgType = mtUNDEF;

        }

    }

    //Return the last type of message processed
    return m_LastMsgType;

}

//---------------------------------------------------------------------------------------

void FFQConsole::AppendLine(const wxString &Line, unsigned int Color, bool ClearCtrl)
{

    //Reset whenever non-progress is added
    m_StatsCount = 0;

    //Freeze
    m_Ctrl->Freeze();

    //Clear statistics animation
    ClearStatsAnim();

    //Flush any queued messages
    FlushMessages(false);

    //Optional clear
    if (ClearCtrl) Clear();

    //Style
    wxTextPos p = m_Ctrl->GetLastPosition();
    m_Ctrl->SetSelection(p, p);
    m_Ctrl->SetDefaultStyle(wxTextAttr(Color));

    //Append
    m_Ctrl->AppendText(Line + CRLF);

    //Update selection
    ConsoleUpdated();

    //Thaw
    m_Ctrl->Thaw();

}

//---------------------------------------------------------------------------------------

void FFQConsole::AppendStatistics(const wxString &Stats)
{

    //Append one line of statistics, incrementint the counter and
    //moving lines as needed

    //Increment total number of progress lines received
    m_StatsTotal++;

    //Freeze
    m_Ctrl->Freeze();

    //Flush any queued messages
    FlushMessages(false);

    //Style
    m_Ctrl->SetDefaultStyle(wxTextAttr(COLOR_GREEN));

    //Index of last line, zero based + empty last line = -2
    int last = m_Ctrl->GetNumberOfLines() - 2;

    //Set to true when animation is being done
    bool anim = false;

    //Check if last line equals the new line
    if (last >= 0)
    {

        //Get last line
        wxString s = m_Ctrl->GetLineText(last);

        //Comparison length
        int len = Stats.Len();

        if (Stats.compare(0, len, s, 0, len) == 0)
        {

            //Lines are equal - run character animation -\/
            len = s.Len() - 1;
            wxUniChar lc = s.GetChar(len);
            if (lc == '-') s.SetChar(len, '\\');
            else if (lc == '\\') s.SetChar(len, '/');
            else if (lc == '/') s.SetChar(len, '-');
            else s += " -";
            ReplaceLine(last, s);
            anim = true;

        }
        else
        {

            //Clear statistics animation
            ClearStatsAnim();

            //Check if any animation characters should be removed from last line
            /*len = s.Len() - 1;
            wxUniChar lc = s.GetChar(len);
            if ((lc == '-') || (lc == '\\') || (lc == '/'))
            {

                s.Remove(len - 1, 2);
                ReplaceLine(last, s);

            }*/

        }

    }

    if ((!anim) && (m_StatsCount == MAX_STATISTIC_LINES) && FFQCFG()->limit_statistics)
    {

        //Move lines up
        for (int i = last - MAX_STATISTIC_LINES + 1; i < last; i++)
            ReplaceLine(i, m_Ctrl->GetLineText(i+1));

        //Set last line as new progress
        ReplaceLine(last, Stats);

    }

    else if (!anim)
    {

        //Append line and increase number of progress lines
        m_Ctrl->AppendText(Stats + CRLF);
        m_StatsCount++;

    }

    //Update selection
    ConsoleUpdated();

    //Thaw
    m_Ctrl->Thaw();

}

//---------------------------------------------------------------------------------------

void FFQConsole::Clear(bool Ctrl)
{

    if (Ctrl)
    {
        //Clear text control
        m_Ctrl->Freeze();
        m_Ctrl->Clear();
        m_Ctrl->Thaw();
    }

    //Reset variables
    m_StatsCount = 0;
    m_StatsTotal = 0;
    m_ClippingsTotal = 0;
    m_EncTime = 0;
    m_EncFrames = 0;
    m_LowContrastFirst = 0;
    m_LowContrastLast = 0;
    m_LastMsgType = mtUNDEF;

}

//---------------------------------------------------------------------------------------

void FFQConsole::FlushMessages(bool Finalize)
{

    //Used to force any pending messages to be flushed to the console

    if (Finalize)
    {

        //Messages that are only flushed when a job/command has finished

        //Show amount of clipping warnings received, if any
        if (m_ClippingsTotal > 0) AppendLine(FFQSF(SID_LOG_CLIPPING_WARNING, m_ClippingsTotal), COLOR_RED);

    }

    if (m_LowContrastFirst < m_LowContrastLast)
    {

        //Low contrast warnings must be flushed - make message
        wxString msg = FFQSF(SID_LOG_LOW_CONTRAST_WARNING, m_LowContrastFirst, m_LowContrastLast);

        //Reset BEFORE ADDING to avoid deadlock
        m_LowContrastFirst = 0;
        m_LowContrastLast = 0;

        //Append
        AppendLine(msg, COLOR_ORANGE);

    }

}

//---------------------------------------------------------------------------------------

size_t FFQConsole::GetEncodedFrames()
{

    //Return the frames processed
    return m_EncFrames;

}

//---------------------------------------------------------------------------------------

TIME_VALUE FFQConsole::GetEncodedTime()
{

    //Return the time processed
    return m_EncTime;

}

//---------------------------------------------------------------------------------------

uint64_t FFQConsole::GetStatisticsTotal()
{

    //Return the total amount if statistics received
    return m_StatsTotal;

}

//---------------------------------------------------------------------------------------

void FFQConsole::ReplaceLine(int LineNo, const wxString &NewLine)
{

    //Replace the contents of line number "LineNo" with "NewLine"
    int ofs = m_Ctrl->XYToPosition(0, LineNo), len = m_Ctrl->GetLineLength(LineNo);
    m_Ctrl->Replace(ofs, ofs+len, NewLine);

}

//---------------------------------------------------------------------------------------

bool FFQConsole::SaveAsHtml(const wxString &FileName)
{

    //Save the contents of the console to a FFQ style HTML log

    //Last line is always empty - don't waste time on it
    int line_count = m_Ctrl->GetNumberOfLines() - 1;

    //If control is empty, return false
    if (line_count <= 0) return false;

    //Handy variables
    wxFile* log = NULL;
    bool ok = true; //Will be set to false in case of exceptions

    try
    {

        //Open the log file
        log = new wxFile(FileName, wxFile::write);

        //If open failed we throw up
        if (!log->IsOpened()) ThrowError(FFQS(SID_LOG_FILE_ERROR));

        try
        {

            //Save UTF-8 BOM
            log->Write(&UTF8_BYTE_ORDER_MARK, sizeof(UTF8_BYTE_ORDER_MARK));

            //Create HTML header
            wxString s = "<html>" + CRLF + "<head>" + CRLF + "<style type=\"text/css\">" + CRLF +
                         "body{background-color:white;color:black;font-family:monospace;font-size:8pt;}" + CRLF +
                         "div{margin:0px;padding:0px;}" + CRLF, t;

            //Save styles for each of the colors used
            for (size_t i = 0; i < CONSOLE_COLOR_COUNT; i++)
            {

                //Color values are back-words; fix this or colors are wrong in HTML
                int c = CONSOLE_COLORS[i];
                c = ((c & 0xFF0000) >> 16) | ((c & 0x0000FF) << 16) | (c & 0x00FF00);

                //Print the  corrected color value as 6-digit hex
                t.Printf("div.c%u{color:#%06x;}", i, c);

                //Append to HTML
                s+= t + CRLF;

            }

            //Finalize header and begin body
            s+= "</style>\n</head>\n<body>" + CRLF;

            //Save what we have so far
            wxScopedCharBuffer scb = s.ToUTF8();
            log->Write((void*)scb.data(), scb.length());

            //Impossible color value to force new <div in HTML
            size_t last_col = 0xff000000;

            //Freeze
            m_Ctrl->Freeze();

            //Handy variable
            wxTextAttr style;

            for (int i = 0; i < line_count; i++)
            {

                //Get style
                m_Ctrl->GetStyle(m_Ctrl->XYToPosition(0, i), style);

                if (style.GetTextColour().GetRGB() != last_col)
                {

                    //Color has changed, close <div if open
                    t = (last_col == 0xff000000) ? "" : t = "</div>";

                    //Update last color to current
                    last_col = style.GetTextColour().GetRGB();

                    //Make new <div
                    t += "<div class=\"c";

                    //Find correct color
                    for (size_t i = 0; i < CONSOLE_COLOR_COUNT; i++)
                    {
                        if (last_col == CONSOLE_COLORS[i])
                        {
                            t << i;
                            break;
                        }
                    }

                    //Finish <div
                    t += "\">" + CRLF;

                    //Save to HTML
                    scb = t.ToUTF8();
                    log->Write((void*)scb.data(), scb.length());

                }

                //Get trimmed log line ended with breaks
                //s = StrTrim(m_Ctrl->GetLineText(i)) + "<br/>" + CRLF;
                s = m_Ctrl->GetLineText(i) + "<br/>" + CRLF;

                //Save to HTML
                scb = s.ToUTF8();
                log->Write((void*)scb.data(), scb.length());

            }

            //Thaw
            m_Ctrl->Thaw();

            //Finish HTML
            s = "</div>" + CRLF + "</body>" + CRLF + "</html>";

            //Save and flush
            scb = s.ToUTF8();
            log->Write((void*)scb.data(), scb.length());
            log->Flush();


        } catch (std::exception &err1)
        {

            ok = false;

        }

        //Close the log file
        log->Close();

    } catch (std::exception &err2)
    {

        ok = false;

    }

    //Release the log file
    if (log != NULL) delete log;

    //Return success or not
    return ok;

}

//---------------------------------------------------------------------------------------

bool FFQConsole::IsLineClipping(const wxString &Line)
{

    //This function checks if a line is related to clipping
    return Line.Lower() == "clipping";
    //return Line.Lower().Find("clipping") != wxNOT_FOUND;

}

//---------------------------------------------------------------------------------------

bool FFQConsole::IsLineMessageRepeat(wxString Line, size_t &Repetitions)
{

    //Examine if a line is info about a repeated message
    int pos = Line.Lower().Find("message repeated");

    if (pos < 0) return false; //No was not

    //Yes it was, extract the repetition count
    Line.Remove(0, pos + 17);
    Repetitions = (size_t)Str2Long(Line.BeforeFirst(' '), 0);

    //If Repetitions if <= 0 the number convert failed
    return Repetitions > 0;

}

//---------------------------------------------------------------------------------------

bool FFQConsole::IsLineStatistics(wxString Line, TIME_VALUE &Time, size_t &Frame)
{

    //This function extracts information from statistical output in the format:
    //frame=  214 fps= 53 q=27.0 size=    5538kB time=00:00:07.87 bitrate=5762.6kbits/s dup=28 drop=0

    //Following is probably redundant - but just in case..
    if ((Line.Find("size=") < 0) || (Line.Find("bitrate=") < 0)) return false;

    int pos;

    //if (Frame != NULL)
    //{

        //Extract frame number if possible
        pos = Line.Find("frame=");

        if (pos < 0) Frame = 0; //No, not possible

        else
        {

            //Yes, maybe possible
            Line.Remove(0, pos + 6);
            Line = StrTrim(Line);
            Frame = Str2Long(Line.BeforeFirst(' '), 0);

        }

    //}

    //Try to extract the time processed
    pos = Line.Find("time=");
    if (pos < 0) return false;

    Line.Remove(0, pos + 5);
    Line = StrTrim(Line);
    Time = TIME_VALUE(Line.BeforeFirst(' '));
    if (Time.IsValid()) return true;

    //Extraction of time failed
    Time = 0;
    return false;

}

//---------------------------------------------------------------------------------------

bool FFQConsole::IsLineVidStabLowContrast(wxString Line, size_t &Frame)
{

    //This function examines the line and checks if it is a report about no transforms
    //and then extracts the frame number - example:
    //[vidstabdetect @ 000000000022f120] too low contrast. (no translations are detected in frame 156)

    if (

        (Line.Len() == 0) || (Line.Find("vidstabdetect") < 0) || (Line.Find("too low contrast") < 0)

       ) return false;

    //Extract the frame number
    Line.Remove(Line.Len() - 1, 1); //Remove last ")"

    //Get last token as frame number
    long long f = Str2LongLong(Line.AfterLast(' '), -1);

    //Check for failure
    if (f < 0) return false;

    //Something might go missing here 64bit => 32bit
    Frame = (size_t)f;

    //Return success
    return true;

}

//---------------------------------------------------------------------------------------

bool FFQConsole::ClearStatsAnim()
{

    //Check if last line was statistics with appended animation
    //character and if so - remove the animation character

    if (m_LastMsgType == mtSTATS)
    {

        int last = m_Ctrl->GetNumberOfLines() - 2;
        wxString s = m_Ctrl->GetLineText(last);
        int len = s.Len() - 1;
        wxUniChar lc = s.GetChar(len);
        if ((lc == '-') || (lc == '\\') || (lc == '/'))
        {

            s.Remove(len - 1, 2);
            ReplaceLine(last, s);
            return true;

        }

    }

    return false;

}

//---------------------------------------------------------------------------------------
void FFQConsole::ConsoleUpdated()
{

    //Make sure last line is visible and that the caret
    //is moved to the first character of the last line

    //Last line visible
    int l = 0, nol = m_Ctrl->GetNumberOfLines() - 1;

    #ifdef __WINDOWS__
    m_Ctrl->GetClientSize(NULL, &l);
    l /= m_Ctrl->GetCharHeight();
    #endif // __WINDOWS__
    m_Ctrl->ShowPosition(m_Ctrl->XYToPosition(0, nol - l));

    //Caret position
    l = m_Ctrl->XYToPosition(0, nol - 1);
    m_Ctrl->SetSelection(l, l);

    //Focus if needed
    //if (!TextCtrl->HasFocus()) TextCtrl->SetFocus();

}
