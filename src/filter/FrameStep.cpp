/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FrameStep.cpp                                                   *
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

#include "FrameStep.h"

FrameStep::FrameStep(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK);
    iVal.SetRange(0, 0xFFFFFF);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 3, 3, 3);

    MakeLabel(FFQS(SID_FRAMESTEP_SELECT), fgs);
    m_Step = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_Step->SetValidator(iVal);
    fgs->Add(m_Step, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    MakeLabel(FFQS(SID_FRAMESTEP_FRAMES), fgs);

    GetTimeLimitControls(fgs, 0, true);

}

//---------------------------------------------------------------------------------------

FrameStep::~FrameStep()
{

}

//---------------------------------------------------------------------------------------

void FrameStep::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Step->SetValue(GetToken(fs, ','));

    }

    else
    {

        //Default values
        m_Step->SetValue("1");

    }

    SetTimeLimitValues(fs);

}

//---------------------------------------------------------------------------------------

bool FrameStep::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftFRAMESTEP;

    unsigned int step = Str2Long(m_Step->GetValue(), 0);
    if (step <= 0) return ShowError(m_Step, FFQS(SID_FRAMESTEP_ERROR));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;
    wxString tf, ft;
    GetTimeLimitFilter(tf, true);
    GetTimeLimitFriendly(ft);

    fltr->friendly = FFQSF(SID_FRAMESTEP_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], step, ft);
    fltr->ff_filter.Printf("%sframestep=%u%s%s", FILTER_VIDEO_IN, step, tf, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%u," + UINT64FMT + "," + UINT64FMT, step, t1, t2);

    return true;

}
