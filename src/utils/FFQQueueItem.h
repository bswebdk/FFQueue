/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQQueueItem.h                                                  *
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

#ifndef FFQQUEUEITEM_H
#define FFQQUEUEITEM_H

#include <wx/string.h>
#include <wx/arrstr.h>
#include "FFQTimeValue.h"
#include "FFQNvpList.h"

//#include "../utils/FFQLang.h"

//Helper structure to parse and pack cuts
typedef struct FFQ_CUTS
{

    bool filter_first, //Filter placement first=true, last=false
         keep; //Keep the parts or remove them?
    unsigned int frame_time; //Time in milliseconds each frame is displayed
    wxString cuts; //The cuts

    FFQ_CUTS();
    FFQ_CUTS(wxString from);
    void Reset();
    wxString ToString();

} FFQ_CUTS, *LPFFQ_CUTS;

//Helper structure that defines an input file and its parameters
typedef struct FFQ_INPUT_FILE
{

    wxString path; //Path to the input file
    FFQ_CUTS cuts; //Cuts to perform on this file

    TIME_VALUE start, //The start from where to process content (-ss)
               duration; //The duration of the media (internal use)

    unsigned int width, //Width of video (internal use)
                 height; //Height of video (internal use)

    int itsoffset; //-itsoffset

    wxString framerate; //-framerate

    //fflags to add
    bool fflags_discardcorrupt,
         fflags_genpts,
         fflags_igndts,
         fflags_ignidx,
         fflags_sortdts/*, //Unused for now
         fflags_fastseek,
         fflags_bitexact*/;

    FFQ_INPUT_FILE();
    FFQ_INPUT_FILE(wxString from);

    void Reset();
    wxString ToString();

} FFQ_INPUT_FILE, *LPFFQ_INPUT_FILE;

//Function for fast creation of input file string
//wxString MakeInputFile(wxString path, TIME_VALUE start = TIME_VALUE(), TIME_VALUE duration = TIME_VALUE(), int width = 0, int height = 0);

//The status of items in the queue
typedef enum QUEUE_STATUS { qsQUEUED, qsACTIVE, qsPASS1, qsPASS2, qsTHUMBS, qsDONE, qsFAILED, qsABORTED, qsSKIPPED, qsDORMANT } QUEUE_STATUS;
const size_t QUEUE_STATUS_COUNT = 10;

//Types of items (used for saving and loading)
typedef enum QUEUE_ITEM_TYPE { qtUNKNOWN, qtJOB, qtSTATIC_JOB, qtTHUMB_JOB, qtVIDSTAB_JOB, qtCONCAT_JOB } QUEUE_ITEM_TYPE;

//Flags used in the queue
typedef unsigned short QUEUE_FLAG;
const QUEUE_FLAG QUEUE_FLAG_OVERWRITE      = 1 << 0; //Can output file be overwritten?
const QUEUE_FLAG QUEUE_FLAG_PRESET_CHANGED = 1 << 1; //Has the preset been modified during encoding?

//The item structure
typedef struct FFQ_QUEUE_ITEM
{

    QUEUE_STATUS status; //The status of the item
    wxArrayString inputs; //All the files required for this command
    bool save_log; //Used to determine if a log should be saved for this item
    unsigned short version; //Can be used to specify versions for the items
    QUEUE_FLAG queue_flags; //Used to define flags in the queue (like overwrite allowed etc.)

    FFQ_QUEUE_ITEM();
    FFQ_QUEUE_ITEM(wxString &item);
    FFQ_QUEUE_ITEM(const FFQ_QUEUE_ITEM &copy_item);
    virtual ~FFQ_QUEUE_ITEM();

    bool CheckNotActive(); //Called by editors to prevent an active job from being modified
    virtual void Cleanup(); //Called when the job has been processed
    static FFQ_QUEUE_ITEM* Clone(FFQ_QUEUE_ITEM* item); //Used to clone an item
    virtual wxString DisplayName(); //Gets the display name (or info) of the item
    //virtual TIME_VALUE Duration(uint64_t *frames = NULL) = 0; //Gets the duration of the current command in time or frames
    FFQ_INPUT_FILE& GetInput(size_t index); //Return an input file from "inputs"
    virtual QUEUE_ITEM_TYPE GetItemType() = 0; //Returns the type of item
    virtual bool GetLogFileName(wxString &name); //Return true if log should be saved and sets name accordingly
    bool GetNextCommand(wxString &command); //Called to get the next command for the item
    wxString GetValue(wxString name, wxString default_value = wxEmptyString); //Get value from m_Values
    bool IsActive(); //Return true if status is one of the active ones
    static FFQ_QUEUE_ITEM* Parse(wxString from); //Static function to create items from a string
    virtual void PrepareProcessing(); //Called whenever an item is about to be processed
    void SetValue(wxString name, wxString value); //Set value to m_Values
    virtual wxString ToString(); //Returns a string representation of the item
    virtual bool UsesPreset(wxString pst_id); //Return true if the item uses the preset with the given id

protected:

    bool CpFileTime(void* pst, wxString to_file);
    virtual wxString GetCommandAtIndex(int index) = 0; //Virtual abstract void to get command at index

private:

    int m_CommandIdx; //Index of current command
    FFQ_INPUT_FILE m_TempInput; //Used in GetInput()
    size_t m_TempIndex; //As above
    LPFFQ_NVP_LIST m_Values; //Used to set extra values with SetValue & GetValue

    void Reset(); //Reset vars

} FFQ_QUEUE_ITEM, *LPFFQ_QUEUE_ITEM;

#endif // FFQQUEUEITEM_H
