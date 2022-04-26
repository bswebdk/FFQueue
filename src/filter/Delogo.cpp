/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Delogo.cpp                                                      *
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

#include "Delogo.h"
#include "../utils/FFQBuildCmd.h"
#include <wx/statline.h>

Delogo::Delogo(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(1, 10000);

    wxFlexGridSizer *fgsx, *fgsxx;

    //Create sizer with delogo controls
    #ifdef MAKE_BAND_CTRLS
    #define CTRL_ROWS 3
    #else
    #define CTRL_ROWS 2
    #endif // MAKE_BLUR_CTRLS

    m_DelogoCtrls = new wxFlexGridSizer(CTRL_ROWS, 2, 0, 0); //Rows, Cols, HGap, VGap
    m_DelogoCtrls->AddGrowableCol(1);

    //Position row
    MakeLabel(FFQS(SID_DELOGO_POSITION), m_DelogoCtrls);
    m_DelogoCtrls->Add(GetLeftAndTopControls(iVal), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Dimensions row
    MakeLabel(FFQS(SID_DELOGO_SIZE), m_DelogoCtrls);
    m_DelogoCtrls->Add(GetWidthAndHeightControls(iVal), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Blurry band row
    #ifdef MAKE_BAND_CTRLS
    wxStaticText *st = new wxStaticText(this, wxID_ANY, FFQS(SID_DELOGO_BLUR_EDGE), wxDefaultPosition, wxDefaultSize, 0);
    m_DelogoCtrls->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        fgsx = new wxFlexGridSizer(1, 4, 0, 0);
        fgsx->AddGrowableCol(0);
        fgsx->AddGrowableCol(2);

        MakeLabel(FFQS(SID_FILTER_POSITION_WIDTH), fgsx, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);

        m_Band = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
        m_Band->SetValidator(iVal);
        fgsx->Add(m_Band, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        fgsx->Add(40, -1, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
        fgsx->Add(80, -1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    m_DelogoCtrls->Add(fgsx, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    #endif

    //Create outer sizer
    fgsx = new wxFlexGridSizer(6, 1, 0, 0);
    fgsx->AddGrowableCol(0); //Just in case...

    //Add blur sizer
    fgsx->Add(m_DelogoCtrls, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);


    //Separating line
    fgsx->Add(new wxStaticLine(this), 1, wxTOP|wxBOTTOM|wxEXPAND, 5);


    //Image mask controls
    m_UseImgMask = new wxCheckBox(this, wxID_ANY, FFQS(SID_DELOGO_USE_IMG_MASK));
    m_UseImgMask->Bind(wxEVT_CHECKBOX, &Delogo::OnAction, this, wxID_ANY);
    fgsx->Add(m_UseImgMask, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);


    fgsxx = new wxFlexGridSizer(1, 2, 0, 0);
    fgsxx->AddGrowableCol(0);
    m_ImgMask = new wxTextCtrl(this, wxID_ANY);
    fgsxx->Add(m_ImgMask, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    m_BrowseImg = new wxButton(this, wxID_ANY, "...", wxDefaultPosition, wxSize(-1, -1));
    m_BrowseImg->Bind(wxEVT_BUTTON, &Delogo::OnAction, this, wxID_ANY);
    fgsxx->Add(m_BrowseImg, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    fgsx->Add(fgsxx, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);

    //Separating line
    fgsx->Add(new wxStaticLine(this), 1, wxTOP|wxBOTTOM|wxEXPAND, 5);

    fgsx->Add(GetTimeLimitControls(true), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgsx);
    fgsx->Fit(this);
    fgsx->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Delogo::~Delogo()
{

}

//---------------------------------------------------------------------------------------

void Delogo::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable, band = "10";

    if (fs.Len() > 0)
    {

        //Load values from filter string
        SetLeftAndTopValues(fs);
        SetWidthAndHeightValues(fs);
        band = GetToken(fs, ",");

    }

    else
    {

        //Default values
        SetLeftAndTopValues(1, 1);
        SetWidthAndHeightValues(100, 100);

    }

    #ifdef MAKE_BAND_CTRLS
    m_Band->SetValue(band);
    #endif

    SetTimeLimitValues(fs, true);

    m_UseImgMask->SetValue(STRBOOL(GetToken(fs, ",")));
    m_ImgMask->SetValue(fs);

    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool Delogo::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftDELOGO;

    wxString mask = m_ImgMask->GetValue();

    int left = 0, top = 0, width = 0, height = 0;
    GetLeftAndTopValues(left, top);
    GetWidthAndHeightValues(width, height);

    //The delogo filter will fail if 0,0 is used as top left
    if (left < 1) left = 1;
    if (top < 1) top = 1;

    //Validate controls and return false if invalid
    if (m_UseImgMask->GetValue())
    {
        if (!wxFileExists(mask)) return ShowError(m_ImgMask, FFQS(SID_DELOGO_IMG_MASK_ERROR));
    }
    else if ((width <= 0) || (height <= 0)) return ShowError(FFQS(SID_DELOGO_SIZE_ERROR));


    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2)) return false;
    wxString timeLimit, tf, band = "";
    GetTimeLimitFilter(timeLimit, true);
    GetTimeLimitFriendly(tf);

    #ifdef MAKE_BAND_CTRLS
    band = StrTrim(m_Band->GetValue());
    #endif // MAKE_BAND_CTRLS

    if (m_UseImgMask->GetValue())
    {

        fltr->friendly.Printf("%s=%s", FFQL()->FILTER_NAMES[fltr->type], mask);
        fltr->ff_filter.Printf("%sremovelogo=f=%s%s%s", FILTER_VIDEO_IN, EscapeFilterString(mask, false), timeLimit, FILTER_VIDEO_OUT);

    }

    else
    {

        fltr->friendly = FFQSF(SID_DELOGO_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], (unsigned int)left, (unsigned int)top, (unsigned int)width, (unsigned int)height, band, tf);
        fltr->ff_filter.Printf("%sdelogo=x=%i:y=%i:w=%i:h=%i%s%s%s",
                               FILTER_VIDEO_IN,
                               left, top, width, height,
                               band.Len() > 0 ? wxString::Format(":band=%s", band) : "",
                               timeLimit,
                               FILTER_VIDEO_OUT);
    }

    fltr->editable.Printf("%i,%i,%i,%i,%s," + UINT64FMT + "," + UINT64FMT + ",%s,%s",
                          left, top, width, height,
                          band,//m_Blur->GetValue(),
                          t1, t2,
                          BOOLSTR(m_UseImgMask->GetValue()), mask);

    return true;

}

//---------------------------------------------------------------------------------------

void Delogo::OnAction(wxCommandEvent &event)
{

    if (event.GetEventObject() == m_BrowseImg)
    {

        //Browse for image mask
        wxString fn = m_ImgMask->GetValue();
        if (BrowseForFile(fn)) m_ImgMask->SetValue(fn);

    }

    else UpdateControls();

}

//---------------------------------------------------------------------------------------

void Delogo::UpdateControls()
{

    bool mask = m_UseImgMask->GetValue();
    EnableSizer(m_DelogoCtrls, !mask);
    m_ImgMask->Enable(mask);
    m_BrowseImg->Enable(mask);

}
