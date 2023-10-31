/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQQueueItem.cpp                                                *
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

#include "FFQQueueItem.h"
#include "FFQConst.h"
#include "FFQMisc.h"
#include "FFQJob.h"
#include "FFQStaticJob.h"
#include "FFQToolJobs.h"
#include "../utils/FFQLang.h"
#include "../utils/FFQConfig.h"
#include <wx/filename.h>

//---------------------------------------------------------------------------------------

const wxString ITEM_INPUT = "input=";
const wxString ITEM_SAVE_LOG = "save_log";
const wxString ITEM_VERSION = "version";

//---------------------------------------------------------------------------------------

FFQ_CUTS::FFQ_CUTS()
{

    //Default constructor, reset
    Reset();

}

//---------------------------------------------------------------------------------------

FFQ_CUTS::FFQ_CUTS(wxString from)
{

    //Reset first
    Reset();

    //Parse values from parse_from
    wxString cfg = GetToken(from, ":", true);
    filter_first = STRBOOLDEF(GetToken(cfg, "|", true), filter_first);
    keep = STRBOOLDEF(GetToken(cfg, "|", true), keep);
    frame_time = Str2Long(GetToken(cfg, "|", true), frame_time);
    quick = STRBOOLDEF(GetToken(cfg, "|", true), quick);
    cuts = from;


}

//---------------------------------------------------------------------------------------

wxString FFQ_CUTS::KeepParts(TIME_VALUE file_duration)
{
    //Return a string with parts to keep
    if (keep || (cuts.Len() == 0)) return cuts;
    bool odd = true;
    wxString res = "", c = cuts;
    TIME_VALUE tv(GetToken(c, ";", true));
    if (tv.ToMilliseconds() > 0) res = "0;" + TIME_VALUE(tv.ToMilliseconds() - frame_time).ToString() + ";";
    while (c.Len() > 0)
    {
        tv = TIME_VALUE(GetToken(c, ";", true));
        if (tv.ToMilliseconds() < file_duration.ToMilliseconds())
        {
            tv = odd ? TIME_VALUE(tv.ToMilliseconds() + frame_time) : TIME_VALUE(tv.ToMilliseconds() - frame_time);
            res += tv.ToString() + ";";
        }
        odd = !odd;
    }
    return res + (odd ? file_duration.ToString() : "");
}

//---------------------------------------------------------------------------------------

void FFQ_CUTS::Reset()
{

    //Reset cuts to default
    filter_first = false;
    keep = false;
    quick = false;
    frame_time = 0;
    cuts.Clear();

}

//---------------------------------------------------------------------------------------

