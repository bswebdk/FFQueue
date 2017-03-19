/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQBitRatePanel.cpp                                             *
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

#include "FFQBitRatePanel.h"
#include "utils/FFQMisc.h"
#include "utils/FFQLang.h"

#include <wx/valnum.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQBitRatePanel)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQBitRatePanel)
//*)

//(*IdInit(FFQBitRatePanel)
const long FFQBitRatePanel::ID_ST1 = wxNewId();
const long FFQBitRatePanel::ID_BITRATETYPE = wxNewId();
const long FFQBitRatePanel::ID_ST2 = wxNewId();
const long FFQBitRatePanel::ID_BITRATE = wxNewId();
const long FFQBitRatePanel::ID_BST1 = wxNewId();
const long FFQBitRatePanel::ID_ST3 = wxNewId();
const long FFQBitRatePanel::ID_MINRATE = wxNewId();
const long FFQBitRatePanel::ID_BST2 = wxNewId();
const long FFQBitRatePanel::ID_ST5 = wxNewId();
const long FFQBitRatePanel::ID_BUFSIZE = wxNewId();
const long FFQBitRatePanel::ID_BST4 = wxNewId();
const long FFQBitRatePanel::ID_ST4 = wxNewId();
const long FFQBitRatePanel::ID_MAXRATE = wxNewId();
const long FFQBitRatePanel::ID_BST3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQBitRatePanel,wxPanel)
	//(*EventTable(FFQBitRatePanel)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQBitRatePanel::FFQBitRatePanel(wxWindow* parent)
{

	//(*Initialize(FFQBitRatePanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	MainSizer = new wxFlexGridSizer(5, 6, 0, 0);
	MainSizer->AddGrowableCol(2);
	ST1 = new wxStaticText(this, ID_ST1, _T("Brt"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST1"));
	ST1->SetLabel(FFQS(SID_PRESET_BITRATE_DECLARED_AS));
	MainSizer->Add(ST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	BitRateType = new wxChoice(this, ID_BITRATETYPE, wxDefaultPosition, wxSize(100,-1), 0, 0, 0, wxDefaultValidator, _T("ID_BITRATETYPE"));
	BitRateType->Append(FFQS(SID_PRESET_BITRATE_BIT_SEC));
	BitRateType->SetSelection( BitRateType->Append(FFQS(SID_PRESET_BITRATE_KBIT_SEC)) );
	BitRateType->Append(FFQS(SID_PRESET_BITRATE_MBIT_SEC));
	BitRateType->Append(FFQS(SID_PRESET_BITRATE_BYTE_SEC));
	BitRateType->Append(FFQS(SID_PRESET_BITRATE_KBYTE_SEC));
	BitRateType->Append(FFQS(SID_PRESET_BITRATE_MBYTE_SEC));
	MainSizer->Add(BitRateType, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	MainSizer->Add(80,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	MainSizer->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	MainSizer->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	MainSizer->Add(80,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ST2 = new wxStaticText(this, ID_ST2, _T("Abr"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST2"));
	ST2->SetLabel(FFQS(SID_PRESET_AVERAGE_BITRATE));
	MainSizer->Add(ST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	BitRate = new wxTextCtrl(this, ID_BITRATE, wxEmptyString, wxDefaultPosition, wxSize(100,-1), 0, wxDefaultValidator, _T("ID_BITRATE"));
	MainSizer->Add(BitRate, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BST1 = new wxStaticText(this, ID_BST1, _T("Brt"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_BST1"));
	MainSizer->Add(BST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ST3 = new wxStaticText(this, ID_ST3, _T("Min"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST3"));
	ST3->SetLabel(FFQS(SID_PRESET_MINIMUM_BITRATE));
	MainSizer->Add(ST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	MinRate = new wxTextCtrl(this, ID_MINRATE, wxEmptyString, wxDefaultPosition, wxSize(100,-1), 0, wxDefaultValidator, _T("ID_MINRATE"));
	MainSizer->Add(MinRate, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BST2 = new wxStaticText(this, ID_BST2, _T("Brt"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_BST2"));
	MainSizer->Add(BST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ST5 = new wxStaticText(this, ID_ST5, _T("Bs"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST5"));
	ST5->SetLabel(FFQS(SID_PRESET_BUFFER_SIZE));
	MainSizer->Add(ST5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	BufSize = new wxTextCtrl(this, ID_BUFSIZE, wxEmptyString, wxDefaultPosition, wxSize(100,-1), 0, wxDefaultValidator, _T("ID_BUFSIZE"));
	MainSizer->Add(BufSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BST4 = new wxStaticText(this, ID_BST4, _T("Brt"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_BST4"));
	MainSizer->Add(BST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ST4 = new wxStaticText(this, ID_ST4, _T("Max"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST4"));
	ST4->SetLabel(FFQS(SID_PRESET_MAXIMUM_BITRATE));
	MainSizer->Add(ST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	MaxRate = new wxTextCtrl(this, ID_MAXRATE, wxEmptyString, wxDefaultPosition, wxSize(100,-1), 0, wxDefaultValidator, _T("ID_MAXRATE"));
	MainSizer->Add(MaxRate, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BST3 = new wxStaticText(this, ID_BST3, _T("Brt"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_BST3"));
	MainSizer->Add(BST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SetSizer(MainSizer);
	MainSizer->Fit(this);
	MainSizer->SetSizeHints(this);

	Connect(ID_BITRATETYPE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&FFQBitRatePanel::OnChoiceChange);
	//*)

	wxIntegerValidator<int> iVal;
	iVal.SetRange(0, 999999);
	iVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK);

	BitRate->SetValidator(iVal);
	MinRate->SetValidator(iVal);
	MaxRate->SetValidator(iVal);
	BufSize->SetValidator(iVal);

}

//---------------------------------------------------------------------------------------

FFQBitRatePanel::~FFQBitRatePanel()
{
	//(*Destroy(FFQBitRatePanel)
	//*)
}

//---------------------------------------------------------------------------------------

void FFQBitRatePanel::SetValues(wxString values)
{

    //Set values to controls
    BitRate->SetValue(GetToken(values, ',', true));
    BitRateType->SetSelection(Str2Long(GetToken(values, ',', true), 1));
    MinRate->SetValue(GetToken(values, ',', true));
    MaxRate->SetValue(GetToken(values, ',', true));
    BufSize->SetValue(GetToken(values, ',', true));

	UpdateLabels();

}

//---------------------------------------------------------------------------------------

bool FFQBitRatePanel::GetValues(wxString &values, bool showErr)
{

    //Max rate requires bufsize
    bool ok = (MaxRate->GetValue() == "") || (BufSize->GetValue() != "");

    //Show error if required
    if ((!ok) && showErr) return ShowError(FFQS(SID_PRESET_BUFFER_SIZE_REQUIRED));

    //Make values
    wxString br = BitRate->GetValue(), mir = MinRate->GetValue(), mar = MaxRate->GetValue(), buf = BufSize->GetValue();

    //If no values are set clear the resulting string
    if (br.Len() + mir.Len() + mar.Len() + buf.Len() == 0) values.Clear();

    //Values are set - pack them to the resulting string
    else values.Printf("%s,%i,%s,%s,%s", br, BitRateType->GetSelection(), mir, mar, buf);

    //Return result
    return ok;

}

//---------------------------------------------------------------------------------------

void FFQBitRatePanel::UpdateLabels()
{
    wxString l = BitRateType->GetStringSelection();
    BST1->SetLabel(l);
    BST2->SetLabel(l);
    BST3->SetLabel(l);
    BST4->SetLabel(l);
    //MainSizer->RecalcSizes();
    //MainSizer->Layout();
}


//---------------------------------------------------------------------------------------

void FFQBitRatePanel::OnChoiceChange(wxCommandEvent& event)
{
    event.Skip();
    UpdateLabels();
}
