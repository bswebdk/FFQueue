/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQBatchMake.cpp                                                *
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

#include "FFQBatchMake.h"
#include "FFQPresetMgr.h"
#include "utils/FFQLang.h"
#include "FFQMain.h"
#include "utils/FFQConsole.h"
#include "utils/FFQProcess.h"
#include "utils/FFQConfig.h"
#include "utils/FFQBuildCmd.h"
#include "utils/FFQMisc.h"
#include "utils/FFQConst.h"

#include <wx/filename.h>
#include <wx/dir.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQBatchMake)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQBatchMake)
#include <wx/font.h>
//*)

//(*IdInit(FFQBatchMake)
const long FFQBatchMake::ID_JOBINFO = wxNewId();
const long FFQBatchMake::ID_ST1 = wxNewId();
const long FFQBatchMake::ID_INCLUDEVIDEO = wxNewId();
const long FFQBatchMake::ID_INCLUDEAUDIO = wxNewId();
const long FFQBatchMake::ID_INCLUDESUBS = wxNewId();
const long FFQBatchMake::ID_NOFAILVIDEO = wxNewId();
const long FFQBatchMake::ID_NOFAILAUDIO = wxNewId();
const long FFQBatchMake::ID_NOFAILSUBS = wxNewId();
const long FFQBatchMake::ID_STATICLINE1 = wxNewId();
const long FFQBatchMake::ID_ST2 = wxNewId();
const long FFQBatchMake::ID_PREFAUDIO = wxNewId();
const long FFQBatchMake::ID_AUDINVERSE = wxNewId();
const long FFQBatchMake::ID_STATICLINE2 = wxNewId();
const long FFQBatchMake::ID_ST6 = wxNewId();
const long FFQBatchMake::ID_PREFSUBS = wxNewId();
const long FFQBatchMake::ID_SUBSINVERSE = wxNewId();
const long FFQBatchMake::ID_STATICLINE3 = wxNewId();
const long FFQBatchMake::ID_ST3 = wxNewId();
const long FFQBatchMake::ID_OUTPUTFORMAT = wxNewId();
const long FFQBatchMake::ID_ST4 = wxNewId();
const long FFQBatchMake::ID_OUTPUTPATH = wxNewId();
const long FFQBatchMake::ID_BROWSEBUTTON = wxNewId();
const long FFQBatchMake::ID_ST5 = wxNewId();
const long FFQBatchMake::ID_PRESET = wxNewId();
const long FFQBatchMake::ID_MAKELOGS = wxNewId();
const long FFQBatchMake::ID_DRYRUNBUTTON = wxNewId();
const long FFQBatchMake::ID_MAKEBUTTON = wxNewId();
const long FFQBatchMake::ID_CANCELBUTTON = wxNewId();
const long FFQBatchMake::ID_GAUGE = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQBatchMake,wxDialog)
	//(*EventTable(FFQBatchMake)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

const wxString PATH_TOKEN = "\"*?*\""; //Just something that is forbidden in paths

//---------------------------------------------------------------------------------------

