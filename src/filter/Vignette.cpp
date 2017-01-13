/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Vignette.cpp                                                    *
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

#include "Vignette.h"

Vignette::Vignette(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFloatingPointValidator<float> fVal;
    fVal.SetRange(0, 180);

    wxIntegerValidator<unsigned int> iVal;
    fVal.SetRange(0, 99999);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(7, 2, 0, 0);
    wxBoxSizer *bs;
    fgs->AddGrowableCol(1);

    MakeLabel(FFQS(SID_VIGNETTE_ANGLE), fgs);
    m_Angle = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), 0);
    m_Angle->SetValidator(fVal);
    fgs->Add(m_Angle, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    MakeLabel(FFQS(SID_VIGNETTE_ORIGIN), fgs);

        bs = new wxBoxSizer(wxVERTICAL);
        m_Centered = new wxCheckBox(this, wxID_ANY, FFQS(SID_VIGNETTE_CALC_CENTER_COORDS));
        bs->Add(m_Centered, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
        bs->Add(GetLeftAndTopControls(iVal), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(bs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_Forward = new wxCheckBox(this, wxID_ANY, FFQS(SID_VIGNETTE_FORWARD));
    fgs->Add(m_Forward, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_PerFrame = new wxCheckBox(this, wxID_ANY, FFQS(SID_VIGNETTE_CALC_EACH_FRAME));
    fgs->Add(m_PerFrame, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_Flicker = new wxCheckBox(this, wxID_ANY, FFQS(SID_VIGNETTE_APPLY_FLICKER));
    fgs->Add(m_Flicker, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_Dither = new wxCheckBox(this, wxID_ANY, FFQS(SID_VIGNETTE_REDUCE_BANDING));
    fgs->Add(m_Dither, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_FILTER_TIME_LIMIT), fgs);
    fgs->Add(GetTimeLimitControls(), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Vignette::~Vignette()
{

}

//---------------------------------------------------------------------------------------

void Vignette::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Angle->SetValue(GetToken(fs, ','));
        m_Centered->SetValue(GetToken(fs, ',') == STR_YES);
        SetLeftAndTopValues(fs);
        m_Forward->SetValue(GetToken(fs, ',') == STR_YES);
        m_PerFrame->SetValue(GetToken(fs, ',') == STR_YES);
        m_Flicker->SetValue(GetToken(fs, ',') == STR_YES);
        m_Dither->SetValue(GetToken(fs, ',') == STR_YES);

    }

    else
    {

        //Default values
        m_Angle->SetValue("45");
        m_Centered->SetValue(true);
        SetLeftAndTopValues(0, 0);
        m_Forward->SetValue(true);
        m_PerFrame->SetValue(true);
        m_Flicker->SetValue(false);
        m_Dither->SetValue(true);

    }

    SetTimeLimitValues(fs, true);

}

//---------------------------------------------------------------------------------------

bool Vignette::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftVIGNETTE;

    double a, ra;
    if (!m_Angle->GetValue().ToDouble(&a) || (a == 0)) return ShowError(m_Angle, FFQS(SID_VIGNETTE_ANGLE_ERROR));
    ra = a * (3.14159265358979323846 / 180.0);

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2, true)) return false;

    wxString tf, tfltr;
    GetTimeLimitFriendly(tf);
    GetTimeLimitFilter(tfltr, true);

    int left, top;
    GetLeftAndTopValues(left, top);

    wxString fcnt = m_Centered->GetValue() ? "auto" : "";
    if (fcnt == "") fcnt.Printf("%ix%i", left, top);

    wxString cnt = BOOLSTR(m_Centered->GetValue()),
             fwd = BOOLSTR(m_Forward->GetValue()),
             pfr = BOOLSTR(m_PerFrame->GetValue()),
             flc = BOOLSTR(m_Flicker->GetValue()),
             dtr = BOOLSTR(m_Dither->GetValue());

    fltr->friendly = FFQSF(SID_VIGNETTE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               a, fcnt, fwd, pfr, flc, dtr, tf);

    fcnt = "";
    if (!m_Centered->GetValue()) fcnt.Printf(":x0=%i:y0=%i", left, top);
    wxString flicker;
    if (m_Flicker->GetValue()) flicker.Printf("-(random(1)*%g)", ra / 8);
    fltr->ff_filter.Printf("%svignette=a=%g%s%s:mode=%sward:eval=%s:dither=%s%s%s",
                              FILTER_VIDEO_IN, ra, flicker, fcnt,
                              m_Forward->GetValue() ? "for" : "back",
                              m_PerFrame->GetValue() ? "frame" : "init",
                              dtr, tfltr, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%s,%s,%i,%i,%s,%s,%s,%s," + UINT64FMT + "," + UINT64FMT,
                            m_Angle->GetValue(), cnt, left, top, fwd, pfr, flc, dtr, t1, t2);

    return true;

}
