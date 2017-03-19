/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQVidStab.cpp                                                  *
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

#include "FFQVidStab.h"
#include "../utils/FFQLang.h"
#include "../utils/FFQConfig.h"
#include "../utils/FFQBuildCmd.h"
#include "../utils/FFQConst.h"
#include "../utils/FFQMisc.h"
#include "../utils/FFQFilter.h"
#include "../utils/FFQStreamMapping.h"

#include <wx/filename.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQVidStab)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQVidStab)
//*)

//(*IdInit(FFQVidStab)
const long FFQVidStab::ID_SRCFILE = wxNewId();
const long FFQVidStab::ID_BROWSESRC = wxNewId();
const long FFQVidStab::ID_FILEINFO = wxNewId();
const long FFQVidStab::ID_SHAKINESS = wxNewId();
const long FFQVidStab::ID_ACCURACY = wxNewId();
const long FFQVidStab::ID_STEPSIZE = wxNewId();
const long FFQVidStab::ID_MINCONTRAST = wxNewId();
const long FFQVidStab::ID_TRIPODFRAME = wxNewId();
const long FFQVidStab::ID_PAGE0 = wxNewId();
const long FFQVidStab::ID_SMOOTHING = wxNewId();
const long FFQVidStab::ID_MAXSHIFT = wxNewId();
const long FFQVidStab::ID_MAXANGLE = wxNewId();
const long FFQVidStab::ID_ZOOM = wxNewId();
const long FFQVidStab::ID_ZOOMSPEED = wxNewId();
const long FFQVidStab::ID_OPTALGO = wxNewId();
const long FFQVidStab::ID_INTERPOLATION = wxNewId();
const long FFQVidStab::ID_OPTZOOM = wxNewId();
const long FFQVidStab::ID_CROPBLACK = wxNewId();
const long FFQVidStab::ID_INVERT = wxNewId();
const long FFQVidStab::ID_RELATIVE = wxNewId();
const long FFQVidStab::ID_VIRTUALTRIPOD = wxNewId();
const long FFQVidStab::ID_PAGE1 = wxNewId();
const long FFQVidStab::ID_ONLY1ST = wxNewId();
const long FFQVidStab::ID_VERBOSE = wxNewId();
const long FFQVidStab::ID_ONLY2ND = wxNewId();
const long FFQVidStab::ID_KEEPTRANSFORMS = wxNewId();
const long FFQVidStab::ID_PAGE2 = wxNewId();
const long FFQVidStab::ID_PAGES = wxNewId();
const long FFQVidStab::ID_DESTFILE = wxNewId();
const long FFQVidStab::ID_BROWSEDEST = wxNewId();
const long FFQVidStab::ID_PRESET = wxNewId();
const long FFQVidStab::ID_SAVELOG = wxNewId();
const long FFQVidStab::ID_DEFAULTSBUTTON = wxNewId();
const long FFQVidStab::ID_OKBUTTON = wxNewId();
const long FFQVidStab::ID_CANCELBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQVidStab,wxDialog)
	//(*EventTable(FFQVidStab)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQVidStab::FFQVidStab(wxWindow* parent)
{

    wxString *OptAlgoItems, *InterpolationItems, *OptZoomItems;

    OptAlgoItems = FFQL()->GetStringArray(SID_VIDSTAB_OPT_ALGO_ITEMS, VIDSTAB_OPT_ALGO_COUNT);
    InterpolationItems = FFQL()->GetStringArray(SID_VIDSTAB_INTERPOLATION_ITEMS, VIDSTAB_INTERPOLATION_COUNT);
    OptZoomItems = FFQL()->GetStringArray(SID_VIDSTAB_OPT_ZOOM_ITEMS, VIDSTAB_OPT_ZOOM_COUNT);

	//(*Initialize(FFQVidStab)
	wxFlexGridSizer* FlexGridSizer8;
	wxStaticText* ST13;
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* SBS1;
	wxFlexGridSizer* FlexGridSizer2;
	wxStaticText* ST00;
	wxStaticBoxSizer* SBS2;
	wxStaticText* ST2;
	wxStaticText* ST11;
	wxFlexGridSizer* FlexGridSizer11;
	wxStaticText* ST1;
	wxStaticText* ST7;
	wxStaticText* ST12;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FirstPassSizer;
	wxStaticText* ST8;
	wxBoxSizer* BoxSizer2;
	wxStaticText* ST20;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer9;
	wxFlexGridSizer* FlexGridSizer14;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer3;
	wxStaticText* ST4;
	wxFlexGridSizer* FlexGridSizer10;
	wxBoxSizer* BoxSizer1;
	wxStaticText* ST3;
	wxStaticText* ST9;
	wxStaticText* ST01;
	wxFlexGridSizer* FlexGridSizer5;
	wxStaticText* ST21;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(4, 1, 0, 0);
	SBS1 = new wxStaticBoxSizer(wxVERTICAL, this, _T("If"));
	FlexGridSizer2 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	SrcFile = new wxTextCtrl(this, ID_SRCFILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SRCFILE"));
	SrcFile->SetMinSize(wxSize(400,-1));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_VIDSTAB_IN_FILE));
	FlexGridSizer2->Add(SrcFile, 1, wxALL|wxEXPAND, 3);
	BrowseSrc = new wxButton(this, ID_BROWSESRC, _T("..."), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_BROWSESRC"));
	FlexGridSizer2->Add(BrowseSrc, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FileInfo = new wxStaticText(this, ID_FILEINFO, _T("Inf"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_FILEINFO"));
	SBS1->Add(FileInfo, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer1->Add(SBS1, 1, wxALL|wxEXPAND, 3);
	Pages = new wxNotebook(this, ID_PAGES, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PAGES"));
	Page0 = new wxPanel(Pages, ID_PAGE0, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PAGE0"));
	FirstPassSizer = new wxFlexGridSizer(3, 1, 0, 0);
	FirstPassSizer->AddGrowableCol(0);
	FlexGridSizer4 = new wxFlexGridSizer(5, 3, 0, 0);
	FlexGridSizer4->AddGrowableCol(1);
	FlexGridSizer4->Add(-1,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer5 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer5->AddGrowableCol(0);
	FlexGridSizer5->AddGrowableCol(1);
	ST00 = new wxStaticText(Page0, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer5->Add(ST00, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 10);
	ST01 = new wxStaticText(Page0, wxID_ANY, _T("+"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer5->Add(ST01, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 10);
	FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer4->Add(-1,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ST1 = new wxStaticText(Page0, wxID_ANY, _T("Shk"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST1->SetLabel(FFQS(SID_VIDSTAB_SHAKINESS));
	FlexGridSizer4->Add(ST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Shakiness = new wxSlider(Page0, ID_SHAKINESS, 5, 1, 10, wxDefaultPosition, wxSize(-1,20), 0, wxDefaultValidator, _T("ID_SHAKINESS"));
	FlexGridSizer4->Add(Shakiness, 1, wxALL|wxEXPAND, 3);
	ShakinessText = new wxStaticText(Page0, wxID_ANY, _T("0000"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ShakinessText->SetMinSize(ShakinessText->GetSize());
	FlexGridSizer4->Add(ShakinessText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ST2 = new wxStaticText(Page0, wxID_ANY, _T("Acc"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST2->SetLabel(FFQS(SID_VIDSTAB_ACCURACY));
	FlexGridSizer4->Add(ST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Accuracy = new wxSlider(Page0, ID_ACCURACY, 15, 1, 15, wxDefaultPosition, wxSize(-1,20), 0, wxDefaultValidator, _T("ID_ACCURACY"));
	FlexGridSizer4->Add(Accuracy, 1, wxALL|wxEXPAND, 3);
	AccuracyText = new wxStaticText(Page0, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	AccuracyText->SetMinSize(ShakinessText->GetSize());
	FlexGridSizer4->Add(AccuracyText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ST3 = new wxStaticText(Page0, wxID_ANY, _T("Ss"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST3->SetLabel(FFQS(SID_VIDSTAB_STEP_SIZE));
	FlexGridSizer4->Add(ST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	StepSize = new wxSlider(Page0, ID_STEPSIZE, 6, 1, 32, wxDefaultPosition, wxSize(-1,20), 0, wxDefaultValidator, _T("ID_STEPSIZE"));
	FlexGridSizer4->Add(StepSize, 1, wxALL|wxEXPAND, 3);
	StepSizeText = new wxStaticText(Page0, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	StepSizeText->SetMinSize(ShakinessText->GetSize());
	FlexGridSizer4->Add(StepSizeText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ST4 = new wxStaticText(Page0, wxID_ANY, _T("MC"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST4->SetLabel(FFQS(SID_VIDSTAB_MIN_CONTRAST));
	FlexGridSizer4->Add(ST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	MinContrast = new wxSlider(Page0, ID_MINCONTRAST, 30, 0, 100, wxDefaultPosition, wxSize(-1,20), 0, wxDefaultValidator, _T("ID_MINCONTRAST"));
	FlexGridSizer4->Add(MinContrast, 1, wxALL|wxEXPAND, 3);
	MinContrastText = new wxStaticText(Page0, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	MinContrastText->SetMinSize(ShakinessText->GetSize());
	FlexGridSizer4->Add(MinContrastText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FirstPassSizer->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer6 = new wxFlexGridSizer(1, 2, 0, 0);
	ST5 = new wxStaticText(Page0, wxID_ANY, _T("Tri"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST5->SetLabel(FFQS(SID_VIDSTAB_TRIPOD_REF_FRAME));
	FlexGridSizer6->Add(ST5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	TripodFrame = new wxSpinCtrl(Page0, ID_TRIPODFRAME, _T("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 10000000, 0, _T("ID_TRIPODFRAME"));
	TripodFrame->SetValue(_T("0"));
	FlexGridSizer6->Add(TripodFrame, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FirstPassSizer->Add(FlexGridSizer6, 1, wxALL|wxEXPAND, 3);
	Page0->SetSizer(FirstPassSizer);
	FirstPassSizer->Fit(Page0);
	FirstPassSizer->SetSizeHints(Page0);
	Page1 = new wxPanel(Pages, ID_PAGE1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PAGE1"));
	FlexGridSizer8 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer8->AddGrowableCol(0);
	FlexGridSizer9 = new wxFlexGridSizer(5, 2, 0, 0);
	FlexGridSizer9->AddGrowableCol(1);
	ST7 = new wxStaticText(Page1, wxID_ANY, _T("Sm"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST7->SetLabel(FFQS(SID_VIDSTAB_SMOOTHING));
	FlexGridSizer9->Add(ST7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Smoothing = new wxSpinCtrl(Page1, ID_SMOOTHING, _T("10"), wxDefaultPosition, wxSize(100,-1), 0, 0, 10000000, 10, _T("ID_SMOOTHING"));
	Smoothing->SetValue(_T("10"));
	FlexGridSizer9->Add(Smoothing, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	ST8 = new wxStaticText(Page1, wxID_ANY, _T("Ms"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST8->SetLabel(FFQS(SID_VIDSTAB_MAX_SHIFT));
	FlexGridSizer9->Add(ST8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	MaxShift = new wxSpinCtrl(Page1, ID_MAXSHIFT, _T("0"), wxDefaultPosition, wxSize(100,-1), 0, -1, 1000000, 0, _T("ID_MAXSHIFT"));
	MaxShift->SetValue(_T("0"));
	FlexGridSizer9->Add(MaxShift, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	ST9 = new wxStaticText(Page1, wxID_ANY, _T("Ma"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST9->SetLabel(FFQS(SID_VIDSTAB_MAX_ANGLE));
	FlexGridSizer9->Add(ST9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	MaxAngle = new wxSpinCtrl(Page1, ID_MAXANGLE, _T("0"), wxDefaultPosition, wxSize(100,-1), 0, -1, 180, 0, _T("ID_MAXANGLE"));
	MaxAngle->SetValue(_T("0"));
	FlexGridSizer9->Add(MaxAngle, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	ST13 = new wxStaticText(Page1, wxID_ANY, _T("Zm"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST13->SetLabel(FFQS(SID_VIDSTAB_ZOOM));
	FlexGridSizer9->Add(ST13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Zoom = new wxSpinCtrl(Page1, ID_ZOOM, _T("0"), wxDefaultPosition, wxSize(100,-1), 0, -100, 100, 0, _T("ID_ZOOM"));
	Zoom->SetValue(_T("0"));
	FlexGridSizer9->Add(Zoom, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	ZoomSpeedText = new wxStaticText(Page1, wxID_ANY, _T("ZmS"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ZoomSpeedText->SetLabel(FFQS(SID_VIDSTAB_ZOOM_SPEED));
	FlexGridSizer9->Add(ZoomSpeedText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ZoomSpeed = new wxSpinCtrl(Page1, ID_ZOOMSPEED, _T("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 100, 0, _T("ID_ZOOMSPEED"));
	ZoomSpeed->SetValue(_T("0"));
	FlexGridSizer9->Add(ZoomSpeed, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer8->Add(FlexGridSizer9, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer10 = new wxFlexGridSizer(3, 2, 0, 0);
	FlexGridSizer10->AddGrowableCol(1);
	ST10 = new wxStaticText(Page1, wxID_ANY, _T("Opt"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST10->SetLabel(FFQS(SID_VIDSTAB_OPT_ALGO));
	FlexGridSizer10->Add(ST10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OptAlgo = new wxChoice(Page1, ID_OPTALGO, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_OPTALGO"));
	OptAlgo->Append(VIDSTAB_OPT_ALGO_COUNT, OptAlgoItems);
	FlexGridSizer10->Add(OptAlgo, 1, wxALL|wxEXPAND, 3);
	ST11 = new wxStaticText(Page1, wxID_ANY, _T("Intp"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST11->SetLabel(FFQS(SID_VIDSTAB_INTERPOLATION));
	FlexGridSizer10->Add(ST11, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Interpolation = new wxChoice(Page1, ID_INTERPOLATION, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_INTERPOLATION"));
	Interpolation->Append(VIDSTAB_INTERPOLATION_COUNT, InterpolationItems);
	FlexGridSizer10->Add(Interpolation, 1, wxALL|wxEXPAND, 3);
	ST12 = new wxStaticText(Page1, wxID_ANY, _T("OpZm"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST12->SetLabel(FFQS(SID_VIDSTAB_OPT_ZOOM));
	FlexGridSizer10->Add(ST12, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OptZoom = new wxChoice(Page1, ID_OPTZOOM, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_OPTZOOM"));
	OptZoom->Append(VIDSTAB_OPT_ZOOM_COUNT, OptZoomItems);
	FlexGridSizer10->Add(OptZoom, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer8->Add(FlexGridSizer10, 1, wxALL|wxEXPAND, 3);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	CropBlack = new wxCheckBox(Page1, ID_CROPBLACK, _T("Crop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CROPBLACK"));
	CropBlack->SetValue(false);
	CropBlack->SetLabel(FFQS(SID_VIDSTAB_CROP_FILL_BLACK));
	BoxSizer1->Add(CropBlack, 1, wxALL|wxALIGN_LEFT, 3);
	Invert = new wxCheckBox(Page1, ID_INVERT, _T("Inv"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_INVERT"));
	Invert->SetValue(false);
	Invert->SetLabel(FFQS(SID_VIDSTAB_INVERT));
	BoxSizer1->Add(Invert, 1, wxALL|wxALIGN_LEFT, 3);
	Relative = new wxCheckBox(Page1, ID_RELATIVE, _T("Rel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RELATIVE"));
	Relative->SetValue(false);
	Relative->SetLabel(FFQS(SID_VIDSTAB_RELATIVE));
	BoxSizer1->Add(Relative, 1, wxALL|wxALIGN_LEFT, 3);
	VirtualTripod = new wxCheckBox(Page1, ID_VIRTUALTRIPOD, _T("VTri"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_VIRTUALTRIPOD"));
	VirtualTripod->SetValue(false);
	VirtualTripod->SetLabel(FFQS(SID_VIDSTAB_VIRTUAL_TRIPOD));
	BoxSizer1->Add(VirtualTripod, 1, wxALL|wxALIGN_LEFT, 3);
	FlexGridSizer8->Add(BoxSizer1, 1, wxALL|wxEXPAND, 3);
	Page1->SetSizer(FlexGridSizer8);
	FlexGridSizer8->Fit(Page1);
	FlexGridSizer8->SetSizeHints(Page1);
	Page2 = new wxPanel(Pages, ID_PAGE2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PAGE2"));
	FlexGridSizer11 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer11->AddGrowableCol(0);
	FlexGridSizer14 = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer14->AddGrowableCol(0);
	Only1st = new wxCheckBox(Page2, ID_ONLY1ST, _T("1st"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ONLY1ST"));
	Only1st->SetValue(false);
	Only1st->SetLabel(FFQS(SID_VIDSTAB_ONLY_1ST_PASS));
	FlexGridSizer14->Add(Only1st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer3 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(1);
	FlexGridSizer3->AddGrowableRow(0);
	FlexGridSizer3->Add(15,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	Verbose = new wxCheckBox(Page2, ID_VERBOSE, _T("Vb"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_VERBOSE"));
	Verbose->SetValue(false);
	Verbose->SetLabel(FFQS(SID_VIDSTAB_VERBOSE));
	FlexGridSizer3->Add(Verbose, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer14->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 3);
	Only2nd = new wxCheckBox(Page2, ID_ONLY2ND, _T("2nd"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ONLY2ND"));
	Only2nd->SetValue(false);
	Only2nd->SetLabel(FFQS(SID_VIDSTAB_ONLY_2ND_PASS));
	FlexGridSizer14->Add(Only2nd, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KeepTransforms = new wxCheckBox(Page2, ID_KEEPTRANSFORMS, _T("Keep"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_KEEPTRANSFORMS"));
	KeepTransforms->SetValue(false);
	KeepTransforms->SetLabel(FFQS(SID_VIDSTAB_KEEP_TRANSFORMS));
	FlexGridSizer14->Add(KeepTransforms, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer11->Add(FlexGridSizer14, 1, wxALL|wxEXPAND, 5);
	Page2->SetSizer(FlexGridSizer11);
	FlexGridSizer11->Fit(Page2);
	FlexGridSizer11->SetSizeHints(Page2);
	Pages->AddPage(Page0, _T("1st"), false);
	Pages->AddPage(Page1, _T("2nd"), false);
	Pages->AddPage(Page2, _T("Opt"), false);
	FlexGridSizer1->Add(Pages, 1, wxALL|wxEXPAND, 3);
	SBS2 = new wxStaticBoxSizer(wxVERTICAL, this, _T("Dst"));
	FlexGridSizer7 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer7->AddGrowableCol(1);
	FlexGridSizer7->AddGrowableRow(0);
	ST20 = new wxStaticText(this, wxID_ANY, _T("Fn"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST20->SetLabel(FFQS(SID_CONCAT_FILE_NAME));
	SBS2->GetStaticBox()->SetLabel(FFQS(SID_CONCAT_DESTINATION));
	FlexGridSizer7->Add(ST20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	DestFile = new wxTextCtrl(this, ID_DESTFILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DESTFILE"));
	FlexGridSizer7->Add(DestFile, 1, wxALL|wxEXPAND, 3);
	BrowseDest = new wxButton(this, ID_BROWSEDEST, _T("..."), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_BROWSEDEST"));
	FlexGridSizer7->Add(BrowseDest, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS2->Add(FlexGridSizer7, 1, wxBOTTOM|wxEXPAND, 2);
	Sizer3 = new wxFlexGridSizer(1, 2, 0, 0);
	Sizer3->AddGrowableCol(1);
	Sizer3->AddGrowableRow(0);
	ST21 = new wxStaticText(this, wxID_ANY, _T("Pst"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST21->SetLabel(FFQS(SID_CONCAT_PRESET));
	Sizer3->Add(ST21, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	Preset = new FFQPresetPanel(this);
	Sizer3->Add(Preset, 1, wxALL|wxEXPAND, 3);
	SBS2->Add(Sizer3, 1, wxBOTTOM|wxEXPAND, 2);
	SaveLog = new wxCheckBox(this, ID_SAVELOG, _T("Log"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SAVELOG"));
	SaveLog->SetValue(false);
	SaveLog->SetLabel(FFQS(SID_COMMON_SAVE_LOG));
	SBS2->Add(SaveLog, 1, wxALL|wxALIGN_LEFT, 2);
	FlexGridSizer1->Add(SBS2, 1, wxALL|wxEXPAND, 3);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	DefaultsButton = new wxButton(this, ID_DEFAULTSBUTTON, _T("D"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DEFAULTSBUTTON"));
	DefaultsButton->SetLabel(FFQS(SID_COMMON_DEFAULTS));
	BoxSizer2->Add(DefaultsButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OkButton = new wxButton(this, ID_OKBUTTON, _T("Y"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBUTTON"));
	OkButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer2->Add(OkButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("N"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer2->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	OpenFileDlg = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, _T("*.*"), wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	OpenFileDlg->SetMessage(FFQS(SID_COMMON_SELECT_INPUT_FILE));
	SaveFileDlg = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, _T("*.*"), wxFD_SAVE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	SaveFileDlg->SetMessage(FFQS(SID_COMMON_SELECT_OUTPUT_FILE));
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_SRCFILE,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_BROWSESRC,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_SHAKINESS,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQVidStab::OnSliderChange);
	Connect(ID_ACCURACY,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQVidStab::OnSliderChange);
	Connect(ID_STEPSIZE,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQVidStab::OnSliderChange);
	Connect(ID_MINCONTRAST,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQVidStab::OnSliderChange);
	Connect(ID_OPTZOOM,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_ONLY1ST,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_ONLY2ND,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_BROWSEDEST,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_DEFAULTSBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_OKBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVidStab::OnAction);
	//*)

    delete[] OptAlgoItems;
    delete[] InterpolationItems;
    delete[] OptZoomItems;

	SetTitle(FFQS(SID_VIDSTAB_TITLE));
    Pages->SetPageText(0, FFQS(SID_VIDSTAB_1ST_PASS));
    Pages->SetPageText(1, FFQS(SID_VIDSTAB_2ND_PASS));
    Pages->SetPageText(2, FFQS(SID_VIDSTAB_OTHER_OPTIONS));

    //m_FirstExecute = true;
    m_LastPreset = "";

    Connect(wxID_ANY, wxEVT_IDLE, (wxObjectEventFunction)&FFQVidStab::OnIdle);

}

//---------------------------------------------------------------------------------------

FFQVidStab::~FFQVidStab()
{

	//(*Destroy(FFQVidStab)
	//*)

}

//---------------------------------------------------------------------------------------

bool FFQVidStab::Execute(LPFFQ_VIDSTAB_JOB job)
{

    m_EditJob = job;

    if (job->inputs.Count() > 0)
    {
        //The job is being edited, load values
        SrcFile->ChangeValue(job->GetInput(0).path);
        DestFile->ChangeValue(job->out);
        Preset->FillAndSelect(job->preset.ToString());
        SetValuesFrom(job);

    }

    else
    {

        //Clear dialog
        SrcFile->Clear();
        DestFile->Clear();
        Preset->FillAndSelect(m_LastPreset);
        DefaultSettings(FFQCFG()->vidstab_settings);

    }

    //Save log?
    SaveLog->SetValue(job->save_log);

    //Reset
    m_StreamMap = job->stream_map;
    m_Duration = job->duration;
    m_SrcValid = false;
    m_LastSrc.Clear();
    FileInfo->SetLabel("");

    Pages->SetSelection(0);
    UpdateControls(true);

    //Execute dialog
    if (ShowModal() == wxID_OK)
    {

        //Store values
        FFQ_INPUT_FILE inf;
        inf.path = SrcFile->GetValue();
        job->inputs.Clear();
        job->inputs.Add(inf.ToString());
        job->out = DestFile->GetValue();
        job->stream_map = m_StreamMap;
        job->duration = m_Duration;
        job->preset = Preset->GetSelectedPreset()->preset_id;
        job->save_log = SaveLog->GetValue();
        FFQCFG()->SetSaveLog(job->save_log);


        //1st pass
        job->shakiness = Shakiness->GetValue();
        job->accuracy = Accuracy->GetValue();
        job->step_size = StepSize->GetValue();
        job->min_contrast = MinContrast->GetValue();
        job->tripod_frame = TripodFrame->GetValue();

        //2nd pass
        job->smoothing = Smoothing->GetValue();
        job->max_shift = MaxShift->GetValue();
        job->max_angle = MaxAngle->GetValue();
        job->zoom = Zoom->GetValue();
        job->zoom_speed = ZoomSpeed->GetValue();
        job->opt_algo = OptAlgo->GetSelection();
        job->interpolation = Interpolation->GetSelection();
        job->opt_zoom = OptZoom->GetSelection();
        job->crop_black = CropBlack->GetValue();
        job->invert_trf = Invert->GetValue();
        job->relative_trf = Relative->GetValue();
        job->virtual_tripod = VirtualTripod->GetValue();

        //Other options
        job->only_1st = Only1st->GetValue();
        job->verbose = Verbose->GetValue();
        job->only_2nd = Only2nd->GetValue();
        job->keep_trf = KeepTransforms->GetValue();

        //Save current settings to APPCFG
        wxString cfg;//, dst = DestFile->GetValue();
        cfg.Printf(

            "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%s,%s,%s,%s,%s,%s,%s,%s,%s",

            //1st pass
            Shakiness->GetValue(), Accuracy->GetValue(), StepSize->GetValue(), MinContrast->GetValue(), TripodFrame->GetValue(),

            //2nd pass
            Smoothing->GetValue(), MaxShift->GetValue(), MaxAngle->GetValue(), Zoom->GetValue(), ZoomSpeed->GetValue(),
            OptAlgo->GetSelection(), Interpolation->GetSelection(), OptZoom->GetSelection(),
            BOOLSTR(CropBlack->GetValue()), BOOLSTR(Invert->GetValue()),
            BOOLSTR(Relative->GetValue()), BOOLSTR(VirtualTripod->GetValue()),

            //Other
            BOOLSTR(Only1st->GetValue()), BOOLSTR(Verbose->GetValue()),
            BOOLSTR(Only2nd->GetValue()), BOOLSTR(KeepTransforms->GetValue()),

            //Log
            BOOLSTR(SaveLog->GetValue())


        );

        //Have settings changed?
        bool cfg_changed = (cfg != FFQCFG()->vidstab_settings);

        //Update settings in APPCFG regardless if it is necessary or not
        FFQCFG()->vidstab_settings = cfg;

        //Set preferred and save (if necessary)
        wxString dst = DestFile->GetValue();
        FFQCFG()->SetPreferredAndSave(dst.AfterLast('.'), dst.BeforeLast(wxFileName::GetPathSeparator()), cfg_changed);

        return true;

    }

    return false;

}

//---------------------------------------------------------------------------------------

void FFQVidStab::DefaultSettings(wxString load_cfg)
{

    FFQ_VIDSTAB_JOB job; //Now has default settings

    if (load_cfg.Len() > 0)
    {

        //Parse values from arg

        //1st pass
        job.shakiness = Str2Long(GetToken(load_cfg, ","), job.shakiness);
        job.accuracy = Str2Long(GetToken(load_cfg, ","), job.accuracy);
        job.step_size = Str2Long(GetToken(load_cfg, ","), job.step_size);
        job.min_contrast = Str2Long(GetToken(load_cfg, ","), job.min_contrast);
        job.tripod_frame = Str2Long(GetToken(load_cfg, ","), job.tripod_frame);

        //2nd pass
        job.smoothing = Str2Long(GetToken(load_cfg, ","), job.smoothing);
        job.max_shift = Str2Long(GetToken(load_cfg, ","), job.max_shift);
        job.max_angle = Str2Long(GetToken(load_cfg, ","), job.max_angle);
        job.zoom = Str2Long(GetToken(load_cfg, ","), job.zoom);
        job.zoom_speed = Str2Long(GetToken(load_cfg, ","), job.zoom_speed);
        job.opt_algo = Str2Long(GetToken(load_cfg, ","), job.opt_algo);
        job.interpolation = Str2Long(GetToken(load_cfg, ","), job.interpolation);
        job.opt_zoom = Str2Long(GetToken(load_cfg, ","), job.opt_zoom);
        job.crop_black = Str2Long(GetToken(load_cfg, ","), job.crop_black);
        job.invert_trf = Str2Long(GetToken(load_cfg, ","), job.invert_trf);
        job.relative_trf = Str2Long(GetToken(load_cfg, ","), job.relative_trf);
        job.virtual_tripod = Str2Long(GetToken(load_cfg, ","), job.virtual_tripod);


        //Other options
        job.only_1st = STRBOOL(GetToken(load_cfg, ","));
        job.verbose = STRBOOL(GetToken(load_cfg, ","));
        job.only_2nd = STRBOOL(GetToken(load_cfg, ","));
        job.keep_trf = STRBOOL(GetToken(load_cfg, ","));

        //Log
        job.save_log = STRBOOL(GetToken(load_cfg, ","));

    }

    SetValuesFrom(&job);

}

//---------------------------------------------------------------------------------------

void FFQVidStab::SetValuesFrom(LPFFQ_VIDSTAB_JOB job)
{

    //1st pass
    Shakiness->SetValue(job->shakiness);
    Accuracy->SetValue(job->accuracy);
    StepSize->SetValue(job->step_size);
    MinContrast->SetValue(job->min_contrast);
    TripodFrame->SetValue(job->tripod_frame);

    //2nd pass
    Smoothing->SetValue(job->smoothing);
    MaxShift->SetValue(job->max_shift);
    MaxAngle->SetValue(job->max_angle);
    Zoom->SetValue(job->zoom);
    ZoomSpeed->SetValue(job->zoom_speed);
    OptAlgo->SetSelection(job->opt_algo);
    Interpolation->SetSelection(job->interpolation);
    OptZoom->SetSelection(job->opt_zoom);
    CropBlack->SetValue(job->crop_black);
    Invert->SetValue(job->invert_trf);
    Relative->SetValue(job->relative_trf);
    VirtualTripod->SetValue(job->virtual_tripod);

    //Other options
    Only1st->SetValue(job->only_1st);
    Verbose->SetValue(job->verbose);
    Only2nd->SetValue(job->only_2nd);
    KeepTransforms->SetValue(job->keep_trf);

    //Log
    SaveLog->SetValue(job->save_log);

}

//---------------------------------------------------------------------------------------

void FFQVidStab::UpdateControls(bool SliderLabels)
{

    //2nd pass
    Page1->Enable(!Only1st->GetValue());
    ZoomSpeedText->Enable(OptZoom->GetSelection() == 2);
    ZoomSpeed->Enable(ZoomSpeedText->IsEnabled());

    //Other options
    Verbose->Enable(Only1st->GetValue());
    Only2nd->Enable(wxFileExists(SrcFile->GetValue() + VIDSTAB_TRANSFORM_EXT));
    if (!Only2nd->IsEnabled()) Only2nd->SetValue(false);

    //1st pass - requires "Only2nd" to be updated first
    Page0->Enable(!Only2nd->GetValue());

    if (SliderLabels)
    {

        wxScrollEvent evt;
        OnSliderChange(evt);

    }

}

//---------------------------------------------------------------------------------------

void FFQVidStab::OnIdle(wxIdleEvent &event)
{

    event.Skip();

    //Get info about source file
    wxString src = SrcFile->GetValue();
    bool src_ok = (src.Len() > 0) && wxFileExists(src);

    if ((!src_ok) && Only2nd->GetValue())
    {
        Only2nd->SetValue(false);
        UpdateControls();
    }

    //Set preferred destination file
    if ( (DestFile->GetValue().Len() == 0) &&  src_ok) DestFile->SetValue(FFQCFG()->GetPreferredOutputName(src));

    if (src != m_LastSrc)
    {

        //Set m_LastSrc to avoid endless working
        m_LastSrc = src;

        if (src_ok)
        {

            //Examinate the source file and update
            if (!m_PIP.RunFFProbe(src)) ShowError(SrcFile, FFQS(SID_FFPROBE_BAD_OUTPUT));

            //Get duration of source file
            if (!m_PIP.GetDuration(m_Duration)) m_Duration = 0;

            int auds = 0, subs = 0;
            LPFFPROBE_STREAM_INFO vid = NULL;

            //Make stream for source file mapping
            STREAM_MAPPING sm;
            sm.file_id = 1;
            sm.checked = true;
            m_StreamMap = "";
            for (unsigned int i = 0; i < m_PIP.GetStreamCount(); i++)
            {

                LPFFPROBE_STREAM_INFO si = m_PIP.GetStreamInfo(i);
                if (!si->IsTeleText())
                {

                    if (si->codec_type == CODEC_TYPE_AUDIO) auds++;
                    else if (si->codec_type == CODEC_TYPE_SUBTITLE) subs++;
                    else if (si->codec_type == CODEC_TYPE_VIDEO) vid = si;

                    sm.codec_type = si->codec_type;
                    sm.stream_id = i;
                    if (m_StreamMap.Len() > 0) m_StreamMap += STREAM_MAPPING_SEPERATOR;

                    m_StreamMap += sm.ToString();

                }

            }

            if (vid != NULL)
            {

                FileInfo->SetLabel( FFQSF(SID_VIDSTAB_FILE_INFO, m_Duration.ToString(), vid->codec_name, vid->width, vid->height, auds, subs) );
                m_SrcValid = true;
                return;

            }

        }

        //Clear file info
        FileInfo->SetLabel("");

    }

}

//---------------------------------------------------------------------------------------

void FFQVidStab::OnSliderChange(wxScrollEvent& event)
{

    //event.Skip();
    wxObject *o = event.GetEventObject();
    if ((o == NULL) || (o == Shakiness)) ShakinessText->SetLabel(ToStr(Shakiness->GetValue()));
    if ((o == NULL) || (o == Accuracy)) AccuracyText->SetLabel(ToStr(Accuracy->GetValue()));
    if ((o == NULL) || (o == StepSize)) StepSizeText->SetLabel(ToStr(StepSize->GetValue()));
    if ((o == NULL) || (o == MinContrast)) MinContrastText->SetLabel(wxString::Format("%.2f", (double)MinContrast->GetValue() / 100.0));

}

//---------------------------------------------------------------------------------------

void FFQVidStab::OnAction(wxCommandEvent& event)
{

    //event.Skip();
    int evtId = event.GetId();

    if (evtId == ID_BROWSESRC)
    {

        OpenFileDlg->SetPath(SrcFile->GetValue());
        if (OpenFileDlg->ShowModal() != wxID_CANCEL) SrcFile->SetValue(OpenFileDlg->GetPath());

    }

    else if (evtId == ID_BROWSEDEST)
    {

        SaveFileDlg->SetPath(DestFile->GetValue());
        if (SaveFileDlg->ShowModal() != wxID_CANCEL) DestFile->SetValue(SaveFileDlg->GetPath());

    }

    else if (evtId == ID_ONLY1ST) Only2nd->SetValue(false);
    else if (evtId == ID_ONLY2ND) Only1st->SetValue(false);

    else if (evtId == ID_DEFAULTSBUTTON)
    {

        DefaultSettings();
        UpdateControls(true);
        return;

    }

    else if (evtId == ID_CANCELBUTTON)
    {

        EndModal(wxID_CANCEL);
        return;

    }

    else if (evtId == ID_OKBUTTON)
    {

        //Make sure the job is not running
        if (!m_EditJob->CheckNotActive()) return;

        //Validate source file
        if (!wxFileExists(SrcFile->GetValue()))
        {

            ShowError(SrcFile, FFQS(SID_INPUT_FILE_NOT_FOUND));
            return;

        }

        //Validate destination file
        wxString dest = DestFile->GetValue();
        if (dest.Len() == 0)
        {

            ShowError(DestFile, FFQS(SID_OUTPUT_FILE_REQUIRED));
            return;

        }

        //Confirm overwrite
        if (wxFileExists(dest) && (!DoConfirm(FFQS(SID_OVERWRITE_OUTPUT_FILE)))) return;

        //Validate preset
        LPFFQ_PRESET pst = Preset->GetSelectedPreset();
        if (pst == NULL)
        {

            ShowError(Preset, FFQS(SID_INVALID_PRESET));
            return;

        }

        else if (pst->video_codec == CODEC_COPY)
        {

            ShowError(Preset, FFQS(SID_COPY_VIDEO_NOT_ALLOWED));
            return;

        }

        else if (pst->two_pass)
        {

            ShowError(Preset, FFQS(SID_TWO_PASS_NOT_ALLOWED));
            return;

        }

        //All green
        EndModal(wxID_OK);
        return;

    }

    UpdateControls();

}
