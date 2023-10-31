/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQStaticJob.h                                                  *
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

#ifndef FFQSTATICJOB_H
#define FFQSTATICJOB_H

#include <wx/string.h>
#include "FFQQueueItem.h"

//The FFQ_JOB structure
typedef struct FFQ_STATIC_JOB : FFQ_QUEUE_ITEM
{

    wxArrayString commands, del_paths;
    wxString save_log_for;

    FFQ_STATIC_JOB();
    FFQ_STATIC_JOB(const FFQ_STATIC_JOB &copy_from);
    FFQ_STATIC_JOB(wxString from);

    virtual QUEUE_ITEM_TYPE GetItemType() { return qtSTATIC_JOB; };
    virtual bool GetLogFileName(wxString &name);
    virtual wxString ToString();

protected:

    virtual wxString GetCommandAtIndex(int index, bool for_encode);

private:

    void Reset();

} FFQ_STATIC_JOB, *LPFFQ_STATIC_JOB;

#endif // FFQSTATICJOB_H
