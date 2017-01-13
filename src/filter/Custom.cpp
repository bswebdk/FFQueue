/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Custom.cpp                                                      *
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

#include "Custom.h"

Custom::Custom(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(2, 2, 0, 0), *fgsx;
    fgs->AddGrowableCol(1);

    MakeLabel(FFQS(SID_CUSTFLTR_TYPE) + " ", fgs);

        fgsx = new wxFlexGridSizer(1, 2, 0, 0);
        m_Video = new wxRadioButton(this, wxID_ANY, FFQS(SID_COMMON_VIDEO));
        fgsx->Add(m_Video, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
        m_Audio = new wxRadioButton(this, wxID_ANY, FFQS(SID_COMMON_AUDIO));
        fgsx->Add(m_Audio, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
        fgs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_CUSTFLTR_VALUE) + " ", fgs);

        fgsx = new wxFlexGridSizer(1, 3, 0, 0);
        fgsx->AddGrowableCol(1);
        MakeLabel("[IN]", fgsx);
        m_Filter = new wxTextCtrl(this, wxID_ANY);
        fgsx->Add(m_Filter, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
        MakeLabel("[OUT]", fgsx);
        fgs->Add(fgsx, 1, wxALL|wxEXPAND, 3);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Custom::~Custom()
{

}

//---------------------------------------------------------------------------------------

void Custom::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    //Default values
    m_Video->SetValue(true);
    m_Audio->SetValue(false);

    if (fs.Len() > 0)
    {
        //Load values from filter string
        m_Video->SetValue(GetToken(fs, ',') == STR_YES);
        m_Audio->SetValue(GetToken(fs, ',') == STR_YES);
    }

    m_Filter->SetValue(fs);

}

//---------------------------------------------------------------------------------------

bool Custom::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftCUSTOM;

    wxString a, b, n, v = StrTrim(m_Filter->GetValue());

    if ((v.Len() == 0) || (v.Find(FILTER_SEPARATOR) >= 0)) return ShowError(m_Filter, FFQSF(SID_CUSTFLTR_ERROR, FILTER_SEPARATOR));

    if (m_Video->GetValue())
    {

        a = FILTER_VIDEO_IN;
        b = FILTER_VIDEO_OUT;
        n = m_Video->GetLabel();

    }
    else
    {

        a = FILTER_AUDIO_IN;
        b = FILTER_AUDIO_OUT;
        n = m_Audio->GetLabel();

    }


    fltr->friendly = FFQSF(SID_CUSTFLTR_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], n.Lower(), v);
    fltr->editable.Printf("%s,%s,%s", BOOLSTR(m_Video->GetValue()), BOOLSTR(m_Audio->GetValue()), v);

    v.Replace("\"", "\\\"");
    fltr->ff_filter.Printf("%s%s%s", a, v, b);

    return true;

}
