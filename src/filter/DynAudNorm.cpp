/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: DynAudNorm.cpp                                                  *
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

#include "DynAudNorm.h"

#define FRAME_LENGTH(v) ( v==0 ? 10 : (int)((float)v*((float)8000/400)) )
#define GAUSSIAN_WND(v) ((v*2)+3)
#define PEAK_VALUE(v) ((float)v/400.0f)
#define MAX_GAIN(v) ((float)v/4.0f+1.0f)
#define COMPRESS_FACTOR(v) ((float)v/10.0f)

//---------------------------------------------------------------------------------------

DynAudNorm::DynAudNorm(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs;

    fgs = new wxFlexGridSizer(10, 4, 0, 0); //Rows, Cols, HGap, VGap
    fgs->AddGrowableCol(1);

    MakeLabel(FFQS(SID_DYNAUDNORM_FRAME_LENGTH), fgs);
    m_FrameLen = new wxSlider(this, wxID_ANY, 25, 0, 400, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_FrameLen, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(20, -1);
    m_FrameLenText = MakeLabel("00000", fgs);
    wxSize ms = wxSize(m_FrameLenText->GetSize().GetWidth(), -1);
    m_FrameLenText->SetMinSize(ms);

    MakeLabel(FFQS(SID_DYNAUDNORM_GAUS_WND_SIZE), fgs);
    m_GaussWnd = new wxSlider(this, wxID_ANY, 14, 0, 149, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_GaussWnd, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(20, -1);
    m_GaussWndText = MakeLabel(wxEmptyString, fgs);
    m_GaussWndText->SetMinSize(ms);

    MakeLabel(FFQS(SID_DYNAUDNORM_TARGET_PEAK_VALUE), fgs);
    m_PeakVal = new wxSlider(this, wxID_ANY, 380, 0, 400, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_PeakVal, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(20, -1);
    m_PeakValText = MakeLabel(wxEmptyString, fgs);
    m_PeakValText->SetMinSize(ms);

    MakeLabel(FFQS(SID_DYNAUDNORM_MAX_GAIN_FACTOR), fgs);
    m_MaxGain = new wxSlider(this, wxID_ANY, 36, 0, 396, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_MaxGain, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(20, -1);
    m_MaxGainText = MakeLabel(wxEmptyString, fgs);
    m_MaxGainText->SetMinSize(ms);

    MakeLabel(FFQS(SID_DYNAUDNORM_TARGET_RMS), fgs);
    m_TargetRMS = new wxSlider(this, wxID_ANY, 0, 0, 400, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_TargetRMS, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(20, -1);
    m_TargetRMSText = MakeLabel(wxEmptyString, fgs);
    m_TargetRMSText->SetMinSize(ms);

    MakeLabel(FFQS(SID_DYNAUDNORM_COMPRESS_FACTOR), fgs);
    m_CompressFactor = new wxSlider(this, wxID_ANY, 0, 0, 300, wxDefaultPosition, wxSize(450, -1));
    fgs->Add(m_CompressFactor, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(20, -1);
    m_CompressFactorText = MakeLabel(wxEmptyString, fgs);
    m_CompressFactorText->SetMinSize(ms);

    fgs->Add(-1, 10);
    fgs->Add(-1, 10);
    fgs->Add(-1, 10);
    fgs->Add(-1, 10);

    fgs->Add(-1, -1);
    m_ChannelCoupling = new wxCheckBox(this, wxID_ANY, FFQS(SID_DYNAUDNORM_CHANNEL_COUPLING));
    fgs->Add(m_ChannelCoupling, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(-1, -1);
    fgs->Add(-1, -1);

    fgs->Add(-1, -1);
    m_DCBiasCorrection = new wxCheckBox(this, wxID_ANY, FFQS(SID_DYNAUDNORM_DC_BIAS_CORRECT));
    fgs->Add(m_DCBiasCorrection, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(-1, -1);
    fgs->Add(-1, -1);

    fgs->Add(-1, -1);
    m_AltBoundaryMode = new wxCheckBox(this, wxID_ANY, FFQS(SID_DYNAUDNORM_BOUNDARY_MODE));
    fgs->Add(m_AltBoundaryMode, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(-1, -1);
    fgs->Add(-1, -1);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

    Connect(wxID_ANY, wxEVT_SLIDER, (wxObjectEventFunction)&DynAudNorm::OnSliderChange);

}

//---------------------------------------------------------------------------------------

DynAudNorm::~DynAudNorm()
{

}

//---------------------------------------------------------------------------------------

void DynAudNorm::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    m_FrameLen->SetValue(Str2Long(GetToken(fs, ",", true), m_FrameLen->GetValue()));
    m_GaussWnd->SetValue(Str2Long(GetToken(fs, ",", true), m_GaussWnd->GetValue()));
    m_PeakVal->SetValue(Str2Long(GetToken(fs, ",", true), m_PeakVal->GetValue()));
    m_MaxGain->SetValue(Str2Long(GetToken(fs, ",", true), m_MaxGain->GetValue()));
    m_TargetRMS->SetValue(Str2Long(GetToken(fs, ",", true), m_TargetRMS->GetValue()));
    m_CompressFactor->SetValue(Str2Long(GetToken(fs, ",", true), m_CompressFactor->GetValue()));

    m_ChannelCoupling->SetValue(STRBOOLDEF(GetToken(fs, ",", true), true));
    m_DCBiasCorrection->SetValue(STRBOOLDEF(GetToken(fs, ",", true), false));
    m_AltBoundaryMode->SetValue(STRBOOLDEF(GetToken(fs, ",", true), false));

    wxCommandEvent dummy;
    OnSliderChange(dummy);

}

//---------------------------------------------------------------------------------------

bool DynAudNorm::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftDYNAUDNORM;

    int flen = FRAME_LENGTH(m_FrameLen->GetValue()),
        gwnd = GAUSSIAN_WND(m_GaussWnd->GetValue());
    float pval = PEAK_VALUE(m_PeakVal->GetValue()),
          mgain = MAX_GAIN(m_MaxGain->GetValue()),
          trms = PEAK_VALUE(m_TargetRMS->GetValue()),
          cfact = COMPRESS_FACTOR(m_CompressFactor->GetValue());

    fltr->friendly = FFQSF(SID_DYNAUDNORM_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                           flen, gwnd, pval, mgain, trms, cfact,
                           BOOLSTR(m_ChannelCoupling->GetValue()),
                           BOOLSTR(m_DCBiasCorrection->GetValue()),
                           BOOLSTR(m_AltBoundaryMode->GetValue()));

    fltr->ff_filter.Printf("%sdynaudnorm=f=%u:g=%u:p=%g:m=%g:r=%g:s=%g:n=%s:c=%s:b=%s%s",
                           FILTER_AUDIO_IN,
                           flen, gwnd, pval, mgain, trms, cfact,
                           BOOLSTR(m_ChannelCoupling->GetValue()),
                           BOOLSTR(m_DCBiasCorrection->GetValue()),
                           BOOLSTR(m_AltBoundaryMode->GetValue()),
                           FILTER_AUDIO_OUT);

    fltr->editable.Printf("%u,%u,%u,%u,%u,%u,%s,%s,%s",
                          m_FrameLen->GetValue(),
                          m_GaussWnd->GetValue(),
                          m_PeakVal->GetValue(),
                          m_MaxGain->GetValue(),
                          m_TargetRMS->GetValue(),
                          m_CompressFactor->GetValue(),
                          BOOLSTR(m_ChannelCoupling->GetValue()),
                          BOOLSTR(m_DCBiasCorrection->GetValue()),
                          BOOLSTR(m_AltBoundaryMode->GetValue()));

    return true;

}

//---------------------------------------------------------------------------------------

void DynAudNorm::OnSliderChange(wxCommandEvent &event)
{
    event.Skip();
    wxObject *o = event.GetEventObject();
    if ((o == NULL) || (o == m_FrameLen)) m_FrameLenText->SetLabel(wxString::Format("%u", FRAME_LENGTH(m_FrameLen->GetValue())));
    if ((o == NULL) || (o == m_GaussWnd)) m_GaussWndText->SetLabel(wxString::Format("%u", GAUSSIAN_WND(m_GaussWnd->GetValue())));
    if ((o == NULL) || (o == m_PeakVal)) m_PeakValText->SetLabel(wxString::Format("%.2f", PEAK_VALUE(m_PeakVal->GetValue())));
    if ((o == NULL) || (o == m_MaxGain)) m_MaxGainText->SetLabel(wxString::Format("%.2f", MAX_GAIN(m_MaxGain->GetValue())));
    if ((o == NULL) || (o == m_TargetRMS)) m_TargetRMSText->SetLabel(wxString::Format("%.2f", PEAK_VALUE(m_TargetRMS->GetValue())));
    if ((o == NULL) || (o == m_CompressFactor)) m_CompressFactorText->SetLabel(wxString::Format("%.2f", COMPRESS_FACTOR(m_CompressFactor->GetValue())));

}
