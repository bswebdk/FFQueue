/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Scale.cpp                                                       *
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

#include "Scale.h"

//---------------------------------------------------------------------------------------

const size_t ALGORITHM_COUNT = 12;
const wxString ALGORITHMS[ALGORITHM_COUNT] = {
    "default", "fast_bilinear", "bilinear", "bicubic", "experimental",
    "neighbor", "area", "bicublin", "gauss", "sinc", "lanczos", "spline"
};

//---------------------------------------------------------------------------------------

const size_t DITHER_COUNT = 6;
const wxString DITHERS[DITHER_COUNT] = {
    "auto", "none", "bayer", "ed", "a_dither", "x_dither"
};

//---------------------------------------------------------------------------------------

const size_t INTERLACE_COUNT = 3;
const wxString INTERLACE[INTERLACE_COUNT] = { "0", "1", "-1" };

//---------------------------------------------------------------------------------------

const size_t COLOR_SPACE_COUNT = 5;
const wxString COLOR_SPACES[COLOR_SPACE_COUNT] = {
   "auto", "bt709", "fcc", "bt601", "smpte240m"
};

//---------------------------------------------------------------------------------------

const size_t SAMPLE_RANGE_COUNT = 3;
const wxString SAMPLE_RANGES[SAMPLE_RANGE_COUNT] = {
   "auto", "jpeg/full/pc", "mpeg/tv"
};

//---------------------------------------------------------------------------------------

