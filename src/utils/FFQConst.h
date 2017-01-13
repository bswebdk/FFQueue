/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConst.h                                                      *
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

#ifndef FFQCONST_H
#define FFQCONST_H

#include <wx/string.h>


#ifdef __WINDOWS__
const wxString CRLF = "\r\n";
const wxString NULL_FILENAME = "NUL";
#else
const wxString CRLF = "\n";
const wxString NULL_FILENAME = "/dev/null";
#endif

//UTF-8 file header
const char UTF8_BYTE_ORDER_MARK[] = {'\xEF', '\xBB', '\xBF'};

//Convert bool to string
const wxString STR_YES = "1";
const wxString STR_NO = "0";

#define BOOLSTR(bval) (bval ? STR_YES : STR_NO)
#define STRBOOL(sval) (sval == STR_YES)

bool STRBOOLDEF(wxString sval, bool def);

//Blank chars (trim chars)
const wxString BLANKS = "\r\n\t\f ";

const wxString INT64FMT = "%" + wxString(wxLongLongFmtSpec) + "d";
const wxString UINT64FMT = "%" + wxString(wxLongLongFmtSpec) + "u";

#endif // FFQCONST_H
