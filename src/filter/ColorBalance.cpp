/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: ColorBalance.cpp                                                *
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

#include "ColorBalance.h"

//---------------------------------------------------------------------------------------

const int ID_BASE = 10000;

const int ID_SHADOW_RED = 0;
const int ID_SHADOW_GREEN = 1;
const int ID_SHADOW_BLUE = 2;

const int ID_MIDTONE_RED = 3;
const int ID_MIDTONE_GREEN = 4;
const int ID_MIDTONE_BLUE = 5;

const int ID_HIGHLIGHT_RED = 6;
const int ID_HIGHLIGHT_GREEN = 7;
const int ID_HIGHLIGHT_BLUE = 8;

const int ID_LOCK_GROUPS = 9;
const int ID_LOCK_COLORS = 10;


//---------------------------------------------------------------------------------------

ColorBalance::ColorBalance(wxWindow* parent) : FilterBasePanel(parent)
{

    m_AddID = ID_BASE;

    wxFlexGridSizer *fgs = new wxFlexGridSizer(5, 1, 0, 0), *fgsx;

    MakeGroup(fgs, FFQS(SID_CLRBAL_SHADOW_ADJUST), &m_ShadowR, &m_ShadowG, &m_ShadowB);
    MakeGroup(fgs, FFQS(SID_CLRBAL_MIDTONE_ADJUST), &m_MidtoneR, &m_MidtoneG, &m_MidtoneB);
    MakeGroup(fgs, FFQS(SID_CLRBAL_HIGHLIGHT_ADJUST), &m_LightR, &m_LightG, &m_LightB);

    wxBoxSizer *bs = new wxBoxSizer(wxHORIZONTAL);
    m_LockGroups = new wxCheckBox(this, m_AddID++, FFQS(SID_CLRBAL_LOCK_IN_GROUPS), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_LockGroups, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    m_LockColors = new wxCheckBox(this, m_AddID++, FFQS(SID_CLRBAL_LOCK_ACROSS_GROUPS), wxDefaultPosition, wxDefaultSize, 0);
    bs->Add(m_LockColors, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    fgs->Add(bs, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

        fgsx = new wxFlexGridSizer(1, 2, 0, 0);
        fgsx->AddGrowableCol(1);
        MakeLabel(FFQS(SID_FILTER_THIS), fgsx);
        fgsx->Add(GetTimeLimitControls(), wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(fgsx, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

	Connect(wxID_ANY, wxEVT_SLIDER, (wxObjectEventFunction)&ColorBalance::OnCtrlChange);
	Connect(wxID_ANY, wxEVT_CHECKBOX,(wxObjectEventFunction)&ColorBalance::OnCtrlChange);

}

//---------------------------------------------------------------------------------------

ColorBalance::~ColorBalance()
{

}

//---------------------------------------------------------------------------------------

void ColorBalance::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_ShadowR->SetValue(Str2Long(GetToken(fs, ',')));
        m_ShadowG->SetValue(Str2Long(GetToken(fs, ',')));
        m_ShadowB->SetValue(Str2Long(GetToken(fs, ',')));
        m_MidtoneR->SetValue(Str2Long(GetToken(fs, ',')));
        m_MidtoneG->SetValue(Str2Long(GetToken(fs, ',')));
        m_MidtoneB->SetValue(Str2Long(GetToken(fs, ',')));
        m_LightR->SetValue(Str2Long(GetToken(fs, ',')));
        m_LightG->SetValue(Str2Long(GetToken(fs, ',')));
        m_LightB->SetValue(Str2Long(GetToken(fs, ',')));
        m_LockColors->SetValue(GetToken(fs, ',') == STR_YES);
        m_LockGroups->SetValue(GetToken(fs, ',') == STR_YES);

    }

    else
    {

        //Default values
        m_ShadowR->SetValue(100);
        m_ShadowG->SetValue(100);
        m_ShadowB->SetValue(100);
        m_MidtoneR->SetValue(100);
        m_MidtoneG->SetValue(100);
        m_MidtoneB->SetValue(100);
        m_LightR->SetValue(100);
        m_LightG->SetValue(100);
        m_LightB->SetValue(100);
        m_LockColors->SetValue(false);
        m_LockGroups->SetValue(true);

    }

    SetTimeLimitValues(fs, true);

}

//---------------------------------------------------------------------------------------

float gv(wxSlider *s)
{
    return ((float)s->GetValue() - 100.0) / 100.0;
}

//---------------------------------------------------------------------------------------

bool ColorBalance::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftCOLORBALANCE;

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2, true)) return false;

    float sr = gv(m_ShadowR), sg = gv(m_ShadowG), sb = gv(m_ShadowB),
          mr = gv(m_MidtoneR), mg = gv(m_MidtoneG), mb = gv(m_MidtoneB),
          lr = gv(m_LightR), lg = gv(m_LightG), lb = gv(m_LightB);

    wxString tf;
    GetTimeLimitFriendly(tf);
    fltr->friendly = FFQSF(SID_CLRBAL_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], sr, sg, sb, mr, mg, mb, lr, lg, lb, tf);

    wxString tfltr;
    GetTimeLimitFilter(tfltr, true);

    fltr->ff_filter.Printf("%scolorbalance=rs=%.2f:gs=%.2f:bs=%.2f:rm=%.2f:gm=%.2f:bm=%.2f:rh=%.2f:gh=%.2f:bh=%.2f%s%s",
                              FILTER_VIDEO_IN, sr, sg, sb, mr, mg, mb, lr, lg, lb, tfltr, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%i,%i,%i,%i,%i,%i,%i,%i,%i,%s,%s," + UINT64FMT + "," + UINT64FMT,
                            m_ShadowR->GetValue(), m_ShadowG->GetValue(), m_ShadowB->GetValue(),
                            m_MidtoneR->GetValue(), m_MidtoneG->GetValue(), m_MidtoneB->GetValue(),
                            m_LightR->GetValue(), m_LightG->GetValue(), m_LightB->GetValue(),
                            BOOLSTR(m_LockColors->GetValue()), BOOLSTR(m_LockGroups->GetValue()),
                            t1, t2);

    return true;

}

