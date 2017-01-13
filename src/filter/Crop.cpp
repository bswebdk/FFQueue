/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Crop.cpp                                                        *
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

#include "Crop.h"

Crop::Crop(wxWindow* parent) : FilterBasePanel(parent)
{

    wxIntegerValidator<unsigned int> iVal;
    iVal.SetRange(0, 10000);

    wxFlexGridSizer *fgs;

    fgs = new wxFlexGridSizer(5, 7, 0, 0); //Rows, Cols, HGap, VGap
    fgs->AddGrowableCol(0);
    fgs->AddGrowableCol(6);

	const int col = 25; //Centering columns width
	//Row 1 (top label)
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	MakeLabel(FFQS(SID_CROP_TOP), fgs, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer

	//Row 2 (top input)
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
    m_Top = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_Top->SetValidator(iVal);
    fgs->Add(m_Top, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer

    //Row 3 (left & right)
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	MakeLabel(FFQS(SID_CROP_LEFT), fgs, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
    m_Left = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_Left->SetValidator(iVal);
    fgs->Add(m_Left, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
    m_Right = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_Right->SetValidator(iVal);
    fgs->Add(m_Right, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
	MakeLabel(FFQS(SID_CROP_RIGHT), fgs, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer

    //Row 4 (bottom input)
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
    m_Bottom = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1));
    m_Bottom->SetValidator(iVal);
    fgs->Add(m_Bottom, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL, 3);
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer

	//Row 5 (bottom label)
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	MakeLabel(FFQS(SID_CROP_BOTTOM), fgs, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL);
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(-1, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer
	fgs->Add(col, -1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3); //Spacer

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

Crop::~Crop()
{

}

//---------------------------------------------------------------------------------------

void Crop::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Left->SetValue(GetToken(fs, ','));
        m_Top->SetValue(GetToken(fs, ','));
        m_Right->SetValue(GetToken(fs, ','));
        m_Bottom->SetValue(GetToken(fs, ','));

    }

    else
    {

        //Default values
        m_Left->SetValue("0");
        m_Top->SetValue("0");
        m_Right->SetValue("0");
        m_Bottom->SetValue("0");

    }

}

//---------------------------------------------------------------------------------------

bool Crop::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftCROP;

    unsigned int left = Str2Long(m_Left->GetValue()),
                 top = Str2Long(m_Top->GetValue()),
                 right = Str2Long(m_Right->GetValue()),
                 bottom = Str2Long(m_Bottom->GetValue()),
                 width = left + right,
                 height = top + bottom;

    if (left + top + right + bottom <= 0) return ShowError(FFQS(SID_CROP_ERROR));

    fltr->friendly = FFQSF(SID_CROP_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], left, top, right, bottom);

    wxString cw = "iw", ch = "ih";
    if (width > 0) cw << "-" << width;
    if (height > 0) ch << "-" << height;

    fltr->ff_filter.Printf("%scrop=%s:%s:%u:%u%s", FILTER_VIDEO_IN, cw, ch, left, top, FILTER_VIDEO_OUT);

    fltr->editable.Printf("%u,%u,%u,%u", left, top, right, bottom);

    return true;

}
