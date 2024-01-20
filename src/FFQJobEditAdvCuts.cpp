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
#include "utils/FFQParsing.h"
#include "FFQMain.h"

#include <wx/filename.h>
#include <wx/ffile.h>
#include <wx/menu.h>

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
const long FFQJobEditAdvCuts::ID_QUICKCUT = wxNewId();
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
const long FFQJobEditAdvCuts::ID_JUMPSIZE = wxNewId();
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

//Used to define menu items in the cuts menu
const uint8_t MENU_ADD_SUBMENU    = 1;
const uint8_t MENU_ADD_FIRST      = 2;
const uint8_t MENU_ADD_LAST       = 3;
const uint8_t MENU_ADD_SORTED     = 4;
const uint8_t MENU_SORT_NOW       = 5;
const uint8_t MENU_MERGE_OVERLAPS = 6;
const uint8_t MENU_JUMP_SUBMENU   = 7;
const uint8_t MENU_JUMP_TO_FROM   = 8;
const uint8_t MENU_JUMP_TO_TO     = 9;
const uint8_t MENU_MOVE_UP        = 10;
const uint8_t MENU_MOVE_DOWN      = 11;
const uint8_t MENU_DUPLICATE      = 12;

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

    wxClientDC dc(this);
    dc.SetFont(GetFont());
    wxSize time_size = dc.GetTextExtent("0");
    time_size.SetHeight(-1);
    time_size.SetWidth(time_size.GetWidth() * 15);

	//(*Initialize(FFQJobEditAdvCuts)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer11;
	wxFlexGridSizer* FlexGridSizer15;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* FlexGridSizer9;
	wxPanel* P1;
	wxPanel* P2;
	wxStaticBoxSizer* SBS1;
	wxStaticBoxSizer* SBS2;
	wxStaticBoxSizer* SBS3;
	wxStaticBoxSizer* SBS4;
	wxStaticBoxSizer* SBS5;
	wxStaticText* ST1;

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
	FlexGridSizer4->Add(CutList, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer5 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer5->AddGrowableCol(0);
	QuickCut = new wxCheckBox(this, ID_QUICKCUT, _T("Qc"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_QUICKCUT"));
	QuickCut->SetValue(false);
	QuickCut->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_QUICK_CUT));
	FlexGridSizer5->Add(QuickCut, 1, wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 0);
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
	FlexGridSizer15->Add(P1, 1, wxLEFT|wxEXPAND, 5);
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
	FlexGridSizer15->Add(P2, 1, wxTOP|wxLEFT|wxEXPAND, 5);
	FlexGridSizer4->Add(FlexGridSizer15, 1, wxTOP|wxBOTTOM|wxEXPAND, 5);
	SBS1->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer8->Add(SBS1, 1, wxALL|wxEXPAND, 3);
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
	FromTime->SetMinClientSize(time_size);
	FlexGridSizer2->Add(FromTime, 1, wxALL|wxEXPAND, 3);
	ST2 = new wxStaticText(this, wxID_ANY, _T("T"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST2->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_TO));
	FlexGridSizer2->Add(ST2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ToTime = new wxTextCtrl(this, ID_TOTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT, wxDefaultValidator, _T("ID_TOTIME"));
	ToTime->SetMinClientSize(time_size);
	FlexGridSizer2->Add(ToTime, 1, wxALL|wxEXPAND, 3);
	AddCut = new wxButton(this, ID_ADDCUT, _T("  +  "), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_ADDCUT"));
	FlexGridSizer2->Add(AddCut, 1, wxALL|wxEXPAND, 3);
	RemoveCut = new wxButton(this, ID_REMOVECUT, _T("  -  "), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_REMOVECUT"));
	FlexGridSizer2->Add(RemoveCut, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer6->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 0);
	FrameConv = new wxCheckBox(this, ID_FRAMECONV, _T("FRC"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FRAMECONV"));
	FrameConv->SetValue(false);
	FrameConv->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_FRAME_CONV));
	FlexGridSizer6->Add(FrameConv, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SBS2->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer8->Add(SBS2, 1, wxALL|wxEXPAND, 3);
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
	TimePreviewAt->SetMinClientSize(time_size);
	FlexGridSizer3->Add(TimePreviewAt, 1, wxALL|wxEXPAND, 3);
	TimePreview = new wxButton(this, ID_TIMEPREVIEW, _T("Td"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TIMEPREVIEW"));
	TimePreview->SetLabel(FFQS(SID_COMMON_PREVIEW));
	FlexGridSizer3->Add(TimePreview, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer11->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 0);
	DurationLab = new wxStaticText(this, wxID_ANY, _T("Dur"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	DurationLab->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
	FlexGridSizer11->Add(DurationLab, 1, wxALL|wxEXPAND, 3);
	SBS3->Add(FlexGridSizer11, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer8->Add(SBS3, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer7->Add(FlexGridSizer8, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer9 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer9->AddGrowableCol(0);
	FlexGridSizer9->AddGrowableRow(0);
	SBS4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Fpv"));
	PreviewSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
	PreviewSizer1->AddGrowableCol(0);
	PreviewSizer1->AddGrowableRow(0);
	FrameView = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE, _T("wxID_ANY"));
	SBS4->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_PREVIEW));
	FrameView->SetMinSize(fv_size);
	PreviewSizer1->Add(FrameView, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FrameSlider = new wxSlider(this, ID_FRAMESLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_TOP, wxDefaultValidator, _T("ID_FRAMESLIDER"));
	FrameSlider->SetPageSize(25);
	PreviewSizer1->Add(FrameSlider, 1, wxBOTTOM|wxEXPAND, 3);
	FlexGridSizer10 = new wxFlexGridSizer(0, 6, 0, 0);
	FlexGridSizer10->AddGrowableCol(5);
	FlexGridSizer10->AddGrowableRow(0);
	PrevFrame = new wxButton(this, ID_PREVFRAME, _T("  <  "), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_PREVFRAME"));
	FlexGridSizer10->Add(PrevFrame, 1, wxALL|wxEXPAND, 3);
	JumpSize = new wxSpinCtrl(this, ID_JUMPSIZE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 1, 1000, 0, _T("ID_JUMPSIZE"));
	JumpSize->SetValue(_T("0"));
	JumpSize->SetMaxSize(wxSize(-1,-1));
	FlexGridSizer10->Add(JumpSize, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	NextFrame = new wxButton(this, ID_NEXTFRAME, _T("  >  "), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_NEXTFRAME"));
	FlexGridSizer10->Add(NextFrame, 1, wxALL|wxEXPAND, 3);
	SetFrom = new wxButton(this, ID_SETFROM, _T("Sf"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SETFROM"));
	SetFrom->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_SET_FROM));
	FlexGridSizer10->Add(SetFrom, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SetTo = new wxButton(this, ID_SETTO, _T("Sn"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SETTO"));
	SetTo->SetLabel(FFQS(SID_JOBEDIT_ADV_CUTS_SET_TO));
	FlexGridSizer10->Add(SetTo, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FramePos = new wxStaticText(this, wxID_ANY, _T("Pos"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer10->Add(FramePos, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
	PreviewSizer1->Add(FlexGridSizer10, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 3);
	SBS4->Add(PreviewSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer9->Add(SBS4, 1, wxALL|wxEXPAND, 3);
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
	PreviewSizer2->Add(CfgAccuracy, 1, wxALL|wxEXPAND, 0);
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
	PreviewSizer2->Add(CfgDelay, 1, wxALL|wxEXPAND, 0);
	ST10 = new wxStaticText(this, wxID_ANY, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST10->SetLabel(lng_delay[2]);
	PreviewSizer2->Add(ST10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SBS5->Add(PreviewSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer9->Add(SBS5, 1, wxALL|wxEXPAND, 3);
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
	FlexGridSizer9->Add(BoxSizer2, 1, wxTOP|wxBOTTOM|wxEXPAND, 10);
	FlexGridSizer7->Add(FlexGridSizer9, 1, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND, 2);
	FlexGridSizer1->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 0);
	SetSizer(FlexGridSizer1);
	FrameTimer.SetOwner(this, ID_FRAMETIMER);
	FrameTimer.Start(250, true);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_CUTLIST,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
	Connect(ID_CUTLIST,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::OnCutListDClick);
	Connect(ID_QUICKCUT,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvCuts::ActionClick);
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

    m_FirstShow = true;

	if (FrameTimer.IsRunning()) FrameTimer.Stop();
	m_DrawBuf = NULL;
	FrameView->SetBackgroundStyle(wxBG_STYLE_PAINT); //Prevent flicker
	FrameSlider->Bind(wxEVT_SLIDER, &FFQJobEditAdvCuts::ActionClick, this);

    //Add order|First|Last|Sorted|Jump to|From|To|Sort now|Merge overlaps
	wxString menus = FFQS(SID_JOBEDIT_ADV_CUTS_MENU_ITEMS);
	FFQTokenParser tp(menus, PIPE);
	m_CutMenu = new wxMenu();
	m_CutMenu->Bind(wxEVT_MENU, &FFQJobEditAdvCuts::OnMenu, this);

	wxMenu *sub = new wxMenu();
	m_CutMenu->Append(MENU_ADD_SUBMENU, tp.next(), sub);

	sub->Append(MENU_ADD_FIRST, tp.next(), wxEmptyString, wxITEM_RADIO);
	sub->Append(MENU_ADD_LAST, tp.next(), wxEmptyString, wxITEM_RADIO);
	sub->Append(MENU_ADD_SORTED, tp.next(), wxEmptyString, wxITEM_RADIO)->Check();

	sub = new wxMenu();
	m_CutMenu->Append(MENU_JUMP_SUBMENU, tp.next(), sub);

	sub->Append(MENU_JUMP_TO_FROM, tp.next());
	sub->Append(MENU_JUMP_TO_TO, tp.next());

	m_CutMenu->Append(MENU_DUPLICATE, tp.next());
	m_CutMenu->Append(MENU_SORT_NOW, tp.next());
	m_CutMenu->Append(MENU_MERGE_OVERLAPS, tp.next());

	m_CutMenu->Append(MENU_MOVE_UP, FFQS(SID_COMMON_MOVE_UP));
	m_CutMenu->Append(MENU_MOVE_DOWN, FFQS(SID_COMMON_MOVE_DOWN));

	CutList->Bind(wxEVT_RIGHT_DOWN, &FFQJobEditAdvCuts::OnMouse, this);

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

    //Adjust control sizes
    if (m_FirstShow)
    {
        m_FirstShow = false;
        wxSize ms = OkButton->GetSize();
        ms.SetWidth(-1);
        FromTime->SetMinSize(ms);
        PrevFrame->SetMinSize(ms);
    }

    //Initialize the time span list from cuts
    CutList->Clear();
    wxString s = cuts.cuts;
    while (s.Len() > 0)
    {

        wxString f = GetToken(s, ";", true);
        CutList->Append(f + CUT_SEP + GetToken(s, ";", true));

    }

    //Cut settings
    QuickCut->SetValue(cuts.quick);
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

	//Disable accuracy by filter if accurate_seek is enabled
	/*CfgAccuracy->Enable(!FFQCFG()->accurate_seek);
	ST5->Enable(CfgAccuracy->IsEnabled());
	ST6->Enable(CfgAccuracy->IsEnabled());
	ST7->Enable(CfgAccuracy->IsEnabled());*/

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
        cuts.quick = QuickCut->GetValue();
        cuts.keep = KeepCuts->GetValue();
        cuts.filter_first = FilterFirst->GetValue();
        cuts.frame_time = m_FrameTime;
        cuts.cuts.Clear();

        if (CutList->GetCount() > 0)
        {

            //Store cuts
            for (unsigned int i = 0; i < CutList->GetCount(); i++)
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
    wxString s = StrTrim(FromTime->GetValue());
    TIME_VALUE tvf;
    if (!GetTimeValue(s, FrameConv->GetValue(), m_VidRate, tvf))
        return ShowError(FromTime, FrameConv->GetValue() ? FFQS(SID_JOBEDIT_ADV_CUTS_BAD_FRAME) : FFQS(SID_JOBEDIT_ADV_CUTS_BAD_TIME));

    //Validate to..
    s = StrTrim(ToTime->GetValue());
    TIME_VALUE tvt;
    if (!GetTimeValue(s, FrameConv->GetValue(), m_VidRate, tvt))
        return ShowError(ToTime, FrameConv->GetValue() ? FFQS(SID_JOBEDIT_ADV_CUTS_BAD_FRAME) : FFQS(SID_JOBEDIT_ADV_CUTS_BAD_TIME));

    //If to is 0, set to duration
    if (tvt.ToMilliseconds() == 0) tvt.SetMilliseconds(m_Duration.ToMilliseconds());

    //Validate to > from
    if (tvf.ToMilliseconds() >= tvt.ToMilliseconds())
        return ShowError(FrameConv->GetValue() ? FFQS(SID_JOBEDIT_ADV_CUTS_BAD_FRAME) : FFQS(SID_JOBEDIT_ADV_CUTS_BAD_TIME));

    //Sorted position in list
    long find_pos = -1;

    //Set the cut as a string
    cut = tvf.ToString(true, true) + CUT_SEP + tvt.ToString(true, true);

    //Find the sorted insert position
    for (long i = 0; i < (long)CutList->GetCount(); i++)
    {

        //Skip the cuts current position
        if (pos == i) continue;

        //Compare and break if found
        if (cut.Cmp(CutList->GetString(i)) <= 0)
        {
            find_pos = i;
            break;
        }

    }

    //Set the sorted position and return success
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
    pst->audio_codec = encode ? "aac" : "copy"; //Used to be aac
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

int FFQJobEditAdvCuts::MergeOverlaps(wxArrayString &merged)
{

    //Exit if overlap is not possible
    if (CutList->GetCount() < 2) return 0;

    //Get a copy of the cuts in the list
    //merged.Clear();
    for (size_t i = 0; i < CutList->GetCount(); i++) merged.Add(CutList->GetString(i));

    //Count the number of overlapping cuts and merge them
    int res = 0, i = 0, ii;
    uint64_t from1, to1, from2, to2;
    wxString s;

    while (i < (int)merged.Count() - 1) //The last cut cannot be tested against anything, so that is not tested
    {

        //Get the cut we are about to test for overlaps
        s = merged[i];
        from1 = TIME_VALUE(GetToken(s, CUT_SEP, true)).ToMilliseconds();
        to1 = TIME_VALUE(s).ToMilliseconds();

        //Test the cut against all the following cuts
        ii = i + 1;
        while (ii < (int)merged.Count())
        {

            //Get the item to test against
            s = merged[ii];
            from2 = TIME_VALUE(GetToken(s, CUT_SEP, true)).ToMilliseconds();
            to2 = TIME_VALUE(s).ToMilliseconds();

            //Test for overlaps
            if ( ((from1 >= from2) && (from1 < to2)) || //Beginning inside
                 ((to1 >= from2) && (to1 < to2)) || //End inside
                 ((from1 < from2) && (to1 >= to2)) ) //Complete overlap
            {

                //Merge the overlap by getting the first "from" and the last "to"
                if (from1 > from2) from1 = from2;
                if (to1 < to2) to1 = to2;

                //Set the merged cut back to the list
                merged[i] = TIME_VALUE(from1).ToString(true, true) + CUT_SEP + TIME_VALUE(to1).ToString(true, true);

                //Remove the overlapping / unused cut
                merged.RemoveAt(ii);

                //Increment overlap counter
                res++;

            }

            //If no overlap, increment index
            else ii++;

        }

        //Increment index
        i++;

    }

    //Return the number of merged items
    return res;

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
    unsigned int srt_iv = (siv < 5) ? 5 : (unsigned int)froundi(siv), sttl_idx = 0;
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
        sttl.Printf("%u\r\n%s --> %s\r\n%s\r\n\r\n", SIZEFMT(++sttl_idx), ft, tt, print.ToString());
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

    //Room for optimization?
    wxArrayInt selections;
    int sel = CutList->GetSelections(selections);

    /*bool sel = false;
    for(unsigned int i = 0; i < CutList->GetCount(); i++)
        if (CutList->IsSelected(i)) { sel=true; break; }*/

    //CutsMenu
    m_CutMenu->FindItem(MENU_JUMP_SUBMENU)->Enable(sel == 1);
    m_CutMenu->FindItem(MENU_MOVE_UP)->Enable((sel > 0) && (selections[0] > 0));
    m_CutMenu->FindItem(MENU_MOVE_DOWN)->Enable((sel > 0) && (selections[selections.Count() - 1] < (int)CutList->GetCount() - 1));
    m_CutMenu->FindItem(MENU_DUPLICATE)->Enable(sel > 0);

    //AddCut->Enable(true);
    RemoveCut->Enable(sel > 0);

    //Disable filer position controls if quick cuts are enabled
    ST11->Enable(!QuickCut->GetValue());
    FilterFirst->Enable(ST11->IsEnabled());
    FilterLast->Enable(ST11->IsEnabled());

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

void SwapItems(wxListBox *lb, size_t i1, size_t i2)
{
    wxString s1 = lb->GetString(i1), s2 = lb->GetString(i2);
    lb->Delete(i1);
    lb->Insert(s2, i1);
    lb->Delete(i2);
    lb->Insert(s1, i2);
    lb->Deselect(i1);
    lb->SetSelection(i2);
    //lb->SetSelection(i2);
}

void FFQJobEditAdvCuts::OnMenu(wxCommandEvent &event)
{

    int evtId = event.GetId();

    if (evtId == MENU_SORT_NOW)
    {

        wxArrayString items;
        for (size_t i = 0; i < CutList->GetCount(); i++) items.Add(CutList->GetString(i));
        items.Sort();
        CutList->Freeze();
        CutList->Set(items);
        CutList->Thaw();

    }

    if (evtId == MENU_DUPLICATE)
    {

        wxArrayString items;
        for (size_t i = 0; i < CutList->GetCount(); i++)
        {
            items.Add(CutList->GetString(i));
            if (CutList->IsSelected(i)) items.Add(CutList->GetString(i));
        }
        CutList->Freeze();
        CutList->Set(items);
        CutList->Thaw();

    }

    else if (evtId == MENU_MERGE_OVERLAPS)
    {

        wxArrayString merged;
        if (MergeOverlaps(merged) > 0)
        {

            CutList->Freeze();
            CutList->Set(merged);
            CutList->Thaw();

        }

    }

    else if ((evtId == MENU_JUMP_TO_FROM) || (evtId == MENU_JUMP_TO_TO))
    {

        wxArrayInt sel;
        if (CutList->GetSelections(sel) != 1) return;
        wxString c = CutList->GetString(sel[0]);
        if (evtId == MENU_JUMP_TO_FROM) c = GetToken(c, CUT_SEP, false);
        else GetToken(c, CUT_SEP, true);
        TIME_VALUE tv(c);
        FrameSlider->SetValue(tv.ToMilliseconds() / m_FrameTime);
        ExtractFrame();
        return; //Do not UpdateControls() twice..
    }

    else if ((evtId == MENU_MOVE_UP) || (evtId == MENU_MOVE_DOWN))
    {

        bool up = (evtId == MENU_MOVE_UP);
        wxArrayInt sel;
        CutList->GetSelections(sel);
        //if ((up && (sel[0] == 0)) || ((!up) && (sel[sel.Count() - 1] == CutList->GetCount() - 1))) return;
        CutList->Freeze();
        for (size_t i = 0; i < sel.Count(); i++)
        {
            int &from = sel[i], to = up ? from - 1 : from + 1;
            SwapItems(CutList, from, to);
            from = to;
        }
        for (size_t i = 0; i < CutList->GetCount(); i++)
            if (sel.Index((int)i) == wxNOT_FOUND) CutList->Deselect(i);
            else CutList->SetSelection(i);
        CutList->Thaw();


    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvCuts::OnMouse(wxMouseEvent &event)
{

    CutList->PopupMenu(m_CutMenu);

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
            if (m_CutMenu->FindItem(MENU_ADD_FIRST)->IsChecked()) pos = CutList->Insert(cut, 0);
            else if (m_CutMenu->FindItem(MENU_ADD_LAST)->IsChecked() || (pos < 0)) pos = CutList->Append(cut);
            else pos = CutList->Insert(cut, pos);
            CutList->EnsureVisible(pos);
            for (long i = 0; i < (long)CutList->GetCount(); i++)
                if (i == pos) CutList->SetSelection(i);
                else CutList->Deselect(i);
            FromTime->Clear();
            ToTime->Clear();

        }

    }

    else if (evtId == ID_REMOVECUT)
    {

        //Remove cuts
        ListBoxDeleteSelectedItems(CutList);

        /*wxArrayInt sel;
        CutList->GetSelections(sel);
        CutList->Freeze();
        for (long i = (long)sel.Count() - 1; i > -1; i--) CutList->Delete(sel[i]);
        CutList->Thaw();*/

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
        int frame = FrameSlider->GetValue() + (evtId == ID_PREVFRAME ? -JumpSize->GetValue() : JumpSize->GetValue());
        if (frame < 0) frame = 0;
        else if (frame > FrameSlider->GetMax()) frame = FrameSlider->GetMax();
        FrameSlider->SetValue(frame);//FrameSlider->GetValue() + (evtId == ID_PREVFRAME ? -1 : 1));
        ExtractFrame();

        //UpdateControls called in ExtractFrame
        return;

    }

    //Set current frame pos to from
    else if (evtId == ID_SETFROM) FromTime->SetValue(m_FramePos.ToString());

    //Set current frame pos to to
    else if (evtId == ID_SETTO) ToTime->SetValue(m_FramePos.ToString());

    //End dialog
    else if (evtId == ID_OKBUTTON)
    {

        if (RemoveCuts->GetValue())
        {

            //Since we cannot remove the same part twice, we must test for overlaps
            wxArrayString merged;
            int overlaps = MergeOverlaps(merged);
            if (overlaps > 0)
            {

                //Ask user if the overlaps should be merged?
                if (DoConfirm(CutList, FFQS(SID_JOBEDIT_ADV_CUTS_OVERLAP_REMOVE)))
                {

                    //Yep, set the merged items back to the list
                    CutList->Freeze();
                    CutList->Set(merged);
                    CutList->Thaw();

                }

                //Do not end modal; Let user review the changes
                return;

            }

        }

        //All OK, end the dialog
        EndModal(wxID_OK);

    }
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
