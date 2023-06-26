/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FilterBasePanel.cpp                                             *
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

#include "FilterBasePanel.h"

#include "ColorOverlay.h"
#include "Deinterlace.h"
#include "Crop.h"
#include "Scale.h"
#include "Delogo.h"
#include "Watermark.h"
#include "Flip.h"
#include "Dejudder.h"
#include "Deshake.h"
#include "BoxBlur.h"
#include "ColorBalance.h"
#include "Pad.h"
#include "Rotate.h"
#include "Unsharp.h"
#include "Vignette.h"
#include "VolumeAndTone.h"
#include "Echo.h"
#include "Phaser.h"
#include "Flanger.h"
#include "HighLowBandPass.h"
#include "Equalizer.h"
#include "ColorMatrix.h"
#include "Curves.h"
#include "ChannelMap.h"
#include "Custom.h"
#include "Resample.h"
#include "Negate.h"
#include "Elbg.h"
#include "AudioDelay.h"
#include "AudioTempo.h"
#include "Decimate.h"
#include "Interlace.h"
#include "FrameStep.h"
#include "Hqdn3d.h"
#include "Hue.h"
#include "LensCorrection.h"
#include "Noise.h"
#include "Perspective.h"
#include "Transpose.h"
#include "DynAudNorm.h"
#include "Deflicker.h"

FilterBasePanel::FilterBasePanel(wxWindow* parent) : wxPanel(parent)
{
    m_FilterUID = 0;
    m_Padding = 3;
    m_TimeEditor = NULL;
    m_TimeSizer = NULL;
    m_PosSizer = NULL;
    m_DimSizer = NULL;
    m_FileDlg = NULL;
    m_CtrlParent = this;
}

//---------------------------------------------------------------------------------------

FilterBasePanel::~FilterBasePanel()
{
    if (m_TimeEditor != NULL)
    {
        delete m_TimeEditor;
        m_TimeEditor = NULL;
    }
    if (m_FileDlg != NULL)
    {
        delete m_FileDlg;
        m_FileDlg = NULL;
    }
}

//---------------------------------------------------------------------------------------

FilterBasePanel* FilterBasePanel::GetFilterPanel(wxWindow* parent, FILTER_TYPE ft)
{

    switch (ft)
    {
        case (ftCOLOROVERLAY) : return new ColorOverlay(parent); break;
        case (ftDEINTERLACE) : return new Deinterlace(parent); break;
        case (ftCROP) : return new Crop(parent); break;
        case (ftSCALE) : return new Scale(parent); break;
        //case (ftSUBSBURNIN) : return new SubsBurnIn(parent); break;
        case (ftDELOGO) : return new Delogo(parent); break;
        case (ftWATERMARK) : return new Watermark(parent); break;
        case (ftFLIP) : return new Flip(parent); break;
        case (ftDEJUDDER) : return new Dejudder(parent); break;
        case (ftDESHAKE) : return new Deshake(parent); break;
        case (ftBOXBLUR) : return new BoxBlur(parent); break;
        case (ftCOLORBALANCE) : return new ColorBalance(parent); break;
        case (ftPAD) : return new Pad(parent); break;
        case (ftROTATE) : return new Rotate(parent); break;
        case (ftUNSHARP) : return new Unsharp(parent); break;
        case (ftVIGNETTE) : return new Vignette(parent); break;
        case (ftVOLUME_AND_TONE) : return new VolumeAndTone(parent); break;
        case (ftECHO) : return new Echo(parent); break;
        case (ftPHASER) : return new Phaser(parent); break;
        case (ftFLANGER) : return new Flanger(parent); break;
        case (ftHIGHPASS) : return new HighLowBandPass(parent); break;
        case (ftLOWPASS) : return new HighLowBandPass(parent); break;
        case (ftBANDPASS) : return new HighLowBandPass(parent); break;
        case (ftBANDREJECT) : return new HighLowBandPass(parent); break;
        case (ftEQUALIZER) : return new Equalizer(parent); break;
        case (ftCOLORMATRIX) : return new ColorMatrix(parent); break;
        case (ftCURVES) : return new Curves(parent); break;
        case (ftCHANNELMAP) : return new ChannelMap(parent); break;
        case (ftCUSTOM) : return new Custom(parent); break;
        case (ftRESAMPLE) : return new Resample(parent); break;
        case (ftNEGATE) : return new Negate(parent); break;
        case (ftELBG) : return new Elbg(parent); break;
        case (ftADELAY) : return new AudioDelay(parent); break;
        case (ftATEMPO) : return new AudioTempo(parent); break;
        case (ftDECIMATE) : return new Decimate(parent); break;
        case (ftINTERLACE) : return new Interlace(parent); break;
        case (ftFRAMESTEP) : return new FrameStep(parent); break;
        case (ftHQDN3D) : return new Hqdn3d(parent); break;
        case (ftHUE) : return new Hue(parent); break;
        case (ftLENSCORRECTION) : return new LensCorrection(parent); break;
        case (ftNOISE) : return new Noise(parent); break;
        case (ftPERSPECTIVE) : return new Perspective(parent); break;
        case (ftTRANSPOSE) : return new Transpose(parent); break;
        case (ftDYNAUDNORM) : return new DynAudNorm(parent); break;
        case (ftDEFLICKER) : return new Deflicker(parent); break;
        default: return NULL;
    }

}

