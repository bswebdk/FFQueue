/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQThumbMake.cpp                                                *
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

#include "FFQThumbMake.h"
#include "../utils/FFQLang.h"
#include "../utils/FFQConfig.h"
#include "../utils/FFQMisc.h"
#include "../utils/FFQProbing.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQThumbMake)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQThumbMake)
//*)

//(*IdInit(FFQThumbMake)
const long FFQThumbMake::ID_INFILE = wxNewId();
const long FFQThumbMake::ID_BROWSEBUTTON = wxNewId();
const long FFQThumbMake::ID_ThumbPanel = wxNewId();
const long FFQThumbMake::ID_SAVELOG = wxNewId();
const long FFQThumbMake::ID_MAKEBUTTON = wxNewId();
const long FFQThumbMake::ID_CANCELBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQThumbMake,wxDialog)
	//(*EventTable(FFQThumbMake)
	//*)
END_EVENT_TABLE()

FFQThumbMake::FFQThumbMake(wxWindow* parent)
{
	//(*Initialize(FFQThumbMake)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxStaticBoxSizer* SBS1;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	SBS1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("In"));
	FlexGridSizer2 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	FlexGridSizer2->AddGrowableRow(0);
	InFile = new wxTextCtrl(this, ID_INFILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_INFILE"));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_THUMBS_INPUT_FILE));
	FlexGridSizer2->Add(InFile, 1, wxALL|wxEXPAND, 0);
	BrowseButton = new wxButton(this, ID_BROWSEBUTTON, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BROWSEBUTTON"));
	FlexGridSizer2->Add(BrowseButton, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer1->Add(SBS1, 1, wxALL|wxEXPAND, 5);
	ThumbPanel = new FFQThumbPanel(this,ID_ThumbPanel);
	FlexGridSizer1->Add(ThumbPanel, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer3 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	FlexGridSizer3->AddGrowableRow(0);
	SaveLog = new wxCheckBox(this, ID_SAVELOG, _T("Log"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SAVELOG"));
	SaveLog->SetValue(true);
	SaveLog->SetLabel(FFQS(SID_COMMON_SAVE_LOG));
	FlexGridSizer3->Add(SaveLog, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MakeButton = new wxButton(this, ID_MAKEBUTTON, _T("M"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MAKEBUTTON"));
	MakeButton->SetDefault();
	MakeButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer1->Add(MakeButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer1->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FileDlg = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	FileDlg->SetMessage(FFQS(SID_COMMON_SELECT_INPUT_FILE));
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_BROWSEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQThumbMake::OnAction);
	Connect(ID_MAKEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQThumbMake::OnAction);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQThumbMake::OnAction);
	//*)

	SetTitle(FFQS(SID_THUMBS_TITLE));
	m_Reset = true;
	FFQCFG()->SetBrowseRootFor(FileDlg);


}

//---------------------------------------------------------------------------------------

FFQThumbMake::~FFQThumbMake()
{
	//(*Destroy(FFQThumbMake)
	//*)
}

//---------------------------------------------------------------------------------------

bool FFQThumbMake::Execute(LPFFQ_THUMB_JOB job)
{

    //Set edit pointer
    m_EditJob = job;

    if (job->inputs.Count() > 0)
    {

        //Load values from a job being edited
        InFile->ChangeValue(job->GetInput(0).path);
        ThumbPanel->SetValues(job->thumbs);
        SaveLog->SetValue(job->save_log);


    } else if (m_Reset) {


        //Reset to values from config
        ThumbPanel->ClearValues(true);

    }

    //Prevent second reset
    m_Reset = false;

    //Center
    CenterOnParent();

    //Launch dialog
    return (ShowModal() == wxID_OK);

}

//---------------------------------------------------------------------------------------

void FFQThumbMake::OnAction(wxCommandEvent& event)
{

    int evtId = event.GetId();

    if (evtId == ID_BROWSEBUTTON)
    {
        FileDlg->SetPath(InFile->GetValue());
        if (FileDlg->ShowModal() != wxID_CANCEL) InFile->SetValue(FileDlg->GetPath());
    }

    else if (evtId == ID_CANCELBUTTON) EndModal(wxID_CANCEL);

    else if (evtId == ID_MAKEBUTTON)
    {

        //Make sure the job is not running
        if (!m_EditJob->CheckNotActive()) return;

        //Validate input file
        FFQ_INPUT_FILE inf;
        inf.path = InFile->GetValue();
        if (!wxFileExists(inf.path))
        {
            ShowError(InFile, FFQS(SID_INPUT_FILE_NOT_FOUND));
            return;
        }

        //Validate and get thumb settings
        if (!ThumbPanel->GetValues(m_EditJob->thumbs, true)) return;

        //Set input file
        if (m_EditJob->inputs.Count() == 0) m_EditJob->inputs.Add(inf.ToString());
        else m_EditJob->inputs[0] = inf.ToString();

        //Set save log
        m_EditJob->save_log = SaveLog->GetValue();
        FFQCFG()->SetSaveLog(m_EditJob->save_log);

        //Return OK
        EndModal(wxID_OK);

    }

}
