/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPresetPanel.cpp                                              *
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

#include "FFQPresetPanel.h"
#include "FFQPresetMgr.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQPresetPanel)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQPresetPanel)
//*)

//(*IdInit(FFQPresetPanel)
const long FFQPresetPanel::ID_PRESETS = wxNewId();
const long FFQPresetPanel::ID_MGRBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQPresetPanel,wxPanel)
	//(*EventTable(FFQPresetPanel)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQPresetPanel::FFQPresetPanel(wxWindow* parent)
{
	//(*Initialize(FFQPresetPanel)
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	Presets = new wxChoice(this, ID_PRESETS, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_PRESETS"));
	FlexGridSizer1->Add(Presets, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	MgrButton = new wxButton(this, ID_MGRBUTTON, _T("..."), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_MGRBUTTON"));
	FlexGridSizer1->Add(MgrButton, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_MGRBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetPanel::OnAction);
	//*)

	m_FixedFirst = "";
}

//---------------------------------------------------------------------------------------

FFQPresetPanel::~FFQPresetPanel()
{
	//(*Destroy(FFQPresetPanel)
	//*)
}

//---------------------------------------------------------------------------------------

void FFQPresetPanel::FillAndSelect(wxString selectId)
{
    Presets->SetMaxSize(Presets->GetSize());
    FFQPresetMgr::Get()->FillChoice(Presets, selectId);
    if (m_FixedFirst.Len() > 0) Presets->Insert(m_FixedFirst, 0);
}

//---------------------------------------------------------------------------------------

void FFQPresetPanel::SelectPreset(LPFFQ_PRESET pst)
{
    int idx = (pst == NULL) ? -1 : FFQPresetMgr::Get()->IndexOf(pst);
    if (idx >= 0)
    {
        if (m_FixedFirst.Len() > 0) idx++;
        Presets->SetSelection(idx);
    }
}

//---------------------------------------------------------------------------------------

void FFQPresetPanel::SetFixedFirstItem(wxString item)
{

    //Remove previous
    if ((m_FixedFirst.Len() > 0) && (Presets->GetCount() > 0) && (Presets->GetString(0) == m_FixedFirst)) Presets->Delete(0);

    //Set current
    m_FixedFirst = item;
    if (m_FixedFirst.Len() > 0) Presets->Insert(m_FixedFirst, 0);

}

//---------------------------------------------------------------------------------------

LPFFQ_PRESET FFQPresetPanel::GetSelectedPreset()
{
    if (Presets->GetSelection() < 0) return NULL;
    return (LPFFQ_PRESET)Presets->GetClientData(Presets->GetSelection());
}

//---------------------------------------------------------------------------------------

void FFQPresetPanel::OnAction(wxCommandEvent& event)
{
    if (event.GetId() == ID_MGRBUTTON)
    {
        if (FFQPresetMgr::Get()->Execute(Presets) && (m_FixedFirst.Len() > 0)) Presets->Insert(m_FixedFirst, 0);
        Presets->SetFocus();
    }
}