wxString FFQ_CUTS::ToString()
{

    //Pack cuts - since this is a value stored in
    //FFQ_INPUT_FILE we cannot use "," as separator
    return wxString::Format("%s|%s|%u|%s:%s", BOOLSTR(filter_first), BOOLSTR(keep), frame_time, BOOLSTR(quick), cuts);

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQ_INPUT_FILE::FFQ_INPUT_FILE()
{

    //Default constructor - reset values
    Reset();

}

//---------------------------------------------------------------------------------------

FFQ_INPUT_FILE::FFQ_INPUT_FILE(wxString from)
{

    //Parsing constructor - extract values from a string

    //Get version
    int version = Str2Long(GetToken(from, ","), 1), d;

    start = TIME_VALUE(GetToken(from, ","));
    duration = TIME_VALUE(GetToken(from, ","));

    d = Str2Long(GetToken(from, ","), -1);
    width = (d < 0) ? 0 : (unsigned int)d;
    d = Str2Long(GetToken(from, ","), -1);
    height = (d < 0) ? 0 : (unsigned int)d;

    itsoffset = Str2Long(GetToken(from, ","));
    if (version > 2) loop = Str2Long(GetToken(from, ","));
    framerate = GetToken(from, ",");

    fflags_discardcorrupt = STRBOOL(GetToken(from, ","));
    fflags_genpts = STRBOOL(GetToken(from, ","));
    fflags_igndts = STRBOOL(GetToken(from, ","));
    fflags_ignidx = STRBOOL(GetToken(from, ","));
    fflags_sortdts = STRBOOL(GetToken(from, ","));
    //fflags_fastseek = STRBOOL(GetToken(from, ","));
    //fflags_bitexact = STRBOOL(GetToken(from, ","));

    //Check version, and get cuts
    if (version > 1) cuts = FFQ_CUTS(GetToken(from, ",", true));

    //Now there is only the path left
    path = from;

}

//---------------------------------------------------------------------------------------

void FFQ_INPUT_FILE::Reset()
{

    //Reset values
    path.Clear();
    cuts.Reset();

    start = TIME_VALUE();
    duration = TIME_VALUE();

    width = 0;
    height = 0;

    itsoffset = 0;
    loop = 0;
    framerate.Clear();

    fflags_discardcorrupt = false;
    fflags_genpts = false;
    fflags_igndts = false;
    fflags_ignidx = false;
    fflags_sortdts = false;
    //fflags_fastseek = false;
    //fflags_bitexact = false;

}

//---------------------------------------------------------------------------------------

wxString FFQ_INPUT_FILE::ToString()
{

    //Packs values to a string. The first number in the string
    //is intended as a version indicator that can be used to
    //determine the number of values packed into the string -
    //path must always be last!

    wxString res;

    res.Printf(

        "3,%s,%s,%u,%u,%i,%i,%s,%s,%s,%s,%s,%s,%s,%s", //,%s,%s",

        start.ToString(), duration.ToString(),

        width, height,

        itsoffset, loop, framerate,

        BOOLSTR(fflags_discardcorrupt), BOOLSTR(fflags_genpts),
        BOOLSTR(fflags_igndts), BOOLSTR(fflags_ignidx),
        BOOLSTR(fflags_sortdts), /*BOOLSTR(fflags_fastseek),
        BOOLSTR(fflags_bitexact),*/

        cuts.ToString(), path

    );

    return res;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQ_QUEUE_ITEM::FFQ_QUEUE_ITEM()
{

    //Reset values
    Reset();

}

//---------------------------------------------------------------------------------------

FFQ_QUEUE_ITEM::FFQ_QUEUE_ITEM(wxString &item)
{

    //Parsing constructor, reset values
    Reset();

    //Parse input files
    while (item.Find(ITEM_INPUT) == 0)
    {

        wxString v = GetLine(item, true);
        inputs.Add(v.AfterFirst('='));

    }

    //Make NVP list
    m_Values = NVPListFromLines(item);

    //Load values
    save_log = STRBOOL( GetValue(ITEM_SAVE_LOG, BOOLSTR(save_log)) );
    version = Str2Long( GetValue(ITEM_VERSION), version );

}

//---------------------------------------------------------------------------------------

FFQ_QUEUE_ITEM::FFQ_QUEUE_ITEM(const FFQ_QUEUE_ITEM &copy_item)
{

    //Copy constructor to clone items
    Reset();

    //status = copy_item.status; //DO NOT CLONE STATUS!
    inputs = wxArrayString(copy_item.inputs);
    save_log = copy_item.save_log;
    version = copy_item.version;
    m_Values = NVPListClone(copy_item.m_Values);

}

//---------------------------------------------------------------------------------------

FFQ_QUEUE_ITEM::~FFQ_QUEUE_ITEM()
{

    //Release the NVP list for values
    delete m_Values;

}

//---------------------------------------------------------------------------------------

bool FFQ_QUEUE_ITEM::CheckNotActive()
{
    if (IsActive())
    {
        ShowError(FFQS(SID_CANNOT_SAVE_ACTIVE_QUEUE_ITEM));
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------

FFQ_QUEUE_ITEM* FFQ_QUEUE_ITEM::Clone(FFQ_QUEUE_ITEM* item)
{

    //Clone the item

    switch(item->GetItemType())
    {

        case qtCONCAT_JOB: return new FFQ_CONCAT_JOB(*((LPFFQ_CONCAT_JOB)item)); //break;

        case qtJOB: return new FFQ_JOB(*((LPFFQ_JOB)item)); //break;

        case qtSTATIC_JOB: return new FFQ_STATIC_JOB(*((LPFFQ_STATIC_JOB)item)); //break;

        case qtTHUMB_JOB: return new FFQ_THUMB_JOB(*((LPFFQ_THUMB_JOB)item)); //break;

        case qtVIDSTAB_JOB: return new FFQ_VIDSTAB_JOB(*((LPFFQ_VIDSTAB_JOB)item)); //break;

        case qtVID2GIF_JOB: return new FFQ_VID2GIF_JOB(*((LPFFQ_VID2GIF_JOB)item)); //break;

        default: return NULL;

    }

    return NULL;

}

//---------------------------------------------------------------------------------------

void FFQ_QUEUE_ITEM::Cleanup()
{

    //Only implemented to avoid it being abstract

}

//---------------------------------------------------------------------------------------

wxString FFQ_QUEUE_ITEM::DisplayName()
{

    //Make a display name for the queue list
    wxFileName fn = GetInput(0).path;
    return fn.GetFullName();

}

//---------------------------------------------------------------------------------------

FFQ_INPUT_FILE& FFQ_QUEUE_ITEM::GetInput(unsigned int index)
{

    //Convenient function for getting a variable to an input file

    if (index != m_TempIndex)
    {

        if (index < inputs.Count()) m_TempInput = FFQ_INPUT_FILE(inputs[index]);
        else m_TempInput.Reset(); //Return empty file info

        m_TempIndex = index;

    }

    return m_TempInput;

}

//---------------------------------------------------------------------------------------

bool FFQ_QUEUE_ITEM::GetLogFileName(wxString &name)
{

    //Return default log file name based on first input file
    name = (save_log && (inputs.Count() > 0)) ? MakeLogFileName(GetInput(0).path) : "";
    return name.Len() > 0;

}

//---------------------------------------------------------------------------------------

bool FFQ_QUEUE_ITEM::GetNextCommand(wxString &command, bool for_encode)
{

    //Check if item has an active status
    //wxASSERT( (status >= qsACTIVE) && (status <= qsTHUMBS) );
    //if (status != qsACTIVE) ThrowError("Item has not been prepared for processing");
    if ( (status < qsACTIVE) || (status > qsTHUMBS) )
    {

        command.Clear();
        return false;

    }

    //Get the command at index
    command = GetCommandAtIndex(m_CommandIdx, for_encode);

    //Increment command index
    m_CommandIdx++;

    //Return success
    return command.Len() > 0;

}

//---------------------------------------------------------------------------------------

wxString FFQ_QUEUE_ITEM::GetValue(wxString name, wxString default_value)
{

    //Find a value in m_Values
    if (m_Values)
    {

        LPFFQ_NVP_LIST nvp = m_Values->Find(name, false);
        if (nvp) return nvp->value;

    }

    //Not found
    return default_value;

}

//---------------------------------------------------------------------------------------

bool FFQ_QUEUE_ITEM::IsActive()
{

    //Check for an active status
    return (status == qsACTIVE) || (status == qsPASS1) || (status == qsPASS2) || (status == qsTHUMBS);

}

//---------------------------------------------------------------------------------------

FFQ_QUEUE_ITEM* FFQ_QUEUE_ITEM::Parse(wxString from)
{

    //Get the type of the item
    long qt = Str2Long(GetToken(from, CRLF, false), (long)qtUNKNOWN);

    if (qt == qtUNKNOWN) qt = qtJOB; //If type is undefined, JOB is assumed

    else GetToken(from, CRLF, true); //Type was valid and must be removed from "from"

    //Create the item and return it
    if (qt == qtJOB) return new FFQ_JOB(from);
    else if (qt == qtSTATIC_JOB) return new FFQ_STATIC_JOB(from);
    else if (qt == qtTHUMB_JOB) return new FFQ_THUMB_JOB(from);
    else if (qt == qtVIDSTAB_JOB) return new FFQ_VIDSTAB_JOB(from);
    else if (qt == qtVID2GIF_JOB) return new FFQ_VID2GIF_JOB(from);
    else if (qt == qtCONCAT_JOB) return new FFQ_CONCAT_JOB(from);

    #ifdef DEBUG
    //No matching types found, error out if debugging
    else ThrowError("Invalid item type in FFQ_QUEUE_ITEM::Parse(): " + ToStr((int)qt));
    #endif // DEBUG

    //Impossible to parse
    return NULL;

}

//---------------------------------------------------------------------------------------

void FFQ_QUEUE_ITEM::PrepareProcessing()
{

    //Set index of current command and update status
    m_CommandIdx = 0;
    status = qsACTIVE;

}

//---------------------------------------------------------------------------------------

void FFQ_QUEUE_ITEM::SetValue(wxString name, wxString value)
{

    //Set/remove a value to/from m_Values
    if (value.Len() == 0) NVPListRemove(m_Values, name); //Remove
    else NVPListAppend(m_Values, name, value, false); //Set

}

//---------------------------------------------------------------------------------------

wxString FFQ_QUEUE_ITEM::ToString()
{

    //Return item as string

    //Item type for reconstruction
    wxString res = ToStr((int)GetItemType()) + CRLF;

    //Inputs
    for (unsigned int i = 0; i < inputs.GetCount(); i++) res += ITEM_INPUT + inputs[i] + CRLF;

    //Save values
    SetValue(ITEM_SAVE_LOG, BOOLSTR(save_log));
    SetValue(ITEM_VERSION, version == 0 ? "" : ToStr(version));

    //Append m_Values
    res += NVPListToLines(m_Values);

    //Remove last CRLF
    res.RemoveLast(CRLF.Len());

    return res;

}

//---------------------------------------------------------------------------------------

bool FFQ_QUEUE_ITEM::UsesPreset(wxString pst_id)
{

    //Must be inherited
    return false;

}

//---------------------------------------------------------------------------------------

bool FFQ_QUEUE_ITEM::CpFileTime(void* pst, wxString to_file)
{

    //Convenient function used by descendants to copy the file
    //time of the first input file to to_file

    if ((inputs.Count() > 0) && (pst != NULL) && ((LPFFQ_PRESET)pst)->keep_filetime)
        return CopyFileTime(GetInput(0).path, to_file);

    return false;

}

//---------------------------------------------------------------------------------------

void FFQ_QUEUE_ITEM::Reset()
{

    //Reset values
    status = qsDORMANT;
    queue_flags = 0;
    inputs.Clear();
    save_log = FFQCFG()->save_log;
    version = 0;
    m_TempIndex = (unsigned int)-1;
    m_Values = NULL;

}
