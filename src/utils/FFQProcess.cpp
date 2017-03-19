/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQProcess.cpp                                                  *
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

#include "FFQProcess.h"
#include <wx/mstream.h>
//#include <wx/thread.h>
#include "FFQConfig.h"
#include "FFQMisc.h"
#include "FFQBuildCmd.h"
#include "../utils/FFQLang.h"

//---------------------------------------------------------------------------------------

//The size of the buffer used to read output from FF
#define READ_BUFFER_SIZE 50000

//The arguments passed to ffprobe
//#define FFPROBE_ARGS wxString("-pretty -show_format -print_format default -show_streams -i")
#define FFPROBE_ARGS wxString("-pretty -show_format -show_streams")

//Arguments used with ffmpeg
#define FFMPEG_CODECS "-codecs"
#define FFMPEG_ENCODERS "-encoders"
#define FFMPEG_VERSION "-version"
#define FFMPEG_FILTERS "-filters"
#define FFMPEG_FORMATS "-formats"
#define FFMPEG_PIXFMTS "-pix_fmts"


//---------------------------------------------------------------------------------------

FFQProcess::FFQProcess()
{

    //Default constructor - reset all
    m_Aborted = false;
    m_Waiting = false;
    m_Buffer = new char[READ_BUFFER_SIZE];

    m_CommandLine.Clear();
    m_ErrOut.Clear();
    m_StdOut.Clear();

    m_Process = NULL;
    m_ProcessId = wxNewId();

    //Connect the event handler in order to handle terminate events
    Connect(m_ProcessId, wxEVT_END_PROCESS, (wxObjectEventFunction)&FFQProcess::OnTerminate);

}

//---------------------------------------------------------------------------------------

FFQProcess::~FFQProcess()
{

    //Force abort to prevent hanging
    Abort(false);

    //Release any occupied memory
    delete[] m_Buffer;
    m_Buffer = NULL;

}

//---------------------------------------------------------------------------------------

bool FFQProcess::Abort(bool send_quit, int wait_timeout)
{

    //If no process is running, exit
    if (!IsProcessRunning()) return true;

    //Signal aborted
    m_Aborted = true;

    //If we need to send a quit request we do so
    if (send_quit) m_Process->GetOutputStream()->PutC('q');

    //If not we kill the process the hard way!
    else if (wxProcess::Kill(m_Process->GetPid(), wxSIGKILL) == wxKILL_OK) {

        //The terminate event is not called when the process is killed,
        //so we must fake it here
        wxProcessEvent event;
        OnTerminate(event);

    }

    else ThrowError(FFQS(SID_KILL_FFMPEG_FAILED));

    //Wait for the terminate event until timeout
    return (wait_timeout >= 0) ? WaitFor((unsigned int)wait_timeout) : true;

}

//---------------------------------------------------------------------------------------

bool FFQProcess::WasAborted()
{

    //Return true if abort was performed
    return m_Aborted;

}

//---------------------------------------------------------------------------------------

bool FFQProcess::IsProcessRunning()
{

    //Returns true if a process is running
    return (m_Process != NULL);

}

//---------------------------------------------------------------------------------------

void FFQProcess::SetCommand(bool ffprobe, wxString args)
{

    //Set the command to ffmpeg or ffprobe with the given arguments
    SetCommand(FFQCFG()->GetFFMpegCommand(ffprobe), args);

}

//---------------------------------------------------------------------------------------

void FFQProcess::SetCommand(wxString command, wxString args)
{

    //Ensure that the process is not already running
    if (IsProcessRunning()) ThrowError(FFQS(SID_ACTIVE_CANNOT_SET_COMMAND));

    //Set the raw command line
    m_CommandLine = command;

    //If the path to command includes spaces it must be quoted
    if ((m_CommandLine.Find(' ') >= 0) && (wxString("\"\'").Find(m_CommandLine.at(0)) < 0))
        m_CommandLine = "\"" + m_CommandLine + "\"";

    //Append any arguments
    if (args.Len() > 0) m_CommandLine += " " + args;

}

//---------------------------------------------------------------------------------------

bool FFQProcess::TransactPipes(bool in, bool err)
{

    //If no process is running we fail
    if ( !IsProcessRunning() ) return false;

    //Now we can transact the pipes to the running process
    //First check which pipes has content
    bool std_ok = in && m_Process->IsInputAvailable(),
         err_ok = err && m_Process->IsErrorAvailable();

    //Read content send to stdout - if any
    if (std_ok) m_StdOut += ReadInputStream(m_Process->GetInputStream());

    //Read content send to errout - if any
    if (err_ok) m_ErrOut += ReadInputStream(m_Process->GetErrorStream());

    //Return true if any content was read
    return std_ok || err_ok;

}

//---------------------------------------------------------------------------------------

