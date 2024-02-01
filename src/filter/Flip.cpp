/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Flip.cpp                                                        *
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

#include "Flip.h"

Flip::Flip(wxWindow* parent) : FilterBasePanel(parent)
{

    wxBoxSizer *bs;

    bs = new wxBoxSizer(wxVERTICAL);

    m_Horz = new wxCheckBox(this, wxID_ANY, FFQS(SID_FLIP_HORZ), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_Horz, 1, wxALL|wxALIGN_LEFT, 10);

    m_Vert = new wxCheckBox(this, wxID_ANY, FFQS(SID_FLIP_VERT), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_Vert, 1, wxALL|wxALIGN_LEFT, 10);

    SetSizer(bs);
    bs->Fit(this);
    //bs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Flip::~Flip()
{

}

//---------------------------------------------------------------------------------------

void Flip::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Horz->SetValue(GetToken(fs, ',') == STR_YES);
        m_Vert->SetValue(GetToken(fs, ',') == STR_YES);

    }

    else
    {

        //Default values
        m_Horz->SetValue(false);
        m_Vert->SetValue(true);

    }

}

//---------------------------------------------------------------------------------------

bool Flip::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftFLIP;

    if ((!m_Horz->GetValue()) && (!m_Vert->GetValue())) return ShowError(FFQS(SID_FLIP_ERROR));

    wxString h = BOOLSTR(m_Horz->GetValue()), v = BOOLSTR(m_Vert->GetValue()),
             f = m_Horz->GetValue() ? "hflip" : "";

    if (m_Vert->GetValue())
    {
        if (f.Len() > 0) f += ",";
        f += "vflip";
    }

    fltr->friendly = FFQSF(SID_FLIP_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], h, v);
    fltr->ff_filter.Printf("%s%s%s", FILTER_VIDEO_IN, f, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%s,%s", h, v);

    return true;

}
