/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Phaser.cpp                                                      *
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

#include "Phaser.h"

Phaser::Phaser(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFloatingPointValidator<float> gainVal, delayVal, decayVal, speedVal;
    gainVal.SetRange(0, 1);
    delayVal.SetRange(0, 90000);
    decayVal.SetRange(0, 10);
    speedVal.SetRange(0, 10);

    wxFlexGridSizer *fgs1 = new wxFlexGridSizer(5, 2, 0, 0), *fgs2;

    MakeLabel(FFQS(SID_ECHO_GAIN), fgs1);

        fgs2 = new wxFlexGridSizer(1, 4, 0, 0);
        MakeLabel(FFQS(SID_ECHO_GAIN_INPUT), fgs2);
        m_InGain = new wxTextCtrl(this, wxID_ANY);
        m_InGain->SetValidator(gainVal);
        fgs2->Add(m_InGain, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        MakeLabel(FFQS(SID_ECHO_GAIN_OUTPUT), fgs2);
        m_OutGain = new wxTextCtrl(this, wxID_ANY);
        m_OutGain->SetValidator(gainVal);
        fgs2->Add(m_OutGain, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(fgs2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_FLANGER_DELAY), fgs1);

        fgs2 = new wxFlexGridSizer(1, 5, 0, 0);
        m_Delay = new wxTextCtrl(this, wxID_ANY);
        m_Delay->SetValidator(delayVal);
        fgs2->Add(m_Delay, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        MakeLabel(FFQS(SID_PHASER_DECAY), fgs2);
        m_Decay = new wxTextCtrl(this, wxID_ANY);
        m_Decay->SetValidator(decayVal);
        fgs2->Add(m_Decay, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        MakeLabel(FFQS(SID_FLANGER_SPEED), fgs2);
        m_Speed = new wxTextCtrl(this, wxID_ANY);
        m_Speed->SetValidator(speedVal);
        fgs2->Add(m_Speed, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(fgs2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_PHASER_TYPE), fgs1);
    wxBoxSizer *bs = new wxBoxSizer(wxHORIZONTAL);
    m_TypeTri = new wxRadioButton(this, wxID_ANY, FFQS(SID_FLANGER_SHAPE_TRI));
    bs->Add(m_TypeTri, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    m_TypeSin = new wxRadioButton(this, wxID_ANY, FFQS(SID_FLANGER_SHAPE_SIN));
    bs->Add(m_TypeSin, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    fgs1->Add(bs, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs1);
    fgs1->Fit(this);
    //fgs1->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Phaser::~Phaser()
{

}

//---------------------------------------------------------------------------------------

void Phaser::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable, s;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_InGain->SetValue(GetToken(fs, ','));
        m_OutGain->SetValue(GetToken(fs, ','));
        m_Delay->SetValue(GetToken(fs, ','));
        m_Decay->SetValue(GetToken(fs, ','));
        m_Speed->SetValue(GetToken(fs, ','));
        m_TypeTri->SetValue(GetToken(fs, ',') == "t");

    }

    else
    {

        //Default values
        m_InGain->SetValue("0.6");
        m_OutGain->SetValue("0.3");
        m_Delay->SetValue("3.0");
        m_Decay->SetValue("0.4");
        m_Speed->SetValue("0.5");
        m_TypeTri->SetValue(true);

    }

    m_TypeSin->SetValue(!m_TypeTri->GetValue());

}

//---------------------------------------------------------------------------------------

bool testDbl(wxTextCtrl *tc, double &dbl)
{
    return tc->GetValue().ToDouble(&dbl) && (dbl > 0);
}

//---------------------------------------------------------------------------------------

bool Phaser::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftPHASER;

    double ig, og, dl, dc, sp;

    if ((!testDbl(m_InGain, ig)) || (!testDbl(m_OutGain, og)) || (!testDbl(m_Delay, dl)) || (!testDbl(m_Decay, dc)) || (!testDbl(m_Speed, sp)))
        return ShowError(FFQS(SID_PHASER_ERROR));

    wxString t = m_TypeTri->GetValue() ? m_TypeTri->GetLabel() : m_TypeSin->GetLabel();
    t.LowerCase();

    fltr->friendly = FFQSF(SID_PHASER_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], ig, og, dl, dc, sp, t);

    t = t.GetChar(0);

    fltr->ff_filter.Printf("%saphaser=%g:%g:%g:%g:%g:%s%s", FILTER_AUDIO_IN, ig, og, dl, dc, sp, t, FILTER_AUDIO_OUT);

    fltr->editable.Printf("%g,%g,%g,%g,%g,%s", ig, og, dl, dc, sp, t);

    return true;

}
