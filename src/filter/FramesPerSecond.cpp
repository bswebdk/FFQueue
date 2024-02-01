/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FramesPerSecond.cpp                                             *
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

#include "FramesPerSecond.h"
#include <wx/valnum.h>

const int ROUND_METHOD_COUNT = 5;
const wxString ROUND_METHODS[ROUND_METHOD_COUNT] = { "near", "zero", "inf", "down", "up" };

const int EOF_ACTION_COUNT = 2;
const wxString EOF_ACTIONS[EOF_ACTION_COUNT] = { "round", "pass" };

//---------------------------------------------------------------------------------------

FramesPerSecond::FramesPerSecond(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFloatingPointValidator<float> fpval(6, 0, wxNUM_VAL_DEFAULT);// wxNUM_VAL_ZERO_AS_BLANK);

    m_Sizer = new wxFlexGridSizer(4, 2, 0, 0);
    m_Sizer->AddGrowableCol(1);

    MakeLabel(FFQS(SID_FRAMESPERSECOND_FPS), m_Sizer);
    m_FPS = new wxTextCtrl(this, wxID_ANY);
    //m_FPS->SetValidator(fpval);
    m_Sizer->Add(m_FPS, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_FRAMESPERSECOND_STARTTIME), m_Sizer);
    m_Start = new wxTextCtrl(this, wxID_ANY);
    m_Start->SetValidator(fpval);
    m_Sizer->Add(m_Start, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_FRAMESPERSECOND_RND_METHOD), m_Sizer);
    m_Round = new wxChoice(this, 1, wxDefaultPosition, wxDefaultSize);
    wxString* sl = FFQL()->GetStringArray(SID_FRAMESPERSECOND_RND_METHODS, ROUND_METHOD_COUNT);
    m_Round->Set(ROUND_METHOD_COUNT, sl);
    delete[] sl;
    m_Sizer->Add(m_Round, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_FRAMESPERSECOND_EOF_ACTION), m_Sizer);
    m_EOF = new wxChoice(this, 1, wxDefaultPosition, wxDefaultSize);
    sl = FFQL()->GetStringArray(SID_FRAMESPERSECOND_EOF_ACTIONS, EOF_ACTION_COUNT);
    m_EOF->Set(EOF_ACTION_COUNT, sl);
    delete[] sl;
    m_Sizer->Add(m_EOF, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(m_Sizer);
    m_Sizer->Fit(this);
    //m_Sizer->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

FramesPerSecond::~FramesPerSecond()
{

}

//---------------------------------------------------------------------------------------

void FramesPerSecond::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Start->SetValue(GetToken(fs, COLON));
        m_Round->SetSelection(Str2Long(GetToken(fs, COLON), 0));
        m_EOF->SetSelection(Str2Long(GetToken(fs, COLON), 0));
        m_FPS->SetValue(fs);

    }

    else
    {

        //Default values
        m_FPS->SetValue("25");
        m_Start->SetValue("0");
        m_Round->SetSelection(0);
        m_EOF->SetSelection(0);

    }

}

//---------------------------------------------------------------------------------------

bool FramesPerSecond::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftFRAMESPERSECOND;

    wxString fps = StrTrim(m_FPS->GetValue());
    if (fps.Len() == 0) return ShowError(m_FPS, FFQSF(SID_FULLSPEC_REQUIRED_FIELD, FFQL()->FILTER_NAMES[fltr->type]));

    wxString ff = fps, start = m_Start->GetValue(), round = ROUND_METHODS[m_Round->GetSelection()], eof = EOF_ACTIONS[m_EOF->GetSelection()];
    if (start.Len() == 0) start = "0";

    fltr->friendly = FFQSF(SID_FRAMESPERSECOND_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], fps, start, round, eof);

    if ((start.Len() > 0) && (start != "0")) ff += ":start_time=" + start;
    if (m_Round->GetSelection() > 0) ff += ":round=" + round;
    if (m_EOF->GetSelection() > 0) ff += ":eof_action=" + eof;
    fltr->ff_filter.Printf("%sfps=%s%s", FILTER_VIDEO_IN, ff, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%s:%i:%i:%s", start, m_Round->GetSelection(), m_EOF->GetSelection(), fps);

    return true;

}

//---------------------------------------------------------------------------------------

