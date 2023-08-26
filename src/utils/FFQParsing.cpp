/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQParsing.cpp***                                               *
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

#include "FFQParsing.h"
#include "FFQConst.h"

/*
    The purpose of these parsers is to extract tokens of sorts
    from a source string without modifying that source string.

    They are in short terms an optimized version of the functions
    FFQMisc::GetLine(..) and FFQMisc::GetToken(..)
*/

const wxString& FFQParser::all()
{

    //Return the internal string
    return m_Str;

}

//---------------------------------------------------------------------------------------

bool FFQParser::done()
{

    //Return true if there are nothing more available
    return m_Pos >= m_Str.Len();

}

//---------------------------------------------------------------------------------------

wxString FFQParser::first()
{

    //Return the first chunk
    m_Index = 0;
    m_Pos = 0;
    return next();

}

//---------------------------------------------------------------------------------------

bool FFQParser::has_more()
{

    //Return true if there is more available
    return m_Pos < m_Str.Len();

}

//---------------------------------------------------------------------------------------

size_t FFQParser::index()
{
    return m_Index;
}

//---------------------------------------------------------------------------------------

wxString FFQParser::last()
{
    return m_Last;
}

//---------------------------------------------------------------------------------------

size_t FFQParser::pos()
{

    //Return the current position
    return m_Pos;

}

//---------------------------------------------------------------------------------------

wxString FFQParser::rest()
{

    //Return whatever has not yet been parsed
    return done() ? wxString() : m_Str.SubString(m_Pos, m_Str.Len());

}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQLineParser::FFQLineParser(const wxString &from_string) : FFQParser(from_string)
{

    //Nothing here, someone must have taken it..

}

//---------------------------------------------------------------------------------------

wxString FFQLineParser::next()
{

    if (done()) return wxEmptyString;

    m_Index++;
    size_t p = m_Pos;
    bool eol = false;
    wxUniChar cc = 0;
    while ((!eol) && (p < m_Str.Len()))
    {
        cc = m_Str.GetChar(p++);
        eol = (cc == CR) || (cc == LF);
    }
    m_Last = eol ? m_Str.SubString(m_Pos, p - 2) : m_Str.SubString(m_Pos, p - 1);
    if ((p < m_Str.Len()) && (cc == CR) && (m_Str.GetChar(p) == LF)) p++;
    m_Pos = p;
    return m_Last;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQTokenParser::FFQTokenParser(const wxString &from_string, const wxString separator) : FFQParser(from_string), m_Sep(separator)
{

    //Is it here then? Nope, sorry..

}

//---------------------------------------------------------------------------------------

wxString FFQTokenParser::next()
{

    if (done()) return wxEmptyString;

    m_Index++;
    size_t p = m_Str.find(m_Sep, m_Pos);
    if (p == wxString::npos) p = m_Str.Len();
    m_Last = m_Str.SubString(m_Pos, p - 1);
    m_Pos = p + m_Sep.Len();
    return m_Last;

}

//---------------------------------------------------------------------------------------
