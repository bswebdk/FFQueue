/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQTimeValue.h                                                  *
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

#ifndef FFQTIMEVALUE_H
#define FFQTIMEVALUE_H

#include <wx/string.h>
#include "FFQConst.h"

//Used to declare an invalid time component
const unsigned short BAD_TIME_COMP = 0xFFFF;

//The TIME_VALUE structure
typedef struct TIME_VALUE
{

    unsigned short hours, minutes, seconds, milliseconds; //Obvious!

    //Constructors
    TIME_VALUE();
    TIME_VALUE(unsigned short hh, unsigned short mm, unsigned short ss, unsigned short ms);
    TIME_VALUE(wxString time_value);
    TIME_VALUE(uint64_t time_value);

    //Validators
    bool IsUndefined();
    bool IsValid();

    //Setters
    void SetMilliseconds(uint64_t millis);

    //Converters
    double   ToDouble();
    wxString ToReadableString();
    wxString ToShortString();
    wxString ToString(bool ZeroPadding = true, bool AppendMillis = true, wxUniChar MillisSep = '.');
    uint64_t ToMilliseconds();

    //Handy operators
    const bool operator==(TIME_VALUE &tv) { return ToMilliseconds() == tv.ToMilliseconds(); }
    const bool operator==(const uint64_t tv) { return ToMilliseconds() == tv; }
    const bool operator!=(TIME_VALUE &tv) { return ToMilliseconds() != tv.ToMilliseconds(); }
    const bool operator!=(const uint64_t tv) { return ToMilliseconds() != tv; }

} TIME_VALUE, *LPTIME_VALUE;

#endif // FFQTIMEVALUE_H
