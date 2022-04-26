/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Deshake.cpp                                                     *
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

#include "Deshake.h"

const unsigned int FILL_EDGE_COUNT = 4;

//---------------------------------------------------------------------------------------

Deshake::Deshake(wxWindow* parent) : FilterBasePanel(parent)
{

    wxString s;
    wxIntegerValidator<unsigned int> iVal, blockVal, contrastVal;
    iVal.SetRange(0, 99999);
    blockVal.SetRange(0, 128);
    contrastVal.SetRange(0, 255);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(9, 2, 0, 0), *fgsx;
    fgs->AddGrowableCol(1);

    MakeLabel(FFQS(SID_DESHAKE_DETECT_FRAME_1), fgs);
    m_EntireFrame = new wxCheckBox(this, wxID_ANY, FFQS(SID_DESHAKE_DETECT_FRAME_2), wxDefaultPosition, wxDefaultSize, 0);
    fgs->Add(m_EntireFrame, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 6);

    m_PositionST = MakeLabel(FFQS(SID_DESHAKE_DETECT_POSITION), fgs);
    fgs->Add(GetLeftAndTopControls(iVal), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    m_SizeST = MakeLabel(FFQS(SID_DESHAKE_DETECT_SIZE), fgs);
    fgs->Add(GetWidthAndHeightControls(iVal), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_DESHAKE_MAX_SHAKE), fgs);

        fgsx = new wxFlexGridSizer(1, 4, 0, 0);
        fgsx->AddGrowableCol(0);
        fgsx->AddGrowableCol(2);

        wxString shake[] = {"16", "32", "48", "64"};

        MakeLabel(FFQS(SID_DESHAKE_HORZ), fgsx, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
        m_ShakeX = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, shake, 0);
        fgsx->Add(m_ShakeX, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

        MakeLabel(FFQS(SID_DESHAKE_VERT), fgsx, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL);
        m_ShakeY = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, shake, 0);
        fgsx->Add(m_ShakeY, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_DESHAKE_FILL_EDGE), fgs);
    s = FFQS(SID_DESHAKE_FILL_EDGE_VALUES);
    m_FillEdge = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    for (unsigned int i = 0; i < FILL_EDGE_COUNT; i++) m_FillEdge->Append(GetToken(s, "|"));
    fgs->Add(m_FillEdge, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 6);

    MakeLabel(FFQS(SID_DESHAKE_BLOCK_SIZE), fgs);
    m_BlockSize = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_BlockSize->SetValidator(blockVal);
    fgs->Add(m_BlockSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 6);

    MakeLabel(FFQS(SID_DESHAKE_MIN_CONTRAST), fgs);
    m_Contrast = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1));
    m_Contrast->SetValidator(contrastVal);
    fgs->Add(m_Contrast, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 6);

    MakeLabel(FFQS(SID_DESHAKE_SEARCH), fgs);
    s = FFQS(SID_DESHAKE_SEARCH_VALUES);

        fgsx = new wxFlexGridSizer(1, 2, 0, 0);
        m_Search0 = new wxRadioButton(this, wxID_ANY, GetToken(s, "|"), wxDefaultPosition, wxDefaultSize, 0);
        fgsx->Add(m_Search0, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
        m_Search1 = new wxRadioButton(this, wxID_ANY, GetToken(s, "|"), wxDefaultPosition, wxDefaultSize, 0);
        fgsx->Add(m_Search1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    fgs->Add(fgsx, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);

    MakeLabel(FFQS(SID_DESHAKE_OPENCL), fgs);
    m_OpenCL = new wxCheckBox(this, wxID_ANY, FFQS(SID_DESHAKE_USE_OPENCL), wxDefaultPosition, wxDefaultSize, 0);
    fgs->Add(m_OpenCL, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 6);

    SetSizer(fgs);
    fgs->Fit(this);
    fgs->SetSizeHints(this);

	Connect(wxID_ANY, wxEVT_CHECKBOX,(wxObjectEventFunction)&Deshake::OnCtrlChange);

}

//---------------------------------------------------------------------------------------

Deshake::~Deshake()
{

}

//---------------------------------------------------------------------------------------

void Deshake::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable, s;

    if (fs.Len() > 0)
    {

        SetLeftAndTopValues(fs);
        SetWidthAndHeightValues(fs);
        m_ShakeX->SetSelection(Str2Long(GetToken(fs, ',')));
        m_ShakeY->SetSelection(Str2Long(GetToken(fs, ',')));
        m_FillEdge->SetSelection(Str2Long(GetToken(fs, ',')));
        m_BlockSize->SetValue(GetToken(fs, ','));
        m_Contrast->SetValue(GetToken(fs, ','));
        s = GetToken(fs, ',');
        m_Search0->SetValue(s == "1");
        m_Search1->SetValue(s == "0");
        m_OpenCL->SetValue(GetToken(fs, ',') == STR_YES);
        m_EntireFrame->SetValue(GetToken(fs, ',') == STR_YES);

    }
    else
    {

        SetLeftAndTopValues(0, 0);
        SetWidthAndHeightValues(100, 100);
        m_ShakeX->SetSelection(0);
        m_ShakeY->SetSelection(0);
        m_FillEdge->SetSelection(3);
        m_BlockSize->SetValue("8");
        m_Contrast->SetValue("125");
        m_Search0->SetValue(false);
        m_Search1->SetValue(true);
        m_OpenCL->SetValue(false);
        m_EntireFrame->SetValue(false);

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool Deshake::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftDESHAKE;

    int left, top, width, height, rx, ry, edge, block, contrast;

    if (m_EntireFrame->GetValue())
    {
        left = 0; top = 0; width = 0; height = 0;
    }
    else
    {
        if (!GetWidthAndHeightValues(width, height, true)) return false;
        GetLeftAndTopValues(left, top);
    }

    rx = Str2Long(m_ShakeX->GetString(m_ShakeX->GetSelection()));
    ry = Str2Long(m_ShakeY->GetString(m_ShakeY->GetSelection()));
    edge = m_FillEdge->GetSelection();
    block = Str2Long(m_BlockSize->GetValue());
    contrast = Str2Long(m_Contrast->GetValue());

    if (block < 4) return ShowError(m_BlockSize, FFQS(SID_DESHAKE_BLOCK_SIZE_ERROR));

    if (contrast < 1) return ShowError(m_Contrast, FFQS(SID_DESHAKE_MIN_CONTRAST_ERROR));

    wxString box_friendly, box_filter, s;

    if (left+top+width+height == 0)
    {
        box_friendly = FFQS(SID_DESHAKE_DETECT_ENTIRE_FRAME);
        box_filter = "x=-1";
    }
    else
    {
        box_friendly.Printf("%i,%i %ix%i", left, top, width, height);
        box_filter.Printf("x=%i:y=%i:w=%i:h=%i", left, top, width, height);
    }

    int simple = m_Search0->GetValue() ? 1 : 0, opencl = m_OpenCL->GetValue() ? 1 : 0;

    s = FFQS(SID_DESHAKE_FILL_EDGE_FRIENDLY);
    wxString fill[FILL_EDGE_COUNT];
    for (unsigned int i = 0; i < FILL_EDGE_COUNT; i++) fill[i] = GetToken(s, "|");

    fltr->friendly = FFQSF(SID_DESHAKE_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type],
                           box_friendly, rx, ry, fill[edge], block, contrast, simple==1 ? m_Search0->GetLabel().Lower() : m_Search1->GetLabel().Lower(),
                           BOOLSTR(m_OpenCL->GetValue()));

    fltr->ff_filter.Printf("%sdeshake=%s:rx=%i:ry=%i:edge=%i:blocksize=%i:contrast=%i:search=%i:opencl=%i%s",
                           FILTER_VIDEO_IN, box_filter, rx, ry, edge, block, contrast, simple, opencl, FILTER_VIDEO_OUT);

    GetLeftAndTopValues(left, top);
    GetWidthAndHeightValues(width, height);
    fltr->editable.Printf("%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%s",
                            left, top, width, height, m_ShakeX->GetSelection(), m_ShakeY->GetSelection(),
                            edge, block, contrast, simple, opencl, BOOLSTR(m_EntireFrame->GetValue()));

    return true;

}

//---------------------------------------------------------------------------------------

void Deshake::OnCtrlChange(wxCommandEvent &event)
{
    event.Skip();
    UpdateControls();
}

//---------------------------------------------------------------------------------------

void Deshake::UpdateControls()
{
    bool enable = !m_EntireFrame->GetValue();
    m_PositionST->Enable(enable);
    m_SizeST->Enable(enable);
    EnableLeftAndTop(enable);
    EnableWidthAndHeight(enable);
}
