/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQJobEditAdvCuts.cpp                                           *
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

#include "FFQJobEditAdvCuts.h"
#include "utils/FFQMisc.h"
#include "utils/FFQBuildCmd.h"
#include "utils/FFQStreamMapping.h"
#include "utils/FFQConfig.h"
#include "FFQMain.h"

#include <wx/filename.h>
#include <wx/ffile.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQJobEditAdvCuts)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQJobEditAdvCuts)
#include <wx/settings.h>
//*)

//(*IdInit(FFQJobEditAdvCuts)
const long FFQJobEditAdvCuts::ID_CUTLIST = wxNewId();
const long FFQJobEditAdvCuts::ID_FILTERFIRST = wxNewId();
const long FFQJobEditAdvCuts::ID_FILTERLAST = wxNewId();
const long FFQJobEditAdvCuts::ID_FROMTIME = wxNewId();
const long FFQJobEditAdvCuts::ID_TOTIME = wxNewId();
const long FFQJobEditAdvCuts::ID_ADDCUT = wxNewId();
const long FFQJobEditAdvCuts::ID_REMOVECUT = wxNewId();
const long FFQJobEditAdvCuts::ID_FRAMECONV = wxNewId();
const long FFQJobEditAdvCuts::ID_TIMEPREVIEWAT = wxNewId();
const long FFQJobEditAdvCuts::ID_TIMEPREVIEW = wxNewId();
const long FFQJobEditAdvCuts::ID_FRAMESLIDER = wxNewId();
const long FFQJobEditAdvCuts::ID_PREVFRAME = wxNewId();
const long FFQJobEditAdvCuts::ID_NEXTFRAME = wxNewId();
const long FFQJobEditAdvCuts::ID_SETFROM = wxNewId();
const long FFQJobEditAdvCuts::ID_SETTO = wxNewId();
const long FFQJobEditAdvCuts::ID_OKBUTTON = wxNewId();
const long FFQJobEditAdvCuts::ID_CANCELBUTTON = wxNewId();
const long FFQJobEditAdvCuts::ID_FRAMETIMER = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQJobEditAdvCuts,wxDialog)
	//(*EventTable(FFQJobEditAdvCuts)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

//Used for previews
#define MARGIN 7000
#define SUBMARG(from) ( from >= MARGIN ? from - MARGIN : from )

//---------------------------------------------------------------------------------------

