/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQMisc.cpp                                                     *
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

#include "FFQMisc.h"
#include "FFQConst.h"
#include "../utils/FFQLang.h"
//#include "../FFQApp.h"
#include <wx/app.h>
#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/base64.h>
#include <wx/time.h>
#include <wx/checklst.h>

#ifdef DEBUG
#include "FFQConsole.h"
#endif // DEBUG

//---------------------------------------------------------------------------------------

size_t Base64DataLen(const wxString &base64)
{

    //Returns the actual byte-size of the base64 encoded data
    size_t res = base64.Len();
    while ((res > 0) && (base64.GetChar(res - 1) == '=')) res--;
    return wxBase64DecodedSize(res);

}

//---------------------------------------------------------------------------------------

void Base64Unwrap(wxString &base64)
{

    //Remove any blank chars (line breaks etc) from base64
    size_t i = 0;
    while (i < base64.Len())
    {
        if (BLANKS.Find(base64.GetChar(i)) >= 0) base64.Remove(i, 1);
        else i++;
    }

}

//---------------------------------------------------------------------------------------

void Base64Wrap(wxString &base64, size_t wrap_len)
{

    //Wrap long base64 string into smaller blocks of "wrap_len" width

    wxString s = "";
    while (base64.Len() > wrap_len)
    {

        //Get a chunk
        s += base64.Left(wrap_len) + CRLF;

        //Remove the chunk
        base64.Remove(0, wrap_len);

    }

    //If anything remains (incomplete chunk) add it here
    if (base64.Len() > 0) s+= base64;

    //Set the result to the argument
    base64 = s;

}

//---------------------------------------------------------------------------------------

wxString StrFromBase64(wxString base64)
{

    //Converts a base64 encoded string into an un-encoded string

    //First unwrap - this is not needed when wxBase64DecodeMode_Relaxed is used
    Base64Unwrap(base64);

    //Get length of data
    size_t datalen = Base64DataLen(base64);

    //Make a buffer
    unsigned char* data = new unsigned char[datalen];

    //Decode
    wxBase64Decode((void*)data, datalen, base64/*, wxBase64DecodeMode_Relaxed*/);

    //Handy variable
    wxString res;

    //Check for UTF-8 header
    if ((datalen >= 3) && (memcmp((void*)data, (void*)UTF8_BYTE_ORDER_MARK, sizeof(UTF8_BYTE_ORDER_MARK)) == 0))
    {

        //UTF8 string data
        res = wxString::FromUTF8((char*)&data[3], datalen - 3);

    }
    else
    {

        //Unicode string data
        res = wxString((wchar_t*)data, datalen / sizeof(wchar_t));

    }

    //Release the buffer
    delete[] data;

    //Return the result
    return res;

}

//---------------------------------------------------------------------------------------

