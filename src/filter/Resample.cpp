/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Resample.cpp                                                    *
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

#include "Resample.h"
#include <wx/dcclient.h>

const wxString STR_UNDEF = "undef";

//---------------------------------------------------------------------------------------

const unsigned int MATRIX_ENC_COUNT = 4;
const wxString MATRIX_ENCS[MATRIX_ENC_COUNT] = {
    STR_UNDEF, "none", "dolby", "dplii"
};

//---------------------------------------------------------------------------------------

const unsigned int FILTER_TYPE_COUNT = 4;
const wxString FILTER_TYPES[FILTER_TYPE_COUNT] = {
    STR_UNDEF, "cubic", "blackman_nuttall", "kaiser"
};

//---------------------------------------------------------------------------------------

const unsigned int DITHER_METHOD_COUNT = 11;
const wxString DITHER_METHODS[DITHER_METHOD_COUNT] = {
    STR_UNDEF,
    "rectangular",
    "triangular",
    "triangular_hp",
    "lipshitz",
    "shibata",
    "low_shibata",
    "high_shibata",
    "f_weighted",
    "modified_e_weighted",
    "improved_e_weighted"
};

//---------------------------------------------------------------------------------------

const unsigned int CUSTOM_VALUE_COUNT = 26;
const wxString CUSTOM_VALUES[CUSTOM_VALUE_COUNT] = { //26
    "ich",
    "och",
    "uch",
    "isr",
    "osr",
    "isf",
    "osf",
    "tsf",
    "icl",
    "ocl",
    "clev",
    "slev",
    "lfe_mix_level",
    "rmvol",
    "rematrix_maxval",
    "dither_scale",
    "filter_size",
    "phase_shift",
    "cutoff",
    "first_pts",
    "min_comp",
    "min_hard_comp",
    "comp_duration",
    "max_soft_comp",
    "kaiser_beta",
    "output_sample_bits",
};

//---------------------------------------------------------------------------------------

int CustomValueIndex(wxString cw)
{
    for (unsigned int i = 0; i < CUSTOM_VALUE_COUNT; i++)
        if (CUSTOM_VALUES[i] == cw) return i;
    return -1;
}

//---------------------------------------------------------------------------------------

Resample::Resample(wxWindow* parent) : FilterBasePanel(parent)
{

    wxString *as;
    wxFlexGridSizer *fgs1 = new wxFlexGridSizer(7, 1, 0, 0), *fgs2 = new wxFlexGridSizer(5, 2, 0, 0);
    fgs1->AddGrowableCol(0);
    fgs2->AddGrowableCol(1);

    MakeLabel(FFQS(SID_RESAMPLE_SAMPLE_RATE), fgs2);
    m_Rate = new wxComboBox(this, wxID_ANY);
	m_Rate->Append(FFQS(SID_RESAMPLE_KEEP_ORIGINAL_RATE));
	m_Rate->Append(_T("8000"));
	m_Rate->Append(_T("11025"));
	m_Rate->Append(_T("16000"));
	m_Rate->Append(_T("22050"));
	m_Rate->Append(_T("32000"));
	m_Rate->Append(_T("44100"));
	m_Rate->Append(_T("48000"));
	m_Rate->Append(_T("192000"));
    fgs2->Add(m_Rate, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_RESAMPLE_AUDIO_SYNC), fgs2);
    m_Async = new wxTextCtrl(this, wxID_ANY);
    fgs2->Add(m_Async, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_RESAMPLE_MATRIX_ENCODING), fgs2);
    as = FFQL()->GetStringArray(SID_RESAMPLE_MATRIX_ENC_NAMES, MATRIX_ENC_COUNT);
    m_MatrixEnc = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, MATRIX_ENC_COUNT, as);
    delete[] as;
    fgs2->Add(m_MatrixEnc, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_RESAMPLE_FILTER_TYPE), fgs2);
    as = FFQL()->GetStringArray(SID_RESAMPLE_FILTER_TYPE_NAMES, FILTER_TYPE_COUNT);
    m_FilterType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, FILTER_TYPE_COUNT, as);
    delete[] as;
    fgs2->Add(m_FilterType, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_RESAMPLE_DITHER_METHOD), fgs2);
    as = FFQL()->GetStringArray(SID_RESAMPLE_DITHER_METHOD_NAMES, DITHER_METHOD_COUNT);
    m_Dither = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, DITHER_METHOD_COUNT, as);
    delete[] as;
    fgs2->Add(m_Dither, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND, 0);

    fgs1->Add(-1, 10, 1); //Spacer
    m_ForceResample = new wxCheckBox(this, wxID_ANY, FFQS(SID_RESAMPLE_FORCE_RESAMPLE));
    fgs1->Add(m_ForceResample, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    m_LinearInterp = new wxCheckBox(this, wxID_ANY, FFQS(SID_RESAMPLE_INTERPOLATION));
    fgs1->Add(m_LinearInterp, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    fgs1->Add(-1, 10, 1); //Spacer

    MakeLabel(FFQS(SID_RESAMPLE_CUSTOM_VALUES), fgs1);

    m_Grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 200));
	m_Grid->CreateGrid(CUSTOM_VALUE_COUNT, 1);
	m_Grid->SetDefaultColSize(100);
    m_Grid->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER_VERTICAL);
	m_Grid->EnableEditing(true);
	m_Grid->EnableGridLines(true);
	m_Grid->SetColLabelValue(0, FFQS(SID_RESAMPLE_VALUE));
	m_Grid->SetDefaultCellFont( m_Grid->GetFont() );
	m_Grid->SetDefaultCellTextColour( m_Grid->GetForegroundColour() );

    as = FFQL()->GetStringArray(SID_RESAMPLE_CUSTOM_VALUE_NAMES, CUSTOM_VALUE_COUNT);
    for (unsigned int i = 0; i < CUSTOM_VALUE_COUNT; i++)
    {
        m_Grid->SetRowLabelValue(i, as[i]);
        m_Grid->SetCellAlignment(i, 0, wxALIGN_LEFT, wxALIGN_CENTRE);
    }
    delete[] as;

    fgs1->Add(m_Grid, 1, wxALL|wxEXPAND, 3);

    SetSizer(fgs1);
    fgs1->Fit(this);
    //fgs1->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Resample::~Resample()
{

}

