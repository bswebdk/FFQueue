/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Hue.cpp                                                         *
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

#include "Hue.h"

#define CALC_SAT(v) ( ( (double)(v) / 20.0) - 10 )

//---------------------------------------------------------------------------------------

Hue::Hue(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs;

    fgs = new wxFlexGridSizer(3, 3, 0, 0); //Rows, Cols, HGap, VGap
    fgs->AddGrowableCol(1);


    MakeLabel(FFQS(SID_HUE_HUE), fgs);
    m_Hue = new wxSlider(this, wxID_ANY, 0, 0, 360, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_Hue, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_HueText = MakeLabel("000000", fgs);
    wxSize ms = wxSize(m_HueText->GetSize().GetWidth(), -1);
    m_HueText->SetMinSize(ms);

    MakeLabel(FFQS(SID_HUE_SATURATION), fgs);
    m_Sat = new wxSlider(this, wxID_ANY, 200, 0, 400, wxDefaultPosition, wxSize(-1, -1));
    fgs->Add(m_Sat, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_SatText = MakeLabel("", fgs);
    m_SatText->SetMinSize(ms);

    MakeLabel(FFQS(SID_HUE_BRIGHTNESS), fgs);
    m_Bright = new wxSlider(this, wxID_ANY, 200, 0, 400, wxDefaultPosition, wxSize(-1, -1));
    fgs->Add(m_Bright, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_BrightText = MakeLabel("", fgs);
    m_BrightText->SetMinSize(ms);

    GetTimeLimitControls(fgs, 0, true);

    Connect(wxID_ANY, wxEVT_SLIDER, (wxObjectEventFunction)&Hue::OnSliderChange);

}

//---------------------------------------------------------------------------------------

Hue::~Hue()
{

}

//---------------------------------------------------------------------------------------

void Hue::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    m_Hue->SetValue(Str2Long(GetToken(fs, ",", true), m_Hue->GetValue()));
    m_Sat->SetValue(Str2Long(GetToken(fs, ",", true), m_Sat->GetValue()));
    m_Bright->SetValue(Str2Long(GetToken(fs, ",", true), m_Bright->GetValue()));

    SetTimeLimitValues(fs);

    wxCommandEvent dummy;
    OnSliderChange(dummy);

}

//---------------------------------------------------------------------------------------

bool Hue::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftHUE;

    int h = m_Hue->GetValue();
    double s = CALC_SAT(m_Sat->GetValue()), b = CALC_SAT(m_Bright->GetValue());
    bool sok = (froundi(s * 100) != 0), bok = (froundi(b * 100) != 0);

    if ((h == 0) && (!sok) && (!bok)) return ShowError(m_Hue, FFQS(SID_HUE_ERROR));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;
    wxString tf, ft;
    GetTimeLimitFilter(tf, true);
    GetTimeLimitFriendly(ft);

    wxString f = "hue=";
    if (h > 0) f += wxString::Format("h=%i:", h);
    if (sok) f += wxString::Format("s=%.2f:", s);
    if (bok) f += wxString::Format("b=%.2f:", b);
    f.Remove(f.Len() - 1, 1);

    fltr->friendly = FFQSF(SID_HUE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], h, s, b, ft);

    fltr->ff_filter.Printf("%s%s%s%s", FILTER_VIDEO_IN, f, tf, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%i,%i,%i," + UINT64FMT + "," + UINT64FMT, h, m_Sat->GetValue(), m_Bright->GetValue(), t1, t2);

    return true;

}

//---------------------------------------------------------------------------------------

void Hue::OnSliderChange(wxCommandEvent &event)
{

    event.Skip();
    wxObject *o = event.GetEventObject();

    if ((o == NULL) || (o == m_Hue)) m_HueText->SetLabel(ToStr(m_Hue->GetValue()));
    if ((o == NULL) || (o == m_Sat)) m_SatText->SetLabel(wxString::Format("%.2f", CALC_SAT(m_Sat->GetValue())));
    if ((o == NULL) || (o == m_Bright)) m_BrightText->SetLabel(wxString::Format("%.2f", CALC_SAT(m_Bright->GetValue())));

}