FFQBatchMake::FFQBatchMake(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(FFQBatchMake)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* FlexGridSizer9;
	wxStaticBoxSizer* SBS1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	MainSizer = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	FlexGridSizer2->AddGrowableRow(0);
	JobInfo = new wxStaticText(this, ID_JOBINFO, _T("L"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_JOBINFO"));
	wxFont JobInfoFont(12,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	JobInfo->SetFont(JobInfoFont);
	FlexGridSizer2->Add(JobInfo, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MainSizer->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
	SBS1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("Props"));
	FlexGridSizer3 = new wxFlexGridSizer(14, 1, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	FlexGridSizer4 = new wxFlexGridSizer(2, 4, 0, 0);
	FlexGridSizer4->AddGrowableCol(1);
	FlexGridSizer4->AddGrowableCol(2);
	FlexGridSizer4->AddGrowableCol(3);
	ST1 = new wxStaticText(this, ID_ST1, _T("SelC"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST1"));
	ST1->SetLabel(FFQS(SID_BATCHMAKE_SEL_CONTENT));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_BATCHMAKE_DEFINE_PROPERTIES));
	FlexGridSizer4->Add(ST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	IncludeVideo = new wxCheckBox(this, ID_INCLUDEVIDEO, _T("V"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_INCLUDEVIDEO"));
	IncludeVideo->SetValue(false);
	IncludeVideo->SetLabel(FFQS(SID_COMMON_VIDEO));
	FlexGridSizer4->Add(IncludeVideo, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	IncludeAudio = new wxCheckBox(this, ID_INCLUDEAUDIO, _T("A"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_INCLUDEAUDIO"));
	IncludeAudio->SetValue(false);
	IncludeAudio->SetLabel(FFQS(SID_COMMON_AUDIO));
	FlexGridSizer4->Add(IncludeAudio, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	IncludeSubs = new wxCheckBox(this, ID_INCLUDESUBS, _T("S"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_INCLUDESUBS"));
	IncludeSubs->SetValue(false);
	IncludeSubs->SetLabel(FFQS(SID_COMMON_SUBTITLES));
	FlexGridSizer4->Add(IncludeSubs, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	ST7 = new wxStaticText(this, wxID_ANY, _T("NoFa"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST7->SetLabel(FFQS(SID_BATCHMAKE_NOFAIL_CONTENT));
	FlexGridSizer4->Add(ST7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	NoFailVideo = new wxCheckBox(this, ID_NOFAILVIDEO, _T("V"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_NOFAILVIDEO"));
	NoFailVideo->SetValue(false);
	NoFailVideo->SetLabel(FFQS(SID_COMMON_VIDEO));
	FlexGridSizer4->Add(NoFailVideo, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	NoFailAudio = new wxCheckBox(this, ID_NOFAILAUDIO, _T("A"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_NOFAILAUDIO"));
	NoFailAudio->SetValue(false);
	NoFailAudio->SetLabel(FFQS(SID_COMMON_AUDIO));
	FlexGridSizer4->Add(NoFailAudio, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	NoFailSubs = new wxCheckBox(this, ID_NOFAILSUBS, _T("S"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_NOFAILSUBS"));
	NoFailSubs->SetValue(false);
	NoFailSubs->SetLabel(FFQS(SID_COMMON_SUBTITLES));
	FlexGridSizer4->Add(NoFailSubs, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 3);
	SL1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("wxID_ANY"));
	FlexGridSizer3->Add(SL1, 1, wxTOP|wxBOTTOM|wxEXPAND, 3);
	FlexGridSizer9 = new wxFlexGridSizer(1, 3, 0, 10);
	FlexGridSizer9->AddGrowableCol(0);
	FlexGridSizer9->AddGrowableRow(0);
	ST8 = new wxStaticText(this, wxID_ANY, _T("IncX"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST8->SetLabel(FFQS(SID_BATCHMAKE_ALSO_INCLUDE));
	FlexGridSizer9->Add(ST8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	IncludeAttachment = new wxCheckBox(this, wxID_ANY, _T("T"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	IncludeAttachment->SetValue(false);
	IncludeAttachment->SetLabel(FFQS(SID_COMMON_ATTACHMENT));
	FlexGridSizer9->Add(IncludeAttachment, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	IncludeData = new wxCheckBox(this, wxID_ANY, _T("D"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	IncludeData->SetValue(false);
	IncludeData->SetLabel(FFQS(SID_COMMON_DATA));
	FlexGridSizer9->Add(IncludeData, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3->Add(FlexGridSizer9, 1, wxALL|wxEXPAND, 3);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer3->Add(StaticLine1, 1, wxTOP|wxBOTTOM|wxEXPAND, 3);
	FlexGridSizer1 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer1->AddGrowableCol(1);
	ST2 = new wxStaticText(this, ID_ST2, _T("PrefA"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST2"));
	ST2->SetLabel(FFQS(SID_BATCHMAKE_PREF_AUDIO_CODEC));
	FlexGridSizer1->Add(ST2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	PrefAudio = new wxComboBox(this, ID_PREFAUDIO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_PREFAUDIO"));
	PrefAudio->SetSelection( PrefAudio->Append(_T("aac")) );
	PrefAudio->Append(_T("eac3"));
	PrefAudio->Append(_T("mp2"));
	PrefAudio->Append(_T("mp3"));
	FlexGridSizer1->Add(PrefAudio, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 3);
	AudInverse = new wxCheckBox(this, ID_AUDINVERSE, _T("AI"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUDINVERSE"));
	AudInverse->SetValue(false);
	AudInverse->SetLabel(FFQS(SID_BATCHMAKE_MATCH_INVERSE));
	FlexGridSizer3->Add(AudInverse, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	FlexGridSizer3->Add(StaticLine2, 1, wxTOP|wxBOTTOM|wxEXPAND, 3);
	FlexGridSizer8 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer8->AddGrowableCol(1);
	ST6 = new wxStaticText(this, ID_ST6, _T("PrefS"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST6"));
	ST6->SetLabel(FFQS(SID_BATCHMAKE_PREF_SUBS_CODEC));
	FlexGridSizer8->Add(ST6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	PrefSubs = new wxComboBox(this, ID_PREFSUBS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_PREFSUBS"));
	PrefSubs->Append(_T("dvb_subtitle"));
	PrefSubs->Append(_T("dan"));
	PrefSubs->Append(_T("eng"));
	FlexGridSizer8->Add(PrefSubs, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3->Add(FlexGridSizer8, 1, wxALL|wxEXPAND, 3);
	SubsInverse = new wxCheckBox(this, ID_SUBSINVERSE, _T("SI"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSINVERSE"));
	SubsInverse->SetValue(false);
	SubsInverse->SetLabel(FFQS(SID_BATCHMAKE_MATCH_INVERSE));
	FlexGridSizer3->Add(SubsInverse, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	FlexGridSizer3->Add(StaticLine3, 1, wxTOP|wxBOTTOM|wxEXPAND, 3);
	FlexGridSizer5 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer5->AddGrowableCol(1);
	ST3 = new wxStaticText(this, ID_ST3, _T("DestFmt"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST3"));
	ST3->SetLabel(FFQS(SID_BATCHMAKE_DEST_FORMAT));
	FlexGridSizer5->Add(ST3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	OutputFormat = new wxComboBox(this, ID_OUTPUTFORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_OUTPUTFORMAT"));
	OutputFormat->SetSelection( OutputFormat->Append(_T("mp4")) );
	OutputFormat->Append(_T("mkv"));
	OutputFormat->Append(_T("m4v"));
	OutputFormat->Append(_T("avi"));
	OutputFormat->Append(_T("mpg"));
	OutputFormat->Append(_T("flv"));
	OutputFormat->Append(_T("mp3"));
	OutputFormat->Append(_T("flac"));
	OutputFormat->Append(_T("wav"));
	FlexGridSizer5->Add(OutputFormat, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer7 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer7->AddGrowableCol(1);
	ST4 = new wxStaticText(this, ID_ST4, _T("DestFo"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST4"));
	ST4->SetLabel(FFQS(SID_COMMON_DEST_FOLDER));
	FlexGridSizer7->Add(ST4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	OutputPath = new wxTextCtrl(this, ID_OUTPUTPATH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OUTPUTPATH"));
	FlexGridSizer7->Add(OutputPath, 1, wxALL|wxEXPAND, 0);
	BrowseButton = new wxButton(this, ID_BROWSEBUTTON, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BROWSEBUTTON"));
	FlexGridSizer7->Add(BrowseButton, 1, wxLEFT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer6 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer6->AddGrowableCol(1);
	ST5 = new wxStaticText(this, ID_ST5, _T("SelP"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST5"));
	ST5->SetLabel(FFQS(SID_BATCHMAKE_SELECT_PRESET));
	FlexGridSizer6->Add(ST5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	Preset = new FFQPresetPanel(this);
	FlexGridSizer6->Add(Preset, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3->Add(FlexGridSizer6, 1, wxALL|wxEXPAND, 3);
	SaveLogs = new wxCheckBox(this, ID_MAKELOGS, _T("Sl"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MAKELOGS"));
	SaveLogs->SetValue(false);
	SaveLogs->SetLabel(FFQS(SID_COMMON_SAVE_LOG));
	FlexGridSizer3->Add(SaveLogs, 1, wxALL|wxEXPAND, 3);
	SBS1->Add(FlexGridSizer3, 1, wxTOP|wxEXPAND, 5);
	MainSizer->Add(SBS1, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	DryRunButton = new wxButton(this, ID_DRYRUNBUTTON, _T("D"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DRYRUNBUTTON"));
	DryRunButton->SetLabel(FFQS(SID_COMMON_DRY_RUN));
	BoxSizer1->Add(DryRunButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MakeButton = new wxButton(this, ID_MAKEBUTTON, _T("M"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MAKEBUTTON"));
	MakeButton->SetDefault();
	MakeButton->SetLabel(FFQS(SID_BATCHMAKE_MAKE_JOBS));
	BoxSizer1->Add(MakeButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer1->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MainSizer->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	Gauge = new wxGauge(this, ID_GAUGE, 1000, wxDefaultPosition, wxSize(450,-1), 0, wxDefaultValidator, _T("ID_GAUGE"));
	MainSizer->Add(Gauge, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	SetSizer(MainSizer);
	DestPathDlg = new wxDirDialog(this, _T("Select destination folder"), wxEmptyString, wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxDirDialog"));
	MainSizer->Fit(this);
	MainSizer->SetSizeHints(this);

	Connect(ID_INCLUDEVIDEO,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_INCLUDEAUDIO,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_INCLUDESUBS,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_PREFAUDIO,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_AUDINVERSE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_PREFSUBS,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_BROWSEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_DRYRUNBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_MAKEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQBatchMake::OnAction);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&FFQBatchMake::OnClose);
	//*)

	Connect(wxID_ANY, wxEVT_IDLE, (wxObjectEventFunction)&FFQBatchMake::OnIdle);
	//wxSize cs = Gauge->GetSize();
	//cs.x -= 20; //My bad
	//JobInfo->SetMaxSize(cs);

	//wxSize s = GetSize();
	//SetMaxSize(s);

    SetTitle(FFQS(SID_BATCHMAKE_TITLE));

	m_Files = NULL;
	m_DoIdleTask = false;

	/*wxString cc = FFQCFG()->GetFFMpegCodecs(ctAUDIO);
	while (cc.Len() > 0)
    {
        wxString c = GetToken(cc, "\n", true);
        PrefAudio->Append(GetToken(c, " ", false));
    }*/

}

//---------------------------------------------------------------------------------------

FFQBatchMake::~FFQBatchMake()
{
	//(*Destroy(FFQBatchMake)
	//*)
}

//---------------------------------------------------------------------------------------

void FFQBatchMake::Clear(bool releaseJobPointers)
{
    //Clear parsers
    for (unsigned int i = 0; i < PIP_COUNT; i++) m_PIPS[i].SetProbeOutput("");

    //Release job pointers
    if (releaseJobPointers)
    {
        for (unsigned int i = 0; i < m_Jobs.size(); i++) delete (LPFFQ_JOB)m_Jobs[i];
    }

    //Clear jobs
    m_Jobs.clear();
}

//---------------------------------------------------------------------------------------

bool FFQBatchMake::Execute()
{
    unsigned int c = (m_Files == NULL) ? 0 : m_Files->Count(), pc = FFQPresetMgr::Get()->GetPresetCount();

    if ((c == 0) || (pc == 0)) return ShowError(NULL, FFQS(SID_NO_FILES_OR_NO_PRESET));

    m_ClearLog = false;//true;
    m_DoIdleTask = true;

    JobInfo->SetLabel(FFQSF(SID_BATCHMAKE_MAKE_JOBS_FOR, c));
    //Layout();
	//MainSizer->Fit(this);
	//MainSizer->SetSizeHints(this);

    Gauge->SetValue(0);
    DryRunButton->Enable(true);
    MakeButton->Enable(true);
    CancelButton->Enable(true);
    m_Making = false;

	//Load options
	wxString cfg = FFQCFG()->batch_config;
    IncludeVideo->SetValue(GetToken(cfg, ',') == STR_YES);
    IncludeAudio->SetValue(GetToken(cfg, ',') == STR_YES);
    IncludeSubs->SetValue(GetToken(cfg, ',') == STR_YES);
    PrefAudio->SetValue(GetToken(cfg, ','));
    Preset->FillAndSelect(GetToken(cfg, ','));
    AudInverse->SetValue(GetToken(cfg, ',') == STR_YES);
    PrefSubs->SetValue(GetToken(cfg, ','));
    SubsInverse->SetValue(GetToken(cfg, ',') == STR_YES);
    SaveLogs->SetValue(GetToken(cfg, ',') == STR_YES);

    //Global preferences we can use here
    OutputFormat->SetValue(FFQCFG()->preferred_format);
    OutputPath->SetValue(FFQCFG()->preferred_path);

    CenterOnParent();
    UpdateControls();

    bool res = (ShowModal() == wxOK);

    m_Files = NULL;

    return res;

}

//---------------------------------------------------------------------------------------

unsigned int FFQBatchMake::GetJobCount()
{
    return m_Jobs.size();
}

//---------------------------------------------------------------------------------------

void* FFQBatchMake::GetJob(unsigned int index)
{
    return m_Jobs[index];
}

//---------------------------------------------------------------------------------------

void FFQBatchMake::SetFiles(wxArrayString *files)
{
    m_Files = files;
}

//---------------------------------------------------------------------------------------

void FFQBatchMake::OnAction(wxCommandEvent& event)
{

    int evtId = event.GetId();

    if (evtId == ID_BROWSEBUTTON)
    {

        DestPathDlg->SetPath(OutputPath->GetValue());
        if (DestPathDlg->ShowModal() == wxID_OK) OutputPath->SetValue(DestPathDlg->GetPath());

    }

    else if ((evtId == ID_MAKEBUTTON) || (evtId == ID_DRYRUNBUTTON))
    {

        if ((!IncludeVideo->GetValue()) && (!IncludeAudio->GetValue()) && (!IncludeSubs->GetValue()))
        {
            ShowError(IncludeVideo, FFQS(SID_NO_CONTENT_SELECTED));
            return;
        }

        if (StrTrim(OutputFormat->GetValue()).Len() == 0)
        {
            ShowError(OutputFormat, FFQS(SID_OUTPUT_FORMAT_REQUIRED));
            return;
        }

        wxString dst = StrTrim(OutputPath->GetValue());
        if ((dst.Len() > 0) && (!wxDirExists(dst)))
        {
            ShowError(OutputPath, FFQS(SID_OUTPUT_PATH_EXIST_OR_EMPTY));
            return;
        }

        MakeButton->Enable(false);
        DryRunButton->Enable(false);

        MakeJobs(evtId == ID_DRYRUNBUTTON);

        if (m_Abort || (evtId == ID_DRYRUNBUTTON)) EndModal(wxCANCEL);

        else
        {
            SaveConfig();
            EndModal(wxOK);
        }

    }

    else if (evtId == ID_CANCELBUTTON)
    {

        if (m_Making) m_Abort = true;
        else EndModal(wxCANCEL);

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool FFQBatchMake::FindJobForDest(wxString dest)
{

    //Find in already created jobs
    for (unsigned int i = 0; i < m_Jobs.size(); i++)
        if ( CompareFileNames(((LPFFQ_JOB)m_Jobs[i])->out, dest) ) return true;

    //Find in main
    if ( ((FFQMain*)GetParent())->FindItemForFile(dest, 0, true) >= 0 ) return true;

    //Not found
    return false;

}

//---------------------------------------------------------------------------------------

/*wxString FFQBatchMake::GetStreamMapping(FFProbeInfoParser *pip, unsigned int file_id)
{

    //Create a stream mapping for the file
    wxString res = "";

    for (unsigned int sidx = 0; sidx < pip->GetStreamCount(); sidx++)
    {

        LPFFPROBE_STREAM_INFO si = pip->GetStreamInfo(sidx);
        STREAM_MAPPING smap;
        smap.checked = HasStreamFromParser(si);
        smap.codec_type = si->codec_type;
        smap.file_id = file_id;
        smap.stream_id = si->index;
        res += smap.ToString() + STREAM_MAPPING_SEPERATOR;

    }

    return res.Len() > 0 ? res.BeforeLast(STREAM_MAPPING_SEPERATOR) : "";

}*/

//---------------------------------------------------------------------------------------

void FFQBatchMake::GetStreamsFromParser(FFProbeInfoParser *pip, unsigned int file_id, wxString &stream_map)
{

    //Extract the streams from "pip" which should be included in the job
    unsigned int used = 0;

    for (unsigned int sidx = 0; sidx < pip->GetStreamCount(); sidx++)
    {

        LPFFPROBE_STREAM_INFO si = pip->GetStreamInfo(sidx);

        STREAM_MAPPING smap;
        smap.codec_type = si->codec_type;
        smap.file_id    = file_id;
        smap.stream_id  = si->index;
        smap.checked    = false;

        if (IncludeVideo->GetValue() && (si->codec_type == CODEC_TYPE_VIDEO) && (m_VidIdx < 0))
        {

            m_VidIdx = m_StreamInf.size();
            m_StreamInf.push_back(si);
            smap.checked = true;
            used++;

        }

        else if (IncludeAudio->GetValue() && (si->codec_type == CODEC_TYPE_AUDIO) && IncludeStream(si, m_FindAud, true, AudInverse->GetValue())) //IncludeAudioStream(si))
        {

            if (m_AudIdx < 0) m_AudIdx = m_StreamInf.size();
            m_StreamInf.push_back(si);
            smap.checked = true;
            used++;

        }

        else if (IncludeSubs->GetValue() && (si->codec_type == CODEC_TYPE_SUBTITLE) && (!si->IsTeleText()) && IncludeStream(si, m_FindSubs, true, SubsInverse->GetValue())) //IncludeSubtitleStream(si))
        {

            if (m_SubsIdx < 0) m_SubsIdx = m_StreamInf.size();
            m_StreamInf.push_back(si);
            smap.checked = true;
            used++;

        }

        else if ((IncludeData->GetValue() && (si->codec_type == CODEC_TYPE_DATA)) || (IncludeAttachment->GetValue() && (si->codec_type == CODEC_TYPE_ATTACHMENT)))
        {

            m_StreamInf.push_back(si);
            smap.checked = true;
            used++;

        }

        stream_map += smap.ToString() + STREAM_MAPPING_SEPERATOR;

    }

    //If nothing was used, clear stream map
    if (used == 0) stream_map = "";

    //Else remove last separator
    else stream_map = stream_map.BeforeLast(STREAM_MAPPING_SEPERATOR);

}

//---------------------------------------------------------------------------------------

/*bool FFQBatchMake::HasStreamFromParser(LPFFPROBE_STREAM_INFO si)
{
    for (std::vector<LPFFPROBE_STREAM_INFO>::const_iterator ite = m_StreamInf.begin(); ite != m_StreamInf.end(); ite++)
        if (*ite == si) return true;
    return false;
}*/

//---------------------------------------------------------------------------------------

/*bool FFQBatchMake::IncludeAudioStream(LPFFPROBE_STREAM_INFO si)
{
    //Test is an audio stream can be included
    if (m_FindAud.Len() == 0) return true; //!AudInverse->GetValue();
    bool match = false;
    wxString s = m_FindAud, t;
    while ((!match) && (s.Len() > 0))
    {
        t = GetToken(s, " ");
        match = (si->codec_name.Lower().Find(t) >= 0) || (si->codec_long_name.Lower().Find(t) >= 0);
    }
    if (AudInverse->GetValue()) return !match;
    return match;
}*/

//---------------------------------------------------------------------------------------

/*bool FFQBatchMake::IncludeSubtitleStream(LPFFPROBE_STREAM_INFO si)
{
    //Test if a subtitle stream can be included
    if (m_FindSubs.Len() == 0) return true;
    bool match = false;
    FFQ_NVP_LIST *nvp = si->items->Find("tag:language", false);
    wxString v = nvp ? nvp->value : "", s = m_FindSubs, t;
    while ((!match) && (s.Len() > 0))
    {
        t = GetToken(s, " ");
        match = ((v.Lower().Find(t) >= 0) || (si->codec_name.Lower().Find(t) >= 0) || (si->codec_long_name.Lower().Find(t) >= 0));
    }
    if (SubsInverse->GetValue()) return !match;
    return match;
}*/

//---------------------------------------------------------------------------------------

bool FFQBatchMake::IncludeStream(LPFFPROBE_STREAM_INFO si, wxString search_for, bool find_language, bool invert_result)
{
    //Test if the applied stream matches the criteria for inclusion
    if (search_for.Len() == 0) return true;
    bool match = false;
    wxString lng, s;
    if (find_language)
    {
        FFQ_NVP_LIST *nvp = si->items->Find("tag:language", false);
        if (nvp) lng = nvp->value.Lower();
    }
    while ((!match) && (search_for.Len() > 0))
    {
        s = GetToken(search_for, SPACE);
        match = ((lng.Find(s) >= 0) || (si->codec_name.Lower().Find(s) >= 0) || (si->codec_long_name.Lower().Find(s) >= 0));
    }
    return invert_result ? !match : match;
}

//---------------------------------------------------------------------------------------

void FFQBatchMake::LogLine(wxString line, unsigned int color)
{

    FFQConsole::Get()->AppendLine(line, color, m_ClearLog);
    m_ClearLog = false;

}

//---------------------------------------------------------------------------------------

/*bool StreamInfoMatch(LPFFPROBE_STREAM_INFO si, wxString &findStr, bool inverse = false)
{
    if (findStr.Len() == 0) return true;
    bool match = (si->codec_name.Lower().Find(findStr) >= 0) || (si->codec_long_name.Lower().Find(findStr) >= 0);
    if (inverse) return !match;
    return match;
}*/

//---------------------------------------------------------------------------------------

/*LPFFPROBE_STREAM_INFO FindStreamInfo(FFProbeInfoParser *PIPs, CODEC_TYPE ct, wxString findStr = "", wxVector<LPFFPROBE_STREAM_INFO> *dest = NULL)
{

    findStr.LowerCase(); //Always search for lowercase string

    for (unsigned int pidx = 0; pidx < PIP_COUNT; pidx++)
    {

        FFProbeInfoParser* pip = &PIPs[pidx];

        for (unsigned int sidx = 0; sidx < pip->GetStreamCount(); sidx++)
        {

            LPFFPROBE_STREAM_INFO si = pip->GetStreamInfo(sidx);

            if (StreamInfoMatch(si, findStr))
            {

                if ((ct == ctVIDEO) && (si->codec_type == CODEC_TYPE_VIDEO)) return si;
                else if ((ct == ctAUDIO) && (si->codec_type == CODEC_TYPE_AUDIO)) return si;
                else if ((ct == ctSUBTITLE) && (si->codec_type == CODEC_TYPE_SUBTITLE) && (!si->IsTeleText())) return si;

            }

        }

    }

    return NULL;
}*/

//---------------------------------------------------------------------------------------

/*wxString MakeStreamMapping(LPFFPROBE_STREAM_INFO sinf, int fileID, bool chk)
{
    STREAM_MAPPING smap;
    smap.checked = chk;
    smap.codec_type = sinf->codec_type;
    smap.file_id = fileID;
    smap.stream_id = sinf->index;
    return smap.ToString();
}*/

//---------------------------------------------------------------------------------------

void FFQBatchMake::MakeJobs(bool dry_run)
{

    m_Making = true;
    m_Abort = false;

    //Grab the search string used for audio streams
    m_FindAud = StrTrim(PrefAudio->GetValue());
    m_FindAud.LowerCase();
    m_FindSubs = StrTrim(PrefSubs->GetValue());
    m_FindSubs.LowerCase();

    LPFFQ_JOB job;
    FFQ_INPUT_FILE in1, in2;

    unsigned int fileCount = m_Files->Count();
    wxString cur, sub_path;
    long idx, pt_pos;
    LPFFQ_PRESET preset = Preset->GetSelectedPreset();

    FFQMain* main = (FFQMain*)GetParent();
    FFQProcess* proc = new FFQProcess();
    FFQConfig *cfg = FFQConfig::GetInstance();

    for (unsigned int i = 0; i < fileCount; i++)
    {

        Gauge->SetValue((long)round((float)(i+1) / (float)fileCount * 1000.0));
        FFQCFG()->GetTaskBar()->SetTaskBarProgress(i, fileCount - 1);

        cur = m_Files->Item(i);

        //Make sure that the sub-path is preserved
        pt_pos = cur.Find(PATH_TOKEN);
        if (pt_pos > 0)
        {
            sub_path = cur.SubString(pt_pos + PATH_TOKEN.Len(), cur.Len());
            cur = cur.SubString(0, pt_pos - 1) + wxFileName::GetPathSeparator() + sub_path;
            #ifdef DEBUG
            //LogLine("sub_path=" + sub_path + CRLF + "cur=" + cur + CRLF, COLOR_BLUE);
            #endif // DEBUG
        }
        else sub_path = cur.AfterLast(wxFileName::GetPathSeparator());

        idx = main->FindItemForFile(cur);
        if (idx >= 0) LogLine(FFQSF(SID_LOG_ALREADY_QUEUED, cur), COLOR_RED);
        else try
        {

            job = new FFQ_JOB();
            job->save_log = SaveLogs->GetValue();
            in1.Reset();
            in2.Reset();

            in1.path = cur;
            in2.path = FFQCFG()->FindSecondaryInputFile(cur);

            LogLine(FFQSF(SID_LOG_MAKE_JOB_FOR, in1.path), COLOR_BLACK);

            //File 1
            proc->FFProbe(in1.path);
            m_PIPS[0].SetProbeOutput(proc->GetProcessOutput(false, true));

            //File 2 - if necessary
            if (in2.path.Len() > 0)
            {
                proc->FFProbe(in2.path);
                m_PIPS[1].SetProbeOutput(proc->GetProcessOutput(false, true));
                if (m_PIPS[1].GetStreamCount() == 0) in2.path = ""; //File 2 was invalid
            }

            //Clear values before searching for streams
            m_StreamInf.clear();
		    m_VidIdx = -1;
		    m_AudIdx = -1;
		    m_SubsIdx = -1;

            //Find streams and create a stream map
            wxString smap1, smap2;
            GetStreamsFromParser(&m_PIPS[0], 1, smap1);
            GetStreamsFromParser(&m_PIPS[1], 2, smap2);

            if ((m_VidIdx < 0) && (!NoFailVideo->GetValue()) && IncludeVideo->GetValue()) LogLine("\t" + FFQSF(SID_LOG_CONTENT_NOT_FOUND, FFQS(SID_COMMON_VIDEO)), COLOR_RED);
            else if ((m_AudIdx < 0) && (!NoFailAudio->GetValue()) && IncludeAudio->GetValue()) LogLine("\t" + FFQSF(SID_LOG_CONTENT_NOT_FOUND, FFQS(SID_COMMON_AUDIO)), COLOR_RED);
            else if ((m_SubsIdx < 0) && (!NoFailSubs->GetValue()) && IncludeSubs->GetValue()) LogLine("\t" + FFQSF(SID_LOG_CONTENT_NOT_FOUND, FFQS(SID_COMMON_SUBTITLES)), COLOR_RED);
            else
            {

                //Initialize default command line
                job->cmd_line = CMD_DEFAULT;

                //Get values for in1
                m_PIPS[0].GetDuration(in1.duration);
                m_PIPS[0].GetVideoDimension(in1.width, in1.height);

                //Get values for in2 - if possible
                if (smap2.Len() > 0)
                {
                    m_PIPS[1].GetDuration(in2.duration);
                    m_PIPS[1].GetVideoDimension(in2.width, in2.height);
                }

                //Name of output file
                job->out = StrTrim(OutputPath->GetValue());
                if (job->out.Len() == 0)
                {
                    //No output path = use source path
                    job->out = in1.path.BeforeLast(DOT) + DOT + StrTrim(OutputFormat->GetValue());
                }
                else
                {
                    //Add sub_path to output path
                    if (job->out[job->out.Len() - 1] != wxFileName::GetPathSeparator()) job->out += wxFileName::GetPathSeparator();
                    job->out += sub_path.BeforeLast(DOT) + DOT + StrTrim(OutputFormat->GetValue());
                }

                //Make sure that the name is coherent with the naming scheme - except preferred format
                job->out = cfg->GetPreferredOutputName(job->out, preset, false, false);

                //Make sure that source and destination are not equal
                if (in1.path.Lower() == job->out.Lower()) LogLine("\t" + FFQSF(SID_SOURCE_EQUALS_OUTPUT, job->out), COLOR_RED);

                //Make sure that no jobs has already been created with the destination
                else if (FindJobForDest(job->out))
                {

                    //If suggest unique names is enabled, generate a unique name
                    if (cfg->preferred_unique)
                    {

                        unsigned int cc = 0;
                        wxString a = job->out.BeforeLast(DOT), b = job->out.AfterLast(DOT);
                        do
                        {
                            job->out = a + wxString::Format("_%u", ++cc) + DOT + b;
                        }
                        while (FindJobForDest(job->out) || wxFileExists(job->out));

                    }

                    //Else bug out on this one
                    else LogLine("\t" + FFQSF(SID_DUPLICATE_JOB_FOR_OUTPUT, job->out), COLOR_RED);

                }
                else
                {

                    //Preset to use
                    job->preset_id = preset->preset_id;

                    //Print included streams
                    for (std::vector<LPFFPROBE_STREAM_INFO>::const_iterator ite = m_StreamInf.begin(); ite != m_StreamInf.end(); ite++)
                        LogLine("\t" + (*ite)->codec_type + " : " + (*ite)->codec_long_name, COLOR_GREEN);

                    //All streams has to be implemented to prevent job-editor from sorting streams wrong
                    job->stream_map = smap1;
                    if (smap2.Len() > 0) job->stream_map += STREAM_MAPPING_SEPERATOR + smap2;

                    //Add input files
                    job->inputs.Add(in1.ToString());
                    if (smap2.Len() > 0) job->inputs.Add(in2.ToString());

                    if (!dry_run)
                    {
                        //Add to list and set pointer to NULL to prevent deletion
                        m_Jobs.push_back(job);
                        job = NULL;
                    }

                }

            }

            //Free unused pointer
            if (job != NULL) delete job;

        }

        catch (std::exception &err)
        {

            LogLine("Error: " + wxString(err.what()), false);

        }

        Yield_App(); //Process messages in order to accept aborts

        if (m_Abort)
        {
            ShowInfo(this, FFQS(SID_TASK_ABORTED));
            break;
            //EndModal(wxCANCEL);
            //return;
        }

        LogLine("", true); //Blank line before next job..

    }

    //delete pip;
    delete proc;

    //Restore values
    //FFQCFG()->preferred_format = cfg_fmt;
    //FFQCFG()->preferred_path = cfg_path;
    FFQCFG()->GetTaskBar()->SetTaskBarProgress(0, 0);

}

//---------------------------------------------------------------------------------------

void FFQBatchMake::SaveConfig()
{
    LPFFQ_PRESET pst = Preset->GetSelectedPreset();

    wxString s, pa = StrTrim(PrefAudio->GetValue()), ps = StrTrim(PrefSubs->GetValue());
    pa.Replace(COMMA, SPACE);
    ps.Replace(COMMA, SPACE);
    s.Printf(
        "%s,%s,%s,%s,%s,%s,%s,%s,%s",
        BOOLSTR(IncludeVideo->GetValue()),
        BOOLSTR(IncludeAudio->GetValue()),
        BOOLSTR(IncludeSubs->GetValue()),
        pa,
        pst->preset_id.ToString(),
        BOOLSTR(AudInverse->GetValue()),
        ps,
        BOOLSTR(SubsInverse->GetValue()),
        BOOLSTR(SaveLogs->GetValue())
    );
    bool save = FFQCFG()->batch_config != s;
    FFQCFG()->batch_config = s;
    FFQCFG()->SetPreferredAndSave(StrTrim(OutputFormat->GetValue()), StrTrim(OutputPath->GetValue()), save);
}

//---------------------------------------------------------------------------------------

void FFQBatchMake::UpdateControls()
{

    //Enable audio controls
    ST2->Enable(IncludeAudio->GetValue());
    PrefAudio->Enable(IncludeAudio->GetValue());
    AudInverse->Enable(IncludeAudio->GetValue() && (StrTrim(PrefAudio->GetValue()).Len() > 0));

    //Enable subtitle controls
    ST6->Enable(IncludeSubs->GetValue());
    PrefSubs->Enable(IncludeSubs->GetValue());
    SubsInverse->Enable(IncludeSubs->GetValue() && (StrTrim(PrefSubs->GetValue()).Len() > 0));

    //Enable no-fail controls
    NoFailVideo->Enable(IncludeVideo->GetValue());
    NoFailAudio->Enable(IncludeAudio->GetValue());
    NoFailSubs->Enable(IncludeSubs->GetValue());

}

//---------------------------------------------------------------------------------------

void FFQBatchMake::OnIdle(wxIdleEvent &event)
{

    if (m_DoIdleTask)
    {

        //Prevent task from being repeated
        m_DoIdleTask = false;

        //Expand all folders in the list of files
        wxString s, t;
        wxArrayString tmp;
        unsigned int i = 0;

        while (i < m_Files->Count())
        {

            //Get item
            s = m_Files->Item(i);

            if (wxDirExists(s))
            {

                //Get all files in folder
                wxDir::GetAllFiles(s, &tmp);

                //Go one level up
                s = s.BeforeLast(wxFileName::GetPathSeparator());

                //Remove the folder
                m_Files->RemoveAt(i);

                //Add files from folder using PATH_TOKEN as separator
                //to signal that a sub-folder must be created in destination folder
                for (unsigned int ii = 0; ii < tmp.Count(); ii++)
                {

                    t = tmp[ii];
                    m_Files->Insert(s + PATH_TOKEN + t.SubString(s.Len() + 1, t.Len()), i);
                    i++;

                }

                //Clear tmp
                tmp.Clear();

                //Update label
                JobInfo->SetLabel(FFQSF(SID_BATCHMAKE_MAKE_JOBS_FOR, SIZEFMT(m_Files->Count())));
                JobInfo->Update();

            }
            else i++;

        }

        if (m_Files->Count() == 0)
        {

            //No files was found
            ShowError(NULL, FFQS(SID_NO_FILES_OR_NO_PRESET));
            EndModal(wxID_CANCEL);
            return;

        }

        //Just in case the label has expanded
        Layout();

    }

    else event.Skip();
}

//---------------------------------------------------------------------------------------

void FFQBatchMake::OnClose(wxCloseEvent& event)
{
    if (m_Making && (!m_Abort)) return;
    event.Skip();
}
