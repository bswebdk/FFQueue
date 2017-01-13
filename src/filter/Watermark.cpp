/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Watermark.cpp                                                   *
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

#include "Watermark.h"

Watermark::Watermark(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(0, 10000);

    wxFlexGridSizer *fgs1, *fgs2;

    //Main sizer
    fgs1 = new wxFlexGridSizer(7, 2, 0, 0); //Rows, Cols, HGap, VGap
    fgs1->AddGrowableCol(1);

    //Overlay file
    MakeLabel(FFQS(SID_WATERMARK_FILE), fgs1);

        //File & browse button
        fgs2 = new wxFlexGridSizer(1, 2, 0, 0); //Rows, Cols, HGap, VGap
        fgs2->AddGrowableCol(1);
        m_File = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(300, -1), 0);
        fgs2->Add(m_File, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
        m_Browse = new wxButton(this, 1000, _T("..."), wxDefaultPosition, wxSize(30, 21), 0);
        fgs2->Add(m_Browse, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Overlay position
    MakeLabel(FFQS(SID_WATERMARK_POSITION), fgs1);
    fgs1->Add(GetLeftAndTopControls(iVal), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    //Overlay size
    MakeLabel(FFQS(SID_WATERMARK_SCALE_TO), fgs1);
    fgs1->Add(GetWidthAndHeightControls(iVal), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    //Scale info
    fgs1->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    MakeLabel(FFQS(SID_WATERMARK_ASPECT_INFO), fgs1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL)->Enable(false);

    //Fade watermark
    MakeLabel(FFQS(SID_WATERMARK_FADE_IN_OUT), fgs1);

        //Fade slider
        fgs2 = new wxFlexGridSizer(1, 3, 0, 0);
        fgs2->AddGrowableCol(1);
        MakeLabel(FFQS(SID_WATERMARK_FADE_INSTANT), fgs2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
        m_Fade = new wxSlider(this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize);
        fgs2->Add(m_Fade, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
        MakeLabel(FFQS(SID_WATERMARK_FADE_SLOW), fgs2);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    //Fade info
    fgs1->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    MakeLabel(FFQS(SID_WATERMARK_ALPHA_CHANNEL_INFO), fgs1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL)->Enable(false);

    //Limit time
    MakeLabel(FFQS(SID_FILTER_TIME_LIMIT), fgs1);
    fgs1->Add(GetTimeLimitControls(), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

	Connect(1000,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Watermark::OnButtonClick);

    SetSizer(fgs1);
    fgs1->Fit(this);
    fgs1->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Watermark::~Watermark()
{

}

//---------------------------------------------------------------------------------------

void Watermark::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        SetLeftAndTopValues(fs);
        SetWidthAndHeightValues(fs);
        m_Fade->SetValue(Str2Long(GetToken(fs, ',')));
        SetTimeLimitValues(fs, true);

        //Set required file
        m_File->SetValue(fltr->required_file.path);

    }

    else
    {

        //Default values
        SetLeftAndTopValues(0, 0);
        SetWidthAndHeightValues(0, 0);
        m_Fade->SetValue(15);
        SetTimeLimitValues(fs);
        m_File->SetValue("");

    }

}

//---------------------------------------------------------------------------------------

bool Watermark::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftWATERMARK;

    wxString file = m_File->GetValue();
    if (!wxFileExists(file)) return ShowError(m_File, FFQS(SID_WATERMARK_FILE_ERROR));

    uint64_t t1, t2;
    if (!GetTimeLimitValues(t1, t2, true)) return false;

    int left, top, width, height;
    GetLeftAndTopValues(left, top);
    GetWidthAndHeightValues(width, height);

    wxString uf_scale = "none", ff_scale = "", reqIn = FILTER_REQUIRED_IN, uid, tf;

    if (width + height > 0)
    {

        wxString sw = "auto", sh = sw;
        if (width > 0) sw.Printf("%i", width);
        if (height > 0) sh.Printf("%i", height);
        uf_scale.Printf("%sx%s", sw, sh);

        uid = NextFilterUID();
        ff_scale.Printf("%sscale=%i:%i%s;", reqIn, width==0 ? -1 : width, height==0 ? -1 : height, uid);
        reqIn = uid;

    }

    float fade = m_Fade->GetValue() / 10.0;

    GetTimeLimitFriendly(tf);

    fltr->friendly = FFQSF(SID_WATERMARK_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], (unsigned int)left, (unsigned int)top, uf_scale, fade, tf, file);

    fltr->ff_filter = ff_scale;

    if (fade > 0)
    {
        double d = t1;
        tf.Printf("%sfade=in:st=%g:d=%g:alpha=1", reqIn, d, fade);
        fltr->ff_filter += tf;
        if (t2 > 0)
        {
            d = t2 - fade;
            tf.Printf(",fade=out:st=%g:d=%g:alpha=1", d, fade);
            fltr->ff_filter += tf;
        }
        uid = NextFilterUID();
        fltr->ff_filter += uid + ";";
        reqIn = uid;
    }

    GetTimeLimitFilter(tf, true);
    tf.Printf("%s%soverlay=%i:%i:repeatlast=0%s%s", FILTER_VIDEO_IN, reqIn, left, top, tf, FILTER_VIDEO_OUT);

    fltr->ff_filter += tf;

    fltr->editable.Printf("%i,%i,%i,%i,%i," + UINT64FMT + "," + UINT64FMT + ",%s", left, top, width, height, m_Fade->GetValue(), t1, t2, file);

    //fltr->RequiredFile.Printf("v:%s", file);
    fltr->required_file.tag = "v";
    fltr->required_file.loop = "1";
    fltr->required_file.path = file;

    return true;

}

//---------------------------------------------------------------------------------------

void Watermark::OnButtonClick(wxCommandEvent &event)
{
    if (event.GetId() == 1000)
    {
        wxString file = m_File->GetValue();
        if (BrowseForFile(file)) m_File->SetValue(file);
    }
}