//---------------------------------------------------------------------------------------

void ColorBalance::MakeGroup(wxSizer *sizer, wxString caption, wxSlider **red, wxSlider **green, wxSlider **blue)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(3, 2, 0, 0);
    fgs->AddGrowableCol(1);
    MakeRow(fgs, FFQS(SID_CLRBAL_RED), red);
    MakeRow(fgs, FFQS(SID_CLRBAL_GREEN), green);
    MakeRow(fgs, FFQS(SID_CLRBAL_BLUE), blue);

    wxStaticBoxSizer *sb = new wxStaticBoxSizer(wxVERTICAL, this, caption);
    sb->Add(fgs, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    sizer->Add(sb, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

}

//---------------------------------------------------------------------------------------

void ColorBalance::MakeRow(wxSizer *sizer, wxString caption, wxSlider **slider)
{

    MakeLabel(caption, sizer);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 3, 0, 0); //Labels sizer
    fgs->AddGrowableCol(1);
    MakeLabel(_T("-1"), fgs, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
    *slider = new wxSlider(this, m_AddID++, 0, 0, 200, wxDefaultPosition, wxSize(400, -1));
    fgs->Add(*slider, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    MakeLabel(_T("+1"), fgs, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);

    sizer->Add(fgs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

}

//---------------------------------------------------------------------------------------

void adjust(wxSlider *a, wxSlider *b, wxSlider *c, int v)
{
    a->SetValue(v);
    b->SetValue(v);
    c->SetValue(v);
}

//---------------------------------------------------------------------------------------

void ColorBalance::OnCtrlChange(wxCommandEvent &event)
{

    int cid = event.GetId() - ID_BASE;

    if ((cid == ID_LOCK_GROUPS) || (cid == ID_LOCK_COLORS))
    {

        if (m_LockGroups->GetValue())
        {
            adjust(m_ShadowR, m_ShadowG, m_ShadowB, m_ShadowR->GetValue());
            adjust(m_MidtoneR, m_MidtoneG, m_MidtoneB, m_MidtoneR->GetValue());
            adjust(m_LightR, m_LightG, m_LightB, m_LightR->GetValue());
        }

        if (m_LockColors->GetValue())
        {
            adjust(m_ShadowR, m_MidtoneR, m_LightR, m_ShadowR->GetValue());
            adjust(m_ShadowG, m_MidtoneG, m_LightG, m_ShadowG->GetValue());
            adjust(m_ShadowB, m_MidtoneB, m_LightB, m_ShadowB->GetValue());
        }

    }

    else if (m_LockGroups->GetValue() || m_LockColors->GetValue())
    {

        wxSlider* src = (wxSlider*)FindWindow(event.GetId());
        int sv = src->GetValue();

        if (m_LockColors->GetValue() && m_LockGroups->GetValue())
        {

            adjust(m_ShadowR, m_ShadowG, m_ShadowB, sv);
            adjust(m_MidtoneR, m_MidtoneG, m_MidtoneB, sv);
            adjust(m_LightR, m_LightG, m_LightB, sv);
            return;

        }

        if (m_LockGroups->GetValue())
        {

            if ((cid == ID_SHADOW_RED) || (cid == ID_SHADOW_GREEN) || (cid == ID_SHADOW_BLUE))
                adjust(m_ShadowR, m_ShadowG, m_ShadowB, sv);

            else if ((cid == ID_MIDTONE_RED) || (cid == ID_MIDTONE_GREEN) || (cid == ID_MIDTONE_BLUE))
                adjust(m_MidtoneR, m_MidtoneG, m_MidtoneB, sv);

            else if ((cid == ID_HIGHLIGHT_RED) || (cid == ID_HIGHLIGHT_GREEN) || (cid == ID_HIGHLIGHT_BLUE))
                adjust(m_LightR, m_LightG, m_LightB, sv);

        }

        if (m_LockColors->GetValue())
        {

            if ((cid == ID_SHADOW_RED) || (cid == ID_MIDTONE_RED) || (cid == ID_HIGHLIGHT_RED))
                adjust(m_ShadowR, m_MidtoneR, m_LightR, sv);

            else if ((cid == ID_SHADOW_GREEN) || (cid == ID_MIDTONE_GREEN) || (cid == ID_HIGHLIGHT_GREEN))
                adjust(m_ShadowG, m_MidtoneG, m_LightG, sv);

            else if ((cid == ID_SHADOW_BLUE) || (cid == ID_MIDTONE_BLUE) || (cid == ID_HIGHLIGHT_BLUE))
                adjust(m_ShadowB, m_MidtoneB, m_LightB, sv);

        }

    }

}
