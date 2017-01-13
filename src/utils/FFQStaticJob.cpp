/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQStaticJob.cpp                                                *
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

#include "FFQStaticJob.h"
#include "FFQMisc.h"
#include "FFQConst.h"

//---------------------------------------------------------------------------------------

//Constants for loading and saving job values
const wxString JOB_COMMAND = "command";
const wxString JOB_SAVE_LOG = "save_log";
const wxString JOB_DEL_PATH = "del_path";

//---------------------------------------------------------------------------------------

FFQ_STATIC_JOB::FFQ_STATIC_JOB() : FFQ_QUEUE_ITEM()
{

    //Default constructor - reset only
    Reset();

}

//---------------------------------------------------------------------------------------

FFQ_STATIC_JOB::FFQ_STATIC_JOB(const FFQ_STATIC_JOB &copy_from) : FFQ_QUEUE_ITEM()
{

    //Constructor to clone another job
    Reset();

    inputs = wxArrayString(copy_from.inputs);
    commands = wxArrayString(copy_from.commands);
    del_paths = wxArrayString(copy_from.del_paths);
    save_log_for = copy_from.save_log_for;

}

//---------------------------------------------------------------------------------------

FFQ_STATIC_JOB::FFQ_STATIC_JOB(wxString from) : FFQ_QUEUE_ITEM(from)
{

    //Constructor that parses values from a string
    Reset();

    //Handy vars
    wxString n, v;

    while (from.Len() > 0)
    {

        //Get name and value
        v = StrTrim(GetToken(from, '\n'));
        n = GetToken(v, '=');

        //Set the correct value(s)
        if (n == JOB_COMMAND) commands.Add(v);
        else if (n == JOB_DEL_PATH) del_paths.Add(v);
        else if (n == JOB_SAVE_LOG) save_log_for = v;

    }

}

//---------------------------------------------------------------------------------------

bool FFQ_STATIC_JOB::GetLogFileName(wxString &name)
{

    //Set log name and return success
    name = (save_log_for.Len() > 0) ? MakeLogFileName(save_log_for) : "";
    return name.Len() > 0;

}

//---------------------------------------------------------------------------------------

wxString FFQ_STATIC_JOB::ToString()
{

    //Pack the job to a string
    wxString res = FFQ_QUEUE_ITEM::ToString();
    if (commands.Count() > 0) for (size_t i = 0; i < commands.Count(); i++) res += JOB_COMMAND + '=' + commands[i] + CRLF;
    if (del_paths.Count() > 0) for (size_t i = 0; i < del_paths.Count(); i++) res += JOB_DEL_PATH + '=' + del_paths[i] + CRLF;
    if (save_log_for.Len() > 0) res += JOB_SAVE_LOG + '=' + save_log_for + CRLF;
    if (res.Len() > 0) res.RemoveLast(CRLF.Len()); //Remove last CRLF
    return res;

}

//---------------------------------------------------------------------------------------

wxString FFQ_STATIC_JOB::GetCommandAtIndex(int index)
{

    //Return next command
    if ((size_t)index < commands.Count())
    {
        status = qsACTIVE;
        return commands[(size_t)index];
    }

    //Set status to done and cleanup
    status = qsDONE;

    //Delete any paths
    DeletePaths(&del_paths);

    //Return empty string; No command
    return wxEmptyString;

}

//---------------------------------------------------------------------------------------

void FFQ_STATIC_JOB::Reset()
{

    //Reset the values of the job
    commands.Clear();
    del_paths.Clear();
    save_log_for.Clear();

}
