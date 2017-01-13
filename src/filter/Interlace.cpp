/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Interlace.cpp                                                   *
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

#include "Interlace.h"
#include <wx/statline.h>

const int TEMPORAL_MODE_COUNT = 8;
const wxString TEMPORAL_MODES[] = {"merge","drop_odd","drop_even","pad","interleave_top","interleave_bottom","interlacex2", "mergex2"};

//---------------------------------------------------------------------------------------

Interlace::Interlace(wxWindow* parent) : FilterBasePanel(parent)
{

	//Create outer sizer
    wxFlexGridSizer *outer = new wxFlexGridSizer(6, 1, 0, 0), *fgs;
	outer->AddGrowableCol(0);

	//Add filter selection to outer
	fgs = new wxFlexGridSizer(1, 3, 0, 0);
	fgs->AddGrowableCol(1);
	fgs->AddGrowableCol(2);
	MakeLabel(FFQS(SID_INTERLACE_FILTER_TYPE), fgs);
	wxString *as = FFQL()->GetStringArray(SID_INTERLACE_FILTER_TYPE_NAMES, 2);
	m_Default = new wxRadioButton(this, wxID_ANY, as[0]);
	m_Default->SetValue(true);
    m_Default->Bind(wxEVT_RADIOBUTTON, &Interlace::OnAction, this);
	fgs->Add(m_Default, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);
	m_Temporal = new wxRadioButton(this, wxID_ANY, as[1]);
    m_Temporal->Bind(wxEVT_RADIOBUTTON, &Interlace::OnAction, this);
	fgs->Add(m_Temporal, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);
	outer->Add(fgs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
	delete[] as;


	//Add separator line
	outer->Add(new wxStaticLine(this), 1, wxTOP|wxBOTTOM|wxEXPAND|wxALIGN_CENTER_VERTICAL, 10);


    //Default interlace
    m_DefaultPan = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    SetControlParent(m_DefaultPan);
    fgs = new wxFlexGridSizer(1, 3, 0, 0);
	fgs->AddGrowableRow(0);
	fgs->AddGrowableCol(1);
	fgs->AddGrowableCol(2);

    MakeLabel(FFQS(SID_INTERLACE_SCAN), fgs);
    m_ScanEven = new wxRadioButton(m_DefaultPan, wxID_ANY, FFQS(SID_INTERLACE_EVEN));
    fgs->Add(m_ScanEven, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);
    m_ScanOdd = new wxRadioButton(m_DefaultPan, wxID_ANY, FFQS(SID_INTERLACE_ODD));
    fgs->Add(m_ScanOdd, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);

    m_DefaultPan->SetSizer(fgs);
    fgs->Fit(m_DefaultPan);
    fgs->SetSizeHints(m_DefaultPan);
	outer->Add(m_DefaultPan, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

	//Temporal interlace
    m_TemporalPan = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    SetControlParent(m_TemporalPan);
    fgs = new wxFlexGridSizer(1, 2, 0, 0);
	fgs->AddGrowableRow(0);
	fgs->AddGrowableCol(1);

    MakeLabel(FFQS(SID_INTERLACE_TINTRL_MODE), fgs);
	as = FFQL()->GetStringArray(SID_INTERLACE_TINTRL_MODE_NAMES, 8);
    m_TemporalMode = new wxChoice(m_TemporalPan, wxID_ANY, wxDefaultPosition, wxDefaultSize, TEMPORAL_MODE_COUNT, as);
    fgs->Add(m_TemporalMode, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    delete[] as;

    m_TemporalPan->SetSizer(fgs);
    fgs->Fit(m_TemporalPan);
    fgs->SetSizeHints(m_TemporalPan);
	outer->Add(m_TemporalPan, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

	//Spacer
	outer->Add(-1, 5);

    //Low-pass (used by both above)
    m_LowPass = new wxCheckBox(this, wxID_ANY, FFQS(SID_INTERLACE_LOWPASS));
    outer->Add(m_LowPass, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    wxSize dp = m_DefaultPan->GetSize(), tp = m_TemporalPan->GetSize();
    wxSize ms(-1, dp.GetHeight() > tp.GetHeight() ? dp.GetHeight() : tp.GetHeight());
    m_DefaultPan->SetMinSize(ms);
    m_TemporalPan->SetMinSize(ms);

	m_DefaultPan->Hide();

    SetSizer(outer);
    outer->Fit(this);
    outer->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Interlace::~Interlace()
{

}

//---------------------------------------------------------------------------------------

void Interlace::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    //Load default interlace options
    m_ScanEven->SetValue(STRBOOLDEF(GetToken(fs, ','), true));
    m_ScanOdd->SetValue(!m_ScanEven->GetValue());
    m_LowPass->SetValue(STRBOOLDEF(GetToken(fs, ','), true));

    //Load filter selection
    m_Temporal->SetValue(STRBOOL(GetToken(fs, ',')));
    m_Default->SetValue(!m_Temporal->GetValue());

    //Load temporal values
    m_TemporalMode->SetSelection(Str2Long(GetToken(fs, ','), 0));

    //Select panel
    SwapPanels(m_Temporal->GetValue() ? m_TemporalPan : m_DefaultPan);

}

//---------------------------------------------------------------------------------------

bool Interlace::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftINTERLACE;

    wxString f, uf;

    if (m_Default->GetValue())
    {

        //Default filter
        f = "interlace=";
        if (m_ScanOdd->GetValue()) f += "scan=bff:";
        if (!m_LowPass->GetValue()) f += "lowpass=0:";
        uf = m_ScanEven->GetValue() ? m_ScanEven->GetLabel().Lower() : m_ScanOdd->GetLabel().Lower();

    }

    else
    {

        //Temporal filter
        uf = TEMPORAL_MODES[m_TemporalMode->GetSelection()];
        f = "tinterlace=";
        if (m_TemporalMode->GetSelection() > 0) f+= wxString::Format("mode=%s:", uf);
        if (m_LowPass->GetValue()) f+= "flags=vlfp:";

    }

    //Remove last ":" or "="
    f.RemoveLast();

    fltr->ff_filter.Printf("%s%s%s", FILTER_VIDEO_IN, f, FILTER_VIDEO_OUT);

    fltr->friendly = FFQSF(SID_INTERLACE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                           m_Default->GetValue() ? m_Default->GetLabel().Lower() : m_Temporal->GetLabel().Lower(),
                           uf, BOOLSTR(m_LowPass->GetValue()));

    fltr->editable.Printf("%s,%s,%s,%i",
                          BOOLSTR(m_ScanEven->GetValue()),
                          BOOLSTR(m_LowPass->GetValue()),
                          BOOLSTR(m_Temporal->GetValue()),
                          m_TemporalMode->GetSelection());

    return true;

}

//---------------------------------------------------------------------------------------

void Interlace::OnAction(wxCommandEvent &event)
{

    //Select panel
    if (event.GetEventObject() == m_Default) SwapPanels(m_DefaultPan);
    else if (event.GetEventObject() == m_Temporal) SwapPanels(m_TemporalPan);

}

//---------------------------------------------------------------------------------------

void Interlace::SwapPanels(wxPanel *select)
{

    if (select == NULL) select = m_DefaultPan->IsShown() ? m_TemporalPan : m_DefaultPan;
    m_DefaultPan->Show(select == m_DefaultPan);
    m_TemporalPan->Show(select == m_TemporalPan);
    Layout();

}
