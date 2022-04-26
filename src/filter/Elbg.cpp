/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Elbg.cpp                                                        *
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

#include "Elbg.h"

Elbg::Elbg(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(2, 2, 0, 0);
    fgs->AddGrowableCol(0);

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK);
    iVal.SetRange(0, 0xFFFFFFFF);

    MakeLabel(FFQS(SID_ELBG_COLORS), fgs);
    m_Colors = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_Colors->SetValidator(iVal);
    fgs->Add(m_Colors, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    MakeLabel(FFQS(SID_ELBG_ITERATIONS), fgs);
    m_Iterations = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_Iterations->SetValidator(iVal);
    fgs->Add(m_Iterations, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    //fgs->Add(GetTimeLimitControls(), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    //fgs->Add(-1, -1);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Elbg::~Elbg()
{

}

//---------------------------------------------------------------------------------------

void Elbg::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Colors->SetValue(GetToken(fs, ','));
        m_Iterations->SetValue(GetToken(fs, ','));

    }

    else
    {

        //Default values
        m_Colors->SetValue("256");
        m_Iterations->SetValue("1");

    }

    //SetTimeLimitValues(fs);

}

//---------------------------------------------------------------------------------------

bool Elbg::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftELBG;

    unsigned int c = Str2Long(m_Colors->GetValue(), 0), i = Str2Long(m_Iterations->GetValue(), 0);

    if ((c <= 0) || (i <= 0)) return ShowError((c<=0) ? m_Colors : m_Iterations, FFQS(SID_ELBG_ERROR));

    //time_millis mf, mt;
    //if (!GetTimeLimitValues(mf, mt)) return false;

    //wxString tf, ft;
    //GetTimeLimitFriendly(ft);
    //GetTimeLimitFilter(tf, true);

    fltr->friendly = FFQSF(SID_ELBG_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], c, i);//, ft);
    fltr->ff_filter.Printf("%selbg=l=%u:n=%u%s", FILTER_VIDEO_IN, c, i, /*tf,*/ FILTER_VIDEO_OUT);
    fltr->editable.Printf("%u,%u", c, i);//, mf, mt);

    return true;

}
