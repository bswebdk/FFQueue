/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Flanger.cpp                                                     *
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

#include "Flanger.h"

Flanger::Flanger(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs1 = new wxFlexGridSizer(8, 2, 0, 0), *fgs2;
    fgs1->AddGrowableCol(1);

    MakeLabel(FFQS(SID_FLANGER_DELAY), fgs1);

        fgs2 = new wxFlexGridSizer(2, 1, 0, 0);
        fgs2->AddGrowableCol(0);
        MakeSliderLabels(fgs2, _T("0"), wxEmptyString, _T("30"), false);
        m_Delay = new wxSlider(this, wxID_ANY, 0, 0, 30, wxDefaultPosition, wxSize(400, -1), wxSL_HORIZONTAL);
        fgs2->Add(m_Delay, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    MakeLabel(FFQS(SID_FLANGER_DEPTH), fgs1);

        fgs2 = new wxFlexGridSizer(2, 1, 0, 0);
        fgs2->AddGrowableCol(0);
        MakeSliderLabels(fgs2, _T("0"), wxEmptyString, _T("10"), false);
        m_Depth = new wxSlider(this, wxID_ANY, 2, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgs2->Add(m_Depth, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    MakeLabel(FFQS(SID_FLANGER_REGENERATION), fgs1);

        fgs2 = new wxFlexGridSizer(2, 1, 0, 0);
        fgs2->AddGrowableCol(0);
        MakeSliderLabels(fgs2, _T("-95"), _T("0"), _T("95"), false);
        m_Regen = new wxSlider(this, wxID_ANY, 95, 0, 190, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgs2->Add(m_Regen, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    MakeLabel(FFQS(SID_FLANGER_WIDTH), fgs1);

        fgs2 = new wxFlexGridSizer(2, 1, 0, 0);
        fgs2->AddGrowableCol(0);
        MakeSliderLabels(fgs2, _T("0"), wxEmptyString, _T("100"), false);
        m_Width = new wxSlider(this, wxID_ANY, 71, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgs2->Add(m_Width, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    MakeLabel(FFQS(SID_FLANGER_SPEED), fgs1);

        fgs2 = new wxFlexGridSizer(2, 1, 0, 0);
        fgs2->AddGrowableCol(0);
        MakeSliderLabels(fgs2, _T("0.1"), wxEmptyString, _T("10"), false);
        m_Speed = new wxSlider(this, wxID_ANY, 5, 1, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgs2->Add(m_Speed, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    MakeLabel(FFQS(SID_FLANGER_PHASE), fgs1);

        fgs2 = new wxFlexGridSizer(2, 1, 0, 0);
        fgs2->AddGrowableCol(0);
        MakeSliderLabels(fgs2, _T("0"), wxEmptyString, _T("100"), false);
        m_Phase = new wxSlider(this, wxID_ANY, 25, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgs2->Add(m_Phase, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    wxPanel *pan;
    wxBoxSizer *bs;

    MakeLabel(FFQS(SID_FLANGER_SHAPE), fgs1);

        pan = new wxPanel(this);
        bs = new wxBoxSizer(wxHORIZONTAL);
        m_ShapeSin = new wxRadioButton(pan, wxID_ANY, FFQS(SID_FLANGER_SHAPE_SIN));
        bs->Add(m_ShapeSin, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
        m_ShapeTri = new wxRadioButton(pan, wxID_ANY, FFQS(SID_FLANGER_SHAPE_TRI));
        bs->Add(m_ShapeTri, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
        pan->SetSizer(bs);
        bs->Fit(pan);
        bs->SetSizeHints(pan);

    fgs1->Add(pan, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);

    MakeLabel(FFQS(SID_FLANGER_INTERPOLATION), fgs1);

        pan = new wxPanel(this);
        bs = new wxBoxSizer(wxHORIZONTAL);
        m_InterLin = new wxRadioButton(pan, wxID_ANY, FFQS(SID_FLANGER_INTERPOLATION_LINE));
        bs->Add(m_InterLin, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
        m_InterQuad = new wxRadioButton(pan, wxID_ANY, FFQS(SID_FLANGER_INTERPOLATION_QUAD));
        bs->Add(m_InterQuad, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
        pan->SetSizer(bs);
        bs->Fit(pan);
        bs->SetSizeHints(pan);

    fgs1->Add(pan, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);

    SetSizer(fgs1);
    fgs1->Fit(this);
    fgs1->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Flanger::~Flanger()
{

}

//---------------------------------------------------------------------------------------

void Flanger::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Delay->SetValue(Str2Long(GetToken(fs, ',')));
        m_Depth->SetValue(Str2Long(GetToken(fs, ',')));
        m_Regen->SetValue(Str2Long(GetToken(fs, ',')));
        m_Width->SetValue(Str2Long(GetToken(fs, ',')));
        m_Speed->SetValue(Str2Long(GetToken(fs, ',')));
        m_Phase->SetValue(Str2Long(GetToken(fs, ',')));
        m_ShapeTri->SetValue(GetToken(fs, ',') == "t");
        m_InterLin->SetValue(GetToken(fs, ',') == "l");

    }

    else
    {

        //Default values
        m_Delay->SetValue(0);
        m_Depth->SetValue(2);
        m_Regen->SetValue(95);
        m_Width->SetValue(71);
        m_Speed->SetValue(5);
        m_Phase->SetValue(25);
        m_ShapeSin->SetValue(true);
        m_InterLin->SetValue(true);

    }

    m_ShapeSin->SetValue(!m_ShapeTri->GetValue());
    m_InterQuad->SetValue(!m_InterLin->GetValue());

}

//---------------------------------------------------------------------------------------

bool Flanger::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftFLANGER;

    int delay = m_Delay->GetValue(),
        depth = m_Depth->GetValue(),
        regen = m_Regen->GetValue(),
        width = m_Width->GetValue(),
        speed = m_Speed->GetValue(),
        phase = m_Phase->GetValue();

    double spd = speed / 10.0;

    wxString shape = m_ShapeSin->GetValue() ? m_ShapeSin->GetLabel() : m_ShapeTri->GetLabel(),
             inter = m_InterLin->GetValue() ? m_InterLin->GetLabel() : m_InterQuad->GetLabel();

    shape.LowerCase();
    inter.LowerCase();

    fltr->friendly = FFQSF(SID_FLANGER_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], delay, depth, regen-95, width, spd, phase, shape, inter);

    fltr->ff_filter.Printf("%sflanger=%i:%i:%i:%i:%g:%s:%i:%s%s", FILTER_AUDIO_IN, delay, depth, regen-95, width, spd, shape, phase, inter, FILTER_AUDIO_OUT);

    shape = shape.GetChar(0);
    inter = inter.GetChar(0);

    fltr->editable.Printf("%i,%i,%i,%i,%i,%i,%s,%s", delay, depth, regen, width, speed, phase, shape, inter);

    return true;

}
