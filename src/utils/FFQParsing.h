/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQParsing.h                                                    *
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

#ifndef FFQPARSING_H
#define FFQPARSING_H

class FFQParser
{
    public:
        FFQParser(const wxString &from_string) : m_Last(""), m_Str(from_string), m_Index(0), m_Pos(0) {};
        const wxString& all();
        bool done();
        wxString first();
        bool has_more();
        size_t index();
        wxString last();
        virtual wxString next() = 0;
        size_t pos();
        wxString rest();

    protected:
        wxString m_Last;
        const wxString &m_Str;
        //wxString m_Res;
        size_t m_Index, m_Pos;
};

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

class FFQLineParser : public FFQParser
{

    public:
        FFQLineParser(const wxString &from_string);
        wxString next();

};

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

class FFQTokenParser : public FFQParser
{

    public:
        FFQTokenParser(const wxString &from_string, const wxString separator);
        wxString next();

    protected:
        const wxString m_Sep;

};

/*class FFQTokenParser
{

    public:
        FFQTokenParser(const wxString &from_string);
        wxString GetFirstToken();
        wxString GetNextToken();
        bool     HasMoreTokens();

    protected:

    private:
        const wxString &m_Str;
        size_t m_Pos;

};*/

#endif // FFQPARSING_H
