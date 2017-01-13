/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQJob.h                                                        *
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

#ifndef FFQJOB_H
#define FFQJOB_H

#include <wx/string.h>
#include "FFQQueueItem.h"
#include "FFQTimeValue.h"
#include "FFQUniqueID.h"
#include "FFQPreset.h"

//Constants to define parts of the job's command line
extern const wxString CMD_INPUTS;
extern const wxString CMD_OUTPUT;
extern const wxString CMD_PRESET;
extern const wxString CMD_DEFAULT;

//The FFQ_JOB structure
typedef struct FFQ_JOB : FFQ_QUEUE_ITEM
{

    wxString out, //Output file
             stream_map, //Stream mapping for input files
             cmd_line; //The job's command line
    TIME_VALUE out_len; //The time limit of the output file
    UNIQUE_ID preset_id; //The id of the preset to use for building the ffmpeg command
    void* preset_ptr; //A pointer to a preset to use for building the ffmpeg command (usually used by tools)
    long skip_in_files, //Amount of input files that must be skipped (see BuildCommandLine for details)
         encoding_pass; //Current encoding pass

    FFQ_JOB();
    FFQ_JOB(const FFQ_JOB &copy_from);
    FFQ_JOB(wxString &from);

    virtual void Cleanup();
    TIME_VALUE Duration();
    virtual QUEUE_ITEM_TYPE GetItemType() { return qtJOB; };
    virtual bool GetLogFileName(wxString &name);
    LPFFQ_PRESET GetPreset();
    virtual wxString ToString();
    virtual bool UsesPreset(wxString pst_id);

protected:

    virtual wxString GetCommandAtIndex(int index);

private:

    wxString m_CmdList; //Used for thumb and tile commands

    void Reset(bool load);

} FFQ_JOB, *LPFFQ_JOB;

#endif // FFQJOB_H
