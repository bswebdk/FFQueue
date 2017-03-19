/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Perspective.cpp                                                 *
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

#include "Perspective.h"

Perspective::Perspective(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(6, 2, 0, 0);
    fgs->AddGrowableCol(1);

    //Slider labels
    fgs->Add(-1, -1); //Spacer
    MakeSliderLabels(fgs, FFQS(SID_PERSPECTIVE_SQUEEZE), "0", FFQS(SID_PERSPECTIVE_STRETCH), false);

    //Left
    MakeLabel(FFQS(SID_PERSPECTIVE_LEFT), fgs);
    m_Left = new wxSlider(this, wxID_ANY, 100, 0, 200, wxDefaultPosition, wxSize(300, -1));
    fgs->Add(m_Left, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Right
    MakeLabel(FFQS(SID_PERSPECTIVE_RIGHT), fgs);
    m_Right = new wxSlider(this, wxID_ANY, 100, 0, 200, wxDefaultPosition, wxSize(300, -1));
    fgs->Add(m_Right, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Top
    MakeLabel(FFQS(SID_PERSPECTIVE_TOP), fgs);
    m_Top = new wxSlider(this, wxID_ANY, 100, 0, 200, wxDefaultPosition, wxSize(300, -1));
    fgs->Add(m_Top, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Bottom
    MakeLabel(FFQS(SID_PERSPECTIVE_BOTTOM), fgs);
    m_Bottom = new wxSlider(this, wxID_ANY, 100, 0, 200, wxDefaultPosition, wxSize(300, -1));
    fgs->Add(m_Bottom, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Interpolation
    fgs->Add(-1, -1); //Spacer
    m_InterpolCubic = new wxCheckBox(this, wxID_ANY, FFQS(SID_PERSPECTIVE_INTERPOL_CUBIC));
    fgs->Add(m_InterpolCubic, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    //Time line
    GetTimeLimitControls(fgs, 0, true);

    //SetSizer(fgs);
    //fgs->Fit(this);
    //fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Perspective::~Perspective()
{

}

//---------------------------------------------------------------------------------------

void Perspective::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Left->SetValue(Str2Long(GetToken(fs, ","), m_Left->GetValue()));
        m_Right->SetValue(Str2Long(GetToken(fs, ","), m_Right->GetValue()));
        m_Top->SetValue(Str2Long(GetToken(fs, ","), m_Top->GetValue()));
        m_Bottom->SetValue(Str2Long(GetToken(fs, ","), m_Bottom->GetValue()));
        m_InterpolCubic->SetValue(STRBOOL(GetToken(fs, ",")));

    }

    SetTimeLimitValues(fs);

}

//---------------------------------------------------------------------------------------

wxString Friendly(int val)
{
    if (val < 0) return wxString::Format(">%.2g<", (float)abs(val) / 200.0);
    else if (val > 0) return wxString::Format("<%.2g>", (float)val / 200.0);
    else return "0";
}

//---------------------------------------------------------------------------------------

bool Perspective::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftPERSPECTIVE;

    int l = m_Left->GetValue() - 100,
        r = m_Right->GetValue() - 100,
        t = m_Top->GetValue() - 100,
        b = m_Bottom->GetValue() - 100;

    if ((l == 0) && (r == 0) && (t == 0) && (b == 0)) return ShowError(FFQS(SID_PERSPECTIVE_ERROR));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;

    bool ic = m_InterpolCubic->GetValue();
    wxString f, s, tl, ics = ic ? ":interpolation=cubic" : "";
    float v;

    //Time limit
    GetTimeLimitFilter(tl, true);

    //Left
    if (l != 0)
    {
        s = (l < 0) ? "1" : "0"; //Squeeze or stretch
        v = (float)abs(l) / 400.0; //Fraction
        f += wxString::Format("perspective=y0=H*%g:y2=H-(H*%g):sense=%s%s%s,", v, v, s, ics, tl);
    }

    //Right
    if (r != 0)
    {
        s = (r < 0) ? "1" : "0"; //Squeeze or stretch
        v = (float)abs(r) / 400.0; //Fraction
        f += wxString::Format("perspective=y1=H*%g:y3=H-(H*%g):sense=%s%s%s,", v, v, s, ics, tl);
    }

    //Top
    if (t != 0)
    {
        s = (t < 0) ? "1" : "0"; //Squeeze or stretch
        v = (float)abs(t) / 400.0; //Fraction
        f += wxString::Format("perspective=x0=W*%g:x1=W-(W*%g):sense=%s%s%s,", v, v, s, ics, tl);
    }

    //Bottom
    if (b != 0)
    {
        s = (b < 0) ? "1" : "0"; //Squeeze or stretch
        v = (float)abs(b) / 400.0; //Fraction
        f += wxString::Format("perspective=x2=W*%g:x3=W-(W*%g):sense=%s%s%s,", v, v, s, ics, tl);
    }

    //Remove last comma
    f.Remove(f.Len() - 1, 1);

    //Friendly time limit
    GetTimeLimitFriendly(tl);

    fltr->friendly = FFQSF(SID_PERSPECTIVE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                           Friendly(l), Friendly(r), Friendly(t), Friendly(b), BOOLSTR(ic), tl);
    fltr->ff_filter.Printf("%s%s%s", FILTER_VIDEO_IN, f, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%i,%i,%i,%i,%s,"+UINT64FMT+","+UINT64FMT, l+100, r+100, t+100, b+100, BOOLSTR(ic), t1, t2);

    return true;

}
