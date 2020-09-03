/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Negate.cpp                                                      *
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

#include "Negate.h"

Negate::Negate(wxWindow* parent) : FilterBasePanel(parent)
{

    wxBoxSizer *bs;

    bs = new wxBoxSizer(wxVERTICAL);

    m_Alpha = new wxCheckBox(this, wxID_ANY, FFQS(SID_NEGATE_ALPHA_CHANNEL), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_Alpha, 1, wxALL|wxALIGN_LEFT/*|wxALIGN_CENTER_VERTICAL*/, 10);

    bs->Add(GetTimeLimitControls(true), 1, wxALL|wxALIGN_LEFT/*|wxALIGN_CENTER_VERTICAL*/, 0);

    SetSizer(bs);
    bs->Fit(this);
    bs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Negate::~Negate()
{

}

//---------------------------------------------------------------------------------------

void Negate::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Alpha->SetValue(GetToken(fs, ',') == STR_YES);

    }

    else
    {

        //Default values
        m_Alpha->SetValue(false);

    }

    SetTimeLimitValues(fs);

}

//---------------------------------------------------------------------------------------

bool Negate::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftNEGATE;

    uint64_t mf, mt;
    if (!GetTimeLimitValues(mf, mt)) return false;

    wxString fv = m_Alpha->GetValue() ? "1" : "0",
             un = m_Alpha->GetValue() ? FFQS(SID_NEGATE_ALPHA) : FFQS(SID_NEGATE_IMAGE), tf, ft;

    GetTimeLimitFriendly(ft);
    GetTimeLimitFilter(tf, true);

    fltr->friendly = FFQSF(SID_NEGATE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], un, ft);
    fltr->ff_filter.Printf("%snegate=%s%s%s", FILTER_VIDEO_IN, fv, tf, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%s," + UINT64FMT + "," + UINT64FMT, BOOLSTR(m_Alpha->GetValue()), mf, mt);

    return true;

}
