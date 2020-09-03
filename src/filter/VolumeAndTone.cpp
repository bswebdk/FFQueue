/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: VolumeAndTone.cpp                                               *
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

#include "VolumeAndTone.h"

VolumeAndTone::VolumeAndTone(wxWindow* parent) : FilterBasePanel(parent)
{

    SetControlPadding(5); //Label padding

    wxFlexGridSizer *fgs1 = new wxFlexGridSizer(2, 5, 0, 0)/*, //
                    *fgs2 = new wxFlexGridSizer(3, 1, 0, 0)*/;

    fgs1->AddGrowableCol(4);

    //Top row
    fgs1->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    MakeLabel(FFQS(SID_VOLTONE_VOLUME), fgs1, wxLEFT|wxRIGHT|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);
    MakeLabel(FFQS(SID_VOLTONE_BASS), fgs1, wxLEFT|wxRIGHT|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);
    MakeLabel(FFQS(SID_VOLTONE_TREBLE), fgs1, wxLEFT|wxRIGHT|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);
    fgs1->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    MakeSliderLabels(fgs1, _T("+20"), _T("0"), _T("-20"), true);

    //Bottom row - sliders
    m_Volume = new wxSlider(this, wxID_ANY, 20, 0, 40, wxDefaultPosition, wxSize(-1, 300), wxSL_VERTICAL|wxSL_INVERSE);
    fgs1->Add(m_Volume, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);

    m_Bass = new wxSlider(this, wxID_ANY, 20, 0, 40, wxDefaultPosition, wxSize(-1, 300), wxSL_VERTICAL|wxSL_INVERSE);
    fgs1->Add(m_Bass, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);

    m_Treble = new wxSlider(this, wxID_ANY, 20, 0, 40, wxDefaultPosition, wxSize(-1, 300), wxSL_VERTICAL|wxSL_INVERSE);
    fgs1->Add(m_Treble, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    SetSizer(fgs1);
    fgs1->Fit(this);
    fgs1->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

VolumeAndTone::~VolumeAndTone()
{

}

//---------------------------------------------------------------------------------------

void VolumeAndTone::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Volume->SetValue(Str2Long(GetToken(fs, ',')));
        m_Bass->SetValue(Str2Long(GetToken(fs, ',')));
        m_Treble->SetValue(Str2Long(GetToken(fs, ',')));

    }

    else
    {

        //Default values
        m_Volume->SetValue(20);
        m_Bass->SetValue(20);
        m_Treble->SetValue(20);

    }

}

//---------------------------------------------------------------------------------------

bool VolumeAndTone::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftVOLUME_AND_TONE;

    int v = m_Volume->GetValue() - 20, b = m_Bass->GetValue() - 20, t = m_Treble->GetValue() - 20;

    if ((v == 0) && (t == 0) && (b == 0)) return ShowError(m_Volume, FFQS(SID_VOLTONE_ERROR));

    float fv = (v + 20) / 20.0;

    fltr->friendly = FFQSF(SID_VOLTONE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], v, b, t);
    fltr->ff_filter.Printf("%svolume=%f,bass=g=%i,treble=g=%i%s", FILTER_AUDIO_IN, fv, b, t, FILTER_AUDIO_OUT);
    fltr->editable.Printf("%i,%i,%i", m_Volume->GetValue(), m_Bass->GetValue(), m_Treble->GetValue());

    return true;

}
