/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: BoxBlur.cpp                                                     *
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

#include "BoxBlur.h"

BoxBlur::BoxBlur(wxWindow* parent) : FilterBasePanel(parent)
{

    m_Val.SetRange(0, 99999);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(4, 2, 0, 0);

    MakeRow(fgs, FFQS(SID_BOXBLUR_LUMA), &m_LumaRad, &m_LumaPow);
    MakeRow(fgs, FFQS(SID_BOXBLUR_CHROMA), &m_ChromaRad, &m_ChromaPow);
    MakeRow(fgs, FFQS(SID_BOXBLUR_ALPHA), &m_AlphaRad, &m_AlphaPow);

    MakeLabel(FFQS(SID_FILTER_TIME_LIMIT), fgs);
    fgs->Add(GetTimeLimitControls(), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

BoxBlur::~BoxBlur()
{

}

//---------------------------------------------------------------------------------------

void BoxBlur::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_LumaRad->SetValue(GetToken(fs, ','));
        m_LumaPow->SetValue(GetToken(fs, ','));
        m_ChromaRad->SetValue(GetToken(fs, ','));
        m_ChromaPow->SetValue(GetToken(fs, ','));
        m_AlphaRad->SetValue(GetToken(fs, ','));
        m_AlphaPow->SetValue(GetToken(fs, ','));

    }

    else
    {

        //Default values
        m_LumaRad->SetValue("2");
        m_LumaPow->SetValue("2");
        m_ChromaRad->SetValue("0");
        m_ChromaPow->SetValue("0");
        m_AlphaRad->SetValue("0");
        m_AlphaPow->SetValue("0");

    }

    SetTimeLimitValues(fs, true);

}

//---------------------------------------------------------------------------------------

bool BoxBlur::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftBOXBLUR;

    int lr = Str2Long(m_LumaRad->GetValue()), lp = Str2Long(m_LumaPow->GetValue()),
        cr = Str2Long(m_ChromaRad->GetValue()), cp = Str2Long(m_ChromaPow->GetValue()),
        ar = Str2Long(m_AlphaRad->GetValue()), ap = Str2Long(m_AlphaPow->GetValue());

    wxString f = "", s;

    if (lp > 0)
    {
        if (lr == 0) return ShowError(m_LumaRad, FFQSF(SID_BOXBLUR_RADIUS_ERROR, FFQS(SID_BOXBLUR_LUMA)));

        s.Printf("lr=%i:lp=%i:", lr, lp);
        f += s;
    }

    if (cp > 0)
    {
        if (cr == 0) return ShowError(m_ChromaRad, FFQSF(SID_BOXBLUR_RADIUS_ERROR, FFQS(SID_BOXBLUR_CHROMA)));

        s.Printf("cr=%i:cp=%i:", cr, cp);
        f += s;
    }

    if (ap > 0)
    {
        if (ar == 0) return ShowError(m_AlphaRad, FFQSF(SID_BOXBLUR_RADIUS_ERROR, FFQS(SID_BOXBLUR_ALPHA)));

        s.Printf("ar=%i:ap=%i:", ar, ap);
        f += s;
    }

    if (f.Len() == 0) return ShowError(m_LumaPow, FFQS(SID_BOXBLUR_POWER_ERROR));

    uint64_t mf, mt;
    if (!GetTimeLimitValues(mf, mt, true)) return false;

    wxString tf, ft;
    GetTimeLimitFriendly(tf);
    GetTimeLimitFilter(ft, true);

    fltr->friendly = FFQSF(SID_BOXBLUR_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], lr, lp, cr, cp, ar, ap, tf);
    fltr->ff_filter.Printf("%sboxblur=%s%s%s", FILTER_VIDEO_IN, f.BeforeLast(':'), ft, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%i,%i,%i,%i,%i,%i," + UINT64FMT + "," + UINT64FMT, lr, lp, cr, cp, ar, ap, mf, mt);

    return true;

}

//---------------------------------------------------------------------------------------

void BoxBlur::MakeRow(wxSizer *sizer, wxString caption, wxTextCtrl **rad, wxTextCtrl **pow)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 4, 0, 0);

    MakeLabel(FFQS(SID_BOXBLUR_RADIUS), fgs, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
    *rad = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    (*rad)->SetValidator(m_Val);
    fgs->Add(*rad, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_BOXBLUR_POWER), fgs, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
    *pow = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    (*pow)->SetValidator(m_Val);
    fgs->Add(*pow, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(caption, sizer);
    sizer->Add(fgs, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

}
