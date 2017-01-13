/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Noise.cpp                                                       *
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

#include "Noise.h"

const wxString RGBA = "rgba";

//---------------------------------------------------------------------------------------

Noise::Noise(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK);
    iVal.SetRange(0, 0xFFFFFFFF);

    SetControlPadding(0);

    wxString *cn = FFQL()->GetStringArray(SID_NOISE_COMPONENT_NAMES, COMPONENT_COUNT);
    wxFlexGridSizer *fgs = new wxFlexGridSizer(4, 1, 10, 0), *fgsx;
    fgs->AddGrowableCol(0);

    //Seeds
    fgsx = new wxFlexGridSizer(3, 4, 1, 10);
    for (int i = 0; i < COMPONENT_COUNT; i++)
    {
        m_Seed[i].use = new wxCheckBox(this, wxID_ANY, cn[i]);
        fgsx->Add(m_Seed[i].use, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        m_Seed[i].val = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(100, -1));
        fgsx->Add(m_Seed[i].val, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        if (i == 0)
        {
            fgsx->Add(-1, -1);
            fgsx->Add(-1, -1);
        }
    }
    wxStaticBoxSizer *sbs = new wxStaticBoxSizer(wxVERTICAL, this, FFQS(SID_NOISE_SEED));
    sbs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    fgs->Add(sbs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    //Strengths
    fgsx = new wxFlexGridSizer(3, 6, 1, 10);
    for (int i = 0; i < COMPONENT_COUNT; i++)
    {
        m_Strength[i].use = new wxCheckBox(this, wxID_ANY, cn[i]);
        fgsx->Add(m_Strength[i].use, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        m_Strength[i].val = new wxSlider(this, wxID_ANY, 1, 1, 100, wxDefaultPosition, wxSize(125, -1));
        fgsx->Add(m_Strength[i].val, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        m_Strength[i].show = MakeLabel("000", fgsx);
        m_Strength[i].show->SetMinSize(m_Strength[i].show->GetSize());
        if (i == 0)
        {
            fgsx->Add(-1, -1);
            fgsx->Add(-1, -1);
            fgsx->Add(-1, -1);
        }
    }
    sbs = new wxStaticBoxSizer(wxVERTICAL, this, FFQS(SID_NOISE_STRENGTH));
    sbs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    fgs->Add(sbs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    //Flags
    fgsx = new wxFlexGridSizer(COMPONENT_COUNT, 6, 0, 0);
    wxString *fn = FFQL()->GetStringArray(SID_NOISE_FLAG_NAMES, 4);
    for (int i = 0; i < COMPONENT_COUNT; i++)
    {
        m_Flags[i].use = new wxCheckBox(this, wxID_ANY, cn[i]);
        fgsx->Add(m_Flags[i].use, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
        fgsx->Add(10, -1);

        m_Flags[i].a = new wxCheckBox(this, wxID_ANY, fn[0]);
        fgsx->Add(m_Flags[i].a, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
        m_Flags[i].p = new wxCheckBox(this, wxID_ANY, fn[1]);
        fgsx->Add(m_Flags[i].p, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
        m_Flags[i].t = new wxCheckBox(this, wxID_ANY, fn[2]);
        fgsx->Add(m_Flags[i].t, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
        m_Flags[i].u = new wxCheckBox(this, wxID_ANY, fn[3]);
        fgsx->Add(m_Flags[i].u, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

    }
    delete[] fn;
    sbs = new wxStaticBoxSizer(wxVERTICAL, this, FFQS(SID_NOISE_FLAGS));
    sbs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    fgs->Add(sbs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    delete[] cn;

    //GetTimeLimitControls(fgs, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, true);

    Connect(wxID_ANY, wxEVT_CHECKBOX,(wxObjectEventFunction)&Noise::OnCtrlChange);
    Connect(wxID_ANY, wxEVT_SLIDER, (wxObjectEventFunction)&Noise::OnCtrlChange);

    fgs->Add(GetTimeLimitControls(true), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Noise::~Noise()
{

}

//---------------------------------------------------------------------------------------

void Noise::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Get seeds
        for (int i = 0; i < COMPONENT_COUNT; i++)
        {
            m_Seed[i].use->SetValue(GetToken(fs, ",") == STR_YES);
            m_Seed[i].val->SetValue(GetToken(fs, ","));
        }

        //Get strengths
        for (int i = 0; i < COMPONENT_COUNT; i++)
        {
            m_Strength[i].use->SetValue(GetToken(fs, ",") == STR_YES);
            m_Strength[i].val->SetValue(Str2Long(GetToken(fs, ","), 1));
        }

        //Get flags
        for (int i = 0; i < COMPONENT_COUNT; i++)
        {
            m_Flags[i].use->SetValue(GetToken(fs, ",") == STR_YES);
            wxString f = GetToken(fs, ",");
            m_Flags[i].a->SetValue(f.Find('a') != wxNOT_FOUND);
            m_Flags[i].p->SetValue(f.Find('p') != wxNOT_FOUND);
            m_Flags[i].t->SetValue(f.Find('t') != wxNOT_FOUND);
            m_Flags[i].u->SetValue(f.Find('u') != wxNOT_FOUND);
        }

    }
    else
    {

        //Default values
        m_Seed[0].use->SetValue(true);
        m_Seed[0].val->SetValue("123457");

        m_Strength[0].use->SetValue(true);
        m_Strength[0].val->SetValue(25);

        m_Flags[0].use->SetValue(true);
        m_Flags[0].t->SetValue(true);
        m_Flags[0].u->SetValue(true);

    }

    SetTimeLimitValues(fs);
    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool Noise::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftNOISE;

    wxString seed, stren, flag, edit, useed, ustren, uflag, n, s;

    //Pack seeds
    for (int i = 0; i < COMPONENT_COUNT; i++)
    {

        if (m_Seed[i].use->GetValue())
        {

            //For ffmpeg
            n = (i == 0) ? "all_seed=" : wxString::Format("c%i_seed=", i - 1);
            int64_t v = Str2Long(m_Seed[i].val->GetValue(), -1);
            if (v < 0) return ShowError(m_Seed[i].val, FFQS(SID_NOISE_SEED_ERROR));
            seed += n + ToStr(v) + ":";

            //For user
            n = (i == 0) ? "all" : wxString(RGBA.at(i-1));
            useed += n + "=" + m_Seed[i].val->GetValue() + " ";

        }

        //For edit
        edit += BOOLSTR(m_Seed[i].use->GetValue()) + "," + m_Seed[i].val->GetValue() + ",";

    }

    //Pack strengths
    for (int i = 0; i < COMPONENT_COUNT; i++)
    {

        s = ToStr(m_Strength[i].val->GetValue());

        if (m_Strength[i].use->GetValue())
        {

            //For ffmpeg
            n = (i == 0) ? "alls=" : wxString::Format("c%is=", i - 1);
            stren += n + s + ":";

            //For user
            n = (i == 0) ? "all" : wxString(RGBA.at(i-1));
            ustren += n + "=" + s + " ";

        }

        //For edit
        edit += BOOLSTR(m_Strength[i].use->GetValue()) + "," + s + ",";

    }

    //Validate that noise is applied
    if (stren.Len() == 0) return ShowError(m_Strength[0].use, FFQS(SID_NOISE_STRENGTH_ERROR));

    //Pack flags
    for (int i = 0; i < COMPONENT_COUNT; i++)
    {

        s = "";
        if (m_Flags[i].a->GetValue()) s += "a+";
        if (m_Flags[i].p->GetValue()) s += "p+";
        if (m_Flags[i].t->GetValue()) s += "t+";
        if (m_Flags[i].u->GetValue()) s += "u+";
        if (s.Len() > 0) s.Remove(s.Len() - 1, 1);

        if (m_Flags[i].use->GetValue())
        {

            if (s.Len() == 0) return ShowError(m_Flags[i].a, FFQS(SID_NOISE_FLAGS_ERROR));

            //For ffmpeg
            flag += ((i == 0) ? "allf=" : wxString::Format("c%if=", i - 1)) + s + ":";

            //For user
            n = (i == 0) ? "all" : wxString(RGBA.at(i-1));
            uflag += n + "=" + s + " ";

        }

        //For edit
        s.Replace("+", "");
        edit += BOOLSTR(m_Flags[i].use->GetValue()) + "," + s + ",";

    }

    //Get time limits
    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;
    wxString tf, ft;
    GetTimeLimitFilter(tf, true);
    GetTimeLimitFriendly(ft);

    //Remove trailing separators
    if (seed.Len() > 0) seed.Remove(seed.Len() - 1, 1);
    if (useed.Len() > 0) useed.Remove(useed.Len() - 1, 1);
    if (stren.Len() > 0) stren.Remove(stren.Len() - 1, 1);
    if (ustren.Len() > 0) ustren.Remove(ustren.Len() - 1, 1);
    if (flag.Len() > 0) flag.Remove(flag.Len() - 1, 1);
    if (uflag.Len() > 0) uflag.Remove(uflag.Len() - 1, 1);

    //Combine ffmpeg values
    s = seed;
    if (stren.Len() > 0)
    {
        if (s.Len() > 0) s += ":";
        s += stren;
    }
    if (flag.Len() > 0)
    {
        if (s.Len() > 0) s += ":";
        s += flag;
    }
    s = "noise=" + s;

    fltr->friendly = FFQSF(SID_NOISE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], useed, ustren, uflag, ft);
    fltr->ff_filter.Printf("%s%s%s%s", FILTER_VIDEO_IN, s, tf, FILTER_VIDEO_OUT);
    fltr->editable = edit + wxString::Format(UINT64FMT + "," + UINT64FMT, t1, t2);

    return true;

}

//---------------------------------------------------------------------------------------

void Noise::UpdateControls()
{
    bool b;
    for (int i = 0; i < COMPONENT_COUNT; i++)
    {
        m_Seed[i].val->Enable(m_Seed[i].use->GetValue());
        b = m_Strength[i].use->GetValue();
        m_Strength[i].val->Enable(b);
        m_Strength[i].show->Enable(b);
        m_Strength[i].show->SetLabel(ToStr(m_Strength[i].val->GetValue()));
        b = m_Flags[i].use->GetValue();
        m_Flags[i].a->Enable(b);
        m_Flags[i].p->Enable(b);
        m_Flags[i].t->Enable(b);
        m_Flags[i].u->Enable(b);
    }
}

//---------------------------------------------------------------------------------------

void Noise::OnCtrlChange(wxCommandEvent &event)
{
    event.Skip();
    UpdateControls();
}
