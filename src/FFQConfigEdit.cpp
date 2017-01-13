/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConfigEdit.cpp                                               *
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

#include "FFQConfigEdit.h"
#include "utils/FFQLang.h"
#include "FFQLangEdit.h"
#include "utils/FFQConfig.h"

#include <wx/filename.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQConfigEdit)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQConfigEdit)
//*)

//(*IdInit(FFQConfigEdit)
const long FFQConfigEdit::ID_FFMPEGPATH = wxNewId();
const long FFQConfigEdit::ID_BROWSEFFMPEG = wxNewId();
const long FFQConfigEdit::ID_CUSTPLAYER = wxNewId();
const long FFQConfigEdit::ID_BROWSECUSTPLAYER = wxNewId();
const long FFQConfigEdit::ID_TP_SYSTEM = wxNewId();
const long FFQConfigEdit::ID_TP_DEST = wxNewId();
const long FFQConfigEdit::ID_TP_CUSTOM = wxNewId();
const long FFQConfigEdit::ID_TPRPANEL = wxNewId();
const long FFQConfigEdit::ID_CUSTOMTEMP = wxNewId();
const long FFQConfigEdit::ID_BROWSETEMP = wxNewId();
const long FFQConfigEdit::ID_SECONDFILEEXTS = wxNewId();
const long FFQConfigEdit::ID_CONSOLECMD = wxNewId();
const long FFQConfigEdit::ID_LANGBUTTON = wxNewId();
const long FFQConfigEdit::ID_OKBUTTON = wxNewId();
const long FFQConfigEdit::ID_CANCELBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQConfigEdit,wxDialog)
	//(*EventTable(FFQConfigEdit)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQConfigEdit::FFQConfigEdit(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(FFQConfigEdit)
	wxStaticBoxSizer* SBS1;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticBoxSizer* SBS3;
	wxStaticBoxSizer* SBS5;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer2;
	wxStaticBoxSizer* SBS4;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* SBS2;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(7, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(1);
	SBS1 = new wxStaticBoxSizer(wxVERTICAL, this, _("FFP"));
	FlexGridSizer2 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	FFMpegPath = new wxTextCtrl(this, ID_FFMPEGPATH, wxEmptyString, wxDefaultPosition, wxSize(420,-1), 0, wxDefaultValidator, _T("ID_FFMPEGPATH"));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_OPTIONS_FFMPEG_PATH));
	FlexGridSizer2->Add(FFMpegPath, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer2->Add(5,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BrowseFFMpeg = new wxButton(this, ID_BROWSEFFMPEG, _("..."), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_BROWSEFFMPEG"));
	FlexGridSizer2->Add(BrowseFFMpeg, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SBS1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer1->Add(SBS1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS4 = new wxStaticBoxSizer(wxVERTICAL, this, _("Playr"));
	FlexGridSizer5 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer5->AddGrowableCol(0);
	CustPlayer = new wxTextCtrl(this, ID_CUSTPLAYER, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CUSTPLAYER"));
	SBS4->GetStaticBox()->SetLabel(FFQS(SID_OPTIONS_CUSTOM_PLAYER));
	FlexGridSizer5->Add(CustPlayer, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer5->Add(5,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BrowseCustPlayer = new wxButton(this, ID_BROWSECUSTPLAYER, _("..."), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_BROWSECUSTPLAYER"));
	FlexGridSizer5->Add(BrowseCustPlayer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SBS4->Add(FlexGridSizer5, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer1->Add(SBS4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Tp"));
	FlexGridSizer4 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	TempPathRadioPanel = new wxPanel(this, ID_TPRPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_TPRPANEL"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	TP_System = new wxRadioButton(TempPathRadioPanel, ID_TP_SYSTEM, _("Sd"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TP_SYSTEM"));
	TP_System->SetValue(true);
	TP_System->SetLabel(FFQS(SID_OPTIONS_TEMP_SYS_DEFAULT));
	SBS2->GetStaticBox()->SetLabel(FFQS(SID_OPTIONS_TEMPORARY_PATH));
	BoxSizer1->Add(TP_System, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TP_Dest = new wxRadioButton(TempPathRadioPanel, ID_TP_DEST, _("Dp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TP_DEST"));
	TP_Dest->SetLabel(FFQS(SID_OPTIONS_TEMP_DEST_PATH));
	BoxSizer1->Add(TP_Dest, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TP_Custom = new wxRadioButton(TempPathRadioPanel, ID_TP_CUSTOM, _("Cp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TP_CUSTOM"));
	TP_Custom->SetLabel(FFQS(SID_OPTIONS_TEMP_CUSTOM));
	BoxSizer1->Add(TP_Custom, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TempPathRadioPanel->SetSizer(BoxSizer1);
	BoxSizer1->Fit(TempPathRadioPanel);
	BoxSizer1->SetSizeHints(TempPathRadioPanel);
	FlexGridSizer4->Add(TempPathRadioPanel, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer6 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer6->AddGrowableCol(0);
	FlexGridSizer6->AddGrowableRow(0);
	CustomTemp = new wxTextCtrl(this, ID_CUSTOMTEMP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CUSTOMTEMP"));
	FlexGridSizer6->Add(CustomTemp, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer6->Add(3,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BrowseTemp = new wxButton(this, ID_BROWSETEMP, _("..."), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_BROWSETEMP"));
	FlexGridSizer6->Add(BrowseTemp, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer4->Add(FlexGridSizer6, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer1->Add(SBS2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS3 = new wxStaticBoxSizer(wxVERTICAL, this, _("2exts"));
	SecondFileExts = new wxTextCtrl(this, ID_SECONDFILEEXTS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SECONDFILEEXTS"));
	SBS3->GetStaticBox()->SetLabel(FFQS(SID_OPTIONS_FIND_2ND_INPUT_EXTS));
	SBS3->Add(SecondFileExts, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer1->Add(SBS3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS5 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Ccc"));
	ConsoleCmd = new wxTextCtrl(this, ID_CONSOLECMD, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CONSOLECMD"));
	SBS5->GetStaticBox()->SetLabel(FFQS(SID_OPTIONS_CUSTOM_CONSOLE_CMD));
	SBS5->Add(ConsoleCmd, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer1->Add(SBS5, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer3 = new wxFlexGridSizer(8, 2, 5, 0);
	FlexGridSizer3->AddGrowableCol(1);
	FlexGridSizer3->AddGrowableRow(0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SaveOnExit = new wxCheckBox(this, wxID_ANY, _("Sox"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	SaveOnExit->SetValue(false);
	SaveOnExit->SetLabel(FFQS(SID_OPTIONS_SAVE_JOBS_ON_EXIT));
	FlexGridSizer3->Add(SaveOnExit, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	AutoRemove = new wxCheckBox(this, wxID_ANY, _("Ar"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	AutoRemove->SetValue(false);
	AutoRemove->SetLabel(FFQS(SID_OPTIONS_REMOVE_FINISHED_JOBS));
	FlexGridSizer3->Add(AutoRemove, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	LimitStats = new wxCheckBox(this, wxID_ANY, _("Ls"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	LimitStats->SetValue(false);
	LimitStats->SetLabel(FFQS(SID_OPTIONS_LIMIT_STATISTICS));
	FlexGridSizer3->Add(LimitStats, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ShowBanner = new wxCheckBox(this, wxID_ANY, _("Sbi"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ShowBanner->SetValue(false);
	ShowBanner->SetLabel(FFQS(SID_OPTIONS_SHOW_BANNER_INFO));
	FlexGridSizer3->Add(ShowBanner, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SaveWindowPos = new wxCheckBox(this, wxID_ANY, _("Swp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	SaveWindowPos->SetValue(false);
	SaveWindowPos->SetLabel(FFQS(SID_OPTIONS_SAVE_WINDOW_POS));
	FlexGridSizer3->Add(SaveWindowPos, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FullCodecList = new wxCheckBox(this, wxID_ANY, _("Fc"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	FullCodecList->SetValue(false);
	FullCodecList->SetLabel(FFQS(SID_OPTIONS_FULL_CODEC_LIST));
	FlexGridSizer3->Add(FullCodecList, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	UniqueNames = new wxCheckBox(this, wxID_ANY, _("Un"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	UniqueNames->SetValue(false);
	UniqueNames->SetLabel(FFQS(SID_OPTIONS_ALWAYS_UNIQUE_NAMES));
	FlexGridSizer3->Add(UniqueNames, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3->Add(5,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	KeepConsole = new wxCheckBox(this, wxID_ANY, _("Kc"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	KeepConsole->SetValue(false);
	KeepConsole->SetLabel(FFQS(SID_OPTIONS_KEEP_CONSOLE_OPEN));
	FlexGridSizer3->Add(KeepConsole, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	LangButton = new wxButton(this, ID_LANGBUTTON, _("Lng"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_LANGBUTTON"));
	LangButton->SetLabel(FFQS(SID_OPTIONS_EDIT_LANGUAGE));
	BoxSizer2->Add(LangButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OKButton = new wxButton(this, ID_OKBUTTON, _("O"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBUTTON"));
	OKButton->SetDefault();
	OKButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer2->Add(OKButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer2->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	OpenDialog = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	DirDlg = new wxDirDialog(this, _("Select path"), wxEmptyString, wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxDirDialog"));
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_BROWSEFFMPEG,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_BROWSECUSTPLAYER,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_TP_SYSTEM,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_TP_DEST,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_TP_CUSTOM,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_BROWSETEMP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_LANGBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_OKBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConfigEdit::OnButtonClick);
	//*)

	SetTitle(FFQS(SID_OPTIONS_TITLE));

}

//---------------------------------------------------------------------------------------

FFQConfigEdit::~FFQConfigEdit()
{
	//(*Destroy(FFQConfigEdit)
	//*)
}

//---------------------------------------------------------------------------------------

bool FFQConfigEdit::Execute()
{

    //Reset form to stored options
    FFMpegPath->SetValue(FFQCFG()->GetFFMpegCommand());
    CustPlayer->SetValue(FFQCFG()->cust_player);

    wxString s = FFQCFG()->temp_path;
    TP_System->SetValue(s == TEMP_PATH_SYST);
    TP_Dest->SetValue(s == TEMP_PATH_DEST);
    TP_Custom->SetValue( (!TP_System->GetValue()) && (!TP_Dest->GetValue()) );
    CustomTemp->SetValue( TP_Custom->GetValue() ? s : "");

    SecondFileExts->SetValue(FFQCFG()->second_file_extensions);
    ConsoleCmd->SetValue(FFQCFG()->console_cmd);

    AutoRemove->SetValue(FFQCFG()->auto_remove_jobs);
    SaveOnExit->SetValue(FFQCFG()->save_on_exit);
    LimitStats->SetValue(FFQCFG()->limit_statistics);
    ShowBanner->SetValue(!FFQCFG()->hide_banner);
    SaveWindowPos->SetValue(FFQCFG()->save_window_pos);
    FullCodecList->SetValue(FFQCFG()->full_codec_listings);
    UniqueNames->SetValue(FFQCFG()->preferred_unique);
    KeepConsole->SetValue(FFQCFG()->keep_console);

    //Center and update
    CenterOnParent();
    UpdateControls();

    //Execute dialog
    if (this->ShowModal() == wxID_OK)
    {

        //Save changes
        //FFQCFG()->strFFMpegPath = FFMpegPath->GetValue(); is set in OnButtonClick
        FFQCFG()->cust_player = CustPlayer->GetValue();

        if (TP_System->GetValue()) FFQCFG()->temp_path = TEMP_PATH_SYST;
        else if (TP_Dest->GetValue()) FFQCFG()->temp_path = TEMP_PATH_DEST;
        else FFQCFG()->temp_path = CustomTemp->GetValue();

        FFQCFG()->second_file_extensions = SecondFileExts->GetValue();
        FFQCFG()->console_cmd = ConsoleCmd->GetValue();

        FFQCFG()->auto_remove_jobs = AutoRemove->GetValue();
        FFQCFG()->save_on_exit = SaveOnExit->GetValue();
        FFQCFG()->limit_statistics = LimitStats->GetValue();
        FFQCFG()->hide_banner = !ShowBanner->GetValue();
        FFQCFG()->save_window_pos = SaveWindowPos->GetValue();
        FFQCFG()->full_codec_listings = FullCodecList->GetValue();
        FFQCFG()->preferred_unique = UniqueNames->GetValue();
        FFQCFG()->keep_console = KeepConsole->GetValue();

        FFQCFG()->SaveConfig();

        return true;

    }

    //Discard changes
    return false;

}

//---------------------------------------------------------------------------------------

void FFQConfigEdit::OnButtonClick(wxCommandEvent& event)
{

    int evtId = event.GetId();

    wxString s;

    if (evtId == ID_BROWSEFFMPEG)
    {

        s = FFQCFG()->GetExecutableName(etENCODER, false);
        OpenDialog->SetFilename(FFQCFG()->GetExecutableName(etENCODER));
        OpenDialog->SetWildcard(s + "|" + s + "*");
        OpenDialog->SetPath(FFMpegPath->GetValue());
        if (OpenDialog->ShowModal() != wxID_CANCEL) FFMpegPath->SetValue(OpenDialog->GetPath());

    }

    else if (evtId == ID_BROWSECUSTPLAYER)
    {

        s = "vlc";
        #ifdef __WINDOWS__
        s += ".exe";
        #endif // __WINDOWS__
        OpenDialog->SetFilename(s);
        OpenDialog->SetWildcard("VLC Media Player|" + s + "|*.*|*.*");
        OpenDialog->SetPath(CustPlayer->GetValue());
        if (OpenDialog->ShowModal() != wxID_CANCEL) CustPlayer->SetValue(OpenDialog->GetPath());

    }

    else if (evtId == ID_BROWSETEMP)
    {

        DirDlg->SetPath(CustomTemp->GetValue());
        if (DirDlg->ShowModal() != wxID_CANCEL) CustomTemp->SetValue(DirDlg->GetPath());

    }

    else if (evtId == ID_LANGBUTTON)
    {

        //#ifdef DEBUG
        //ShowInfo(ToStr(GetNextInteger()));
        //return;
        //#endif // DEBUG

        //FFQLangEdit::Get(this)->Execute();
        #ifndef __WINDOWS__
        EndModal(wxID_CANCEL);
        #endif // __WINDOWS__
        FFQLangEdit::Get(GetParent())->Execute();

    }

    else if (evtId == ID_OKBUTTON)
    {

        //Check custom player
        wxString s = CustPlayer->GetValue();
        if ((s.Len() > 0) &&  !wxFileExists(wxFileName(s).GetFullPath()))
        {

            ShowError(CustPlayer, FFQSF(SID_PATH_NOT_FOUND, s));
            return;

        }

        //Check the custom temporary path
        if (TP_Custom->GetValue() && (!wxDirExists(CustomTemp->GetValue())))
        {
            ShowError(TP_Custom, FFQS(SID_CUSTOM_TEMP_PATH_NOT_FOUND));
            return;
        }

        //Check the custom console command (not perfect!)
        s = ConsoleCmd->GetValue();
        if ((s.Len() > 0) && ((s.Find(' ') <= 0) || (s.Find("%s") <= 0)))
        {
            ShowError(ConsoleCmd, FFQS(SID_INVALID_CONSOLE_COMMAND));
            return;
        }

        s = wxFileName(FFMpegPath->GetValue()).GetFullPath();

        //Validate path to ffmpeg
        if (FFQCFG()->GetFFMpegCommand() != s)
        {

            if (!FFQCFG()->ValidateFFMpegPath(s, true))
            {

                ShowError(FFMpegPath, FFQS(SID_FF_BINARIES_NOT_FOUND));
                return;

            }

        }

        //Check if font.conf exists
        FFQCFG()->CheckFontsConf();

        EndModal(wxID_OK);
        return;

    }

    else if (evtId == ID_CANCELBUTTON)
    {
        EndModal(wxID_CANCEL);
        return;
    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQConfigEdit::UpdateControls()
{
    //Update controls
    CustomTemp->Enable(TP_Custom->GetValue());
    BrowseTemp->Enable(TP_Custom->GetValue());
}
