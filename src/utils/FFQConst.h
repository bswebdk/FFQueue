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

const wxUniChar CR = '\r';
const wxUniChar LF = '\n';

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
const wxString  STR_YES = "1";
const wxString  STR_NO = "0";
const wxUniChar SPACE = ' ';
const wxUniChar COLON = ':';
const wxUniChar SCOLON = ';';
const wxUniChar COMMA = ',';
const wxUniChar HASH = '#';
const wxUniChar DOT = '.';

#define BOOLSTR(bval) (bval ? STR_YES : STR_NO)
#define STRBOOL(sval) (sval == STR_YES)

inline bool STRBOOLDEF(wxString sval, bool def)
{
    return sval.Len() == 0 ? def : STRBOOL(sval);
}

//Blank chars (trim chars)
const wxString BLANKS = "\r\n\t\f ";

const wxString INT64FMT = "%" + wxString(wxLongLongFmtSpec) + "d";
const wxString UINT64FMT = "%" + wxString(wxLongLongFmtSpec) + "u";

//used to prevent assertions when formatting "size_t" data types
inline unsigned int SIZEFMT(size_t x)
{
    if (x > 0xffffffff) wxMessageBox("size_t shot in the knee!");
    return (unsigned int)x;
}

//---------------------------------------------------------------------------------------

//Number of internal color constants
#define COLOR_COUNT 6

//Standard colors used primarily in the console
extern uint32_t COLOR_BLACK;
extern uint32_t COLOR_BLUE;
extern uint32_t COLOR_GRAY;
extern uint32_t COLOR_GREEN;
extern uint32_t COLOR_ORANGE;
extern uint32_t COLOR_RED;

extern uint32_t DEFAULT_COLORS[];
extern uint32_t DEFAULT_COLORS_DARK[]; //Used when a dark theme is detected

//Used to extract the high order byte from a colors which maps to FFQConfig::colors
#define COLOR_INDEX(c) ((c >> 24) & 0xff)

//---------------------------------------------------------------------------------------

#endif // FFQCONST_H
