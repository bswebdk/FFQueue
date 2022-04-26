/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Pad.cpp                                                         *
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

#include "Pad.h"
#include "../utils/FFQConfig.h"

Pad::Pad(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(0, 99999);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(3, 2, 0, 0);
    fgs->AddGrowableCol(1);

    m_UseColor = new wxRadioButton(this, wxID_ANY, FFQS(SID_PAD_COLOR));
    m_UseColor->SetValue(true);
    fgs->Add(m_UseColor, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    //MakeLabel(FFQS(SID_PAD_COLOR), fgs);
    m_Color = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, 0);
    fgs->Add(m_Color, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    m_UseBlur = new wxRadioButton(this, wxID_ANY, FFQS(SID_PAD_BLUR));
    m_UseBlur->SetValue(false);
    fgs->Add(m_UseBlur, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    m_BlurSizer = new wxFlexGridSizer(1, 3, 0, 0);
        m_BlurSizer->AddGrowableCol(1);
        MakeLabel("-", m_BlurSizer);
        m_Blur = new wxSlider(this, wxID_ANY, 2, 1, 10, wxDefaultPosition, wxSize(-1, -1));
        m_BlurSizer->Add(m_Blur, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
        MakeLabel("+", m_BlurSizer);
    fgs->Add(m_BlurSizer, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_PAD_SIZE), fgs);
    fgs->Add(GetWidthAndHeightControls(iVal), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);


    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

    Connect(wxID_ANY, wxEVT_RADIOBUTTON, (wxObjectEventFunction)&Pad::OnRadioClick);
    m_BlurOK = FFQCFG()->AreFiltersAvailable("split,scale,boxblur,overlay");
    m_UseBlur->Enable(m_BlurOK);

}

//---------------------------------------------------------------------------------------

Pad::~Pad()
{

}

//---------------------------------------------------------------------------------------

void Pad::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Color->SetColour(GetToken(fs, ','));
        SetWidthAndHeightValues(fs);

    }

    else
    {

        //Default values
        m_Color->SetColour(*wxBLACK);
        SetWidthAndHeightValues(800, 600);

    }

    bool blur = m_BlurOK && (!STRBOOLDEF(GetToken(fs, ","), m_UseColor->GetValue()));

    m_UseColor->SetValue(!blur);
    m_UseBlur->SetValue(blur);
    m_Blur->SetValue(Str2Long(GetToken(fs, ","), m_Blur->GetValue()));

    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool Pad::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftPAD;

    int w, h;
    if (!GetWidthAndHeightValues(w, h, true)) return false;
    wxString c = m_UseBlur->GetValue() ? FFQSF(SID_PAD_USERFRIENDLY_BLUR, m_Blur->GetValue()) : m_Color->GetColour().GetAsString(wxC2S_HTML_SYNTAX);

    fltr->friendly = FFQSF(SID_PAD_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], w, h, c);
    if (m_UseBlur->GetValue())
    {
        wxString fid1 = wxString::Format(FILTER_UNIQUE_ID, 1),
                 fid2 = wxString::Format(FILTER_UNIQUE_ID, 2),
                 fid3 = wxString::Format(FILTER_UNIQUE_ID, 3);
        int blur = m_Blur->GetValue();
        fltr->ff_filter.Printf("%ssplit%s%s;%sscale=%i:%i,boxblur=%i:%i%s;%s%soverlay=(W-w)/2:(H-h)/2%s",
                               FILTER_VIDEO_IN, fid1, fid2, //Split
                               fid1, w, h, blur*2, blur, fid3, //Scale & blur
                               fid3, fid2, FILTER_VIDEO_OUT); //Overlay
    }
    else fltr->ff_filter.Printf("%spad=%i:%i:(ow-iw)/2:(oh-ih)/2:%s%s", FILTER_VIDEO_IN, w, h, c, FILTER_VIDEO_OUT);
    fltr->editable.Printf("%s,%i,%i,%s,%i", c, w, h, BOOLSTR(m_UseColor->GetValue()), m_Blur->GetValue());

    return true;

}

//---------------------------------------------------------------------------------------

void Pad::OnRadioClick(wxCommandEvent &event)
{

    event.Skip();
    UpdateControls();

}

//---------------------------------------------------------------------------------------

void Pad::UpdateControls()
{

    m_Color->Enable(m_UseColor->GetValue());
    EnableSizer(m_BlurSizer, m_UseBlur->GetValue());

}
