/*************************************************************************
*                                                                        *
*  Copyright (c) 2023 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FullSpec.cpp                                                  *
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

#include "FullSpec.h"

FullSpec::FullSpec(wxWindow* parent, void *data) : FilterBasePanel(parent)
{

    m_FullSpec = (FULLSPEC_FILE*)data;
    m_HasTime = m_FullSpec->extra.Find("time") >= 0;

    FFQFullSpec::MakeControlsFor(*m_FullSpec, this);
    wxFlexGridSizer *fgs = new wxFlexGridSizer(m_HasTime ? 2 : 1, 1, 0, 0);
    fgs->Add(m_FullSpec->evt_handler->GetParent(), 1, wxALL|wxEXPAND, 0);

    if (m_HasTime) fgs->Add(GetTimeLimitControls(true), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs);
    fgs->Fit(this);
    //fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

FullSpec::~FullSpec()
{

    FFQFullSpec::ClearControlsFor(*m_FullSpec, true);

}

//---------------------------------------------------------------------------------------

void FullSpec::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString after;
    wxString tv = fltr->editable.BeforeFirst(COLON, &after);
    if (m_HasTime) SetTimeLimitValues(tv, false);
    FFQFullSpec::SetCommandLine(*m_FullSpec, m_FullSpec->composite + SPACE + after);
    m_FullSpec->evt_handler->UpdateControls();

}

//---------------------------------------------------------------------------------------

bool FullSpec::GetFilter(LPFFMPEG_FILTER fltr)
{

    if (!FFQFullSpec::ValidateCtrls(*m_FullSpec, true)) return false;

    fltr->type = ftFULLSPEC;

    //Filter
    wxString cmd, rest;
    FFQFullSpec::GetCommandLine(*m_FullSpec, cmd, DQUOTE, QUOTE);
    cmd = cmd.BeforeFirst(SPACE, &rest); //Filter name
    rest.Remove(0, 1); //First quote
    rest.RemoveLast(); //Last quote

    //Time
    wxString time(wxEmptyString), frtime(wxEmptyString), fftime(wxEmptyString);
    if (m_HasTime)
    {
        uint64_t f, t;
        GetTimeLimitValues(f, t, false);
        time.Printf(UINT64FMT + COMMA + UINT64FMT, f, t);
        GetTimeLimitFriendly(frtime);
        GetTimeLimitFilter(fftime, true);
    }

    //Compose
    fltr->editable = time + COLON + rest;
    if (m_FullSpec->matches.StartsWith('a')) fltr->ff_filter = FILTER_AUDIO_IN + cmd + EQUAL + rest + fftime + FILTER_AUDIO_OUT;
    else fltr->ff_filter = FILTER_VIDEO_IN + cmd + EQUAL + rest + fftime + FILTER_VIDEO_OUT;
    rest.Replace(COLON, SPACE);
    rest.Replace(EQUAL, COLON);
    fltr->friendly = m_FullSpec->display + EQUAL + rest + SPACE + frtime;
    return true;

}
