/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPresetEdit.cpp                                               *
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

#include "FFQPresetEdit.h"
#include "FFQMain.h"
#include "FFQFullSpec.h"
#include "utils/FFQLang.h"
#include "utils/FFQConfig.h"
#include "utils/FFQBuildCmd.h"
#include "utils/FFQMisc.h"
#include "utils/FFQConst.h"
#include <wx/display.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQPresetEdit)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQPresetEdit)
//*)

// The following crappy workaround is to prevent problems with
// Code::Block using the old event name without the ending D.
#ifndef wxEVT_GRID_CELL_CHANGE
#define wxEVT_GRID_CELL_CHANGE wxEVT_GRID_CELL_CHANGED
#endif // wxEVT_GRID_CELL_CHANGE

#define SUBS_CHARENC_SEP ' '

//(*IdInit(FFQPresetEdit)
const long FFQPresetEdit::ID_PRESETNAME = wxNewId();
const long FFQPresetEdit::ID_PRESETTEMP = wxNewId();
const long FFQPresetEdit::ID_VIDEOCODEC = wxNewId();
const long FFQPresetEdit::ID_FULLSPECVIDBUTTON = wxNewId();
const long FFQPresetEdit::ID_TWOPASS = wxNewId();
const long FFQPresetEdit::ID_TWOPASSNULL = wxNewId();
const long FFQPresetEdit::ID_SKIPENCODESAMEVID = wxNewId();
const long FFQPresetEdit::ID_VIDEOBITRATE = wxNewId();
const long FFQPresetEdit::ID_BITRATEPAGE = wxNewId();
const long FFQPresetEdit::ID_QST1 = wxNewId();
const long FFQPresetEdit::ID_QST2 = wxNewId();
const long FFQPresetEdit::ID_USEVIDEOQSCALE = wxNewId();
const long FFQPresetEdit::ID_VIDEOQSCALE = wxNewId();
const long FFQPresetEdit::ID_VIDEOQSCALEVAL = wxNewId();
const long FFQPresetEdit::ID_USECONSTRATE = wxNewId();
const long FFQPresetEdit::ID_CONSTRATE = wxNewId();
const long FFQPresetEdit::ID_CONSTRATEVAL = wxNewId();
const long FFQPresetEdit::ID_USEMINCONSTRATE = wxNewId();
const long FFQPresetEdit::ID_MINCONSTRATE = wxNewId();
const long FFQPresetEdit::ID_MINCONSTRATEVAL = wxNewId();
const long FFQPresetEdit::ID_USEQMIN = wxNewId();
const long FFQPresetEdit::ID_QMIN = wxNewId();
const long FFQPresetEdit::ID_QMINVAL = wxNewId();
const long FFQPresetEdit::ID_USEQMAX = wxNewId();
const long FFQPresetEdit::ID_QMAX = wxNewId();
const long FFQPresetEdit::ID_QMAXVAL = wxNewId();
const long FFQPresetEdit::ID_USEQDIFF = wxNewId();
const long FFQPresetEdit::ID_QDIFF = wxNewId();
const long FFQPresetEdit::ID_QDIFFVAL = wxNewId();
const long FFQPresetEdit::ID_QUALITYPAGE = wxNewId();
const long FFQPresetEdit::ID_KEYFRAMEMIN = wxNewId();
const long FFQPresetEdit::ID_KEYFRAMEMAX = wxNewId();
const long FFQPresetEdit::ID_KEYFRAMEMAXB = wxNewId();
const long FFQPresetEdit::ID_KEYFRAMEMAXP = wxNewId();
const long FFQPresetEdit::ID_MOTIONESTIMATION = wxNewId();
const long FFQPresetEdit::ID_USESCENECHANGESENSITIVITY = wxNewId();
const long FFQPresetEdit::ID_SCENECHANGESENS = wxNewId();
const long FFQPresetEdit::ID_USECLOSEDGOP = wxNewId();
const long FFQPresetEdit::ID_KEYFRAMESPAGE = wxNewId();
const long FFQPresetEdit::ID_OST3 = wxNewId();
const long FFQPresetEdit::ID_VIDEOSYNC = wxNewId();
const long FFQPresetEdit::ID_OST1 = wxNewId();
const long FFQPresetEdit::ID_FRAMERATE = wxNewId();
const long FFQPresetEdit::ID_OST2 = wxNewId();
const long FFQPresetEdit::ID_OST4 = wxNewId();
const long FFQPresetEdit::ID_PIXFMT = wxNewId();
const long FFQPresetEdit::ID_OST6 = wxNewId();
const long FFQPresetEdit::ID_TRELLIS = wxNewId();
const long FFQPresetEdit::ID_OST7 = wxNewId();
const long FFQPresetEdit::ID_SPEEDPRESET = wxNewId();
const long FFQPresetEdit::ID_OST8 = wxNewId();
const long FFQPresetEdit::ID_VIDEOTUNING = wxNewId();
const long FFQPresetEdit::ID_OST5 = wxNewId();
const long FFQPresetEdit::ID_VIDEOTHREADS = wxNewId();
const long FFQPresetEdit::ID_OTHERPAGE = wxNewId();
const long FFQPresetEdit::ID_HWD_ACCEL = wxNewId();
const long FFQPresetEdit::ID_HWD_CODEC = wxNewId();
const long FFQPresetEdit::ID_HWD_DEVICE = wxNewId();
const long FFQPresetEdit::ID_HWD_DEVICEINIT = wxNewId();
const long FFQPresetEdit::ID_HWDECODEPAGE = wxNewId();
const long FFQPresetEdit::ID_VIDEOPAGES = wxNewId();
const long FFQPresetEdit::ID_VIDEOPAGE = wxNewId();
const long FFQPresetEdit::ID_AUDIOCODEC = wxNewId();
const long FFQPresetEdit::ID_FULLSPECAUDBUTTON = wxNewId();
const long FFQPresetEdit::ID_SKIPENCODESAMEAUD = wxNewId();
const long FFQPresetEdit::ID_AUDIOBITRATE = wxNewId();
const long FFQPresetEdit::ID_USEAUDIOQSCALE = wxNewId();
const long FFQPresetEdit::ID_AST1 = wxNewId();
const long FFQPresetEdit::ID_AUDIOQSCALE = wxNewId();
const long FFQPresetEdit::ID_AST2 = wxNewId();
const long FFQPresetEdit::ID_AUDIOQSCALEVAL = wxNewId();
const long FFQPresetEdit::ID_AST4 = wxNewId();
const long FFQPresetEdit::ID_AUDIOCHANNELS = wxNewId();
const long FFQPresetEdit::ID_AST5 = wxNewId();
const long FFQPresetEdit::ID_AUDIOPROFILE = wxNewId();
const long FFQPresetEdit::ID_AUDIOPAGE = wxNewId();
const long FFQPresetEdit::ID_SUBSCODEC = wxNewId();
const long FFQPresetEdit::ID_ST53 = wxNewId();
const long FFQPresetEdit::ID_SUBSTEXT = wxNewId();
const long FFQPresetEdit::ID_SUBSBITMAP = wxNewId();
const long FFQPresetEdit::ID_SUBSLAB2 = wxNewId();
const long FFQPresetEdit::ID_SUBSSIZEDEF = wxNewId();
const long FFQPresetEdit::ID_SUBSSIZEPAL = wxNewId();
const long FFQPresetEdit::ID_SUBSSIZECUST = wxNewId();
const long FFQPresetEdit::ID_SUBSWIDTH = wxNewId();
const long FFQPresetEdit::ID_STATICTEXT11 = wxNewId();
const long FFQPresetEdit::ID_SUBSHEIGHT = wxNewId();
const long FFQPresetEdit::ID_SUBSSIZEPAN = wxNewId();
const long FFQPresetEdit::wxID_NONE = wxNewId();
const long FFQPresetEdit::ID_SUBSCHARENC = wxNewId();
const long FFQPresetEdit::ID_SUBSLAB1 = wxNewId();
const long FFQPresetEdit::ID_SUBSSCALE = wxNewId();
const long FFQPresetEdit::ID_STATICTEXT8 = wxNewId();
const long FFQPresetEdit::ID_SUBTITLESPAGE = wxNewId();
const long FFQPresetEdit::ID_FILTERLIST = wxNewId();
const long FFQPresetEdit::ID_ADDFILTERBUTTON = wxNewId();
const long FFQPresetEdit::ID_EDITFILTERBUTTON = wxNewId();
const long FFQPresetEdit::ID_REMOVEFILTERBUTTON = wxNewId();
const long FFQPresetEdit::ID_FILTERUPBUTTON = wxNewId();
const long FFQPresetEdit::ID_FILTERDOWNBUTTON = wxNewId();
const long FFQPresetEdit::ID_FILTERPREVIEWBUTTON = wxNewId();
const long FFQPresetEdit::ID_AUDIOFILTERSCOMPLEX = wxNewId();
const long FFQPresetEdit::ID_FILTERTIP = wxNewId();
const long FFQPresetEdit::ID_FILTERPAGE = wxNewId();
const long FFQPresetEdit::ID_METADATA = wxNewId();
const long FFQPresetEdit::ID_CLEARMETADATABTN = wxNewId();
const long FFQPresetEdit::ID_STMD1 = wxNewId();
const long FFQPresetEdit::ID_METADATAFOR = wxNewId();
const long FFQPresetEdit::ID_METADATAPAGE = wxNewId();
const long FFQPresetEdit::ID_THUMBSPANEL = wxNewId();
const long FFQPresetEdit::ID_THUMBSPAGE = wxNewId();
const long FFQPresetEdit::ID_ST51 = wxNewId();
const long FFQPresetEdit::ID_FOURCC_VID = wxNewId();
const long FFQPresetEdit::ID_ST52 = wxNewId();
const long FFQPresetEdit::ID_FOURCC_AUD = wxNewId();
const long FFQPresetEdit::ID_CST1 = wxNewId();
const long FFQPresetEdit::ID_CUSTOM_ARGS_1 = wxNewId();
const long FFQPresetEdit::ID_CST2 = wxNewId();
const long FFQPresetEdit::ID_CUSTOM_ARGS_2 = wxNewId();
const long FFQPresetEdit::ID_SEGMENTLEN = wxNewId();
const long FFQPresetEdit::ID_SEGMENTLENTYPE = wxNewId();
const long FFQPresetEdit::ID_SEGMENTLISTTYPE = wxNewId();
const long FFQPresetEdit::ID_SEGMENTRESETTS = wxNewId();
const long FFQPresetEdit::ID_SEGMENTINCTIME = wxNewId();
const long FFQPresetEdit::ID_SEGMENTSTREAMING = wxNewId();
const long FFQPresetEdit::ID_SEGMENTBREAKB = wxNewId();
const long FFQPresetEdit::ID_STATICTEXT13 = wxNewId();
const long FFQPresetEdit::ID_ASPECT = wxNewId();
const long FFQPresetEdit::ID_DISPOSITIONSBTN = wxNewId();
const long FFQPresetEdit::ID_OUTPUTFORMAT = wxNewId();
const long FFQPresetEdit::ID_MF_FASTSTART = wxNewId();
const long FFQPresetEdit::ID_KEEPFILETIME = wxNewId();
const long FFQPresetEdit::ID_STOPENCSHORTEST = wxNewId();
const long FFQPresetEdit::ID_MISCPAGE = wxNewId();
const long FFQPresetEdit::ID_PAGES = wxNewId();
const long FFQPresetEdit::ID_OKBUTTON = wxNewId();
const long FFQPresetEdit::ID_CANCELBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQPresetEdit,wxDialog)
	//(*EventTable(FFQPresetEdit)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

//Meta data constants
const wxString ILLEGAL_METADATA_CHARS = BLANKS + "\"\'=|";
const wxString METADATA_FIELDS = "title,author,album_artist,album,grouping,composer,year,date,track," \
                                 "comment,genre,copyright,description,synopsis,show,episode_id," \
                                 "network,lyrics,language,rating";

//---------------------------------------------------------------------------------------

const wxString VSYNC_FORMAT = "%s (%s)";

//---------------------------------------------------------------------------------------

const int DISPOSITION_MENU_ID_COUNT = 30;
const int FILTER_MENU_BASE_ID = 1000;

//---------------------------------------------------------------------------------------

void GetCodecNameAndID(wxString src, wxString &name, wxString &id)
{
    name = src.BeforeFirst(SPACE);
    src = src.AfterLast('(');
    if (src.StartsWith("codec ") && src.EndsWith(')')) id = src.AfterFirst(SPACE).BeforeLast(')');
    else id = name;
}

//---------------------------------------------------------------------------------------

