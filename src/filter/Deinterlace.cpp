/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Deinterlace.cpp                                                 *
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

#include "Deinterlace.h"
#include <wx/statline.h>

const unsigned int DEINTER_MODE_COUNT = 4;
const unsigned int DEINTER_PARITY_COUNT = 3;
const unsigned int DEINTER_DEINTERLACE_COUNT = 2;

//---------------------------------------------------------------------------------------

Deinterlace::Deinterlace(wxWindow* parent) : FilterBasePanel(parent)
{

    //Handy vars
    wxFlexGridSizer *fgs, *fgsx, *outer;
    wxBoxSizer *bs;
    wxPanel *grp;
	wxString *as;

	//Create outer sizer
	outer = new wxFlexGridSizer(4, 1, 0, 0);
	outer->AddGrowableCol(0);

	//Add filter selection to outer
	fgs = new wxFlexGridSizer(1, 3, 0, 0);
	fgs->AddGrowableCol(1);
	fgs->AddGrowableCol(2);
	MakeLabel(FFQS(SID_DEINTER_FILTER_TYPE), fgs);
	as = FFQL()->GetStringArray(SID_DEINTER_FILTER_TYPE_NAMES, 2);
	m_Yadif = new wxRadioButton(this, wxID_ANY, as[0]);
	m_Yadif->SetValue(true);
    m_Yadif->Bind(wxEVT_RADIOBUTTON, &Deinterlace::OnAction, this);
	fgs->Add(m_Yadif, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);
	m_KernDeint = new wxRadioButton(this, wxID_ANY, as[1]);
    m_KernDeint->Bind(wxEVT_RADIOBUTTON, &Deinterlace::OnAction, this);
	fgs->Add(m_KernDeint, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);
	outer->Add(fgs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
	delete[] as;

	//Add separator line
	outer->Add(new wxStaticLine(this), 1, wxTOP|wxBOTTOM|wxEXPAND|wxALIGN_CENTER_VERTICAL, 10);

    //Create yadif page and set as control parent (in MakeLabel etc.)
    m_YadifPan = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    SetControlParent(m_YadifPan);

    //Create yadif page
    fgs = new wxFlexGridSizer(3, 2, 0, 0);
    fgs->AddGrowableCol(1);

	//Mode
	MakeLabel(FFQS(SID_DEINTER_MODE_NAME), fgs);
	as = FFQL()->GetStringArray(SID_DEINTER_MODE_VALUES, DEINTER_MODE_COUNT);
	m_DeintMode = new wxChoice(m_YadifPan, wxID_ANY, wxDefaultPosition, wxDefaultSize, DEINTER_MODE_COUNT, as);
    m_DeintMode->SetSelection(0);
	delete[] as;
	fgs->Add(m_DeintMode, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

	//Parity
	MakeLabel(FFQS(SID_DEINTER_PARITY_NAME), fgs);
	as = FFQL()->GetStringArray(SID_DEINTER_PARITY_VALUES, DEINTER_PARITY_COUNT);
	grp = new wxPanel(m_YadifPan, wxID_ANY);
	bs = new wxBoxSizer(wxHORIZONTAL);
	m_ParityAuto = new wxRadioButton(grp, wxID_ANY, as[0], wxDefaultPosition, wxDefaultSize, 0);
	bs->Add(m_ParityAuto, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	m_ParityTop = new wxRadioButton(grp, wxID_ANY, as[1], wxDefaultPosition, wxDefaultSize, 0);
	bs->Add(m_ParityTop, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	m_ParityBottom = new wxRadioButton(grp, wxID_ANY, as[2], wxDefaultPosition, wxDefaultSize, 0);
	bs->Add(m_ParityBottom, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	grp->SetSizer(bs);
	bs->Fit(grp);
	bs->SetSizeHints(grp);
	fgs->Add(grp, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
	delete[] as;

	//Limit frames
	MakeLabel(FFQS(SID_DEINTER_DEINTERLACE_NAME), fgs);
	as = FFQL()->GetStringArray(SID_DEINTER_DEINTERLACE_VALUES, DEINTER_DEINTERLACE_COUNT);
	grp = new wxPanel(m_YadifPan, wxID_ANY);
	bs = new wxBoxSizer(wxHORIZONTAL);
	m_FramesAll = new wxRadioButton(grp, wxID_ANY, as[0], wxDefaultPosition, wxDefaultSize, 0);
	bs->Add(m_FramesAll, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	m_FramesInterlaced = new wxRadioButton(grp, wxID_ANY, as[1], wxDefaultPosition, wxDefaultSize, 0);
	bs->Add(m_FramesInterlaced, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	bs->Add(-1, -1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	grp->SetSizer(bs);
	bs->Fit(grp);
	bs->SetSizeHints(grp);
	fgs->Add(grp, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
	delete[] as;

	//Set panel sizer and add to outer
    m_YadifPan->Hide();
    m_YadifPan->SetSizer(fgs);
    fgs->Fit(m_YadifPan);
    fgs->SetSizeHints(m_YadifPan);
	outer->Add(m_YadifPan, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);


    //Create kerndeint page
    m_KernDeintPan = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    SetControlParent(m_KernDeintPan);

    //Create kerndeint controls
    fgs = new wxFlexGridSizer(5, 1, 0, 0);
    fgs->AddGrowableCol(0);

    //Threshold (2 rows)
    fgsx = new wxFlexGridSizer(6, 3, 0, 0);
    fgsx->AddGrowableCol(1);
    fgsx->Add(10, 10); //Spacer
    MakeSliderLabels(fgsx, FFQS(SID_PRESET_LOW), _(""), FFQS(SID_PRESET_HIGH), false);
    fgsx->Add(10, 10);
    MakeLabel(FFQS(SID_DEINTER_KDEINT_TRESH), fgsx);
    m_Threshold = new wxSlider(m_KernDeintPan, wxID_ANY, 10, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_TOP|wxSL_INVERSE);
    m_Threshold->SetMinSize(wxSize(300, -1));
    m_Threshold->Bind(wxEVT_SLIDER, &Deinterlace::OnAction, this);
    fgsx->Add(m_Threshold, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_ThreshLab = MakeLabel("XXXX", fgsx, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
    int w, h;
    m_ThreshLab->GetTextExtent(m_ThreshLab->GetLabel(), &w, &h);
    m_ThreshLab->SetMinSize(wxSize(w, -1));
    fgs->Add(fgsx, 1, wxALL|wxEXPAND|wxALIGN_TOP, 0);

    //Checks
    fgsx = new wxFlexGridSizer(2, 2, 0, 0);
    m_MapToWhite = new wxCheckBox(m_KernDeintPan, wxID_ANY, FFQS(SID_DEINTER_KDEINT_MAP));
    fgsx->Add(m_MapToWhite, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    m_SwapOrder = new wxCheckBox(m_KernDeintPan, wxID_ANY, FFQS(SID_DEINTER_KDEINT_ORDER));
    fgsx->Add(m_SwapOrder, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    m_AddSharp = new wxCheckBox(m_KernDeintPan, wxID_ANY, FFQS(SID_DEINTER_KDEINT_SHARP));
    fgsx->Add(m_AddSharp, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    m_TwoWaySharp = new wxCheckBox(m_KernDeintPan, wxID_ANY, FFQS(SID_DEINTER_KDEINT_TWOWAY));
    fgsx->Add(m_TwoWaySharp, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    fgs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);


	//Set size and add
    m_KernDeintPan->SetSizer(fgs);
    fgs->Fit(m_KernDeintPan);
    fgs->SetSizeHints(m_KernDeintPan);
	outer->Add(m_KernDeintPan, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    //Set outer sizer
    SetSizer(outer);
    outer->Fit(this);
    outer->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Deinterlace::~Deinterlace()
{

}

//---------------------------------------------------------------------------------------

void Deinterlace::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    //Load yadif values from filter string
    m_DeintMode->SetSelection(Str2Long(GetToken(fs, ','), 0));

    wxString s = GetToken(fs, ',');
    m_ParityTop->SetValue(s == "0");
    m_ParityBottom->SetValue(s == "1");
    m_ParityAuto->SetValue( (!m_ParityTop->GetValue()) && (!m_ParityBottom->GetValue()) );

    m_FramesInterlaced->SetValue(GetToken(fs, ',') == "1");
    m_FramesAll->SetValue(!m_FramesInterlaced->GetValue());

    //Load filter type
    m_KernDeint->SetValue(STRBOOL(GetToken(fs, ',')));
    m_Yadif->SetValue(!m_KernDeint->GetValue());

    //Load kerndeint values
    m_Threshold->SetValue(Str2Long(GetToken(fs, ','), m_Threshold->GetValue()));
    m_MapToWhite->SetValue(STRBOOL(GetToken(fs, ',')));
    m_SwapOrder->SetValue(STRBOOL(GetToken(fs, ',')));
    m_AddSharp->SetValue(STRBOOL(GetToken(fs, ',')));
    m_TwoWaySharp->SetValue(STRBOOL(GetToken(fs, ',')));

    //Update label & select panel
    wxCommandEvent evt;
    evt.SetEventObject(m_Yadif->GetValue() ? m_Yadif : m_KernDeint);
    OnAction(evt);

}

//---------------------------------------------------------------------------------------

bool Deinterlace::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftDEINTERLACE;

    //Yadif values
    int mode = m_DeintMode->GetSelection(), parity = -1, frames = m_FramesAll->GetValue() ? 0 : 1;
    if (m_ParityTop->GetValue()) parity = 0;
    else if (m_ParityBottom->GetValue()) parity = 1;

    if (m_Yadif->GetValue())
    {

        //Store yadif friendly & filter


        wxString *mf = FFQL()->GetStringArray(SID_DEINTER_MODE_FRIENDLY, DEINTER_MODE_COUNT),
                 *pf = FFQL()->GetStringArray(SID_DEINTER_PARITY_FRIENDLY, DEINTER_PARITY_COUNT),
                 *df = FFQL()->GetStringArray(SID_DEINTER_DEINTERLACE_FRIENDLY, DEINTER_DEINTERLACE_COUNT);

        fltr->friendly = FFQSF(SID_DEINTER_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               mf[mode], pf[parity + 1], df[frames]);

        delete[] mf;
        delete[] pf;
        delete[] df;

        fltr->ff_filter.Printf("%syadif=%i:%i:%i%s", FILTER_VIDEO_IN, mode, parity, frames, FILTER_VIDEO_OUT);

    }

    else
    {

        //Store kerndeint friendly & filter
        fltr->friendly = FFQSF(SID_DEINTER_KDEINT_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               m_Threshold->GetValue(),
                               BOOLSTR(m_MapToWhite->GetValue()),
                               BOOLSTR(m_SwapOrder->GetValue()),
                               BOOLSTR(m_AddSharp->GetValue()),
                               BOOLSTR(m_TwoWaySharp->GetValue()));

        wxString kf;
        if (m_Threshold->GetValue() != 10) kf << "thresh=" << m_Threshold->GetValue() << ":";
        if (m_MapToWhite->GetValue()) kf += "map=1:";
        if (m_SwapOrder->GetValue()) kf += "order=1:";
        if (m_AddSharp->GetValue()) kf += "sharp=1:";
        if (m_TwoWaySharp->GetValue()) kf += "twoway=1:";
        if (kf.Len() > 0) kf.RemoveLast();

        fltr->ff_filter.Printf("%skerndeint=%s%s", FILTER_VIDEO_IN, kf, FILTER_VIDEO_OUT);

    }


    //Store editable
    fltr->editable.Printf("%i,%i,%i,%s,%i,%s,%s,%s,%s",
                          //Yadif
                          mode, parity, frames,
                          //Filter type
                          BOOLSTR(m_KernDeint->GetValue()),
                          //Kerndeint
                          m_Threshold->GetValue(),
                          BOOLSTR(m_MapToWhite->GetValue()),
                          BOOLSTR(m_SwapOrder->GetValue()),
                          BOOLSTR(m_AddSharp->GetValue()),
                          BOOLSTR(m_TwoWaySharp->GetValue()));

    return true;

}

//---------------------------------------------------------------------------------------

void Deinterlace::OnAction(wxCommandEvent &event)
{

    //Update slider label
    m_ThreshLab->SetLabel(wxString::Format("%i", m_Threshold->GetValue()));

    //Select panel
    if (event.GetEventObject() == m_Yadif) SwapPanels(m_YadifPan);
    else if (event.GetEventObject() == m_KernDeint) SwapPanels(m_KernDeintPan);

}

//---------------------------------------------------------------------------------------

void Deinterlace::SwapPanels(wxPanel *select)
{

    //Show the correct panel / swap panels
    if (select == NULL) select = m_YadifPan->IsShown() ? m_KernDeintPan : m_YadifPan;
    m_YadifPan->Show(select == m_YadifPan);
    m_KernDeintPan->Show(select == m_KernDeintPan);
    Layout();

}
