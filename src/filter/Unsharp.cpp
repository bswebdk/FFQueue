/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Unsharp.cpp                                                     *
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

#include "Unsharp.h"

Unsharp::Unsharp(wxWindow* parent) : FilterBasePanel(parent)
{

    m_Labels = FFQL()->GetStringArray(SID_UNSHARP_GROUP_LABELS, 10);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(4, 1, 0, 0), *fgsx;
    fgs->AddGrowableCol(0);

    MakeSizeGroup(fgs, FFQS(SID_UNSHARP_SHARPEN_AMOUNT), &m_LumaP, &m_ChromaP, &m_PowerLock, true);

    MakeSizeGroup(fgs, FFQS(SID_UNSHARP_LUMA_SIZE), &m_LumaX, &m_LumaY, &m_LumaLock, false);
    MakeSizeGroup(fgs, FFQS(SID_UNSHARP_CHROMA_SIZE), &m_ChromaX, &m_ChromaY, &m_ChromaLock, false);

    fgsx = new wxFlexGridSizer(1, 2, 0, 0);
    MakeLabel(FFQS(SID_FILTER_THIS), fgsx);
    fgsx->Add(GetTimeLimitControls(), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);

    fgs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    SetSizer(fgs);
    fgs->Fit(this);
    //fgs->SetSizeHints(this);

	Connect(wxID_ANY, wxEVT_SLIDER, (wxObjectEventFunction)&Unsharp::OnCtrlChange);
	Connect(wxID_ANY, wxEVT_CHECKBOX, (wxObjectEventFunction)&Unsharp::OnCtrlChange);

}

//---------------------------------------------------------------------------------------

Unsharp::~Unsharp()
{
    delete[] m_Labels;
}

//---------------------------------------------------------------------------------------

void Unsharp::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_LumaX->SetValue(Str2Long(GetToken(fs, ',')));
        m_LumaY->SetValue(Str2Long(GetToken(fs, ',')));
        m_LumaP->SetValue(Str2Long(GetToken(fs, ',')));
        m_ChromaX->SetValue(Str2Long(GetToken(fs, ',')));
        m_ChromaY->SetValue(Str2Long(GetToken(fs, ',')));
        m_ChromaP->SetValue(Str2Long(GetToken(fs, ',')));
        m_LumaLock->SetValue(GetToken(fs, ',') == STR_YES);
        m_ChromaLock->SetValue(GetToken(fs, ',') == STR_YES);
        m_PowerLock->SetValue(GetToken(fs, ',') == STR_YES);

    }

    else
    {

        //Default values
        m_LumaX->SetValue(1);
        m_LumaY->SetValue(1);
        m_LumaP->SetValue(50);
        m_ChromaX->SetValue(1);
        m_ChromaY->SetValue(1);
        m_ChromaP->SetValue(50);
        m_LumaLock->SetValue(true);
        m_ChromaLock->SetValue(true);
        m_PowerLock->SetValue(false);

    }

    SetTimeLimitValues(fs, true);

}

//---------------------------------------------------------------------------------------

bool Unsharp::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftUNSHARP;

    double lp = (m_LumaP->GetValue() - 100) / 25.0,
           cp = (m_ChromaP->GetValue() - 100) / 25.0;

    if ((lp == 0) && (cp == 0)) return ShowError(m_LumaP, FFQS(SID_UNSHARP_SHARPEN_ERROR));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2, true)) return false;
    wxString ft, tfltr;
    GetTimeLimitFriendly(ft);
    GetTimeLimitFilter(tfltr, true);

    int lx = m_LumaX->GetValue(), ly = m_LumaY->GetValue(),
        cx = m_ChromaX->GetValue(), cy = m_ChromaY->GetValue();

    fltr->friendly = FFQSF(SID_UNSHARP_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               lp, cp, 3+(lx*2), 3+(ly*2), 3+(cx*2), 3+(cy*2), ft);

    fltr->ff_filter.Printf("%sunsharp=%i:%i:%f:%i:%i:%f%s%s", FILTER_VIDEO_IN, 3+(lx*2), 3+(ly*2), lp, 3+(cx*2), 3+(cy*2), cp, tfltr, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%i,%i,%i,%i,%i,%i,%s,%s,%s," + UINT64FMT + "," + UINT64FMT,
                            lx, ly, m_LumaP->GetValue(), cx, cy, m_ChromaP->GetValue(),
                            BOOLSTR(m_LumaLock->GetValue()),
                            BOOLSTR(m_ChromaLock->GetValue()),
                            BOOLSTR(m_PowerLock->GetValue()),
                            t1, t2);

    return true;

}