//---------------------------------------------------------------------------------------

bool Resample::Layout()
{
    bool res = FilterBasePanel::Layout();


    //Calculate and set label column width
    wxClientDC dc(this);
    dc.SetFont(m_Grid->GetLabelFont());
    int cw = 0;
    for (unsigned int i = 0; i < CUSTOM_VALUE_COUNT; i++)
    {
        wxSize ts = dc.GetTextExtent(m_Grid->GetRowLabelValue(i));
        if (cw < ts.GetWidth()) cw = ts.GetWidth();
    }
    m_Grid->SetRowLabelSize(cw + 5);

    //Set value column width
    //m_Grid->SetColumnWidth(0, m_Grid->GetClientSize().GetWidth() - cw - 10);
    m_Grid->SetColSize(0, m_Grid->GetClientSize().GetWidth() - cw - 10);

    return res;
}

//---------------------------------------------------------------------------------------

void Resample::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable, s, t;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        s = GetToken(fs, ',');
        if (s == "0") m_Rate->SetSelection(0);
        else m_Rate->SetValue(s);
        m_Async->SetValue(GetToken(fs, ','));
        m_MatrixEnc->SetSelection(Str2Long(GetToken(fs, ',')));
        m_FilterType->SetSelection(Str2Long(GetToken(fs, ',')));
        m_Dither->SetSelection(Str2Long(GetToken(fs, ',')));
        m_ForceResample->SetValue(GetToken(fs, ',') == STR_YES);
        m_LinearInterp->SetValue(GetToken(fs, ',') == STR_YES);

    }

    else
    {

        //Default values
        m_Rate->SetSelection(0);
        m_Async->SetValue("");
        m_MatrixEnc->SetSelection(0);
        m_FilterType->SetSelection(0);
        m_Dither->SetSelection(0);
        m_ForceResample->SetValue(false);
        m_LinearInterp->SetValue(false);

    }

    //Grid
    int idx;
    while (fs.Len() > 0)
    {
        s = GetToken(fs, ':');
        t = GetToken(s, '=');
        idx = CustomValueIndex(t);
        //if (idx >= 0) m_Grid->SetCellValue(s, idx, 0);
        if (idx >= 0) m_Grid->SetCellValue(idx, 0, s);
    }

}

//---------------------------------------------------------------------------------------

bool Resample::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftRESAMPLE;

    wxString s = StrTrim(m_Rate->GetValue());
    if (s == m_Rate->GetString(0)) s = "0";
    int rate = Str2Long(s, -1);

    if (rate < 0) return ShowError(m_Rate, FFQS(SID_RESAMPLE_SAMPLE_RATE_ERROR));

    wxString async = StrTrim(m_Async->GetValue()), grid = "", t;
    s.clear();

    fltr->friendly = FFQSF(SID_RESAMPLE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                            rate, async.Len() == 0 ? STR_UNDEF : async, MATRIX_ENCS[m_MatrixEnc->GetSelection()],
                            FILTER_TYPES[m_FilterType->GetSelection()], DITHER_METHODS[m_Dither->GetSelection()]);

    if (async.Len() > 0) s+= ":async=" + async;
    if (m_MatrixEnc->GetSelection() > 0) s+= ":matrix_encoding=" + MATRIX_ENCS[m_MatrixEnc->GetSelection()];
    if (m_FilterType->GetSelection() > 0) s+= ":filter_type=" + FILTER_TYPES[m_FilterType->GetSelection()];
    if (m_Dither->GetSelection() > 0) s+= ":dither_method=" + DITHER_METHODS[m_Dither->GetSelection()];
    if (m_ForceResample->GetValue()) s+= ":flags=res";
    if (m_LinearInterp->GetValue()) s+= ":linear_iterp=1";

    for (unsigned int i = 0; i < CUSTOM_VALUE_COUNT; i++)
    {
        t = StrTrim(m_Grid->GetCellValue(i, 0));
        if (t.Len() > 0) grid += ":" + CUSTOM_VALUES[i] + "=" + t;
    }

    s += grid;

    if ((rate == 0) && (s.Len() == 0)) return ShowError(m_Rate, FFQS(SID_RESAMPLE_EMPTY_ERROR));

    fltr->ff_filter.Printf("%saresample=%i%s%s", FILTER_AUDIO_IN, rate, s, FILTER_AUDIO_OUT);

    if (grid.Len() > 0) grid.Remove(0, 1);

    fltr->editable.Printf(
        "%i,%s,%i,%i,%i,%s,%s,%s",
        rate, async, m_MatrixEnc->GetSelection(), m_FilterType->GetSelection(), m_Dither->GetSelection(),
        BOOLSTR(m_ForceResample->GetValue()), BOOLSTR(m_LinearInterp->GetValue()), grid
    );

    return true;

}
