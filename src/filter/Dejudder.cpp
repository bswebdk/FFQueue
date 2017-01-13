/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Dejudder.cpp                                                    *
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

#include "Dejudder.h"

Dejudder::Dejudder(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned short> iVal;
    iVal.SetRange(0, 0xFFFF);

    wxFlexGridSizer *fgs;
    wxBoxSizer *bs;

    bs = new wxBoxSizer(wxVERTICAL);

    m_Film2NTSC = new wxRadioButton(this, wxID_ANY, FFQS(SID_DEJUDDER_FILM_2_NTSC), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_Film2NTSC, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    m_PAL2NTSC = new wxRadioButton(this, wxID_ANY, FFQS(SID_DEJUDDER_PAL_2_NTSC), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_PAL2NTSC, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    m_MixBoth = new wxRadioButton(this, wxID_ANY, FFQS(SID_DEJUDDER_MIX_OF_BOTH), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_MixBoth, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

        fgs = new wxFlexGridSizer(1, 2, 0, 0);
        fgs->AddGrowableCol(1);
        m_Custom = new wxRadioButton(this, wxID_ANY, FFQS(SID_DEJUDDER_CUSTOM), wxDefaultPosition, wxDefaultSize, 0);
        fgs->Add(m_Custom, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
        m_Cycle = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), 0);
        m_Cycle->SetValidator(iVal);
        fgs->Add(m_Cycle, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    bs->Add(fgs, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);

    SetSizer(bs);
    bs->Fit(this);
    bs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Dejudder::~Dejudder()
{

}

//---------------------------------------------------------------------------------------

void Dejudder::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    int sel = Str2Long(GetToken(fs, ','));

    m_Film2NTSC->SetValue(sel == 0);
    m_PAL2NTSC->SetValue(sel == 1);
    m_MixBoth->SetValue(sel == 2);
    m_Custom->SetValue(sel == 3);
    m_Cycle->SetValue(fs.Len() == 0 ? "10" : fs);

}

//---------------------------------------------------------------------------------------

bool Dejudder::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftDEJUDDER;

    int cycle = Str2Long(m_Cycle->GetValue());

    if (m_Custom->GetValue() && (cycle < 2)) return ShowError(m_Cycle, FFQS(SID_DEJUDDER_CUSTOM_ERROR));

    unsigned int sel = 0, cycle_f = cycle;

    if (m_Film2NTSC->GetValue())
    {
        sel = 0;
        cycle_f = 4;
    }
    else if (m_PAL2NTSC->GetValue())
    {
        sel = 1;
        cycle_f = 5;
    }
    else if (m_MixBoth->GetValue())
    {
        sel = 2;
        cycle_f = 20;
    }
    else if (m_Custom->GetValue()) sel = 3;

    fltr->friendly = FFQSF(SID_DEJUDDER_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], cycle_f);
    fltr->ff_filter.Printf("%sdejudder=%i%s", FILTER_VIDEO_IN, cycle_f, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%u,%u", sel, cycle);

    return true;

}
