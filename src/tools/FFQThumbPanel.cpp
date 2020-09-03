/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQThumbPanel.cpp                                               *
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

#include "FFQThumbPanel.h"
#include "../utils/FFQLang.h"
#include "../utils/FFQConfig.h"
#include "../utils/FFQMisc.h"

#include <wx/valnum.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQThumbPanel)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQThumbPanel)
//*)

//(*IdInit(FFQThumbPanel)
const long FFQThumbPanel::ID_MAKETHUMBS = wxNewId();
const long FFQThumbPanel::ID_MAKETILES = wxNewId();
const long FFQThumbPanel::ID_ST1 = wxNewId();
const long FFQThumbPanel::ID_IMGDEST = wxNewId();
const long FFQThumbPanel::ID_ST2 = wxNewId();
const long FFQThumbPanel::ID_IMGFORMAT = wxNewId();
const long FFQThumbPanel::ID_ST3 = wxNewId();
const long FFQThumbPanel::ID_WIDTH = wxNewId();
const long FFQThumbPanel::ID_ST4 = wxNewId();
const long FFQThumbPanel::ID_STARTTIME = wxNewId();
const long FFQThumbPanel::ID_EVERYCUSTOM = wxNewId();
const long FFQThumbPanel::ID_CUSTFREQ = wxNewId();
const long FFQThumbPanel::ID_CUSTFREQTYPE = wxNewId();
const long FFQThumbPanel::ID_EVERYSCENE = wxNewId();
const long FFQThumbPanel::ID_ST5 = wxNewId();
const long FFQThumbPanel::ID_MAXTHUMBS = wxNewId();
const long FFQThumbPanel::ID_ST6 = wxNewId();
const long FFQThumbPanel::ID_MAXTILES = wxNewId();
const long FFQThumbPanel::ID_ST7 = wxNewId();
const long FFQThumbPanel::ID_ROWS = wxNewId();
const long FFQThumbPanel::ID_ST8 = wxNewId();
const long FFQThumbPanel::ID_COLUMNS = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQThumbPanel,wxPanel)
	//(*EventTable(FFQThumbPanel)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQThumbPanel::FFQThumbPanel(wxWindow* parent,wxWindowID id)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(0, 99999);

    wxFloatingPointValidator<float> fVal;
    fVal.SetRange(0, 99999);


	//(*Initialize(FFQThumbPanel)
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* MakeSizer;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	FGS_Main = new wxFlexGridSizer(7, 1, 0, 0);
	FGS_Main->AddGrowableCol(0);
	FGS_Main->AddGrowableRow(0);
	MakeSizer = new wxFlexGridSizer(1, 3, 0, 0);
	MakeThumbs = new wxCheckBox(this, ID_MAKETHUMBS, _T("Tb"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MAKETHUMBS"));
	MakeThumbs->SetValue(false);
	MakeThumbs->SetLabel(FFQS(SID_THUMBS_CREATE_THUMBS));
	MakeSizer->Add(MakeThumbs, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MakeSizer->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	MakeTiles = new wxCheckBox(this, ID_MAKETILES, _T("Tm"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MAKETILES"));
	MakeTiles->SetValue(false);
	MakeTiles->SetLabel(FFQS(SID_THUMBS_CREATE_MOSAICS));
	MakeSizer->Add(MakeTiles, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FGS_Main->Add(MakeSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	DestSizer = new wxFlexGridSizer(2, 2, 0, 0);
	DestSizer->AddGrowableCol(1);
	ST1 = new wxStaticText(this, ID_ST1, _T("Dst"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST1"));
	ST1->SetLabel(FFQS(SID_COMMON_DEST_FOLDER));
	DestSizer->Add(ST1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ImgDest = new wxChoice(this, ID_IMGDEST, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_IMGDEST"));
	ImgDest->SetSelection( ImgDest->Append(FFQS(SID_THUMBS_DEST_NEW_UNIQUE)) );
	ImgDest->Append(FFQS(SID_THUMBS_DEST_SAME));
	ImgDest->Append(FFQS(SID_THUMBS_DEST_THUMBS));
	DestSizer->Add(ImgDest, 1, wxALL|wxEXPAND, 2);
	ST2 = new wxStaticText(this, ID_ST2, _T("ImgF"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST2"));
	ST2->SetLabel(FFQS(SID_THUMBS_IMAGE_FORMAT));
	DestSizer->Add(ST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ImgFormat = new wxComboBox(this, ID_IMGFORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_IMGFORMAT"));
	ImgFormat->SetSelection( ImgFormat->Append(_T("png")) );
	ImgFormat->Append(_T("PNG"));
	ImgFormat->Append(_T("jpg"));
	ImgFormat->Append(_T("JPG"));
	ImgFormat->Append(_T("jpeg"));
	ImgFormat->Append(_T("JPEG"));
	ImgFormat->Append(_T("bmp"));
	ImgFormat->Append(_T("BMP"));
	DestSizer->Add(ImgFormat, 1, wxALL|wxEXPAND, 2);
	FGS_Main->Add(DestSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	WidthSizer = new wxFlexGridSizer(1, 3, 0, 0);
	WidthSizer->AddGrowableCol(2);
	ST3 = new wxStaticText(this, ID_ST3, _T("Tbw"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST3"));
	ST3->SetLabel(FFQS(SID_THUMBS_WIDTH));
	WidthSizer->Add(ST3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Width = new wxTextCtrl(this, ID_WIDTH, wxEmptyString, wxDefaultPosition, wxSize(50,-1), 0, iVal, _T("ID_WIDTH"));
	WidthSizer->Add(Width, 1, wxALL|wxEXPAND, 2);
	ST4 = new wxStaticText(this, ID_ST4, _T("Inf"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST4"));
	ST4->Disable();
	ST4->SetLabel(FFQS(SID_THUMBS_HEIGHT_INFO));
	WidthSizer->Add(ST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FGS_Main->Add(WidthSizer, 1, wxALL|wxEXPAND, 3);
	TimeSizer = new wxFlexGridSizer(1, 1, 0, 0);
	TimeSizer->AddGrowableCol(0);
	TimeSizer->AddGrowableRow(0);
	StartTime = new wxHyperlinkCtrl(this, ID_STARTTIME, _T("Start Time"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE, _T("ID_STARTTIME"));
	TimeSizer->Add(StartTime, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FGS_Main->Add(TimeSizer, 1, wxALL|wxEXPAND, 2);
	FreqSizer = new wxFlexGridSizer(2, 1, 0, 0);
	FreqSizer->AddGrowableCol(0);
	FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer7->AddGrowableCol(2);
	EveryCustom = new wxRadioButton(this, ID_EVERYCUSTOM, _T("Iv"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EVERYCUSTOM"));
	EveryCustom->SetLabel(FFQS(SID_THUMBS_INTERVAL));
	FlexGridSizer7->Add(EveryCustom, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CustFreq = new wxTextCtrl(this, ID_CUSTFREQ, wxEmptyString, wxDefaultPosition, wxSize(60,-1), 0, fVal, _T("ID_CUSTFREQ"));
	FlexGridSizer7->Add(CustFreq, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	CustFreqType = new wxChoice(this, ID_CUSTFREQTYPE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CUSTFREQTYPE"));
	CustFreqType->SetSelection( CustFreqType->Append(FFQS(SID_THUMBS_INTERVAL_FRAME)) );
	CustFreqType->Append(FFQS(SID_THUMBS_INTERVAL_SECOND));
	CustFreqType->Append(FFQS(SID_THUMBS_INTERVAL_MINUTE));
	FlexGridSizer7->Add(CustFreqType, 1, wxALL|wxEXPAND, 2);
	FreqSizer->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer8 = new wxFlexGridSizer(0, 3, 0, 0);
	EveryScene = new wxRadioButton(this, ID_EVERYSCENE, _T("Sc"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EVERYSCENE"));
	EveryScene->SetValue(true);
	EveryScene->SetLabel(FFQS(SID_THUMBS_SCENE_CHANGE));
	FlexGridSizer8->Add(EveryScene, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FreqSizer->Add(FlexGridSizer8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FGS_Main->Add(FreqSizer, 0, wxALIGN_LEFT|wxALIGN_TOP, 0);
	AmountSizer = new wxFlexGridSizer(1, 4, 0, 0);
	ST5 = new wxStaticText(this, ID_ST5, _T("Mtb"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST5"));
	ST5->SetLabel(FFQS(SID_THUMBS_MAX_THUMBS));
	AmountSizer->Add(ST5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MaxThumbs = new wxTextCtrl(this, ID_MAXTHUMBS, wxEmptyString, wxDefaultPosition, wxSize(50,-1), 0, iVal, _T("ID_MAXTHUMBS"));
	AmountSizer->Add(MaxThumbs, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ST6 = new wxStaticText(this, ID_ST6, _T("Mtm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST6"));
	ST6->SetLabel(FFQS(SID_THUMBS_MAX_MOSAICS));
	AmountSizer->Add(ST6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MaxTiles = new wxTextCtrl(this, ID_MAXTILES, wxEmptyString, wxDefaultPosition, wxSize(50,-1), 0, iVal, _T("ID_MAXTILES"));
	AmountSizer->Add(MaxTiles, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FGS_Main->Add(AmountSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	MosaicSizer = new wxFlexGridSizer(2, 4, 0, 0);
	ST7 = new wxStaticText(this, ID_ST7, _T("Mr"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST7"));
	ST7->SetLabel(FFQS(SID_THUMBS_MOSAIC_ROWS));
	MosaicSizer->Add(ST7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Rows = new wxTextCtrl(this, ID_ROWS, wxEmptyString, wxDefaultPosition, wxSize(50,-1), 0, iVal, _T("ID_ROWS"));
	MosaicSizer->Add(Rows, 1, wxALL|wxEXPAND, 2);
	ST8 = new wxStaticText(this, ID_ST8, _T("Mc"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST8"));
	ST8->SetLabel(FFQS(SID_THUMBS_MOSAIC_COLS));
	MosaicSizer->Add(ST8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Columns = new wxTextCtrl(this, ID_COLUMNS, wxEmptyString, wxDefaultPosition, wxSize(50,-1), 0, iVal, _T("ID_COLUMNS"));
	MosaicSizer->Add(Columns, 1, wxALL|wxEXPAND, 2);
	FGS_Main->Add(MosaicSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	SetSizer(FGS_Main);
	FGS_Main->Fit(this);
	FGS_Main->SetSizeHints(this);

	Connect(ID_MAKETHUMBS,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQThumbPanel::OnAction);
	Connect(ID_MAKETILES,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQThumbPanel::OnAction);
	Connect(ID_STARTTIME,wxEVT_COMMAND_HYPERLINK,(wxObjectEventFunction)&FFQThumbPanel::OnAction);
	Connect(ID_EVERYCUSTOM,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQThumbPanel::OnAction);
	Connect(ID_EVERYSCENE,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&FFQThumbPanel::OnAction);
	//*)

	m_TimeEdit = NULL;

}

//---------------------------------------------------------------------------------------

FFQThumbPanel::~FFQThumbPanel()
{

	//(*Destroy(FFQThumbPanel)
	//*)

	if (m_TimeEdit)
    {
        delete m_TimeEdit;
        m_TimeEdit = NULL;
    }

}

//---------------------------------------------------------------------------------------

void FFQThumbPanel::ClearValues(bool load_config)
{

    THUMBS_AND_TILES tnt;
    if (load_config) tnt = THUMBS_AND_TILES(FFQCFG()->thumb_settings);
    SetValues(tnt);

}

//---------------------------------------------------------------------------------------

bool FFQThumbPanel::GetValues(THUMBS_AND_TILES &tnt, bool ThumbOrTileRequired)
{

    tnt.make_thumbs = MakeThumbs->GetValue();
    tnt.make_tiles = MakeTiles->GetValue();
    bool make_any = tnt.make_thumbs || tnt.make_tiles;

    if (ThumbOrTileRequired && (!make_any)) return ShowError(MakeThumbs, FFQS(SID_THUMBS_OR_MOSAICS_REQUIRED));

    tnt.img_dest = ImgDest->GetSelection();
    tnt.img_format = StrTrim(ImgFormat->GetValue());
    if (make_any && (tnt.img_format.Len() == 0)) return ShowError(ImgFormat, FFQS(SID_IMAGE_FORMAT_REQUIRED));

    tnt.thumb_width = Str2Long(Width->GetValue());
    if (make_any && (tnt.thumb_width <= 0)) return ShowError(Width, FFQSF(SID_MUST_BE_A_POSITIVE_NUMBER, FFQS(SID_THUMB_WIDTH)));

    tnt.start_time = m_StartTime;

    tnt.thumb_freq = Str2Float(CustFreq->GetValue());
    tnt.freq_type = CustFreqType->GetSelection();
    if (make_any && (tnt.thumb_freq <= 0)) return ShowError(CustFreq, FFQSF(SID_MUST_BE_A_POSITIVE_NUMBER, FFQS(SID_THUMB_INTERVAL)));

    tnt.every_scene = EveryScene->GetValue();

    tnt.thumb_count = Str2Long(MaxThumbs->GetValue());

    tnt.tile_rows = Str2Long(Rows->GetValue());
    tnt.tile_cols = Str2Long(Columns->GetValue());

    if (make_any && (tnt.tile_rows * tnt.tile_cols < 2)) return ShowError(Rows, FFQS(SID_ROW_AND_COLUMN_TOO_SMALL));

    tnt.tile_count = Str2Long(MaxTiles->GetValue());

    if (make_any)
    {

        wxString ts = tnt.ToString();

        if (ts != FFQCFG()->thumb_settings)
        {
            FFQCFG()->thumb_settings = ts;
            FFQCFG()->SaveConfig();
        }

    }

    return true;

}

//---------------------------------------------------------------------------------------

#define SetNumVal(tc, val) tc->SetValue(val == 0 ? "" : ToStr(val));

//---------------------------------------------------------------------------------------

void FFQThumbPanel::SetValues(THUMBS_AND_TILES &tnt)
{

    MakeThumbs->SetValue(tnt.make_thumbs);
    MakeTiles->SetValue(tnt.make_tiles);

    ImgDest->SetSelection(tnt.img_dest);
    ImgFormat->SetValue(tnt.img_format);

    Width->SetValue(ToStr(tnt.thumb_width));

    m_StartTime = tnt.start_time;

    CustFreq->SetValue(ToStr(tnt.thumb_freq));
    CustFreqType->SetSelection(tnt.freq_type);

    EveryScene->SetValue(tnt.every_scene);

    MaxThumbs->SetValue(ToStr(tnt.thumb_count));

    Rows->SetValue(ToStr(tnt.tile_rows));
    Columns->SetValue(ToStr(tnt.tile_cols));

    MaxTiles->SetValue(ToStr(tnt.tile_count));

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQThumbPanel::UpdateControls()
{

    StartTime->SetLabelText(FFQSF(SID_CREATE_FIRST_THUMB_FROM,  (m_StartTime.IsUndefined() ? FFQS(SID_THE_BEGINNING) : m_StartTime.ToString())));
    //TimeSizer->RecalcSizes();
    TimeSizer->Layout();

    bool makeAny = MakeThumbs->GetValue() || MakeTiles->GetValue();

    EnableSizer(DestSizer, makeAny);
    EnableSizer(WidthSizer, makeAny, &ID_ST4);
    EnableSizer(TimeSizer, makeAny);
    EnableSizer(FreqSizer, makeAny);
    EnableSizer(AmountSizer, makeAny);
    EnableSizer(MosaicSizer, MakeTiles->GetValue());

    if (makeAny)
    {

        CustFreq->Enable(EveryCustom->GetValue());
        CustFreqType->Enable(EveryCustom->GetValue());

        ST5->Enable(MakeThumbs->GetValue());
        MaxThumbs->Enable(MakeThumbs->GetValue());

        ST6->Enable(MakeTiles->GetValue());
        MaxTiles->Enable(MakeTiles->GetValue());

    }

}

//---------------------------------------------------------------------------------------

bool FFQThumbPanel::EditTime(TIME_VALUE &tv)
{
    if (m_TimeEdit == NULL) m_TimeEdit = new FFQTimeEdit(this);
    return m_TimeEdit->Execute(tv);
}

//---------------------------------------------------------------------------------------

void FFQThumbPanel::OnAction(wxCommandEvent& event)
{

    int evtId = event.GetId();

    if (evtId == ID_STARTTIME) EditTime(m_StartTime);

    UpdateControls();

}
