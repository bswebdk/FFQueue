/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Equalizer.cpp                                                   *
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

#include "Equalizer.h"
#include "HighLowBandPass.h"
#include <wx/statline.h>

//---------------------------------------------------------------------------------------

const int EQ_FREQ_COUNT = 10;
const int EQ_FREQUENCIES[EQ_FREQ_COUNT] = { 32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000 };

//---------------------------------------------------------------------------------------

wxString freqToStr(int freq)
{
   return (freq < 1000) ? ToStr(freq) : ToStr((int)freq/1000) + "K";
}

//---------------------------------------------------------------------------------------

wxString gainToStr(int gain)
{
    if (gain >= 0) return "+" + ToStr(gain);
    return ToStr(gain);
}

//---------------------------------------------------------------------------------------

Equalizer::Equalizer(wxWindow* parent) : FilterBasePanel(parent)
{

    SetControlPadding(0); //Label padding

    wxFlexGridSizer *fgs = new wxFlexGridSizer(3, 1, 0, 0),
                    *fgs1 = new wxFlexGridSizer(3, EQ_FREQ_COUNT + 2, 0, 0), *fgs2;

    //Left gain labels
    fgs2 = new wxFlexGridSizer(3, 1, 0, 0);
    fgs2->AddGrowableRow(1);
    MakeLabel(_T("+20"), fgs2, wxALIGN_RIGHT|wxALIGN_TOP);
    MakeLabel(_T("0"), fgs2, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
    MakeLabel(_T("-20"), fgs2, wxALIGN_RIGHT|wxALIGN_BOTTOM);

    fgs1->Add(fgs2, 1, wxEXPAND, 0);

    //Band adjusters
    m_EqBands = new EQ_BAND[EQ_FREQ_COUNT];
    int pw = 0;
    for (int i = 0; i < EQ_FREQ_COUNT; i++)
    {
        int f = EQ_FREQUENCIES[i],
            w = (pw == 0) ? f / 4 : f - (EQ_FREQUENCIES[i-1] + pw);
        MakeEqBand(fgs1, m_EqBands[i], f, w);
    }

    //Right gain labels
    fgs2 = new wxFlexGridSizer(3, 1, 0, 0);
    fgs2->AddGrowableRow(1);
    MakeLabel(_T("+20"), fgs2, wxALIGN_LEFT|wxALIGN_TOP);
    MakeLabel(_T("0"), fgs2, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
    MakeLabel(_T("-20"), fgs2, wxALIGN_LEFT|wxALIGN_BOTTOM);

    fgs1->Add(fgs2, 1, wxEXPAND, 0);

    //Bottom adjuster labels
    fgs1->Add(-1, -1, 1, wxEXPAND, 0);
    for (int i = 0; i < EQ_FREQ_COUNT; i++)
        MakeLabel(freqToStr(m_EqBands[i].freq), fgs1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP);
    fgs1->Add(-1, -1, 1, wxEXPAND, 0);

    //Bottom value labels
    fgs1->Add(-1, -1, 1, wxEXPAND, 0);
    for (int i = 0; i < EQ_FREQ_COUNT; i++)
    {
        LPEQ_BAND band = &m_EqBands[i];
        band->label = MakeLabel(_T("0"), fgs1, wxEXPAND|wxALIGN_TOP);
        band->label->SetWindowStyle(band->label->GetWindowStyle() | wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);
    }
    fgs1->Add(-1, -1, 1, wxEXPAND, 0);

    //Add whole equalizer to main sizer
    fgs->Add(fgs1, 1, wxEXPAND, 0);

    //Add static line as separator
    wxStaticLine *sl = new wxStaticLine(this);
    fgs->Add(sl, 1, wxTOP|wxBOTTOM|wxEXPAND, 10);


    //Make sizer for band-width definition
    fgs2 = new wxFlexGridSizer(1, 3, 0, 0);
    MakeLabel(FFQS(SID_EQUALIZER_BAND_WIDTH), fgs2);

    m_BandWidth = new wxTextCtrl(this, wxID_ANY, "1.5", wxDefaultPosition, wxSize(-1, -1));
    wxFloatingPointValidator<float> fVal(NULL, wxNUM_VAL_NO_TRAILING_ZEROES);
    fVal.SetRange(0, 99999);
    m_BandWidth->SetValidator(fVal);
    fgs2->Add(m_BandWidth, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    wxString *as = FFQL()->GetStringArray(SID_BANDPASS_BAND_WIDTH_TYPES, BAND_WIDTH_TYPE_COUNT);
    m_WidthType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, BAND_WIDTH_TYPE_COUNT, as);
    m_WidthType->SetSelection(1);
    delete[] as;
    fgs2->Add(m_WidthType, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    //Add band-width controls to main sizer
    fgs->Add(fgs2, 1, wxBOTTOM|wxALIGN_LEFT, 3);

    //Set main sizer
    SetSizer(fgs);
    fgs->Fit(this);
    //fgs->SetSizeHints(this);

    Connect(wxID_ANY, wxEVT_SLIDER, (wxObjectEventFunction)&Equalizer::OnSliderChange);

}

//---------------------------------------------------------------------------------------

Equalizer::~Equalizer()
{
   delete[] m_EqBands;
}

//---------------------------------------------------------------------------------------

void Equalizer::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    for (int i = 0; i < EQ_FREQ_COUNT; i++)
    {
        if (fs.Len() == 0) m_EqBands[i].gain->SetValue(20);
        else m_EqBands[i].gain->SetValue(Str2Long(GetToken(fs, ',')) + 20);
    }

    if (fs.Len() > 0) m_BandWidth->SetValue(GetToken(fs, ','));
    m_WidthType->SetSelection(Str2Long(fs, m_WidthType->GetSelection()));

    UpdateLabels();

}

//---------------------------------------------------------------------------------------

bool Equalizer::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftEQUALIZER;

    bool all_null = true;
    wxString uf, ff, ef, s, w, wt;

    w = m_BandWidth->GetValue();
    wt = WIDTH_TYPES[m_WidthType->GetSelection()];

    for (int i = 0; i < EQ_FREQ_COUNT; i++)
    {

        LPEQ_BAND b = &m_EqBands[i];
        int gain = b->gain->GetValue() - 20;

        if (gain != 0)
        {

            all_null = false;

            //UserFriendly
            s.Printf("%sHz%s ", freqToStr(b->freq), gainToStr(gain));
            uf += s;

            //FFMpeg
            s.Printf("equalizer=f=%i:g=%i:w=%s:width_type=%s,", b->freq, gain, w, wt);
            ff += s;

        }

        //Editable (add always)
        s.Printf("%i,", gain);
        ef += s;

    }

    if (all_null) return ShowError(FFQS(SID_EQUALIZER_ERROR));

    //Add band-width to editable
    s.Printf("%s,%i", w, m_WidthType->GetSelection());

    uf.Remove(uf.Len() - 1, 1);
    ff.Remove(ff.Len() - 1, 1);
    //ef.Remove(ff.Len() - 1, 1); //Leave last ","

    fltr->friendly = FFQSF(SID_EQUALIZER_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], uf);
    fltr->ff_filter.Printf("%s%s%s", FILTER_AUDIO_IN, ff, FILTER_AUDIO_OUT);
    fltr->editable = ef + s;

    return true;

}

//---------------------------------------------------------------------------------------

void Equalizer::MakeEqBand(wxSizer *addToSizer, EQ_BAND &band, int f, float w)
{
    band.freq = f;
    //band.width = w;
    band.gain = new wxSlider(this, wxID_ANY, 20, 0, 40, wxDefaultPosition, wxSize(-1, 300), wxSL_VERTICAL|wxSL_INVERSE);
    addToSizer->Add(band.gain, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 15);
}

//---------------------------------------------------------------------------------------

void Equalizer::OnSliderChange(wxCommandEvent &event)
{
    event.Skip();
    UpdateLabels();
}

//---------------------------------------------------------------------------------------

void Equalizer::UpdateLabels(wxSlider *forSlider)
{

    wxString s;

    for (int i = 0; i < EQ_FREQ_COUNT; i++)
    {

        LPEQ_BAND b = &m_EqBands[i];

        if ((forSlider == NULL) || (forSlider == b->gain))
        {

            s.Printf("(%s)", gainToStr(b->gain->GetValue() - 20));
            b->label->SetLabel(s);

        }

    }

}