bool FFQProcess::WaitFor(unsigned int timeout)
{

    //WARNING!! This method is dangerous! The yielding done with Yield_App
    //which ensures this function not to block might cause FFQProcess::this
    //to be deleted by a window, thus causing a SIGSEGV crash

    //Waits for the process to terminate in timeout
    //milliseconds and return the result

    //A process must be running - else we might deadlock
    //if (!IsProcessRunning()) return true;

    if (m_Waiting)
    {

        //Already waiting = problem
        #ifdef DEBUG
        ShowError("Already waiting!");
        #endif // DEBUG
        return false;

    }

    uint64_t st = GetTimeTickCount(); //start tick
    bool to = false; //timeout
    m_Waiting = true;

    do
    {

        //Check for timeout
        to = (timeout > 0) && ((GetTimeTickCount() - st) >= timeout);

        //If not timed out we must transact pipes to
        //prevent the process from hanging on a blocking
        //pipe call. If nothing is transacted we yield
        //in order to let the terminate event being received
        if ((!to) && (!TransactPipes())) Yield_App(5);

    } while ((!m_Terminated) && (!to));

    m_Waiting = false;

    //Return success
    return m_Terminated;

}

//---------------------------------------------------------------------------------------

void FFQProcess::Execute(bool wait, bool redirect)
{

    //If already executing we fail
    if (IsProcessRunning()) ThrowError(FFQS(SID_PROCESS_ALREADY_RUNNING));

    //Reset variables for the new process
    m_Terminated = false;
    m_Aborted = false;
    //m_Waiting = wait;
    m_StdOut = "";
    m_ErrOut = "";

    //Create process with the correct id (for event filtering)
    m_Process = new wxProcess(this, m_ProcessId);

    //Redirect as required
    if (redirect) m_Process->Redirect();

    //Execute the command
    long pid = wxExecute(m_CommandLine, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER, m_Process);

    //Set time of start
    m_StartTime = GetTimeTickCount();
    m_FinalTransact = true;

    if (pid == 0)
    {

        //Execution failed for some reason
        delete m_Process;
        m_Process = NULL;
        ThrowError(FFQSF(SID_EXECUTE_COMMAND_ERROR, m_CommandLine));

    }

    //If we should wait for the process to finish we do so
    if (wait) WaitFor();

}

//---------------------------------------------------------------------------------------

void FFQProcess::ExecuteAndWait()
{

    //Execute a command and wait for it to finish
    Execute(true, true);

}

//---------------------------------------------------------------------------------------

bool FFQProcess::ExtractFrameFromFile(wxString file_name, TIME_VALUE frame_time, wxImage *img, unsigned int timeout, unsigned int accuracy, wxSize fit_to)
{

    //Extract one frame from a movie as an image;

    bool success = false;

    //Create video filter
    wxString vf = "-vf \"";

    if ( (fit_to.GetWidth() > 0) && (fit_to.GetHeight() > 0) )
    {

        //Apply scaling (fit to rect)
        vf += wxString::Format("scale='iw*min(%i/iw,%i/ih)':'ih*min(%i/iw,%i/ih)'",
                fit_to.GetWidth(), fit_to.GetHeight(), fit_to.GetWidth(), fit_to.GetHeight()
              );

    }

    if (accuracy > 0)
    {

        //Create filter to extract frame with specified accuracy (milliseconds to encode - higher = more accurate)
        uint64_t ft = frame_time.ToMilliseconds();
        if (ft > accuracy)
        {
            frame_time.SetMilliseconds(ft - accuracy);
            ft = accuracy;
        }
        else frame_time.SetMilliseconds(0);

        if (ft > 0) vf += ",select='gte(t\\," + TIME_VALUE(ft).ToShortString() + ")'";

    }

    //Use or clear vf?
    if (vf.Right(1) == "\"") vf.Clear();
    else vf += "\" ";

    //Create ss
    wxString ss = (frame_time.ToMilliseconds() == 0) ? "" : "-ss " + frame_time.ToShortString() + " ";

    //Set the command
    SetCommand(false, "-hide_banner " + ss + "-i \"" + FormatFileName(file_name) + "\" " + vf + "-an -c:v mjpeg -vframes 1 -f mjpeg -y pipe:1");

    //ShowInfo(m_CommandLine);

    try
    {

        //Execute but not wait
        Execute(false, true);

        //Prevent final transaction
        m_FinalTransact = false;

        //Temporary memory
        wxMemoryOutputStream memory;

        //Read image data into memory
        bool tout;

        do
        {

            //Copy data
            if (m_Process->IsInputAvailable()) memory.Write(*m_Process->GetInputStream());

            //Empty IO-buffer for errout or yield
            else if (!TransactPipes(false, true)) Yield_App(0);

            //Time out?
            tout = (timeout > 0) && (GetTimeTickCount() - m_StartTime >= timeout);

        } while ( (!tout) && (!m_Terminated) );

        //Completed ok?
        if ((!tout) && (memory.GetSize() > 0))
        {

            //Yes, load image
            wxMemoryInputStream imgdata(memory);
            img->LoadFile(imgdata, wxBITMAP_TYPE_JPEG);
            success = true;

        }

    }
    catch (std::exception &err)
    {

        //ShowError(wxString(err.what()));

    }

    //Always abort for good measures
    if (IsProcessRunning()) try
    {

        /*if (!Abort(true, 3000))*/ Abort(false);

    } catch(std::exception &err) { }

    return success;

}