//---------------------------------------------------------------------------------------

void adjust(wxSlider* s1, wxSlider* s2, int v)
{
    s1->SetValue(v);
    s2->SetValue(v);
}

//---------------------------------------------------------------------------------------

void Unsharp::OnCtrlChange(wxCommandEvent &event)
{

    event.Skip();

    bool ll = m_LumaLock->GetValue(),
         cl = m_ChromaLock->GetValue(),
         pl = m_PowerLock->GetValue();

    wxObject *obj = event.GetEventObject();

    if ((obj == m_PowerLock) && pl) adjust(m_LumaP, m_ChromaP, m_LumaP->GetValue());

    else if ((obj == m_LumaLock) && ll) adjust(m_LumaX, m_LumaY, m_LumaX->GetValue());

    else if ((obj == m_ChromaLock) && cl) adjust(m_ChromaX, m_ChromaY, m_ChromaX->GetValue());

    else if (ll && ((obj == m_LumaX) || (obj == m_LumaY))) adjust(m_LumaX, m_LumaY, ((wxSlider*)obj)->GetValue());

    else if (cl && ((obj == m_ChromaX) || (obj == m_ChromaY))) adjust(m_ChromaX, m_ChromaY, ((wxSlider*)obj)->GetValue());

    else if (pl && ((obj == m_LumaP) || (obj == m_ChromaP))) adjust(m_LumaP, m_ChromaP, ((wxSlider*)obj)->GetValue());

}

//---------------------------------------------------------------------------------------

void Unsharp::MakeSizeGroup(wxSizer *sizer, wxString caption, wxSlider **sX, wxSlider **sY, wxCheckBox **cL, bool power)
{

    wxStaticBoxSizer *sbs = new wxStaticBoxSizer(wxVERTICAL, this, caption);
    wxFlexGridSizer *fgs = new wxFlexGridSizer(4, 2, 0, 0),
                     *fgsx = new wxFlexGridSizer(1, 3, 0, 0);

    fgs->AddGrowableCol(0);
    fgsx->AddGrowableCol(1);

    //Softer|Sharper|Smaller|Larger|None|Luma|Chroma|X|Y|Lock values

    MakeLabel(power ? m_Labels[0] : m_Labels[2], fgsx, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
    MakeLabel(power ? m_Labels[4] : _T(""), fgsx, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
    MakeLabel(power ? m_Labels[1] : m_Labels[3], fgsx, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);

    fgs->Add(50, -1, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);
    fgs->Add(fgsx, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);

    MakeLabel(power ? m_Labels[5] : m_Labels[7], fgs);
    *sX = new wxSlider(this, wxID_ANY, 0, 0, power ? 200 : 30, wxDefaultPosition, wxSize(400, -1));
    fgs->Add(*sX, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);

    MakeLabel(power ? m_Labels[6] : m_Labels[8], fgs);
    *sY = new wxSlider(this, wxID_ANY, 0, 0, power ? 200 : 30, wxDefaultPosition, wxSize(400, -1));
    fgs->Add(*sY, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);

    fgs->Add(-1, -1, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);
    fgsx = new wxFlexGridSizer(1, 2, 0, 0);
    fgsx->Add(10, -1, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);
    *cL = new wxCheckBox(this, wxID_ANY, m_Labels[9]);
    fgsx->Add(*cL, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    fgs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);

    sbs->Add(fgs, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);

    sizer->Add(sbs, 1, wxALL|wxEXPAND/*|wxALIGN_CENTER_VERTICAL*/, 0);

}
