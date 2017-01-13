/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQJob.cpp                                                      *
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

#include "FFQJob.h"
#include "FFQConfig.h"
#include "FFQMisc.h"
#include "FFQConst.h"
#include "FFQBuildCmd.h"
#include "FFQProbing.h"
#include "FFQThumbs.h"
#include "../FFQPresetMgr.h"
#include <wx/filename.h>

//---------------------------------------------------------------------------------------

//extern declarations
const wxString CMD_INPUTS  = "<%inputs%>";
const wxString CMD_OUTPUT  = "<%output%>";
const wxString CMD_PRESET  = "<%preset%>";
const wxString CMD_DEFAULT = CMD_INPUTS + CMD_PRESET + CMD_OUTPUT;

//---------------------------------------------------------------------------------------

//Constants for loading and saving job values
const wxString JOB_INPUT1 = "input1";
const wxString JOB_INPUT2 = "input2";
const wxString JOB_OUTPUT = "output";
const wxString JOB_STREAM_MAP = "stream_map";
const wxString JOB_CMD_LINE = "command_line";
const wxString JOB_PRESET = "preset";
const wxString JOB_START_1 = "start_1";
const wxString JOB_START_2 = "start_2";
const wxString JOB_OUT_LEN = "out_len";
const wxString JOB_DURATION_1 = "duration_1";
const wxString JOB_DURATION_2 = "duration_2";
//const wxString JOB_SAVE_LOG = "save_log";
const wxString JOB_SIZE_1 = "size_1";
const wxString JOB_SIZE_2 = "size_2";

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQ_JOB::FFQ_JOB() : FFQ_QUEUE_ITEM()
{

    //Default constructor - reset only
    Reset(false);

}

//---------------------------------------------------------------------------------------

FFQ_JOB::FFQ_JOB(const FFQ_JOB &copy_from) : FFQ_QUEUE_ITEM(copy_from)
{

    //Constructor to clone another job
    out = copy_from.out;
    stream_map = copy_from.stream_map;
    cmd_line = copy_from.cmd_line;
    out_len = copy_from.out_len;
    preset_id = copy_from.preset_id;
    preset_ptr = copy_from.preset_ptr;
    //save_log = copy_from.save_log;
    skip_in_files = copy_from.skip_in_files;

}

//---------------------------------------------------------------------------------------

FFQ_JOB::FFQ_JOB(wxString &from) : FFQ_QUEUE_ITEM(from)
{

    //Constructor that parses values from a string
    Reset(true);

}

//---------------------------------------------------------------------------------------

void FFQ_JOB::Cleanup()
{

    //Cleanup the finished job
    CleanupFinishedJob(this);

    //Copy the file time
    CpFileTime(GetPreset(), out);

}

//---------------------------------------------------------------------------------------

TIME_VALUE FFQ_JOB::Duration()
{

    //Return the duration of the job (the time to encode)

    TIME_VALUE res = out_len;

    if (res.IsUndefined())
    {

        //Find first valid duration of input file
        for (size_t i = 0; i < inputs.Count(); i++)
        {

            FFQ_INPUT_FILE inf = FFQ_INPUT_FILE(inputs[i]);
            res = inf.duration;
            if (!res.IsUndefined()) break;

        }

    }

    return res;

}

//---------------------------------------------------------------------------------------

bool FFQ_JOB::GetLogFileName(wxString &name)
{

    //Override default behavior and return log file name based on "out"
    name = save_log ? MakeLogFileName(out) : "";
    return name.Len() > 0;

}

//---------------------------------------------------------------------------------------

LPFFQ_PRESET FFQ_JOB::GetPreset()
{

    //Return the preset indicated by preset_id
    return FFQPresetMgr::Get()->GetPreset(preset_id.ToString());

}

//---------------------------------------------------------------------------------------

wxString FFQ_JOB::ToString()
{

    //Store values
    SetValue(JOB_OUTPUT, out);
    SetValue(JOB_STREAM_MAP, stream_map);
    SetValue(JOB_CMD_LINE, cmd_line);
    SetValue(JOB_OUT_LEN, out_len.ToString());
    SetValue(JOB_PRESET, preset_id.ToString());
    //SetValue(JOB_SAVE_LOG, BOOLSTR(save_log));

    //Pack the job to a string
    return FFQ_QUEUE_ITEM::ToString();

}

//---------------------------------------------------------------------------------------

bool FFQ_JOB::UsesPreset(wxString pst_id)
{

    //Check if preset id matches
    return (preset_id.ToString() == pst_id);

}

