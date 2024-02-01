/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Deflicker.cpp                                                   *
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

#include "Deflicker.h"

const int NUM_MODES = 7;
const wxString MODES[] = { "am", "gm", "hm", "qm", "cm", "pm", "median" };

Deflicker::Deflicker(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(2, 3, 0, 0);

    MakeLabel(FFQS(SID_DEFLICKER_SAMPLE_COUNT), fgs, wxALL|wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL);
    m_Size = new wxSlider(this, wxID_ANY, 5, 2, 129, wxDefaultPosition, wxSize(250, -1));
    fgs->Add(m_Size, 1, wxALL|wxEXPAND, 10);
    m_SliderText = MakeLabel("000", fgs, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);

    MakeLabel(FFQS(SID_DEFLICKER_AVG_MODE), fgs, wxALL|wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL);
    m_Mode = new wxChoice(this, wxID_ANY);
    wxString *as = FFQL()->GetStringArray(SID_DEFLICKER_AVG_MODES, NUM_MODES);
    for (int i = 0; i < NUM_MODES; i++) m_Mode->Append(as[i]);
    m_Mode->SetSelection(0);
    delete[] as;
    fgs->Add(m_Mode, 1, wxALL|wxEXPAND, 10);

    SetSizer(fgs);
    fgs->Fit(this);
    //fgs->SetSizeHints(this);

    m_Size->Bind(wxEVT_SLIDER, &Deflicker::OnSliderChange, this);

}

//---------------------------------------------------------------------------------------

Deflicker::~Deflicker()
{

}

//---------------------------------------------------------------------------------------

void Deflicker::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;
    m_Size->SetValue( Str2Long(GetToken(fs, ","), m_Size->GetValue()) );
    m_Mode->SetSelection( Str2Long(fs, m_Mode->GetSelection()) );
    wxCommandEvent dummy;
    OnSliderChange(dummy);

}

//---------------------------------------------------------------------------------------

bool Deflicker::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftDEFLICKER;

    fltr->friendly = FFQSF(SID_DEFLICKER_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], m_Size->GetValue(), m_Mode->GetStringSelection());
    wxString f = wxString::Format("deflicker=s=%u:m=%s", m_Size->GetValue(), MODES[m_Mode->GetSelection()]);
    fltr->ff_filter.Printf("%s%s%s", FILTER_VIDEO_IN, f, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%u,%u", m_Size->GetValue(), m_Mode->GetSelection());

    return true;

}

void Deflicker::OnSliderChange(wxCommandEvent &event)
{
    m_SliderText->SetLabel(wxString::Format("%i", m_Size->GetValue()));
}

