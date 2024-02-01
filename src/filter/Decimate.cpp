/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Decimate.cpp                                                    *
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

#include "Decimate.h"

#define DEFAULT_CYCLE 5
#define DEFAULT_DUPTHRESH 1.1
#define DEFAULT_SCTHRESH 15
#define DEFAULT_BLOCKXY 32
#define DEFAULT_PREPROC false
#define DEFAULT_CHROMAMETRIC true

//---------------------------------------------------------------------------------------

Decimate::Decimate(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK);
    iVal.SetRange(0, 0xFFFFFF);

    wxFloatingPointValidator<float> fVal;
    fVal.SetStyle(wxNUM_VAL_ZERO_AS_BLANK | wxNUM_VAL_NO_TRAILING_ZEROES);
    fVal.SetRange(0, 1000);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(6, 2, 0, 0), *fgsx;
    fgs->AddGrowableCol(0);

    MakeLabel(FFQS(SID_DECIMATE_CYCLE), fgs);
    m_Cycle = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_Cycle->SetValidator(iVal);
    fgs->Add(m_Cycle, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_DECIMATE_DUP_THRESHOLD), fgs);
    m_DupThresh = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_DupThresh->SetValidator(fVal);
    fgs->Add(m_DupThresh, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_DECIMATE_SC_THRESHOLD), fgs);
    m_ScThresh = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_ScThresh->SetValidator(iVal);
    fgs->Add(m_ScThresh, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_DECIMATE_BLOCK_SIZE), fgs);
        fgsx = new wxFlexGridSizer(1, 3, 0, 0);
        m_BlockX = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
        m_BlockX->SetValidator(iVal);
        fgsx->Add(m_BlockX, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        MakeLabel(" X ", fgsx);
        m_BlockY = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
        m_BlockY->SetValidator(iVal);
        fgsx->Add(m_BlockY, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    fgs->Add(fgsx, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    m_PreProc = new wxCheckBox(this, wxID_ANY, FFQS(SID_DECIMATE_MAIN_PREPROCESSED));
    fgs->Add(m_PreProc, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(-1, -1);

    m_ChromaMetric = new wxCheckBox(this, wxID_ANY, FFQS(SID_DECIMATE_CHROMA_METRIC));
    fgs->Add(m_ChromaMetric, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    fgs->Add(-1, -1);

    SetSizer(fgs);
    fgs->Fit(this);
    //fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Decimate::~Decimate()
{

}

//---------------------------------------------------------------------------------------

void Decimate::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;


    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Cycle->SetValue(GetToken(fs, ",", true));
        m_DupThresh->SetValue(GetToken(fs, ",", true));
        m_ScThresh->SetValue(GetToken(fs, ",", true));
        m_BlockX->SetValue(GetToken(fs, ",", true));
        m_BlockY->SetValue(GetToken(fs, ",", true));
        m_PreProc->SetValue(GetToken(fs, ",", true) == STR_YES);
        m_ChromaMetric->SetValue(GetToken(fs, ",", true) == STR_YES);

    }

    else
    {

        //Default values
        m_Cycle->SetValue(ToStr(DEFAULT_CYCLE));
        m_DupThresh->SetValue(ToStr(DEFAULT_DUPTHRESH));
        m_ScThresh->SetValue(ToStr(DEFAULT_SCTHRESH));
        m_BlockX->SetValue(ToStr(DEFAULT_BLOCKXY));
        m_BlockY->SetValue(ToStr(DEFAULT_BLOCKXY));
        m_PreProc->SetValue(DEFAULT_PREPROC);
        m_ChromaMetric->SetValue(DEFAULT_CHROMAMETRIC);

    }

}

//---------------------------------------------------------------------------------------

bool Decimate::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftDECIMATE;

    unsigned int cycle = Str2Long(m_Cycle->GetValue(), 0);
    if (cycle < 2) return ShowError(m_Cycle, FFQS(SID_DECIMATE_CYCLE_ERROR));

    double dthresh = Str2Float(m_DupThresh->GetValue(), 0);
    unsigned int scthresh = Str2Long(m_ScThresh->GetValue(), 0),
                 bx = Str2Long(m_BlockX->GetValue(), 0),
                 by = Str2Long(m_BlockY->GetValue(), 0);

    if ((bx <= 0) || (bx % 2 != 0)) return ShowError(m_BlockX, FFQS(SID_DECIMATE_BLOCK_SIZE_ERROR));
    if ((by <= 0) || (by % 2 != 0)) return ShowError(m_BlockY, FFQS(SID_DECIMATE_BLOCK_SIZE_ERROR));

    wxString pp = BOOLSTR(m_PreProc->GetValue()), cm = BOOLSTR(m_ChromaMetric->GetValue()), f = "decimate=";

    if (cycle != DEFAULT_CYCLE) f += wxString::Format("cycle=%u:", cycle);
    if (dthresh != DEFAULT_DUPTHRESH) f += wxString::Format("dupthresh=%g:", dthresh);
    if (scthresh != DEFAULT_SCTHRESH) f += wxString::Format("scthresh=%u:", scthresh);
    if (bx != DEFAULT_BLOCKXY) f += wxString::Format("blockx=%u:", bx);
    if (by != DEFAULT_BLOCKXY) f += wxString::Format("blocky=%u:", by);
    if (m_PreProc->GetValue() != DEFAULT_PREPROC) f += wxString::Format("ppsrc=%s:", pp);
    if (m_ChromaMetric->GetValue() != DEFAULT_CHROMAMETRIC) f += wxString::Format("chroma=%s:", cm);

    f.Remove(f.Len() - 1, 1);

    fltr->friendly = FFQSF(SID_DECIMATE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               cycle, dthresh, scthresh, bx, by, pp, cm);
    fltr->ff_filter.Printf("%s%s%s", FILTER_VIDEO_IN, f, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%u,%g,%u,%u,%u,%s,%s", cycle, dthresh, scthresh, bx, by, pp, cm);

    return true;

}