//---------------------------------------------------------------------------------------

wxString FilterBasePanel::NextFilterUID()
{

    m_FilterUID++;
    return wxString::Format(FILTER_UNIQUE_ID, m_FilterUID);

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetControlPadding(unsigned int pad)
{
    m_Padding = pad;
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetControlParent(wxWindow *parent)
{
    m_CtrlParent = (parent == NULL) ? this : parent;
}

//---------------------------------------------------------------------------------------

bool FilterBasePanel::BrowseForFile(wxString &file)
{
    if (m_FileDlg == NULL) m_FileDlg = new wxFileDialog(this, FFQS(SID_COMMON_SELECT_FILE), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    m_FileDlg->SetPath(file);
    bool res = m_FileDlg->ShowModal() != wxID_CANCEL;
    if (res) file = m_FileDlg->GetPath();
    return res;
}

//---------------------------------------------------------------------------------------

wxStaticText* FilterBasePanel::MakeLabel(wxString text, wxSizer *addToSizer, int addSizerFlags)
{

    wxStaticText *st = new wxStaticText(m_CtrlParent, wxID_ANY, text);

    if (addToSizer != NULL)
    {
        if (addSizerFlags == 0) addSizerFlags = wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL;
        addToSizer->Add(st, 1, addSizerFlags, m_Padding);
    }

    return st;

}

//---------------------------------------------------------------------------------------

wxFlexGridSizer* FilterBasePanel::MakeSliderLabels(wxSizer *addToSizer, wxString l1, wxString l2, wxString l3, bool vert)
{

    wxFlexGridSizer *res = new wxFlexGridSizer(vert ? 3 : 1, vert ? 1 : 3, 0, 0);

    if (vert) res->AddGrowableRow(1);
    else res->AddGrowableCol(1);

    MakeLabel(l1, res, vert ? wxALL|wxALIGN_TOP|wxALIGN_RIGHT : wxALL|wxALIGN_BOTTOM|wxALIGN_LEFT);
    MakeLabel(l2, res, vert ? wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT : wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL);
    MakeLabel(l3, res, vert ? wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT : wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT);

    if (addToSizer != NULL) addToSizer->Add(res, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, m_Padding);

    return res;

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::EnableLeftAndTop(bool enable)
{
    if (m_PosSizer) EnableSizer(m_PosSizer, enable);
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::EnableWidthAndHeight(bool enable)
{
    if (m_DimSizer) EnableSizer(m_DimSizer, enable);
}

//---------------------------------------------------------------------------------------

bool FilterBasePanel::EditTimeValue(TIME_VALUE &tv, bool allowMillis)
{

    if (m_TimeEditor == NULL) m_TimeEditor = new FFQTimeEdit(this);

    return m_TimeEditor->Execute(tv, allowMillis);

}

//---------------------------------------------------------------------------------------

wxSizer* FilterBasePanel::GetTimeLimitControls(bool AddLabelThis)
{

     if (m_TimeSizer) return m_TimeSizer;

     m_TimeSizer = new wxFlexGridSizer(1, AddLabelThis ? 5 : 4, 0, 0); //Rows, Cols, HGap, VGap
     wxStaticText *st;

     if (AddLabelThis)
     {

        st = new wxStaticText(m_CtrlParent, wxID_ANY, FFQS(SID_FILTER_THIS) + SPACE);
        m_TimeSizer->Add(st, 1, wxTOP|/*wxBOTTOM|*/wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);

     }

     st = new wxStaticText(m_CtrlParent, wxID_ANY, FFQS(SID_FILTER_TIME_LIMIT_FROM) + SPACE);
     m_TimeSizer->Add(st, 1, wxTOP|/*wxBOTTOM|*/wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);
     m_ShowTime1 = new wxGenericHyperlinkCtrl(m_CtrlParent, 1000, _(SPACE), wxEmptyString);//, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
     FFQCFG()->SetCtrlColors(m_ShowTime1);
     m_TimeSizer->Add(m_ShowTime1, 1, wxTOP|/*wxBOTTOM|*/wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);

     st = new wxStaticText(m_CtrlParent, wxID_ANY, " " + FFQS(SID_FILTER_TIME_LIMIT_TO) + SPACE);
     m_TimeSizer->Add(st, 1, wxTOP|/*wxBOTTOM|*/wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);
     m_ShowTime2 = new wxGenericHyperlinkCtrl(m_CtrlParent, 1001, _(SPACE), wxEmptyString);//, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
     FFQCFG()->SetCtrlColors(m_ShowTime2);
     m_TimeSizer->Add(m_ShowTime2, 1, wxTOP|/*wxBOTTOM|*/wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);

     Connect(1000, wxEVT_HYPERLINK, (wxObjectEventFunction)&FilterBasePanel::OnTimeLinkClick);
     Connect(1001, wxEVT_HYPERLINK, (wxObjectEventFunction)&FilterBasePanel::OnTimeLinkClick);

     return m_TimeSizer;

}

//---------------------------------------------------------------------------------------

wxSizer* FilterBasePanel::GetTimeLimitControls(wxSizer *AppendToSizer, int AddSizerFlags, bool SetAsSizer)
{

    if (m_TimeSizer) return m_TimeSizer;

    if (AddSizerFlags == 0) AddSizerFlags = wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL;

    GetTimeLimitControls(true);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(2, 1, 0, 0);
    fgs->AddGrowableCol(0);
    fgs->AddGrowableRow(0);
    fgs->Add(AppendToSizer, 1, AddSizerFlags, m_Padding);
    fgs->Add(m_TimeSizer, 1, AddSizerFlags, 3);

    if (SetAsSizer)
    {
        m_CtrlParent->SetSizer(fgs);
        fgs->Fit(m_CtrlParent);
        fgs->SetSizeHints(m_CtrlParent);
    }

    return m_TimeSizer;

}

//---------------------------------------------------------------------------------------

bool FilterBasePanel::GetTimeLimitFilter(wxString &str, bool colonFirst)
{

    uint64_t s, e;
    str = "";
    if (!GetTimeLimitValues(s, e, true)) return false;
    if (s + e == 0) return true;
    if (e == 0) e = 0xffffffff;
    str.Printf("enable='between(t," + UINT64FMT + COMMA + UINT64FMT + ")'", s, e);
    if (colonFirst) str = COLON + str;
    else str += COLON;
    return true;

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::GetTimeLimitFriendly(wxString &from, wxString &to)
{
    from = m_Time1.IsUndefined() ? FFQS(SID_FILTER_TIME_LIMIT_START) : m_Time1.ToString(true, false);
    to = m_Time2.IsUndefined() ? FFQS(SID_FILTER_TIME_LIMIT_END) : m_Time2.ToString(true, false);
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::GetTimeLimitFriendly(wxString &fromAndTo)
{
    wxString f, t;
    GetTimeLimitFriendly(f, t);
    fromAndTo = FFQSF(SID_FILTER_TIME_LIMIT_FRIENDLY, f, t);
}

//---------------------------------------------------------------------------------------

bool FilterBasePanel::GetTimeLimitValues(TIME_VALUE &from, TIME_VALUE &to)
{

    if ((!m_Time1.IsUndefined()) && (!m_Time2.IsUndefined()) && (m_Time1.ToMilliseconds() >= m_Time2.ToMilliseconds()))
        return ShowError(FFQS(SID_FILTER_TIME_LIMIT_ERROR));

    from = m_Time1;
    to = m_Time2;

    return true;

}

//---------------------------------------------------------------------------------------

bool FilterBasePanel::GetTimeLimitValues(uint64_t &from, uint64_t &to, bool asSeconds)
{
    TIME_VALUE t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;
    from = t1.ToMilliseconds();
    if (asSeconds) from /= 1000;
    to = t2.ToMilliseconds();
    if (asSeconds) to /= 1000;
    return true;
}


//---------------------------------------------------------------------------------------

void FilterBasePanel::SetTimeLimitValues(TIME_VALUE &from, TIME_VALUE &to)
{

     m_Time1 = from;
     m_Time2 = to;

     UpdateTimeLimitLinks();

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetTimeLimitValues(uint64_t &from, uint64_t &to, bool isSeconds)
{

    TIME_VALUE t1 = TIME_VALUE(isSeconds ? from * 1000 : from),
               t2 = TIME_VALUE(isSeconds ? to * 1000 : to);

    SetTimeLimitValues(t1, t2);

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetTimeLimitValues(wxString &str, bool isSeconds)
{

    uint64_t t1 = Str2LongLong(GetToken(str, ','), 0), t2 = Str2LongLong(GetToken(str, ','), 0);
    SetTimeLimitValues(t1, t2, isSeconds);

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::OnTimeLinkClick(wxHyperlinkEvent &event)
{

    if ((event.GetId() == 1000) && EditTimeValue(m_Time1)) UpdateTimeLimitLinks();
    else if ((event.GetId() == 1001) && EditTimeValue(m_Time2)) UpdateTimeLimitLinks();

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::UpdateTimeLimitLinks()
{

    wxString t1, t2;
    GetTimeLimitFriendly(t1, t2);
    m_ShowTime1->SetLabelText(t1);
    m_ShowTime1->Refresh(); //2023
    m_ShowTime2->SetLabelText(t2);
    m_ShowTime2->Refresh(); //2023
    m_TimeSizer->Layout();
    //Layout();

}

//---------------------------------------------------------------------------------------

wxSizer* FilterBasePanel::GetLeftAndTopControls(wxValidator &val)
{

    if (m_PosSizer == NULL)
    {

        m_PosSizer = new wxFlexGridSizer(1, 4, 0, 0);
        m_PosSizer->AddGrowableCol(0);
        m_PosSizer->AddGrowableCol(2);

        MakeLabel(FFQS(SID_FILTER_POSITION_LEFT), m_PosSizer, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);

        m_Left = new wxTextCtrl(m_CtrlParent, wxID_ANY);
        m_Left->SetValidator(val);
        m_PosSizer->Add(m_Left, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);

        MakeLabel(FFQS(SID_FILTER_POSITION_TOP), m_PosSizer, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);

        m_Top = new wxTextCtrl(m_CtrlParent, wxID_ANY);
        m_Top->SetValidator(val);
        m_PosSizer->Add(m_Top, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);

    }

    return m_PosSizer;

}

//---------------------------------------------------------------------------------------

void FilterBasePanel::GetLeftAndTopValues(int &left, int &top)
{
    //wxIntegerValidator<unsigned int> *val = dynamic_cast<wxIntegerValidator<unsigned int>*>(m_Left->GetValidator());
    //left = Str2Long(m_Left->GetValue(), val ? val->GetMin() : 0);
    //top = Str2Long(m_Top->GetValue(), val ? val->GetMin() : 0);
    left = Str2Long(m_Left->GetValue(), 0);
    top = Str2Long(m_Top->GetValue(), 0);
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetLeftAndTopValues(int left, int top)
{
    m_Left->SetValue(ToStr(left));
    m_Top->SetValue(ToStr(top));
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetLeftAndTopValues(wxString &from)
{
    m_Left->SetValue(GetToken(from, COMMA));
    m_Top->SetValue(GetToken(from, COMMA));
}

//---------------------------------------------------------------------------------------

wxSizer* FilterBasePanel::GetWidthAndHeightControls(wxValidator &val)
{

    if (m_DimSizer == NULL)
    {

        m_DimSizer = new wxFlexGridSizer(1, 4, 0, 0);
        m_DimSizer->AddGrowableCol(0);
        m_DimSizer->AddGrowableCol(2);

        MakeLabel(FFQS(SID_FILTER_POSITION_WIDTH), m_DimSizer, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);

        m_Width = new wxTextCtrl(m_CtrlParent, wxID_ANY);
        m_Width->SetValidator(val);
        m_DimSizer->Add(m_Width, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);

        MakeLabel(FFQS(SID_FILTER_POSITION_HEIGHT), m_DimSizer, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);

        m_Height = new wxTextCtrl(m_CtrlParent, wxID_ANY);
        m_Height->SetValidator(val);
        m_DimSizer->Add(m_Height, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, m_Padding);

    }

    return m_DimSizer;

}

//---------------------------------------------------------------------------------------

bool FilterBasePanel::GetWidthAndHeightValues(int &width, int &height, bool required)
{
    width = Str2Long(m_Width->GetValue(), 0);
    height = Str2Long(m_Height->GetValue(), 0);
    if (required && (width <= 0)) return ShowError(m_Width, FFQS(SID_FILTER_WIDTH_HEIGHT_REQUIRED));
    if (required && (height <= 0)) return ShowError(m_Height, FFQS(SID_FILTER_WIDTH_HEIGHT_REQUIRED));
    return true;
}

//---------------------------------------------------------------------------------------

bool FilterBasePanel::GetWidthAndHeightValues(wxString &width, wxString &height, bool required)
{
    width = StrTrim(m_Width->GetValue());
    height = StrTrim(m_Height->GetValue());
    if (required && (width.Len() == 0)) return ShowError(m_Width, FFQS(SID_FILTER_WIDTH_HEIGHT_REQUIRED));
    if (required && (height.Len() == 0)) return ShowError(m_Height, FFQS(SID_FILTER_WIDTH_HEIGHT_REQUIRED));
    return true;
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetWidthAndHeightValues(int width, int height)
{
    m_Width->SetValue(ToStr(width));
    m_Height->SetValue(ToStr(height));
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetWidthAndHeightValues(wxString &from)
{
    m_Width->SetValue(GetToken(from, COMMA));
    m_Height->SetValue(GetToken(from, COMMA));
}

//---------------------------------------------------------------------------------------

void FilterBasePanel::SetWidthAndHeightValues(wxString width, wxString height)
{
    m_Width->SetValue(width);
    m_Height->SetValue(height);
}
