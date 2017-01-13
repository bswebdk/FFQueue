/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQNvpList.cpp                                                  *
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

#include "FFQNvpList.h"
#include "FFQMisc.h"
#include "FFQConst.h"

//---------------------------------------------------------------------------------------

FFQ_NVP_LIST::FFQ_NVP_LIST(wxString n, wxString v)
{

    //Create a name value pair
    name = n;
    value = v;
    lpNext = NULL;

}

//---------------------------------------------------------------------------------------

FFQ_NVP_LIST::~FFQ_NVP_LIST()
{

    //Destroy next in list and nullify
    if (lpNext) delete lpNext;
    lpNext = NULL;

}

//---------------------------------------------------------------------------------------

FFQ_NVP_LIST* FFQ_NVP_LIST::Append(wxString n, wxString v)
{

    //Create a name value pair and return it
    if (lpNext == NULL) lpNext = new FFQ_NVP_LIST(n, v);
    else return lpNext->Append(n, v);
    return lpNext;

}

//---------------------------------------------------------------------------------------

FFQ_NVP_LIST* FFQ_NVP_LIST::Find(wxString s, bool find_value, FFQ_NVP_LIST** parent)
{

    //Find the name or value
    if (s == (find_value ? value : name)) return this;
    if (parent) *parent = this;
    if (lpNext) return lpNext->Find(s, find_value, parent);
    return NULL;

}

//---------------------------------------------------------------------------------------

size_t FFQ_NVP_LIST::GetCount()
{

    //Returns the number of items in the list
    return (lpNext == NULL) ? 1 : lpNext->GetCount() + 1;

}

//---------------------------------------------------------------------------------------

FFQ_NVP_LIST* FFQ_NVP_LIST::GetNext()
{

    //Return next item in list
    return lpNext;

}

//---------------------------------------------------------------------------------------

void FFQ_NVP_LIST::SetNext(FFQ_NVP_LIST* next)
{

    //Set lpNext to "next"
    lpNext = next;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

LPFFQ_NVP_LIST NVPListFromLines(wxString &lines, wxString separator, bool allow_duplicates)
{

    //Creates a list of NVP's from a string with multiple lines
    //of: name + separator + value

    LPFFQ_NVP_LIST res = NULL;
    wxString n, v;

    while (lines.Len() > 0)
    {

        //Get and remove a line
        v = GetLine(lines, true);

        //Split into name and value
        n = GetToken(v, separator, true);

        //Append to result
        NVPListAppend(res, n, v, allow_duplicates);

    }

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

wxString NVPListToLines(LPFFQ_NVP_LIST first, wxString separator, bool pad)
{

    size_t namew = 0;
    wxString res, nm;
    LPFFQ_NVP_LIST ptr = first;

    if (pad)
    {

        //Find the widest name for padding
        while (ptr)
        {

            if (ptr->name.Len() > namew) namew = ptr->name.Len();
            ptr = ptr->GetNext();

        }

    }

    ptr = first;
    while (ptr)
    {

        //Get name
        nm = ptr->name;

        //Pad as needed
        if (pad) nm.Pad(namew - nm.Len());

        //Append
        res << nm << separator << ptr->value << CRLF;

        //Go to next
        ptr = ptr->GetNext();

    }

    //Return it
    return res;

}

//---------------------------------------------------------------------------------------

LPFFQ_NVP_LIST NVPListAppend(LPFFQ_NVP_LIST &root, wxString n, wxString v, bool allow_duplicates)
{

    //Append method - just to avoid repeating code
    if (root)
    {

        if (!allow_duplicates)
        {

            //Make sure "n" is unique
            LPFFQ_NVP_LIST dup = root->Find(n);
            if (dup)
            {

                //Found, update value
                dup->value = v;
                return dup;

            }

        }

        return root->Append(n, v);

    }

    root = new FFQ_NVP_LIST(n, v);
    return root;

}

//---------------------------------------------------------------------------------------

LPFFQ_NVP_LIST NVPListClone(LPFFQ_NVP_LIST root)
{

    //Clones the entire NVP list to another pointer

    LPFFQ_NVP_LIST res = NULL, cur, add;

    while (root)
    {

        //Following line is less efficient and will not allow any duplicates
        //NVPListAppend(res, root->name, root->value);

        //Clone
        add = new FFQ_NVP_LIST(root->name, root->value);

        if (res == NULL)
        {

            //Set as root
            res = add;
            cur = add;

        }
        else
        {

            //Append to cur
            cur->SetNext(add);
            cur = add;

        }

        //Next to clone
        root = root->GetNext();

    }

    //Return the result
    return res;

}

//---------------------------------------------------------------------------------------

LPFFQ_NVP_LIST NVPListFindParent(LPFFQ_NVP_LIST root, LPFFQ_NVP_LIST child)
{

    //Find the parent of a NVP
    while (root)
    {

        if (root->GetNext() == child) return root;
        root = root->GetNext();

    }

    //Not found
    return NULL;
}

//---------------------------------------------------------------------------------------

bool NVPListRemove(LPFFQ_NVP_LIST &root, wxString n)
{

    //Removes an item from a NVP list if it exists
    if (root)
    {

        LPFFQ_NVP_LIST pnt = NULL, nvp = root->Find(n, false, &pnt);

        if (nvp)
        {

            //Item found

            //Root must be set to next
            if (nvp == root) root = root->GetNext();

            //A link in the chain must be removed
            else pnt->SetNext(nvp->GetNext());

            //Clear next to avoid chain reaction by deletion
            nvp->SetNext(NULL);

            //Delete the item & return success
            delete nvp;
            return true;

        }

    }

    //Nothing removed
    return false;

}

//---------------------------------------------------------------------------------------
