/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Rotate.cpp                                                      *
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

#include "Rotate.h"

Rotate::Rotate(wxWindow* parent) : FilterBasePanel(parent)
{

    SetControlPadding(5);

    wxFloatingPointValidator<float> fVal;
    fVal.SetRange(0, 360);

    wxFlexGridSizer *fgs1 = new wxFlexGridSizer(5, 2, 0, 0);
    fgs1->AddGrowableCol(1);

    MakeLabel(FFQS(SID_ROTATE_FILL_COLOR), fgs1);
    m_FillColor = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_FillColor, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);

    MakeLabel(FFQS(SID_ROTATE_ROTATE_ANGLE), fgs1);
    m_Angle = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), 0);
    m_Angle->SetValidator(fVal);
    fgs1->Add(m_Angle, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    fgs1->Add(-1, -1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    m_Bilinear = new wxCheckBox(this, wxID_ANY, FFQS(SID_ROTATE_BILINEAR_INTERPOLATE), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_Bilinear, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    fgs1->Add(-1, -1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    m_CounterClock = new wxCheckBox(this, wxID_ANY, FFQS(SID_ROTATE_ROTATE_CCW), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_CounterClock, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    MakeLabel(FFQS(SID_FILTER_TIME_LIMIT), fgs1);
    fgs1->Add(GetTimeLimitControls(), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    SetSizer(fgs1);
    fgs1->Fit(this);
    fgs1->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Rotate::~Rotate()
{

}

//---------------------------------------------------------------------------------------

void Rotate::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Angle->SetValue(GetToken(fs, ','));
        m_FillColor->SetColour(wxColour(GetToken(fs, ',')));
        m_Bilinear->SetValue(GetToken(fs, ',') == STR_YES);
        m_CounterClock->SetValue(GetToken(fs, ',') == STR_YES);

    }

    else
    {

        //Default values
        m_Angle->SetValue("45");
        m_FillColor->SetColour(*wxBLACK);
        m_Bilinear->SetValue(true);
        m_CounterClock->SetValue(false);

    }

    SetTimeLimitValues(fs, true);

}

//---------------------------------------------------------------------------------------

bool Rotate::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftROTATE;

    double a, ra;
    if (!m_Angle->GetValue().ToDouble(&a) || (a == 0)) return ShowError(m_Angle, FFQS(SID_ROTATE_ROTATE_ANGLE_ERROR));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2, true)) return false;

    wxString c = m_FillColor->GetColour().GetAsString(wxC2S_HTML_SYNTAX),
             b = BOOLSTR(m_Bilinear->GetValue()),
             cc = BOOLSTR(m_CounterClock->GetValue()),
             ft, tfltr;

    GetTimeLimitFriendly(ft);
    GetTimeLimitFilter(tfltr, true);

    if (m_CounterClock->GetValue()) a = -a;
    ra = a * (3.14159265358979323846 / 180.0);

    fltr->friendly = FFQSF(SID_ROTATE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], a, c, b, ft);

    fltr->ff_filter.Printf("%srotate=a=%f:c=%s:bilinear=%s%s%s", FILTER_VIDEO_IN, ra, c, b, tfltr, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%s,%s,%s,%s," + UINT64FMT + "," + UINT64FMT, m_Angle->GetValue(), c, b, cc, t1, t2);

    return true;

}
