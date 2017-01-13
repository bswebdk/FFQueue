/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Curves.cpp                                                      *
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

#include "Curves.h"

const size_t CURVE_PRESET_COUNT = 11;

//---------------------------------------------------------------------------------------

Curves::Curves(wxWindow* parent) : FilterBasePanel(parent)
{

    m_Sizer = new wxFlexGridSizer(6, 2, 0, 0);
    m_Sizer->AddGrowableCol(1);

    MakeLabel(FFQS(SID_CURVES_PRESET), m_Sizer);
    m_Preset = new wxChoice(this, 1, wxDefaultPosition, wxDefaultSize);
    for (size_t i = 0; i < CURVE_PRESET_COUNT; i++) m_Preset->Append(FFQS(SID_CURVES_PRESET_BASE + i));
    m_Sizer->Add(m_Preset, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_CURVES_MASTER_KEYS), m_Sizer);
    m_Master = new wxTextCtrl(this, wxID_ANY);
    m_Sizer->Add(m_Master, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_CURVES_RED_KEYS), m_Sizer);
    m_Red = new wxTextCtrl(this, wxID_ANY);
    m_Sizer->Add(m_Red, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_CURVES_GREEN_KEYS), m_Sizer);
    m_Green = new wxTextCtrl(this, wxID_ANY);
    m_Sizer->Add(m_Green, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_CURVES_BLUE_KEYS), m_Sizer);
    m_Blue = new wxTextCtrl(this, wxID_ANY);
    m_Sizer->Add(m_Blue, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_CURVES_ALL_KEYS), m_Sizer);
    m_All = new wxTextCtrl(this, wxID_ANY);
    m_Sizer->Add(m_All, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(m_Sizer);
    m_Sizer->Fit(this);
    m_Sizer->SetSizeHints(this);

	Connect(1, wxEVT_CHOICE, (wxObjectEventFunction)&Curves::OnChoiceChange);
}

//---------------------------------------------------------------------------------------

Curves::~Curves()
{

}

//---------------------------------------------------------------------------------------

void Curves::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Preset->SetSelection(Str2Long(GetToken(fs, ','), 0));
        m_Master->SetValue(GetToken(fs, ','));
        m_Red->SetValue(GetToken(fs, ','));
        m_Green->SetValue(GetToken(fs, ','));
        m_Blue->SetValue(GetToken(fs, ','));
        m_All->SetValue(GetToken(fs, ','));

    }

    else
    {

        //Default values
        m_Preset->SetSelection(0);
        m_Master->SetValue("");
        m_Red->SetValue("");
        m_Green->SetValue("");
        m_Blue->SetValue("");
        m_All->SetValue("");

    }

    UpdateCtrls();

}

//---------------------------------------------------------------------------------------

bool GetVal(wxTextCtrl *from, wxString &to)
{
    to = StrTrim(from->GetValue());
    if ((to.Find(",") >= 0) || (to.Find("'") >= 0) || (to.Find("\"") >= 0)) return ShowError(from, FFQS(SID_CURVES_ERROR));
    return true;
}

//---------------------------------------------------------------------------------------

wxString FmtVal(wxString val, wxString argN)
{
    if (val.Len() > 0) return ":" + argN + "='" + val + "'";
    return "";
}

//---------------------------------------------------------------------------------------

bool Curves::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftCURVES;

    wxString pst = m_Preset->GetStringSelection(), mv, rv, gv, bv, av;

    if (!GetVal(m_Master, mv)) return false;
    if (!GetVal(m_Red, rv)) return false;
    if (!GetVal(m_Green, gv)) return false;
    if (!GetVal(m_Blue, bv)) return false;
    if (!GetVal(m_All, av)) return false;

    pst = GetToken(pst, " (", false).Lower(); //Remove info: (..) and lowercase
    pst.Replace(" ", "_"); //Replace space with underscore

    fltr->friendly = FFQSF(SID_CURVES_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], pst, mv, rv, gv, bv, av);

    fltr->ff_filter.Printf("%scurves=preset=%s%s%s%s%s%s%s",
                              FILTER_VIDEO_IN,
                              pst,
                              FmtVal(mv, "m"),
                              FmtVal(rv, "r"),
                              FmtVal(gv, "g"),
                              FmtVal(bv, "b"),
                              FmtVal(av, "a"),
                              FILTER_VIDEO_OUT);

    fltr->editable.Printf("%d,%s,%s,%s,%s,%s", m_Preset->GetSelection(), mv, rv, gv, bv, av);

    return true;

}

//---------------------------------------------------------------------------------------

void Curves::OnChoiceChange(wxCommandEvent& event)
{
    event.Skip();
    if (event.GetEventObject() == m_Preset) UpdateCtrls();
}

//---------------------------------------------------------------------------------------

void Curves::UpdateCtrls()
{
    bool enable = m_Preset->GetSelection() == 0;
    for (size_t i = 2; i < m_Sizer->GetItemCount(); i++)
    {
        wxSizerItem *si = m_Sizer->GetItem(i);
        if (si->GetWindow() != NULL) si->GetWindow()->Enable(enable);
    }
}
