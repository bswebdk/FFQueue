/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: AudioTempo.cpp                                                  *
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

#include "AudioTempo.h"

#define SLIDER_MIN 0
#define SLIDER_MAX 300
#define SLIDER_DEF 150

AudioTempo::AudioTempo(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs;

    fgs = new wxFlexGridSizer(1, 4, 0, 0); //Rows, Cols, HGap, VGap
    fgs->AddGrowableCol(1);

    MakeLabel(FFQS(SID_ATEMPO_HALF_SPEED), fgs);

    m_Tempo = new wxSlider(this, 1000, SLIDER_DEF, SLIDER_MIN, SLIDER_MAX, wxDefaultPosition, wxSize(300, -1));
    fgs->Add(m_Tempo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_ATEMPO_DOUBLE_SPEED), fgs);
    m_SliderText = MakeLabel("0.00", fgs);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

    Connect(1000, wxEVT_SLIDER, (wxObjectEventFunction)&AudioTempo::OnSliderChange);

}

//---------------------------------------------------------------------------------------

AudioTempo::~AudioTempo()
{

}

//---------------------------------------------------------------------------------------

void AudioTempo::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    int sp = Str2Long(GetToken(fs, ",", SLIDER_DEF));

    if (sp < SLIDER_MIN) m_Tempo->SetValue(SLIDER_MIN);
    else if (sp > SLIDER_MAX) m_Tempo->SetValue(SLIDER_MAX);
    else m_Tempo->SetValue(sp);

    wxCommandEvent dummy;
    OnSliderChange(dummy);

}

//---------------------------------------------------------------------------------------

bool AudioTempo::GetFilter(LPFFMPEG_FILTER fltr)
{

    //Validate controls and return false if invalid
    double v = GetTempo();

    if (froundi(v * 100) == 100) return ShowError(m_Tempo, FFQS(SID_ATEMPO_ERROR));

    fltr->type = ftATEMPO;

    fltr->friendly = FFQSF(SID_ATEMPO_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], wxString::Format("%.2f", v));

    fltr->ff_filter.Printf("%satempo=%.2f%s", FILTER_AUDIO_IN, v, FILTER_AUDIO_OUT);

    fltr->editable.Printf("%u", m_Tempo->GetValue());

    return true;

}

//---------------------------------------------------------------------------------------

double AudioTempo::GetTempo()
{
    double v = m_Tempo->GetValue();
    return (v / (double)SLIDER_MAX * 1.5) + 0.5;
}

//---------------------------------------------------------------------------------------

void AudioTempo::OnSliderChange(wxCommandEvent &event)
{
    event.Skip();
    m_SliderText->SetLabel(wxString::Format("%1.2f", GetTempo()));
}
