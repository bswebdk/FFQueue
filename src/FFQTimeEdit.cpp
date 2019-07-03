/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQTimeEdit.cpp                                                 *
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

#include "FFQTimeEdit.h"
#include "utils/FFQLang.h"
#include "utils/FFQMisc.h"

#include <wx/valnum.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQTimeEdit)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQTimeEdit)
//*)

//(*IdInit(FFQTimeEdit)
const long FFQTimeEdit::ID_ST1 = wxNewId();
const long FFQTimeEdit::ID_HOURS = wxNewId();
const long FFQTimeEdit::ID_ST2 = wxNewId();
const long FFQTimeEdit::ID_MINUTES = wxNewId();
const long FFQTimeEdit::ID_ST3 = wxNewId();
const long FFQTimeEdit::ID_SECONDS = wxNewId();
const long FFQTimeEdit::ID_MILLISLABEL = wxNewId();
const long FFQTimeEdit::ID_MILLIS = wxNewId();
const long FFQTimeEdit::ID_CLEARBUTTON = wxNewId();
const long FFQTimeEdit::ID_OKBUTTON = wxNewId();
const long FFQTimeEdit::ID_CANCELBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQTimeEdit,wxDialog)
	//(*EventTable(FFQTimeEdit)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

const long FFQTimeEdit::ID_SUBTRACT = wxNewId();

//---------------------------------------------------------------------------------------

