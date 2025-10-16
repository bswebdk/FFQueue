/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQAbout.cpp                                                    *
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

#include "FFQAbout.h"
#include "utils/FFQLang.h"
#include "utils/FFQConfig.h"
#include "utils/FFQConst.h"
#include "utils/FFQMisc.h"
#include "bin_res.h"
#include "../version.h"

#include <wx/hyperlink.h>
#include <wx/mstream.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQAbout)
	#include <wx/string.h>
	#include <wx/intl.h>
	//*)
#endif
//(*InternalHeaders(FFQAbout)
#include <wx/font.h>
//*)

//(*IdInit(FFQAbout)
const long FFQAbout::ID_MAINLOGO = wxNewId();
const long FFQAbout::ID_ANIMPANEL = wxNewId();
const long FFQAbout::ID_MADCLIPPY = wxNewId();
const long FFQAbout::ID_TEXTCTRL = wxNewId();
const long FFQAbout::ID_HELPBUTTON = wxNewId();
const long FFQAbout::ID_CLOSEBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQAbout,wxDialog)
	//(*EventTable(FFQAbout)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

wxString ABOUT_APPINF, ABOUT_LANGX, ABOUT_WXGCC, ABOUT_FFMPEG,// ABOUT_4,
         GUI_HEADER, GUI_BRAG, GUI_VISIT, GUI_URL, GUI_BACON;

//---------------------------------------------------------------------------------------

const wxString BR = "\n";
#define BRBR ( BR+BR )

//---------------------------------------------------------------------------------------

void SetStaticBitmap(wxStaticBitmap *bmp, const unsigned char *data, int data_len)
{
	wxMemoryInputStream *ms = new wxMemoryInputStream(data, data_len);
	wxImage i(*ms, wxBITMAP_TYPE_PNG);
	wxBitmap b(i);
	bmp->SetBitmap(b);
	delete ms;
}
//---------------------------------------------------------------------------------------

