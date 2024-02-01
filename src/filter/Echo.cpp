/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Echo.cpp                                                        *
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

#include "Echo.h"

Echo::Echo(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFloatingPointValidator<float> gainVal, delayVal, decayVal;
    gainVal.SetRange(0, 1);
    delayVal.SetRange(0, 90000);
    decayVal.SetRange(0, 1);

    wxFlexGridSizer *fgs1 = new wxFlexGridSizer(4, 2, 0, 0), *fgs2;

    MakeLabel(FFQS(SID_ECHO_GAIN), fgs1);

        fgs2 = new wxFlexGridSizer(1, 4, 0, 0);
        MakeLabel(FFQS(SID_ECHO_GAIN_INPUT), fgs2);
        m_InGain = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), 0);
        m_InGain->SetValidator(gainVal);
        fgs2->Add(m_InGain, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        MakeLabel(FFQS(SID_ECHO_GAIN_OUTPUT), fgs2);
        m_OutGain = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), 0);
        m_OutGain->SetValidator(gainVal);
        fgs2->Add(m_OutGain, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(fgs2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_ECHO_DELAYS), fgs1);

        fgs2 = new wxFlexGridSizer(1, DELAY_COUNT, 0, 0);
        for (int i = 0; i < DELAY_COUNT; i++)
        {
            m_Delays[i] = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), 0);
            m_Delays[i]->SetValidator(delayVal);
            fgs2->Add(m_Delays[i], 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
        }

    fgs1->Add(fgs2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_ECHO_DECAYS), fgs1);

        fgs2 = new wxFlexGridSizer(1, DECAY_COUNT, 0, 0);
        for (int i = 0; i < DECAY_COUNT; i++)
        {
            m_Decays[i] = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), 0);
            m_Decays[i]->SetValidator(decayVal);
            fgs2->Add(m_Decays[i], 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
        }

    fgs1->Add(fgs2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs1);
    fgs1->Fit(this);
    //fgs1->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Echo::~Echo()
{

}

//---------------------------------------------------------------------------------------

void Echo::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable, dl, dc;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_InGain->SetValue(GetToken(fs, ','));
        m_OutGain->SetValue(GetToken(fs, ','));
        dl = GetToken(fs, ',');
        dc = GetToken(fs, ',');

    }

    else
    {

        //Default values
        m_InGain->SetValue("0.6");
        m_OutGain->SetValue("0.3");
        dl = "1000";
        dc = "0.5";

    }

    for (int i = 0; i < DELAY_COUNT; i++) m_Delays[i]->SetValue(dl.Len() == 0 ? "0" : GetToken(dl, '|'));
    for (int i = 0; i < DECAY_COUNT; i++) m_Decays[i]->SetValue(dc.Len() == 0 ? "0" : GetToken(dc, '|'));

}

//---------------------------------------------------------------------------------------

bool Echo::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftECHO;

    double ig, og, dd;

    if ((!m_InGain->GetValue().ToDouble(&ig)) || (ig == 0) || (!m_OutGain->GetValue().ToDouble(&og)) || (og == 0))
        return ShowError(FFQS(SID_ECHO_GAIN_ERROR));

    wxString dl, dc, s;

    for (int i = 0; i < DELAY_COUNT; i++)
    {
        s = m_Delays[i]->GetValue();
        if (s.ToDouble(&dd) && (dd != 0)) dl += s + "|";
    }

    for (int i = 0; i < DECAY_COUNT; i++)
    {
        s = m_Decays[i]->GetValue();
        if (s.ToDouble(&dd) && (dd != 0)) dc += s + "|";
    }

    if ((dl.Len() == 0) || (dc.Len() == 0)) return ShowError(FFQS(SID_ECHO_DELAY_DECAY_ERROR));

    dl.Remove(dl.Len() - 1, 1);
    dc.Remove(dc.Len() - 1, 1);

    fltr->friendly = FFQSF(SID_ECHO_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], ig, og, dl, dc);

    fltr->ff_filter.Printf("%saecho=%g:%g:%s:%s%s", FILTER_AUDIO_IN, ig, og, dl, dc, FILTER_AUDIO_OUT);

    fltr->editable.Printf("%s,%s,%s,%s", m_InGain->GetValue(), m_OutGain->GetValue(), dl, dc);

    return true;

}
