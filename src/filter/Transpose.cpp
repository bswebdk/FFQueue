/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Transpose.cpp                                                   *
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

#include "Transpose.h"

Transpose::Transpose(wxWindow* parent) : FilterBasePanel(parent)
{

    SetControlPadding(5);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 3, 0, 0);

    MakeLabel(FFQS(SID_TRANSPOSE_DIRECTION), fgs, 0);

    m_Clockwise = new wxRadioButton(this, wxID_ANY, FFQS(SID_TRANSPOSE_CLOCKWISE));
    fgs->Add(m_Clockwise, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    m_CClockwise = new wxRadioButton(this, wxID_ANY, FFQS(SID_TRANSPOSE_COUNTER_CLOCKWISE));
    fgs->Add(m_CClockwise, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
    bs->Add(fgs, 1, wxALL|wxALIGN_LEFT, 0);

    m_Flip = new wxCheckBox(this, wxID_ANY, FFQS(SID_TRANSPOSE_FLIP));
    bs->Add(m_Flip, 1, wxALL|wxALIGN_LEFT, 5);

    SetSizer(bs);
    bs->Fit(this);
    //bs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Transpose::~Transpose()
{

}

//---------------------------------------------------------------------------------------

void Transpose::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    m_Clockwise->SetValue(STRBOOLDEF(GetToken(fs, ",", true), true));
    m_CClockwise->SetValue(!m_Clockwise->GetValue());
    m_Flip->SetValue(STRBOOLDEF(GetToken(fs, ",", true), false));

}

//---------------------------------------------------------------------------------------

bool Transpose::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftTRANSPOSE;

    wxString s = m_Clockwise->GetValue() ? m_Clockwise->GetLabelText() : m_CClockwise->GetLabelText();
    fltr->friendly = FFQSF(SID_TRANSPOSE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], s.Lower(), BOOLSTR(m_Flip->GetValue()));

    s = "clock";
    if (m_CClockwise->GetValue()) s = "c" + s;
    if (m_Flip->GetValue()) s += "_flip";
    fltr->ff_filter.Printf("%stranspose=%s%s", FILTER_VIDEO_IN, s, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%s,%s", BOOLSTR(m_Clockwise->GetValue()), BOOLSTR(m_Flip->GetValue()));

    return true;

}