FFQAbout::FFQAbout(wxWindow* parent,wxWindowID id)
{

	//Init STR_ABOUT
    wxString txt = ABOUT_KEY;
    STR_HASH key;
    HashString(txt, key);
    int len = STR_ABOUT_SIZE;//sizeof(STR_ABOUT);
    unsigned char *buf = new unsigned char[len];
    memcpy(buf, &STR_ABOUT, len);
    EncBuf((uint8_t*)buf, len, key);
    txt = wxString::FromUTF8((const char*)buf, len);
    //txt.Clear();
    //txt.Append(buf, len); //Not working, use next line
    //for (int i = 0; i < len; i++) txt += wxUniChar((int)buf[i]);
    if (FFQCFG()->use_libav) FFQL()->LibAVify(txt);

    GUI_HEADER = GetToken(txt, BR);
    GUI_BRAG = GetToken(txt, BR);
    GUI_VISIT = GetToken(txt, BR);
    GUI_URL = GetToken(txt, BR);
    GUI_BACON = GetToken(txt, BRBR);

    ABOUT_APPINF = GetToken(txt, BRBR);
    ABOUT_LANGX = GetToken(txt, BRBR);
    ABOUT_WXGCC = GetToken(txt, BRBR);
    ABOUT_FFMPEG = GetToken(txt, BRBR);
    //ABOUT_4 = GetToken(txt, BRBR);

	//(*Initialize(FFQAbout)
	wxBoxSizer* BS2;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxGenericHyperlinkCtrl* Link;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	FlexGridSizer1->AddGrowableRow(1);
	FlexGridSizer1->AddGrowableRow(2);
	FlexGridSizer2 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer2->AddGrowableCol(1);
	FlexGridSizer2->AddGrowableRow(0);
	MainLogo = new wxStaticBitmap(this, ID_MAINLOGO, wxNullBitmap, wxDefaultPosition, wxSize(100,100), wxBORDER_NONE, _T("ID_MAINLOGO"));
	FlexGridSizer2->Add(MainLogo, 0, wxALL|wxALIGN_TOP, 5);
	FlexGridSizer3 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	FlexGridSizer3->AddGrowableRow(0);
	FlexGridSizer3->AddGrowableRow(1);
	FlexGridSizer7 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer7->AddGrowableCol(0);
	FlexGridSizer7->AddGrowableRow(0);
	Hdr = new wxStaticText(this, wxID_ANY, _("ffq"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	wxFont HdrFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	Hdr->SetFont(HdrFont);
	Hdr->SetLabel(GUI_HEADER);
	FlexGridSizer7->Add(Hdr, 0, wxALL|wxEXPAND, 5);
	AnimPanel = new wxPanel(this, ID_ANIMPANEL, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTAB_TRAVERSAL, _T("ID_ANIMPANEL"));
	AnimPanel->SetMinSize(wxSize(37,28));
	FlexGridSizer7->Add(AnimPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer4 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	FlexGridSizer4->AddGrowableRow(0);
	BS2 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer8 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer8->AddGrowableCol(0);
	Info = new wxStaticText(this, wxID_ANY, _("gui"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	Info->SetLabel(GUI_BRAG);
	FlexGridSizer8->Add(Info, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Link = new wxGenericHyperlinkCtrl(this, wxID_ANY, _("www"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE, _T("wxID_ANY"));
	Link->SetLabel(GUI_VISIT);
	Link->SetURL(GUI_URL);
	FlexGridSizer8->Add(Link, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BS2->Add(FlexGridSizer8, 1, wxALL|wxEXPAND, 0);
	Bacon = new wxStaticText(this, wxID_ANY, _("100%"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	Bacon->SetLabel(GUI_BACON);
	BS2->Add(Bacon, 1, wxALL, 5);
	FlexGridSizer4->Add(BS2, 1, wxALL|wxEXPAND, 0);
	MadClippy = new wxStaticBitmap(this, ID_MADCLIPPY, wxNullBitmap, wxDefaultPosition, wxSize(32,48), 0, _T("ID_MADCLIPPY"));
	FlexGridSizer4->Add(MadClippy, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer6 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer6->AddGrowableCol(0);
	FlexGridSizer6->AddGrowableRow(0);
	TextCtrl = new wxTextCtrl(this, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDLG_UNIT(this,wxSize(-1,120)), wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxBORDER_STATIC, wxDefaultValidator, _T("ID_TEXTCTRL"));
	FlexGridSizer6->Add(TextCtrl, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
	FlexGridSizer1->Add(FlexGridSizer6, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	HelpButton = new wxButton(this, ID_HELPBUTTON, _("H"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_HELPBUTTON"));
	HelpButton->SetLabel(FFQS(SID_COMMON_HELP));
	BoxSizer1->Add(HelpButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	CloseButton = new wxButton(this, ID_CLOSEBUTTON, _("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLOSEBUTTON"));
	CloseButton->SetDefault();
	CloseButton->SetLabel(FFQS(SID_COMMON_CLOSE));
	BoxSizer1->Add(CloseButton, 1, wxALL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_HELPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQAbout::OnButtonCloseClick);
	Connect(ID_CLOSEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQAbout::OnButtonCloseClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&FFQAbout::OnClose);
	//*)

	#ifdef DEBUG
	Timer.SetOwner(this, wxID_ANY);
	Connect(wxID_ANY, wxEVT_TIMER, (wxObjectEventFunction)&FFQAbout::OnTimer);
	if (Timer.IsRunning()) Timer.Stop();
	#endif // DEBUG

	SetTitle(FFQS(SID_ABOUT_TITLE) + SPACE + GUI_HEADER);
	wxMemoryInputStream *ms = new wxMemoryInputStream(&FLAG_ANIM, FLAG_ANIM_SIZE);
	wxAnimation anm;
	anm.Load(*ms, wxANIMATION_TYPE_GIF);
	m_Anim = new wxAnimationCtrl(AnimPanel, wxID_ANY, anm, wxDefaultPosition, wxDefaultSize, wxAC_DEFAULT_STYLE);
	//m_Anim->SetToolTip(GUI_URL);
	m_Anim->SetAnimation(anm);
	//m_Anim->Play(true);
	m_Anim->Play();
	delete ms;

    SetStaticBitmap(MainLogo, MAIN_LOGO, MAIN_LOGO_SIZE);
    SetStaticBitmap(MadClippy, MAD_CLIPPY, MAD_CLIPPY_SIZE);

	/*ms = new wxMemoryInputStream(&MAIN_LOGO, MAIN_LOGO_SIZE);
	wxImage img(*ms, wxBITMAP_TYPE_PNG);
	//img.LoadFile(*ms, wxBITMAP_TYPE_PNG);
	wxBitmap bmp(img);
	MainLogo->SetBitmap(bmp);
	delete ms;

	ms = new wxMemoryInputStream(&MAD_CLIPPY, MAD_CLIPPY_SIZE);
	wxImage img(*ms, wxBITMAP_TYPE_PNG);
	img.LoadFile(*ms, wxBITMAP_TYPE_PNG);
	wxBitmap bmp(img);
	MadClippy->SetBitmap(bmp);
	delete ms;*/

	FFQCFG()->SetCtrlColors(Link);

}

//---------------------------------------------------------------------------------------

FFQAbout::~FFQAbout()
{
	//(*Destroy(FFQAbout)
	//*)

	//Delete the exported help-file for the snap version
	if ((FFQCFG()->is_snap || FFQCFG()->is_flat) && wxFileExists(m_HelpPath)) wxRemoveFile(m_HelpPath);
}

//---------------------------------------------------------------------------------------

const int color_spring = 0xAAFFAA; // 0xDDFFDD; //Full at 15 April
const int color_summer = 0x88FFFF; // 0xAAFFFF; //Full at 15 July
const int color_fall = 0x79B7E9; // 0x55AFFC; //Full at 15 October
const int color_winter = 0xEDEDED; //0xEEEEEE; //Full at 15 December

//---------------------------------------------------------------------------------------

int FadeByte(int from, int to, float pct)
{

    int res = from + int( float(to - from) * pct);
    if (res < 0) res = 0;
    else if (res > 255) res = 255;

    //ShowInfo(wxString::Format("from=%x to=%x res=%x", from, to, res));

    return res;

}

//---------------------------------------------------------------------------------------

int FadeColor(int from, int to, float pct)
{

    int res = 0;
    res |= FadeByte(from & 0xFF, to & 0xFF, pct);
    res |= (FadeByte((from & 0xFF00) >> 8, (to & 0xFF00) >> 8, pct) << 8);
    res |= (FadeByte((from & 0xFF0000) >> 16, (to & 0xFF0000) >> 16, pct) << 16);
    return res;

}

//---------------------------------------------------------------------------------------

int GetSeasonColor(int day_of_year)
{

    wxDateTime now = wxDateTime::Now();
    int y = now.GetYear(), diy = 365, feb = 28;
    if ( (y % 4 == 0) && ((y % 100 != 0) || (y % 400 == 0)))
    {
        diy++;
        feb++;
    }

    int apr15 = 31 + feb + 31 + 15,
        jul15 = apr15 + 15 + 31 + 30 + 15,
        oct15 = jul15 + 16 + 31 + 30 + 15,
        dec15 = oct15 + 16 + 30 + 15,
        c1, c2;

    float fade;

    if ((day_of_year >= apr15) && (day_of_year < jul15))
    {
        //spring -> summer
        c1 = color_spring;
        c2 = color_summer;
        fade = (day_of_year == apr15) ? 0.0f : (float)(day_of_year-apr15) / float(jul15-apr15);
    }
    else if ((day_of_year >= jul15) && (day_of_year < oct15))
    {
        //summer -> fall
        c1 = color_summer;
        c2 = color_fall;
        fade = (day_of_year == jul15) ? 0.0f : (float)(day_of_year-jul15) / float(oct15-jul15);
    }
    else if ((day_of_year >= oct15) && (day_of_year < dec15))
    {
        //fall -> winter
        c1 = color_fall;
        c2 = color_winter;
        fade = (day_of_year == oct15) ? 0.0f : (float)(day_of_year-oct15) / float(dec15-oct15);
    }
    else
    {
        //winter -> spring
        c1 = color_winter;
        c2 = color_spring;
        if (day_of_year == dec15) fade = 0.0f;
        else
        {
            if (day_of_year < dec15) day_of_year += diy;
            fade = (float)(day_of_year + 16 - diy) / (apr15 + 16);
        }
    }

    return FadeColor(c1, c2, fade);

}

//---------------------------------------------------------------------------------------

void FFQAbout::Execute()
{

    //Get help path and enable button accordingly
    m_HelpPath = FFQCFG()->GetHelpPath();
    HelpButton->Enable(m_HelpPath.Len() > 0);

    //Set color of the text control to suit the time of the year
    //TextCtrl->SetBackgroundColour(GetSeasonColor(wxDateTime::Now().GetDayOfYear()));
    TextCtrl->Clear();
    wxString txt, s;

    //Language percentages
    long pct = (long)((double)FFQL()->GetFlagCount(SF_TRANSLATED) / (double)FFQL()->GetCount() * 100.0);
    if ((pct > 0) && (pct < 100)) s = wxString::Format(ABOUT_LANGX, (unsigned int)(100 - pct), (unsigned int)pct);

    //Info about programmer, version and language
    TextCtrl->SetDefaultStyle(wxTextAttr(0xAA0000));
    TextCtrl->AppendText(wxString::Format(ABOUT_APPINF, AutoVersion::FULLVERSION_STRING, s, FFQL()->GetDescription() + " - " + FFQL()->GetFFQVersion(), FFQCFG()->GetConfigPath()));

    //Info about environment and compiler
    s = wxString::Format("%i.%i.%i", wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);
    TextCtrl->SetDefaultStyle(wxTextAttr(0x008800));
    TextCtrl->AppendText(CRLF + CRLF + wxString::Format(ABOUT_WXGCC, s, __VERSION__));

    //Info about ffmpeg (if available)
    if (FFQCFG()->ffmpeg_ok)
    {
        TextCtrl->SetDefaultStyle(wxTextAttr(0xAAAA00));
        TextCtrl->AppendText(CRLF + CRLF + wxString::Format(ABOUT_FFMPEG, FFQCFG()->GetFFMpegVersion(false)));
    }

    //Info about graphics
    //TextCtrl->SetDefaultStyle(wxTextAttr(0x555555));
    //TextCtrl->AppendText(CRLF + CRLF + ABOUT_4);

    TextCtrl->SetSelection(0, 0);
    TextCtrl->ShowPosition(0);
    CenterOnParent();

    bool tbs = FFQCFG()->GetTaskBar()->GetTaskBarState() == tbsNORMAL; //Fun stuff
    if (tbs) FFQCFG()->GetTaskBar()->SetTaskBarState(tbsMARQUEE); //Fun stuff

    #ifdef DEBUG
    cc = 1;
    Timer.Start(25);
    #endif // DEBUG

    ShowModal();

    #ifdef DEBUG
    Timer.Stop();
    #endif // DEBUG

    if (tbs) FFQCFG()->GetTaskBar()->SetTaskBarState(tbsNORMAL); //Fun stuff

}

//---------------------------------------------------------------------------------------

void FFQAbout::OnButtonCloseClick(wxCommandEvent& event)
{
    if (event.GetId() == ID_HELPBUTTON) wxLaunchDefaultBrowser("file://" + m_HelpPath);// Application(m_HelpPath);
    else EndModal(0);
}

//---------------------------------------------------------------------------------------

void FFQAbout::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}

//---------------------------------------------------------------------------------------

#ifdef DEBUG
void FFQAbout::OnTimer(wxTimerEvent& event)
{
    //int sc = GetSeasonColor(cc);
    //TextCtrl->SetBackgroundColour(sc);

    /*wxTextAttr ds;
    if (TextCtrl->GetStyle(1, ds))
    {
      TextCtrl->GetLi
      ds.SetBackgroundColour(sc);
      TextCtrl->SetStyle(1, ds);
    }*/

    //cc++;
    //if (cc > 365) cc = 1;
}
#endif // DEBUG