FFQJobEditAdvCuts::FFQJobEditAdvCuts(wxWindow* parent)
{

    //Get size used by frame view
    wxSize fv_size(
        wxSystemSettings::GetMetric(wxSYS_SCREEN_X, parent) / 3,
        wxSystemSettings::GetMetric(wxSYS_SCREEN_Y, parent) / 3
    );

    //Get strings for labels
    wxString *lng_trim_mode = FFQL()->GetStringArray(SID_JOBEDIT_ADV_CUTS_TRIM_MODE, 3);
    wxString *lng_accuracy = FFQL()->GetStringArray(SID_JOBEDIT_ADV_CUTS_CFG_ACCURACY, 3);
    wxString *lng_delay = FFQL()->GetStringArray(SID_JOBEDIT_ADV_CUTS_CFG_DELAY, 3);
    wxString *lng_placement = FFQL()->GetStringArray(SID_JOBEDIT_ADV_CUTS_CFG_PLACEMENT, 3);

	//(*Initialize(FFQJobEditAdvCuts)
	wxStaticBoxSizer* SBS1;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticBoxSizer* SBS3;
	wxStaticBoxSizer* SBS5;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer9;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer15;
	wxFlexGridSizer* FlexGridSizer8;
	wxStaticText* ST1;
	wxPanel* P1;
	wxStaticBoxSizer* SBS4;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer1;
	wxPanel* P2;
	wxFlexGridSizer* FlexGridSizer11;
	wxBoxSizer* BoxSizer3;
	wxStaticBoxSizer* SBS2;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetTitle(FFQS(SID_JOBEDIT_ADV_CUTS_TITLE));
	FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer7 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer7->AddGrowableCol(0);
	FlexGridSizer7->AddGrowableCol(1);
	FlexGridSizer7->AddGrowableRow(0);
	FlexGridSizer8 = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer8->AddGrowableCol(0);
	FlexGridSizer8->AddGrowableRow(0);
	SBS1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("L"));
	FlexGridSizer4 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	FlexGridSizer4->AddGrowableRow(0);
	CutList = new wxListBox(this, ID_CUTLIST, wxDefaultPosition, wxSize(-1,150), 0, 0, wxLB_EXTENDED, wxDefaultValidator, _T("ID_CUTLIST"));
	FlexGridSizer4->Add(CutList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer15 = new wxFlexGridSizer(2, 2, 0, 0);
	ST4 = new wxStaticText(this, wxID_ANY, _T("Cm"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_LIST_TITLE));
	ST4->SetLabel(lng_trim_mode[0]);
	FlexGridSizer15->Add(ST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	P1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	KeepCuts = new wxRadioButton(P1, wxID_ANY, _T("Kp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	KeepCuts->SetLabel(lng_trim_mode[1]);
	BoxSizer1->Add(KeepCuts, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RemoveCuts = new wxRadioButton(P1, wxID_ANY, _T("Rm"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	RemoveCuts->SetLabel(lng_trim_mode[2]);
	BoxSizer1->Add(RemoveCuts, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	P1->SetSizer(BoxSizer1);
	BoxSizer1->Fit(P1);
	BoxSizer1->SetSizeHints(P1);
	FlexGridSizer15->Add(P1, 1, wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ST11 = new wxStaticText(this, wxID_ANY, _T("Fp"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST11->SetLabel(lng_placement[0]);
	FlexGridSizer15->Add(ST11, 1, wxTOP|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	P2 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	FilterFirst = new wxRadioButton(P2, ID_FILTERFIRST, _T("Ff"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FILTERFIRST"));
	FilterFirst->SetLabel(lng_placement[1]);
	BoxSizer3->Add(FilterFirst, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FilterLast = new wxRadioButton(P2, ID_FILTERLAST, _T("Fl"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FILTERLAST"));
	FilterLast->SetLabel(lng_placement[2]);
	BoxSizer3->Add(FilterLast, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	P2->SetSizer(BoxSizer3);
	BoxSizer3->Fit(P2);
	BoxSizer3->SetSizeHints(P2);
	FlexGridSizer15->Add(P2, 1, wxTOP|wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer15, 1, wxTOP|wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SBS1->Add(FlexGridSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer8->Add(SBS1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("T"));
	FlexGridSizer6 = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer6->AddGrowableCol(0);
	FlexGridSizer2 = new wxFlexGridSizer(1, 6, 0, 0);
	FlexGridSizer2->AddGrowableCol(1);
	FlexGridSizer2->AddGrowableCol(3);
	ST1 = new wxStaticText(this, wxID_ANY, _T("F"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	SBS2->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_TOOLS_TITLE));
	ST1->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_FROM));
	FlexGridSizer2->Add(ST1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FromTime = new wxTextCtrl(this, ID_FROMTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT, wxDefaultValidator, _T("ID_FROMTIME"));
	FlexGridSizer2->Add(FromTime, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ST2 = new wxStaticText(this, wxID_ANY, _T("T"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST2->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_TO));
	FlexGridSizer2->Add(ST2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ToTime = new wxTextCtrl(this, ID_TOTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT, wxDefaultValidator, _T("ID_TOTIME"));
	FlexGridSizer2->Add(ToTime, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	AddCut = new wxButton(this, ID_ADDCUT, _T("+"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_ADDCUT"));
	FlexGridSizer2->Add(AddCut, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	RemoveCut = new wxButton(this, ID_REMOVECUT, _T("-"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_REMOVECUT"));
	FlexGridSizer2->Add(RemoveCut, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer6->Add(FlexGridSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FrameConv = new wxCheckBox(this, ID_FRAMECONV, _T("FRC"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FRAMECONV"));
	FrameConv->SetValue(false);
	FrameConv->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_FRAME_CONV));
	FlexGridSizer6->Add(FrameConv, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SBS2->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer8->Add(SBS2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Inf"));
	FlexGridSizer11 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer11->AddGrowableCol(0);
	FlexGridSizer3 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	ST3 = new wxStaticText(this, wxID_ANY, _T("Adt"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	SBS3->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_INPUT_INFO));
	ST3->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_TIME_DETAILS));
	FlexGridSizer3->Add(ST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	TimePreviewAt = new wxTextCtrl(this, ID_TIMEPREVIEWAT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT, wxDefaultValidator, _T("ID_TIMEPREVIEWAT"));
	FlexGridSizer3->Add(TimePreviewAt, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	TimePreview = new wxButton(this, ID_TIMEPREVIEW, _T("Td"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TIMEPREVIEW"));
	TimePreview->SetLabel(FFQS(SID_COMMON_PREVIEW));
	FlexGridSizer3->Add(TimePreview, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer11->Add(FlexGridSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	DurationLab = new wxStaticText(this, wxID_ANY, _T("Dur"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	DurationLab->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
	FlexGridSizer11->Add(DurationLab, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SBS3->Add(FlexGridSizer11, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer8->Add(SBS3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer7->Add(FlexGridSizer8, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer9 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer9->AddGrowableCol(0);
	FlexGridSizer9->AddGrowableRow(0);
	SBS4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Fpv"));
	PreviewSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
	PreviewSizer1->AddGrowableCol(0);
	PreviewSizer1->AddGrowableRow(0);
	FrameView = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("wxID_ANY"));
	SBS4->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_PREVIEW));
	FrameView->SetMinSize(fv_size);
	PreviewSizer1->Add(FrameView, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FrameSlider = new wxSlider(this, ID_FRAMESLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_TOP, wxDefaultValidator, _T("ID_FRAMESLIDER"));
	FrameSlider->SetPageSize(25);
	PreviewSizer1->Add(FrameSlider, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer10 = new wxFlexGridSizer(0, 5, 0, 0);
	FlexGridSizer10->AddGrowableCol(4);
	FlexGridSizer10->AddGrowableRow(0);
	PrevFrame = new wxButton(this, ID_PREVFRAME, _T("<"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_PREVFRAME"));
	FlexGridSizer10->Add(PrevFrame, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	NextFrame = new wxButton(this, ID_NEXTFRAME, _T(">"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_NEXTFRAME"));
	FlexGridSizer10->Add(NextFrame, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SetFrom = new wxButton(this, ID_SETFROM, _T("Sf"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SETFROM"));
	SetFrom->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_SET_FROM));
	FlexGridSizer10->Add(SetFrom, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SetTo = new wxButton(this, ID_SETTO, _T("Sn"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SETTO"));
	SetTo->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_SET_TO));
	FlexGridSizer10->Add(SetTo, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FramePos = new wxStaticText(this, wxID_ANY, _T("Pos"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer10->Add(FramePos, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	PreviewSizer1->Add(FlexGridSizer10, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS4->Add(PreviewSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer9->Add(SBS4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS5 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Cfg"));
	PreviewSizer2 = new wxFlexGridSizer(2, 4, 0, 0);
	PreviewSizer2->AddGrowableCol(2);
	ST5 = new wxStaticText(this, wxID_ANY, _T("Ac"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	SBS5->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_PREVIEW_CFG));
	ST5->SetLabel(lng_accuracy[0]);
	PreviewSizer2->Add(ST5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ST6 = new wxStaticText(this, wxID_ANY, _T("L"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST6->SetLabel(lng_accuracy[1]);
	PreviewSizer2->Add(ST6, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	CfgAccuracy = new wxSlider(this, wxID_ANY, 2, 0, 20, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	PreviewSizer2->Add(CfgAccuracy, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ST7 = new wxStaticText(this, wxID_ANY, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST7->SetLabel(lng_accuracy[2]);
	PreviewSizer2->Add(ST7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ST8 = new wxStaticText(this, wxID_ANY, _T("Dl"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST8->SetLabel(lng_delay[0]);
	PreviewSizer2->Add(ST8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ST9 = new wxStaticText(this, wxID_ANY, _T("L"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST9->SetLabel(lng_delay[1]);
	PreviewSizer2->Add(ST9, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	CfgDelay = new wxSlider(this, wxID_ANY, 3, 0, 20, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	PreviewSizer2->Add(CfgDelay, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ST10 = new wxStaticText(this, wxID_ANY, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST10->SetLabel(lng_delay[2]);
	PreviewSizer2->Add(ST10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SBS5->Add(PreviewSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer9->Add(SBS5, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	OkButton = new wxButton(this, ID_OKBUTTON, _T("Y"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBUTTON"));
	OkButton->SetDefault();
	OkButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer2->Add(OkButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("N"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer2->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer9->Add(BoxSizer2, 1, wxTOP|wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	FlexGridSizer7->Add(FlexGridSizer9, 1, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer1->Add(FlexGridSizer7, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(FlexGridSizer1);
	FrameTimer.SetOwner(this, ID_FRAMETIMER);
	FrameTimer.Start(250, true);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_CUTLIST,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_CUTLIST,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::OnCutListDClick);
	Connect(ID_FROMTIME,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_TOTIME,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_ADDCUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_REMOVECUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_TIMEPREVIEWAT,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_TIMEPREVIEW,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	FrameView->Connect(wxEVT_PAINT,(wxObjectEventFunction)&FFQJobEditAdvCuts::OnFrameViewPaint,0,this);
	Connect(ID_PREVFRAME,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_NEXTFRAME,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_SETFROM,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_SETTO,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_OKBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_FRAMETIMER,wxEVT_TIMER,(wxObjectEventFunction)&FFQJobEditAdvCuts::OnFrameTimerTrigger);
	//*)

    //Release strings for labels
    delete[] lng_trim_mode;
    delete[] lng_accuracy;
    delete[] lng_delay;
    delete[] lng_placement;

	if (FrameTimer.IsRunning()) FrameTimer.Stop();
	m_DrawBuf = NULL;
	FrameView->SetBackgroundStyle(wxBG_STYLE_PAINT); //Prevent flicker
	FrameSlider->Bind(wxEVT_SLIDER, &FFQJobEditAdvCuts::ActionClick, this);

}

//---------------------------------------------------------------------------------------

FFQJobEditAdvCuts::~FFQJobEditAdvCuts()
{

	//(*Destroy(FFQJobEditAdvCuts)
	//*)
	if (m_DrawBuf) delete m_DrawBuf;

}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdvCuts::Execute(FFQ_CUTS &cuts, wxString file_path, FFProbeInfoParser *probe)
{

    if (m_DrawBuf == NULL)
    {

        //Prepare draw buffer
        wxRect cr = FrameView->GetClientRect();
        m_DrawBuf = new wxBitmap(cr.GetWidth(), cr.GetHeight());

    }

    //Clear draw buffer
    wxMemoryDC mdc(*m_DrawBuf);
    mdc.SetPen(wxNullPen);
    wxBrush brs(FrameView->GetBackgroundColour());
    mdc.SetBrush(brs);
    mdc.DrawRectangle(0, 0, m_DrawBuf->GetWidth(), m_DrawBuf->GetHeight());

    //Initialize the time span list from cuts
    CutList->Clear();
    wxString s = cuts.cuts;
    while (s.Len() > 0)
    {

        wxString f = GetToken(s, ";", true);
        CutList->Append(f + CUT_SEP + GetToken(s, ";", true));

    }

    //Cut settings
    KeepCuts->SetValue(cuts.keep);
    RemoveCuts->SetValue(!cuts.keep);
    FilterFirst->SetValue(cuts.filter_first);
    FilterLast->SetValue(!cuts.filter_first);


    //Clear, update and center
    probe->GetDuration(m_Duration);
    m_FramePos.SetMilliseconds(0);
    m_FilePath = file_path;
    m_Probe = probe;
    m_VidInfo = probe->FindStreamByCodecType(CODEC_TYPE_VIDEO);
    #ifdef DEBUG
    //m_VidInfo = NULL;
    #endif // DEBUG
    probe->GetVideoRate(m_VidRate);
    FromTime->Clear();
    ToTime->Clear();
    TimePreviewAt->Clear();
    wxString vrate = m_VidRate <= 0 ? "??" : wxString::Format("%g", m_VidRate);
    DurationLab->SetLabel(FFQSF(SID_JOBEDIT_ADV_CUTS_DURATION, m_Duration.ToString(true, true), m_Duration.ToShortString(), vrate));
    FrameConv->Enable(m_VidRate > 0);
    FrameConv->SetValue(false);
	m_SrtPath = FFQConfig::GetInstance()->GetTmpPath(m_FilePath.BeforeLast(wxFileName::GetPathSeparator()), false, "srt");

	//Frame preview controls available?
	bool has_vid = (m_VidInfo != NULL) && (m_VidRate > 0) && (m_Duration.ToMilliseconds() > 0);
	EnableSizer(PreviewSizer1, has_vid);
	EnableSizer(PreviewSizer2, has_vid);

    //if (FrameView->IsEnabled())
    if (has_vid)
    {
        //Update slider max
        FrameSlider->SetValue(0);
        m_FrameTime = froundi(1000.0 / m_VidRate);
        int m = m_Duration.ToMilliseconds() / m_FrameTime;
        if (m_Duration.ToMilliseconds() % m_FrameTime > 0) m++;
        FrameSlider->SetMax( m );

        //Create resources used
        m_CurFrame = new wxImage();
        m_Process = new FFQProcess();

        //Load first frame
        FrameTimer.StartOnce();

    }
    else
    {

        //Nullify
        m_CurFrame = NULL;
        m_Process = NULL;
        m_FrameTime = 0;

    }

    //Update and center
    UpdateControls();
    CenterOnParent();

    //Show modal
    bool res = (ShowModal() == wxID_OK);

    //Stop timer and release current frame
	if (FrameTimer.IsRunning()) FrameTimer.Stop();
    if (m_CurFrame) delete m_CurFrame;
    if (m_Process) delete m_Process;

    if (res)
    {

        //Store cut settings (we cannot use "," as separator since it is used in FFQ_INPUT_FILE where cuts are stored)
        cuts.keep = KeepCuts->GetValue();
        cuts.filter_first = FilterFirst->GetValue();
        cuts.frame_time = m_FrameTime;
        cuts.cuts.Clear();

        if (CutList->GetCount() > 0)
        {

            //Store cuts
            for (size_t i = 0; i < CutList->GetCount(); i++)
            {

                wxString cut = CutList->GetString(i);
                cuts.cuts += GetToken(cut, CUT_SEP, true) + ";" + cut + ";";

            }

            //Remove last ";"
            cuts.cuts.RemoveLast();

        }

    }

    //Delete temp file
    if (wxFileExists(m_SrtPath)) wxRemoveFile(m_SrtPath);

    //Return success
    return res;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::ExtractFrame()
{

    //Calculate frame pos
    m_FramePos.SetMilliseconds(FrameSlider->GetValue() * m_FrameTime);
    if (m_FramePos.ToMilliseconds() > m_Duration.ToMilliseconds()) m_FramePos = m_Duration;

    //Extract the frame
    bool ok = false;
    try
    {

        ok = m_Process->ExtractFrameFromFile(m_FilePath, m_FramePos, m_CurFrame, 5000, CfgAccuracy->GetValue() * 500, wxSize(m_DrawBuf->GetWidth(), m_DrawBuf->GetHeight()));

    }
    catch (std::exception &err)
    {

        #ifdef DEBUG
        ShowError(wxString(err.what()));
        #endif // DEBUG

    }

    //Clear draw buffer
    wxMemoryDC mdc(*m_DrawBuf);
    mdc.SetPen(*wxTRANSPARENT_PEN);
    wxBrush brs(FrameView->GetBackgroundColour());
    mdc.SetBrush(brs);
    mdc.DrawRectangle(0, 0, m_DrawBuf->GetWidth(), m_DrawBuf->GetHeight());

    if (ok && m_CurFrame->IsOk())
    {

        //Draw to buffer
        wxBitmap tmp(*m_CurFrame);
        mdc.DrawBitmap(tmp, (m_DrawBuf->GetWidth() - tmp.GetWidth()) / 2, (m_DrawBuf->GetHeight() - tmp.GetHeight()) / 2);

    }

    //Repaint view
    FrameView->Refresh();

    //Update controls
    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool GetTimeValue(wxString str, bool frame, double vrate, TIME_VALUE &res)
{
    //Convert str to time value
    if (str.Len() == 0) res = TIME_VALUE(0);
    else if (frame)
    {
        double f = Str2Float(str, -1);
        if (f < 0) return false;
        res = TIME_VALUE(wxString::Format("%g", f / vrate));
    }
    else
    {
        str.Replace(",", ".");
        res = TIME_VALUE(str);
    }
    return res.IsValid();
}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdvCuts::GetCut(wxString &cut, long &pos)
{

    //Validate from..
    wxString tsf = StrTrim(FromTime->GetValue());
    TIME_VALUE tvf;
    if (!GetTimeValue(tsf, FrameConv->GetValue(), m_VidRate, tvf))
        return ShowError(FromTime, FrameConv->GetValue() ? FFQS(SID_JOBEDIT_ADV_CUTS_BAD_FRAME) : FFQS(SID_JOBEDIT_ADV_CUTS_BAD_TIME));

    //Validate to..
    wxString tst = StrTrim(ToTime->GetValue());
    TIME_VALUE tvt;
    if (!GetTimeValue(tst, FrameConv->GetValue(), m_VidRate, tvt))
        return ShowError(ToTime, FrameConv->GetValue() ? FFQS(SID_JOBEDIT_ADV_CUTS_BAD_FRAME) : FFQS(SID_JOBEDIT_ADV_CUTS_BAD_TIME));

    //If to is 0, set to duration
    if (tvt.ToMilliseconds() == 0) tvt.SetMilliseconds(m_Duration.ToMilliseconds());

    //Validate to > from
    if (tvf.ToMilliseconds() >= tvt.ToMilliseconds())
        return ShowError(FrameConv->GetValue() ? FFQS(SID_JOBEDIT_ADV_CUTS_BAD_FRAME) : FFQS(SID_JOBEDIT_ADV_CUTS_BAD_TIME));

    //Get values as millis
    uint64_t msf = tvf.ToMilliseconds(), mst = tvt.ToMilliseconds(), ft, tt;

    //Position in list
    long find_pos = -1;

    //Check for overlapping
    for (long i = 0; i < (long)CutList->GetCount(); i++)
    {

        //Skip the cuts current position
        if (pos == i) continue;

        //Get values
        wxString s = CutList->GetString(i);
        ft = TIME_VALUE(StrTrim(GetToken(s, CUT_SEP, true))).ToMilliseconds();
        tt = TIME_VALUE(s).ToMilliseconds();

        //From in cut, to in cut or complete overlap
        if ( ((msf >= ft) && (msf < tt)) ||
             ((mst >= ft) && (mst < tt)) ||
             ((msf < ft) && (mst > tt)) ) return ShowError(FFQS(SID_JOBEDIT_ADV_CUTS_OVERLAP_TIME));

        //Insert position found?
        if ((find_pos < 0) && (mst < ft)) find_pos = i;

    }

    //All green, set values and return success
    cut = tvf.ToString(true, true) + CUT_SEP + tvt.ToString(true, true);
    pos = find_pos;
    return true;

}

//---------------------------------------------------------------------------------------

void* FFQJobEditAdvCuts::MakePreviewJob(TIME_VALUE start_time, FFQ_CUTS cuts, bool encode)
{

    //Creates a job that can be used to build a command
    //line for previewing

    //Make the input file
    FFQ_INPUT_FILE inf;
    inf.cuts = cuts;
    inf.path = m_FilePath;
    inf.start = start_time;

    //Make the stream mapping
    wxString smap;
    STREAM_MAPPING sm;
    sm.checked = true;
    sm.file_id = 1;

    //Video (255 will be used if video is unavailable, used for searching)
    sm.codec_type = CODEC_TYPE_VIDEO;
    sm.stream_id = m_VidInfo == NULL ? 255 : m_Probe->IndexOf(m_VidInfo);
    smap += sm.ToString();

    //Audio (if available)
    sm.codec_type = CODEC_TYPE_AUDIO;
    LPFFPROBE_STREAM_INFO si = m_Probe->FindStreamByCodecType(sm.codec_type);
    if (si)
    {
        sm.stream_id = m_Probe->IndexOf(si);
        smap += STREAM_MAPPING_SEPERATOR + sm.ToString();
    }

    //Create a preset
    LPFFQ_PRESET pst = new FFQ_PRESET();
    pst->video_codec = encode ? "libx264" : "copy";
    pst->video_rate = encode ? "2024,1" : "";
    pst->audio_codec = encode ? "libvo_aacenc" : "copy";
    pst->audio_rate = encode ? "128,1" : "";

    //Apply scaling for videos not fitting in 640x480
    /*if (encode && ((m_VidInfo->width > 640) || (m_VidInfo->height > 480)))
    {
        int w = (m_VidInfo->width > m_VidInfo->height) ? 640 : -1,
            h = (m_VidInfo->width > m_VidInfo->height) ? -1 : 480;
        FFMPEG_FILTER fltr;
        fltr.ff_filter.Printf("%sscale=%d:%d%s", FILTER_VIDEO_IN, w, h, FILTER_VIDEO_OUT);
        pst->filters.Add(fltr.ToString());
    }*/

    //Make the job
    LPFFQ_JOB job = new FFQ_JOB();
    job->inputs.Add(inf.ToString());
    job->stream_map = smap;
    job->cmd_line = CMD_DEFAULT;
    job->preset_ptr = pst;

    //Return it
    return job;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::ShowTimePreview(TIME_VALUE &at_time)
{

    //This method shows time with milliseconds using
    //a subtitle track. The subtitles are shown "margin"
    //before and after "at_time". Playback is started
    //2 seconds before the subtitles kick in

    //Make sure that grandparent is valid
    wxWindow *main_wnd = GetGrandParent();
    if ((main_wnd == NULL) || (dynamic_cast<FFQMain*>(main_wnd) == NULL)) return;

    //Get a player and verify its validity
    PLAYER_TYPE ptype;
    wxString cmd = FFQConfig::GetInstance()->GetFFPlayCommand(true, &ptype);
    if (cmd.Len() == 0) return;

    //Is it ffplay and can the special ffplay sauce be used? (Requires video..)
    bool ffplay = (ptype == ptFFPLAY) && (m_VidInfo != NULL);

    //Make time values
    TIME_VALUE start(SUBMARG(at_time.ToMilliseconds())),
               stop(start.ToMilliseconds() + (MARGIN * 2)),
               cur = ffplay ? start : 0, //Begin at 0 for full recode
               print = start; //Time to print

    //Calculate subtitle interval (5ms as minimum, 10ms as default)
    double siv = (m_VidRate <= 0) ? 10.0 : (1000.0 / m_VidRate);
    size_t srt_iv = (siv < 5) ? 5 : (size_t)froundi(siv), sttl_idx = 0;
    wxString sttl, ft, tt;

    //Remove any existing subtitle file, and create a new one
    if (wxFileExists(m_SrtPath)) wxRemoveFile(m_SrtPath);
    wxFFile ffile(m_SrtPath, "wb");

    //Make the subtitles
    while (sttl_idx < (MARGIN / srt_iv) * 2)
    {

        //Create from time and to time (the time subtitle is displayed)
        ft = cur.ToString(true, true, ',');
        cur = TIME_VALUE(cur.ToMilliseconds() + srt_iv);
        tt = cur.ToString(true, true, ',');

        //Make subtitle string and write it to file as ascii
        sttl.Printf("%i\r\n%s --> %s\r\n%s\r\n\r\n", ++sttl_idx, ft, tt, print.ToString());
        wxScopedCharBuffer scb = sttl.ToAscii();
        ffile.Write(scb.data(), scb.length());

        //Increment the time to print
        print = TIME_VALUE(print.ToMilliseconds() + srt_iv);

    }

    //Close file
    ffile.Close();

    //Create a preview job and get the preset
    LPFFQ_JOB job = (LPFFQ_JOB)MakePreviewJob(start, FFQ_CUTS(), !ffplay);
    LPFFQ_PRESET pst = ((LPFFQ_PRESET)job->preset_ptr);

    if (ffplay)
    {

        //Add values only used with ffplay
        job->out_len = TIME_VALUE(stop.ToMilliseconds() - start.ToMilliseconds()); //May crash VLC
        pst->custom_args_1 = "-output_ts_offset " + start.ToShortString(); //Offset ts

    }
    else
    {

        //Add the filter to burn in the subs with ffmpeg
        FFMPEG_FILTER fltr;
        fltr.ff_filter = FILTER_VIDEO_IN + "subtitles=" + EscapeFilterString(m_SrtPath, false) + FILTER_VIDEO_OUT;
        pst->filters.Add(fltr.ToString());

    }

    //Build the command
    long ep = 0;
    cmd = BuildCommandLine(job, ep, true, false);

    //Release mem
    delete pst;
    delete job;

    //Apply subtitle filter for ffplay
    if (ffplay) cmd.insert(cmd.Len() - 1, "-vf \"subtitles=" + EscapeFilterString(m_SrtPath, false) + "\" ");

    //Test video presence
    else if (m_VidInfo == NULL)
    {

        //No video, remove bogus video mapping
        cmd.Replace("-map 0:255 ", "");

        //Find insert position
        int idx = cmd.Find("-filter_complex \"");
        if (idx >= 0)
        {

            //Insert video source
            cmd.insert(idx + 17, wxString("color=0x000000:640x480[VID];"));

            //Replace video tag for the subtitle filter
            cmd.Replace(";[0:255]sub", ";[VID]sub");

        }

    }

    #ifdef DEBUG
    if(!DoConfirm(cmd)) return;
    bool ShowCmd = true;
    //cmd = "cmd /k \"" + cmd + "\"";
    #else
    bool ShowCmd = false;
    #endif // DEBUG

    //Execute command
    ((FFQMain*)main_wnd)->PreviewCommand(cmd, ShowCmd);

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::UpdateControls()
{

    bool sel = false;
    for(size_t i = 0; i < CutList->GetCount(); i++)
        if (CutList->IsSelected(i)) { sel=true; break; }

    //AddCut->Enable(true);
    RemoveCut->Enable(sel);

    //Preview controls
    if ( FrameView->IsEnabled() )
    {

        uint64_t fp = m_FramePos.ToMilliseconds();
        PrevFrame->Enable( fp > 0 );
        NextFrame->Enable( fp < m_Duration.ToMilliseconds() );
        FramePos->SetLabel(m_FramePos.ToString());

    }

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::ActionClick(wxCommandEvent& event)
{

    int evtId = event.GetId();

    if ((evtId == ID_ADDCUT) || (evtId == ID_FROMTIME) || (evtId == ID_TOTIME))
    {

        //Add cut
        wxString cut;
        long pos = -1;

        if (GetCut(cut, pos))
        {

            if (pos >= 0) pos = CutList->Insert(cut, pos);
            else pos = CutList->Append(cut);
            CutList->EnsureVisible(pos);
            FromTime->Clear();
            ToTime->Clear();

        }

    }

    else if (evtId == ID_REMOVECUT)
    {

        //Remove cuts
        CutList->Freeze();
        size_t idx = 0;
        while (idx < CutList->GetCount())
        {
            if (CutList->IsSelected(idx)) CutList->Delete(idx);
            else idx++;
        }
        CutList->Thaw();

    }

    else if (evtId == ID_CUTLIST)
    {

        //Load cut from list
        wxString s = CutList->GetString(event.GetInt());
        if (FrameConv->GetValue())
        {

            TIME_VALUE f(GetToken(s, CUT_SEP, true));
            TIME_VALUE t(s);
            FromTime->SetValue(f.ToMilliseconds() == 0 ? "0" : wxString::Format("%g", f.ToDouble() / 1000.0 * m_VidRate));
            ToTime->SetValue(t.ToMilliseconds() == 0 ? "0" : wxString::Format("%g", t.ToDouble() / 1000.0 * m_VidRate));

        }
        else
        {

            FromTime->SetValue(GetToken(s, CUT_SEP, true));
            ToTime->SetValue(s);

        }

    }

    else if ((evtId == ID_TIMEPREVIEW) || (evtId == ID_TIMEPREVIEWAT))
    {

        //Show a preview of the specified time
        TIME_VALUE tv(StrTrim(TimePreviewAt->GetValue()));
        if (tv.IsValid()) ShowTimePreview(tv);
        else ShowError(TimePreviewAt, FFQS(SID_JOBEDIT_ADV_CUTS_BAD_TIME_2));

    }

    else if (evtId == ID_FRAMESLIDER)
    {

        //Start the frame timer for delayed ExtractFrame
        FrameTimer.StartOnce(CfgDelay->GetValue() * 100);

        //Do not update controls
        return;

    }

    else if ((evtId == ID_PREVFRAME) || (evtId == ID_NEXTFRAME))
    {

        //Navigate one frame update with no delay
        if (m_Process->IsProcessRunning()) return; //Relax m8! ;-)
        FrameSlider->SetValue(FrameSlider->GetValue() + (evtId == ID_PREVFRAME ? -1 : 1));
        ExtractFrame();

        //UpdateControls called in ExtractFrame
        return;

    }

    //Set current frame pos to from
    else if (evtId == ID_SETFROM) FromTime->SetValue(m_FramePos.ToString());

    //Set current frame pos to to
    else if (evtId == ID_SETTO) ToTime->SetValue(m_FramePos.ToString());

    //End dialog
    else if (evtId == ID_OKBUTTON) EndModal(wxID_OK);
    else if (evtId == ID_CANCELBUTTON) EndModal(wxID_CANCEL);

    //Update controls
    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::OnCutListDClick(wxCommandEvent& event)
{

    //This function creates a preview of the cut that has just
    //been double clicked. This works awfully bad and has been
    //removed from the release build!

    #ifndef DEBUG
    return;
    #endif // DEBUG

    //Video info is required
    if (m_VidInfo == NULL) return;

    //Main frame is needed
    wxWindow *main = GetGrandParent();
    if ((main == NULL) || (dynamic_cast<FFQMain*>(main) == NULL)) return;

    //Get time values of the cut
    wxString cut = CutList->GetString(event.GetInt());
    TIME_VALUE ft(GetToken(cut, CUT_SEP, true));
    TIME_VALUE tt(cut), et = TIME_VALUE(m_Duration.ToMilliseconds() + 1000), ss(0);

    //If cut is from the beginning and onward we must show "margin" after the cut
    if (ft.ToMilliseconds() == 0)
    {

        //The cut itself (ft == 0)
        cut = "0;" + tt.ToShortString() + ";";

        //From "margin" after the cut to the end
        cut += TIME_VALUE(tt.ToMilliseconds() + MARGIN).ToShortString() + ";" + et.ToShortString() + ";";

    }

    //Is the cut past or at the end we must show "margin" before the cut
    else if (tt.ToMilliseconds() >= m_Duration.ToMilliseconds())
    {

        //From the beginning to "margin" before the cut
        cut = "0;" + TIME_VALUE(SUBMARG(ft.ToMilliseconds())).ToShortString() + ";";

        //From the cut itself to the end
        cut += ft.ToShortString() + ";" + et.ToShortString() + ";";

    }

    //If cut is somewhere in the middle we must show "margin" before and after the cut
    else
    {

        //From the beginning to "margin" before the cut
        cut = "0;" + TIME_VALUE(SUBMARG(ft.ToMilliseconds())).ToShortString() + ";";
        //cut = "";
        //ss = TIME_VALUE(SUBMARG(ft.ToMilliseconds()));

        //The cut itself
        cut += ft.ToShortString() + ";" + tt.ToShortString() + ";";

        //From "margin" after the cut and to the end
        cut += TIME_VALUE(tt.ToMilliseconds() + MARGIN).ToShortString() + ";" + et.ToShortString() + ";";


    }

    //Make the job
    LPFFQ_JOB job = (LPFFQ_JOB)MakePreviewJob(ss, cut, true);

    //Build a preview command
    long ep = 0;
    wxString cmd = BuildCommandLine(job, ep, true);

    //Release mem
    delete (LPFFQ_PRESET)job->preset_ptr;
    delete job;

    #ifdef DEBUG
    if (!DoConfirm(cut + "\n\n" + cmd)) return;
    bool ShowCmd = true;
    #else
    bool ShowCmd = false;
    #endif // DEBUG

    //Execute preview command
    ((FFQMain*)main)->PreviewCommand(cmd, ShowCmd);

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::OnFrameViewPaint(wxPaintEvent& event)
{

    wxPaintDC dc(FrameView);
    dc.DrawBitmap(*m_DrawBuf, 0, 0);

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::OnFrameTimerTrigger(wxTimerEvent& event)
{

    //Extract frame or wait another 100 ms
    if (!m_Process->IsProcessRunning()) ExtractFrame();
    else FrameTimer.StartOnce(50);

}
