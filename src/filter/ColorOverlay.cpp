/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: ColorOverlay.cpp                                                *
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

#include "ColorOverlay.h"

ColorOverlay::ColorOverlay(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(0, 10000);

    wxFlexGridSizer *fgs1, *fgs2;
    wxStaticText *st;

    fgs1 = new wxFlexGridSizer(5, 2, 0, 0); //Rows, Cols, HGap, VGap
    fgs1->AddGrowableCol(1);

    //Color picker row
    MakeLabel(FFQS(SID_COLOR_PICK_COLOR), fgs1);
    //st = new wxStaticText(this, wxID_ANY, "Pick a color", wxDefaultPosition, wxDefaultSize, 0);
    //fgs1->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    m_Color = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxSize(250,-1), 0);
    fgs1->Add(m_Color, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    //fgs1->Add(m_Color, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    //Visibility row
    MakeLabel(FFQS(SID_COLOR_VISIBILITY), fgs1);
    //st = new wxStaticText(this, wxID_ANY, "Visibility", wxDefaultPosition, wxDefaultSize, 0);
    //fgs1->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        fgs2 = new wxFlexGridSizer(1, 4, 0, 0);
        fgs2->AddGrowableCol(1);

        st = new wxStaticText(this, wxID_ANY, FFQS(SID_COLOR_VISIBILITY_NONE), wxDefaultPosition, wxDefaultSize, 0);
        fgs2->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        m_Visibility = new wxSlider(this, 1000, 255, 0, 255, wxDefaultPosition, wxDefaultSize);
        fgs2->Add(m_Visibility, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

        st = new wxStaticText(this, wxID_ANY, FFQS(SID_COLOR_VISIBILITY_OPAQUE), wxDefaultPosition, wxDefaultSize, 0);
        fgs2->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        m_SliderText = new wxStaticText(this, wxID_ANY, "100.0%", wxDefaultPosition, wxDefaultSize, 0);
        fgs2->Add(m_SliderText, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Position row
    MakeLabel(FFQS(SID_COLOR_POSITION), fgs1);
    fgs1->Add(GetLeftAndTopControls(iVal), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Dimensions row
    MakeLabel(FFQS(SID_COLOR_SIZE), fgs1);
    fgs1->Add(GetWidthAndHeightControls(iVal), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Time limit
    MakeLabel(FFQS(SID_FILTER_TIME_LIMIT), fgs1);
    //st = new wxStaticText(this, wxID_ANY, "Time limit", wxDefaultPosition, wxDefaultSize, 0);
    //fgs1->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    fgs1->Add(GetTimeLimitControls(), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs1);
    fgs1->Fit(this);
    fgs1->SetSizeHints(this);

    Connect(1000, wxEVT_SLIDER, (wxObjectEventFunction)&ColorOverlay::OnSliderChange);

}

//---------------------------------------------------------------------------------------

ColorOverlay::~ColorOverlay()
{

}

//---------------------------------------------------------------------------------------

void ColorOverlay::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Color->SetColour(wxColour(GetToken(fs, ',')));
        m_Visibility->SetValue(Str2Long(GetToken(fs, ',')));
        SetLeftAndTopValues(fs);
        SetWidthAndHeightValues(fs);

    }

    else
    {

        //Default values
        m_Color->SetColour(*wxBLACK);
        m_Visibility->SetValue(100);
        SetLeftAndTopValues(0, 0);
        SetWidthAndHeightValues(100, 100);

    }

    SetTimeLimitValues(fs, true);

    wxCommandEvent dummy;
    OnSliderChange(dummy);

}

//---------------------------------------------------------------------------------------

bool ColorOverlay::GetFilter(LPFFMPEG_FILTER fltr)
{

    int left = 0, top = 0, width = 0, height = 0;
    GetLeftAndTopValues(left, top);
    GetWidthAndHeightValues(width, height);

    //Validate controls and return false if invalid
    if ((width <= 0) || (height <= 0)) return ShowError(FFQS(SID_COLOR_SIZE_ERROR));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2, true)) return false;
    wxString timeLimit, tf;
    GetTimeLimitFilter(timeLimit, true);
    GetTimeLimitFriendly(tf);

    fltr->type = ftCOLOROVERLAY;

    wxString rgb = m_Color->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
    float alpha = m_Visibility->GetValue();
    if (alpha > 0) alpha = alpha / 255.0 * 100.0;

    fltr->friendly = FFQSF(SID_COLOR_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                               rgb, left, top, width, height, alpha, tf);

    wxString uid = NextFilterUID();
    fltr->ff_filter.Printf("color=%s@0x%X:%ux%u%s;%s%soverlay=%u:%u:shortest=1%s%s",
                              rgb,
                              m_Visibility->GetValue(),
                              width, height,
                              uid,
                              FILTER_VIDEO_IN,
                              uid,
                              left, top,
                              timeLimit,
                              FILTER_VIDEO_OUT);

    fltr->editable.Printf("%s,%u,%u,%u,%u,%u," + UINT64FMT + "," + UINT64FMT,
                            rgb,
                            m_Visibility->GetValue(),
                            left, top, width, height,
                            t1, t2);

    return true;

}

//---------------------------------------------------------------------------------------

void ColorOverlay::OnSliderChange(wxCommandEvent &event)
{
    event.Skip();
    m_SliderText->SetLabel(wxString::Format("%3.1f%%", m_Visibility->GetValue() / 255.0 * 100.0));
}

