/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQTimeValue.cpp                                                *
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

#include "FFQTimeValue.h"
#include "FFQMisc.h"
#include "../utils/FFQLang.h"
#include "../utils/FFQConst.h"

TIME_VALUE::TIME_VALUE()
{

    //Default constructor
    hours = 0;
    minutes = 0;
    seconds = 0;
    milliseconds = 0;

}

//---------------------------------------------------------------------------------------

TIME_VALUE::TIME_VALUE(unsigned short hh, unsigned short mm, unsigned short ss, unsigned short ms)
{

    //Parametric constructor
    hours = hh;
    minutes = mm;
    seconds = ss;
    milliseconds = ms;

}

//---------------------------------------------------------------------------------------

TIME_VALUE::TIME_VALUE(wxString time_value)
{

    //String parsing constructor

    //Get hh:mm:ss
    wxString s = GetToken(time_value, '.');

    //Sometimes millis are longer than 3 digits and this must be fixed to prevent "spill over" in seconds
    if (time_value.Len() > 3) time_value = time_value.Left(3);

    //Parse values from string
    milliseconds = (time_value.Len() == 0) ? 0 : Str2Long(time_value, BAD_TIME_COMP);
    seconds = (s.Len() == 0) ? 0 : Str2Long(GetLastToken(s, ':'), BAD_TIME_COMP);
    minutes = (s.Len() == 0) ? 0 : Str2Long(GetLastToken(s, ':'), BAD_TIME_COMP);
    hours = (s.Len() == 0) ? 0 : Str2Long(GetLastToken(s, ':'), BAD_TIME_COMP);

    //Make sure that .65 not turns into .065
    if (time_value.Len() == 1) milliseconds *= 100;
    else if (time_value.Len() == 2) milliseconds *= 10;

    //Since it is possible that seconds/minutes are above 60
    //we validate by encoding and decoding in millis
    SetMilliseconds(ToMilliseconds());

}

//---------------------------------------------------------------------------------------

TIME_VALUE::TIME_VALUE(uint64_t time_value)
{

    //Millisecond extracting constructor
    SetMilliseconds(time_value);

}

//---------------------------------------------------------------------------------------

bool TIME_VALUE::IsUndefined()
{

    //Check if any values has been defined
    return (hours + minutes + seconds + milliseconds) == 0;

}

//---------------------------------------------------------------------------------------

bool TIME_VALUE::IsValid()
{

    //Check if any of the values are invalid (usually used after parsing constructor)
    return (hours != BAD_TIME_COMP) && (minutes != BAD_TIME_COMP) && (seconds != BAD_TIME_COMP) && (milliseconds != BAD_TIME_COMP);

}

//---------------------------------------------------------------------------------------

void TIME_VALUE::SetMilliseconds(uint64_t millis)
{
    milliseconds = millis % 1000;
    millis /= 1000;
    seconds = millis % 60;
    millis /= 60;
    minutes = millis % 60;
    millis /= 60;
    hours = millis;
}

//---------------------------------------------------------------------------------------

double TIME_VALUE::ToDouble()
{

    //Return milliseconds as double
    return (double)ToMilliseconds();

}

//---------------------------------------------------------------------------------------

wxString TIME_VALUE::ToReadableString()
{

    //Convert the time value to a human readable string using language strings

    wxString res;

    if (hours > 0)
    {

        //Hours and minutes
        res = wxString::Format(FFQL()->TIME_VALUE_NAMES[(hours > 1) ? 5 : 4], hours) + " " + FFQS(SID_AND) + " ";
        res += wxString::Format(FFQL()->TIME_VALUE_NAMES[(minutes > 1) ? 3 : 2], minutes);

    }

    else if (minutes > 0)
    {

        //Minutes and seconds
        res = wxString::Format(FFQL()->TIME_VALUE_NAMES[(minutes > 1) ? 3 : 2], minutes) + " " + FFQS(SID_AND) + " ";
        res += wxString::Format(FFQL()->TIME_VALUE_NAMES[(seconds > 1) ? 1 : 0], seconds);

    }

    //Only seconds
    else res = wxString::Format(FFQL()->TIME_VALUE_NAMES[(seconds > 1) ? 1 : 0], seconds);

    //Return the mess..
    return res;

}

//---------------------------------------------------------------------------------------

wxString TIME_VALUE::ToShortString()
{

    //Return the time value as a short time representation
    //in the format [seconds].[millis]
    uint64_t ms = ToMilliseconds(), secs = ms / 1000;
    ms = ms % 1000;
    return wxString::Format(UINT64FMT + ".%03u", secs, (unsigned int)ms);

}

//---------------------------------------------------------------------------------------

wxString TIME_VALUE::ToString(bool ZeroPadding, bool AppendMillis, wxUniChar MillisSep)
{

    //Convert the time value to a FF-compatible string
    wxString res = ZeroPadding ? "%02u:%02u:%02u.%03u" : "%u:%u:%u.%u";
    res.Printf(res, (unsigned int)hours, (unsigned int)minutes, (unsigned int)seconds, (unsigned int)milliseconds);
    if (!AppendMillis) return res.BeforeLast('.');
    if (MillisSep != '.') res.Replace('.', MillisSep);
    return res;

}

//---------------------------------------------------------------------------------------

uint64_t TIME_VALUE::ToMilliseconds()
{

    //Return milliseconds as unsigned 64bit integer
    uint64_t res = hours * 60;
    res = (res + minutes) * 60;
    return ((res + seconds) * 1000) + milliseconds;

}
