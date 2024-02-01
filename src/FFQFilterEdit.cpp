/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQFilterEdit.cpp                                               *
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

#include "FFQFilterEdit.h"
#include "FFQFullSpec.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQFilterEdit)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQFilterEdit)
//*)

//(*IdInit(FFQFilterEdit)
const long FFQFilterEdit::ID_OKBUTTON = wxNewId();
const long FFQFilterEdit::ID_CANCELBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQFilterEdit,wxDialog)
	//(*EventTable(FFQFilterEdit)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQFilterEdit::FFQFilterEdit(wxWindow* parent)
{
	//(*Initialize(FFQFilterEdit)
	wxBoxSizer* BoxSizer1;

	Create(parent, wxID_ANY, _T("Video filter"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	GridSizer = new wxFlexGridSizer(4, 1, 0, 0);
	FilterContainer = new wxStaticBoxSizer(wxVERTICAL, this, _T("Fc"));
	GridSizer->Add(FilterContainer, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OKButton = new wxButton(this, ID_OKBUTTON, _T("O"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBUTTON"));
	OKButton->SetDefault();
	OKButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer1->Add(OKButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	FilterContainer->GetStaticBox()->SetLabel(FFQS(SID_FILTER_DEFINE_PROPERTIES));
	BoxSizer1->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	GridSizer->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	SetSizer(GridSizer);
	GridSizer->Fit(this);
	GridSizer->SetSizeHints(this);

	Connect(ID_OKBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQFilterEdit::OnButtonClick);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQFilterEdit::OnButtonClick);
	//*)

}

//---------------------------------------------------------------------------------------

FFQFilterEdit::~FFQFilterEdit()
{
	//(*Destroy(FFQFilterEdit)
	//*)
}


//---------------------------------------------------------------------------------------

bool FFQFilterEdit::Execute(LPFFMPEG_FILTER filter)
{

    //Create properties panel
    m_Filter = filter;

    FULLSPEC_FILE *fs = nullptr;
    if (m_Filter->type == ftFULLSPEC)
    {
        int fsidx = FFQFullSpec::FindFullSpecFilter(m_Filter->ff_filter.AfterFirst(FILTER_VIDEO_IN.Last()).BeforeFirst(EQUAL));
        if (fsidx < 0) return false;
        fs = FFQFullSpec::GetFullSpec(fsidx);
    }

    m_FilterPanel = FilterBasePanel::GetFilterPanel(this, m_Filter->type, fs);
    if (m_FilterPanel == NULL) return ShowError(FFQSF(SID_FILTER_INVALID_ID, (unsigned int)m_Filter->type));

    FixControlSizes(m_FilterPanel);

    //Add to container and fit window
    FilterContainer->Add(m_FilterPanel, 1, wxALL|wxEXPAND, 3);
    FilterContainer->Layout();
	GridSizer->Fit(this);
	//GridSizer->SetSizeHints(this);

    //Set title of dialog
	SetTitle(fs == nullptr ? FFQL()->FILTER_NAMES[m_Filter->type] : fs->display);

    //Set property values
    m_FilterPanel->SetFilter(m_Filter);

    //Center dialog
    CenterOnParent();

    //ShowModal
    bool ok = (ShowModal() == wxID_OK);

    //Destroy property panel
    FilterContainer->Remove(0);

    delete m_FilterPanel;
    m_FilterPanel = NULL;

    return ok;

}

//---------------------------------------------------------------------------------------

void FFQFilterEdit::OnButtonClick(wxCommandEvent& event)
{

    if (event.GetId() == ID_CANCELBUTTON) EndModal(wxID_CANCEL);

    else if (event.GetId() == ID_OKBUTTON)
    {

        if (m_FilterPanel->GetFilter(m_Filter)) EndModal(wxID_OK);

    }
}
