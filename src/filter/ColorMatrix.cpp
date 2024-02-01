/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: ColorMatrix.cpp                                                 *
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

#include "ColorMatrix.h"

const unsigned int COLOR_MATRIX_COUNT = 4;
const wxString COLOR_MATRICES_FRIENDLY[] = {"BT.601", "BT.709", "FCC", "SMPTE-240M" };
const wxString COLOR_MATRICES_ACTUAL[] = {"bt601", "bt709", "fcc", "smpte240m"};

//---------------------------------------------------------------------------------------

ColorMatrix::ColorMatrix(wxWindow* parent) : FilterBasePanel(parent)
{

    wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 4, 0, 0);

    MakeLabel(FFQS(SID_CLRMATRIX_SRC_MATRIX), fgs);
    m_Src = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, COLOR_MATRIX_COUNT, COLOR_MATRICES_FRIENDLY);
    fgs->Add(m_Src, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel("  " + FFQS(SID_CLRMATRIX_DST_MATRIX), fgs);
    m_Dst = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, COLOR_MATRIX_COUNT, COLOR_MATRICES_FRIENDLY);
    fgs->Add(m_Dst, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    SetSizer(fgs);
    fgs->Fit(this);
    //fgs->SetSizeHints(this);

}

//---------------------------------------------------------------------------------------

ColorMatrix::~ColorMatrix()
{

}

//---------------------------------------------------------------------------------------

void ColorMatrix::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0)
    {

        //Load values from filter string
        m_Src->SetSelection(Str2Long(GetToken(fs, ','), 0));
        m_Dst->SetSelection(Str2Long(GetToken(fs, ','), 1));

    }

    else
    {

        //Default values
        m_Src->SetSelection(0);
        m_Dst->SetSelection(1);

    }

}

//---------------------------------------------------------------------------------------

bool ColorMatrix::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftCOLORMATRIX;

    unsigned int s = m_Src->GetSelection(), d = m_Dst->GetSelection();

    if (s == d) return ShowError(FFQS(SID_CLRMATRIX_ERROR));

    fltr->friendly = FFQSF(SID_CLRMATRIX_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], m_Src->GetStringSelection(), m_Dst->GetStringSelection());
    fltr->ff_filter.Printf("%scolormatrix=%s:%s%s", FILTER_VIDEO_IN, COLOR_MATRICES_ACTUAL[s], COLOR_MATRICES_ACTUAL[d], FILTER_VIDEO_OUT);
    fltr->editable.Printf("%u,%u", s, d);

    return true;

}
