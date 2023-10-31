/*************************************************************************
*                                                                        *
*  Copyright (c) 2017 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQVideo2GIF.cpp                                                *
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

#include "../../wx_pch.h"
#include "FFQVideoToGIF.h"
#include "../utils/FFQLang.h"
#include "../utils/FFQConfig.h"
#include "../utils/FFQMisc.h"
#include <wx/valnum.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQVideoToGIF)
	#include <wx/string.h>
	#include <wx/intl.h>
	//*)
#endif
//(*InternalHeaders(FFQVideoToGIF)
//*)

//(*IdInit(FFQVideoToGIF)
const long FFQVideoToGIF::ID_SOURCE = wxNewId();
const long FFQVideoToGIF::ID_BROWSESRC = wxNewId();
const long FFQVideoToGIF::ID_FILEINFO = wxNewId();
const long FFQVideoToGIF::ID_STARTTIME = wxNewId();
const long FFQVideoToGIF::ID_WIDTH = wxNewId();
const long FFQVideoToGIF::ID_HEIGHT = wxNewId();
const long FFQVideoToGIF::ID_FRAMERATE = wxNewId();
const long FFQVideoToGIF::ID_TWOPASS = wxNewId();
const long FFQVideoToGIF::ID_PRECISECUTS = wxNewId();
const long FFQVideoToGIF::ID_LIMITLEN = wxNewId();
const long FFQVideoToGIF::ID_DEST = wxNewId();
const long FFQVideoToGIF::ID_BROWSEDST = wxNewId();
const long FFQVideoToGIF::ID_SAVELOG = wxNewId();
const long FFQVideoToGIF::ID_OKBTN = wxNewId();
const long FFQVideoToGIF::ID_NOBTN = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQVideoToGIF,wxDialog)
	//(*EventTable(FFQVideoToGIF)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQVideoToGIF::FFQVideoToGIF(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FFQVideoToGIF)
	wxBoxSizer* BS1;
	wxFlexGridSizer* FGS1;
	wxFlexGridSizer* FGS2;
	wxFlexGridSizer* FGS3;
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* SBS1;
	wxStaticBoxSizer* SBS2;
	wxStaticBoxSizer* SBS3;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FGS1 = new wxFlexGridSizer(4, 1, 0, 0);
	FGS1->AddGrowableCol(0);
	SBS1 = new wxStaticBoxSizer(wxVERTICAL, this, _("SV"));
	TopSizer = new wxFlexGridSizer(3, 1, 0, 0);
	TopSizer->AddGrowableCol(0);
	FlexGridSizer1 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	Source = new wxTextCtrl(this, ID_SOURCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SOURCE"));
	Source->SetMinSize(wxSize(400,-1));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_COMMON_SELECT_INPUT_FILE));
	FlexGridSizer1->Add(Source, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
	BrowseSrc = new wxButton(this, ID_BROWSESRC, _("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BROWSESRC"));
	FlexGridSizer1->Add(BrowseSrc, 1, wxLEFT|wxRIGHT|wxEXPAND, 3);
	TopSizer->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 0);
	FileInfo = new wxStaticText(this, ID_FILEINFO, _("Inf"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_FILEINFO"));
	TopSizer->Add(FileInfo, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StartTime = new wxGenericHyperlinkCtrl(this, ID_STARTTIME, _("St"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_ALIGN_CENTRE|wxBORDER_NONE, _T("ID_STARTTIME"));
	TopSizer->Add(StartTime, 1, wxALL, 5);
	SBS1->Add(TopSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FGS1->Add(SBS1, 1, wxALL|wxEXPAND, 5);
	SBS3 = new wxStaticBoxSizer(wxVERTICAL, this, _("Prop"));
	MidSizer = new wxFlexGridSizer(4, 1, 0, 0);
	MidSizer->AddGrowableCol(0);
	FGS2 = new wxFlexGridSizer(3, 6, 0, 0);
	ST7 = new wxStaticText(this, wxID_ANY, _("w"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	SBS3->GetStaticBox()->SetLabel(FFQS(SID_VIDEO2GIF_PROPERTIES));
	ST7->SetLabel(FFQS(SID_VIDEO2GIF_WIDTH));
	FGS2->Add(ST7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Width = new wxTextCtrl(this, ID_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WIDTH"));
	FGS2->Add(Width, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ST8 = new wxStaticText(this, wxID_ANY, _("h"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST8->SetLabel(FFQS(SID_VIDEO2GIF_HEIGHT));
	FGS2->Add(ST8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Height = new wxTextCtrl(this, ID_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_HEIGHT"));
	FGS2->Add(Height, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ST9 = new wxStaticText(this, wxID_ANY, _("Fr"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST9->SetLabel(FFQS(SID_PRESET_FRAMERATE));
	FGS2->Add(ST9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FrameRate = new wxTextCtrl(this, ID_FRAMERATE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FRAMERATE"));
	FGS2->Add(FrameRate, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	MidSizer->Add(FGS2, 1, wxALL|wxEXPAND, 0);
	TwoPass = new wxCheckBox(this, ID_TWOPASS, _("Tp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TWOPASS"));
	TwoPass->SetValue(false);
	TwoPass->SetLabel(FFQS(SID_VIDEO2GIF_TWOPASS));
	MidSizer->Add(TwoPass, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	PreciseCuts = new wxCheckBox(this, ID_PRECISECUTS, _("Pc"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_PRECISECUTS"));
	PreciseCuts->SetValue(false);
	PreciseCuts->SetLabel(FFQS(SID_VIDEO2GIF_PRECISE_CUTS));
	MidSizer->Add(PreciseCuts, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	LimitLen = new wxGenericHyperlinkCtrl(this, ID_LIMITLEN, _("Len"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE, _T("ID_LIMITLEN"));
	MidSizer->Add(LimitLen, 1, wxALL|wxALIGN_LEFT, 3);
	SBS3->Add(MidSizer, 1, wxALL|wxEXPAND, 5);
	FGS1->Add(SBS3, 1, wxALL|wxEXPAND, 5);
	SBS2 = new wxStaticBoxSizer(wxVERTICAL, this, _("DF"));
	FGS3 = new wxFlexGridSizer(1, 2, 0, 0);
	FGS3->AddGrowableCol(0);
	Dest = new wxTextCtrl(this, ID_DEST, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DEST"));
	SBS2->GetStaticBox()->SetLabel(FFQS(SID_COMMON_SELECT_OUTPUT_FILE));
	FGS3->Add(Dest, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
	BrowseDst = new wxButton(this, ID_BROWSEDST, _("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BROWSEDST"));
	FGS3->Add(BrowseDst, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
	SBS2->Add(FGS3, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	SaveLog = new wxCheckBox(this, ID_SAVELOG, _("Log"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SAVELOG"));
	SaveLog->SetValue(false);
	SaveLog->SetLabel(FFQS(SID_COMMON_SAVE_LOG));
	SBS2->Add(SaveLog, 1, wxLEFT|wxRIGHT, 10);
	FGS1->Add(SBS2, 1, wxALL|wxEXPAND, 5);
	BS1 = new wxBoxSizer(wxHORIZONTAL);
	BS1->Add(-1,-1,1, wxALL|wxEXPAND, 3);
	BS1->Add(-1,-1,1, wxALL|wxEXPAND, 3);
	OkBtn = new wxButton(this, ID_OKBTN, _("Y"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBTN"));
	OkBtn->SetDefault();
	OkBtn->SetLabel(FFQS(SID_COMMON_OK));
	BS1->Add(OkBtn, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
	NoBtn = new wxButton(this, ID_NOBTN, _("N"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_NOBTN"));
	NoBtn->SetLabel(FFQS(SID_COMMON_CANCEL));
	BS1->Add(NoBtn, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
	FGS1->Add(BS1, 1, wxALL|wxEXPAND, 5);
	SetSizer(FGS1);
	OpenFile = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	OpenFile->SetMessage(FFQS(SID_COMMON_SELECT_INPUT_FILE));
	SaveFile = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	SaveFile->SetMessage(FFQS(SID_COMMON_SELECT_OUTPUT_FILE));
	FGS1->Fit(this);
	FGS1->SetSizeHints(this);

	Connect(ID_BROWSESRC,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVideoToGIF::OnAction);
	Connect(ID_STARTTIME,wxEVT_COMMAND_HYPERLINK,(wxObjectEventFunction)&FFQVideoToGIF::OnAction);
	Connect(ID_LIMITLEN,wxEVT_COMMAND_HYPERLINK,(wxObjectEventFunction)&FFQVideoToGIF::OnAction);
	Connect(ID_BROWSEDST,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVideoToGIF::OnAction);
	Connect(ID_OKBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVideoToGIF::OnAction);
	Connect(ID_NOBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQVideoToGIF::OnAction);
	//*)


	m_PIP = NULL;
	Bind(wxEVT_IDLE, &FFQVideoToGIF::OnIdle, this);
	FileInfo->SetForegroundColour(wxColour(100,100,100));

	m_TimeEdit = NULL;
	SetTitle(FFQS(SID_VIDEO2GIF_TITLE));

	wxIntegerValidator<unsigned int> iVal(NULL, wxNUM_VAL_ZERO_AS_BLANK);
	iVal.SetRange(0, 10000);
	Width->SetValidator(iVal);
	Height->SetValidator(iVal);

	wxIntegerValidator<unsigned short> sVal(NULL, wxNUM_VAL_ZERO_AS_BLANK);
	sVal.SetRange(0, 100);
	FrameRate->SetValidator(sVal);

	FFQCFG()->SetBrowseRootFor(OpenFile);
	FFQCFG()->SetBrowseRootFor(SaveFile);

	FFQCFG()->SetCtrlColors(StartTime);
	FFQCFG()->SetCtrlColors(LimitLen);

}

//---------------------------------------------------------------------------------------

FFQVideoToGIF::~FFQVideoToGIF()
{

	if (m_TimeEdit)
    {
        delete m_TimeEdit;
        m_TimeEdit = NULL;
    }

	//(*Destroy(FFQVideoToGIF)
	//*)

}

//---------------------------------------------------------------------------------------

#define TEXTCTRLVAL(ctrl, stdval) (ctrl->GetValue().Len() == 0) ? 0 : Str2Long(ctrl->GetValue(), stdval)

bool FFQVideoToGIF::Execute(LPFFQ_VID2GIF_JOB job)
{

    //Check if we can perform two passes
	TwoPass->Enable(FFQCFG()->AreFiltersAvailable("palettegen,paletteuse"));

    //Load values from argument
    m_Job = job;
    m_StartTime = job->start_time;
    m_LimitLen = job->limit_len;

    if (job->inputs.Count() == 0)
    {

        //New job
        Source->Clear();// ChangeValue(job->GetInput(0).path);

    }

    else
    {

        //Edit job
        Source->ChangeValue(job->GetInput(0).path);

    }

    FrameRate->ChangeValue(job->fps == 0 ? "" : ToStr(job->fps));
    Width->ChangeValue(job->width == 0 ? "" : ToStr(job->width));
    Height->ChangeValue(job->height == 0 ? "" : ToStr(job->height));
    TwoPass->SetValue(TwoPass->IsEnabled() && job->two_pass);
    PreciseCuts->SetValue(job->precise_cuts);
    Dest->ChangeValue(job->out);
    SaveLog->SetValue(job->save_log);

    //Clear info for last probed file
    m_LastProbed.Clear();
	m_ProbeDuration = 0;
	m_ProbeW = 0;
	m_ProbeH = 0;
	m_ProbeFPS = 0;
	FileInfo->SetLabel(FFQS(SID_VIDEO2GIF_NO_VIDEO));

    //Update time links
    UpdateLinks();

    //Execute the dialog
    if (ShowModal() == wxID_OK)
    {

        //Save values to argument
        FFQ_INPUT_FILE inf;
        inf.Reset();
        inf.path = Source->GetValue();
        inf.width = m_ProbeW;
        inf.height = m_ProbeH;
        inf.duration = m_ProbeDuration;
        inf.framerate = wxString::Format("%.2f", m_ProbeFPS);

        //ShowInfo(inf.ToString());

        job->inputs.Clear();
        job->inputs.Add(inf.ToString());
        job->start_time = m_StartTime;
        job->limit_len = m_LimitLen;
        job->fps = TEXTCTRLVAL(FrameRate, job->fps);// (FrameRate->GetValue().Len() == 0) ? 0 : Str2Long(FrameRate->GetValue(), job->fps);
        job->width = TEXTCTRLVAL(Width, job->width);// (Width->GetValue().Len() == 0) ? 0 : Str2Long(Width->GetValue(), job->width);
        job->height = TEXTCTRLVAL(Height, job->height);// (Height->GetValue().Len() == 0) ? 0 : Str2Long(Height->GetValue(), job->height);
        job->two_pass = TwoPass->IsEnabled() && TwoPass->GetValue();
        job->precise_cuts = PreciseCuts->GetValue();
        job->out = Dest->GetValue();
        job->save_log = SaveLog->GetValue();

        //Return success
        return true;

    }

    //Return canceled
    return false;

}

//---------------------------------------------------------------------------------------

bool FFQVideoToGIF::EditTime(TIME_VALUE &t)
{

    //Edit time value and create editor if required
    if (m_TimeEdit == NULL) m_TimeEdit = new FFQTimeEdit(this);
    return m_TimeEdit->Execute(t, true);

}

//---------------------------------------------------------------------------------------

void FFQVideoToGIF::UpdateLinks()
{

    //Start time
    wxString s = FFQSF(SID_VIDEO2GIF_START_FROM, m_StartTime == 0 ? FFQS(SID_THE_BEGINNING) : m_StartTime.ToString());
    if (s != StartTime->GetLabel())
    {
        StartTime->SetLabel(s);
        StartTime->Refresh(); //2023
        TopSizer->Layout();
    }

    //Limit len
    s = FFQSF(SID_LIMIT_LENGTH, m_LimitLen == 0 ? FFQS(SID_NO_LIMIT) : m_LimitLen.ToString());
    if (s != LimitLen->GetLabel())
    {
        LimitLen->SetLabel(s);
        LimitLen->Refresh(); //2023
        MidSizer->Layout();
    }

}

//---------------------------------------------------------------------------------------

void FFQVideoToGIF::OnIdle(wxIdleEvent &event)
{

    if (m_PIP) return;

    wxString src = Source->GetValue();
    if ((m_LastProbed != src) && wxFileExists(src))
    {

        m_PIP = new FFProbeInfoParser();

        try
        {

            if (m_PIP->RunFFProbe(src))
            {

                if (!m_PIP->GetDuration(m_ProbeDuration)) m_ProbeDuration = 0;
                if (!m_PIP->GetVideoRate(m_ProbeFPS)) m_ProbeFPS = 0;
                if (!m_PIP->GetVideoDimension(m_ProbeW, m_ProbeH))
                {
                    m_ProbeW = 0;
                    m_ProbeH = 0;
                }

                FileInfo->SetLabel(FFQSF(SID_VIDEO2GIF_VIDEO_INFO, m_ProbeW, m_ProbeH, m_ProbeFPS, m_ProbeDuration.ToString()));

            }

            else FileInfo->SetLabel(FFQS(SID_VIDEO2GIF_NO_VIDEO));

            m_LastProbed = src;

        } catch (...) {}

        delete m_PIP;
        m_PIP = NULL;

    }

}

//---------------------------------------------------------------------------------------

bool BrowseFile(wxFileDialog *dlg, wxTextCtrl *ctl)
{

    //Helper used to browse for a file
    dlg->SetPath(ctl->GetValue());
    if (dlg->ShowModal() == wxID_OK)
    {
        ctl->ChangeValue(dlg->GetPath());
        return true;
    }
    return false;

}

void FFQVideoToGIF::OnAction(wxCommandEvent& event)
{

    //Process action event
    int evtId = event.GetId();

    if (evtId == ID_BROWSESRC)
    {

        //Browse source video
        if (BrowseFile(OpenFile, Source) && (Dest->GetValue().Len() == 0))
        {

            //Change value of Dest
            wxString s = Source->GetValue().BeforeLast('.');
            Dest->ChangeValue(s + ".gif");

        }

    }

    else if (evtId == ID_STARTTIME)
    {

        //Edit start time
        if (EditTime(m_StartTime)) UpdateLinks();

    }

    else if (evtId == ID_LIMITLEN)
    {

        //Edit limit length
        if (EditTime(m_LimitLen)) UpdateLinks();

    }

    //Browse destination file
    else if (evtId == ID_BROWSEDST) BrowseFile(SaveFile, Dest);

    else if (evtId == ID_OKBTN)
    {

        //Validate dialog, first check if input file exists
        if (!wxFileExists(Source->GetValue()))
        {
            ShowError(Source, FFQS(SID_INPUT_FILE_NOT_FOUND));
            return;
        }

        //Verify frame rate to a positive integer
        /*int f = Str2Long(FrameRate->GetValue(), -1);
        if (f <= 0)
        {
            ShowError(FrameRate, FFQS(SID_VIDEO2GIF_INVALID_FRAME_RATE));
            return;
        }*/

        //Verify the start time and length limit
        uint64_t vlen = m_ProbeDuration.ToMilliseconds();
        if ((vlen > 0) && (m_StartTime.ToMilliseconds() + m_LimitLen.ToMilliseconds() > vlen))
        {
            ShowError(FFQS(SID_VIDEO2GIF_BEYOND_END_OF_VID));
            return;
        }

        //All green
        EndModal(wxID_OK);

    }

    //Cancel dialog
    else if (evtId == ID_NOBTN) EndModal(wxID_CANCEL);

}
