/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQUniqueID.h                                                   *
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

#ifndef FFQUNIQUEID_H
#define FFQUNIQUEID_H

#include <wx/time.h>
#include <wx/base64.h>
#include <wx/string.h>

const long UNIQUE_ID_NULL = 0;

typedef struct UNIQUE_ID
{

    //The unique values
    wxLongLong id[2];

    //Constructs a new unique id
    UNIQUE_ID() { New(); }

    //Extract id from a string
    UNIQUE_ID(wxString uid) { wxBase64Decode((void*)&id, sizeof(id), uid.c_str().AsChar(), uid.Len()); }

    //Make new ID
    void New() { id[0] = wxGetLocalTimeMillis(); id[1] = wxLongLong(rand(), rand()); }

    //Reset to NULL
    void Reset() { memset(&id, 0, sizeof(id)); }

    //Encode id to string
    wxString ToString() { return wxBase64Encode((void*)&id, sizeof(id)); }


    //Handy operators for comparison and assignment
    bool operator ==(UNIQUE_ID &src) const { return (id[0] == src.id[0]) && (id[1] == src.id[1]); };
    bool operator ==(long src) const { return (id[0] == src) && (id[1] == src); };
    void operator =(const UNIQUE_ID src) { memcpy(&id, &src.id, sizeof(id)); };

} UNIQUE_ID, *LPUNIQUE_ID;

#endif // FFQUNIQUEID_H
