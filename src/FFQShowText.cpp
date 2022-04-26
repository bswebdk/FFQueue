/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQShowText.cpp                                                 *
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

#include "FFQShowText.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQShowText)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQShowText)
//*)

//(*IdInit(FFQShowText)
const long FFQShowText::ID_TEXT = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQShowText,wxDialog)
	//(*EventTable(FFQShowText)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQShowText::FFQShowText(wxWindow* parent)
{
	//(*Initialize(FFQShowText)
	wxBoxSizer* BoxSizer1;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	Text = new wxTextCtrl(this, ID_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_DONTWRAP|wxBORDER_NONE, wxDefaultValidator, _T("ID_TEXT"));
	Text->SetMinSize(wxSize(450,550));
	BoxSizer1->Add(Text, 1, wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)

	Text->SetBackgroundColour(wxColour(0xCCFFFF));
	fBold = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	fNormal = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    Text->SetDefaultStyle(wxTextAttr(wxNullColour, wxNullColour, fNormal));

}

//---------------------------------------------------------------------------------------

FFQShowText::~FFQShowText()
{
	//(*Destroy(FFQShowText)
	//*)
}

//---------------------------------------------------------------------------------------

void FFQShowText::Execute(const wxString &title, wxString text)
{

    Text->SetValue(text);
    SetTitleAndShow(title);

}

//---------------------------------------------------------------------------------------

void FFQShowText::Execute(const wxString &title, LPFFQ_NVP_LIST nvp)
{

    Text->Freeze();
    Text->Clear();

    //Find widest name
    unsigned int namew = 0;
    LPFFQ_NVP_LIST ptr = nvp;

    while (ptr)
    {

        if (ptr->name.Len() > namew) namew = ptr->name.Len();
        ptr = ptr->GetNext();

    }

    ptr = nvp;
    while (ptr)
    {

        wxString nm = ptr->name;
        nm.Pad(namew - nm.Len());
        Text->SetDefaultStyle(wxTextAttr(wxColour(0x770000)));
        Text->AppendText(nm);
        Text->SetDefaultStyle(wxTextAttr(*wxBLACK));
        Text->AppendText(" = ");
        Text->SetDefaultStyle(wxTextAttr(wxColour(0x007700)));
        Text->AppendText(ptr->value + "\n");
        ptr = ptr->GetNext();

    }

    Text->Thaw();
    SetTitleAndShow(title);

}

//---------------------------------------------------------------------------------------

void FFQShowText::SetTitleAndShow(const wxString &title)
{

    SetTitle(title);
    Text->SetSelection(0, 0);
    CenterOnParent();
    ShowModal();

}