Scale::Scale(wxWindow* parent) : FilterBasePanel(parent)
{


    //wxIntegerValidator<unsigned int> iVal;
    //iVal.SetRange(0, 10000);
    wxValidator iVal = wxDefaultValidator;
    wxString *as;

    wxFlexGridSizer *fgs1, *fgs2, *fgs3;

    //Main sizer
    fgs1 = new wxFlexGridSizer(11, 1, 0, 0);
    fgs1->AddGrowableCol(0);

    //Explanatory text
    MakeLabel(FFQS(SID_SCALE_EMPTY_INFO), fgs1)->Enable(false);

        //Resize controls
        fgs2 = new wxFlexGridSizer(1, 2, 0, 0);
        MakeLabel(FFQS(SID_SCALE_RESIZE_TO), fgs2);
        fgs2->Add(GetWidthAndHeightControls(iVal), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(fgs2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    //Percentage or not
    m_Percent = new wxCheckBox(this, wxID_ANY, FFQS(SID_SCALE_PERCENTAGE), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_Percent, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    //Percentage or not
    m_FitRect = new wxCheckBox(this, wxID_ANY, FFQS(SID_SCALE_FIT_RECT), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_FitRect, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        //Scaling algorithm
        fgs2 = new wxFlexGridSizer(5, 2, 0, 0);
        fgs2->AddGrowableCol(1);
        MakeLabel(FFQS(SID_SCALE_ALGORITHM), fgs2);
        as = FFQL()->GetStringArray(SID_SCALE_ALGORITHM_NAMES, ALGORITHM_COUNT);
        m_Algorithm = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ALGORITHM_COUNT, as);
        delete[] as;
        fgs2->Add(m_Algorithm, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

        //Dithering algorithm
        MakeLabel(FFQS(SID_SCALE_DITHER), fgs2);
        as = FFQL()->GetStringArray(SID_SCALE_DITHER_NAMES, DITHER_COUNT);
        m_Dither = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, DITHER_COUNT, as);
        delete[] as;
        fgs2->Add(m_Dither, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

        //Interlaced awareness
        MakeLabel(FFQS(SID_SCALE_INTERLACE_AWARE), fgs2);
        as = FFQL()->GetStringArray(SID_SCALE_INTERLACE_AWARE_NAMES, INTERLACE_COUNT);
        m_Interlace = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, INTERLACE_COUNT, as);
        delete[] as;
        fgs2->Add(m_Interlace, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

        //Color space
        MakeLabel(FFQS(SID_SCALE_COLOR_SPACE), fgs2);
        fgs3 = new wxFlexGridSizer(1, 4, 0, 0);
        fgs3->AddGrowableCol(1);
        fgs3->AddGrowableCol(3);
        MakeLabel(FFQS(SID_COMMON_INPUT), fgs3);
        m_ClrSpaceIn = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, COLOR_SPACE_COUNT, COLOR_SPACES);
        fgs3->Add(m_ClrSpaceIn, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
        MakeLabel(FFQS(SID_COMMON_OUTPUT), fgs3);
        m_ClrSpaceOut = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, COLOR_SPACE_COUNT, COLOR_SPACES);
        fgs3->Add(m_ClrSpaceOut, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
        fgs2->Add(fgs3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

        //Sample range
        MakeLabel(FFQS(SID_SCALE_SAMPLE_RANGE), fgs2);
        fgs3 = new wxFlexGridSizer(1, 4, 0, 0);
        fgs3->AddGrowableCol(1);
        fgs3->AddGrowableCol(3);
        MakeLabel(FFQS(SID_COMMON_INPUT), fgs3);
        m_SampleRngIn = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, SAMPLE_RANGE_COUNT, SAMPLE_RANGES);
        fgs3->Add(m_SampleRngIn, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
        MakeLabel(FFQS(SID_COMMON_OUTPUT), fgs3);
        m_SampleRngOut = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, SAMPLE_RANGE_COUNT, SAMPLE_RANGES);
        fgs3->Add(m_SampleRngOut, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
        fgs2->Add(fgs3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    fgs1->Add(-1, 10, 1); //Spacer
    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    fgs1->Add(-1, 10, 1); //Spacer

    //Other flags
    m_AccurateRnd = new wxCheckBox(this, wxID_ANY, FFQS(SID_SCALE_ACCURATE_ROUNDING), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_AccurateRnd, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    m_FullChromaInt = new wxCheckBox(this, wxID_ANY, FFQS(SID_SCALE_FULL_CHROMA_INTERPOLATE), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_FullChromaInt, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    m_FullChromaInp = new wxCheckBox(this, wxID_ANY, FFQS(SID_SCALE_FULL_CHROMA_INPUT), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_FullChromaInp, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    m_BitExact = new wxCheckBox(this, wxID_ANY, FFQS(SID_SCALE_BIT_EXACT_OUTPUT), wxDefaultPosition, wxDefaultSize, 0);
    fgs1->Add(m_BitExact, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs1);
    fgs1->Fit(this);
    fgs1->SetSizeHints(this);

    Connect(wxID_ANY, wxEVT_CHECKBOX, (wxObjectEventFunction)&Scale::OnCheckBox);

}

//---------------------------------------------------------------------------------------

Scale::~Scale()
{

}

//---------------------------------------------------------------------------------------

void Scale::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable, w, h;

    if ((fs.Len() > 0) && (fs.Find(':') < 0))
    {
        //Set old filter style
        w = GetToken(fs, ',');
        h = GetToken(fs, ',');
        if (w == "0") w = "";
        if (h == "0") h = "";
        SetWidthAndHeightValues(w, h);
        m_Percent->SetValue(fs == STR_YES);
        m_Algorithm->SetSelection(0);
        m_Dither->SetSelection(0);
        m_Interlace->SetSelection(0);
        m_ClrSpaceIn->SetSelection(0);
        m_ClrSpaceOut->SetSelection(0);
        m_SampleRngIn->SetSelection(0);
        m_SampleRngOut->SetSelection(0);
        return;
    }

    //Always set w&h - defaults to empty fields anyway
    w = GetToken(fs, ':');
    h = GetToken(fs, ':');
    SetWidthAndHeightValues(w, h);

    //Load values from filter string
    m_Percent->SetValue( STRBOOLDEF(GetToken(fs, ':'), m_Percent->GetValue()) );
    m_Algorithm->SetSelection(Str2Long(GetToken(fs, ':'), 0));
    m_Dither->SetSelection(Str2Long(GetToken(fs, ':'), 0));
    m_AccurateRnd->SetValue( STRBOOLDEF(GetToken(fs, ':'), m_AccurateRnd->GetValue()) );
    m_FullChromaInt->SetValue( STRBOOLDEF(GetToken(fs, ':'), m_FullChromaInt->GetValue()) );
    m_FullChromaInp->SetValue( STRBOOLDEF(GetToken(fs, ':'), m_FullChromaInp->GetValue()) );
    m_BitExact->SetValue( STRBOOLDEF(GetToken(fs, ':'), m_BitExact->GetValue()) );
    m_Interlace->SetSelection(Str2Long(GetToken(fs, ':'), 0));
    m_ClrSpaceIn->SetSelection(Str2Long(GetToken(fs, ':'), 0));
    m_ClrSpaceOut->SetSelection(Str2Long(GetToken(fs, ':'), 0));
    m_SampleRngIn->SetSelection(Str2Long(GetToken(fs, ':'), 0));
    m_SampleRngOut->SetSelection(Str2Long(GetToken(fs, ':'), 0));
    m_FitRect->SetValue( STRBOOLDEF(GetToken(fs, ':'), m_FitRect->GetValue()) );

    if (m_Percent->GetValue() && m_FitRect->GetValue()) m_FitRect->SetValue(false);

}

//---------------------------------------------------------------------------------------

void sapp(wxString &s, const wxString val)
{
    if (s.Len() > 0) s += ":";
    s += val;
}

//---------------------------------------------------------------------------------------

bool Scale::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftSCALE;

    wxString width, height;
    GetWidthAndHeightValues(width, height);
    if (width == "0") width = "";
    if (height == "0") height = "";
    int wlen = width.Len(), hlen = height.Len();
    double numw = Str2Float(width), numh = Str2Float(height);

    if ( (wlen == 0) && (hlen == 0) ) return ShowError(FFQS(SID_SCALE_SIZE_ERROR));

    if ( (numw < 0) || (numh < 0) ) return ShowError(FFQS(SID_SCALE_NEGATIVE_NUMBER_ERROR));

    if ( m_Percent->GetValue() && ( ((wlen>0) && (numw==0)) || ((hlen>0) && (numh==0)) ) )
        return ShowError(FFQS(SID_SCALE_PERCENTAGE_ERROR));

    if ( m_FitRect->GetValue() && ( (numw<=0) || (numh<=0)) )
        return ShowError(FFQS(SID_SCALE_FIT_RECT_ERROR));

    wxString sw = width, sh = height, s = "",
             algorithm = ALGORITHMS[m_Algorithm->GetSelection()],
             dither = DITHERS[m_Dither->GetSelection()],
             interlace = INTERLACE[m_Interlace->GetSelection()],
             clrin = COLOR_SPACES[m_ClrSpaceIn->GetSelection()],
             clrout = COLOR_SPACES[m_ClrSpaceOut->GetSelection()],
             srngin = SAMPLE_RANGES[m_SampleRngIn->GetSelection()],
             srngout = SAMPLE_RANGES[m_SampleRngOut->GetSelection()];

    if (wlen == 0) sw = "auto";
    else if (m_Percent->GetValue()) sw.Printf("%g%%", numw);

    if (hlen == 0) sh = "auto";
    else if (m_Percent->GetValue()) sh.Printf("%g%%", numh);

    fltr->friendly = FFQSF(SID_SCALE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               sw, sh, algorithm, dither, interlace, clrin, clrout, srngin, srngout);

    if (wlen == 0) sw = "-1";
    else if (m_Percent->GetValue()) sw.Printf("iw*%g", numw / 100.0);
    else if (m_FitRect->GetValue()) sw.Printf("'iw*min(%g/iw,%g/ih)'", numw, numh);
    else sw.Printf("%s", width);

    if (hlen == 0) sh = "-1";
    else if (m_Percent->GetValue()) sh.Printf("ih*%g", numh / 100.0);
    else if (m_FitRect->GetValue()) sh.Printf("'ih*min(%g/iw,%g/ih)'", numw, numh);
    else sh.Printf("%s", height);

    //Algorithm and flags
    if (m_Algorithm->GetSelection() > 0) s += algorithm + "+";
    if (m_AccurateRnd->GetValue()) s += "accurate_rnd+";
    if (m_FullChromaInt->GetValue()) s += "full_chroma_int+";
    if (m_FullChromaInp->GetValue()) s += "full_chroma_inp+";
    if (m_BitExact->GetValue()) s += "bitexact+";
    if (s.Len() > 0) s = ":flags=" + s.Left(s.Len() - 1); //Remove last "+"

    //Dither
    if (m_Dither->GetSelection() > 0) sapp(s, "sws_dither=" + dither);

    //Interlace
    if (m_Interlace->GetSelection() > 0) sapp(s, "interl=" + interlace);

    //Color space in/out
    if (m_ClrSpaceIn->GetSelection() > 0) sapp(s, "in_color_matrix=" + clrin);
    if (m_ClrSpaceOut->GetSelection() > 0) sapp(s, "out_color_matrix=" + clrout);

    //Sample range in/out
    if (m_SampleRngIn->GetSelection() > 0) sapp(s, "in_range=" + srngin);
    if (m_SampleRngOut->GetSelection() > 0) sapp(s, "out_range=" + srngout);

    fltr->ff_filter.Printf(
        "%sscale=%s:%s%s%s", FILTER_VIDEO_IN, sw, sh, s, FILTER_VIDEO_OUT
    );

    fltr->editable.Printf(
        "%s:%s:%s:%u:%u:%s:%s:%s:%s:%u:%u:%u:%u:%u:%s",
        width, height, BOOLSTR(m_Percent->GetValue()),
        m_Algorithm->GetSelection(), m_Dither->GetSelection(),
        BOOLSTR(m_AccurateRnd->GetValue()),
        BOOLSTR(m_FullChromaInt->GetValue()),
        BOOLSTR(m_FullChromaInp->GetValue()),
        BOOLSTR(m_BitExact->GetValue()),
        m_Interlace->GetSelection(),
        m_ClrSpaceIn->GetSelection(), m_ClrSpaceOut->GetSelection(),
        m_SampleRngIn->GetSelection(), m_SampleRngOut->GetSelection(),
        BOOLSTR(m_FitRect->GetValue())
    );

    return true;

}

//---------------------------------------------------------------------------------------

void Scale::OnCheckBox(wxCommandEvent &event)
{

    //Make sure that percentage and fit are not both checked
    event.Skip();
    if ( (event.GetEventObject() == m_Percent) && m_Percent->GetValue() ) m_FitRect->SetValue(false);
    else if ( (event.GetEventObject() == m_FitRect) && m_FitRect->GetValue() ) m_Percent->SetValue(false);

}
