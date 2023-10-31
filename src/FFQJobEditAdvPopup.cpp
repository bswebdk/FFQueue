/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQJobEditAdvPopup.cpp                                          *
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

#include "FFQJobEditAdvPopup.h"
#include "FFQJobEditAdv.h"
#include "utils/FFQLang.h"
#include "utils/FFQMisc.h"

#include <wx/valnum.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQJobEditAdvPopup)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQJobEditAdvPopup)
#include <wx/font.h>
#include <wx/settings.h>
//*)

//(*IdInit(FFQJobEditAdvPopup)
const long FFQJobEditAdvPopup::ID_PANEL1 = wxNewId();
const long FFQJobEditAdvPopup::ID_ITSOFFSET = wxNewId();
const long FFQJobEditAdvPopup::ID_STATICTEXT1 = wxNewId();
const long FFQJobEditAdvPopup::ID_LOOPSTREAMS = wxNewId();
const long FFQJobEditAdvPopup::ID_STATICTEXT2 = wxNewId();
const long FFQJobEditAdvPopup::ID_STATICLINE1 = wxNewId();
const long FFQJobEditAdvPopup::ID_CLEARBTN = wxNewId();
const long FFQJobEditAdvPopup::ID_OKBTN = wxNewId();
const long FFQJobEditAdvPopup::ID_NOBTN = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQJobEditAdvPopup,wxDialog)
	//(*EventTable(FFQJobEditAdvPopup)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQJobEditAdvPopup::FFQJobEditAdvPopup(wxWindow* parent)
{

	//(*Initialize(FFQJobEditAdvPopup)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxBORDER_STATIC|wxWANTS_CHARS, _T("wxID_ANY"));
	FlexGridSizer4 = new wxFlexGridSizer(7, 1, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));
	Panel1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	ST0 = new wxStaticText(Panel1, wxID_ANY, _T("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	wxFont ST0Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	ST0->SetFont(ST0Font);
	ST0->SetLabel(FFQS(SID_JOBEDIT_ADV_INPUT_SETTINGS));
	BoxSizer2->Add(ST0, 1, wxALL|wxEXPAND, 5);
	Panel1->SetSizer(BoxSizer2);
	BoxSizer2->Fit(Panel1);
	BoxSizer2->SetSizeHints(Panel1);
	FlexGridSizer4->Add(Panel1, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer2 = new wxFlexGridSizer(3, 3, 0, 0);
	ST1 = new wxStaticText(this, wxID_ANY, _T("L1"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST1->SetLabel(FFQS(SID_JOBEDIT_ADV_ITSOFFSET));
	FlexGridSizer2->Add(ST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ItsOffset = new wxSpinCtrl(this, ID_ITSOFFSET, _T("0"), wxDefaultPosition, wxDefaultSize, 0, -999999, 999999, 0, _T("ID_ITSOFFSET"));
	ItsOffset->SetValue(_T("0"));
	FlexGridSizer2->Add(ItsOffset, 1, wxALL|wxEXPAND, 3);
	ST2 = new wxStaticText(this, wxID_ANY, _T("L2"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST2->SetLabel(FFQS(SID_JOBEDIT_ADV_MILLISECONDS));
	FlexGridSizer2->Add(ST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ST5 = new wxStaticText(this, ID_STATICTEXT1, _T("L5"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	ST5->SetLabel(FFQS(SID_JOBEDIT_ADV_LOOP_STREAMS));
	FlexGridSizer2->Add(ST5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	LoopStreams = new wxSpinCtrl(this, ID_LOOPSTREAMS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, -1, 99999, 0, _T("ID_LOOPSTREAMS"));
	LoopStreams->SetValue(_T("0"));
	FlexGridSizer2->Add(LoopStreams, 1, wxALL|wxEXPAND, 3);
	ST6 = new wxStaticText(this, ID_STATICTEXT2, _T("L6"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	ST6->SetLabel(FFQS(SID_JOBEDIT_ADV_LOOP_STREAMS_INFO));
	FlexGridSizer2->Add(ST6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ST3 = new wxStaticText(this, wxID_ANY, _T("L3"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST3->SetLabel(FFQS(SID_JOBEDIT_ADV_FRAMERATE));
	FlexGridSizer2->Add(ST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FrameRate = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer2->Add(FrameRate, 1, wxALL|wxEXPAND, 3);
	ST4 = new wxStaticText(this, wxID_ANY, _T("L4"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST4->SetLabel(FFQS(SID_JOBEDIT_ADV_FPS));
	FlexGridSizer2->Add(ST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer4->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1 = new wxFlexGridSizer(1, 1, 0, 0);
	FrameRateSwitch = new wxCheckBox(this, wxID_ANY, _T("Fr"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	FrameRateSwitch->SetValue(false);
	FrameRateSwitch->SetLabel(FFQS(SID_JOBEDIT_ADV_FRAMERATE_SWITCH));
	FlexGridSizer1->Add(FrameRateSwitch, 1, wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer4->Add(FlexGridSizer1, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	Line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL, _T("wxID_ANY"));
	FlexGridSizer4->Add(Line, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
	DiscardCorrupt = new wxCheckBox(this, wxID_ANY, _T("Dc"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	DiscardCorrupt->SetValue(false);
	DiscardCorrupt->SetLabel(FFQS(SID_JOBEDIT_ADV_DISCARDCORRUPT));
	FlexGridSizer3->Add(DiscardCorrupt, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	GenPTS = new wxCheckBox(this, wxID_ANY, _T("Gp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	GenPTS->SetValue(false);
	GenPTS->SetLabel(FFQS(SID_JOBEDIT_ADV_GENPTS));
	FlexGridSizer3->Add(GenPTS, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	IgnDTS = new wxCheckBox(this, wxID_ANY, _T("Id"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	IgnDTS->SetValue(false);
	IgnDTS->SetLabel(FFQS(SID_JOBEDIT_ADV_IGNOREDTS));
	FlexGridSizer3->Add(IgnDTS, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	IgnIdx = new wxCheckBox(this, wxID_ANY, _T("Ii"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	IgnIdx->SetValue(false);
	IgnIdx->SetLabel(FFQS(SID_JOBEDIT_ADV_IGNOREIDX));
	FlexGridSizer3->Add(IgnIdx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer4->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer4->Add(StaticLine1, 1, wxALL|wxEXPAND, 0);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	ClearBtn = new wxButton(this, ID_CLEARBTN, _T("Cl"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLEARBTN"));
	ClearBtn->SetLabel(FFQS(SID_COMMON_CLEAR));
	BoxSizer1->Add(ClearBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	OkBtn = new wxButton(this, ID_OKBTN, _T("Ok"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBTN"));
	OkBtn->SetDefault();
	OkBtn->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer1->Add(OkBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	NoBtn = new wxButton(this, ID_NOBTN, _T("No"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_NOBTN"));
	NoBtn->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer1->Add(NoBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer4->Add(BoxSizer1, 1, wxALL|wxEXPAND, 3);
	SetSizer(FlexGridSizer4);
	FlexGridSizer4->Fit(this);
	FlexGridSizer4->SetSizeHints(this);

	Connect(ID_CLEARBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvPopup::OnAction);
	Connect(ID_OKBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvPopup::OnAction);
	Connect(ID_NOBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdvPopup::OnAction);
	Connect(wxEVT_CHAR,(wxObjectEventFunction)&FFQJobEditAdvPopup::OnChar);
	//*)

	//wxIntegerValidator<int> offsval(NULL, wxNUM_VAL_ZERO_AS_BLANK);
	//offsval.SetRange(-1000000, 1000000);
	//ItsOffset->SetValidator(offsval);

	Connect(wxID_ANY, wxEVT_CHAR_HOOK, (wxObjectEventFunction)&FFQJobEditAdvPopup::OnChar);

}

//---------------------------------------------------------------------------------------

FFQJobEditAdvPopup::~FFQJobEditAdvPopup()
{
	//(*Destroy(FFQJobEditAdvPopup)
	//*)
}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdvPopup::Execute(void* data)
{

    //Cast data
    LPINPUT_CTRLS ctrls = (LPINPUT_CTRLS)data;

    //Load values
    ItsOffset->SetValue(ctrls->itsoffset.Len() == 0 ? "0" : ctrls->itsoffset);
    wxString fr = ctrls->framerate;
    FrameRateSwitch->SetValue(fr.StartsWith('~'));
    if (FrameRateSwitch->GetValue()) fr.Remove(0, 1);
    FrameRate->SetValue(fr);
    LoopStreams->SetValue(ctrls->loop_streams.Len() == 0 ? "0" : ctrls->loop_streams);
    DiscardCorrupt->SetValue(ctrls->discard_corrupt);
    GenPTS->SetValue(ctrls->genpts);
    IgnDTS->SetValue(ctrls->igndts);
    IgnIdx->SetValue(ctrls->ignidx);

    ItsOffset->SetFocus();

    if (ShowModal() != wxID_OK) return false;

    //Save values
    ctrls->itsoffset = ItsOffset->GetValue() == 0 ? "" : ToStr(ItsOffset->GetValue());
    ctrls->framerate = wxString(FrameRateSwitch->GetValue() ? "~" : "") + FrameRate->GetValue();
    ctrls->loop_streams = LoopStreams->GetValue() == 0 ? "" : ToStr(LoopStreams->GetValue());
    ctrls->discard_corrupt = DiscardCorrupt->GetValue();
    ctrls->genpts = GenPTS->GetValue();
    ctrls->igndts = IgnDTS->GetValue();
    ctrls->ignidx = IgnIdx->GetValue();

    return true;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvPopup::ResetCtrls()
{

    //Reset controls to default values
    ItsOffset->SetValue(0);
    LoopStreams->SetValue(0);
    FrameRate->Clear();
    FrameRateSwitch->SetValue(false);
    DiscardCorrupt->SetValue(false);
    GenPTS->SetValue(false);
    IgnDTS->SetValue(false);
    IgnIdx->SetValue(false);
    ItsOffset->SetFocus();

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvPopup::OnAction(wxCommandEvent& event)
{

    //Button click handler

    int evt_id = event.GetId();

    if (evt_id == ID_CLEARBTN) ResetCtrls();

    else if (evt_id == ID_OKBTN)
    {

        //Validate the content
        if (IsValidFrameRate(FrameRate->GetValue())) EndModal(wxID_OK);

        else
        {

            //Invalid, show and focus
            ShowError(FrameRate, FFQS(SID_PRESET_INVALID_FRAME_RATE));
            FrameRate->SelectAll();

        }

    }

    //Canceled?
    else if (evt_id == ID_NOBTN) EndModal(wxID_CANCEL);

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdvPopup::OnChar(wxKeyEvent& event)
{

    //Cancel if escape, skip otherwise
    if (event.GetKeyCode() == WXK_ESCAPE) EndModal(wxID_CANCEL);
    else event.Skip();

}
