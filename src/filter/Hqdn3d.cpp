/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Hqdn3d.cpp                                                      *
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

#include "Hqdn3d.h"

Hqdn3d::Hqdn3d(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFloatingPointValidator<float> fVal;
    fVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK | wxNUM_VAL_NO_TRAILING_ZEROES);
    fVal.SetRange(0, 1000000);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(4, 2, 0, 0);
    fgs->AddGrowableCol(0);

    MakeLabel(FFQS(SID_HQDN3D_LUMA_SPARTIAL), fgs);
    m_LumaS = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_LumaS->SetValidator(fVal);
    fgs->Add(m_LumaS, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_HQDN3D_LUMA_TEMP), fgs);
    m_LumaT = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_LumaT->SetValidator(fVal);
    fgs->Add(m_LumaT, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_HQDN3D_CHROMA_SPARTIAL), fgs);
    m_ChromaS = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_ChromaS->SetValidator(fVal);
    fgs->Add(m_ChromaS, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_HQDN3D_CHROMA_TEMP), fgs);
    m_ChromaT = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_ChromaT->SetValidator(fVal);
    fgs->Add(m_ChromaT, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    GetTimeLimitControls(fgs, 0, true);

    /*SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);*/

}

//---------------------------------------------------------------------------------------

Hqdn3d::~Hqdn3d()
{

}

//---------------------------------------------------------------------------------------

void Hqdn3d::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;


    if (fs.Len() > 0)
    {

        //Load values from filter string
        wxString v = GetToken(fs, ",", true);
        m_LumaS->SetValue(v == "-1" ? "" : v);

        v = GetToken(fs, ",", true);
        m_LumaT->SetValue(v == "-1" ? "" : v);

        v = GetToken(fs, ",", true);
        m_ChromaS->SetValue(v == "-1" ? "" : v);

        v = GetToken(fs, ",", true);
        m_ChromaT->SetValue(v == "-1" ? "" : v);

    }

    else
    {

        //Default values
        m_LumaS->SetValue("4");
        m_LumaT->SetValue("");
        m_ChromaS->SetValue("");
        m_ChromaT->SetValue("");

    }

    SetTimeLimitValues(fs);

}

//---------------------------------------------------------------------------------------

bool Hqdn3d::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftHQDN3D;

    double lumas = Str2Float(m_LumaS->GetValue(), -1),
           lumat = Str2Float(m_LumaT->GetValue(), -1),
           chromas = Str2Float(m_ChromaS->GetValue(), -1),
           chromat = Str2Float(m_ChromaT->GetValue(), -1);

    if (lumas == 0) return ShowError(m_LumaS, FFQSF(SID_MUST_BE_A_POSITIVE_NUMBER, FFQS(SID_HQDN3D_LUMA_SPARTIAL)));
    if (lumat == 0) return ShowError(m_LumaT, FFQSF(SID_MUST_BE_A_POSITIVE_NUMBER, FFQS(SID_HQDN3D_LUMA_TEMP)));
    if (chromas == 0) return ShowError(m_ChromaS, FFQSF(SID_MUST_BE_A_POSITIVE_NUMBER, FFQS(SID_HQDN3D_CHROMA_SPARTIAL)));
    if (chromat == 0) return ShowError(m_ChromaT, FFQSF(SID_MUST_BE_A_POSITIVE_NUMBER, FFQS(SID_HQDN3D_CHROMA_TEMP)));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;

    wxString f = "hqdn3d=", tf, ft;
    if (lumas > 0) f += wxString::Format("luma_spatial=%g:", lumas);
    if (lumat > 0) f += wxString::Format("luma_tmp=%g:", lumat);
    if (chromas > 0) f += wxString::Format("chroma_spatial=%g:", chromas);
    if (chromat > 0) f += wxString::Format("chroma_tmp=%g:", chromat);
    f.Remove(f.Len() - 1, 1);

    GetTimeLimitFilter(tf, true);
    GetTimeLimitFriendly(ft);

    fltr->friendly = FFQSF(SID_HQDN3D_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               lumas < 0 ? "" : wxString::Format("%g", lumas),
                               lumat < 0 ? "" : wxString::Format("%g", lumat),
                               chromas < 0 ? "" : wxString::Format("%g", chromas),
                               chromat < 0 ? "" : wxString::Format("%g", chromat),
                               ft);
    fltr->ff_filter.Printf("%s%s%s%s", FILTER_VIDEO_IN, f, tf, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%g,%g,%g,%g," + UINT64FMT + "," + UINT64FMT, lumas, lumat, chromas, chromat, t1, t2);

    return true;

}
