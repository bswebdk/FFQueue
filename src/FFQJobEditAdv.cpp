/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQJobEditAdv.cpp                                               *
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

#include "FFQJobEditAdv.h"
#include "utils/FFQLang.h"
#include "FFQMain.h"
#include "FFQPresetMgr.h"
#include "utils/FFQStreamMapping.h"
#include "utils/FFQConfig.h"
#include "utils/FFQCompress.h"
#include "utils/FFQMisc.h"
#include "utils/FFQHash.h"
#include "utils/FFQBuildCmd.h"
#include "bin_res.h"

#include <wx/filename.h>
#include <wx/tooltip.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQJobEditAdv)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQJobEditAdv)
//*)

//(*IdInit(FFQJobEditAdv)
const long FFQJobEditAdv::ID_INPUTS = wxNewId();
const long FFQJobEditAdv::ID_ADDINPUT = wxNewId();
const long FFQJobEditAdv::ID_DELINPUT = wxNewId();
const long FFQJobEditAdv::ID_ADDSECONDARY = wxNewId();
const long FFQJobEditAdv::ID_STREAMLIST = wxNewId();
const long FFQJobEditAdv::ID_STREAMUP = wxNewId();
const long FFQJobEditAdv::ID_STREAMDOWN = wxNewId();
const long FFQJobEditAdv::ID_STREAMREFRESH = wxNewId();
const long FFQJobEditAdv::ID_OUTPUT = wxNewId();
const long FFQJobEditAdv::ID_BROWSEOUTPUT = wxNewId();
const long FFQJobEditAdv::ID_OUTPUTLENGTH = wxNewId();
const long FFQJobEditAdv::ID_STATICTEXT3 = wxNewId();
const long FFQJobEditAdv::ID_CMDLINE = wxNewId();
const long FFQJobEditAdv::ID_STATICTEXT4 = wxNewId();
const long FFQJobEditAdv::ID_PRESET = wxNewId();
const long FFQJobEditAdv::ID_SAVELOG = wxNewId();
const long FFQJobEditAdv::ID_SAVEJOB = wxNewId();
const long FFQJobEditAdv::ID_CANCELDLG = wxNewId();
//*)

//Assign static ID's
const long FFQJobEditAdv::ID_BROWSE = wxNewId();
const long FFQJobEditAdv::ID_PLAY = wxNewId();
const long FFQJobEditAdv::ID_START = wxNewId();
const long FFQJobEditAdv::ID_CUTS = wxNewId();
const long FFQJobEditAdv::ID_MORE = wxNewId();

//Other constants
const int LINK_CURRENT = -1;
const int LINK_LIMIT_LEN = -2;

