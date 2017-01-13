/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: AudioDelay.cpp                                                  *
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

#include "AudioDelay.h"

AudioDelay::AudioDelay(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(CHANNEL_COUNT + 1, 3, 0, 0);
    //fgs->AddGrowableCol(0);
    //fgs->AddGrowableCol(2);

    MakeLabel(FFQS(SID_ADELAY_CHANNEL_NUMBER), fgs, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);
    fgs->Add(10, -1);
    MakeLabel(FFQS(SID_ADELAY_DELAY_MILLIS), fgs, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(0, 0xFFFFFF);

    for (int i = 0; i < CHANNEL_COUNT; i++)
    {

        MakeLabel(ToStr(i+1), fgs, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);
        fgs->Add(10, -1);
        m_Delays[i] = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
        m_Delays[i]->SetValidator(iVal);
        fgs->Add(m_Delays[i], 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);

    }

    GetTimeLimitControls(fgs, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, true);

}

//---------------------------------------------------------------------------------------

AudioDelay::~AudioDelay()
{

}

//---------------------------------------------------------------------------------------

void AudioDelay::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
        //If only time limit remains, break!
        if (fs.Freq(',') <= 1) break;

        //Extract and set channel delay
        int d = Str2Long(GetToken(fs, ",", true), 0);
        if (d > 0) m_Delays[i]->SetValue(ToStr(d));

    }

    SetTimeLimitValues(fs);

}

//---------------------------------------------------------------------------------------

bool AudioDelay::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftADELAY;

    wxString f = "", tf, ft;
    unsigned int d;

    for (int i = CHANNEL_COUNT - 1; i >= 0; i--)
    {

        //This needs to be done backwards in order to fill in empty channels
        d = Str2Long(m_Delays[i]->GetValue());
        if ((d > 0) || (f.Len() > 0)) f = ToStr(d) + "," + f;
    }

    if (f.Len() == 0) return ShowError(m_Delays[0], FFQS(SID_ADELAY_ERROR));
    f.Remove(f.Len() - 1, 1);

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;
    GetTimeLimitFilter(tf, true);
    GetTimeLimitFriendly(ft);

    fltr->friendly = FFQSF(SID_ADELAY_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], f, ft);
    fltr->editable = f + wxString::Format("," + UINT64FMT + "," + UINT64FMT, t1, t2);
    f.Replace(",", "|");
    fltr->ff_filter.Printf("%sadelay=%s%s%s", FILTER_AUDIO_IN, f, tf, FILTER_AUDIO_OUT);

    return true;

}
