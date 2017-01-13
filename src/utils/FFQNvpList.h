/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQNvpList.h                                                    *
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

#ifndef FFQNVPLIST_H
#define FFQNVPLIST_H

#include <wx/string.h>

//Structure to hold a name value pair
typedef struct FFQ_NVP_LIST
{

    wxString name, value; //Name and value of the item

    FFQ_NVP_LIST(wxString n, wxString v);
    ~FFQ_NVP_LIST();

    FFQ_NVP_LIST* Append(wxString n, wxString v);
    FFQ_NVP_LIST* Find(wxString s, bool find_value = false, FFQ_NVP_LIST** parent = NULL);
    size_t        GetCount();
    FFQ_NVP_LIST* GetNext();
    void          SetNext(FFQ_NVP_LIST* next);

private:

    FFQ_NVP_LIST* lpNext;


} FFQ_NVP_LIST, *LPFFQ_NVP_LIST;

//Method to create a NVP_LIST
LPFFQ_NVP_LIST NVPListFromLines(wxString &lines, wxString separator = "=", bool allow_duplicates = true);

//Method to make a string from a NVP_LIST
wxString NVPListToLines(LPFFQ_NVP_LIST first, wxString separator = "=", bool pad = false);

//Method to append a NVP
LPFFQ_NVP_LIST NVPListAppend(LPFFQ_NVP_LIST &root, wxString n, wxString v, bool allow_duplicates = true);

//Method to clone a NVP
LPFFQ_NVP_LIST NVPListClone(LPFFQ_NVP_LIST root);

//Method to find parent of a NVP
LPFFQ_NVP_LIST NVPListFindParent(LPFFQ_NVP_LIST root, LPFFQ_NVP_LIST child);

//Method to remove a NVP
bool NVPListRemove(LPFFQ_NVP_LIST &root, wxString n);


#endif // FFQNVPLIST_H