//---------------------------------------------------------------------------------------

void FFQProcess::FFProbe(wxString input_file)
{

    //Set the ffprobe command for the input file
    SetCommand(true, FFPROBE_ARGS + " \"" + input_file + "\"");

    //Execute and wait
    ExecuteAndWait();

}

//---------------------------------------------------------------------------------------

uint64_t FFQProcess::GetRunningTimeMillis()
{

    //Returns the amount of milliseconds elapsed since the process was started
    return GetTimeTickCount() - m_StartTime;

}

//---------------------------------------------------------------------------------------

uint64_t FFQProcess::GetStartTimeMillis()
{

    //Get the start time in milliseconds
    return m_StartTime;

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetFFMpegCodecs(wxString ff_path)
{

    //Get list of supported codecs
    return ExecArgAndGetResult(FFMPEG_CODECS, ff_path);

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetFFMpegEncoders(wxString ff_path)
{

    //Get list of supported encoders
    return ExecArgAndGetResult(FFMPEG_ENCODERS, ff_path);

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetFFMpegFilters(wxString ff_path)
{

    //Get list of supported filters
    return ExecArgAndGetResult(FFMPEG_FILTERS, ff_path);

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetFFMpegFormats(wxString ff_path)
{

    //Get list of supported formats
    return ExecArgAndGetResult(FFMPEG_FORMATS, ff_path);

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetFFMpegOther(wxString ff_args, wxString ff_path)
{
    //Get other output from ffmpeg
    return ExecArgAndGetResult(ff_args, ff_path);
}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetFFMpegPixelFormats(wxString ff_path)
{

    //Get list of supported pixel formats
    return ExecArgAndGetResult(FFMPEG_PIXFMTS, ff_path);

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetFFMpegVersion(wxString ff_path)
{

    //Get ffmpeg version
    return ExecArgAndGetResult(FFMPEG_VERSION, ff_path, true);

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetProcessOutput(bool error_out, bool clear)
{

    //If we are not waiting for a process to terminate
    //we transact any pending output
    if (!m_Waiting) TransactPipes();

    //Get pointer to the correct output
    wxString* pstr = error_out ? &m_ErrOut : &m_StdOut;

    //Set as result
    wxString res = *pstr;

    //Clear if requested
    if (clear) *pstr = "";

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::GetProcessOutputLine(bool error_out, bool clear)
{

    //GetLine could be used here - but it will return incomplete lines which is unwanted

    //As GetProcessOutput
    if (!m_Waiting) TransactPipes();

    //Pointer to the correct content
    wxString *str = error_out ? &m_ErrOut : &m_StdOut;

    //Find first CR or LF character
    char delChar = '\n';
    int cr = str->Find('\r'), lf = str->Find('\n');
    if ((lf >= 0) && (lf < cr))
    {

        //LF is before CR - so we switch around
        cr = lf;
        delChar = '\r';

    }

    if (cr >= 0)
    {

        //A line is available - get it
        wxString res = str->SubString(0, cr - 1);

        if (clear)
        {

            //Delete until and including CR or LF or CR+LF or LF+CR
            if (((unsigned int)cr + 1 < str->Len()) && (str->at(cr + 1) == delChar)) cr++;
            str->Remove(0, cr + 1);

        }

        //Return the line
        return res;

    }

    else if (!IsProcessRunning())
    {

        //Return anything available after process has terminated
        return GetProcessOutput(error_out, clear);

    }

    //Nothing available
    return "";

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::ExecArgAndGetResult(wxString arg, wxString ff_path, bool prepend_err_out)
{

    //If ff_path is provided we use it rather than the one in the config
    //which makes it possible to verify a path to the ff-binaries
    if (ff_path.Len() > 0) SetCommand(ff_path, arg);

    //Use the path from config instead
    else SetCommand(false, arg);

    //Execute and wait
    ExecuteAndWait();

    //Return the output
    return prepend_err_out ? (GetProcessOutput(true) + GetProcessOutput(false)) : GetProcessOutput(false);

}

//---------------------------------------------------------------------------------------

wxString FFQProcess::ReadInputStream(wxInputStream *in)
{

    if (in->CanRead())
    {

        //If read is possible we copy as much as possible to the buffer
        in->Read(m_Buffer, READ_BUFFER_SIZE);

        //Get the amount of bytes read
        unsigned int read = in->LastRead();

        //If anything was read it is assumed to be UTF-8
        if (read > 0) return wxString::FromUTF8(m_Buffer, read);

    }

    //Nothing available
    return "";

}

//---------------------------------------------------------------------------------------

void FFQProcess::OnTerminate(wxProcessEvent& event)
{

    //We need to be safe of exceptions here since they might cause deadlock
    if (m_FinalTransact) try
    {

        //Perform a last pipe transaction after terminate
        TransactPipes();

    } catch (...) {}

    //Signal termination for any waiting operations
    m_Terminated = true;

    //Delete and null the process (makes IsProcessRunning() return false)
    delete m_Process;
    m_Process = NULL;

}
