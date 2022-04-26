/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConcat.cpp                                                   *
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

#include "FFQConcat.h"
#include "../utils/FFQLang.h"
#include "../FFQTimeEdit.h"
#include "../utils/FFQConfig.h"
#include "../utils/FFQBuildCmd.h"
#include "../utils/FFQMisc.h"
#include "../utils/FFQConst.h"
#include "../utils/FFQConsole.h"

#include <wx/filename.h>
#include <wx/dir.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQConcat)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQConcat)
//*)

#define LIST_EXT ".txt"
#define DEFAULT_WIDTH "800"
#define DEFAULT_HEIGHT "600"

//---------------------------------------------------------------------------------------

const wxString FILTER_FIT_AND_PAD = "scale='if(gt(iw,ih),$W,-1)':'if(gt(ih,iw),$H,-1)',pad=$W:$H:(ow-iw)/2:(oh-ih)/2:$C";
const wxString FILTER_KEY1 = "$F1$";
const wxString FILTER_KEY2 = "$F2$";

//---------------------------------------------------------------------------------------

//size_t TestFileRange(wxString &path, wxString &name, size_t start_num);
//size_t FilePathToRange(wxString &path, size_t &start_num);

//---------------------------------------------------------------------------------------

unsigned int TestFileRange(wxString &path, wxString &name, unsigned int start_num)
{
    //Return how many files are in the suggested range
    unsigned int res = 0;
    while (wxFileExists(path + wxString::Format(name, start_num))) { start_num++; res++; }
    return res;
}

//---------------------------------------------------------------------------------------

const wxString NUMS = "0123456789";

//---------------------------------------------------------------------------------------

unsigned int FilePathToRange(wxString &path, unsigned int &start_num)
{

    //Convert first file name in a range to a number pattern: %03d etc.
    wxString s = path.AfterLast(wxFileName::GetPathSeparator()), t, pat;
    path = path.BeforeLast(wxFileName::GetPathSeparator()) + wxFileName::GetPathSeparator();
    unsigned int idx = 0, nStart, nEnd, nLen;

    while (idx < s.Len())
    {

        //Test for a numeric value
        if (NUMS.Find(s.GetChar(idx)) >= 0)
        {

            //Find out how many digits the number is
            nStart = idx;
            nEnd = idx + 1;
            while ((NUMS.Find(s.GetChar(nEnd)) >= 0) && (nEnd < s.Len())) nEnd++;

            //Convert the number string to decimal value
            start_num = Str2Long(s.SubString(nStart, nEnd - 1), 0);

            //Create the number pattern
            pat = "%";
            nLen = nEnd - nStart;
            if (nLen > 1) pat += "0" + ToStr(nLen);
            pat += "d";

            //Create the file name with number pattern
            if (nStart > 0) t = s.SubString(0, nStart - 1);
            else t = "";
            t += pat + s.SubString(nEnd, s.Len());

            //Test the file name with pattern
            nLen = TestFileRange(path, t, start_num);
            if (nLen > 1)
            {
                //More than one file found = success
                path += t;
                return nLen;
            }

            //Try the next number in the string
            idx = nEnd;

        }

        //Go to next character in string
        else idx++;

    }

    //Oh my god! We have failed to find more than one!
    path += s;
    return 1;

}

//---------------------------------------------------------------------------------------

wxString ConcatDataToString(LPCONCAT_DATA cd, bool SimpleConcat)
{
    wxString fn = cd->path.AfterLast(wxFileName::GetPathSeparator()),
             dur = ((cd->custDuration != 0) && SimpleConcat) ? cd->custDuration.ToString() : cd->duration.ToString();
    if (cd->width > 0) return FFQSF(SID_CONCAT_VIDEO_ITEM, fn, cd->width, cd->height, dur);
    return FFQSF(SID_CONCAT_AUDIO_ITEM, fn, dur);
}

//---------------------------------------------------------------------------------------

//(*IdInit(FFQConcat)
const long FFQConcat::ID_ST1 = wxNewId();
const long FFQConcat::ID_IMGSRC = wxNewId();
const long FFQConcat::ID_SSBROWSEIMG = wxNewId();
const long FFQConcat::ID_SSFRAMESTATUS = wxNewId();
const long FFQConcat::ID_SSFIT = wxNewId();
const long FFQConcat::ID_ST2 = wxNewId();
const long FFQConcat::ID_TEXTCTRL1 = wxNewId();
const long FFQConcat::ID_ST22 = wxNewId();
const long FFQConcat::ID_TEXTCTRL2 = wxNewId();
const long FFQConcat::ID_ST3 = wxNewId();
const long FFQConcat::ID_SSPADDING = wxNewId();
const long FFQConcat::ID_ST4 = wxNewId();
const long FFQConcat::ID_ST5 = wxNewId();
const long FFQConcat::ID_SSFRAMETIME = wxNewId();
const long FFQConcat::ID_ST6 = wxNewId();
const long FFQConcat::ID_SSSETPTS = wxNewId();
const long FFQConcat::ID_SSAUDIO = wxNewId();
const long FFQConcat::ID_SSBROWSEAUDIO = wxNewId();
const long FFQConcat::ID_SSLOOPFRAMES = wxNewId();
const long FFQConcat::ID_SLIDESHOWPAGE = wxNewId();
const long FFQConcat::ID_CCSOURCES = wxNewId();
const long FFQConcat::ID_CCADD = wxNewId();
const long FFQConcat::ID_CCREMOVE = wxNewId();
const long FFQConcat::ID_CCUP = wxNewId();
const long FFQConcat::ID_CCDOWN = wxNewId();
const long FFQConcat::ID_CCSIMPLE = wxNewId();
const long FFQConcat::ID_CCEXPLICITMAP = wxNewId();
const long FFQConcat::ID_CCSETSAR = wxNewId();
const long FFQConcat::ID_CCPADDING = wxNewId();
const long FFQConcat::ID_CCUSEPADCOLOR = wxNewId();
const long FFQConcat::ID_CCPADCOLOR = wxNewId();
const long FFQConcat::ID_CCUSEPADBLUR = wxNewId();
const long FFQConcat::ID_STATICTEXT1 = wxNewId();
const long FFQConcat::ID_CCPADBLUR = wxNewId();
const long FFQConcat::ID_STATICTEXT2 = wxNewId();
const long FFQConcat::ID_ST9 = wxNewId();
const long FFQConcat::ID_CCVideo = wxNewId();
const long FFQConcat::ID_CCAUDIO = wxNewId();
const long FFQConcat::ID_CCSUBTITLES = wxNewId();
const long FFQConcat::ID_MERGEPAGE = wxNewId();
const long FFQConcat::ID_PAGES = wxNewId();
const long FFQConcat::ID_ST7 = wxNewId();
const long FFQConcat::ID_DESTFILE = wxNewId();
const long FFQConcat::ID_BROWSEDEST = wxNewId();
const long FFQConcat::ID_ST8 = wxNewId();
const long FFQConcat::ID_PRESET = wxNewId();
const long FFQConcat::ID_LIMITDEST = wxNewId();
const long FFQConcat::ID_SAVELOG = wxNewId();
const long FFQConcat::ID_OKBUTTON = wxNewId();
const long FFQConcat::ID_CANCELBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQConcat,wxDialog)
	//(*EventTable(FFQConcat)
	//*)