wxString StrToBase64(const wxString &str, size_t wrap_len)
{

    //Converts a string to UTF-8 and encodes it to a base64 block

    //Get UTF-8
    wxCharBuffer cb = str.ToUTF8();

    //Make buffer
    unsigned char* buf = new unsigned char[cb.length() + 3];

    //Set UTF-8 header
    buf[0] = UTF8_BYTE_ORDER_MARK[0];
    buf[1] = UTF8_BYTE_ORDER_MARK[1];
    buf[2] = UTF8_BYTE_ORDER_MARK[2];

    //Copy the UTF-8 string data
    memcpy(&buf[3], cb.data(), cb.length());

    //Encode
    wxString res = wxBase64Encode(buf, cb.length() + 3);

    //Release buffer
    delete[] buf;

    //Wrap as needed
    if (wrap_len > 0) Base64Wrap(res, wrap_len);

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

bool IsNumber(wxString value, bool allow_float)
{
    if (value.Len() == 0) return false;
    wxString nums = wxString("+-0123456789") + (allow_float ? "." : "");
    for (size_t i = 0; i < value.Len(); i++) if (nums.Find(value.GetChar(i)) < 0) return false;
    return true;
}

//---------------------------------------------------------------------------------------

double Str2Float(wxString str, double def)
{

    //Convert a string to float (double) and if not
    //successful return the "def" argument
    double res;
    return str.ToDouble(&res) ? res : def;

}

//---------------------------------------------------------------------------------------

long Str2Long(wxString str, long def)
{

    //Same as Str2Float with long
    long res;
    return str.ToLong(&res) ? res : def;

}

//---------------------------------------------------------------------------------------

long long Str2LongLong(wxString str, long long def)
{

    //Convert a long long to string, returning the
    //"def" argument if fail
    if (!IsNumber(str, false)) return def;
    //wxString nums = "+-0123456789";
    //for (unsigned int i = 0; i < str.Len(); i++) if (nums.Find(str.GetChar(i)) < 0) return def;
    return strtol(str.c_str().AsChar(), NULL, 10);

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

bool LogicCompare(wxString value, wxString logic)
{
    //Compare a value against a logic statement which starts with
    // "=": equal, "!": not equal, ">": lesser, "<": greater
    // using ">" and "<" requires numeric values

    //Empty equals empty
    if (logic.Len() == 0) return (value.Len() == 0);

    //Check for valid logic, perform string compare if not found
    if (wxString("!=<>").Find(logic.GetChar(0)) < 0) return value.compare(logic) == 0;

    //Get and remove the logic
    char l;
    if (!logic.GetChar(0).GetAsChar(&l)) return false;
    logic.Remove(0, 1);

    //The seals are howling today, maybe the wind..
    if (IsNumber(value) && IsNumber(logic))
    {

        //Numeric compare
        if ((value.Find('.') < 0) && (logic.Find('.') < 0))
        {

            //Integer
            switch (l)
            {
                case('='): return Str2Long(value) == Str2Long(logic); break;
                case('!'): return Str2Long(value) != Str2Long(logic); break;
                case('<'): return Str2Long(value) < Str2Long(logic); break;
                case('>'): return Str2Long(value) > Str2Long(logic); break;
            }

        }
        else
        {

            //Floating point
            switch (l)
            {
                case('='): return fabs(Str2Float(value) - Str2Float(logic)) < std::numeric_limits<double>::epsilon(); break;
                case('!'): return fabs(Str2Float(value) - Str2Float(logic)) > std::numeric_limits<double>::epsilon(); break;
                case('<'): return Str2Float(value) < Str2Float(logic); break;
                case('>'): return Str2Float(value) > Str2Float(logic); break;
            }

        }

    }
    else
    {

        //String compare
        switch (l)
        {
            case('='): return value.compare(logic) == 0; break;
            case('!'): return value.compare(logic) != 0; break;
            case('<'): return value.compare(logic) < 0; break;
            case('>'): return value.compare(logic) > 0; break;
        }

    }

    //Unable to compare = no match
    return false;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

wxString GetLastToken(wxString &from, const wxString delim, bool del_token)
{

    //Returns the right most piece of the string that comes after "delim"

    //Handy variable
    wxString res = "";

    //Reverse find
    size_t idx = from.rfind(delim);

    if (idx == std::string::npos)
    {

        //Not found, return entire string
        res = from;

        //If del_token then set string to empty
        if (del_token) from = "";

    }
    else
    {

        //Found - get the token
        res = from.SubString(idx + delim.Len(), from.Len());

        //And delete token and delimiter as required
        if (del_token) from.Remove(idx, from.Len() - idx);

    }

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

wxString GetLine(wxString &from, bool del_line)
{

    //Returns whatever is before the first line break char
    //or everything if no line break is present

    //We do not care about empty strings
    if (from.Len() == 0) return from;

    //Handy variable
    wxString res = "";

    for (unsigned int i = 0; i < from.Len(); i++)
    {

        //Get current char
        wxUniChar cc = from.GetChar(i);

        if ((cc == '\n') || (cc == '\r'))
        {
            //Line break character found
            if (del_line)
            {

                //If line must be deleted we check for a double-char
                //line break (CRLF or LFCR)

                unsigned int del = i;
                if (del < from.Len() - 1)
                {
                    if ((cc == '\r') && (from.GetChar(del+1) == '\n')) del++; //CR+LF
                    else if ((cc == '\n') && (from.GetChar(del+1) == '\r')) del++; //LF+CR
                }

                //Delete the line and the break chars
                from = from.Remove(0, del + 1);

            }

            //Return result
            return res;

        } else res += cc; //Add char to result

    }

    //No line breaks found, "res" is now a copy of "from".
    //Set "from" to empty if delete is required
    if (del_line) from = "";

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

wxString GetSubLine(wxString &from, unsigned int &offset)
{

    //Coded by Torben Bruchhaus @ 2014
    //Copies whatever content starts at "offset" and stops
    //at first encountered line break, returning the copied,
    //adjusting "offset" to the beginning of the next line.

    //We do not care about empty strings
    if (from.Len() == 0) return from;

    //Handy variable
    wxString res;

    for (unsigned int i = offset; i < from.Len(); i++)
    {

        //Get current char
        wxUniChar cc = from.GetChar(i);

        if ((cc == '\n') || (cc == '\r'))
        {

            //Line break character found - adjust the offset to
            //whatever comes after "\n" or "\r" or "\r\n" or "\n\r"

            offset = i + 1;

            if (offset < from.Len() - 1)
            {

                if ((cc == '\r') && (from.GetChar(offset) == '\n')) offset++; //CR+LF
                else if ((cc == '\n') && (from.GetChar(offset) == '\r')) offset++; //LF+CR

            }

            //Return sub line
            return res;

        } else res << cc; //Add char to result

    }

    //No line breaks found, "res" is now a copy of "from".
    //Set "offset" to length of "from"
    offset = from.Len();

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

wxString GetToken(wxString &from, const wxString delim, bool del_token)
{

    //Returns the left most piece of the string that comes before "delim"

    //Handy variable
    wxString res = "";

    //Find the delimiter
    int idx = from.Find(delim);

    if (idx == wxNOT_FOUND)
    {

        //Not found - return everything
        res = from;

        //Set "from" to empty if delete is required
        if (del_token) from = "";

    }
    else
    {

        //Delimiter found, get substring
        res = from.SubString(0, idx-1);

        //And delete if required
        if (del_token) from.Remove(0, idx + delim.Len());

    }

    //Return the result
    return res;

}

//---------------------------------------------------------------------------------------

bool StrSplit(wxString &value, wxString &key, wxUniChar separator, bool trim)
{
    if (value.Find(separator) < 0) return false;
    if (trim)
    {
        key = StrTrim(value.BeforeFirst(separator));
        value = StrTrim(value.AfterFirst(separator));
    }
    else
    {
        key = value.BeforeFirst(separator);
        value = value.AfterFirst(separator);
    }
    return true;

}


//---------------------------------------------------------------------------------------
wxString StrTrim(wxString str)
{

    //Trims blanks chars from both ends of "str" and returns the result
    //This does the same as str.Trim().Trim(false);
    int s = 0, e = str.Len() - 1;
    while ((s < e) && (BLANKS.Find(str.GetChar(s)) >= 0)) s++;
    while ((e >= s) && (BLANKS.Find(str.GetChar(e)) >= 0)) e--;
    if (s > e) return "";
    return str.SubString(s, e);

}

//---------------------------------------------------------------------------------------

wxString Unescape(wxString str)
{

    //Un-escape string
    wxString res;
    unsigned int i = 0;
    while (i < str.Len())
    {
        if (str.at(i) == '\\') i++;
        res += str.at(i);
        i++;
    }
    return res;

}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

uint64_t GetTimeTickCount(wxLongLong *tick)
{

    //Get time milliseconds as an unsigned int64 or
    //converts a wxLongLong.. Huh??!?
    wxLongLong ll = (tick == NULL) ? wxGetLocalTimeMillis() : *tick;
    uint64_t res = ll.GetHi();
    res = (res << 32) | ll.GetLo();
    return res;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

float ConvertPctToMinMaxFloat(float pct, float min, float max, float pct_base)
{

    //Maps a percentage / fraction into a range defined by min..max
    return min + ((max - min) / pct_base * pct);

}

//---------------------------------------------------------------------------------------

int ConvertPctToMinMaxInt(float pct, float min, float max, float pct_base)
{

    //Integer version of above
    return froundi(ConvertPctToMinMaxFloat(pct, min, max, pct_base));

}

//---------------------------------------------------------------------------------------

int froundi(float n)
{

	//Simple round function
	return n < 0.0f ? (int)ceil(n - 0.5f) : (int)floor(n + 0.5f);

}

//---------------------------------------------------------------------------------------

int64_t droundi(double n)
{

	//As above
	return n < 0.0f ? (int64_t)ceil(n - 0.5f) : (int64_t)floor(n + 0.5f);

}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

bool IsValidFrameRate(wxString frame_rate)
{

    //Validates a string that represents a frame rate
    //that can either be an integer, a float or a
    //nominator / denominator representation

    //Empty value always true
    if (frame_rate.Len() == 0) return true;

    if (frame_rate.Find('/') < 0)
    {

        if (frame_rate.Find('.') < 0)
        {

            //Integer validation
            return Str2Long(frame_rate, -1) >= 0;

        }

        //Float validation
        return Str2Float(frame_rate, -1) >= 0;

    }

    //Nominator / denominator validation
    return (Str2Long(frame_rate.BeforeFirst('/'), 0) > 0) && (Str2Long(frame_rate.AfterFirst('/'), 0) > 0);

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

wxString MakeLogFileName(wxString &forFile)
{

    //Make a unique name for the log file
    wxString res = forFile.BeforeLast('.'), ext = "_log.htm";
    if (wxFileExists(res + ext))
    {
        int i = 0;
        do
        {
            i++;
            ext.Printf("_log_%i.htm", i);
        } while (wxFileExists(res + ext));
    }

    return res + ext;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void EnableSizer(wxSizer *sizer, bool enable, const long *skip, const unsigned int skip_count)
{

    //Enables or disables all controls or sub-sizers in a sizer
    //except the ones that are defined by the array of control
    //id's "skip"

    for (unsigned int i = 0; i < sizer->GetItemCount(); i++)
    {
        wxSizerItem* cur = sizer->GetItem(i);
        wxWindow* ctrl = cur->GetWindow();
        if (ctrl == NULL)
        {
            wxSizer *sub = cur->GetSizer();
            if (sub != NULL) EnableSizer(sub, enable);
        }
        else if (skip == NULL) ctrl->Enable(enable);
        else
        {
            bool ok = true;
            for (unsigned int ii = 0; (ii < skip_count) && ok; ii++) ok = (ctrl->GetId() != skip[ii]);
            if (ok) ctrl->Enable(enable);
        }
    }
}

//---------------------------------------------------------------------------------------

void ListBoxMoveSelectedItems(wxListBox *lb, bool up, bool is_check_list_box)
{

    //Moves the selected items in the list box a notch up or down
    //minding the client data and eventually the state of the check box

    lb->Freeze();
    if (up)
    {
        for (unsigned int i = 0; i < lb->GetCount(); i++)
        {
            if (lb->IsSelected(i)) ListBoxSwapItems(lb, i, i-1, is_check_list_box);
        }
    }
    else
    {
        for (unsigned int i = lb->GetCount() - 1; i < lb->GetCount(); i--)
        {
            if (lb->IsSelected(i)) ListBoxSwapItems(lb, i, i+1, is_check_list_box);
        }

    }
    lb->Thaw();

}

//---------------------------------------------------------------------------------------

void ListBoxSwapItems(wxListBox *lb, unsigned int a, unsigned int b, bool is_check_list_box)
{

    //Swaps two items in a (check) list box

    //Get list box values
    void *data = lb->HasClientData() ? lb->GetClientData(a) : NULL;
    wxString str = lb->GetString(a);
    bool sel = lb->IsSelected(a);

    //Freeze and swap
    lb->Freeze();
    lb->SetString(a, lb->GetString(b));
    lb->SetString(b, str);
    if (lb->HasClientData())
    {
        lb->SetClientData(a, lb->GetClientData(b));
        lb->SetClientData(b, data);
    }
    lb->SetSelection(a, lb->IsSelected(b));
    lb->SetSelection(b, sel);
    if (is_check_list_box)
    {
        //If check list box swap checks
        bool chk = ((wxCheckListBox*)lb)->IsChecked(a);
        ((wxCheckListBox*)lb)->Check(a, ((wxCheckListBox*)lb)->IsChecked(b));
        ((wxCheckListBox*)lb)->Check(b, chk);
    }

    //Thaw and thats it :)
    lb->SetFocus();
    lb->Thaw();

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

bool CopyFileTime(wxString from_path, wxString to_path)
{

    //Copy the file time(s) from one file/path to another

    wxDateTime acc, mod, cr;
    wxFileName srcf(from_path), dstf(to_path);

    if (srcf.Exists() && srcf.GetTimes(&acc, &mod, &cr))
    {

        wxFileName dstf(to_path);
        return dstf.Exists() && dstf.SetTimes(&acc, &mod, &cr);

    }

    return false;

}

//---------------------------------------------------------------------------------------

bool CompareFileNames(wxString f1, wxString f2)
{

    wxFileName fn(f1);
    return fn.SameAs(f2);

}

//---------------------------------------------------------------------------------------

bool DeletePaths(wxArrayString *path_list)
{

    //Recursively delete all files and folders in the list

    if (path_list->Count() == 0) return true;

    bool ok = true;

    for (unsigned int i = 0; i < path_list->Count(); i++)
    {

        wxString cur = path_list->Item(i);

        if (wxDirExists(cur))
        {

            wxArrayString *sub_items = new wxArrayString();
            wxDir::GetAllFiles(cur, sub_items);
            ok = ok && DeletePaths(sub_items);
            delete sub_items;
            ok = ok && wxFileName::Rmdir(cur);

        }

        else if (wxFileExists(cur)) ok = ok && wxRemoveFile(cur);

    }

    return ok;

}

//---------------------------------------------------------------------------------------

bool ForcePath(wxString path)
{

    //Creates the entire folder tree
    wxString mkd, psep = wxFileName::GetPathSeparator();

    //Remove trailing psep
    if (path.Right(psep.Len()) == psep) path.RemoveLast(psep.Len());

    //Find first existing folder
    while (!wxDirExists(path))
    {

        if (mkd.Len() == 0) mkd = GetLastToken(path, psep, true);
        else mkd = GetLastToken(path, psep, true) + psep + mkd;

    }

    //Create the non-existing sub-folders
    while (mkd.Len() > 0)
    {

        //Append dir to create
        path += psep + GetToken(mkd, psep, true);

        //Create dir (return value might be false even if success)
        wxMkDir(path, wxS_DIR_DEFAULT);

        //Check for failure
        if (!wxDirExists(path)) return false;

    }

    //Success
    return true;

}

//---------------------------------------------------------------------------------------

#define PATH_NUM_FMT(num) ( wxString::Format("_%i", ++num) )

//---------------------------------------------------------------------------------------

void EnsureUniquePath(wxString &path)
{

    //This function adds _%i to a path until is does not exist

    int cc = 0;
    wxString s;

    if (wxDirExists(path))
    {

        //For directory

        do
        {

            s = path + PATH_NUM_FMT(cc);

        } while (wxDirExists(s));

        path = s;

    }

    else if (wxFileExists(path))
    {

        //For file (before extension)
        int i = path.Last('.'); //Insert pos
        if (i < path.Last(wxFileName::GetPathSeparator())) i = -1; //Append

        do
        {

            if (i < 0) s = path + PATH_NUM_FMT(cc);
            else
            {
                s = path;
                s.insert(i, PATH_NUM_FMT(cc));
            }

        } while (wxFileExists(s));

        path = s;

    }

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

//Local variable to make sure that yield calls are not nested (which would cause a deadlock)
bool m_Yielding = false;
bool Yield_App(unsigned long sleep_millis)
{

    //Sleep a bit if tired
    if (sleep_millis > 0) wxMilliSleep(sleep_millis);


    if (m_Yielding)
    {

        //If already yielding we return false - this should never happen!

        #ifdef DEBUG
        ShowError(NULL, "Already yielding");
        #endif // DEBUG

        return false;

    }

    //Set yielding state
    m_Yielding = true;

    try
    {
        //Yield execution
        wxSafeYield();
        //wxGetApp().Yield();

    } catch (...) {}

    //Set yielding state
    m_Yielding = false;

    //Return success
    return true;

}