FFQTimeEdit::FFQTimeEdit(wxWindow* parent)
{
	//(*Initialize(FFQTimeEdit)
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* SBS1;
	wxBoxSizer* BoxSizer1;

	Create(parent, wxID_ANY, _T("Edit stream time"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	MainSizer = new wxFlexGridSizer(3, 1, 0, 0);
	SBS1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Etv"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 8, 0, 0);
	ST1 = new wxStaticText(this, ID_ST1, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST1"));
	ST1->SetLabel(FFQS(SID_TIMEEDIT_HOURS));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_TIMEEDIT_ENTER_VALUE));
	FlexGridSizer1->Add(ST1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Hours = new wxTextCtrl(this, ID_HOURS, wxEmptyString, wxDefaultPosition, wxSize(25,21), 0, wxDefaultValidator, _T("ID_HOURS"));
	FlexGridSizer1->Add(Hours, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ST2 = new wxStaticText(this, ID_ST2, _T("M"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST2"));
	ST2->SetLabel(FFQS(SID_TIMEEDIT_MINUTES));
	FlexGridSizer1->Add(ST2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Minutes = new wxTextCtrl(this, ID_MINUTES, wxEmptyString, wxDefaultPosition, wxSize(25,21), 0, wxDefaultValidator, _T("ID_MINUTES"));
	FlexGridSizer1->Add(Minutes, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ST3 = new wxStaticText(this, ID_ST3, _T("S"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST3"));
	ST3->SetLabel(FFQS(SID_TIMEEDIT_SECONDS));
	FlexGridSizer1->Add(ST3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Seconds = new wxTextCtrl(this, ID_SECONDS, wxEmptyString, wxDefaultPosition, wxSize(25,21), 0, wxDefaultValidator, _T("ID_SECONDS"));
	FlexGridSizer1->Add(Seconds, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MillisLabel = new wxStaticText(this, ID_MILLISLABEL, _T("Ms"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_MILLISLABEL"));
	MillisLabel->SetLabel(FFQS(SID_TIMEEDIT_MILLISECONDS));
	FlexGridSizer1->Add(MillisLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Millis = new wxTextCtrl(this, ID_MILLIS, wxEmptyString, wxDefaultPosition, wxSize(30,21), 0, wxDefaultValidator, _T("ID_MILLIS"));
	FlexGridSizer1->Add(Millis, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SBS1->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	MainSizer->Add(SBS1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SubtractSizer = new wxStaticBoxSizer(wxVERTICAL, this, _T("Sub"));
	MainSizer->Add(SubtractSizer, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	ClearButton = new wxButton(this, ID_CLEARBUTTON, _T("Cl"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLEARBUTTON"));
	ClearButton->SetLabel(FFQS(SID_COMMON_CLEAR));
	SubtractSizer->GetStaticBox()->SetLabel(FFQS(SID_TIMEEDIT_SUBTRACT_VALUES));
	BoxSizer1->Add(ClearButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OKButton = new wxButton(this, ID_OKBUTTON, _T("O"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBUTTON"));
	OKButton->SetDefault();
	OKButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer1->Add(OKButton, 1, wxALL|wxEXPAND, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer1->Add(CancelButton, 1, wxALL|wxEXPAND, 5);
	MainSizer->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	SetSizer(MainSizer);
	MainSizer->Fit(this);
	MainSizer->SetSizeHints(this);

	Connect(ID_CLEARBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQTimeEdit::OnButtonClick);
	Connect(ID_OKBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQTimeEdit::OnButtonClick);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQTimeEdit::OnButtonClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&FFQTimeEdit::OnClose);
	//*)

	SetTitle(FFQS(SID_TIMEEDIT_TITLE));
	wxIntegerValidator<unsigned short> hVal, mVal, msVal;
	hVal.SetRange(0, 100);
	Hours->SetValidator(hVal);

	mVal.SetRange(0, 59);
	Minutes->SetValidator(mVal);
	Seconds->SetValidator(mVal);

	msVal.SetRange(0, 999);
	Millis->SetValidator(msVal);

	//Adjust minimum control sizes
	wxClientDC cdc(this);
	cdc.SetFont(GetFont());
	int cw = cdc.GetTextExtent("0").GetWidth(), ch = Hours->GetClientSize().GetHeight();
	Hours->SetMinClientSize(wxSize(cw*4, ch));
	Minutes->SetMinClientSize(wxSize(cw*4, ch));
	Seconds->SetMinClientSize(wxSize(cw*4, ch));
	Millis->SetMinClientSize(wxSize(cw*6, ch));

}

//---------------------------------------------------------------------------------------

FFQTimeEdit::~FFQTimeEdit()
{
	//(*Destroy(FFQTimeEdit)
	//*)
}

//---------------------------------------------------------------------------------------

bool FFQTimeEdit::Execute(TIME_VALUE &timeVal, bool allowMilliseconds)
{
    return DoExecute(timeVal, NULL, 0, allowMilliseconds);
}

//---------------------------------------------------------------------------------------

bool FFQTimeEdit::Execute(TIME_VALUE &timeVal, LPTIME_VALUE subtractVal1, LPTIME_VALUE subtractVal2, bool allowMilliseconds)
{

    SUBTRACT_TIME *subtract = NULL;
    int cnt = 0;

    if ((subtractVal1 != NULL) || (subtractVal2 != NULL))
    {
        cnt = 2;
        subtract = new SUBTRACT_TIME[cnt];
        subtract[0].name = FFQS(SID_JOBEDIT_INPUT_NO_1);
        subtract[0].time = (subtractVal1 == NULL) ? 0 : *subtractVal1;
        subtract[1].name = FFQS(SID_JOBEDIT_INPUT_NO_2);
        subtract[1].time = (subtractVal2 == NULL) ? 0 : *subtractVal2;
    }

    return DoExecute(timeVal, subtract, cnt, allowMilliseconds);

}

//---------------------------------------------------------------------------------------

bool FFQTimeEdit::Execute(TIME_VALUE &timeVal, SUBTRACT_TIME subtractVals[], int subtractCount, bool allowMilliseconds)
{
    return DoExecute(timeVal, subtractVals, subtractCount, allowMilliseconds);
}

//---------------------------------------------------------------------------------------

int FFQTimeEdit::ButtonIndex(wxButton *button)
{

    //Return the index of the button in the subtract sizer
    for(unsigned int i = 0; i < SubtractSizer->GetItemCount(); i++)
        if (button == SubtractSizer->GetItem(i)->GetWindow()) return i;

    //Not found
    return -1;

}

//---------------------------------------------------------------------------------------

bool FFQTimeEdit::DoExecute(TIME_VALUE &time, SUBTRACT_TIME subtract[], int count, bool allowMillis)
{

    //Set the time value
    SetTimeValue(time);

    //Set the subtract list
    m_Subtract = subtract;

    //Add buttons to SubtractSizer
    wxButton *b = NULL;
    for (int i = 0; i < count; i++)
    {

        b = new wxButton(this, ID_SUBTRACT);
        b->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FFQTimeEdit::OnButtonClick, this);
        SubtractSizer->Add(b, 1, wxALL|wxEXPAND, 3);

    }

    //Adjust tab order
    OKButton->MoveAfterInTabOrder((b == NULL) ? (wxWindow*)Millis : (wxWindow*)b);
    CancelButton->MoveAfterInTabOrder(OKButton);
    ClearButton->MoveAfterInTabOrder(CancelButton);

    //Hide button sizer if unused, and fit window
    MainSizer->Show(SubtractSizer, count > 0, true);
    MainSizer->Fit(this);

    //Disable milliseconds if not required
    MillisLabel->Enable(allowMillis);
    Millis->Enable(allowMillis);

    UpdateControls();
    CenterOnParent();
    Hours->SelectAll();
    Hours->SetFocus();

    int res = ShowModal();

    //Clear subtract sizer
    SubtractSizer->Clear(true);

    //Release subtract values
    if (m_Subtract)
    {
        delete[] m_Subtract;
        m_Subtract = NULL;
    }

    if (res == wxID_OK)
    {
        time = GetTimeValue();
        return true;
    }

    if (res == wxID_NONE)
    {
        time = 0;
        return true;
    }

    return false;

}

//---------------------------------------------------------------------------------------

TIME_VALUE FFQTimeEdit::GetTimeValue()
{
    return TIME_VALUE(Str2Long(Hours->GetValue()), Str2Long(Minutes->GetValue()), Str2Long(Seconds->GetValue()), Str2Long(Millis->GetValue()));
}

//---------------------------------------------------------------------------------------

void FFQTimeEdit::SetTimeValue(TIME_VALUE &val)
{
    Hours->SetValue(ToStr(val.hours));
    Minutes->SetValue(ToStr(val.minutes));
    Seconds->SetValue(ToStr(val.seconds));
    Millis->SetValue(ToStr(val.milliseconds));
}

//---------------------------------------------------------------------------------------

void FFQTimeEdit::SubtractTimeValue(LPTIME_VALUE time)
{
    TIME_VALUE tv(Str2Long(Hours->GetValue()), Str2Long(Minutes->GetValue()), Str2Long(Seconds->GetValue()), Str2Long(Millis->GetValue()));
    tv = TIME_VALUE(tv.ToMilliseconds() - time->ToMilliseconds());
    SetTimeValue(tv);
    *time = 0;
    UpdateControls();
}

//---------------------------------------------------------------------------------------

void FFQTimeEdit::UpdateControls()
{

    if (!MillisLabel->IsEnabled()) return; //No need to update buttons..

    //Update subtract buttons
    for (unsigned int i = 0; i < SubtractSizer->GetItemCount(); i++)
    {

        wxButton *b = (wxButton*)SubtractSizer->GetItem(i)->GetWindow();
        TIME_VALUE time = m_Subtract[i].time;
        b->Enable(!time.IsUndefined());
        if (time.IsUndefined()) b->SetLabel(FFQS(SID_TIMEEDIT_NOT_AVAILABLE));
        else b->SetLabel(FFQSF(SID_TIMEEDIT_SUBTRACT_TIME, m_Subtract[i].name, time.ToString()));

    }

}

//---------------------------------------------------------------------------------------

void FFQTimeEdit::OnButtonClick(wxCommandEvent& event)
{

    int evtId = event.GetId();

    if (evtId == ID_SUBTRACT) SubtractTimeValue(&m_Subtract[ButtonIndex((wxButton*)event.GetEventObject())].time);

    else if (evtId == ID_CLEARBUTTON) EndModal(wxID_NONE);

    else if (evtId == ID_CANCELBUTTON) EndModal(wxID_CANCEL);

    else if (evtId == ID_OKBUTTON) EndModal(wxID_OK);

}

//---------------------------------------------------------------------------------------

void FFQTimeEdit::OnClose(wxCloseEvent& event)
{
    EndModal(wxID_CANCEL);
}