FFQPresetEdit::FFQPresetEdit(wxWindow* parent)
{
	//(*Initialize(FFQPresetEdit)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer100;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer11;
	wxFlexGridSizer* FlexGridSizer12;
	wxFlexGridSizer* FlexGridSizer13;
	wxFlexGridSizer* FlexGridSizer14;
	wxFlexGridSizer* FlexGridSizer15;
	wxFlexGridSizer* FlexGridSizer16;
	wxFlexGridSizer* FlexGridSizer17;
	wxFlexGridSizer* FlexGridSizer18;
	wxFlexGridSizer* FlexGridSizer19;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer20;
	wxFlexGridSizer* FlexGridSizer21;
	wxFlexGridSizer* FlexGridSizer22;
	wxFlexGridSizer* FlexGridSizer23;
	wxFlexGridSizer* FlexGridSizer24;
	wxFlexGridSizer* FlexGridSizer25;
	wxFlexGridSizer* FlexGridSizer26;
	wxFlexGridSizer* FlexGridSizer27;
	wxFlexGridSizer* FlexGridSizer28;
	wxFlexGridSizer* FlexGridSizer29;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer33;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* FlexGridSizer9;
	wxStaticBoxSizer* ASBS1;
	wxStaticBoxSizer* SBS1;
	wxStaticBoxSizer* SBS31;
	wxStaticBoxSizer* SBS51;
	wxStaticBoxSizer* SBS52;
	wxStaticBoxSizer* SSBS1;
	wxStaticBoxSizer* SegmentingSizer;
	wxStaticBoxSizer* VSBS1;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(1);
	SBS1 = new wxStaticBoxSizer(wxVERTICAL, this, _T("PstN"));
	PresetName = new wxTextCtrl(this, ID_PRESETNAME, wxEmptyString, wxDefaultPosition, wxSize(550,-1), 0, wxDefaultValidator, _T("ID_PRESETNAME"));
	SBS1->Add(PresetName, 1, wxALL|wxEXPAND, 2);
	PresetTemp = new wxCheckBox(this, ID_PRESETTEMP, _T("Temp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_PRESETTEMP"));
	PresetTemp->SetValue(false);
	PresetTemp->SetLabel(FFQS(SID_PRESET_TEMPORARY_PRESET));
	SBS1->GetStaticBox()->SetLabel(FFQS(SID_PRESET_PRESET_NAME));
	SBS1->Add(PresetTemp, 1, wxLEFT|wxALIGN_LEFT, 2);
	FlexGridSizer1->Add(SBS1, 1, wxALL|wxEXPAND, 5);
	Pages = new wxNotebook(this, ID_PAGES, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PAGES"));
	VideoPage = new wxPanel(Pages, ID_VIDEOPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_VIDEOPAGE"));
	FlexGridSizer13 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer13->AddGrowableCol(0);
	VSBS1 = new wxStaticBoxSizer(wxHORIZONTAL, VideoPage, _T("Cd"));
	FlexGridSizer18 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer18->AddGrowableCol(0);
	VideoCodecSizer = new wxFlexGridSizer(1, 2, 0, 0);
	VideoCodecSizer->AddGrowableCol(0);
	VideoCodec = new wxChoice(VideoPage, ID_VIDEOCODEC, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_VIDEOCODEC"));
	VSBS1->GetStaticBox()->SetLabel(FFQS(SID_PRESET_CODEC));
	VideoCodecSizer->Add(VideoCodec, 1, wxALL|wxEXPAND, 3);
	FullSpecVidButton = new wxButton(VideoPage, ID_FULLSPECVIDBUTTON, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FULLSPECVIDBUTTON"));
	VideoCodecSizer->Add(FullSpecVidButton, 1, wxALL, 3);
	FlexGridSizer18->Add(VideoCodecSizer, 0, wxEXPAND, 0);
	VideoTwoPassSizer = new wxFlexGridSizer(2, 2, 0, 0);
	VideoTwoPassSizer->AddGrowableCol(1);
	VideoTwoPassSizer->AddGrowableRow(0);
	TwoPass = new wxCheckBox(VideoPage, ID_TWOPASS, _T("2pass"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TWOPASS"));
	TwoPass->SetValue(false);
	TwoPass->SetLabel(FFQS(SID_PRESET_TWO_PASS_ENCODING));
	VideoTwoPassSizer->Add(TwoPass, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	TwoPassNull = new wxCheckBox(VideoPage, ID_TWOPASSNULL, _T("2p null"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TWOPASSNULL"));
	TwoPassNull->SetValue(false);
	TwoPassNull->SetLabel(FFQS(SID_PRESET_FIRST_PASS_NULL_TARGET));
	VideoTwoPassSizer->Add(TwoPassNull, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SkipEncodeSameVid = new wxCheckBox(VideoPage, ID_SKIPENCODESAMEVID, _T("Sev"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SKIPENCODESAMEVID"));
	SkipEncodeSameVid->SetValue(false);
	SkipEncodeSameVid->SetLabel(FFQS(SID_PRESET_SKIP_ENCODE_SAME));
	VideoTwoPassSizer->Add(SkipEncodeSameVid, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	VideoTwoPassSizer->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	FlexGridSizer18->Add(VideoTwoPassSizer, 1, wxALL|wxEXPAND, 0);
	VSBS1->Add(FlexGridSizer18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer13->Add(VSBS1, 1, wxALL|wxEXPAND, 5);
	VideoPages = new wxNotebook(VideoPage, ID_VIDEOPAGES, wxDefaultPosition, wxDefaultSize, 0, _T("ID_VIDEOPAGES"));
	BitRatePage = new wxPanel(VideoPages, ID_BITRATEPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_BITRATEPAGE"));
	VideoBitRateSizer = new wxFlexGridSizer(1, 1, 0, 0);
	VideoBitRateSizer->AddGrowableCol(0);
	VideoBitRate = new FFQBitRatePanel(BitRatePage);
	VideoBitRateSizer->Add(VideoBitRate, 1, wxALL|wxEXPAND, 3);
	BitRatePage->SetSizer(VideoBitRateSizer);
	VideoBitRateSizer->Fit(BitRatePage);
	VideoBitRateSizer->SetSizeHints(BitRatePage);
	QualityPage = new wxPanel(VideoPages, ID_QUALITYPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_QUALITYPAGE"));
	VideoQualitySizer = new wxFlexGridSizer(7, 3, 0, 0);
	VideoQualitySizer->AddGrowableCol(1);
	VideoQualitySizer->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	FlexGridSizer23 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer23->AddGrowableCol(0);
	FlexGridSizer23->AddGrowableCol(1);
	QST1 = new wxStaticText(QualityPage, ID_QST1, _T("L"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_QST1"));
	QST1->SetLabel(FFQS(SID_PRESET_LOW));
	FlexGridSizer23->Add(QST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	QST2 = new wxStaticText(QualityPage, ID_QST2, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_QST2"));
	QST2->SetLabel(FFQS(SID_PRESET_HIGH));
	FlexGridSizer23->Add(QST2, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	VideoQualitySizer->Add(FlexGridSizer23, 1, wxALL|wxEXPAND, 3);
	VideoQualitySizer->Add(50,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	UseVideoQScale = new wxCheckBox(QualityPage, ID_USEVIDEOQSCALE, _T("Qs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USEVIDEOQSCALE"));
	UseVideoQScale->SetValue(false);
	UseVideoQScale->SetLabel(FFQS(SID_PRESET_QUALITY_SCALE));
	VideoQualitySizer->Add(UseVideoQScale, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	VideoQScale = new wxSlider(QualityPage, ID_VIDEOQSCALE, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_VIDEOQSCALE"));
	VideoQualitySizer->Add(VideoQScale, 1, wxALL|wxEXPAND, 3);
	VideoQScaleVal = new wxStaticText(QualityPage, ID_VIDEOQSCALEVAL, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_VIDEOQSCALEVAL"));
	VideoQualitySizer->Add(VideoQScaleVal, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	UseConstRate = new wxCheckBox(QualityPage, ID_USECONSTRATE, _T("Crf"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USECONSTRATE"));
	UseConstRate->SetValue(false);
	UseConstRate->SetLabel(FFQS(SID_PRESET_CONSTANT_RATE));
	VideoQualitySizer->Add(UseConstRate, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ConstRate = new wxSlider(QualityPage, ID_CONSTRATE, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CONSTRATE"));
	VideoQualitySizer->Add(ConstRate, 1, wxALL|wxEXPAND, 3);
	ConstRateVal = new wxStaticText(QualityPage, ID_CONSTRATEVAL, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_CONSTRATEVAL"));
	VideoQualitySizer->Add(ConstRateVal, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	UseMinConstRate = new wxCheckBox(QualityPage, ID_USEMINCONSTRATE, _T("MinCrf"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USEMINCONSTRATE"));
	UseMinConstRate->SetValue(false);
	UseMinConstRate->SetLabel(FFQS(SID_PRESET_MIN_CONSTANT_RATE));
	VideoQualitySizer->Add(UseMinConstRate, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	MinConstRate = new wxSlider(QualityPage, ID_MINCONSTRATE, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MINCONSTRATE"));
	VideoQualitySizer->Add(MinConstRate, 1, wxALL|wxEXPAND, 3);
	MinConstRateVal = new wxStaticText(QualityPage, ID_MINCONSTRATEVAL, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_MINCONSTRATEVAL"));
	VideoQualitySizer->Add(MinConstRateVal, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	UseQMin = new wxCheckBox(QualityPage, ID_USEQMIN, _T("Qmin"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USEQMIN"));
	UseQMin->SetValue(false);
	UseQMin->SetLabel(FFQS(SID_PRESET_QUANTIZER_MIN));
	VideoQualitySizer->Add(UseQMin, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	QMin = new wxSlider(QualityPage, ID_QMIN, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_QMIN"));
	VideoQualitySizer->Add(QMin, 1, wxALL|wxEXPAND, 3);
	QMinVal = new wxStaticText(QualityPage, ID_QMINVAL, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_QMINVAL"));
	VideoQualitySizer->Add(QMinVal, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	UseQMax = new wxCheckBox(QualityPage, ID_USEQMAX, _T("QMax"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USEQMAX"));
	UseQMax->SetValue(false);
	UseQMax->SetLabel(FFQS(SID_PRESET_QUANTIZER_MAX));
	VideoQualitySizer->Add(UseQMax, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	QMax = new wxSlider(QualityPage, ID_QMAX, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_QMAX"));
	VideoQualitySizer->Add(QMax, 1, wxALL|wxEXPAND, 3);
	QMaxVal = new wxStaticText(QualityPage, ID_QMAXVAL, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_QMAXVAL"));
	VideoQualitySizer->Add(QMaxVal, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	UseQDiff = new wxCheckBox(QualityPage, ID_USEQDIFF, _T("QDiff"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USEQDIFF"));
	UseQDiff->SetValue(false);
	UseQDiff->SetLabel(FFQS(SID_PRESET_QUANTIZER_MAX_DIFF));
	VideoQualitySizer->Add(UseQDiff, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	QDiff = new wxSlider(QualityPage, ID_QDIFF, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_QDIFF"));
	VideoQualitySizer->Add(QDiff, 1, wxALL|wxEXPAND, 3);
	QDiffVal = new wxStaticText(QualityPage, ID_QDIFFVAL, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_QDIFFVAL"));
	VideoQualitySizer->Add(QDiffVal, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	QualityPage->SetSizer(VideoQualitySizer);
	VideoQualitySizer->Fit(QualityPage);
	VideoQualitySizer->SetSizeHints(QualityPage);
	KeyFramesPage = new wxPanel(VideoPages, ID_KEYFRAMESPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_KEYFRAMESPAGE"));
	KeyFrameSizer = new wxFlexGridSizer(4, 1, 0, 0);
	KeyFrameSizer->AddGrowableCol(0);
	FlexGridSizer4 = new wxFlexGridSizer(4, 3, 0, 0);
	KST1 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("KfMin"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST1->SetLabel(FFQS(SID_PRESET_KEYFRAME_MINIMUM));
	FlexGridSizer4->Add(KST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KeyFrameMin = new wxTextCtrl(KeyFramesPage, ID_KEYFRAMEMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_KEYFRAMEMIN"));
	FlexGridSizer4->Add(KeyFrameMin, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KST2 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("KfInf"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST2->SetLabel(FFQS(SID_PRESET_KEYFRAME_INFO));
	FlexGridSizer4->Add(KST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KST3 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("KfMax"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST3->SetLabel(FFQS(SID_PRESET_KEYFRAME_MAXIMUM));
	FlexGridSizer4->Add(KST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KeyFrameMax = new wxTextCtrl(KeyFramesPage, ID_KEYFRAMEMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_KEYFRAMEMAX"));
	FlexGridSizer4->Add(KeyFrameMax, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KST4 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("KfInf"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST4->SetLabel(FFQS(SID_PRESET_KEYFRAME_INFO));
	FlexGridSizer4->Add(KST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KST8 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("MaxB"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST8->SetLabel(FFQS(SID_PRESET_KEYFRAME_MAX_B_FRAMES));
	FlexGridSizer4->Add(KST8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KeyFrameMaxB = new wxChoice(KeyFramesPage, ID_KEYFRAMEMAXB, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_KEYFRAMEMAXB"));
	FlexGridSizer4->Add(KeyFrameMaxB, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	KST9 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("MaxP"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST9->SetLabel(FFQS(SID_PRESET_KEYFRAME_MAX_P_REFS));
	FlexGridSizer4->Add(KST9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KeyFrameMaxP = new wxChoice(KeyFramesPage, ID_KEYFRAMEMAXP, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_KEYFRAMEMAXP"));
	FlexGridSizer4->Add(KeyFrameMaxP, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	KeyFrameSizer->Add(FlexGridSizer4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer21 = new wxFlexGridSizer(1, 2, 0, 0);
	KST5 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("M-est"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST5->SetLabel(FFQS(SID_PRESET_MOTION_EST_METHOD));
	FlexGridSizer21->Add(KST5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	MotionEstimation = new wxComboBox(KeyFramesPage, ID_MOTIONESTIMATION, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_MOTIONESTIMATION"));
	MotionEstimation->Append(_T("zero"));
	MotionEstimation->Append(_T("full"));
	MotionEstimation->Append(_T("epzs"));
	MotionEstimation->Append(_T("esa"));
	MotionEstimation->Append(_T("tesa"));
	MotionEstimation->Append(_T("dia"));
	MotionEstimation->Append(_T("log"));
	MotionEstimation->Append(_T("phods"));
	MotionEstimation->Append(_T("x1"));
	MotionEstimation->Append(_T("hex"));
	MotionEstimation->Append(_T("umh"));
	MotionEstimation->Append(_T("iter"));
	FlexGridSizer21->Add(MotionEstimation, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	KeyFrameSizer->Add(FlexGridSizer21, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer19 = new wxFlexGridSizer(1, 3, 0, 0);
	UseSceneChangeSens = new wxCheckBox(KeyFramesPage, ID_USESCENECHANGESENSITIVITY, _T("ScSens"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USESCENECHANGESENSITIVITY"));
	UseSceneChangeSens->SetValue(false);
	UseSceneChangeSens->SetLabel(FFQS(SID_PRESET_SCENE_CHANGE_SENSITIVITY));
	FlexGridSizer19->Add(UseSceneChangeSens, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SceneChangeSizer = new wxFlexGridSizer(1, 3, 0, 0);
	SceneChangeSizer->AddGrowableCol(1);
	SceneChangeSizer->AddGrowableRow(0);
	KST6 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("L"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST6->SetLabel(FFQS(SID_PRESET_LOW));
	SceneChangeSizer->Add(KST6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SceneChangeSens = new wxSlider(KeyFramesPage, ID_SCENECHANGESENS, 0, 0, 99, wxDefaultPosition, wxSize(200,-1), 0, wxDefaultValidator, _T("ID_SCENECHANGESENS"));
	SceneChangeSizer->Add(SceneChangeSens, 1, wxALL|wxEXPAND, 0);
	KST7 = new wxStaticText(KeyFramesPage, wxID_ANY, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	KST7->SetLabel(FFQS(SID_PRESET_HIGH));
	SceneChangeSizer->Add(KST7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer19->Add(SceneChangeSizer, 1, wxALL|wxEXPAND, 0);
	ScSensInfo = new wxStaticText(KeyFramesPage, wxID_ANY, _T("ScsI"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer19->Add(ScSensInfo, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 10);
	KeyFrameSizer->Add(FlexGridSizer19, 0, wxALL|wxEXPAND, 3);
	FlexGridSizer17 = new wxFlexGridSizer(1, 1, 0, 0);
	UseClosedGOP = new wxCheckBox(KeyFramesPage, ID_USECLOSEDGOP, _T("Cgop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USECLOSEDGOP"));
	UseClosedGOP->SetValue(false);
	UseClosedGOP->SetLabel(FFQS(SID_PRESET_USE_CLOSED_GOP));
	FlexGridSizer17->Add(UseClosedGOP, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	KeyFrameSizer->Add(FlexGridSizer17, 1, wxALL|wxEXPAND, 3);
	KeyFramesPage->SetSizer(KeyFrameSizer);
	KeyFrameSizer->Fit(KeyFramesPage);
	KeyFrameSizer->SetSizeHints(KeyFramesPage);
	OtherPage = new wxPanel(VideoPages, ID_OTHERPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_OTHERPAGE"));
	VideoOtherSizer = new wxFlexGridSizer(6, 2, 0, 0);
	VideoOtherSizer->AddGrowableCol(1);
	OST3 = new wxStaticText(OtherPage, ID_OST3, _T("VSync"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST3"));
	OST3->SetLabel(FFQS(SID_PRESET_VIDEO_SYNC));
	VideoOtherSizer->Add(OST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	VideoSync = new wxChoice(OtherPage, ID_VIDEOSYNC, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_VIDEOSYNC"));
	VideoSync->SetSelection( VideoSync->Append( wxString::Format(VSYNC_FORMAT,  "AUTO", FFQS(SID_PRESET_VIDEO_SYNC_AUTO)) ) );
	VideoSync->Append( wxString::Format(VSYNC_FORMAT,  "PASSTHROUGH", FFQS(SID_PRESET_VIDEO_SYNC_PASSTHROUGH))  );
	VideoSync->Append( wxString::Format(VSYNC_FORMAT,  "CFR", FFQS(SID_PRESET_VIDEO_SYNC_CFR)) );
	VideoSync->Append( wxString::Format(VSYNC_FORMAT,  "VFR", FFQS(SID_PRESET_VIDEO_SYNC_VFR)) );
	VideoSync->Append( wxString::Format(VSYNC_FORMAT,  "DROP", FFQS(SID_PRESET_VIDEO_SYNC_DROP)) );
	VideoOtherSizer->Add(VideoSync, 1, wxALL|wxEXPAND, 3);
	OST1 = new wxStaticText(OtherPage, ID_OST1, _T("Fr"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST1"));
	OST1->SetLabel(FFQS(SID_PRESET_FRAMERATE));
	VideoOtherSizer->Add(OST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer22 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer22->AddGrowableCol(1);
	FlexGridSizer22->AddGrowableRow(0);
	FrameRate = new wxTextCtrl(OtherPage, ID_FRAMERATE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FRAMERATE"));
	FlexGridSizer22->Add(FrameRate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	OST2 = new wxStaticText(OtherPage, ID_OST2, _T("FPS"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST2"));
	OST2->SetLabel(FFQS(SID_PRESET_FPS));
	FlexGridSizer22->Add(OST2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	VideoOtherSizer->Add(FlexGridSizer22, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	OST4 = new wxStaticText(OtherPage, ID_OST4, _T("Pf"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST4"));
	OST4->SetLabel(FFQS(SID_PRESET_PIXEL_FORMAT));
	VideoOtherSizer->Add(OST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	PixFmt = new wxComboBox(OtherPage, ID_PIXFMT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_PIXFMT"));
	VideoOtherSizer->Add(PixFmt, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OST6 = new wxStaticText(OtherPage, ID_OST6, _T("Tr"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST6"));
	OST6->SetLabel(FFQS(SID_PRESET_TRELLIS));
	VideoOtherSizer->Add(OST6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Trellis = new wxChoice(OtherPage, ID_TRELLIS, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_TRELLIS"));
	wxString *trl = FFQL()->GetStringArray(SID_PRESET_TRELLIS_LIST, 4);
	Trellis->Append(4, trl);
	Trellis->SetSelection(0);
	VideoOtherSizer->Add(Trellis, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OST7 = new wxStaticText(OtherPage, ID_OST7, _T("Pst"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST7"));
	OST7->SetLabel(FFQS(SID_PRESET_SPEED_PRESET));
	VideoOtherSizer->Add(OST7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	PresetTuneSizer = new wxFlexGridSizer(1, 3, 0, 0);
	PresetTuneSizer->AddGrowableCol(2);
	SpeedPreset = new wxComboBox(OtherPage, ID_SPEEDPRESET, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_SPEEDPRESET"));
	SpeedPreset->Append(ENCODER_PRESET_COUNT, ENCODER_PRESETS);
	PresetTuneSizer->Add(SpeedPreset, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OST8 = new wxStaticText(OtherPage, ID_OST8, _T("Tun"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST8"));
	OST8->SetLabel(FFQS(SID_PRESET_VIDEO_TUNING));
	PresetTuneSizer->Add(OST8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	VideoTuning = new wxComboBox(OtherPage, ID_VIDEOTUNING, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_VIDEOTUNING"));
	VideoTuning->Append(ENCODER_TUNINGS_COUNT, ENCODER_TUNINGS);
	PresetTuneSizer->Add(VideoTuning, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	VideoOtherSizer->Add(PresetTuneSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	OST5 = new wxStaticText(OtherPage, ID_OST5, _T("Td"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_OST5"));
	OST5->SetLabel(FFQS(SID_PRESET_THREADS));
	VideoOtherSizer->Add(OST5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	VideoThreads = new wxComboBox(OtherPage, ID_VIDEOTHREADS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_VIDEOTHREADS"));
	VideoThreads->Append(_T("auto"));
	VideoThreads->Append(_T("1"));
	VideoThreads->Append(_T("2"));
	VideoThreads->Append(_T("4"));
	VideoThreads->Append(_T("8"));
	VideoThreads->Append(_T("16"));
	VideoOtherSizer->Add(VideoThreads, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OtherPage->SetSizer(VideoOtherSizer);
	VideoOtherSizer->Fit(OtherPage);
	VideoOtherSizer->SetSizeHints(OtherPage);
	HWDecodePage = new wxPanel(VideoPages, ID_HWDECODEPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_HWDECODEPAGE"));
	VideoHWDecodeSizer = new wxFlexGridSizer(4, 2, 0, 0);
	VideoHWDecodeSizer->AddGrowableCol(1);
	HWD1 = new wxStaticText(HWDecodePage, wxID_ANY, _T("Dc"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	HWD1->SetLabel(FFQS(SID_PRESET_HWDECODE_ACCEL));
	VideoHWDecodeSizer->Add(HWD1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	HWD_Accel = new wxChoice(HWDecodePage, ID_HWD_ACCEL, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_HWD_ACCEL"));
	VideoHWDecodeSizer->Add(HWD_Accel, 1, wxALL|wxEXPAND, 3);
	HWD2 = new wxStaticText(HWDecodePage, wxID_ANY, _T("Cd"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	HWD2->SetLabel(FFQS(SID_PRESET_HWDECODE_CODEC));
	VideoHWDecodeSizer->Add(HWD2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	HWD_Codec = new wxComboBox(HWDecodePage, ID_HWD_CODEC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_HWD_CODEC"));
	VideoHWDecodeSizer->Add(HWD_Codec, 1, wxALL|wxEXPAND, 3);
	HWD3 = new wxStaticText(HWDecodePage, wxID_ANY, _T("Dev"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	HWD3->SetLabel(FFQS(SID_PRESET_HWDECODE_DEVICE));
	VideoHWDecodeSizer->Add(HWD3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	HWD_Device = new wxTextCtrl(HWDecodePage, ID_HWD_DEVICE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_HWD_DEVICE"));
	VideoHWDecodeSizer->Add(HWD_Device, 1, wxALL|wxEXPAND, 3);
	HWD4 = new wxStaticText(HWDecodePage, wxID_ANY, _T("Init"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	HWD4->SetLabel(FFQS(SID_PRESET_HWDECODE_DEVICE_INIT));
	VideoHWDecodeSizer->Add(HWD4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	HWD_DeviceInit = new wxTextCtrl(HWDecodePage, ID_HWD_DEVICEINIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_HWD_DEVICEINIT"));
	VideoHWDecodeSizer->Add(HWD_DeviceInit, 1, wxALL|wxEXPAND, 3);
	HWDecodePage->SetSizer(VideoHWDecodeSizer);
	VideoHWDecodeSizer->Fit(HWDecodePage);
	VideoHWDecodeSizer->SetSizeHints(HWDecodePage);
	VideoPages->AddPage(BitRatePage, _T("Br"), false);
	VideoPages->AddPage(QualityPage, _T("Q"), false);
	VideoPages->AddPage(KeyFramesPage, _T("Kf"), false);
	VideoPages->AddPage(OtherPage, _T("Ot"), false);
	VideoPages->AddPage(HWDecodePage, _T("HWD"), false);
	FlexGridSizer13->Add(VideoPages, 1, wxALL|wxEXPAND, 3);
	VideoPage->SetSizer(FlexGridSizer13);
	FlexGridSizer13->Fit(VideoPage);
	FlexGridSizer13->SetSizeHints(VideoPage);
	AudioPage = new wxPanel(Pages, ID_AUDIOPAGE, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTAB_TRAVERSAL, _T("ID_AUDIOPAGE"));
	FlexGridSizer3 = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	ASBS1 = new wxStaticBoxSizer(wxVERTICAL, AudioPage, _T("Cd"));
	FlexGridSizer12 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer12->AddGrowableCol(0);
	AudioCodec = new wxChoice(AudioPage, ID_AUDIOCODEC, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_AUDIOCODEC"));
	ASBS1->GetStaticBox()->SetLabel(FFQS(SID_PRESET_CODEC));
	FlexGridSizer12->Add(AudioCodec, 1, wxALL|wxEXPAND, 3);
	FullSpecAudButton = new wxButton(AudioPage, ID_FULLSPECAUDBUTTON, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FULLSPECAUDBUTTON"));
	FlexGridSizer12->Add(FullSpecAudButton, 1, wxALL, 3);
	SkipEncodeSameAud = new wxCheckBox(AudioPage, ID_SKIPENCODESAMEAUD, _T("Sea"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SKIPENCODESAMEAUD"));
	SkipEncodeSameAud->SetValue(false);
	SkipEncodeSameAud->SetLabel(FFQS(SID_PRESET_SKIP_ENCODE_SAME));
	FlexGridSizer12->Add(SkipEncodeSameAud, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer12->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	ASBS1->Add(FlexGridSizer12, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3->Add(ASBS1, 1, wxALL|wxEXPAND, 5);
	ASBS2 = new wxStaticBoxSizer(wxHORIZONTAL, AudioPage, _T("Br"));
	AudioBitRate = new FFQBitRatePanel(AudioPage);
	ASBS2->Add(AudioBitRate, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3->Add(ASBS2, 1, wxALL|wxEXPAND, 5);
	ASBS3 = new wxStaticBoxSizer(wxVERTICAL, AudioPage, _T("Q"));
	FlexGridSizer27 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer27->AddGrowableCol(1);
	UseAudioQScale = new wxCheckBox(AudioPage, ID_USEAUDIOQSCALE, _T("Qs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_USEAUDIOQSCALE"));
	UseAudioQScale->SetValue(false);
	ASBS2->GetStaticBox()->SetLabel(FFQS(SID_PRESET_BITRATE));
	ASBS3->GetStaticBox()->SetLabel(FFQS(SID_PRESET_QUALITY));
	UseAudioQScale->SetLabel(FFQS(SID_PRESET_QUALITY_SCALE));
	FlexGridSizer27->Add(UseAudioQScale, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	AudioQScaleSizer = new wxFlexGridSizer(1, 4, 0, 0);
	AudioQScaleSizer->AddGrowableCol(1);
	AST1 = new wxStaticText(AudioPage, ID_AST1, _T("L"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_AST1"));
	AST1->SetLabel(FFQS(SID_PRESET_LOW));
	AudioQScaleSizer->Add(AST1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	AudioQScale = new wxSlider(AudioPage, ID_AUDIOQSCALE, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUDIOQSCALE"));
	AudioQScaleSizer->Add(AudioQScale, 1, wxALL|wxEXPAND, 3);
	AST2 = new wxStaticText(AudioPage, ID_AST2, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_AST2"));
	AST2->SetLabel(FFQS(SID_PRESET_HIGH));
	AudioQScaleSizer->Add(AST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	AudioQScaleVal = new wxStaticText(AudioPage, ID_AUDIOQSCALEVAL, _T("XXXXXX"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_AUDIOQSCALEVAL"));
	AudioQScaleSizer->Add(AudioQScaleVal, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer27->Add(AudioQScaleSizer, 1, wxALL|wxEXPAND, 0);
	ASBS3->Add(FlexGridSizer27, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer3->Add(ASBS3, 1, wxALL|wxEXPAND, 5);
	ASBS4 = new wxStaticBoxSizer(wxHORIZONTAL, AudioPage, _T("Ot"));
	FlexGridSizer25 = new wxFlexGridSizer(2, 2, 0, 0);
	AST4 = new wxStaticText(AudioPage, ID_AST4, _T("AC"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_AST4"));
	ASBS4->GetStaticBox()->SetLabel(FFQS(SID_PRESET_OTHER));
	AST4->SetLabel(FFQS(SID_PRESET_NUM_AUDIO_CHANNELS));
	FlexGridSizer25->Add(AST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	AudioChannels = new wxTextCtrl(AudioPage, ID_AUDIOCHANNELS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUDIOCHANNELS"));
	FlexGridSizer25->Add(AudioChannels, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	AST5 = new wxStaticText(AudioPage, ID_AST5, _T("Ap"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_AST5"));
	AST5->SetLabel(FFQS(SID_PRESET_AUDIO_PROFILE));
	FlexGridSizer25->Add(AST5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	AudioProfile = new wxComboBox(AudioPage, ID_AUDIOPROFILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_AUDIOPROFILE"));
	AudioProfile->Append(_T("unknown"));
	AudioProfile->Append(_T("aac_main"));
	AudioProfile->Append(_T("aac_low"));
	AudioProfile->Append(_T("aac_ssr"));
	AudioProfile->Append(_T("aac_ltp"));
	AudioProfile->Append(_T("aac_he"));
	AudioProfile->Append(_T("aac_he_v2"));
	AudioProfile->Append(_T("aac_ld"));
	AudioProfile->Append(_T("aac_eld"));
	AudioProfile->Append(_T("mpeg2_aac_low"));
	AudioProfile->Append(_T("mpeg2_aac_he"));
	AudioProfile->Append(_T("dts"));
	AudioProfile->Append(_T("dts_es"));
	AudioProfile->Append(_T("dts_96_24"));
	AudioProfile->Append(_T("dts_hd_hra"));
	AudioProfile->Append(_T("dts_hd_ma"));
	FlexGridSizer25->Add(AudioProfile, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	ASBS4->Add(FlexGridSizer25, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3->Add(ASBS4, 1, wxALL|wxEXPAND, 5);
	AudioPage->SetSizer(FlexGridSizer3);
	FlexGridSizer3->Fit(AudioPage);
	FlexGridSizer3->SetSizeHints(AudioPage);
	SubtitlesPage = new wxPanel(Pages, ID_SUBTITLESPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_SUBTITLESPAGE"));
	FlexGridSizer24 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer24->AddGrowableCol(0);
	SSBS1 = new wxStaticBoxSizer(wxVERTICAL, SubtitlesPage, _T("Cd"));
	FlexGridSizer6 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer6->AddGrowableCol(0);
	SubsCodec = new wxChoice(SubtitlesPage, ID_SUBSCODEC, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_SUBSCODEC"));
	SSBS1->GetStaticBox()->SetLabel(FFQS(SID_PRESET_CODEC));
	FlexGridSizer6->Add(SubsCodec, 1, wxALL|wxEXPAND, 3);
	SSBS1->Add(FlexGridSizer6, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer24->Add(SSBS1, 1, wxALL|wxEXPAND, 5);
	SSBS2 = new wxStaticBoxSizer(wxVERTICAL, SubtitlesPage, _T("Burn"));
	FlexGridSizer8 = new wxFlexGridSizer(4, 2, 0, 0);
	FlexGridSizer8->AddGrowableCol(1);
	ST53 = new wxStaticText(SubtitlesPage, ID_ST53, _T("St"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST53"));
	ST53->SetLabel(FFQS(SID_PRESET_SUBTITLE_TYPE));
	SSBS2->GetStaticBox()->SetLabel(FFQS(SID_PRESET_BURN_IN_SUBTITLES));
	FlexGridSizer8->Add(ST53, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer16 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer16->AddGrowableCol(1);
	FlexGridSizer16->AddGrowableRow(0);
	SubsText = new wxRadioButton(SubtitlesPage, ID_SUBSTEXT, _T("Txt"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSTEXT"));
	SubsText->SetValue(true);
	SubsText->SetLabel(FFQS(SID_PRESET_SUBTITLE_TYPE_TEXT));
	FlexGridSizer16->Add(SubsText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SubsBitmap = new wxRadioButton(SubtitlesPage, ID_SUBSBITMAP, _T("Bmp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSBITMAP"));
	SubsBitmap->SetLabel(FFQS(SID_PRESET_SUBTITLE_TYPE_BITMAP));
	FlexGridSizer16->Add(SubsBitmap, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer8->Add(FlexGridSizer16, 1, wxALL|wxEXPAND, 3);
	SubsLab2 = new wxStaticText(SubtitlesPage, ID_SUBSLAB2, _T("Ss"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_SUBSLAB2"));
	SubsLab2->SetLabel(FFQS(SID_PRESET_SUBTITLE_SIZE));
	FlexGridSizer8->Add(SubsLab2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SubsSizePan = new wxPanel(SubtitlesPage, ID_SUBSSIZEPAN, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_SUBSSIZEPAN"));
	SubsSizer3 = new wxFlexGridSizer(1, 6, 0, 0);
	SubsSizer3->AddGrowableCol(5);
	SubsSizer3->AddGrowableRow(0);
	SubsSizeDef = new wxRadioButton(SubsSizePan, ID_SUBSSIZEDEF, _T("Def"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSSIZEDEF"));
	SubsSizeDef->SetLabel(FFQS(SID_PRESET_SUBTITLE_SIZE_DEFAULT));
	SubsSizer3->Add(SubsSizeDef, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SubsSizePAL = new wxRadioButton(SubsSizePan, ID_SUBSSIZEPAL, _T("PAL"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSSIZEPAL"));
	SubsSizePAL->SetLabel(FFQS(SID_PRESET_SUBTITLE_SIZE_PAL));
	SubsSizer3->Add(SubsSizePAL, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SubsSizeCust = new wxRadioButton(SubsSizePan, ID_SUBSSIZECUST, _T("Cust"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSSIZECUST"));
	SubsSizeCust->SetLabel(FFQS(SID_PRESET_SUBTITLE_SIZE_CUSTOM));
	SubsSizer3->Add(SubsSizeCust, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SubsWidth = new wxTextCtrl(SubsSizePan, ID_SUBSWIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSWIDTH"));
	SubsSizer3->Add(SubsWidth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SubsLab3 = new wxStaticText(SubsSizePan, ID_STATICTEXT11, _T("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	SubsSizer3->Add(SubsLab3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SubsHeight = new wxTextCtrl(SubsSizePan, ID_SUBSHEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SUBSHEIGHT"));
	SubsSizer3->Add(SubsHeight, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	SubsSizePan->SetSizer(SubsSizer3);
	SubsSizer3->Fit(SubsSizePan);
	SubsSizer3->SetSizeHints(SubsSizePan);
	FlexGridSizer8->Add(SubsSizePan, 1, wxALL|wxEXPAND, 3);
	SubsLab4 = new wxStaticText(SubtitlesPage, wxID_NONE, _T("Sce"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_NONE"));
	SubsLab4->SetLabel(FFQS(SID_PRESET_SUBTITLE_CHARENC));
	FlexGridSizer8->Add(SubsLab4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SubsCharEnc = new wxComboBox(SubtitlesPage, ID_SUBSCHARENC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_SORT, wxDefaultValidator, _T("ID_SUBSCHARENC"));
	FlexGridSizer8->Add(SubsCharEnc, 1, wxALL|wxEXPAND, 3);
	SubsLab1 = new wxStaticText(SubtitlesPage, ID_SUBSLAB1, _T("Sc"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_SUBSLAB1"));
	SubsLab1->SetLabel(FFQS(SID_PRESET_SUBTITLE_SCALE));
	FlexGridSizer8->Add(SubsLab1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SubsSizer2 = new wxFlexGridSizer(1, 2, 0, 0);
	SubsSizer2->AddGrowableCol(0);
	SubsScale = new wxSlider(SubtitlesPage, ID_SUBSSCALE, 100, 25, 100, wxDefaultPosition, wxSize(-1,23), 0, wxDefaultValidator, _T("ID_SUBSSCALE"));
	SubsSizer2->Add(SubsScale, 1, wxALL|wxEXPAND, 1);
	SubsScaleLab = new wxStaticText(SubtitlesPage, ID_STATICTEXT8, _T("100%"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	SubsSizer2->Add(SubsScaleLab, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer8->Add(SubsSizer2, 1, wxALL|wxEXPAND, 3);
	SSBS2->Add(FlexGridSizer8, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer24->Add(SSBS2, 1, wxALL|wxEXPAND, 5);
	SubtitlesPage->SetSizer(FlexGridSizer24);
	FlexGridSizer24->Fit(SubtitlesPage);
	FlexGridSizer24->SetSizeHints(SubtitlesPage);
	FilterPage = new wxPanel(Pages, ID_FILTERPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_FILTERPAGE"));
	FilterSizer = new wxFlexGridSizer(2, 1, 0, 0);
	FilterSizer->AddGrowableCol(0);
	FilterSizer->AddGrowableRow(0);
	FlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer7->AddGrowableCol(0);
	FlexGridSizer7->AddGrowableRow(0);
	FilterList = new wxListBox(FilterPage, ID_FILTERLIST, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_EXTENDED|wxHSCROLL, wxDefaultValidator, _T("ID_FILTERLIST"));
	FlexGridSizer7->Add(FilterList, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer5 = new wxFlexGridSizer(9, 1, 0, 0);
	FlexGridSizer5->AddGrowableCol(0);
	FlexGridSizer5->AddGrowableRow(8);
	AddFilterButton = new wxButton(FilterPage, ID_ADDFILTERBUTTON, _T("+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ADDFILTERBUTTON"));
	AddFilterButton->SetLabel(FFQS(SID_COMMON_ADD));
	FlexGridSizer5->Add(AddFilterButton, 1, wxALL|wxEXPAND, 3);
	EditFilterButton = new wxButton(FilterPage, ID_EDITFILTERBUTTON, _T("\?"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITFILTERBUTTON"));
	EditFilterButton->SetLabel(FFQS(SID_COMMON_EDIT));
	FlexGridSizer5->Add(EditFilterButton, 1, wxALL|wxEXPAND, 3);
	RemoveFilterButton = new wxButton(FilterPage, ID_REMOVEFILTERBUTTON, _T("-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_REMOVEFILTERBUTTON"));
	RemoveFilterButton->SetLabel(FFQS(SID_COMMON_REMOVE));
	FlexGridSizer5->Add(RemoveFilterButton, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer5->Add(-1,15,1, wxALL|wxEXPAND, 0);
	FilterUpButton = new wxButton(FilterPage, ID_FILTERUPBUTTON, _T("^"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FILTERUPBUTTON"));
	FilterUpButton->SetLabel(FFQS(SID_COMMON_MOVE_UP));
	FlexGridSizer5->Add(FilterUpButton, 1, wxALL|wxEXPAND, 3);
	FilterDownButton = new wxButton(FilterPage, ID_FILTERDOWNBUTTON, _T("v"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FILTERDOWNBUTTON"));
	FilterDownButton->SetLabel(FFQS(SID_COMMON_MOVE_DOWN));
	FlexGridSizer5->Add(FilterDownButton, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer5->Add(-1,15,1, wxALL|wxEXPAND, 0);
	FilterPreviewButton = new wxButton(FilterPage, ID_FILTERPREVIEWBUTTON, _T("Pw"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FILTERPREVIEWBUTTON"));
	FilterPreviewButton->SetLabel(FFQS(SID_COMMON_PREVIEW));
	FlexGridSizer5->Add(FilterPreviewButton, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer5->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	FlexGridSizer7->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 3);
	AudioFiltersComplex = new wxCheckBox(FilterPage, ID_AUDIOFILTERSCOMPLEX, _T("ACpl"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUDIOFILTERSCOMPLEX"));
	AudioFiltersComplex->SetValue(false);
	AudioFiltersComplex->SetLabel(FFQS(SID_PRESET_AUDIO_FILTERS_COMPLEX));
	FlexGridSizer7->Add(AudioFiltersComplex, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer7->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	FilterSizer->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 5);
	FilterTip = new wxStaticText(FilterPage, ID_FILTERTIP, _T("Inf"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_FILTERTIP"));
	FilterTip->Disable();
	FilterTip->SetLabel(FFQS(SID_PRESET_FILTER_INFO));
	FilterSizer->Add(FilterTip, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 8);
	FilterPage->SetSizer(FilterSizer);
	FilterSizer->Fit(FilterPage);
	FilterSizer->SetSizeHints(FilterPage);
	MetaDataPage = new wxPanel(Pages, ID_METADATAPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_METADATAPAGE"));
	FlexGridSizer9 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer9->AddGrowableCol(0);
	FlexGridSizer9->AddGrowableRow(0);
	SBS31 = new wxStaticBoxSizer(wxVERTICAL, MetaDataPage, _T("Md"));
	FlexGridSizer10 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer10->AddGrowableCol(0);
	FlexGridSizer10->AddGrowableRow(0);
	MetaData = new wxGrid(MetaDataPage, ID_METADATA, wxDefaultPosition, wxDefaultSize, 0, _T("ID_METADATA"));
	MetaData->CreateGrid(5,3);
	MetaData->SetColLabelValue(0, FFQS(SID_PRESET_METADATA_KEY));
	MetaData->SetColLabelValue(1, FFQS(SID_PRESET_METADATA_STREAM));
	MetaData->SetColLabelValue(2, FFQS(SID_PRESET_METADATA_VALUE));
	SBS31->GetStaticBox()->SetLabel(FFQS(SID_PRESET_METADATA_DEFINE));
	MetaData->EnableEditing(true);
	MetaData->EnableGridLines(true);
	MetaData->SetRowLabelSize(1);
	MetaData->SetDefaultCellFont( MetaData->GetFont() );
	MetaData->SetDefaultCellTextColour( MetaData->GetForegroundColour() );
	FlexGridSizer10->Add(MetaData, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer29 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer29->AddGrowableCol(0);
	ClearMetaDataBtn = new wxButton(MetaDataPage, ID_CLEARMETADATABTN, _T("Cl"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLEARMETADATABTN"));
	ClearMetaDataBtn->SetLabel(FFQS(SID_COMMON_CLEAR));
	FlexGridSizer29->Add(ClearMetaDataBtn, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	STMD1 = new wxStaticText(MetaDataPage, ID_STMD1, _T("SD"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STMD1"));
	STMD1->SetLabel(FFQS(SID_PRESET_METADATA_FOR));
	FlexGridSizer29->Add(STMD1, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	MetaDataFor = new wxChoice(MetaDataPage, ID_METADATAFOR, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_METADATAFOR"));
	FlexGridSizer29->Add(MetaDataFor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer10->Add(FlexGridSizer29, 1, wxALL|wxEXPAND, 3);
	SBS31->Add(FlexGridSizer10, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer9->Add(SBS31, 1, wxALL|wxEXPAND, 5);
	MetaDataPage->SetSizer(FlexGridSizer9);
	FlexGridSizer9->Fit(MetaDataPage);
	FlexGridSizer9->SetSizeHints(MetaDataPage);
	ThumbsPage = new wxPanel(Pages, ID_THUMBSPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_THUMBSPAGE"));
	FlexGridSizer11 = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizer11->AddGrowableCol(0);
	FlexGridSizer11->AddGrowableRow(0);
	ThumbsPanel = new FFQThumbPanel(ThumbsPage,ID_THUMBSPANEL);
	FlexGridSizer11->Add(ThumbsPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ThumbsPage->SetSizer(FlexGridSizer11);
	FlexGridSizer11->Fit(ThumbsPage);
	FlexGridSizer11->SetSizeHints(ThumbsPage);
	MiscPage = new wxPanel(Pages, ID_MISCPAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_MISCPAGE"));
	FlexGridSizer14 = new wxFlexGridSizer(4, 1, 0, 0);
	FlexGridSizer14->AddGrowableCol(0);
	FourCCSizer = new wxStaticBoxSizer(wxHORIZONTAL, MiscPage, _T("Fcc"));
	FlexGridSizer15 = new wxFlexGridSizer(1, 6, 0, 0);
	FlexGridSizer15->AddGrowableCol(4);
	ST51 = new wxStaticText(MiscPage, ID_ST51, _T("V"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST51"));
	ST51->SetLabel(FFQS(SID_PRESET_FOURCC_VIDS));
	FourCCSizer->GetStaticBox()->SetLabel(FFQS(SID_PRESET_DEFINE_FOURCC));
	FlexGridSizer15->Add(ST51, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FOURCC_Vid = new wxTextCtrl(MiscPage, ID_FOURCC_VID, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FOURCC_VID"));
	FOURCC_Vid->SetMaxLength(4);
	FlexGridSizer15->Add(FOURCC_Vid, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer15->Add(25,-1,1, wxALL|wxEXPAND, 0);
	ST52 = new wxStaticText(MiscPage, ID_ST52, _T("A"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST52"));
	ST52->SetLabel(FFQS(SID_PRESET_FOURCC_AUDS));
	FlexGridSizer15->Add(ST52, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FOURCC_Aud = new wxTextCtrl(MiscPage, ID_FOURCC_AUD, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FOURCC_AUD"));
	FOURCC_Aud->SetMaxLength(4);
	FlexGridSizer15->Add(FOURCC_Aud, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer15->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	FourCCSizer->Add(FlexGridSizer15, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer14->Add(FourCCSizer, 1, wxALL|wxEXPAND, 5);
	SBS52 = new wxStaticBoxSizer(wxHORIZONTAL, MiscPage, _T("CArgs"));
	FlexGridSizer20 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer20->AddGrowableCol(1);
	CST1 = new wxStaticText(MiscPage, ID_CST1, _T("P1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_CST1"));
	SBS52->GetStaticBox()->SetLabel(FFQS(SID_PRESET_CUSTOM_CMDLINE_ARGS));
	CST1->SetLabel(FFQS(SID_PRESET_CUSTOM_ARGS_PASS_1));
	FlexGridSizer20->Add(CST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	CustomArgs1 = new wxTextCtrl(MiscPage, ID_CUSTOM_ARGS_1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CUSTOM_ARGS_1"));
	SBS52->GetStaticBox()->SetLabel(FFQS(SID_PRESET_CUSTOM_CMDLINE_ARGS));
	FlexGridSizer20->Add(CustomArgs1, 1, wxALL|wxEXPAND, 3);
	CST2 = new wxStaticText(MiscPage, ID_CST2, _T("P2"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_CST2"));
	CST2->SetLabel(FFQS(SID_PRESET_CUSTOM_ARGS_PASS_2));
	FlexGridSizer20->Add(CST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	CustomArgs2 = new wxTextCtrl(MiscPage, ID_CUSTOM_ARGS_2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CUSTOM_ARGS_2"));
	FlexGridSizer20->Add(CustomArgs2, 1, wxALL|wxEXPAND, 3);
	SBS52->Add(FlexGridSizer20, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer14->Add(SBS52, 1, wxALL|wxEXPAND, 5);
	SegmentingSizer = new wxStaticBoxSizer(wxVERTICAL, MiscPage, _T("Seg"));
	FlexGridSizer100 = new wxFlexGridSizer(1, 5, 0, 0);
	FlexGridSizer100->AddGrowableCol(1);
	SegST1 = new wxStaticText(MiscPage, wxID_ANY, _T("SLen"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	SegmentingSizer->GetStaticBox()->SetLabel(FFQS(SID_PRESET_SEGMENTING));
	SegST1->SetLabel(FFQS(SID_PRESET_SEGMENTING_LEN));
	FlexGridSizer100->Add(SegST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SegmentLen = new wxTextCtrl(MiscPage, ID_SEGMENTLEN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEGMENTLEN"));
	FlexGridSizer100->Add(SegmentLen, 1, wxALL|wxEXPAND, 3);
	SegmentLenType = new wxChoice(MiscPage, ID_SEGMENTLENTYPE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_SEGMENTLENTYPE"));
	FlexGridSizer100->Add(SegmentLenType, 1, wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	SegST2 = new wxStaticText(MiscPage, wxID_ANY, _T("Lf"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	SegST2->SetLabel(FFQS(SID_PRESET_SEGMENTING_LIST_FILE));
	FlexGridSizer100->Add(SegST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SegmentListType = new wxChoice(MiscPage, ID_SEGMENTLISTTYPE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_SEGMENTLISTTYPE"));
	FlexGridSizer100->Add(SegmentListType, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SegmentingSizer->Add(FlexGridSizer100, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer2 = new wxFlexGridSizer(1, 4, 0, 0);
	SegmentResetTS = new wxCheckBox(MiscPage, ID_SEGMENTRESETTS, _T("RsTS"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEGMENTRESETTS"));
	SegmentResetTS->SetValue(false);
	SegmentResetTS->SetLabel(FFQS(SID_PRESET_SEGMENTING_RESET_TIME));
	FlexGridSizer2->Add(SegmentResetTS, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SegmentIncTime = new wxCheckBox(MiscPage, ID_SEGMENTINCTIME, _T("Itc"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEGMENTINCTIME"));
	SegmentIncTime->SetValue(false);
	SegmentIncTime->SetLabel(FFQS(SID_PRESET_SEGMENTING_INCREMENT_TIME));
	FlexGridSizer2->Add(SegmentIncTime, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SegmentStreaming = new wxCheckBox(MiscPage, ID_SEGMENTSTREAMING, _T("Str"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEGMENTSTREAMING"));
	SegmentStreaming->SetValue(false);
	SegmentStreaming->SetLabel(FFQS(SID_PRESET_SEGMENTING_STREAMING));
	FlexGridSizer2->Add(SegmentStreaming, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SegmentBreakB = new wxCheckBox(MiscPage, ID_SEGMENTBREAKB, _T("Bnkf"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEGMENTBREAKB"));
	SegmentBreakB->SetValue(false);
	SegmentBreakB->SetLabel(FFQS(SID_PRESET_SEGMENTING_BREAK_BFRAMES));
	FlexGridSizer2->Add(SegmentBreakB, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SegmentingSizer->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer14->Add(SegmentingSizer, 1, wxALL|wxEXPAND, 5);
	SBS51 = new wxStaticBoxSizer(wxVERTICAL, MiscPage, _T("Ot"));
	FlexGridSizer28 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer28->AddGrowableCol(0);
	FlexGridSizer28->AddGrowableRow(0);
	FlexGridSizer28->AddGrowableRow(1);
	FlexGridSizer26 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer26->AddGrowableCol(1);
	ST55 = new wxStaticText(MiscPage, ID_STATICTEXT13, _T("Ar"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	SBS51->GetStaticBox()->SetLabel(FFQS(SID_PRESET_OTHER));
	ST55->SetLabel(FFQS(SID_PRESET_ASPECT_RATIO));
	FlexGridSizer26->Add(ST55, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer33 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer33->AddGrowableCol(1);
	Aspect = new wxComboBox(MiscPage, ID_ASPECT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_ASPECT"));
	Aspect->Append(_T("3:2"));
	Aspect->Append(_T("4:3"));
	Aspect->Append(_T("5:3"));
	Aspect->Append(_T("5:4"));
	Aspect->Append(_T("16:9"));
	Aspect->Append(_T("16:10"));
	Aspect->SetMinSize(wxSize(100,-1));
	FlexGridSizer33->Add(Aspect, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	DispositionsBtn = new wxButton(MiscPage, ID_DISPOSITIONSBTN, _T("Dsp"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DISPOSITIONSBTN"));
	DispositionsBtn->SetLabel(FFQS(SID_PRESET_DISPOSITION));
	FlexGridSizer33->Add(DispositionsBtn, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer26->Add(FlexGridSizer33, 1, wxALL|wxEXPAND, 3);
	ST56 = new wxStaticText(MiscPage, wxID_ANY, _T("Of"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	ST56->SetLabel(FFQS(SID_PRESET_OUTPUT_FORMAT));
	FlexGridSizer26->Add(ST56, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	OutputFormat = new wxComboBox(MiscPage, ID_OUTPUTFORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_OUTPUTFORMAT"));
	FlexGridSizer26->Add(OutputFormat, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer28->Add(FlexGridSizer26, 1, wxALL|wxEXPAND, 0);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	MF_FastStart = new wxCheckBox(MiscPage, ID_MF_FASTSTART, _T("Fs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MF_FASTSTART"));
	MF_FastStart->SetValue(false);
	MF_FastStart->SetLabel(FFQS(SID_PRESET_MOVFLAGS_FASTSTART));
	BoxSizer2->Add(MF_FastStart, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT, 3);
	KeepFileTime = new wxCheckBox(MiscPage, ID_KEEPFILETIME, _T("Kft"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_KEEPFILETIME"));
	KeepFileTime->SetValue(false);
	KeepFileTime->SetLabel(FFQS(SID_PRESET_KEEP_FILETIME));
	BoxSizer2->Add(KeepFileTime, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT, 3);
	StopEncShortest = new wxCheckBox(MiscPage, ID_STOPENCSHORTEST, _T("Sts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_STOPENCSHORTEST"));
	StopEncShortest->SetValue(false);
	StopEncShortest->SetLabel(FFQS(SID_PRESET_STOP_ENCODE_SHORTEST));
	BoxSizer2->Add(StopEncShortest, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT, 3);
	FlexGridSizer28->Add(BoxSizer2, 1, wxALL|wxEXPAND, 0);
	SBS51->Add(FlexGridSizer28, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer14->Add(SBS51, 1, wxALL|wxEXPAND, 5);
	MiscPage->SetSizer(FlexGridSizer14);
	FlexGridSizer14->Fit(MiscPage);
	FlexGridSizer14->SetSizeHints(MiscPage);
	Pages->AddPage(VideoPage, _T("Vid"), false);
	Pages->AddPage(AudioPage, _T("Aud"), false);
	Pages->AddPage(SubtitlesPage, _T("Sub"), false);
	Pages->AddPage(FilterPage, _T("Flt"), false);
	Pages->AddPage(MetaDataPage, _T("MD"), false);
	Pages->AddPage(ThumbsPage, _T("Tbs"), false);
	Pages->AddPage(MiscPage, _T("Msc"), false);
	FlexGridSizer1->Add(Pages, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OKButton = new wxButton(this, ID_OKBUTTON, _T("O"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBUTTON"));
	OKButton->SetDefault();
	OKButton->SetLabel(FFQS(SID_COMMON_OK));
	BoxSizer1->Add(OKButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton(this, ID_CANCELBUTTON, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBUTTON"));
	CancelButton->SetLabel(FFQS(SID_COMMON_CANCEL));
	BoxSizer1->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	OpenFileDlg = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	OpenFileDlg->SetMessage(FFQS(SID_PRESET_SELECT_PREVIEW_FILE));
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_VIDEOCODEC,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_FULLSPECVIDBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_TWOPASS,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_USEVIDEOQSCALE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_VIDEOQSCALE,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_USECONSTRATE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_CONSTRATE,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_USEMINCONSTRATE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_MINCONSTRATE,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_USEQMIN,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_QMIN,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_USEQMAX,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_QMAX,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_USEQDIFF,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_QDIFF,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_USESCENECHANGESENSITIVITY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_SCENECHANGESENS,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_PIXFMT,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_PIXFMT,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_AUDIOCODEC,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_FULLSPECAUDBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_USEAUDIOQSCALE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_AUDIOQSCALE,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_SUBSCODEC,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_SUBSTEXT,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_SUBSBITMAP,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_SUBSSIZEDEF,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_SUBSSIZEPAL,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_SUBSSIZECUST,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_SUBSSCALE,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&FFQPresetEdit::OnSubsScaleChange);
	Connect(ID_FILTERLIST,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_FILTERLIST,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_ADDFILTERBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_EDITFILTERBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_REMOVEFILTERBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_FILTERUPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_FILTERDOWNBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_FILTERPREVIEWBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_METADATA,wxEVT_GRID_CELL_CHANGED,(wxObjectEventFunction)&FFQPresetEdit::OnMetaDataCellChange);
	Connect(ID_CLEARMETADATABTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_METADATAFOR,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&FFQPresetEdit::OnChoiceChange);
	Connect(ID_DISPOSITIONSBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_PAGES,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&FFQPresetEdit::OnNotebookPageChanged);
	Connect(ID_OKBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(ID_CANCELBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetEdit::OnButtonClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&FFQPresetEdit::OnClose);
	//*)

	SetTitle(FFQS(SID_PRESET_TITLE));

	//EXPERIMENTAL START
	wxDisplay dsp(wxDisplay::GetFromWindow(this));
	int w = dsp.GetClientArea().GetWidth() / 3;
	wxSize sz = PresetName->GetSize();
	if (w > sz.GetWidth()) PresetName->SetSize(wxSize(w, sz.GetHeight()));
	//EXPERIMENTAL END

	Pages->SetPageText(Pages->FindPage(VideoPage), FFQS(SID_COMMON_VIDEO));
	Pages->SetPageText(Pages->FindPage(AudioPage), FFQS(SID_COMMON_AUDIO));
	Pages->SetPageText(Pages->FindPage(SubtitlesPage), FFQS(SID_COMMON_SUBTITLES));
	Pages->SetPageText(Pages->FindPage(FilterPage), FFQS(SID_PRESET_FILTERS));
	Pages->SetPageText(Pages->FindPage(MetaDataPage), FFQS(SID_PRESET_METADATA));
	Pages->SetPageText(Pages->FindPage(ThumbsPage), FFQS(SID_PRESET_THUMBS));
	Pages->SetPageText(Pages->FindPage(MiscPage), FFQS(SID_PRESET_MISCELLANEOUS));

	VideoPages->SetPageText(VideoPages->FindPage(BitRatePage), FFQS(SID_PRESET_BITRATE));
	VideoPages->SetPageText(VideoPages->FindPage(QualityPage), FFQS(SID_PRESET_QUALITY));
	VideoPages->SetPageText(VideoPages->FindPage(KeyFramesPage), FFQS(SID_PRESET_KEYFRAMES));
	VideoPages->SetPageText(VideoPages->FindPage(OtherPage), FFQS(SID_PRESET_OTHER));
	VideoPages->SetPageText(VideoPages->FindPage(HWDecodePage), FFQS(SID_PRESET_HWDECODE));

	wxMenu *vid = new wxMenu(), *aud = new wxMenu(), *cur = vid;

	AddFilterMenu.Append(wxID_ANY, FFQS(SID_COMMON_VIDEO), vid);
	AddFilterMenu.Append(wxID_ANY, FFQS(SID_COMMON_AUDIO), aud);

	int sep = 0;

	for (unsigned int i = 0; i < FILTER_ORDER_COUNT; i++)
    {

        int f = FILTER_ORDER[i];

        if (f == -1)
        {

            if (sep++ == 0) cur = aud;
            else cur = &AddFilterMenu;

        }

        else
        {

            wxMenuItem *mi = new wxMenuItem(cur, FILTER_MENU_BASE_ID + f, FFQL()->FILTER_NAMES[f], wxEmptyString, wxITEM_NORMAL);
            cur->Append(mi);

        }

    }

    //Get items for B- and P-frame definition
    wxString *as = FFQL()->GetStringArray(SID_PRESET_KEYFRAME_MAX_B_FRAME_ITEMS, 4);

    //Items for B-frames
    KeyFrameMaxB->Append(as[0]);
    KeyFrameMaxB->Append(as[1]);
    KeyFrameMaxB->Append(as[2]);
    for (unsigned int i = 1; i < 17; i++) KeyFrameMaxB->Append(wxString::Format(as[3], i));

    //Items for P-frames
    KeyFrameMaxP->Append(as[0]);
    KeyFrameMaxP->Append(as[2]);
    for (unsigned int i = 1; i < 17; i++) KeyFrameMaxP->Append(wxString::Format(as[3], i));

    //Release string array
    delete[] as;

    //Set items for meta data selection
    as = FFQL()->GetStringArray(SID_PRESET_METADATA_FOR_ITEMS, 4);
    for (unsigned int i = 0; i < 4; i++) MetaDataFor->Append(as[i]);
    delete[] as;
    m_MetaData.SetCount(4); //Global, Video, Audio, Subtitles

    //Items for dispositions
    as = FFQL()->GetStringArray(SID_PRESET_METADATA_FOR_ITEMS, 4);
    for (unsigned int i = 0; i < 3; i++)
    {
        m_DispositionMenus[i] = new wxMenu();
        m_DispositionMenus[i]->AppendRadioItem(i * DISPOSITION_MENU_ID_COUNT, FFQS(SID_COMMON_UNDEFINED));
        for (unsigned int ii = 0; ii <= DISPOSITION_COUNT; ii++)
        {
            m_DispositionMenus[i]->AppendRadioItem((i * DISPOSITION_MENU_ID_COUNT) + (ii + 1), ii == 0 ? FFQS(SID_PRESET_DISPOSITION_UNSET) : DISPOSITIONS[ii-1]);
        }
        DispositionsMenu.AppendSubMenu(m_DispositionMenus[i], as[i+1]);
    }
    delete[] as;

    //Bind menu handler for both filters and dispositions
    Bind(wxEVT_COMMAND_MENU_SELECTED, &FFQPresetEdit::OnMenuSelected, this);

	FFQCFG()->SetBrowseRootFor(OpenFileDlg);

    FilterEditor = NULL;

	m_UIntVal.SetRange(0, 99999);
	m_UIntVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK);

	KeyFrameMin->SetValidator(m_UIntVal);
	KeyFrameMax->SetValidator(m_UIntVal);

	AudioChannels->SetValidator(m_UIntVal);

	SubsWidth->SetValidator(m_UIntVal);
	SubsHeight->SetValidator(m_UIntVal);
	wxString ce = FFQConfig::GetInstance()->subs_charenc;
	while (ce.Len() > 0) SubsCharEnc->Append(GetToken(ce, SUBS_CHARENC_SEP));

	MetaData->SetMaxSize(MetaData->GetSize());
	FilterList->SetMaxSize(FilterList->GetSize());

	SegmentLen->SetValidator(m_UIntVal);
	as = FFQL()->GetStringArray(SID_PRESET_SEGMENTING_LEN_TYPE, 3);
    for (unsigned int i = 0; i < 3; i++) SegmentLenType->Append(as[i]);
    delete[] as;
	as = FFQL()->GetStringArray(SID_PRESET_SEGMENTING_LIST_TYPE, 5);
    for (unsigned int i = 0; i < 5; i++) SegmentListType->Append(as[i]);
    delete[] as;

    //Prepare the full spec editor
    #ifndef DEBUG
    FFQFullSpec::Initialize(this);
    #endif

}

//---------------------------------------------------------------------------------------

FFQPresetEdit::~FFQPresetEdit()
{
	//(*Destroy(FFQPresetEdit)
	//*)
	if (FilterEditor != NULL)
    {
        delete FilterEditor;
        FilterEditor = NULL;
    }
}

//---------------------------------------------------------------------------------------

bool FFQPresetEdit::Execute(LPFFQ_PRESET preset)
{

    #ifdef DEBUG
    FFQFullSpec::Finalize();
    FFQFullSpec::Initialize(this);
    #endif // DEBUG

    //Store pointer to preset
    m_Preset = preset;

    //Variables for handling filter previews
    m_PreviewFile = "";
    m_ShowPreviewDlg = true;

    //Preset name and temp. preset
    PresetName->SetValue(preset->preset_name);
    PresetTemp->SetValue(preset->is_temporary);

    //Video options
    wxString s = FFQCFG()->GetFFMpegCodecs(ctVIDEO), t;
    SetItems(VideoCodec, s, preset->video_codec);
    m_FullSpecVCodec = preset->video_codec;
    m_FullSpecVid = preset->fullspec_vid;
    TwoPass->SetValue(preset->two_pass);
    TwoPassNull->SetValue(preset->two_pass_null);
    SkipEncodeSameVid->SetValue(preset->skip_encode_same & 2);

    //Video bit rate values
    VideoBitRate->SetValues(preset->video_rate);

    //Video quality settings
    s = preset->video_quality;
    //qscale
    UseVideoQScale->SetValue(GetToken(s, ',') == STR_YES);
    VideoQScale->SetValue(Str2Long(GetToken(s, ','), 75));
    //crf
    //ShowInfo("CRF: " + s);
    UseConstRate->SetValue(GetToken(s, ',') == STR_YES);
    ConstRate->SetValue(Str2Long(GetToken(s, ','), 75));
    //crf_min
    UseMinConstRate->SetValue(GetToken(s, ',') == STR_YES);
    MinConstRate->SetValue(Str2Long(GetToken(s, ','), 50));
    //qmin
    UseQMin->SetValue(GetToken(s, ',') == STR_YES);
    QMin->SetValue(Str2Long(GetToken(s, ','), 90));
    //qmax
    UseQMax->SetValue(GetToken(s, ',') == STR_YES);
    QMax->SetValue(Str2Long(GetToken(s, ','), 40));
    //qdiff
    UseQDiff->SetValue(GetToken(s, ',') == STR_YES);
    QDiff->SetValue(Str2Long(GetToken(s, ','), 10));

    //Key frames settings
    s = preset->key_frames;
    KeyFrameMin->SetValue(GetToken(s, ","));
    KeyFrameMax->SetValue(GetToken(s, ","));
    MotionEstimation->SetValue(GetToken(s, ","));
    UseSceneChangeSens->SetValue(GetToken(s, ",") == STR_YES);
    SceneChangeSens->SetValue(Str2Long(GetToken(s, ","), 40));
    KeyFrameMaxB->SetSelection(Str2Long(GetToken(s, ","), 0));
    KeyFrameMaxP->SetSelection(Str2Long(GetToken(s, ","), 0));
    UseClosedGOP->SetValue(GetToken(s, ",") == STR_YES);

    //Other video settings
    VideoSync->SetSelection(0);
    if (preset->video_sync.Len() > 0) for (unsigned int i = 0; i < VideoSync->GetCount(); i++)
    {
        if (VideoSync->GetString(i).BeforeFirst(' ').Lower() == preset->video_sync)
        {
            VideoSync->SetSelection(i);
            break;
        }
    }
    FrameRate->SetValue(preset->frame_rate);
    LPPIXEL_FORMAT pf = FFQCFG()->GetPixelFormats();
    //s = FFQCFG()->getFFMpegPixelFormats();
    PixFmt->Freeze();
    PixFmt->Clear();
    //while (s.Len() > 0) PixFmt->Append(GetLine(s, true));
    while (pf)
    {
        if (pf->out) PixFmt->Append(pf->name, pf);
        pf = pf->GetNext();
    }
    PixFmt->Thaw();
    PixFmt->SetValue(preset->pixel_format);
    Trellis->SetSelection(Str2Long(preset->trellis, -1) + 1);
    SpeedPreset->SetValue(preset->speed_preset);
    VideoTuning->SetValue(preset->video_tuning);
    VideoThreads->SetValue(preset->threads);

    //HW Decode settings
    wxString hwd = preset->hw_decode;
    s = FFQCFG()->GetHWAccelerators();
    t = GetToken(hwd, ",", true);
    SetItems(HWD_Accel, s, t, false);
    s = FFQCFG()->GetHWDecoders();
    t = GetToken(hwd, ",", true);
    SetItems(HWD_Codec, s, t, false);
    HWD_Device->SetValue(GetToken(hwd, ",", true));
    HWD_DeviceInit->SetValue(hwd);


    //Audio settings
    //Codec
    s = FFQCFG()->GetFFMpegCodecs(ctAUDIO);
    SetItems(AudioCodec, s, preset->audio_codec);
    m_FullSpecACodec = preset->audio_codec;
    m_FullSpecAud = preset->fullspec_aud;
    SkipEncodeSameAud->SetValue(preset->skip_encode_same & 1);

    //Bit rate
    AudioBitRate->SetValues(preset->audio_rate);

    //Quality
    s = preset->audio_quality;
    UseAudioQScale->SetValue(GetToken(s, ',') == STR_YES);
    AudioQScale->SetValue(Str2Long(GetToken(s, ','), 80));

    //Audio other
    AudioChannels->SetValue(preset->audio_channels);
    AudioProfile->SetValue(preset->audio_profile);


    //Subtitles settings
    s = FFQCFG()->GetFFMpegCodecs(ctSUBTITLE);
    SetItems(SubsCodec, s, preset->subtitles.codec);
    SubsText->SetValue(!preset->subtitles.bitmap);
    SubsBitmap->SetValue(preset->subtitles.bitmap);
    SubsScale->SetValue(preset->subtitles.scale);
    SubsSizeDef->SetValue(preset->subtitles.size_type == 0);
    SubsSizePAL->SetValue(preset->subtitles.size_type == 1);
    SubsSizeCust->SetValue(preset->subtitles.size_type == 2);
    SubsWidth->SetValue(preset->subtitles.width);
    SubsHeight->SetValue(preset->subtitles.height);
    SubsCharEnc->SetValue(preset->subtitles.charenc);


    //Dispositions
    s = preset->disposition;
    for (int i = 0; i < 3; i++)
    {
        m_DispositionSelection[i] = Str2Long(GetToken(s, ','), -1) + 1;
        //if (m_DispositionSelection[i] < 0) m_DispositionSelection[i] = 0;
        wxMenuItem *mi = m_DispositionMenus[i]->FindChildItem((i * DISPOSITION_MENU_ID_COUNT) + m_DispositionSelection[i]);
        if (mi) mi->Check();
        else m_DispositionMenus[i]->FindItemByPosition(0)->Check();
    }


    //Filter settings
    FFMPEG_FILTER fltr;
    FilterList->Freeze();
    for (unsigned int i = 0; i < preset->filters.size(); i++)
    {
        wxString *f = new wxString(preset->filters[i]);
        fltr = FFMPEG_FILTER(*f);
        if (fltr.type == ftSUBSBURNIN) FilterList->Append(FFQS(SID_SUBSBURNIN_USERFRIENDLY), (void*)f);
        else FilterList->Append(fltr.friendly, (void*)f);
    }
    FilterList->Thaw();
    AudioFiltersComplex->SetValue(preset->af_complex);


    //Metadata settings
    m_MetaData[0] = preset->meta_data;
    m_MetaData[1] = preset->meta_data_v;
    m_MetaData[2] = preset->meta_data_a;
    m_MetaData[3] = preset->meta_data_s;
    m_SelMetaData = 0;
    MetaDataFor->SetSelection(m_SelMetaData);
    SetMetaData(m_MetaData[m_SelMetaData]);
    wxClientDC dc(MetaData);
    wxSize ts = dc.GetTextExtent("W");
    MetaData->SetColSize(0, ts.x * 14);
    MetaData->SetColSize(1, ts.x * 6);
    MetaData->SetColSize(2, Pages->GetClientSize().x - MetaData->GetColLabelSize() - (ts.x * 20));
    //MetaData->SetColSize(2, MetaData->GetGrandParent()->GetSize().GetWidth() - MetaData->GetColLabelSize() - 320);
    //MetaData->SetColSize(1, MetaData->GetClientRect().GetWidth() - MetaData->GetColLabelSize() - 130);


    //Miscellaneous settings
    FOURCC_Vid->SetValue(preset->fourcc.vids);
    FOURCC_Aud->SetValue(preset->fourcc.auds);
    CustomArgs1->SetValue(preset->custom_args_1);
    CustomArgs2->SetValue(preset->custom_args_2);
    Aspect->SetValue(preset->aspect_ratio);
    MF_FastStart->SetValue(preset->mf_faststart);
    KeepFileTime->SetValue(preset->keep_filetime);
    StopEncShortest->SetValue(preset->shortest);
    OutputFormat->Clear();
    s = FFQCFG()->GetFFMpegFormats();
    int sel = -1;
    m_CanSegment = false;
    m_CanSSegment = false;
    while (s.Len() > 0)
    {
        t = GetLine(s, true);
        if (t == "segment") m_CanSegment = true;
        else if (t == "stream_segment") m_CanSSegment = true;
        else
        {
            if ( (preset->output_format.Len() > 0) && t.StartsWith(preset->output_format + SPACE)) sel = OutputFormat->GetCount();
            OutputFormat->Append(t);
        }
    }
    if (sel >= 0) OutputFormat->SetSelection(sel);
    else OutputFormat->SetValue(preset->output_format);

    SegmentLen->SetValue(wxString::Format("%u", preset->segmenting.length));
    SegmentLenType->SetSelection(preset->segmenting.length_type);
    SegmentListType->SetSelection(preset->segmenting.list_file);
    SegmentBreakB->SetValue(preset->segmenting.break_bframes);
    SegmentIncTime->SetValue(preset->segmenting.incremental_tc);
    SegmentStreaming->SetValue(preset->segmenting.streaming);
    SegmentResetTS->SetValue(preset->segmenting.reset_ts);

    //Thumbs settings
    ThumbsPanel->SetValues(preset->thumbs);


    //Select initial pages
    Pages->SetSelection(0);
    VideoPages->SetSelection(0);
    PresetName->SetFocus();


    //Update codec info, subtitle filter and validate controls
    UpdateCodecInfo();
    UpdateFilterMenu();
    UpdateSubtitleFilter(); //Just in case it has not yet been added


    //Tidy
    UpdateControls(true);
    CenterOnParent();


    //Show dialog
    bool ok = (ShowModal() == wxID_OK);

    if (ok)
    {

        //Save preset settings
        preset->preset_name = StrTrim(PresetName->GetValue());
        preset->is_temporary = PresetTemp->GetValue();
        preset->skip_encode_same = 0;

        //Save video settings
        GetCodecNameAndID(VideoCodec->GetStringSelection(), preset->video_codec, preset->video_codec_id);
        //preset->video_codec = VideoCodec->GetStringSelection().BeforeFirst(SPACE);
        preset->fullspec_vid = (preset->video_codec == m_FullSpecVCodec) ?  m_FullSpecVid : "";
        preset->two_pass = TwoPass->GetValue();
        preset->two_pass_null = TwoPassNull->GetValue();
        if (SkipEncodeSameVid->IsEnabled() && SkipEncodeSameVid->GetValue()) preset->skip_encode_same |= 2;

        //Video bit rate
        VideoBitRate->GetValues(preset->video_rate, false);

        //Video quality settings
        preset->video_quality  = BOOLSTR(UseVideoQScale->GetValue()) + COMMA + ToStr(VideoQScale->GetValue()) + COMMA; //qscale
        preset->video_quality += BOOLSTR(UseConstRate->GetValue()) + COMMA + ToStr(ConstRate->GetValue()) + COMMA; //crf
        preset->video_quality += BOOLSTR(UseMinConstRate->GetValue()) + COMMA + ToStr(MinConstRate->GetValue()) + COMMA; //crf_min
        preset->video_quality += BOOLSTR(UseQMin->GetValue()) + COMMA + ToStr(QMin->GetValue()) + COMMA; //qmin
        preset->video_quality += BOOLSTR(UseQMax->GetValue()) + COMMA + ToStr(QMax->GetValue()) + COMMA; //qmax
        preset->video_quality += BOOLSTR(UseQDiff->GetValue()) + COMMA + ToStr(QDiff->GetValue()); //qdiff

        //Key frames settings
        preset->key_frames  = KeyFrameMin->GetValue() + COMMA + KeyFrameMax->GetValue() + COMMA;
        preset->key_frames += StrTrim(MotionEstimation->GetValue()) + COMMA;
        preset->key_frames += BOOLSTR(UseSceneChangeSens->GetValue()) + COMMA;
        preset->key_frames += ToStr(SceneChangeSens->GetValue()) + COMMA;
        preset->key_frames += ToStr(KeyFrameMaxB->GetSelection()) + COMMA;
        preset->key_frames += ToStr(KeyFrameMaxP->GetSelection()) + COMMA;
        preset->key_frames += BOOLSTR(UseClosedGOP->GetValue());

        //Other video settings
        unsigned int sel = VideoSync->GetSelection();
        preset->video_sync = (sel == 0) ? "" : VideoSync->GetString(sel).BeforeFirst(SPACE).Lower();
        preset->frame_rate = StrTrim(FrameRate->GetValue());
        preset->pixel_format = StrTrim(PixFmt->GetValue());
        sel = Trellis->GetSelection();
        preset->trellis = (sel == 0) ? "" : ToStr(sel - 1);
        preset->speed_preset = StrTrim(SpeedPreset->GetValue());
        preset->video_tuning = StrTrim(VideoTuning->GetValue());
        preset->threads = StrTrim(VideoThreads->GetValue());

        //HW Decode settings
        preset->hw_decode = HWD_Accel->GetString(HWD_Accel->GetSelection()) + COMMA;
        s = HWD_Codec->GetValue();
        preset->hw_decode += GetToken(s, SPACE, false) + COMMA;
        preset->hw_decode += HWD_Device->GetValue() + COMMA + HWD_DeviceInit->GetValue();


        //Audio settings
        GetCodecNameAndID(AudioCodec->GetStringSelection(), preset->audio_codec, preset->audio_codec_id);
        preset->fullspec_aud = (preset->audio_codec == m_FullSpecACodec) ?  m_FullSpecAud : "";
        if (SkipEncodeSameAud->IsEnabled() && SkipEncodeSameAud->GetValue()) preset->skip_encode_same |= 1;
        //preset->audio_codec = AudioCodec->GetStringSelection().BeforeFirst(SPACE);

        //Audio bit rate
        AudioBitRate->GetValues(preset->audio_rate, false);

        //Audio quality
        preset->audio_quality = BOOLSTR(UseAudioQScale->GetValue()) + COMMA + ToStr(AudioQScale->GetValue()); //qscale

        //Audio other
        preset->audio_channels = StrTrim(AudioChannels->GetValue());
        preset->audio_profile = StrTrim(AudioProfile->GetValue());

        //Subtitles settings
        preset->subtitles.codec = SubsCodec->GetStringSelection().BeforeFirst(SPACE);
        preset->subtitles.bitmap = SubsBitmap->GetValue();
        preset->subtitles.scale = SubsScale->GetValue();
        if (SubsSizePAL->GetValue()) preset->subtitles.size_type = 1;
        else if (SubsSizeCust->GetValue()) preset->subtitles.size_type = 2;
        else preset->subtitles.size_type = 0; //Default
        preset->subtitles.width = SubsWidth->GetValue();
        preset->subtitles.height = SubsHeight->GetValue();
        preset->subtitles.charenc = SubsCharEnc->GetValue();//StringSelection();
        if (SubsCharEnc->FindString(preset->subtitles.charenc) == wxNOT_FOUND)
        {
            SubsCharEnc->Append(preset->subtitles.charenc);
            wxString ce = "";
            for (unsigned int i = 0; i < SubsCharEnc->GetCount(); i++) ce += SubsCharEnc->GetString(i) + SUBS_CHARENC_SEP;
            FFQConfig::GetInstance()->subs_charenc = ce.BeforeLast(SUBS_CHARENC_SEP);
            FFQConfig::GetInstance()->SaveConfig();
        }

        //Dispositions
        preset->disposition  = (m_DispositionSelection[0] > 0 ? ToStr(m_DispositionSelection[0] - 1) : "") + COMMA;
        preset->disposition += (m_DispositionSelection[1] > 0 ? ToStr(m_DispositionSelection[1] - 1) : "") + COMMA;
        preset->disposition += (m_DispositionSelection[2] > 0 ? ToStr(m_DispositionSelection[2] - 1) : "");

        //Filters
        preset->filters.Clear();
        for (unsigned int i = 0; i < FilterList->GetCount(); i++)
            preset->filters.Add(*((wxString*)FilterList->GetClientData(i)));
        preset->af_complex = AudioFiltersComplex->GetValue();

        //Metadata settings
        preset->meta_data = m_MetaData[0];
        preset->meta_data_v = m_MetaData[1];
        preset->meta_data_a = m_MetaData[2];
        preset->meta_data_s = m_MetaData[3];

        //Thumbs settings
        ThumbsPanel->GetValues(preset->thumbs, false);

        //Miscellaneous settings
        preset->fourcc.vids = StrTrim(FOURCC_Vid->GetValue());
        preset->fourcc.auds = StrTrim(FOURCC_Aud->GetValue());
        preset->custom_args_1 = StrTrim(CustomArgs1->GetValue());
        preset->custom_args_2 = StrTrim(CustomArgs2->GetValue());
        preset->aspect_ratio = StrTrim(Aspect->GetValue());
        preset->mf_faststart = MF_FastStart->GetValue();
        preset->keep_filetime = KeepFileTime->GetValue();
        preset->shortest = StopEncShortest->GetValue();
        s = OutputFormat->GetValue();
        preset->output_format = GetToken(s, SPACE);
        preset->segmenting.length = Str2Long(SegmentLen->GetValue(), 0);
        preset->segmenting.length_type = SegmentLenType->GetSelection();
        preset->segmenting.list_file = SegmentListType->GetSelection();
        preset->segmenting.break_bframes = SegmentBreakB->GetValue();
        preset->segmenting.incremental_tc = SegmentIncTime->GetValue();
        preset->segmenting.streaming = SegmentStreaming->GetValue();
        preset->segmenting.reset_ts = SegmentResetTS->GetValue();

    }

    //Clear video filters
    FilterList->Freeze();
    for (unsigned int i = 0; i < FilterList->GetCount(); i++) delete (wxString*)FilterList->GetClientData(i);
    FilterList->Clear();
    FilterList->Thaw();

    return ok;

}

//---------------------------------------------------------------------------------------

bool FFQPresetEdit::EditFilter(LPFFMPEG_FILTER filter)
{
    if (FilterEditor == NULL) FilterEditor = new FFQFilterEdit(this);
    return FilterEditor->Execute(filter);
}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::EditFullSpec(wxString selected_codec, int fs_idx, wxString &for_codec, wxString &spec)
{

    //Get selected_codec for comparison
    wxString codec, fs;
    GetCodecNameAndID(selected_codec, codec, fs); //fs used as dummy here

    //If the selected codec matches the stored one then use it otherwise use empty string
    fs = (for_codec == codec) ? spec : "";

    //Launch full spec UI
    FFQFullSpec *dlg = new FFQFullSpec(this);
    if (dlg->Execute(fs_idx, fs))
    {

        //If success, store the stuff back
        spec = fs;
        for_codec = codec;

    }

    //Do not keep the dialog in memory
    delete dlg;

}

//---------------------------------------------------------------------------------------

int FFQPresetEdit::FindFilter(FILTER_TYPE ft)
{
    for (unsigned int i = 0; i < FilterList->GetCount(); i++)
    {
        wxString *fs = (wxString*)FilterList->GetClientData(i);
        FFMPEG_FILTER fltr = FFMPEG_FILTER(*fs);
        if (fltr.type == ft) return (int)i;
    }
    return -1;
}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::PreviewFilters()
{

    wxString cmd = FFQCFG()->GetFFPlayCommand(true);
    if (cmd.Len() == 0) return;

    if (m_PreviewFile.Len() == 0)
    {
        /*wxWindow *p = GetParent();
        if (p) while (p->GetParent()) p = p->GetParent();
        if (p)
        {
            LPFFQ_QUEUE_ITEM item = ((FFQMain*)p)->GetSelectedItem();
            if (item) m_PreviewFile = item->GetInput(0).path;
        }*/
        LPFFQ_QUEUE_ITEM item = FFQMain::GetInstance()->GetSelectedItem();
        if (item) m_PreviewFile = item->GetInput(0).path;
    }

    if (m_ShowPreviewDlg)
    {
        OpenFileDlg->SetPath(m_PreviewFile);
        if (OpenFileDlg->ShowModal() == wxID_CANCEL) return;
        m_PreviewFile = OpenFileDlg->GetPath();
        m_ShowPreviewDlg = false;
    }

    cmd += " \"" + m_PreviewFile + "\" -autoexit";

    wxString *fs, vf = "", af = "", vidIn = "[in]", audIn = "[in]", dummy = "";
    int FID = 1;

    for (unsigned int i = 0; i < FilterList->GetCount(); i++)
    {
        fs = (wxString*)FilterList->GetClientData(i);
        FFMPEG_FILTER fltr = FFMPEG_FILTER(*fs);
        if ((!fltr.IsComplex()) && (fltr.type != ftSUBSBURNIN))
        {
            if (fltr.IsVideo() && FormatFilter(fltr.ff_filter, vidIn, dummy, dummy, dummy, FID)) vf += fltr.ff_filter;
            else if (fltr.IsAudio() && FormatFilter(fltr.ff_filter, dummy, audIn, dummy, dummy, FID)) af += fltr.ff_filter;
        }
    }

    if ( (vf.Len() == 0) && (af.Len() == 0) )
    {
        ShowError(FFQS(SID_PRESET_NO_PREVIEWABLE_FILTERS));
        return;
    }

    if (vf.Len() > 0) cmd += " -vf \"" + vf.BeforeLast('[') + "\"";
    if (af.Len() > 0) cmd += " -af \"" + af.BeforeLast('[') + "\"";

    if (wxExecute(cmd) == 0) ShowError(FFQSF(SID_PRESET_PREVIEW_FAILED, cmd));

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::SetItems(wxItemContainer* ch, wxString &items, wxString &select, bool is_codec)
{
    //Freeze and clear
    if (dynamic_cast<wxChoice*>(ch)) ((wxChoice*)ch)->Freeze();
    if (dynamic_cast<wxComboBox*>(ch)) ((wxComboBox*)ch)->Freeze();
    ch->Clear();

    //Selection for later
    unsigned int sel = 0;

    if (is_codec)
    {
        //Set items in a (codec)choice and select the codec from the preset

        //Copy as first item
        ch->Append("copy - Copy unmodifed source stream");

        wxString cur, codec;
        unsigned int pref = 1, idx = 0;
        LPCODEC_INFO cinfo = FFQCFG()->GetCodecInfo(), ci;
        bool subs = (ch == SubsCodec), selected, showAll = FFQCFG()->full_codec_listings || subs;

        if (subs)
        {
            //If subtitles then the second item is BURN-IN
            ch->Append(CODEC_SUBS_BURNIN + " - Burn subtitles onto video");
            if (select == CODEC_SUBS_BURNIN) sel = 1;
        }

        while (items.Len() > 0)
        {

            //Get current codec from list
            cur = GetToken(items, '\n');
            codec = cur.BeforeFirst(' ');

            //Get codec info
            ci = cinfo->Find(codec);

            //Is the current codec selected?
            selected = (codec == select);

            //Insert or append if needed
            if (showAll || selected || ((ci != NULL) && ci->friendly))
            {

                if (ci != NULL)
                {

                    //Preferred codecs are listed in the top
                    idx = ch->Insert(cur, pref);
                    pref++;

                }
                else idx = ch->Append(cur); //Non-preferred codecs are just appended as they are thrown out from ffmpeg

                //Adjust selection
                if (selected) sel = idx;
                else if (sel >= idx) sel++;

            }

        }

        if ((sel == 0) && (select.Len() > 0))
        {
            //Find codec in another way (this might not work with user-friendly lists)
            wxString sl = "(codec " + select + ")";
            for (unsigned int i = 0; i < ch->GetCount(); i++) if (ch->GetString(i).Find(sl) > 0)
            {
                sel = i;
                break;
            }
        }
    }
    else
    {

        //Set items and select
        wxString s;
        ch->Append("");
        while (items.Len() > 0)
        {
            s = GetLine(items, true);
            if (select == s.BeforeFirst(' ')) sel = ch->GetCount();
            ch->Append(s);
        }

    }

    //Select and thaw
    ch->SetSelection(sel);
    if (dynamic_cast<wxChoice*>(ch)) ((wxChoice*)ch)->Thaw();
    if (dynamic_cast<wxComboBox*>(ch)) ((wxComboBox*)ch)->Thaw();

}

//---------------------------------------------------------------------------------------

bool ValidMetaData(wxString &d)
{
    for (unsigned int i = 0; i < d.Len(); i++) if (ILLEGAL_METADATA_CHARS.Find(d.GetChar(i)) >= 0) return false;
    return true;
}

//---------------------------------------------------------------------------------------

bool FFQPresetEdit::GetMetaData(wxString &data)
{

    wxString n, v, s;
    data = "";

    for (int i = 0; i < MetaData->GetNumberRows(); i++)
    {

        n = StrTrim(MetaData->GetCellValue(i, 0));
        s = StrTrim(MetaData->GetCellValue(i, 1));
        v = StrTrim(MetaData->GetCellValue(i, 2));

        if ((n.Len() > 0) && (v.Len() > 0)) //Skip blank values
        {

            if (!ValidMetaData(n)) //Check for invalid characters in the key
            {
                MetaData->GoToCell(i, 0);
                return false;
            }

            if ((s.Len() > 0) && (Str2Long(s, -1) < 0))
            {
                MetaData->GoToCell(i, 1);
                return false;
            }

            if (v.Find(FILTER_SEPARATOR) >= 0) //Check for separator-value which is illegal
            {
                MetaData->GoToCell(i, 2);
                return false;
            }

            if (data.Len() > 0) data += FILTER_SEPARATOR; //Append separator
            if (s.Len() > 0) n += PIPE + s;
            v.Replace("\"", "\\\""); //Escape quotes to prevent command-line skrew-ups
            data += n + "=\"" + v + "\""; //Values are always quoted

        }

    }

    return true;

}

//---------------------------------------------------------------------------------------

void EnsureGridRow(wxGrid *grid, const wxString key)
{
    for (int i = 0; i < grid->GetNumberRows(); i++) if (grid->GetCellValue(i,0).Lower() == key)
    {
        grid->SetCellValue(i, 0, key); //Ensure correct lettercasing
        return;
    }
    grid->AppendRows();
    grid->SetCellValue(grid->GetNumberRows() - 1, 0, key);
}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::SetMetaData(wxString data)
{

    MetaData->Freeze();

    //Clear all rows
    int row = MetaData->GetNumberRows();
    if (row > 0) MetaData->DeleteRows(0, row);

    wxString n, v, s;

    while (data.Len() > 0)
    {

        n = GetToken(data, EQUAL);
        s = (n.Find(PIPE) > 0) ? GetLastToken(n, PIPE, true) : "";
        v = GetToken(data, FILTER_SEPARATOR);

        if ((n.Len() > 0) && (v.Len() > 0))
        {

            //Remove quotes at the ends
            if (v.GetChar(0) == '\"') v.Remove(0, 1);
            if ((v.Len() > 0) && (v.GetChar(v.Len() - 1) == '\"')) v.Remove(v.Len() - 1, 1);

            //Remove escaped quote(s)
            v.Replace("\\\"", "\"");

            MetaData->AppendRows();
            row = MetaData->GetNumberRows() - 1;
            MetaData->SetCellValue(row, 0, n);
            MetaData->SetCellValue(row, 1, s);
            MetaData->SetCellValue(row, 2, v);

        }


    }

    n = METADATA_FIELDS;
    while (n.Len() > 0) EnsureGridRow(MetaData, GetToken(n, ","));
    MetaData->AppendRows();

    MetaData->Thaw();

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::UpdateCodecInfo()
{

    //Update the structures holding information about currently selected codecs
    LPCODEC_INFO ci, root = FFQCFG()->GetCodecInfo();

    //Video
    ci = root->Find(VideoCodec->GetStringSelection().BeforeFirst(' '));
    if (ci == NULL) ci = root->Find(DEFAULT_VIDEO_CODEC_INFO);
    //ShowInfo("Video=" + ci->codec);
    ci->CopyTo(m_VidCodecInfo);

    //Audio
    ci = root->Find(AudioCodec->GetStringSelection().BeforeFirst(' '));
    if (ci == NULL) ci = root->Find(DEFAULT_AUDIO_CODEC_INFO);
    //ShowInfo("Audio=" + ci->codec);
    ci->CopyTo(m_AudCodecInfo);

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::UpdateControls(bool sizers)
{

    UpdateVideoPages(sizers);
    UpdateSliderLabels(-1);

    bool venc = VideoCodec->GetSelection() > 0,
         aenc = AudioCodec->GetSelection() > 0,
         senc = SubsCodec->GetSelection() > 0, b;

    if (sizers)
    {

        //Only performed when codec-choices change, selection of "0" means
        //copy and all possible encoding properties must be disabled

        //m_FullSpecAudIdx = aenc ? FFQFullSpec::FindFullSpec(AudioCodec->GetString(AudioCodec->GetSelection())) : -1;
        //m_FullSpecVidIdx = venc ? FFQFullSpec::FindFullSpec(VideoCodec->GetString(VideoCodec->GetSelection())) : -1;
        m_FullSpecAudIdx = aenc ? FFQFullSpec::FindFullSpec(AudioCodec->GetStringSelection()) : -1;
        m_FullSpecVidIdx = venc ? FFQFullSpec::FindFullSpec(VideoCodec->GetStringSelection()) : -1;

        FullSpecVidButton->Enable(m_FullSpecVidIdx >= 0);
        FullSpecAudButton->Enable(m_FullSpecAudIdx >= 0);

        //Video
        EnableSizer(VideoTwoPassSizer, venc); //Two pass

        //Audio
        EnableSizer(ASBS2, aenc);
        EnableSizer(ASBS3, aenc);
        EnableSizer(ASBS4, aenc);
        SkipEncodeSameAud->Enable(aenc);

        //Filters
        EnableSizer(FilterSizer, venc||aenc, &ID_FILTERTIP);

        //Subtitles
        b = (SubsCodec->GetSelection() == 1) && venc;
        EnableSizer(SSBS2, b);

        if (b)
        {

            b = SubsBitmap->GetValue();
            EnableSizer(SubsSizer2, b);
            SubsLab1->Enable(b);
            SubsLab4->Enable(!b);
            SubsCharEnc->Enable(!b);

            b = true; //use "original_size" for text based subtitles
            EnableSizer(SubsSizer3, b);
            SubsLab2->Enable(b);

            if (b)
            {

                b = SubsSizeCust->GetValue();
                SubsWidth->Enable(b);
                SubsHeight->Enable(b);
                SubsLab3->Enable(b);

            }

        }

    }

    if (venc || aenc)
    {

        int sel_first = -1, sel_last = -2,
            mov_first = 0, f_count = FilterList->GetCount();

        for (int i = 0; i < f_count; i++) if (FilterList->IsSelected(i))
        {
            if (sel_first == -1) sel_first = i;
            if (i > sel_last) sel_last = i;
        }

        if (FindFilter(ftDEINTERLACE) >= 0) mov_first++; //Prevent filters to be moved above deinterlace

        EditFilterButton->Enable(sel_last == sel_first); //Only one selection allowed
        RemoveFilterButton->Enable(sel_last >= sel_first); //Multiple selections allowed
        FilterUpButton->Enable(sel_first > mov_first);
        FilterDownButton->Enable( ((mov_first==0)||(sel_first>0)) && (sel_last < f_count - 1));
                                //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Prevent deinterlace from being moved down
        FilterPreviewButton->Enable(f_count > 0);

        if (venc)
        {

            //Disable video controls that cannot be used
            TwoPassNull->Enable(TwoPass->IsChecked());
            SkipEncodeSameVid->Enable(!TwoPass->IsChecked());
            CST2->Enable(TwoPass->GetValue());
            CustomArgs2->Enable(TwoPass->GetValue());

        }

    }

    if (aenc)
    {

        //Disable audio controls that cannot be changed
        b = (m_AudCodecInfo.min_qscale != m_AudCodecInfo.max_qscale);
        UseAudioQScale->Enable(b);
        if (!b) UseAudioQScale->SetValue(false);
        EnableSizer(AudioQScaleSizer, UseAudioQScale->GetValue());

    }

    if (senc)
    {

        //EnableSizer(SubsSizer1, SubsCodec->GetSelection() == 1, &ID_SUBSINFO);
        //SubsScaleLab->SetLabel(ToStr(SubsScale->GetValue()) + "%");

    }

    ThumbsPanel->UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::UpdateFilterMenu()
{
	for (unsigned int i = 0; i < FILTER_ORDER_COUNT; i++)
    {
        int f = FILTER_ORDER[i];
        if (f != -1)
        {
            wxMenuItem *mi = AddFilterMenu.FindItem(FILTER_MENU_BASE_ID + f);
            if (mi) mi->Enable(FFQCFG()->AreFiltersAvailable(FILTER_NAMES[f]));
        }
    }
}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::UpdateSliderLabels(int SliderId)
{

    int val, qmin, qmax;

    //Update labels attached to sliders and ensure that depending sliders are adjusted as needed

    if ((SliderId == -1) || (SliderId == ID_SUBSSCALE))
        SubsScaleLab->SetLabel(ToStr(SubsScale->GetValue()) + "%");

    if ((SliderId == -1) || (SliderId == ID_SCENECHANGESENS))
    {
        val = SceneChangeSens->GetValue();
        ScSensInfo->SetLabel(val == 0 ? FFQS(SID_DISABLED) : FFQSF(SID_PRESET_SCENE_CHANGE_SENS_INFO, 100-val));
    }

    if ((SliderId == -1) || (SliderId == ID_AUDIOQSCALE))
        AudioQScaleVal->SetLabel(m_AudCodecInfo.GetQScaleStr(AudioQScale->GetValue()));

    if ((SliderId == -1) || (SliderId == ID_VIDEOQSCALE))
        VideoQScaleVal->SetLabel(m_VidCodecInfo.GetQScaleStr(VideoQScale->GetValue()));

    //Adjust slider dependencies

    if (SliderId == ID_CONSTRATE)
    {
        val = ConstRate->GetValue();
        if (MinConstRate->GetValue() > val) MinConstRate->SetValue(val);
    }
    else if (SliderId == ID_MINCONSTRATE)
    {
        val = MinConstRate->GetValue();
        if (ConstRate->GetValue() < val) ConstRate->SetValue(val);
    }
    else if (SliderId == ID_QMIN)
    {
        val = QMin->GetValue();
        if (QMax->GetValue() > val) QMax->SetValue(val);
    }
    else if (SliderId == ID_QMAX)
    {
        val = QMax->GetValue();
        if (QMin->GetValue() < val) QMin->SetValue(val);
    }

    //Adjust slider labels
    ConstRateVal->SetLabel(m_VidCodecInfo.GetConstRateStr(ConstRate->GetValue()));
    MinConstRateVal->SetLabel(m_VidCodecInfo.GetConstRateStr(MinConstRate->GetValue()));

    qmin = ConvertPctToMinMaxInt(QMin->GetValue(), QUANTIZER_WORST_QUALITY, QUANTIZER_BEST_QUALITY);
    QMinVal->SetLabel(ToStr(qmin));

    qmax = ConvertPctToMinMaxInt(QMax->GetValue(), QUANTIZER_WORST_QUALITY, QUANTIZER_BEST_QUALITY);
    QMaxVal->SetLabel(ToStr(qmax));

    QDiffVal->SetLabel(ToStr(ConvertPctToMinMaxInt(QDiff->GetValue(), 0, qmax - qmin)));

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::UpdateSubtitleFilter()
{

    int fidx = FindFilter(ftSUBSBURNIN);
    wxString *fs;

    if ((SubsCodec->GetSelection() == 1) && (fidx < 0))
    {

        //Add subtitle filter
        FFMPEG_FILTER fltr = FFMPEG_FILTER("");
        fltr.type = ftSUBSBURNIN;
        fs = new wxString(fltr.ToString());
        FilterList->Append(FFQS(SID_SUBSBURNIN_USERFRIENDLY), (void*)fs);

    }
    else if ((SubsCodec->GetSelection() != 1) && (fidx >= 0))
    {

        //Remove subtitle filter
        fs = (wxString*)FilterList->GetClientData(fidx);
        delete fs;
        FilterList->Delete(fidx);

    }
}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::UpdateVideoPages(bool sizers)
{

    //Update all controls in the VideoPages notebook
    bool vCopy = (VideoCodec->GetSelection() == 0);

    if (sizers)
    {

        //Enable sizers as needed
        EnableSizer(VideoBitRateSizer, !vCopy);
        EnableSizer(VideoQualitySizer, !vCopy);
        EnableSizer(KeyFrameSizer, !vCopy);
        EnableSizer(VideoOtherSizer, !vCopy);

    }

    VideoPages->Enable(!vCopy);

    //If video is copied, end here
    if (vCopy) return;

    //Quality sliders
    bool b = m_VidCodecInfo.min_qscale != m_VidCodecInfo.max_qscale;
    UseVideoQScale->Enable(b);
    if (!b) UseVideoQScale->SetValue(false);
    VideoQScale->Enable(UseVideoQScale->GetValue());
    VideoQScaleVal->Enable(UseVideoQScale->GetValue());

    b = (!TwoPass->IsChecked()) && (m_VidCodecInfo.min_crf != m_VidCodecInfo.max_crf);
    UseConstRate->Enable(b);
    if (!b) UseConstRate->SetValue(false);
    ConstRate->Enable(UseConstRate->GetValue());
    ConstRateVal->Enable(UseConstRate->GetValue());

    UseMinConstRate->Enable(b);
    if (!b) UseMinConstRate->SetValue(false);
    MinConstRate->Enable(UseMinConstRate->GetValue());
    MinConstRateVal->Enable(UseMinConstRate->GetValue());

    QMin->Enable(UseQMin->GetValue());
    QMinVal->Enable(UseQMin->GetValue());

    QMax->Enable(UseQMax->GetValue());
    QMaxVal->Enable(UseQMax->GetValue());

    QDiff->Enable(UseQDiff->GetValue());
    QDiffVal->Enable(UseQDiff->GetValue());

    EnableSizer(SceneChangeSizer, UseSceneChangeSens->GetValue());

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::OnMenuSelected(wxCommandEvent &event)
{
    int mid = event.GetId(), idx = 0;

    if (mid >= FILTER_MENU_BASE_ID)
    {

        //Filter menu selected
        FFMPEG_FILTER fltr;
        fltr.type = (FILTER_TYPE)(mid - FILTER_MENU_BASE_ID);
        idx = FindFilter(fltr.type);
        wxString *fs;

        if ((fltr.type == ftDEINTERLACE) && (idx != -1)) ShowInfo(FFQS(SID_PRESET_ONLY_ONE_DEINTERLACE));

        else if (!fltr.IsEditable())
        {

            wxString a, b;

            if (fltr.type == ftEARWAX)
            {
                a = FILTER_AUDIO_IN;
                b = FILTER_AUDIO_OUT;
            }
            else
            {
                a = FILTER_VIDEO_IN;
                b = FILTER_VIDEO_OUT;
            }

            fltr.ff_filter = a + FILTER_NAMES[fltr.type] + b;
            fltr.friendly = FFQS(SID_FILTER_NAME_BASE + fltr.type);

            fs = new wxString(fltr.ToString());
            idx = FilterList->Append(fltr.friendly, (void*)fs);

        }

        else if (EditFilter(&fltr))
        {

            fs = new wxString(fltr.ToString());
            if (fltr.type == ftDEINTERLACE) idx = FilterList->Insert(fltr.friendly, 0, (void*)fs);
            else idx = FilterList->Append(fltr.friendly, (void*)fs);

        }

        FilterList->Deselect(-1);
        FilterList->SetSelection(idx);
        FilterList->SetFocus();

    }
    else
    {
        //Dispositions menu selected
        while (mid >= DISPOSITION_MENU_ID_COUNT)
        {
            mid -= DISPOSITION_MENU_ID_COUNT;
            idx++;
        }
        m_DispositionSelection[idx] = mid;
    }
}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::OnClose(wxCloseEvent& event)
{

    EndModal(wxID_CANCEL);

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::OnButtonClick(wxCommandEvent& event)
{

    if (!IsVisible()) return;

    FFMPEG_FILTER fltr;
    wxString *fs;
    int idx, evtId = event.GetId();

    if ((evtId == ID_FULLSPECVIDBUTTON) && (m_FullSpecVidIdx >= 0))
    {

        EditFullSpec(VideoCodec->GetStringSelection(), m_FullSpecVidIdx, m_FullSpecVCodec, m_FullSpecVid);
        /*FFQFullSpec *fs = new FFQFullSpec(this);
        fs->Execute(m_FullSpecVidIdx, m_FullSpecVid);
        delete fs;*/
    }

    else if ((evtId == ID_FULLSPECAUDBUTTON) && (m_FullSpecAudIdx >= 0))
    {

        EditFullSpec(AudioCodec->GetStringSelection(), m_FullSpecAudIdx, m_FullSpecACodec, m_FullSpecAud);
        /*FFQFullSpec *fs = new FFQFullSpec(this);
        fs->Execute(m_FullSpecAudIdx, m_FullSpecAud);
        delete fs;*/
    }

    else if (evtId == ID_ADDFILTERBUTTON)
    {

        AddFilterButton->PopupMenu(&AddFilterMenu);

    }

    else if ((evtId == ID_EDITFILTERBUTTON) || (evtId == ID_FILTERLIST))
    {

        for (unsigned int i = 0; i < FilterList->GetCount(); i++) if (FilterList->IsSelected(i))
        {

            fs = (wxString*)FilterList->GetClientData(i);
            fltr = FFMPEG_FILTER(*fs);

            if (fltr.type == ftSUBSBURNIN) ShowInfo(FFQS(SID_SUBSBURNIN_EDIT_DEL_INFO));
            else if (!fltr.IsEditable()) ShowInfo(FFQS(SID_FILTER_UNEDITABLE));
            else if (EditFilter(&fltr))
            {
                *fs = fltr.ToString();
                FilterList->SetString(i, fltr.friendly);
            }
            FilterList->SetFocus();
            break;

        }

    }

    else if (evtId == ID_REMOVEFILTERBUTTON)
    {

        if (DoConfirm(FFQS(SID_PRESET_DELETE_FILTERS)))
        {

            FilterList->Freeze();

            unsigned int idx = 0;
            while (idx < FilterList->GetCount()) if (FilterList->IsSelected(idx))
            {
                fs = (wxString*)FilterList->GetClientData(idx);
                fltr = FFMPEG_FILTER(*fs);
                if (fltr.type == ftSUBSBURNIN)
                {
                    ShowInfo(FFQS(SID_SUBSBURNIN_EDIT_DEL_INFO));
                    idx++;
                }
                else
                {
                    delete fs;
                    FilterList->Delete(idx);
                }
            }
            else idx++;

            FilterList->Thaw();

        }

        FilterList->SetFocus();

    }

    else if (evtId == ID_FILTERUPBUTTON)
    {

        ListBoxMoveSelectedItems(FilterList, true, false);

    }

    else if (evtId == ID_FILTERDOWNBUTTON)
    {

        ListBoxMoveSelectedItems(FilterList, false, false);

    }

    else if (evtId == ID_FILTERPREVIEWBUTTON)
    {

        PreviewFilters();

    }

    else if (evtId == ID_CLEARMETADATABTN)
    {

        m_MetaData[m_SelMetaData] = "";
        SetMetaData("");

    }

    else if (evtId == ID_DISPOSITIONSBTN)
    {

        DispositionsBtn->PopupMenu(&DispositionsMenu);
        //ShowInfo("Dunno!");

    }

    else if (evtId == ID_OKBUTTON)
    {

        //Confirm that the preset can be saved
        bool can_modify = true;

        if (FFQMain::GetInstance()->IsPresetActive(m_Preset, &can_modify) && (!can_modify))
        {
            ShowError(this, FFQS(SID_PRESET_MODIFY_ACTIVE_ERROR));
            return;
        }

        //Validate parameters
        PresetName->SetValue(StrTrim(PresetName->GetValue()));
        if (PresetName->GetValue().Len() == 0)
        {
            ShowError(PresetName, FFQS(SID_PRESET_NAME_IS_REQUIRED));
            return;
        }

        //Copy or encode?
        bool vCopy = (VideoCodec->GetSelection() == 0), aCopy = (AudioCodec->GetSelection() == 0);
        wxString s;

        //Validate video rates
        if (!VideoBitRate->GetValues(s, !vCopy))
        {
            Pages->SetSelection(Pages->FindPage(VideoPage));
            VideoPages->SetSelection(VideoPages->FindPage(BitRatePage));
            return;
        }

        //Validate audio rates
        if (!AudioBitRate->GetValues(s, !aCopy))
        {
            Pages->SetSelection(Pages->FindPage(AudioPage));
            return;
        }

        //Validate that a quality is selected for video & audio
        bool vq_ok = vCopy || UseConstRate->IsChecked() || UseVideoQScale->IsChecked() || VideoBitRate->HasValue(),
             aq_ok = aCopy || UseAudioQScale->IsChecked() || AudioBitRate->HasValue();
        if ((!vq_ok) || (!aq_ok))
        {
            if (!DoConfirm(Pages, FFQS(SID_PRESET_NO_QUALITY_DEFINED)))
            {
                if (!vq_ok)
                {
                    Pages->SetSelection(Pages->FindPage(VideoPage));
                    VideoPages->SetSelection(VideoPages->FindPage(BitRatePage));
                }
                else
                {
                    Pages->SetSelection(Pages->FindPage(AudioPage));
                    //AudioPages->SetSelection(AudioPages->FindPage(AudBitRatePage));
                }
                return;
            }
        }

        //Validate frame rate
        if ((!vCopy) && (!IsValidFrameRate(StrTrim(FrameRate->GetValue()))))
        {

            Pages->SetSelection(Pages->FindPage(VideoPage));
            VideoPages->SetSelection(VideoPages->FindPage(OtherPage));
            ShowError(FrameRate, FFQS(SID_PRESET_INVALID_FRAME_RATE));
            return;

        }

        //Validate HW Decode
        if (HWD_Device->GetValue().Find(",") >= 0)
        {
            Pages->SetSelection(Pages->FindPage(VideoPage));
            VideoPages->SetSelection(VideoPages->FindPage(HWDecodePage));
            ShowError(HWD_Device, FFQS(SID_PRESET_ERROR_NO_COMMA_ALLOWED));
            return;
        }

        if (vCopy || aCopy)
        {

            //Check encoding vs. filters
            bool vErr = false, aErr = false;
            idx = 0;

            while ((idx < (int)FilterList->GetCount()) && ( ! (vErr||aErr) ))
            {

                fs = (wxString*)FilterList->GetClientData(idx);
                fltr = FFMPEG_FILTER(*fs);
                if (vCopy && (!vErr) && fltr.IsVideo()) vErr = true;
                else if (aCopy && (!aErr) && fltr.IsAudio()) aErr = true;
                idx++;

            }

            if (vErr || aErr)
            {

                if (!DoConfirm(NULL, FFQS(SID_PRESET_INVALID_FILTERS_ADDED), wxICON_WARNING))
                {

                    Pages->SetSelection(Pages->FindPage(FilterPage));
                    return;

                }

            }

        }

        if (!GetMetaData(m_MetaData[MetaDataFor->GetSelection()]))
        {
            Pages->SetSelection(Pages->FindPage(MetaDataPage));
            ShowError(MetaData, FFQS(SID_PRESET_INVALID_METADATA));
            return;
        }

        THUMBS_AND_TILES tnt;
        if (!ThumbsPanel->GetValues(tnt, false))
        {
            Pages->SetSelection(Pages->FindPage(ThumbsPage));
            return;
        }

        //Validate subtitle settings
        if (SubsSizeCust->GetValue())
        {

            int sw = Str2Long(SubsWidth->GetValue(), 0), sh = Str2Long(SubsHeight->GetValue(), 0);

            if ((sw <= 0) || (sh <= 0))
            {

                Pages->SetSelection(Pages->FindPage(SubtitlesPage));
                ShowError(SubsWidth, FFQS(SID_PRESET_INVALID_SUBTITLE_SIZE));
                return;

            }

        }

        EndModal(wxID_OK);
        return; //Don't update controls on success

    }

    else if (evtId == ID_CANCELBUTTON)
    {

        EndModal(wxID_CANCEL);
        return; //Don't update controls on dismiss

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::OnChoiceChange(wxCommandEvent& event)
{

    //Exit if not visible (setting data to controls)
    if (!IsVisible()) return;

    //Get control id
    int id = event.GetId();

    //Update codec info if required
    if ((id == ID_VIDEOCODEC) || (id == ID_AUDIOCODEC)) UpdateCodecInfo();

    else if (id == ID_METADATAFOR)
    {
        if (!GetMetaData(m_MetaData[m_SelMetaData]))
        {
            ShowError(MetaData, FFQS(SID_PRESET_INVALID_METADATA));
            MetaDataFor->SetSelection(m_SelMetaData);
            return;
        }
        m_SelMetaData = MetaDataFor->GetSelection();
        SetMetaData(m_MetaData[m_SelMetaData]);
    }

    //Update controls and sizers
    UpdateControls(true);

    //Update subtitle filter
    if (id == ID_SUBSCODEC) UpdateSubtitleFilter();

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::OnSubsScaleChange(wxScrollEvent& event)
{

    //This event should probably have been named "OnSliderChange"
    UpdateSliderLabels(event.GetId());

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::OnMetaDataCellChange(wxGridEvent& event)
{

    //Make sure that there is always an empty last row
    event.Skip();
    if (event.GetCol() != 0) return;
    int rows = MetaData->GetNumberRows() - 1;
    if ((event.GetRow() == rows) && (StrTrim(MetaData->GetCellValue(rows, 0)) != "")) MetaData->AppendRows(); //EnsureGridRow(MetaData, "", m_SelMetaData > 0);

}

//---------------------------------------------------------------------------------------

void FFQPresetEdit::OnNotebookPageChanged(wxNotebookEvent& event)
{

    //Filter preview dialog (browse for preview file) is always shown when pages
    //change. This makes it easy to change preview file by changing to another
    //page and back to the filter page

    m_ShowPreviewDlg = true;

}
