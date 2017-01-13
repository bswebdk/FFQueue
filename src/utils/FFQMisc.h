/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQMisc.h                                                       *
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

#ifndef FFQMISC_H
#define FFQMISC_H

#include <exception>
#include <wx/string.h>
#include <wx/longlong.h>
#include <wx/sizer.h>
#include <wx/listbox.h>

//Structure to define an FFQueue exception
struct FFQError : std::exception
{

    //The error message
    wxString m_ErrorMessage;

    //Construct
    FFQError(wxString msg) : m_ErrorMessage(msg) {}

    //Throw the error
    ~FFQError() throw() {}

    //Get char representation of the error
    const char* what() const throw() { return m_ErrorMessage.ToUTF8().data(); }

};

//Throw an error
template <typename T> void ThrowError(const T &error_msg)
{
    //Convert argument to string and throw the error
    wxString str = "";
    str << error_msg;
    throw FFQError(str);
}

//Base64 related stuff
size_t Base64DataLen(const wxString &base64);
void Base64Unwrap(wxString &base64);
void Base64Wrap(wxString &base64, size_t wrap_len);
wxString StrFromBase64(wxString base64);
wxString StrToBase64(const wxString &str, size_t wrapLen = 0);

//String conversion
double Str2Float(wxString str, double def = 0);
long Str2Long(wxString str, long def = 0);
long long Str2LongLong(wxString str, long long def = 0);

template <typename T> wxString ToStr(const T &what)
{
    //Convert anything to string
    wxString str;
    str << what;
    return str;
}

//Various string handling
wxString GetLastToken(wxString &from, const wxString delim, bool del_token = true);
wxString GetLine(wxString &from, bool del_line = true);
wxString GetSubLine(wxString &from, size_t &offset);
wxString GetToken(wxString &from, const wxString delim, bool del_token = true);
wxString StrTrim(wxString str);
wxString Unescape(wxString str);

//Time and date related
uint64_t GetTimeTickCount(wxLongLong *tick = NULL);

//Miscellaneous conversion
int ConvertPctToMinMax(float pct, float min, float max, float pct_base = 100);
int froundi(float n);
int64_t droundi(double n);

//Validation of frame rate (might not belong here)
bool IsValidFrameRate(wxString frame_rate);

//Make a log file name for the file (might not belong here)
wxString MakeLogFileName(wxString &for_file);

//UI related function
void EnableSizer(wxSizer *sizer, bool enable, const long *skip = NULL);
void ListBoxMoveSelectedItems(wxListBox *lb, bool up, bool is_check_list_box);
void ListBoxSwapItems(wxListBox *lb, size_t a, size_t b, bool is_check_list_box);

//File handling
bool CopyFileTime(wxString from_path, wxString to_path);
bool CompareFileNames(wxString f1, wxString f2);
bool DeletePaths(wxArrayString *path_list);
bool ForcePath(wxString path);
void EnsureUniquePath(wxString &path);

//Used to yield execution
bool Yield_App(unsigned long sleep_millis = 0);

#endif // FFQMISC_H