END_EVENT_TABLE()

FFQConcat::FFQConcat(wxWindow* parent)
{
	//(*Initialize(FFQConcat)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer12;
	wxFlexGridSizer* FlexGridSizer13;
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
	wxStaticBoxSizer* SBS2;
	wxStaticBoxSizer* SBS3;
	wxStaticBoxSizer* SBS4;

	Create(parent, wxID_ANY, _T("Make slideshow / concat"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	Pages = new wxNotebook(this, ID_PAGES, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PAGES"));
	SlideshowPage = new wxPanel(Pages, ID_SLIDESHOWPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_SLIDESHOWPAGE"));
	FlexGridSizer2 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	SBS1 = new wxStaticBoxSizer(wxVERTICAL, SlideshowPage, _T("Src"));
	FlexGridSizer5 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer5->AddGrowableCol(0);
	Sizer1 = new wxFlexGridSizer(2, 3, 0, 0);
	Sizer1->AddGrowableCol(1);
	Sizer1->AddGrowableRow(0);
	ST1 = new wxStaticText(SlideshowPage, ID_ST1, _T("Frst"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST1"));
	ST1->SetLabel(FFQS(SID_CONCAT_FIRST_IMAGE));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_CONCAT_SOURCE_FRAMES));
	Sizer1->Add(ST1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SSSource = new wxTextCtrl(SlideshowPage, ID_IMGSRC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_IMGSRC"));
	Sizer1->Add(SSSource, 1, wxALL|wxEXPAND, 3);
	SSBrowseImg = new wxButton(SlideshowPage, ID_SSBROWSEIMG, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSBROWSEIMG"));
	Sizer1->Add(SSBrowseImg, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	Sizer1->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	SSFrameStatus = new wxGenericHyperlinkCtrl(SlideshowPage, ID_SSFRAMESTATUS, _T("link"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE, _T("ID_SSFRAMESTATUS"));
	Sizer1->Add(SSFrameStatus, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Sizer1->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	FlexGridSizer5->Add(Sizer1, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer13 = new wxFlexGridSizer(5, 1, 0, 0);
	FlexGridSizer13->AddGrowableCol(0);
	SSFit = new wxCheckBox(SlideshowPage, ID_SSFIT, _T("SaP"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSFIT"));
	SSFit->SetValue(false);
	SSFit->SetLabel(FFQS(SID_CONCAT_SCALE_AND_PAD));
	FlexGridSizer13->Add(SSFit, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SSSizer1 = new wxFlexGridSizer(1, 7, 0, 0);
	SSSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ST2 = new wxStaticText(SlideshowPage, ID_ST2, _T("Si"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST2"));
	ST2->SetLabel(FFQS(SID_CONCAT_SIZE));
	SSSizer1->Add(ST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SSWidth = new wxTextCtrl(SlideshowPage, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	SSSizer1->Add(SSWidth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ST22 = new wxStaticText(SlideshowPage, ID_ST22, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST22"));
	SSSizer1->Add(ST22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SSHeight = new wxTextCtrl(SlideshowPage, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	SSSizer1->Add(SSHeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ST3 = new wxStaticText(SlideshowPage, ID_ST3, _T("Fi"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST3"));
	ST3->SetLabel(FFQS(SID_CONCAT_FILL_COLOR));
	SSSizer1->Add(ST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SSPadding = new wxColourPickerCtrl(SlideshowPage, ID_SSPADDING, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSPADDING"));
	SSSizer1->Add(SSPadding, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer13->Add(SSSizer1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ST4 = new wxStaticText(SlideshowPage, ID_ST4, _T("Inf"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST4"));
	ST4->Disable();
	ST4->SetLabel(FFQS(SID_CONCAT_SCALE_INFORMATION));
	FlexGridSizer13->Add(ST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7 = new wxFlexGridSizer(1, 3, 0, 0);
	ST5 = new wxStaticText(SlideshowPage, ID_ST5, _T("Ft"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST5"));
	ST5->SetLabel(FFQS(SID_CONCAT_FRAME_TIME));
	FlexGridSizer7->Add(ST5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SSFrameTime = new wxTextCtrl(SlideshowPage, ID_SSFRAMETIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSFRAMETIME"));
	FlexGridSizer7->Add(SSFrameTime, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ST6 = new wxStaticText(SlideshowPage, ID_ST6, _T("s"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST6"));
	ST6->SetLabel(FFQS(SID_CONCAT_SECONDS));
	FlexGridSizer7->Add(ST6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer13->Add(FlexGridSizer7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SSSetPTS = new wxCheckBox(SlideshowPage, ID_SSSETPTS, _T("FPTS"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSSETPTS"));
	SSSetPTS->SetValue(false);
	SSSetPTS->SetLabel(FFQS(SID_CONCAT_FORCE_PTS));
	FlexGridSizer13->Add(SSSetPTS, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer13, 1, wxALL|wxEXPAND, 3);
	SBS1->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer2->Add(SBS1, 1, wxALL|wxEXPAND, 5);
	SBS2 = new wxStaticBoxSizer(wxVERTICAL, SlideshowPage, _T("At"));
	FlexGridSizer10 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer10->AddGrowableCol(0);
	SSAudio = new wxTextCtrl(SlideshowPage, ID_SSAUDIO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSAUDIO"));
	FlexGridSizer10->Add(SSAudio, 1, wxALL|wxEXPAND, 3);
	SSBrowseAudio = new wxButton(SlideshowPage, ID_SSBROWSEAUDIO, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSBROWSEAUDIO"));
	FlexGridSizer10->Add(SSBrowseAudio, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS2->Add(FlexGridSizer10, 1, wxALL|wxEXPAND, 0);
	SSLoopFrames = new wxCheckBox(SlideshowPage, ID_SSLOOPFRAMES, _T("Loop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SSLOOPFRAMES"));
	SSLoopFrames->SetValue(false);
	SSLoopFrames->SetLabel(FFQS(SID_CONCAT_LOOP_FRAMES));
	SBS2->GetStaticBox()->SetLabel(FFQS(SID_CONCAT_AUDIO_TRACK));
	SBS2->Add(SSLoopFrames, 1, wxALL|wxALIGN_LEFT, 5);
	FlexGridSizer2->Add(SBS2, 1, wxALL|wxEXPAND, 5);
	SlideshowPage->SetSizer(FlexGridSizer2);
	FlexGridSizer2->Fit(SlideshowPage);
	FlexGridSizer2->SetSizeHints(SlideshowPage);
	MergePage = new wxPanel(Pages, ID_MERGEPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_MERGEPAGE"));
	FlexGridSizer9 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer9->AddGrowableCol(0);
	FlexGridSizer9->AddGrowableRow(0);
	SBS4 = new wxStaticBoxSizer(wxHORIZONTAL, MergePage, _T("Src"));
	FlexGridSizer4 = new wxFlexGridSizer(7, 1, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	FlexGridSizer4->AddGrowableRow(0);
	FlexGridSizer3 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	FlexGridSizer3->AddGrowableRow(0);
	CCSources = new wxListBox(MergePage, ID_CCSOURCES, wxDefaultPosition, wxSize(300,-1), 0, 0, wxLB_EXTENDED|wxLB_HSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_CCSOURCES"));
	FlexGridSizer3->Add(CCSources, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer12 = new wxFlexGridSizer(6, 1, 0, 0);
	FlexGridSizer12->AddGrowableCol(0);
	FlexGridSizer12->AddGrowableRow(5);
	CCAdd = new wxButton(MergePage, ID_CCADD, _T("A"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCADD"));
	CCAdd->SetLabel(FFQS(SID_COMMON_ADD));
	SBS4->GetStaticBox()->SetLabel(FFQS(SID_CONCAT_SOURCE_FILES));
	FlexGridSizer12->Add(CCAdd, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CCRemove = new wxButton(MergePage, ID_CCREMOVE, _T("R"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCREMOVE"));
	CCRemove->SetLabel(FFQS(SID_COMMON_REMOVE));
	FlexGridSizer12->Add(CCRemove, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer12->Add(-1,10,1, wxALL|wxEXPAND, 0);
	CCUp = new wxButton(MergePage, ID_CCUP, _T("U"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCUP"));
	CCUp->SetLabel(FFQS(SID_COMMON_MOVE_UP));
	FlexGridSizer12->Add(CCUp, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CCDown = new wxButton(MergePage, ID_CCDOWN, _T("D"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCDOWN"));
	CCDown->SetLabel(FFQS(SID_COMMON_MOVE_DOWN));
	FlexGridSizer12->Add(CCDown, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer3->Add(FlexGridSizer12, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer4->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 0);
	CCSimple = new wxCheckBox(MergePage, ID_CCSIMPLE, _T("SC"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCSIMPLE"));
	CCSimple->SetValue(false);
	CCSimple->SetLabel(FFQS(SID_CONCAT_SIMPLE));
	FlexGridSizer4->Add(CCSimple, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer6 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer6->AddGrowableCol(1);
	FlexGridSizer6->AddGrowableRow(0);
	FlexGridSizer6->Add(20,10,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CCExplicitMap = new wxCheckBox(MergePage, ID_CCEXPLICITMAP, _T("XM"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCEXPLICITMAP"));
	CCExplicitMap->SetValue(false);
	CCExplicitMap->SetLabel(FFQS(SID_CONCAT_EXPLICIT_MAP));
	FlexGridSizer6->Add(CCExplicitMap, 1, wxALL, 3);
	FlexGridSizer4->Add(FlexGridSizer6, 1, wxALL|wxEXPAND, 0);
	CCSetSar = new wxCheckBox(MergePage, ID_CCSETSAR, _T("FSAR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCSETSAR"));
	CCSetSar->SetValue(false);
	CCSetSar->SetLabel(FFQS(SID_CONCAT_FORCE_ASPECT));
	FlexGridSizer4->Add(CCSetSar, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	CCPadding = new wxCheckBox(MergePage, ID_CCPADDING, _T("Pad"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCPADDING"));
	CCPadding->SetValue(false);
	CCPadding->SetLabel(FFQS(SID_CONCAT_PADDING));
	FlexGridSizer4->Add(CCPadding, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	CCPadSizer = new wxFlexGridSizer(2, 3, 0, 0);
	CCPadSizer->AddGrowableCol(2);
	CCPadSizer->Add(20,10,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CCUsePadColor = new wxRadioButton(MergePage, ID_CCUSEPADCOLOR, _T("PadC"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCUSEPADCOLOR"));
	CCUsePadColor->SetLabel(FFQS(SID_PAD_COLOR));
	CCPadSizer->Add(CCUsePadColor, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	CCPadColor = new wxColourPickerCtrl(MergePage, ID_CCPADCOLOR, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCPADCOLOR"));
	CCPadSizer->Add(CCPadColor, 1, wxALL|wxEXPAND, 0);
	CCPadSizer->Add(20,10,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CCUsePadBlur = new wxRadioButton(MergePage, ID_CCUSEPADBLUR, _T("PadB"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCUSEPADBLUR"));
	CCUsePadBlur->SetLabel(FFQS(SID_PAD_BLUR));
	CCPadSizer->Add(CCUsePadBlur, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	CCBlurSizer = new wxFlexGridSizer(1, 3, 0, 0);
	CCBlurSizer->AddGrowableCol(1);
	StaticText1 = new wxStaticText(MergePage, ID_STATICTEXT1, _T("-"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	CCBlurSizer->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CCPadBlur = new wxSlider(MergePage, ID_CCPADBLUR, 2, 1, 10, wxDefaultPosition, wxSize(-1,20), 0, wxDefaultValidator, _T("ID_CCPADBLUR"));
	CCBlurSizer->Add(CCPadBlur, 1, wxALL|wxEXPAND, 0);
	StaticText2 = new wxStaticText(MergePage, ID_STATICTEXT2, _T("+"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	CCBlurSizer->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CCPadSizer->Add(CCBlurSizer, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer4->Add(CCPadSizer, 1, wxALL|wxEXPAND, 0);
	Sizer4 = new wxFlexGridSizer(0, 4, 0, 0);
	ST9 = new wxStaticText(MergePage, ID_ST9, _T("St"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST9"));
	ST9->SetLabel(FFQS(SID_CONCAT_CONTENT_TYPES));
	Sizer4->Add(ST9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CCVideo = new wxCheckBox(MergePage, ID_CCVideo, _T("V"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCVideo"));
	CCVideo->SetValue(true);
	CCVideo->SetLabel(FFQS(SID_COMMON_VIDEO));
	Sizer4->Add(CCVideo, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CCAudio = new wxCheckBox(MergePage, ID_CCAUDIO, _T("A"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCAUDIO"));
	CCAudio->SetValue(true);
	CCAudio->SetLabel(FFQS(SID_COMMON_AUDIO));
	Sizer4->Add(CCAudio, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	CCSubtitles = new wxCheckBox(MergePage, ID_CCSUBTITLES, _T("S"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CCSUBTITLES"));
	CCSubtitles->SetValue(false);
	CCSubtitles->Disable();
	CCSubtitles->SetLabel(FFQS(SID_COMMON_SUBTITLES));
	Sizer4->Add(CCSubtitles, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(Sizer4, 1, wxALL|wxEXPAND, 0);
	SBS4->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer9->Add(SBS4, 1, wxALL|wxEXPAND, 5);
	MergePage->SetSizer(FlexGridSizer9);
	FlexGridSizer9->Fit(MergePage);
	FlexGridSizer9->SetSizeHints(MergePage);
	Pages->AddPage(SlideshowPage, _T("SS"), false);
	Pages->AddPage(MergePage, _T("CC"), false);
	FlexGridSizer1->Add(Pages, 1, wxALL|wxEXPAND, 5);
	SBS3 = new wxStaticBoxSizer(wxVERTICAL, this, _T("Dst"));
	FlexGridSizer8 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer8->AddGrowableCol(1);
	FlexGridSizer8->AddGrowableRow(0);
	ST7 = new wxStaticText(this, ID_ST7, _T("Fn"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST7"));
	ST7->SetLabel(FFQS(SID_CONCAT_FILE_NAME));
	SBS3->GetStaticBox()->SetLabel(FFQS(SID_CONCAT_DESTINATION));
	FlexGridSizer8->Add(ST7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	DestFile = new wxTextCtrl(this, ID_DESTFILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DESTFILE"));
	FlexGridSizer8->Add(DestFile, 1, wxALL|wxEXPAND, 3);
	BrowseDest = new wxButton(this, ID_BROWSEDEST, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BROWSEDEST"));
	FlexGridSizer8->Add(BrowseDest, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SBS3->Add(FlexGridSizer8, 1, wxBOTTOM|wxEXPAND, 2);
	Sizer3 = new wxFlexGridSizer(1, 2, 0, 0);
	Sizer3->AddGrowableCol(1);
	Sizer3->AddGrowableRow(0);
	ST8 = new wxStaticText(this, ID_ST8, _T("Pst"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST8"));
	ST8->SetLabel(FFQS(SID_CONCAT_PRESET));
	Sizer3->Add(ST8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	Preset = new FFQPresetPanel(this);
	Sizer3->Add(Preset, 1, wxALL|wxEXPAND, 3);
	SBS3->Add(Sizer3, 1, wxBOTTOM|wxEXPAND, 2);
	Sizer2 = new wxFlexGridSizer(1, 1, 0, 0);
	Sizer2->AddGrowableCol(0);
	Sizer2->AddGrowableRow(0);
	LimitDest = new wxGenericHyperlinkCtrl(this, ID_LIMITDEST, _T("limit"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE, _T("ID_LIMITDEST"));
	Sizer2->Add(LimitDest, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SBS3->Add(Sizer2, 1, wxBOTTOM|wxEXPAND, 2);
	SaveLog = new wxCheckBox(this, ID_SAVELOG, _T("Log"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SAVELOG"));
	SaveLog->SetValue(false);
	SaveLog->SetLabel(FFQS(SID_COMMON_SAVE_LOG));
	SBS3->Add(SaveLog, 1, wxALL|wxALIGN_LEFT, 5);
	FlexGridSizer1->Add(SBS3, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OkButton = new wxButton(this, ID_OKBUTTON, _T("O"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBUTTON"));
	OkButton->SetDefault();
	OkButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer1->Add(OkButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer1->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	OpenOneDlg = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	OpenOneDlg->SetMessage(FFQS(SID_COMMON_SELECT_INPUT_FILE));
	SaveFileDlg = new wxFileDialog(this, _T("Select destination file"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	SaveFileDlg->SetMessage(FFQS(SID_COMMON_SELECT_OUTPUT_FILE));
	OpenMoreDlg = new wxFileDialog(this, _T("Select file"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	OpenMoreDlg->SetMessage(FFQS(SID_COMMON_SELECT_INPUT_FILE));
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_SSBROWSEIMG,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_SSFRAMESTATUS,wxEVT_COMMAND_HYPERLINK,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_SSFIT,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_SSBROWSEAUDIO,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCSOURCES,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCSOURCES,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&FFQConcat::OnCCSrcDblClick);
	Connect(ID_CCADD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCREMOVE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCUP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCDOWN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCSIMPLE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCPADDING,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCUSEPADCOLOR,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CCUSEPADBLUR,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_PAGES,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,(wxObjectEventFunction)&FFQConcat::OnPagesPageChanging);
	Connect(ID_BROWSEDEST,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_LIMITDEST,wxEVT_COMMAND_HYPERLINK,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_OKBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQConcat::OnAction);
	//*)

    SetTitle(FFQS(SID_CONCAT_TITLE));

    Pages->SetPageText(0, FFQS(SID_CONCAT_VIDEO_FROM_IMAGES));
    Pages->SetPageText(1, FFQS(SID_CONCAT_MERGE_FILES));

    CCSources->DragAcceptFiles(true);
    CCSources->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(FFQConcat::OnDropFiles), NULL, this);

	Connect(wxID_ANY, wxEVT_IDLE, (wxObjectEventFunction)&FFQConcat::OnIdle);

	FFQCFG()->SetCtrlColors(LimitDest);
	FFQCFG()->SetCtrlColors(SSFrameStatus);

    m_EditJob = NULL;
    m_DoIdleTask = false;
    m_BlurOK = FFQCFG()->AreFiltersAvailable("split,scale,boxblur,overlay");

	m_TimeEdit = NULL;
	m_PIP = NULL;

	m_TempPath = "";

	m_ImgPatn = "";
	m_ImgCount = 0;
	m_ImgFirst = 0;

	m_LastPst = "";
	m_LimitLen = TIME_VALUE(0);

    SSFit->SetValue(true);
    SSWidth->SetValue("800");
    SSHeight->SetValue("600");
    SSPadding->SetColour(wxColour(0, 0, 0));
    SSFrameTime->SetValue("5");

    //SaveLog->SetValue(true);

	//Preset->SetFixedFirstItem("")

}

//---------------------------------------------------------------------------------------

FFQConcat::~FFQConcat()
{
	//(*Destroy(FFQConcat)
	//*)
	if (m_TimeEdit)
    {
        delete m_TimeEdit;
        m_TimeEdit = NULL;
    }
    if (m_PIP)
    {
        delete m_PIP;
        m_PIP = NULL;
    }
    //CleanupLast();
}

//---------------------------------------------------------------------------------------

bool FFQConcat::Execute(LPFFQ_CONCAT_JOB job)
{

    m_EditJob = job;
    FFQ_INPUT_FILE inf;

    //Select and enable pages
    Pages->ChangeSelection(job->slideshow ? 0 : 1);

    //Load values
    if (job->slideshow)
    {

        inf = job->GetInput(0);

        //Slide show
        SSSource->ChangeValue(inf.path);
        SSFit->SetValue(job->scale_pad);
        SSWidth->ChangeValue(inf.path.Len() == 0 ? DEFAULT_WIDTH : ToStr(inf.width));
        SSHeight->ChangeValue(inf.path.Len() == 0 ? DEFAULT_HEIGHT : ToStr(inf.height));
        SSFrameTime->ChangeValue(ToStr(job->frame_time));
        SSSetPTS->SetValue(job->force);
        SSAudio->SetValue(job->GetInput(1).path);
        SSLoopFrames->SetValue(job->loop_frames);


    }
    else
    {

        //Merge / concatenate videos
        CCSimple->SetValue(job->simple_concat);
        CCExplicitMap->SetValue(job->map_streams > 0);
        CCSetSar->SetValue(job->force);

        //pad = [0|1],[0|1],color,blur
        wxString pad = job->merge_padding;
        CCPadding->SetValue( STRBOOLDEF( GetToken(pad, ",", true), false ) );
        CCUsePadColor->SetValue( STRBOOLDEF( GetToken(pad, ",", true), true ) );
        CCUsePadBlur->SetValue(!CCUsePadColor->GetValue());
        wxString col = GetToken(pad, ",", true);
        if (col.Len() == 0) col = "#000000";
        CCPadColor->SetColour(wxColour(col));
        CCPadBlur->SetValue(Str2Long(pad, 2));

        CCVideo->SetValue(job->concat_vid);
        CCAudio->SetValue(job->concat_aud);

    }

    //Destination file
    DestFile->SetValue(job->out);

    //Private values
    m_LimitLen = job->limit_len;
    m_ImgFirst = job->img_first;
    m_ImgCount = job->img_count;
    m_ImgPatn = job->img_pattern;
    m_LastPst = job->preset.ToString();
    m_AudInfo.reset();

    //Fill presets
    Preset->FillAndSelect(m_LastPst);

    //Save log
    SaveLog->SetValue(job->save_log);

    //Center
    CenterOnParent();

    //Update controls
    UpdateControls();

    //Make sure idle tasks are enabled
    m_DoIdleTask = true;

    //Show
    int res = ShowModal();

    if (res == wxID_OK)
    {

        //Clear previous sources
        job->inputs.Clear();

        //Set destination and preset
        job->out = StrTrim(DestFile->GetValue());
        job->preset = UNIQUE_ID(m_LastPst);

        //Update job type
        job->slideshow = (Pages->GetSelection() == 0);

        //Save log
        job->save_log = SaveLog->GetValue();
        FFQCFG()->SetSaveLog(job->save_log);

        //Store values
        if (job->slideshow)
        {

            //Slide show
            inf.path = StrTrim(SSSource->GetValue());
            inf.width = Str2Long(StrTrim(SSWidth->GetValue()));
            inf.height = Str2Long(StrTrim(SSHeight->GetValue()));
            job->inputs.Add(inf.ToString());

            job->scale_pad = SSFit->GetValue();
            job->pad_color = SSPadding->GetColour().GetRGB();
            job->frame_time = Str2Float(StrTrim(SSFrameTime->GetValue()), 5);
            job->force = SSSetPTS->GetValue();

            inf.Reset();
            inf.path = m_AudInfo.path;
            if (inf.path.Len() > 0)
            {
                inf.duration = m_AudInfo.duration;
                job->inputs.Add(inf.ToString());
            }

            job->loop_frames = SSLoopFrames->GetValue();
            job->limit_len = m_LimitLen;
            job->img_first = m_ImgFirst;
            job->img_count = m_ImgCount;
            job->img_pattern = m_ImgPatn;

        }
        else
        {

            //Merge / concatenate videos

            //Store padding
            job->merge_padding.Printf(
                "%s,%s,%s,%i",
                BOOLSTR(CCPadding->GetValue()),
                BOOLSTR(CCUsePadColor->GetValue()),
                CCPadColor->GetColour().GetAsString(wxC2S_HTML_SYNTAX),
                CCPadBlur->GetValue()
            );

            //Build (or clear) filter before CCSources is cleared!
            if (job->simple_concat)
            {

                //Clear filter & mapping, unused
                job->merge_filter.Clear();
                job->merge_smap.Clear();

            }
            else BuildMergeFilter(job->merge_filter, job->merge_smap);

            CCSources->Freeze();
            for (int i = 0; i < (int)CCSources->GetCount(); i++)
            {

                LPCONCAT_DATA cd = (LPCONCAT_DATA)CCSources->GetClientData(i);
                if (i == 0)
                {
                    if (CCExplicitMap->GetValue()) job->map_streams = cd->streams;
                    else job->map_streams = 0;
                }
                inf.Reset();
                inf.path = cd->path;
                inf.duration = cd->duration;
                inf.start = cd->custDuration;
                //inf.width = cd->width;
                //inf.height = cd->height;
                job->inputs.Add(inf.ToString());
                delete cd;


            }
            CCSources->Clear();
            CCSources->Thaw();

            job->simple_concat = CCSimple->GetValue();
            job->force = CCSetSar->GetValue();
            job->concat_vid = CCVideo->GetValue();
            job->concat_aud = CCAudio->GetValue();
            job->concat_subs = CCSubtitles->GetValue();

        }


        //Return success
        return true;

    }

    //Clear CCSources (and concat data)
    CCSources->Freeze();
    for (unsigned int i = 0; i < CCSources->GetCount(); i++) delete (LPCONCAT_DATA)CCSources->GetClientData(i);
    CCSources->Clear();
    CCSources->Thaw();

    //Return canceled
    return false;

}

//---------------------------------------------------------------------------------------

void FFQConcat::AddConcatSources(wxArrayString *paths)
{

    LPCONCAT_DATA cd;
    wxString errors = "";

    for (unsigned int i = 0; i < paths->GetCount(); i++)
    {

        cd = new CONCAT_DATA(paths->Item(i));

        if (!GetFileInfo(cd, false))
        {

            errors += cd->path + CRLF;
            delete cd;

        }

        else CCSources->Append(ConcatDataToString(cd, CCSimple->GetValue()), cd);

    }

    delete paths;

    //Default output name
    if (CCSources->GetCount() > 0) SetDestFile( ((LPCONCAT_DATA)CCSources->GetClientData(0))->path );

    if (errors.Len() > 0) ShowError(NULL, FFQS(SID_FFPROBE_FAILED_ON_FILES) + CRLF + CRLF + errors);

    CCSources->SetFocus();

}

//---------------------------------------------------------------------------------------

void FFQConcat::BuildMergeFilter(wxString &filter, wxString &smap)
{

    //Generate the merge filter

    unsigned int w = 0, h = 0;
    bool scale = false, vid = CCVideo->GetValue(), aud = CCAudio->GetValue();
    LPCONCAT_DATA cd, pcd = NULL;
    filter = "";
    wxString cv = "", ca = "", s, t;

    for (unsigned int i = 0; i < CCSources->GetCount(); i++)
    {

        cd = (LPCONCAT_DATA)CCSources->GetClientData(i);

        //Set dimensions of first video
        if (i == 0) { w = cd->width; h = cd->height; }

        //Check if scaling must be applied to following video(s)
        else scale = ((w != cd->width) || (h != cd->height));

        //Append filter
        if (i > 0)
        {

            //Get concat data from previous
            //pcd = (LPCONCAT_DATA)CCSources->GetClientData(i-1);

            //Make name for current video stream
            s = "[" + ToStr(i) + ":" + ToStr(cd->videoID) + "]";

            if (vid)
            {

                if ((w % 2 != 0) || (h % 2 != 0))
                {

                    //Make sure the video has an even width and height by cropping a line
                    w -= (w % 2);
                    h -= (h % 2);
                    t = "[CCCV" + ToStr(i) + "]";
                    filter += s + "crop=" + ToStr(w) + ":" + ToStr(h) + ":0:0" + t + ";";
                    s = t;

                }

                else if (scale)
                {

                    t = "[CCSV" + ToStr(i) + "]";

                    if (CCPadding->GetValue())
                    {

                        wxString sw, sh, fltr;

                        //Create scaling that remains aspect
                        sw.Printf("'iw*min(%u/iw,%u/ih)'", w, h);
                        sh.Printf("'ih*min(%u/iw,%u/ih)'", w, h);

                        if (CCUsePadBlur->GetValue())
                        {
                            //Pad using blur

                            //Create filter tokens
                            wxString fid1 = "[CCPB" + ToStr(i) + "A]", fid2 = "[CCPB" + ToStr(i) + "B]",
                                     fid3 = "[CCPB" + ToStr(i) + "C]", fid4 = "[CCPB" + ToStr(i) + "D]";

                            //Get blur
                            int blur = CCPadBlur->GetValue();

                            //Make the filter
                            fltr =  "%ssplit%s%s;"; //Split
                            fltr += "%sscale=%u:%u,boxblur=%i:%i%s;"; //Stretch and blur
                            fltr += "%sscale=%s:%s%s;"; //Scale to fit
                            fltr += "%s%soverlay=(W-w)/2:(H-h)/2%s"; //Overlay

                            fltr.Printf(fltr,
                                        s, fid1, fid2, //Split
                                        fid1, w, h, blur*2, blur, fid3, //Stretch and blur
                                        fid2, sw, sh, fid4, //Scale to fit
                                        fid3, fid4, t); //Overlay

                        }
                        else
                        {

                            //Pad using color
                            fltr.Printf(
                                "%sscale=%s:%s,pad=%u:%u:(ow-iw)/2:(oh-ih)/2:%s%s",
                                s, sw, sh, w, h, CCPadColor->GetColour().GetAsString(wxC2S_HTML_SYNTAX), t
                            );

                        }

                        filter += fltr + ";";

                    }
                    else
                    {
                        //Stretch
                        filter += s + "scale=" + ToStr(w) + ":" + ToStr(h) + t + ";";

                    }

                    s = t;

                }

                if (CCSetSar->GetValue())
                {

                    if (i == 1)
                    {

                        //Set SAR of first video
                        t = "[CCSA0]";
                        filter += "[0:" + ToStr(pcd->videoID) + "]setsar=a" + t + ";";
                        cv = t;

                    }

                    //Set SAR of current video
                    t = "[CCSA" + ToStr(i) + "]";
                    filter += s + "setsar=a" + t + ";";
                    s = t;

                }

            }


            //Add previous stream identifiers
            if (vid) filter += cv.Len() == 0 ? "[0:" + ToStr(pcd->videoID) + "]" : cv;
            if (aud) filter += ca.Len() == 0 ? "[0:" + ToStr(pcd->audioID) + "]" : ca;

            //Add current stream identifiers
            if (vid)
            {

                filter += s;
                cv = "[CCV" + ToStr(i) + "]";

            }

            if (aud)
            {

                filter += "[" + ToStr(i) + ":" + ToStr(cd->audioID) + "]";
                ca = "[CCA" + ToStr(i) + "]";

            }

            //Apply concatenation
            filter = filter + "concat=n=2:v=" + (vid ? "1" : "0") + ":a=" + (aud ? "1" : "0") + cv + ca + ";";

        }

        //Set previous for next iteration
        pcd = cd;

    }

    //Remove last ";" from filter
    filter.Remove(filter.Len() - 1, 1);

    //Set smap
    smap = cv + "," + ca;

}

//---------------------------------------------------------------------------------------

bool FFQConcat::EditTime(TIME_VALUE &tv)
{
    if (!m_TimeEdit) m_TimeEdit = new FFQTimeEdit(this);
    return m_TimeEdit->Execute(tv, NULL, 0);
}

//---------------------------------------------------------------------------------------

bool FFQConcat::EnumSlideshowFrames()
{

    //Create pattern and count images
    m_ImgPatn = StrTrim(SSSource->GetValue());
    if (!wxFileExists(m_ImgPatn)) m_ImgPatn = "";
    else m_ImgCount = FilePathToRange(m_ImgPatn, m_ImgFirst);

    //Update destination file
    if (m_ImgPatn.Len() > 0) SetDestFile(StrTrim(SSSource->GetValue()));

    //Return result
    return (m_ImgPatn.Len() > 0);

}

//---------------------------------------------------------------------------------------

bool FFQConcat::GetFileInfo(LPCONCAT_DATA cd, bool dimensionRequired)
{

    if (!wxFileExists(cd->path)) return false;
    if (m_PIP == NULL) m_PIP = new FFProbeInfoParser();
    if (m_PIP->RunFFProbe(cd->path))
    {
        if ((!m_PIP->GetVideoDimension(cd->width, cd->height)) && dimensionRequired) return false;
        if (!m_PIP->GetDuration(cd->duration)) return false;
        cd->streams = 0;
        for (unsigned int i = 0; i < m_PIP->GetStreamCount(); i++)
        {
            LPFFPROBE_STREAM_INFO si = m_PIP->GetStreamInfo(i);
            if ( (cd->videoID < 0) && (si->codec_type == CODEC_TYPE_VIDEO) ) cd->videoID = i;
            else if ( (cd->audioID < 0) && (si->codec_type == CODEC_TYPE_AUDIO) ) cd->audioID = i;

            //Streams are stored as bits rather than a number in order to be able
            //to select stream types for explicit mapping in a future release
            cd->streams |= (1 << i);
        }
        m_PIP->SetProbeOutput(""); //Free memory
        return true;
    }
    return false;

}

//---------------------------------------------------------------------------------------

void SelItem(wxListBox *lb, unsigned int idx)
{
    for (unsigned int i = 0; i < lb->GetCount(); i++) lb->SetSelection(i, i == idx);
}

//---------------------------------------------------------------------------------------

void FFQConcat::SetDestFile(wxString based_on)
{

    //Set default destination file name based on the argument given
    if ((based_on.Len() > 0) && (DestFile->GetValue().Len() == 0))
       DestFile->ChangeValue( FFQConfig::GetInstance()->GetPreferredOutputName(based_on) );

}

//---------------------------------------------------------------------------------------

bool FFQConcat::UpdateAudioInfo()
{

    //Get path
    wxString path = StrTrim(SSAudio->GetValue());

    //Check if update is required
    if (m_AudInfo.path != path)
    {

        //Reset info
        m_AudInfo.reset();

        if (wxFileExists(path))
        {

            //If file exists, update info
            m_AudInfo.path = path;
            GetFileInfo(&m_AudInfo, false);

        }

    }

    //Return if audio is valid
    return (m_AudInfo.path.Len() == 0) || (m_AudInfo.audioID >= 0);

}

//---------------------------------------------------------------------------------------

void FFQConcat::UpdateControls()
{

    wxString s = (m_ImgCount == 0) ? "No" : ToStr(m_ImgCount);
    SSFrameStatus->SetLabelText(FFQSF(SID_CONCAT_IMAGES_FOUND, s));
    //Sizer1->RecalcSizes();
    Sizer1->Layout();

    LimitDest->SetLabelText(FFQSF(SID_LIMIT_LENGTH, (m_LimitLen.IsUndefined() ? FFQS(SID_NO_LIMIT) : m_LimitLen.ToString())));
    //Sizer2->RecalcSizes();
    Sizer2->Layout();

    long s_first = -1, s_last = -1, s_count = 0, vids = 0, auds = 0;

    for (unsigned int i = 0; i < CCSources->GetCount(); i++)
    {

        //Check if audio and video is available
        LPCONCAT_DATA cd = (LPCONCAT_DATA)CCSources->GetClientData(i);
        if (cd->videoID >= 0) vids++;
        if (cd->audioID >= 0) auds++;

        //Set first and last selected + selected count
        if (CCSources->IsSelected(i))
        {
            if (s_first < 0) s_first = i;
            s_last = i;
            s_count++;
        }

    }

    CCVideo->Enable(vids >= auds);
    CCSetSar->Enable(vids >= auds);
    if (!CCVideo->IsEnabled()) CCVideo->SetValue(false);
    CCAudio->Enable(auds >= vids);
    if (!CCAudio->IsEnabled()) CCAudio->SetValue(false);

    CCRemove->Enable(s_count > 0);
    CCUp->Enable(s_first > 0);
    CCDown->Enable((s_last >= 0) && (s_last < (long)CCSources->GetCount() - 1));

    EnableSizer(SSSizer1, SSFit->GetValue(), NULL);

    //Disable preset and limit length for simple concatenation
    EnableSizer(Sizer3, !CCSimple->GetValue());
    EnableSizer(Sizer4, !CCSimple->GetValue());
    CCExplicitMap->Enable(CCSimple->GetValue());

    bool can_pad = (!CCSimple->GetValue()) && CCPadding->GetValue();
    EnableSizer(CCPadSizer, can_pad);

    if (can_pad)
    {
        CCPadColor->Enable(CCUsePadColor->GetValue());
        CCUsePadBlur->Enable(m_BlurOK);
        EnableSizer(CCBlurSizer, m_BlurOK && CCUsePadBlur->GetValue());
    }

    CCSetSar->Enable(!CCSimple->GetValue());
    CCPadding->Enable(!CCSimple->GetValue());
    CCSubtitles->Enable(false);

}

//---------------------------------------------------------------------------------------

bool FFQConcat::ValidateDialog()
{

    //Make sure the job is not running
    if (!m_EditJob->CheckNotActive()) return false;

    wxString s;
    LPFFQ_PRESET pst;

    if (Pages->GetSelection() == 0) //Validate slide show
    {

        //Image source
        if (m_ImgCount == 0) EnumSlideshowFrames();
        if (m_ImgCount == 0) return ShowError(SSSource, FFQS(SID_CONCAT_NO_IMAGES_FOUND));

        if (SSFit->GetValue())
        {

            //Width at least 50 and equal
            long l = Str2Long(StrTrim(SSWidth->GetValue()));
            if ((l < 50) || (l % 2 != 0)) return ShowError(SSWidth, FFQS(SID_CONCAT_MIN_WIDTH_AND_EVEN));

            //Height - as width
            l = Str2Long(StrTrim(SSHeight->GetValue()));
            if ((l < 50) || (l % 2 != 0)) return ShowError(SSHeight, FFQS(SID_CONCAT_MIN_HEIGHT_AND_EVEN));

        }

        //Frame time
        double rate = Str2Float(StrTrim(SSFrameTime->GetValue()), 0);
        if (rate <= 0) return ShowError(SSFrameTime, FFQS(SID_CONCAT_FRAME_TIME_POSITIVE));

        //Audio source
        if (!UpdateAudioInfo()) return ShowError(SSAudio, FFQS(SID_CONCAT_AUDIO_TRACK_ERROR));

        //Preset and destination
        if (!GetOutputFileAndPreset(s, &pst, false, false)) return false;

        //Infinite loop
        if (SSLoopFrames->GetValue() && (m_AudInfo.path.Len() == 0) && (m_LimitLen.ToMilliseconds() == 0))
            return ShowError(FFQS(SID_CONCAT_LOOP_ERROR));

    }

    else //Validate concatenation or merge
    {

        //Check if at least two videos are available
        if (CCSources->GetCount() < 2) return ShowError(CCSources, FFQS(SID_CONCAT_TWO_FILES_MINIMUM));

        LPCONCAT_DATA cd;

        if (CCSimple->GetValue()) //Concatenate
        {

            //Check that all files are same width*height and count video and audio streams
            unsigned int w = 0, h = 0, a = 0, v = 0;

            for (unsigned int i = 0; i < CCSources->GetCount(); i++)
            {

                cd = (LPCONCAT_DATA)CCSources->GetClientData(i);

                if (w == 0) w = cd->width;
                else if (w != cd->width) return ShowError(CCSources, FFQS(SID_CONCAT_UNEQUAL_FILES));

                if (h == 0) h = cd->height;
                else if (h != cd->height) return ShowError(CCSources, FFQS(SID_CONCAT_UNEQUAL_FILES));

                if (cd->audioID >= 0) a++;
                if (cd->videoID >= 0) v++;

            }

            //Validate source streams as good as possible
            if ((a != v) || (a != CCSources->GetCount())) return ShowError(CCSources, FFQS(SID_CONCAT_UNEQUAL_FILES));

            //Output file - no preset
            if (!GetOutputFileAndPreset(s, NULL, false, false)) return false;

        }

        else //Merge
        {

            //Check if video, audio or subtitles are selected
            bool vid = CCVideo->GetValue(), aud = CCAudio->GetValue();
            if ( (!vid) && (!aud) && (!CCSubtitles->GetValue()) ) return ShowError(CCVideo, FFQS(SID_NO_CONTENT_SELECTED));

            //Check if selected content is available
            for (unsigned int i = 0; i < CCSources->GetCount(); i++)
            {

                cd = (LPCONCAT_DATA)CCSources->GetClientData(i);

                if ((vid && (cd->videoID == -1)) || (aud && (cd->audioID == -1)))
                {

                    SelItem(CCSources, i);
                    return ShowError(CCSources, FFQS(SID_CONCAT_CONTENT_MISSING));

                }

            }

            //Output file and preset
            if (!GetOutputFileAndPreset(s, &pst, false, false)) return false;

        }

    }

    //All OK
    return true;

}

//---------------------------------------------------------------------------------------

bool FFQConcat::GetOutputFileAndPreset(wxString &file, LPFFQ_PRESET *pst, bool allowVideoCopyCodec, bool allowTwoPass)
{

    file = StrTrim(DestFile->GetValue());
    if (file.Len() == 0) return ShowError(DestFile, FFQS(SID_OUTPUT_FILE_REQUIRED));
    //if (wxFileExists(file) && (!DoConfirm(DestFile, FFQS(SID_OVERWRITE_OUTPUT_FILE)))) return false;

    if (pst)
    {

        *pst = Preset->GetSelectedPreset();
        if (*pst == NULL) return ShowError(Preset, FFQS(SID_PRESET_REQUIRED));
        if ((!allowVideoCopyCodec) && ((*pst)->video_codec == CODEC_COPY)) return ShowError(Preset, FFQS(SID_COPY_VIDEO_NOT_ALLOWED));
        if ((!allowTwoPass) && (*pst)->two_pass) return ShowError(Preset, FFQS(SID_TWO_PASS_NOT_ALLOWED));


        //Save id of last preset
        m_LastPst = (*pst)->preset_id.ToString();

    }

    return true;

}

//---------------------------------------------------------------------------------------

void FFQConcat::OnIdle(wxIdleEvent &event)
{

    event.Skip();

    //Make sure that idle task is only done once!
    if (!m_DoIdleTask) return;
    m_DoIdleTask = false;

    if (Pages->GetSelection() == 0)
    {

        //Check if slide show frames should be updated
        if (EnumSlideshowFrames()) UpdateControls();

    }

    else if (m_EditJob->inputs.Count() > 0)
    {

        //Add sources from job being edited
        wxArrayString *files = new wxArrayString();
        for (int i = 0; i < (int)m_EditJob->inputs.Count(); i++) files->Add(m_EditJob->GetInput(i).path);
        AddConcatSources(files);

    }

}

//---------------------------------------------------------------------------------------

void FFQConcat::OnDropFiles(wxDropFilesEvent& event)
{
    Raise();
    wxString* fl = event.GetFiles(), s;
    if (fl == NULL) return;
    wxArrayString* files = new wxArrayString();
    for (int i = 0; i < event.GetNumberOfFiles(); i++)
    {
        s = fl[i];
        if (wxFileExists(s)) files->Add(s);
        else if (wxDirExists(s)) wxDir::GetAllFiles(s, files);
    }
    AddConcatSources(files);
}

//---------------------------------------------------------------------------------------

void FFQConcat::OnAction(wxCommandEvent& event)
{
    int evtId = event.GetId();

    if (evtId == ID_SSBROWSEIMG)
    {
        OpenOneDlg->SetPath(SSSource->GetValue());
        if (OpenOneDlg->ShowModal() != wxID_CANCEL)
        {
            SSSource->SetValue(OpenOneDlg->GetPath());
            EnumSlideshowFrames();
        }
    }

    else if (evtId == ID_SSFRAMESTATUS) EnumSlideshowFrames();

    else if (evtId == ID_SSBROWSEAUDIO)
    {
        OpenOneDlg->SetPath(SSAudio->GetValue());
        if (OpenOneDlg->ShowModal() != wxID_CANCEL) SSAudio->SetValue(OpenOneDlg->GetPath());
    }

    else if (evtId == ID_CCADD)
    {

        //long style = OpenFileDlg->GetWindowStyleFlag();
        //OpenFileDlg->SetWindowStyleFlag(style | wxFD_MULTIPLE);
        if (OpenMoreDlg->ShowModal() != wxID_CANCEL)
        {
            wxArrayString *paths = new wxArrayString();
            OpenMoreDlg->GetPaths(*paths);
            AddConcatSources(paths);
        }
        //OpenFileDlg->SetWindowStyleFlag(style);

    }

    else if (evtId == ID_CCREMOVE)
    {

        unsigned int idx = 0;
        while (idx < CCSources->GetCount())
        {
            if (CCSources->IsSelected(idx))
            {
                delete (LPCONCAT_DATA)CCSources->GetClientData(idx);
                CCSources->Delete(idx);
            }
            else idx++;
        }

    }

    else if (evtId == ID_CCUP) ListBoxMoveSelectedItems(CCSources, true, false);

    else if (evtId == ID_CCDOWN) ListBoxMoveSelectedItems(CCSources, false, false);

    else if (evtId == ID_BROWSEDEST)
    {

        SaveFileDlg->SetPath(DestFile->GetValue());
        if (SaveFileDlg->ShowModal() != wxID_CANCEL) DestFile->SetValue(SaveFileDlg->GetPath());

    }

    else if (evtId == ID_LIMITDEST)
    {

        CONCAT_DATA cd;
        cd.duration = m_LimitLen;
        if (cd.duration.IsUndefined() && (Pages->GetCurrentPage() == SlideshowPage))
        {

            if (UpdateAudioInfo()) cd.duration = m_AudInfo.duration;
            //cd.path = StrTrim(SSAudio->GetValue());
            //if (wxFileExists(cd.path)) GetFileInfo(&cd, false);

        }
        if (!EditTime(cd.duration)) return;
        m_LimitLen = cd.duration;

    }

    else if (evtId == ID_CANCELBUTTON) EndModal(wxID_CANCEL);

    else if (evtId == ID_OKBUTTON)
    {

        if (m_EditJob != NULL)
        {
            if (ValidateDialog()) EndModal(wxID_OK);
            return;
        }

    }

    else if (evtId == ID_CCSIMPLE)
    {

        //Update items to ensure that the correct time value is displayed
        CCSources->Freeze();
        for (unsigned int i = 0; i < CCSources->GetCount(); i++)
            CCSources->SetString(i, ConcatDataToString( (LPCONCAT_DATA)CCSources->GetClientData(i), CCSimple->GetValue() ) );
        CCSources->Thaw();

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQConcat::OnCCSrcDblClick(wxCommandEvent& event)
{

    if (CCSimple->GetValue())
    {

        for (unsigned int i = 0; i < CCSources->GetCount(); i++) if (CCSources->IsSelected(i))
        {

            //Get concat data for first selected item
            LPCONCAT_DATA cd = (LPCONCAT_DATA)CCSources->GetClientData(i);

            //Get the time value to edit
            TIME_VALUE tv = (cd->custDuration == 0) ? cd->duration : cd->custDuration;

            //Edit
            if (EditTime(tv))
            {

                //Adjust custDuration
                if (tv == cd->duration) cd->custDuration = TIME_VALUE();
                else cd->custDuration = tv;

                //Set items string
                CCSources->SetString(i, ConcatDataToString(cd, true));

            }

            //Return focus
            CCSources->SetFocus();

            //Done
            return;

        }

    }

}

//---------------------------------------------------------------------------------------

void FFQConcat::OnPagesPageChanging(wxNotebookEvent& event)
{

    //Veto change if we are editing
    if (m_EditJob && (m_EditJob->inputs.Count() > 0)) event.Veto();
    else event.Skip();

}