BEGIN_EVENT_TABLE(FFQJobEditAdv,wxDialog)
	//(*EventTable(FFQJobEditAdv)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQJobEditAdv::FFQJobEditAdv(wxWindow* parent)
{
	//(*Initialize(FFQJobEditAdv)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxStaticBoxSizer* SBS1;
	wxStaticBoxSizer* SBS2;
	wxStaticBoxSizer* SBS3;
	wxStaticBoxSizer* SBS4;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(5, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	SBS1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("SBS1"));
	FlexGridSizer7 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer7->AddGrowableCol(0);
	Inputs = new wxNotebook(this, ID_INPUTS, wxDefaultPosition, wxDefaultSize, 0, _T("ID_INPUTS"));
	AddInputFile(NULL);
	FlexGridSizer7->Add(Inputs, 1, wxALL|wxEXPAND, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	AddInput = new wxButton(this, ID_ADDINPUT, _T("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ADDINPUT"));
	AddInput->SetLabel(FFQS(SID_JOBEDIT_ADV_ADD_INPUT));
	BoxSizer2->Add(AddInput, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	DelInput = new wxButton(this, ID_DELINPUT, _T("Del"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DELINPUT"));
	DelInput->SetLabel(FFQS(SID_JOBEDIT_ADV_DEL_INPUT));
	BoxSizer2->Add(DelInput, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	AddSecondary = new wxButton(this, ID_ADDSECONDARY, _T("Sec"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ADDSECONDARY"));
	AddSecondary->SetLabel(FFQS(SID_JOBEDIT_ADV_ADD_SECONDARY));
	BoxSizer2->Add(AddSecondary, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer7->Add(BoxSizer2, 1, wxALL|wxEXPAND, 0);
	SBS1->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer1->Add(SBS1, 1, wxALL|wxEXPAND, 3);
	SBS2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Strm"));
	FlexGridSizer6 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer6->AddGrowableCol(0);
	FlexGridSizer6->AddGrowableRow(0);
	StreamList = new wxCheckListBox(this, ID_STREAMLIST, wxDefaultPosition, wxSize(-1,100), 0, 0, wxLB_HSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_STREAMLIST"));
	FlexGridSizer6->Add(StreamList, 0, wxEXPAND, 0);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StreamUp = new wxButton(this, ID_STREAMUP, _T("U"), wxDefaultPosition, wxSize(-1,-1), 0, wxDefaultValidator, _T("ID_STREAMUP"));
	StreamUp->SetLabel(FFQS(SID_COMMON_MOVE_UP));
	BoxSizer1->Add(StreamUp, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 3);
	StreamDown = new wxButton(this, ID_STREAMDOWN, _T("D"), wxDefaultPosition, wxSize(-1,-1), 0, wxDefaultValidator, _T("ID_STREAMDOWN"));
	StreamDown->SetLabel(FFQS(SID_COMMON_MOVE_DOWN));
	BoxSizer1->Add(StreamDown, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 3);
	StreamRefresh = new wxButton(this, ID_STREAMREFRESH, _T("R"), wxDefaultPosition, wxSize(-1,-1), 0, wxDefaultValidator, _T("ID_STREAMREFRESH"));
	StreamRefresh->SetLabel(FFQS(SID_COMMON_REFRESH));
	BoxSizer1->Add(StreamRefresh, 0, wxLEFT|wxRIGHT|wxEXPAND, 3);
	FlexGridSizer6->Add(BoxSizer1, 1, wxALL|wxEXPAND, 0);
	ST5 = new wxStaticText(this, wxID_ANY, _T("Inf"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST5->Disable();
	ST5->SetLabel(FFQS(SID_JOBEDIT_DBLCLK_FOR_INFO));
	SBS2->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_SELECT_STREAMS));
	FlexGridSizer6->Add(ST5, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer6->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	SBS2->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer1->Add(SBS2, 1, wxALL|wxEXPAND, 3);
	SBS3 = new wxStaticBoxSizer(wxVERTICAL, this, _T("Out"));
	OutputSizer = new wxFlexGridSizer(2, 2, 0, 0);
	OutputSizer->AddGrowableCol(0);
	Output = new wxTextCtrl(this, ID_OUTPUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OUTPUT"));
	OutputSizer->Add(Output, 0, wxALL|wxEXPAND, 3);
	BrowseOutput = new wxButton(this, ID_BROWSEOUTPUT, _T("  ...  "), wxDefaultPosition, wxSize(-1,-1), 0, wxDefaultValidator, _T("ID_BROWSEOUTPUT"));
	OutputSizer->Add(BrowseOutput, 0, wxALL|wxEXPAND, 3);
	OutputLength = new wxGenericHyperlinkCtrl(this, ID_OUTPUTLENGTH, _T("OutL"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE, _T("ID_OUTPUTLENGTH"));
	OutputSizer->Add(OutputLength, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OutputSizer->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	SBS3->Add(OutputSizer, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer1->Add(SBS3, 1, wxALL|wxEXPAND, 3);
	SBS4 = new wxStaticBoxSizer(wxVERTICAL, this, _T("Enc"));
	FlexGridSizer5 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer5->AddGrowableCol(1);
	ST3 = new wxStaticText(this, ID_STATICTEXT3, _T("Cmd"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	ST3->SetLabel(FFQS(SID_JOBEDIT_COMMAND_LINE));
	SBS3->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_OUTPUT_FILE));
	FlexGridSizer5->Add(ST3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	CmdLine = new wxComboBox(this, ID_CMDLINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_DROPDOWN, wxDefaultValidator, _T("ID_CMDLINE"));
	FlexGridSizer5->Add(CmdLine, 1, wxALL|wxEXPAND, 3);
	ST4 = new wxStaticText(this, ID_STATICTEXT4, _T("Pst"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	ST4->SetLabel(FFQS(SID_JOBEDIT_PRESET));
	SBS4->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_OUTPUT_ENCODING));
	FlexGridSizer5->Add(ST4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Preset = new FFQPresetPanel(this);
	FlexGridSizer5->Add(Preset, 1, wxALL|wxEXPAND, 3);
	SBS4->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer1->Add(SBS4, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer2 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	SaveLog = new wxCheckBox(this, ID_SAVELOG, _T("Log"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SAVELOG"));
	SaveLog->SetValue(false);
	SaveLog->SetLabel(FFQS(SID_COMMON_SAVE_LOG));
	FlexGridSizer2->Add(SaveLog, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SaveJob = new wxButton(this, ID_SAVEJOB, _T("S"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SAVEJOB"));
	SaveJob->SetDefault();
	SaveJob->SetLabel(FFQS(SID_COMMON_SAVE_JOB));
	FlexGridSizer2->Add(SaveJob, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	CancelDlg = new wxButton(this, ID_CANCELDLG, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELDLG"));
	CancelDlg->SetLabel(FFQS(SID_COMMON_CANCEL));
	FlexGridSizer2->Add(CancelDlg, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	OpenFile = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	OpenFile->SetMessage(FFQS(SID_COMMON_SELECT_INPUT_FILE));
	SaveFile = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	SaveFile->SetMessage(FFQS(SID_COMMON_SELECT_OUTPUT_FILE));
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_ADDINPUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_DELINPUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_ADDSECONDARY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_STREAMLIST,wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,(wxObjectEventFunction)&FFQJobEditAdv::OnStreamListChange);
	Connect(ID_STREAMLIST,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&FFQJobEditAdv::OnStreamListChange);
	Connect(ID_STREAMLIST,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_STREAMUP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_STREAMDOWN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_STREAMREFRESH,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_OUTPUT,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&FFQJobEditAdv::OnOutputTextEnter);
	Connect(ID_OUTPUT,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&FFQJobEditAdv::OnOutputTextEnter);
	Connect(ID_BROWSEOUTPUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_OUTPUTLENGTH,wxEVT_COMMAND_HYPERLINK,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_SAVEJOB,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	Connect(ID_CANCELDLG,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQJobEditAdv::OnAction);
	//*)


	SetTitle(FFQS(SID_JOBEDIT_TITLE));
    SBS1->GetStaticBox()->SetLabel(FFQS(SID_JOBEDIT_INPUT_FILES));

    //Initialize command line combo
    CmdLine->Append(CMD_DEFAULT);
    wxString cmd_list = FFQCFG()->saved_commands;
    if (cmd_list.Len() > 0)
    {
        DecompressFromBase64(cmd_list);
        wxString cl_version = GetToken(cmd_list, "\n", true);
        while (cmd_list.Len() > 0) CmdLine->Append(GetToken(cmd_list, "\n", true));
    }

    m_TimeEdit = NULL;
    m_ViewText = NULL;
    m_CutEdit = NULL;
    m_PopupCtrls = new FFQJobEditAdvPopup(this);
    m_PopupValidate = false;
    FFQCFG()->SetCtrlColors(OutputLength);
    //m_Process = new FFQProcess();

	FFQCFG()->SetBrowseRootFor(OpenFile);
	FFQCFG()->SetBrowseRootFor(SaveFile);

	//Connect(wxID_ANY, wxEVT_IDLE, (wxObjectEventFunction)&FFQJobEditAdv::OnIdle);
	Bind(wxEVT_IDLE, &FFQJobEditAdv::OnIdle, this);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &FFQJobEditAdv::OnMenuSelected, this);
	StreamList->Bind(wxEVT_RIGHT_DOWN, &FFQJobEditAdv::OnStreamListRightClick, this);
}

//---------------------------------------------------------------------------------------

FFQJobEditAdv::~FFQJobEditAdv()
{
	//(*Destroy(FFQJobEditAdv)
	//*)
	if (m_TimeEdit) delete m_TimeEdit;
	if (m_ViewText) delete m_ViewText;
	if (m_CutEdit) delete m_CutEdit;
	delete m_PopupCtrls;
	//delete m_Process;
}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdv::Execute(LPFFQ_JOB job)
{

    //Set job being edited
    m_Job = job;
    m_StreamMap = job->stream_map;
    m_FindSecondary = (job->inputs.Count() == 0);
    m_AutoPreset = m_FindSecondary;
    m_CanCut = FFQCFG()->AreFiltersAvailable(ADV_CUTS_REQUIRED_FILTERS);


    //Freeze during updates
    Inputs->Freeze();

    //Clear pages
    RemoveInputFile(-1);

    //Set input files
    FFQ_INPUT_FILE inf;
    for (unsigned int i = 0; i < job->inputs.Count(); i++)
    {
        inf = FFQ_INPUT_FILE(job->inputs[i]);
        AddInputFile(&inf, (i == 0));
    }

    //Ensure at least one page
    if (Inputs->GetPageCount() == 0) AddInputFile(NULL);

    //Thaw and focus
    Inputs->Thaw();
    GetCtrlData(0)->input->SetFocus();

	//Output file and length
	m_OutLen = job->out_len;
	Output->ChangeValue(job->out);
	m_AutoOutputName = job->out.Len() == 0;

	//Command line and preset
	CmdLine->ChangeValue(job->cmd_line == "" ? CMD_DEFAULT : job->cmd_line);
	Preset->SetFixedFirstItem(FFQS(SID_JOBEDIT_NO_PRESET));
	Preset->FillAndSelect(job->inputs.Count() == 0 ? "" : job->preset_id.ToString());
	m_SelectedPresetID = job->preset_id;

	//Save log?
	SaveLog->SetValue(job->save_log);

    //Prepare
    UpdateLink(LINK_LIMIT_LEN);
    UpdateControls();
	CenterOnParent();

    //Show
    bool ok = (ShowModal() == wxID_OK);

    if (ok)
    {

        //Save editor to job


        //Input files
        job->inputs.Clear();
        for (unsigned int i = 0; i < m_CtrlData.Count(); i++)
        {

            //Get data
            LPINPUT_CTRLS ctrls = GetCtrlData(i);

            //Reset
            inf.Reset();

            //Set values
            inf.path = ctrls->input->GetValue();
            inf.start = ctrls->start_val;
            inf.cuts = ctrls->cut_cfg;

            inf.itsoffset = Str2Long(ctrls->itsoffset, 0);
            inf.loop = Str2Long(ctrls->loop_streams, 0);
            inf.framerate = ctrls->framerate;
            inf.fflags_discardcorrupt = ctrls->discard_corrupt;
            inf.fflags_genpts = ctrls->genpts;
            inf.fflags_igndts = ctrls->igndts;
            inf.fflags_ignidx = ctrls->ignidx;

            /*inf.itsoffset = Str2Long(ctrls->itsoffset->GetValue(), 0);
            inf.framerate = StrTrim(ctrls->framerate->GetValue());
            inf.fflags_discardcorrupt = ctrls->discard_corrupt->GetValue();
            inf.fflags_genpts = ctrls->genpts->GetValue();
            inf.fflags_igndts = ctrls->igndts->GetValue();
            inf.fflags_ignidx = ctrls->ignidx->GetValue();*/

            ctrls->probe.GetVideoDimension(inf.width, inf.height);
            ctrls->probe.GetDuration(inf.duration);

            //Store to job
            job->inputs.Add(inf.ToString());

        }

        job->stream_map = GetStreamMapping();
        job->out = StrTrim(Output->GetValue());
        job->cmd_line = StrTrim(CmdLine->GetValue());
        job->out_len = m_OutLen;
        job->status = qsDORMANT; //Reset previous status to dormant

        //Store custom command and find out if config must be saved
        bool force_save_cfg = StoreCommand(&job->cmd_line);

        LPFFQ_PRESET pst = Preset->GetSelectedPreset();
        if (pst == NULL) job->preset_id.Reset();// memset(&job->preset_id, 0, sizeof(job->preset_id));
        else
        {
            job->preset_id = pst->preset_id;

            //Update preset finger print
            wxString pstFp = GetPresetFingerPrint();
            force_save_cfg = force_save_cfg || (pst->finger_print != pstFp);
            if (force_save_cfg) pst->finger_print = pstFp;
        }

        job->save_log = SaveLog->GetValue();
        force_save_cfg = force_save_cfg || FFQCFG()->SetSaveLog(job->save_log, false);

        FFQCFG()->SetPreferredAndSave(
            Output->GetValue().AfterLast('.'),
            Output->GetValue().BeforeLast(wxFileName::GetPathSeparator()),
            force_save_cfg
        );

    }

	//Clear the stream list and the inputs
	ClearStreamList();
    RemoveInputFile(-1);

    //Return result
    return ok;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::ShowPopupCtrls(bool show, bool cancel)
{

    //Show or hide the popup controls
    LPINPUT_CTRLS ctrls = GetCtrlData();

    //Position the popup
    wxPoint p = ctrls->more->GetScreenPosition();
    wxSize s = ctrls->more->GetSize();
    p.x += s.GetWidth();
    p.y += s.GetHeight();
    s = m_PopupCtrls->GetSize();
    p.x -= s.GetWidth();
    m_PopupCtrls->SetPosition(p);

    //Execute the popup
    if (m_PopupCtrls->Execute(ctrls)) UpdateToolTip(ctrls);

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::AddInputFile(LPFFQ_INPUT_FILE in_file, bool select)
{

    //Create a structure to hold the controls
    LPINPUT_CTRLS ctrls = new INPUT_CTRLS();

    //Update the values associated with the tab
    if (in_file == NULL)
    {

        //New input file
        ctrls->start_val = 0;
        ctrls->cut_cfg.Reset();
        ctrls->itsoffset.Clear();
        ctrls->loop_streams.Clear();
        ctrls->framerate.Clear();
        ctrls->discard_corrupt = false;
        ctrls->genpts = false;
        ctrls->igndts = false;
        ctrls->ignidx = false;

    }
    else
    {

        //Load values
        ctrls->start_val = in_file->start;
        ctrls->cut_cfg = in_file->cuts;
        ctrls->itsoffset = (in_file->itsoffset == 0) ? "" : wxString::Format("%i", in_file->itsoffset);
        ctrls->loop_streams = (in_file->loop == 0) ? "" : wxString::Format("%i", in_file->loop);
        ctrls->framerate = in_file->framerate;
        ctrls->discard_corrupt = in_file->fflags_discardcorrupt;
        ctrls->genpts = in_file->fflags_genpts;
        ctrls->igndts = in_file->fflags_igndts;
        ctrls->ignidx = in_file->fflags_ignidx;

    }

    //Create the panel to hold the controls
	ctrls->panel = new wxPanel(Inputs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	//Outer sizer
	ctrls->sizer = new wxBoxSizer(wxVERTICAL);

    //File, browse and play
    wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 3, 0, 0);
	fgs->AddGrowableCol(0);
	//fgs->AddGrowableCol(1);
	fgs->AddGrowableRow(0);
	ctrls->input = new wxTextCtrl(ctrls->panel, wxID_ANY, (in_file == NULL) ? "" : in_file->path);
	ctrls->input->SetMinSize(wxSize(500,-1));
	fgs->Add(ctrls->input, 1, wxALL|wxEXPAND, 3);

	ctrls->browse = new wxButton(ctrls->panel, ID_BROWSE, _T("..."), wxDefaultPosition, wxSize(-1/*50*/,-1));
	ctrls->browse->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FFQJobEditAdv::OnAction, this);
	fgs->Add(ctrls->browse, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);

	wxBitmap bmp;
	if (FFQCFG()->dark_theme) PtrToBitmap((void*)&ICON_PLAY_LIGHT, ICON_PLAY_LIGHT_SIZE, bmp, wxBITMAP_TYPE_PNG, wxSize(16, 16));
	else PtrToBitmap((void*)&ICON_PLAY_DARK, ICON_PLAY_DARK_SIZE, bmp, wxBITMAP_TYPE_PNG, wxSize(16, 16));
	ctrls->play = new wxBitmapButton(ctrls->panel, ID_PLAY, bmp, wxDefaultPosition, wxSize(-1, ctrls->browse->GetSize().GetHeight()));
	ctrls->play->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FFQJobEditAdv::OnAction, this);
	fgs->Add(ctrls->play, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);

	ctrls->sizer->Add(fgs, 1, wxALL|wxEXPAND, 0);

    fgs = new wxFlexGridSizer(1, 3, 0, 0);
	fgs->AddGrowableCol(0);
	fgs->AddGrowableRow(0);
	ctrls->start = new wxGenericHyperlinkCtrl(ctrls->panel, ID_START, "W", wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	ctrls->start->Bind(wxEVT_COMMAND_HYPERLINK, &FFQJobEditAdv::OnAction, this);
	FFQCFG()->SetCtrlColors(ctrls->start);
	fgs->Add(ctrls->start, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ctrls->cuts = new wxGenericHyperlinkCtrl(ctrls->panel, ID_CUTS, FFQS(SID_JOBEDIT_ADV_CUTS), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	ctrls->cuts->Bind(wxEVT_COMMAND_HYPERLINK, &FFQJobEditAdv::OnAction, this);
	FFQCFG()->SetCtrlColors(ctrls->cuts);
	fgs->Add(ctrls->cuts, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	ctrls->more = new wxGenericHyperlinkCtrl(ctrls->panel, ID_MORE, FFQS(SID_JOBEDIT_ADV_MORE), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	ctrls->more->Bind(wxEVT_COMMAND_HYPERLINK, &FFQJobEditAdv::OnAction, this);
	FFQCFG()->SetCtrlColors(ctrls->more);
	fgs->Add(ctrls->more, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

	ctrls->sizer->Add(fgs, 1, wxALL|wxEXPAND, 0);


	//Sizer for input file and start time
	/*ctrls->sizer = new wxFlexGridSizer(2, 2, 0, 0);
	ctrls->sizer->AddGrowableCol(0);
	//ctrls->sizer->AddGrowableRow(0);
	ctrls->input = new wxTextCtrl(ctrls->panel, wxID_ANY, (in_file == NULL) ? "" : in_file->path);
	ctrls->input->SetMinSize(wxSize(500,-1));
	ctrls->sizer->Add(ctrls->input, 1, wxALL|wxEXPAND, 3);
	ctrls->browse = new wxButton(ctrls->panel, ID_BROWSE, _T("..."), wxDefaultPosition, wxSize(50,-1));
	ctrls->sizer->Add(ctrls->browse, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ctrls->browse->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FFQJobEditAdv::OnAction, this);

	wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 2, 0, 0);
	fgs->AddGrowableCol(0);
	fgs->AddGrowableRow(0);
	ctrls->sizer->Add(fgs, 1, wxALL|wxEXPAND, 0);

	ctrls->start = new wxHyperlinkCtrl(ctrls->panel, ID_START, "W", wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	ctrls->start->Bind(wxEVT_COMMAND_HYPERLINK, &FFQJobEditAdv::OnAction, this);
	fgs->Add(ctrls->start, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ctrls->cuts = new wxHyperlinkCtrl(ctrls->panel, ID_CUTS, FFQS(SID_JOBEDIT_ADV_CUTS), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	ctrls->cuts->Bind(wxEVT_COMMAND_HYPERLINK, &FFQJobEditAdv::OnAction, this);
	fgs->Add(ctrls->cuts, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

	ctrls->more = new wxHyperlinkCtrl(ctrls->panel, ID_MORE, FFQS(SID_JOBEDIT_ADV_MORE), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	ctrls->sizer->Add(ctrls->more, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	ctrls->more->Bind(wxEVT_COMMAND_HYPERLINK, &FFQJobEditAdv::OnAction, this);*/

	ctrls->panel->SetSizer(ctrls->sizer);
	ctrls->sizer->Fit(ctrls->panel);
	ctrls->sizer->SetSizeHints(ctrls->panel);

    //Create a name for the tab
    wxString pn = wxString::Format("#%u", SIZEFMT(Inputs->GetPageCount()) + 1);

	//Add the page and control set
	m_CtrlData.Add(ctrls);
	Inputs->AddPage(ctrls->panel, pn, select);

	//Update the start time link
	UpdateLink(Inputs->GetPageCount() - 1);
	UpdateToolTip(ctrls);

}

//---------------------------------------------------------------------------------------

int FFQJobEditAdv::AddSecondaryFiles(wxString &for_file)
{

    //Searches for secondary input files and adds
    //them if they are not already added

    wxArrayString files;
    int res = 0;

    if (FFQCFG()->FindSecondaryInputFiles(for_file, files, 0) > 0)
    {

        //Files found, add them
        Inputs->Freeze();

        for (unsigned int i = 0; i < files.Count(); i++) if (FindInputByPath(files[i]) < 0)
        {

            //One file found, add it
            FFQ_INPUT_FILE in;
            in.path = files[i];
            AddInputFile(&in, false);
            res++;

        }

        Inputs->Thaw();

    }

    //Return how many files where added
    return res;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::ClearStreamList()
{
	StreamList->Freeze();
	for (unsigned int i = 0; i < StreamList->GetCount(); i++)
    {
        delete (LPSTREAM_DATA)StreamList->GetClientData(i);
    }
    StreamList->Clear();
	StreamList->Thaw();
}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdv::EditTime(TIME_VALUE &time, bool subtract)
{

    //Launch editor for the "time" parameter.

    //Ensure editor
    if (m_TimeEdit == NULL) m_TimeEdit = new FFQTimeEdit(this);

    //Subtract necessary?
    SUBTRACT_TIME *st = NULL;
    int cnt = 0;

    if (subtract)
    {

        //Yup, count items required
        for (unsigned int i = 0; i < m_CtrlData.Count(); i++) if(!GetCtrlData(i)->start_val.IsUndefined()) cnt++;

        if (cnt > 0)
        {

            //Allocate the array
            st = new SUBTRACT_TIME[cnt];
            int idx = 0;

            //Set values to the array
            for (unsigned int i = 0; i < m_CtrlData.Count(); i++)
            {

                LPINPUT_CTRLS ctrls = GetCtrlData(i);

                if(!ctrls->start_val.IsUndefined())
                {

                    st[idx].name = Inputs->GetPageText(i);
                    st[idx].time = ctrls->start_val;
                    idx++;

                }

            }

        }

    }

    //Return success of editor
    return m_TimeEdit->Execute(time, st, cnt);

}

//---------------------------------------------------------------------------------------

int FFQJobEditAdv::FindInputByPath(wxString path)
{

    //Find the index of the input with the file name "path"
    for (unsigned int i = 0; i < m_CtrlData.Count(); i++)
        if (CompareFileNames(GetCtrlData(i)->input->GetValue(), path)) return i;

    //Not found
    return -1;

}

//---------------------------------------------------------------------------------------

int FFQJobEditAdv::FindInvalidInput(bool select, bool frame_rate)
{

    //Search for an invalid input file
    for (unsigned int i = 0; i < m_CtrlData.Count(); i++)
    {

        LPINPUT_CTRLS ctrls = (LPINPUT_CTRLS)m_CtrlData[i];

        if (!ctrls->valid)
        {

            //Invalid file found

            if (select)
            {

                //Select and prompt
                Inputs->ChangeSelection(i);
                ctrls->input->SetFocus();
                ctrls->input->SelectAll();
                ShowError(this, FFQS(SID_INVALID_INPUT_NOT_ALLOWED));

            }

            return i;

        }

    }

    return -1;

}

//---------------------------------------------------------------------------------------

LPINPUT_CTRLS FFQJobEditAdv::GetCtrlData(int index)
{

    //Return the control data for the current page
    if (index < 0) index = Inputs->GetSelection();
    return ((index < 0) || (index >= (int)m_CtrlData.Count())) ? NULL : (LPINPUT_CTRLS)m_CtrlData[index];

}

//---------------------------------------------------------------------------------------

wxString FFQJobEditAdv::GetPresetFingerPrint(int file_index)
{

    //This will return a "fingerprint" based on the probe
    //data for the input file. This is used to auto-select
    //a preset when creating new jobs

    LPINPUT_CTRLS ctrls = GetCtrlData(file_index);
    if ((ctrls != NULL) && ctrls->valid)
    {

        //Get list of streams
        wxArrayString inf;
        for (unsigned int i = 0; i < ctrls->probe.GetStreamCount(); i++)
        {
            LPFFPROBE_STREAM_INFO si = ctrls->probe.GetStreamInfo(i);
            inf.Add(wxString::Format("%s|%s|%s|%d|%d", si->codec_type, si->codec_name, si->pixel_fmt, si->width, si->height));
        }

        //Sort the list to avoid fingerprint to defer due to alternate stream arrangement
        //and combine the list to a single string
        inf.Sort();
        wxString fp = "";
        for (unsigned int i = 0; i < inf.GetCount(); i++) fp += inf[i];

        //Hash
        wxScopedCharBuffer cb = fp.ToUTF8();
        SHA1_Context sha1;
        SHA1_Buffer(&sha1, cb.data(), cb.length());

        //Return base64
        return wxBase64Encode(&sha1.hash, sizeof(sha1.hash));

    }

    return wxEmptyString;

}

//---------------------------------------------------------------------------------------

wxString FFQJobEditAdv::GetStreamMapping()
{

    //Return the stream mapping from the StreamList
    wxString res = "", s;
    STREAM_MAPPING smap;

    for (unsigned int i = 0; i < StreamList->GetCount(); i++)
    {

        LPSTREAM_DATA sd = (LPSTREAM_DATA)StreamList->GetClientData(i);
        s = StreamList->GetString(i);
        smap.Parse(s);
        smap.checked = StreamList->IsChecked(i);
        s = "";
        for (unsigned int pi = 0; pi < sd->presets.Count(); pi++) s += sd->presets[pi] + SPACE;
        smap.preset_list = s.BeforeLast(SPACE);
        smap.codec_id = sd->si->codec_name;
        if (res.Len() > 0) res += STREAM_MAPPING_SEPERATOR;
        res += smap.ToString();

    }

    return res;

}

//---------------------------------------------------------------------------------------

int FFQJobEditAdv::RefreshStreamList(bool force_all)
{

    //Probe files that needs it and add new streams to the list

    //Useful little bits and pieces
    int probe_count = 0;
    LPINPUT_CTRLS ctrls;

    for (unsigned int i = 0; i < m_CtrlData.Count(); i++)
    {

        //Get data
        ctrls = (LPINPUT_CTRLS)m_CtrlData[i];

        //Has the item been changed?
        wxString in = ctrls->input->GetValue();
        bool changed = (in != ctrls->validated_path);

        if (changed || force_all)
        {

            //Does the file exists?
            bool exists = wxFileExists(in);

            if (exists || wxFileExists(ctrls->validated_path))
            {

                //Increment counter
                probe_count++;

                //Set probed path to prevent re-probe
                ctrls->validated_path = in;

                if (exists)
                {

                    //If current file exists, we must probe it
                    ctrls->valid = ctrls->probe.RunFFProbe(in, NULL, true);

                    //If probe succeeded, we must figure out if the content can be cut
                    ctrls->can_cut = ctrls->valid && (ctrls->probe.FindStreamByCodecType(CODEC_TYPE_AUDIO) || ctrls->probe.FindStreamByCodecType(CODEC_TYPE_VIDEO));

                }

                else
                {

                    //If previous file exists, we must clear previous probing and invalidate item
                    ctrls->probe.SetProbeOutput("");
                    ctrls->valid = false;
                    ctrls->can_cut = false;

                }

            }

        }

    }

    //If any changes has been made, we must update the list
    if (probe_count > 0)
    {

        STREAM_MAPPING smap;
        ClearStreamList();
        StreamList->Freeze();
        //StreamList->Clear();

        for (unsigned int i = 0; i < m_CtrlData.Count(); i++)
        {

            LPINPUT_CTRLS ctrls = (LPINPUT_CTRLS)m_CtrlData[i];

            //Set id of current file (starting from 1)
            smap.file_id = (i + 1);

            for (unsigned int ii = 0; ii < ctrls->probe.GetStreamCount(); ii++)
            {

                //Construct the stream mapping based on the stream info
                LPSTREAM_DATA sd = new STREAM_DATA();
                sd->si = ctrls->probe.GetStreamInfo(ii);
                //wxString s = smap.preset_id;
                //while (s.Len() > 0) sd->presets.Add(GetToken(s, '*'));
                smap.stream_id = sd->si->index;
                smap.codec_type = sd->si->codec_type;

                //Add and check the mapping
                int idx = StreamList->Append(smap.ToString(sd->si->codec_long_name), sd);
                StreamList->Check(idx, true);

            }

        }

        StreamList->Thaw();

    }

    //Return the number of updates
    return probe_count;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::RemoveInputFile(int index)
{

    //Remove the input file(s) including data and streams
    Inputs->Freeze();

    if (index < 0)
    {

        //All
        Inputs->DeleteAllPages();
        for (unsigned int i = 0; i < m_CtrlData.Count(); i++) delete (LPINPUT_CTRLS)m_CtrlData[i];
        m_CtrlData.Clear();
        ClearStreamList();

    }

    else
    {

        //One
        Inputs->DeletePage(index);
        delete (LPINPUT_CTRLS)m_CtrlData[index];
        m_CtrlData.RemoveAt(index);

        //Update page names and start links
        for (unsigned int i = index; i < Inputs->GetPageCount(); i++)
        {
            Inputs->SetPageText(i, wxString::Format("#%u", SIZEFMT(i+1)));
            UpdateLink(i);
        }

        //Update stream list
        StreamList->Freeze();
        index++; //File index in stream mapping starts from 1, not 0
        unsigned int i = 0;
        while (i < StreamList->GetCount())
        {

            //Get stream mapping
            wxString s = StreamList->GetString(i);
            STREAM_MAPPING smap(s);
            LPSTREAM_DATA sd = (LPSTREAM_DATA)StreamList->GetClientData(i);

            if (smap.file_id == index)
            {
                delete sd;
                StreamList->Delete(i); //Delete
            }

            else
            {

                if (smap.file_id > index)
                {

                    //Decrement file index for the stream mapping
                    smap.file_id--;
                    StreamList->SetString(i, smap.ToString(sd->si->codec_long_name));

                }

                i++;

            }

        }
        StreamList->Thaw();

    }

    Inputs->Thaw();

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::SetStreamMapping(wxString mapping)
{

    //This will restore the stream order and selections based on the
    //stream mapping supplied.

    //If the stream mapping is empty all streams must be selected
    if (mapping.Len() == 0) for (unsigned int i = 0; i < StreamList->GetCount(); i++) StreamList->Check(i, true);


    //Else the mapping must examined and applied per stream
    else
    {

        STREAM_MAPPING smap;
        wxString sw; //starts with
        unsigned int midx = 0; //mapping index

        do
        {

            //Parse one mapping and create the search string
            smap.Parse(mapping);
            sw = HASH;
            sw << smap.file_id << COLON << smap.stream_id << SPACE << smap.codec_type;

            //Locate the current mapping in the stream list
            for (unsigned int i = midx; i < StreamList->GetCount(); i++)
            {

                if (StreamList->GetString(i).StartsWith(sw))
                {

                    //Check the stream according to the mapping, sort it and we are done
                    LPSTREAM_DATA sd = (LPSTREAM_DATA)StreamList->GetClientData(i);
                    sd->presets.Clear();
                    wxString s = smap.preset_list;
                    while (s.Len() > 0) sd->presets.Add(GetToken(s, SPACE));
                    StreamList->Check(i, smap.checked);
                    ListBoxSwapItems(StreamList, i, midx, true);
                    break;

                }

            }

            //Increment mapping index
            midx++;

        } while (mapping.Len() > 0);

    }

}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdv::StoreCommand(wxString *cmd_line)
{

    //This will update the items in the CmdLine combo
    //to have the most recently used command in top
    //and then save the entire list of commands to
    //config and return true if any modifications
    //was performed, get command;

    wxString cmd = (cmd_line == NULL) ? CmdLine->GetValue() : *cmd_line;

    //Get index of the command
    int idx = CmdLine->FindString(cmd, false);

    //Item at index 0 must always be CMD_DEFAULT,
    //so we must ensure the most recent command is
    //at index 1
    if (idx > 1)
    {
        CmdLine->Delete(idx);
        idx = -1;
    }
    else return false; //No change

    if (idx < 0)
    {

        //Insert at pos 1
        CmdLine->Insert(cmd, 1);

        //Delete last item until only 25 custom commands
        //remains; ignore CMD_DEFAULT at pos 0
        while (CmdLine->GetCount() > 26) CmdLine->Delete(CmdLine->GetCount() - 1);

    }

    //Pack and base64 all custom items
    cmd = "1\n"; //Stored commands version
    for (unsigned int i = 1; i < CmdLine->GetCount(); i++) cmd += CmdLine->GetString(i) + LF;
    CompressAndBase64(cmd, 0);

    //Check for no modifications
    if (cmd == FFQCFG()->saved_commands) return false;

    //Modified, alright..
    FFQCFG()->saved_commands = cmd;
    return true;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::UpdateControls()
{

    //Enable controls in the editor
    LPINPUT_CTRLS ctrls = GetCtrlData();
    if (ctrls)
    {

        TIME_VALUE dur;
        if ((!ctrls->valid) || (!ctrls->probe.GetDuration(dur))) dur = 0;
        ctrls->cuts->Enable(m_CanCut && ctrls->can_cut && (dur.ToMilliseconds() > 0));
        ctrls->play->Enable(dur.ToMilliseconds() > 0);
        ctrls->start->Enable((ctrls->cut_cfg.cuts.Len() == 0) || (!ctrls->cut_cfg.quick));

    }

    DelInput->Enable(Inputs->GetPageCount() > 1);
    AddSecondary->Enable((ctrls != NULL) && ctrls->valid);


    //Enable buttons to move streams up and down
    int sel = StreamList->GetSelection();
    StreamUp->Enable((sel > -1) && (!StreamList->IsSelected(0)));
    StreamDown->Enable((sel > -1) && (!StreamList->IsSelected(StreamList->GetCount() - 1)));

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::UpdateLink(int index)
{

    //Update a link to the specified time value
    wxGenericHyperlinkCtrl *link;
    wxSizer *sizer;
    wxString l;

    if (index == LINK_LIMIT_LEN)
    {

        //Limit length of output
        link = OutputLength;
        sizer = OutputSizer;
        l = m_OutLen.IsUndefined() ? FFQS(SID_NO_LIMIT) : m_OutLen.ToString();
        l = FFQSF(SID_LIMIT_LENGTH, l);

    }
    else
    {

        //Set start time of input
        if (index == LINK_CURRENT) index = Inputs->GetSelection();
        LPINPUT_CTRLS ctrls = (LPINPUT_CTRLS)m_CtrlData[index];
        link = ctrls->start;
        sizer = ctrls->sizer;
        l = ctrls->start_val.IsUndefined() ? FFQS(SID_THE_BEGINNING) : ctrls->start_val.ToString();
        l = FFQSF(SID_START_POSITION, Inputs->GetPageText(index), l);

    }

    //Apply and resize
    link->SetLabelText(l);
    link->Refresh(); //2023 - Søg efter flere wxGenericHyperlinkCtrl
    sizer->Layout();

    //wxClientDC dc(link);
    //link->SetClientSize(dc.GetTextExtent(l));

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::UpdateToolTip(LPINPUT_CTRLS ctrls)
{

    //Set the informative tool tip of the more link
    if (ctrls == NULL) ctrls = GetCtrlData();
    wxString ff = "";
    if (ctrls->discard_corrupt) ff += "discardcorrupt,";
    if (ctrls->genpts) ff += "genpts,";
    if (ctrls->igndts) ff += "igndts,";
    if (ctrls->ignidx) ff += "ignidx,";
    if (ff.Len() > 0) ff.Remove(ff.Len() - 1);
    wxToolTip *tip = ctrls->more->GetToolTip();
    if (tip == NULL)
    {
        tip = new wxToolTip(_(""));
        ctrls->more->SetToolTip(tip);
    }
    tip->SetTip(FFQSF(SID_JOBEDIT_ADV_MORE_TOOLTIP, ctrls->itsoffset, ctrls->framerate, ctrls->loop_streams, ff));
    #ifdef __WINDOWS__
    tip->SetMaxWidth(1000);
    #endif // __WINDOWS__

}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdv::ValidateCuts()
{

    //This will try to detect if advances cuts is combined with copy codecs
    //or mapping of subtitles which is known to fail.

    bool ok = true;
    for (size_t i = 0; ok && (i < m_CtrlData.Count()); i++)
    {
        LPINPUT_CTRLS ctrls = GetCtrlData(i);
        ok = ctrls->cut_cfg.cuts.Len() == 0;
    }
    if (ok) return true; //No cuts present

    ok = true;
    LPFFQ_PRESET pst = Preset->GetSelectedPreset();
    wxString smap = GetStreamMapping();
    STREAM_MAPPING sm;

    while (ok && (smap.Len() > 0))
    {

        sm.Parse(smap);
        if (!sm.checked) continue; //Unused stream, go to next

        if (pst)
        {
            //Add the default preset to the list of presets for the stream
            if (sm.preset_list.Len() > 0) sm.preset_list += SPACE;
            sm.preset_list += pst->preset_id.ToString();
        }

        //Check if any of the used presets have "copy" as codec for the stream
        while (ok && (sm.preset_list.Len() > 0))
        {
            pst = FFQPresetMgr::Get()->GetPreset(GetToken(sm.preset_list, SPACE));
            if (pst)
            {
                ok = ((sm.codec_type == CODEC_TYPE_VIDEO) && (pst->video_codec != CODEC_COPY))
                      ||
                     ((sm.codec_type == CODEC_TYPE_AUDIO) && (pst->audio_codec != CODEC_COPY))
                      ||
                     ((sm.codec_type == CODEC_TYPE_SUBTITLE) && (pst->subtitles.codec != CODEC_COPY));
            }

        }

    }

    return ok;

}

//---------------------------------------------------------------------------------------

bool FFQJobEditAdv::ValidateJob()
{

    //Make sure the job is not running
    if (!m_Job->CheckNotActive()) return false;

    //Validate input files
    if (FindInvalidInput(true, true) >= 0) return false;

    //Make sure at least one stream has been selected
    int chk = 0;
    for (unsigned int i = 0; i < StreamList->GetCount(); i++) if (StreamList->IsChecked(i)) chk++;
    if (chk == 0) return ShowError(StreamList, FFQS(SID_SELECT_AT_LEAST_ONE_STREAM));

    //Make sure the output file is defined
    wxString out = StrTrim(Output->GetValue());
    if (out == "") return ShowError(Output, FFQS(SID_OUTPUT_FILE_REQUIRED));

    //Make sure output file is not equal to any input files
    if (FindInputByPath(out) >= 0) return ShowError(Output, FFQS(SID_OUTPUT_FILE_NOT_EQUAL_INPUT));

    //Check if the command line is roughly acceptable
    wxString cmd = CmdLine->GetValue().Lower();
    bool pst_ok = (Preset->GetSelectedPreset() == NULL) || (cmd.Find(CMD_PRESET) >= 0),
         inp_ok = (cmd.Find(CMD_INPUTS) >= 0), out_ok = (cmd.Find(CMD_OUTPUT) >= 0);
    if ((!inp_ok) || (!out_ok) || (!pst_ok))
    {

        //Nope, prompt the wronginess
        cmd = "";
        if (!inp_ok) cmd += CMD_INPUTS + SPACE;
        if (!pst_ok) cmd += CMD_PRESET + SPACE;
        if (!out_ok) cmd += CMD_OUTPUT + SPACE;
        cmd.Trim();
        return ShowError(CmdLine, FFQSF(SID_COMMAND_LINE_INVALID, cmd));

    }

    if (m_Job->inputs.Count() == 0)
    {

        //Check if a job is already defined for input #1 (new jobs only)
        LPINPUT_CTRLS ctrls = GetCtrlData(0);
        long idx = ((FFQMain*)GetParent())->FindItemForFile(ctrls->input->GetValue());
        if ((idx >= 0) && (!DoConfirm(this, FFQSF(SID_JOB_FOR_FILE_ALREADY_QUEUED, Inputs->GetPageText(0)))))
        {

            Inputs->SetSelection(0);
            ctrls->input->SetFocus();
            return false;

        }

    }

    //Check if cuts are valid and warn about it
    if (!ValidateCuts() && ( !DoConfirm(NULL, FFQS(SID_ADVANCED_CUTS_WARNING), wxICON_WARNING) )) return false;

    //Job is valid
    return true;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::OnIdle(wxIdleEvent &event)
{

    //For some reason the focus of the wxTextCtrl for input files looses
    //focus sometimes during idle work. Therefore we store the current focus
    //in order to be able to restore it later
    static bool idling = false;
    if (idling) return;
    idling = true;

    wxWindow *focus = wxWindow::FindFocus();

    //Refresh streams
    if (RefreshStreamList() > 0)
    {

        //Apply stored stream mapping
        SetStreamMapping(m_StreamMap);

        //If all inputs are valid we update the mapping
        if (FindInvalidInput(false) < 0) m_StreamMap = GetStreamMapping();

    }


    //Get the controls from the first input file and validate
    LPINPUT_CTRLS ctrls = GetCtrlData(0);

    if ((ctrls != NULL) && ctrls->valid)
    {

        if (m_CtrlData.Count() == 1)
        {

            //If only one valid file is available we must search for secondary files
            //and set the preferred preset

            //Find secondary files?
            if (m_FindSecondary)
            {

                m_FindSecondary = false; //Make sure this is only done once!
                AddSecondaryFiles(ctrls->validated_path);

            }

            //Detect preset?
            if (m_AutoPreset)
            {

                m_AutoPreset = false; //Make sure this is only done once!
                wxString pstFp = GetPresetFingerPrint();
                if (pstFp.Len() > 0) Preset->SelectPreset(FFQPresetMgr::Get()->GetPresetByFingerPrint(pstFp));

            }

        }

        //If the first input file is valid, generate the output name for it
        if (m_AutoOutputName)
        {

            wxString outfn = FFQCFG()->GetPreferredOutputName(ctrls->input->GetValue(), Preset->GetSelectedPreset());
            if (outfn != Output->GetValue()) Output->ChangeValue(outfn);

            //if (Output->GetValue().Len() == 0)
                //Output->SetValue(FFQCFG()->GetPreferredOutputName(ctrls->input->GetValue()));

        }

    }

    //Update output file format
    LPFFQ_PRESET pst = Preset->GetSelectedPreset();
    if (pst && (pst->preset_id != m_SelectedPresetID))
    {
        m_SelectedPresetID = pst->preset_id;
        wxString outfn = Output->GetValue();
        wxString ext = outfn.AfterLast(DOT);
        if (ext.Len() > 0)
        {
            wxString fext = pst->GetFormatExtension(ext);
            if (fext != ext) Output->SetValue(outfn.BeforeLast(DOT) + DOT + fext);
        }
    }

    //Update controls
    UpdateControls();

    if (focus != NULL)
    {

        //Restore focus if changed
        if (focus != wxWindow::FindFocus()) focus->SetFocus();

        //Check if popup should be hidden
        //m_PopupCtrls->CheckFocus(focus);

    }

    idling = false;

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::OnMenuSelected(wxCommandEvent &event)
{
    LPFFQ_PRESET pst = (LPFFQ_PRESET)Preset->Presets->GetClientData(event.GetId());
    wxString pid = pst->preset_id.ToString();
    LPSTREAM_DATA sd = (LPSTREAM_DATA)StreamList->GetClientData(StreamList->GetSelection());
    for (unsigned int i = 0; i < sd->presets.Count(); i++)
    {
        if (sd->presets[i] == pid)
        {
            sd->presets.RemoveAt(i);
            return;
        }
    }
    sd->presets.Add(pid);
}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::OnStreamListRightClick(wxMouseEvent &event)
{
    int sel = StreamList->GetSelection();
    if ((sel < 0) || (!StreamList->IsChecked(sel))) return;

    LPSTREAM_DATA sd = (LPSTREAM_DATA)StreamList->GetClientData(sel);
    bool aud = sd->si->codec_type == CODEC_TYPE_AUDIO,
         sub = sd->si->codec_type == CODEC_TYPE_SUBTITLE;

    if (aud || sub)
    {
        wxMenu popup;
        for (unsigned int i = 1; i < Preset->Presets->GetCount(); i++)
        {
            LPFFQ_PRESET pst = (LPFFQ_PRESET)Preset->Presets->GetClientData(i);
            if (aud || (pst->subtitles.codec != CODEC_SUBS_BURNIN))
            {
                wxMenuItem *mi = popup.AppendCheckItem((int)i, Preset->Presets->GetString(i));
                mi->Check(sd->presets.Index(pst->preset_id.ToString()) >= 0);
            }
        }
        StreamList->PopupMenu(&popup);
    }
}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::OnAction(wxCommandEvent& event)
{

    int evtId = event.GetId();

    if (evtId == ID_BROWSE)
    {

        //Select a file for current input
        LPINPUT_CTRLS ctrls = GetCtrlData();
        OpenFile->SetPath(ctrls->input->GetValue());
        if (OpenFile->ShowModal() != wxID_CANCEL)
        {

            ctrls->input->ChangeValue(OpenFile->GetPath());
            ctrls->input->SetFocus();

        }


    }

    else if (evtId == ID_PLAY)
    {

        //Play input file with system default player
        LPINPUT_CTRLS ctrls = GetCtrlData();
        wxLaunchDefaultApplication(ctrls->input->GetValue());

    }

    else if (evtId == ID_START)
    {

        //Edit stream offset for current file
        LPINPUT_CTRLS ctrls = GetCtrlData();
        if (EditTime(ctrls->start_val, false)) UpdateLink(LINK_CURRENT);

    }

    else if (evtId == ID_CUTS)
    {

        if (m_CutEdit == NULL) m_CutEdit = new FFQJobEditAdvCuts(this);
        LPINPUT_CTRLS ctrls = GetCtrlData();
        m_CutEdit->Execute(ctrls->cut_cfg, ctrls->input->GetValue(), &ctrls->probe);

    }

    else if (evtId == ID_MORE)
    {

        ShowPopupCtrls(!m_PopupCtrls->IsVisible());

    }

    else if (evtId == ID_ADDINPUT)
    {

        //Add new input file if all are valid
        if (FindInvalidInput(true) < 0) AddInputFile(NULL);

    }

    else if (evtId == ID_DELINPUT)
    {

        //Delete current input file
        Inputs->Freeze();
        RemoveInputFile(Inputs->GetSelection());
        Inputs->Thaw();

    }

    else if (evtId == ID_ADDSECONDARY)
    {

        //Add secondary files for the selected file
        AddSecondaryFiles(GetCtrlData()->validated_path);

    }

    else if (evtId == ID_STREAMLIST)
    {

        //Show details for a stream
        if (m_ViewText == NULL) m_ViewText = new FFQShowText(this);
        LPSTREAM_DATA sd = (LPSTREAM_DATA)StreamList->GetClientData(StreamList->GetSelection());
        m_ViewText->Execute(FFQS(SID_STREAM_INFORMATION), sd->si->items);

    }

    else if (evtId == ID_STREAMUP)
    {

        //Move selected streams up
        ListBoxMoveSelectedItems(StreamList, true, true);

    }

    else if (evtId == ID_STREAMDOWN)
    {

        //Move selected streams down
        ListBoxMoveSelectedItems(StreamList, false, true);

    }

    else if (evtId == ID_STREAMREFRESH)
    {

        //Refresh stream list
        //wxString smap = GetStreamMapping();
        if (RefreshStreamList(true) > 0) SetStreamMapping(m_StreamMap);//smap);

    }

    else if (evtId == ID_BROWSEOUTPUT)
    {

        //Select a file for Output
        SaveFile->SetPath(Output->GetValue());
        if (SaveFile->ShowModal() != wxID_CANCEL)
        {

            Output->SetValue(SaveFile->GetPath());
            Output->SetFocus();

        }

    }

    else if (evtId == ID_OUTPUTLENGTH)
    {

        //Edit length of output file
        if (EditTime(m_OutLen, true)) UpdateLink(LINK_LIMIT_LEN);

    }

    else if (evtId == ID_SAVEJOB)
    {

        //Validate job and close if OK
        if (ValidateJob()) EndModal(wxID_OK);

    }

    else if (evtId == ID_CANCELDLG)
    {

        //Cancel editor
        EndModal(wxID_CANCEL);
        return;

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::OnStreamListChange(wxCommandEvent& event)
{

    if (event.GetEventType() == wxEVT_CHECKLISTBOX)
    {

        //Check box has been toggled, update stream map if all are valid
        if (FindInvalidInput(false) < 0) m_StreamMap = GetStreamMapping();

    }

    //Controls must be updated due to selection change
    else UpdateControls();
}

//---------------------------------------------------------------------------------------

void FFQJobEditAdv::OnOutputTextEnter(wxCommandEvent& event)
{
    //Make sure that generation of automatic output names
    //is stopped when the user does manual modifications
    m_AutoOutputName = false;
}