//---------------------------------------------------------------------------------------

wxString FFQ_JOB::GetCommandAtIndex(int index)
{

    wxString res, dur = Duration().ToString();

    if (index == 0)
    {

        //Reset encoding pass and command list
        encoding_pass = 0;
        m_CmdList.Clear();

        //Make command for first pass
        res = BuildCommandLine(this, encoding_pass, false, false);

        if (res.Len() > 0)
        {

            //Ensure path / directory is created, and fail if not
            if (!ForcePath(out.BeforeLast(wxFileName::GetPathSeparator())))
            {
                status = qsFAILED;
                return "";
            }

            //Update job status for success
            else status = (encoding_pass == 1) ? qsPASS1 : qsACTIVE;

        }

    }

    else if ((index == 1) && (encoding_pass == 1))
    {

        //Second pass
        res = BuildCommandLine(this, encoding_pass, false, false);

        //Status
        if (res.Len() > 0) status = qsPASS2;

    }

    else if ( ((index == 1) && (encoding_pass == 0)) || ((index == 2) && (encoding_pass == 2)) )
    {

        //Make commands for thumbs and tiles

        //Get the preset and test if TnT are needed
        LPFFQ_PRESET pst = GetPreset();
        if (pst && (pst->thumbs.make_thumbs || pst->thumbs.make_tiles))
        {

            //Probe output file
            FFProbeInfoParser* pip = new FFProbeInfoParser();
            if (pip->RunFFProbe(out, NULL, true))
            {

                //Get duration of out
                TIME_VALUE tv;
                pip->GetDuration(tv);

                //Build command(s)
                m_CmdList = BuildThumbsAndTilesCommand(pst->thumbs, out, tv, true);

                //Update status
                status = qsTHUMBS;

            }
            delete pip;

        }

    }

    //Return command from list first
    if (m_CmdList.Len() > 0) return GetLine(m_CmdList, true);

    //Check command validity second
    if (res.Len() == 0)
    {

        //Set finished status
        status = (index == 0) ? qsFAILED : qsDONE;

        //Cleanup (done in FFQMain::AfterItemProcessing)
        //CleanupFinishedJob(this);

        //Return empty string
        return res;

    }

    //Return duration,command third
    return dur + "," + res;

}

//---------------------------------------------------------------------------------------

//Helper for backwards compatibility
void ConvertOldInput(LPFFQ_JOB job, const wxString &path, const wxString &start, const wxString &duration, const wxString &size)
{

    FFQ_INPUT_FILE inf;
    inf.path = job->GetValue(path);

    if (inf.path.Len() > 0)
    {

        //Load in*
        inf.start = TIME_VALUE(job->GetValue(start));
        inf.duration = TIME_VALUE(job->GetValue(duration));
        wxString s = job->GetValue(size);
        inf.width = Str2Long(GetToken(s, ','));
        inf.height = Str2Long(s);

        //Add to inputs
        job->inputs.Add(inf.ToString());

        //Clear old values
        job->SetValue(path, wxEmptyString);
        job->SetValue(start, wxEmptyString);
        job->SetValue(duration, wxEmptyString);
        job->SetValue(size, wxEmptyString);

    }

}

//---------------------------------------------------------------------------------------

void FFQ_JOB::Reset(bool load)
{

    //Reset the values of the job
    out = "";
    stream_map = "";
    cmd_line = "";
    out_len = TIME_VALUE();
    preset_id.Reset();
    preset_ptr = NULL;
    //save_log = true;
    skip_in_files = -1;

    if (load)
    {

        //Load values from FFQ_QUEUE_ITEM::m_Values

        //For backward compatibility
        ConvertOldInput(this, JOB_INPUT1, JOB_START_1, JOB_DURATION_1, JOB_SIZE_1);
        ConvertOldInput(this, JOB_INPUT2, JOB_START_2, JOB_DURATION_2, JOB_SIZE_2);

        //Load
        out = GetValue(JOB_OUTPUT);
        stream_map = GetValue(JOB_STREAM_MAP);
        cmd_line = GetValue(JOB_CMD_LINE);
        out_len = TIME_VALUE(GetValue(JOB_OUT_LEN, out_len.ToString()));
        preset_id = UNIQUE_ID(GetValue(JOB_PRESET, preset_id.ToString()));
        //save_log = STRBOOL(GetValue(JOB_SAVE_LOG, BOOLSTR(save_log)));

    }

}
