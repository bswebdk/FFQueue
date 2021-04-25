/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: HighLowBandPass.cpp                                             *
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

#include "HighLowBandPass.h"

//---------------------------------------------------------------------------------------

const wxString poles[] = { "1", "2" };
const unsigned int BAND_WIDTH_TYPE_COUNT = 4;
const wxString WIDTH_TYPES[] = { "h", "q", "o", "s" };

//---------------------------------------------------------------------------------------

HighLowBandPass::HighLowBandPass(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(0, 99999);

    wxFloatingPointValidator<float> fVal(NULL, wxNUM_VAL_NO_TRAILING_ZEROES);
    fVal.SetRange(0, 99999);

    m_Sizer = new wxFlexGridSizer(1, 10, 0, 0);
    m_MainLab = MakeLabel(SPACE, m_Sizer);

    MakeLabel(" " + FFQS(SID_BANDPASS_FREQUENCY), m_Sizer, wxRIGHT|wxALIGN_CENTER_VERTICAL);
    m_Frequency = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0);
    m_Frequency ->SetValidator(iVal);
    m_Sizer->Add(m_Frequency, 1, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    SetControlPadding(20);
    MakeLabel(FFQS(SID_BANDPASS_HERZ), m_Sizer, wxRIGHT|wxALIGN_CENTER_VERTICAL);
    SetControlPadding(3);

    m_CSG = new wxCheckBox(this, wxID_ANY, FFQS(SID_BANDPASS_SKIRT_GAIN));
    m_Sizer->Add(m_CSG, 1, wxRIGHT|wxALIGN_CENTER_VERTICAL, 20);

    m_PolesLab = MakeLabel(FFQS(SID_BANDPASS_POLES), m_Sizer, wxRIGHT|wxALIGN_CENTER_VERTICAL);
    m_Poles = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, poles);
    m_Sizer->Add(m_Poles, 1, wxRIGHT|wxALIGN_CENTER_VERTICAL, 20);

    m_WidthLab = MakeLabel(FFQS(SID_BANDPASS_BAND_WIDTH), m_Sizer, wxRIGHT|wxALIGN_CENTER_VERTICAL);
    m_Width = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0);
    m_Width->SetValidator(fVal);
    m_Sizer->Add(m_Width, 1, wxRIGHT|wxALIGN_CENTER_VERTICAL, 2);

    wxString *as = FFQL()->GetStringArray(SID_BANDPASS_BAND_WIDTH_TYPES, BAND_WIDTH_TYPE_COUNT);
    m_WidthType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, BAND_WIDTH_TYPE_COUNT, as);
    delete[] as;
    m_Sizer->Add(m_WidthType, 1, wxRIGHT|wxALIGN_CENTER_VERTICAL, 0);

    SetSizer(m_Sizer);
    m_Sizer->Fit(this);
    m_Sizer->SetSizeHints(this);

	Connect(wxID_ANY, wxEVT_CHOICE, (wxObjectEventFunction)&HighLowBandPass::OnChoiceChange);

}

//---------------------------------------------------------------------------------------

HighLowBandPass::~HighLowBandPass()
{

}

//---------------------------------------------------------------------------------------

void HighLowBandPass::SetFilter(LPFFMPEG_FILTER fltr)
{

    m_FilterType = fltr->type;
    wxString fs = fltr->editable, t;
    bool bp = m_FilterType == ftBANDPASS, br = m_FilterType == ftBANDREJECT;

    m_MainLab->SetLabel(FFQL()->FILTER_NAMES[m_FilterType]);

    m_Sizer->Show(m_CSG, bp);
    m_Sizer->Show(m_PolesLab, (!bp) && (!br));
    m_Sizer->Show(m_Poles, (!bp) && (!br));
    m_Sizer->Fit(this);
    m_Sizer->SetSizeHints(this);

    wxWindow *pw = GetParent();
    while (pw != NULL) { pw->Fit(); pw = pw->GetParent(); }

    if (m_FilterType == ftLOWPASS) t = "500";
    else if ( (m_FilterType == ftHIGHPASS) || (m_FilterType == ftBANDPASS) || (m_FilterType == ftBANDREJECT) ) t = "3000";
    else ThrowError(FFQSF(SID_BANDPASS_BAD_FILTERTYPE, (unsigned int)m_FilterType));

    if (fs.Len() == 0) fs = t + ",0,2,0.707,1"; //Set default values

    m_Frequency->SetValue(GetToken(fs, COMMA));
    m_CSG->SetValue(GetToken(fs, COMMA) == "1");
    m_Poles->SetSelection(Str2Long(GetToken(fs, COMMA)) - 1);
    m_Width->SetValue(GetToken(fs, COMMA));
    m_WidthType->SetSelection(Str2Long(GetToken(fs, COMMA)));

    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool HighLowBandPass::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = m_FilterType;

    wxString freq = m_Frequency->GetValue(), s,
             csg = m_CSG->GetValue() ? "1" : "0",
             poles = m_Poles->GetStringSelection(),
             wt = WIDTH_TYPES[m_WidthType->GetSelection()];

    double width = 0;
    m_Width->GetValue().ToDouble(&width);

    if (m_FilterType == ftBANDPASS) s = SPACE + FFQS(SID_BANDPASS_SKIRT_GAIN_FRIENDLY) + csg;
    else if (m_FilterType != ftBANDREJECT) s = SPACE + FFQS(SID_BANDPASS_POLES) + poles;

    fltr->friendly = FFQSF(SID_BANDPASS_USERFRIENDLY, m_MainLab->GetLabel(), freq, s, width, wt == "h" ? "Hz" : wt.Upper());

    if (m_FilterType == ftBANDPASS)

        fltr->ff_filter.Printf("%s%s=f=%s:csg=%s:w=%g:width_type=%s%s", FILTER_AUDIO_IN, m_MainLab->GetLabel().Lower(), freq, csg, width, wt, FILTER_AUDIO_OUT);

    else if (m_FilterType == ftBANDREJECT)

        fltr->ff_filter.Printf("%s%s=f=%s:w=%g:width_type=%s%s", FILTER_AUDIO_IN, m_MainLab->GetLabel().Lower(), freq, width, wt, FILTER_AUDIO_OUT);

    else

        fltr->ff_filter.Printf("%s%s=f=%s:p=%s:w=%g:width_type=%s%s", FILTER_AUDIO_IN, m_MainLab->GetLabel().Lower(), freq, poles, width, wt, FILTER_AUDIO_OUT);

    fltr->editable.Printf("%s,%s,%s,%g,%i", freq, csg, poles, width, m_WidthType->GetSelection());

    return true;

}

//---------------------------------------------------------------------------------------

void HighLowBandPass::OnChoiceChange(wxCommandEvent &event)
{

    if (event.GetEventObject() == m_Poles) UpdateControls();

}

//---------------------------------------------------------------------------------------

void HighLowBandPass::UpdateControls()
{

    bool w = m_Poles->GetSelection() > 0;

    m_WidthLab->Enable(w);
    m_Width->Enable(w);
    m_WidthType->Enable(w);

}
