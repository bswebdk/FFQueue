/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQProcess.h                                                    *
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

#ifndef FFQPROCESS_H
#define FFQPROCESS_H

#include <wx/event.h>
#include <wx/process.h>
#include <wx/string.h>
#include <wx/image.h>
#include "FFQTimeValue.h"

class FFQProcess : public wxEvtHandler
{

    public:

        FFQProcess();
        ~FFQProcess();

        bool Abort(bool send_quit, int wait_timeout = -1);
        void Execute(bool wait, bool redirect);
        void ExecuteAndWait();
        bool ExtractFrameFromFile(wxString file_name, TIME_VALUE frame_time, wxImage *img, unsigned int timeout = 0, unsigned int accuracy = 0, wxSize fit_to = wxDefaultSize);
        void FFProbe(wxString input_file);
        uint64_t GetRunningTimeMillis();
        uint64_t GetStartTimeMillis();
		wxString GetFFMpegCodecs(wxString ff_path = "");
		wxString GetFFMpegEncoders(wxString ff_path = "");
		wxString GetFFMpegFilters(wxString ff_path = "");
		wxString GetFFMpegFormats(wxString ff_path = "");
		wxString GetFFMpegOther(wxString ff_args, wxString ff_path = "");
		wxString GetFFMpegPixelFormats(wxString ff_path = "");
		wxString GetFFMpegVersion(wxString ff_path = "");
        wxString GetProcessOutput(bool error_out, bool clear = true);
        wxString GetProcessOutputLine(bool error_out, bool clear = true);
        bool IsProcessRunning();
        void SetCommand(bool probe, wxString args);
        void SetCommand(wxString command, wxString args);
        bool TransactPipes(bool in = true, bool err = true);
        bool WaitFor(unsigned int timeout = 0);
        bool WasAborted();

    protected:

    private:

        bool        m_Aborted, m_Terminated, m_Waiting, m_FinalTransact;
        char        *m_Buffer;
        wxString    m_CommandLine, m_ErrOut, m_StdOut;
        wxProcess   *m_Process;
        long        m_ProcessId;
        uint64_t    m_StartTime;

        wxString ExecArgAndGetResult(wxString arg, wxString ff_path = "", bool prepend_err_out = false);
        wxString ReadInputStream(wxInputStream *in);
        void OnTerminate(wxProcessEvent& event);

};


#endif // FFQPROCESS_H
