/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: LensCorrection.cpp                                              *
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

#include "LensCorrection.h"

#define CALC_COORD(v) ( (double)(v) / 400.0 )
#define CALC_COEF(v) ( ((double)(v) / 200.0) - 1.0 )

//---------------------------------------------------------------------------------------

LensCorrection::LensCorrection(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs;

    fgs = new wxFlexGridSizer(4, 6, 0, 0); //Rows, Cols, HGap, VGap
    fgs->AddGrowableCol(2);

    wxString *as = FFQL()->GetStringArray(SID_LENSCORRECTION_CX_CY_TEXT, 4);

    MakeLabel(FFQS(SID_LENSCORRECTION_CX), fgs);
    MakeLabel(as[0], fgs);
    m_CX = new wxSlider(this, wxID_ANY, 200, 0, 400, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_CX, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    MakeLabel(as[1], fgs);
    fgs->Add(20, -1);
    m_CXText = MakeLabel("000000", fgs);
    wxSize ms = wxSize(m_CXText->GetSize().GetWidth(), -1);
    m_CXText->SetMinSize(ms);

    MakeLabel(FFQS(SID_LENSCORRECTION_CY), fgs);
    MakeLabel(as[2], fgs);
    m_CY = new wxSlider(this, wxID_ANY, 200, 0, 400, wxDefaultPosition, wxSize(-1, -1));
    fgs->Add(m_CY, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    MakeLabel(as[3], fgs);
    fgs->Add(20, -1);
    m_CYText = MakeLabel("", fgs);
    m_CYText->SetMinSize(ms);

    delete[] as;
    as = FFQL()->GetStringArray(SID_LENSCORRECTION_K1_K2_TEXT, 2);

    MakeLabel(FFQS(SID_LENSCORRECTION_K1), fgs);
    MakeLabel(as[0], fgs);
    m_K1 = new wxSlider(this, wxID_ANY, 200, 0, 400, wxDefaultPosition, wxSize(-1, -1));
    fgs->Add(m_K1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    MakeLabel(as[1], fgs);
    fgs->Add(20, -1);
    m_K1Text = MakeLabel("", fgs);
    m_K1Text->SetMinSize(ms);

    MakeLabel(FFQS(SID_LENSCORRECTION_K2), fgs);
    MakeLabel(as[0], fgs);
    m_K2 = new wxSlider(this, wxID_ANY, 200, 0, 400, wxDefaultPosition, wxSize(-1, -1));
    fgs->Add(m_K2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    MakeLabel(as[1], fgs);
    fgs->Add(20, -1);
    m_K2Text = MakeLabel("", fgs);
    m_K2Text->SetMinSize(ms);

    delete[] as;

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

    Connect(wxID_ANY, wxEVT_SLIDER, (wxObjectEventFunction)&LensCorrection::OnSliderChange);

}

//---------------------------------------------------------------------------------------

LensCorrection::~LensCorrection()
{

}

//---------------------------------------------------------------------------------------

void LensCorrection::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    m_CX->SetValue(Str2Long(GetToken(fs, ",", true), m_CX->GetValue()));
    m_CY->SetValue(Str2Long(GetToken(fs, ",", true), m_CY->GetValue()));
    m_K1->SetValue(Str2Long(GetToken(fs, ",", true), m_K1->GetValue()));
    m_K2->SetValue(Str2Long(GetToken(fs, ",", true), m_K2->GetValue()));

    wxCommandEvent dummy;
    OnSliderChange(dummy);

}

//---------------------------------------------------------------------------------------

bool LensCorrection::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftLENSCORRECTION;

    double cx = CALC_COORD(m_CX->GetValue()), cy = CALC_COORD(m_CY->GetValue()),
           k1 = CALC_COEF(m_K1->GetValue()), k2 = CALC_COEF(m_K2->GetValue());

    if ((k1 == 0) && (k2 == 0)) return ShowError(m_K1, FFQS(SID_LENSCORRECTION_ERROR));

    fltr->friendly = FFQSF(SID_LENSCORRECTION_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], cx, cy, k1, k2);

    fltr->ff_filter.Printf("%slenscorrection=%g:%g:%g:%g%s", FILTER_VIDEO_IN, cx, cy, k1, k2, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%i,%i,%i,%i", m_CX->GetValue(), m_CY->GetValue(), m_K1->GetValue(), m_K2->GetValue());

    return true;

}

//---------------------------------------------------------------------------------------

void LensCorrection::OnSliderChange(wxCommandEvent &event)
{
    event.Skip();
    wxObject *o = event.GetEventObject();
    if ((o == NULL) || (o == m_CX)) m_CXText->SetLabel(wxString::Format("%.3f", CALC_COORD(m_CX->GetValue())));
    if ((o == NULL) || (o == m_CY)) m_CYText->SetLabel(wxString::Format("%.3f", CALC_COORD(m_CY->GetValue())));
    if ((o == NULL) || (o == m_K1)) m_K1Text->SetLabel(wxString::Format("%.3f", CALC_COEF(m_K1->GetValue())));
    if ((o == NULL) || (o == m_K2)) m_K2Text->SetLabel(wxString::Format("%.3f", CALC_COEF(m_K2->GetValue())));
}
