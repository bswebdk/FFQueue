/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPresetMgr.cpp                                                *
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

#include "FFQPresetMgr.h"
#include "utils/FFQLang.h"
#include "FFQMain.h"
#include "utils/FFQCompress.h"
#include "utils/FFQConfig.h"
#include "utils/FFQMisc.h"

#include <wx/clipbrd.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQPresetMgr)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQPresetMgr)
//*)

//(*IdInit(FFQPresetMgr)
const long FFQPresetMgr::ID_PRESETS = wxNewId();
const long FFQPresetMgr::ID_NEWBUTTON = wxNewId();
const long FFQPresetMgr::ID_EDITBUTTON = wxNewId();
const long FFQPresetMgr::ID_DELETEBUTTON = wxNewId();
const long FFQPresetMgr::ID_UPBUTTON = wxNewId();
const long FFQPresetMgr::ID_DOWNBUTTON = wxNewId();
const long FFQPresetMgr::ID_SORTBUTTON = wxNewId();
const long FFQPresetMgr::ID_COPYBUTTON = wxNewId();
const long FFQPresetMgr::ID_PASTEBUTTON = wxNewId();
const long FFQPresetMgr::ID_CLOSEBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQPresetMgr,wxDialog)
	//(*EventTable(FFQPresetMgr)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

FFQPresetMgr* FFQPresetMgr::Get()
{

    //Gets the current preset manager

    #ifdef DEBUG

        FFQConfig *cfg = FFQCFG();

        if (cfg)
        {

            wxWindow *mgr = cfg->GetPresetManager();
            if (mgr) return (FFQPresetMgr*)mgr;

            ShowError("Error: no preset manager");

        }

        ShowError("Error: no FFQConfig");

        return NULL;

    #else

        return (FFQPresetMgr*)FFQCFG()->GetPresetManager();

    #endif // DEBUG
}

//---------------------------------------------------------------------------------------

FFQPresetMgr::FFQPresetMgr(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(FFQPresetMgr)
	wxFlexGridSizer* FlexGridSizer1;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxBoxSizer* BoxSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer3 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	FlexGridSizer3->AddGrowableRow(0);
	Presets = new wxListBox(this, ID_PRESETS, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_EXTENDED|wxLB_HSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_PRESETS"));
	Presets->SetMinSize(wxSize(500,300));
	FlexGridSizer3->Add(Presets, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	NewButton = new wxButton(this, ID_NEWBUTTON, _T("+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_NEWBUTTON"));
	NewButton->SetLabel(FFQS(SID_COMMON_NEW));
	BoxSizer1->Add(NewButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	EditButton = new wxButton(this, ID_EDITBUTTON, _T("\?"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITBUTTON"));
	EditButton->SetLabel(FFQS(SID_COMMON_EDIT));
	BoxSizer1->Add(EditButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	DeleteButton = new wxButton(this, ID_DELETEBUTTON, _T("-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DELETEBUTTON"));
	DeleteButton->SetLabel(FFQS(SID_COMMON_DELETE));
	BoxSizer1->Add(DeleteButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(-1,-1,1, wxALL|wxEXPAND, 2);
	UpButton = new wxButton(this, ID_UPBUTTON, _T("^"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_UPBUTTON"));
	UpButton->SetLabel(FFQS(SID_COMMON_MOVE_UP));
	BoxSizer1->Add(UpButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	DownButton = new wxButton(this, ID_DOWNBUTTON, _T("v"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DOWNBUTTON"));
	DownButton->SetLabel(FFQS(SID_COMMON_MOVE_DOWN));
	BoxSizer1->Add(DownButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SortButton = new wxButton(this, ID_SORTBUTTON, _T("S"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SORTBUTTON"));
	SortButton->SetLabel(FFQS(SID_COMMON_SORT));
	BoxSizer1->Add(SortButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(-1,-1,1, wxALL|wxEXPAND, 2);
	CopyButton = new wxButton(this, ID_COPYBUTTON, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COPYBUTTON"));
	CopyButton->SetLabel(FFQS(SID_COMMON_COPY));
	BoxSizer1->Add(CopyButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	PasteButton = new wxButton(this, ID_PASTEBUTTON, _T("P"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_PASTEBUTTON"));
	PasteButton->SetLabel(FFQS(SID_COMMON_PASTE));
	BoxSizer1->Add(PasteButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer3->Add(BoxSizer1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	CloseButton = new wxButton(this, ID_CLOSEBUTTON, _T("X"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLOSEBUTTON"));
	CloseButton->SetDefault();
	CloseButton->SetMinSize(wxSize(200,-1));
	CloseButton->SetLabel(FFQS(SID_COMMON_CLOSE));
	BoxSizer2->Add(CloseButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_PRESETS,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&FFQPresetMgr::OnPresetsSelect);
	Connect(ID_PRESETS,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_NEWBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_EDITBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_DELETEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_UPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_DOWNBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_SORTBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_COPYBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_PASTEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	Connect(ID_CLOSEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQPresetMgr::OnButtonClick);
	//*)

	SetTitle(FFQS(SID_PRESET_MANAGER_TITLE));

	m_Editor = NULL;

}

//---------------------------------------------------------------------------------------

FFQPresetMgr::~FFQPresetMgr()
{
	//(*Destroy(FFQPresetMgr)
	//*)
	ClearPresets();
	if (m_Editor)
    {
        delete m_Editor;
        m_Editor = NULL;
    }
}

//---------------------------------------------------------------------------------------

unsigned int FFQPresetMgr::AddPreset(LPFFQ_PRESET pst, bool select)
{
    return SetListItem(-1, pst, select);
}

//---------------------------------------------------------------------------------------

void FFQPresetMgr::ClearPresets()
{
    Presets->Freeze();
    for (unsigned int i = 0; i < Presets->GetCount(); i++) delete (LPFFQ_PRESET)Presets->GetClientData(i);
    Presets->Clear();
    Presets->Thaw();
}

//---------------------------------------------------------------------------------------

void FFQPresetMgr::DeletePreset(LPFFQ_PRESET pst)
{
    DeletePreset(IndexOf(pst));
}

//---------------------------------------------------------------------------------------

void FFQPresetMgr::DeletePreset(unsigned int pstIndex)
{
    Presets->Freeze();
    delete (LPFFQ_PRESET)Presets->GetClientData(pstIndex);
    Presets->Delete(pstIndex);
    Presets->Thaw();
    m_Changes++;
}

//---------------------------------------------------------------------------------------

bool FFQPresetMgr::EditPreset(LPFFQ_PRESET pst)
{

    bool ok = false;

    #ifdef DEBUG

        try {

    #endif // DEBUG

    //Create editor if needed
    if (m_Editor == NULL) m_Editor = new FFQPresetEdit(this);

    #ifndef __WIN32__
    //Select the correct parent to use when showing the editor
    if (IsShown()) m_Editor->Reparent(this);
    else m_Editor->Reparent(GetParent());
    #endif

    //Execute the editor
    ok = m_Editor->Execute(pst);

    if ((!IsShown()) && ok)
    {

        //If preset was successfully edited and the preset manager is not visible
        //the associated list item must be updated here
        SetListItem(IndexOf(pst), pst);

        //And then save options
        FFQCFG()->SaveConfig();

    }

    #ifdef DEBUG

        }
        catch (std::exception &err)
        {
            wxString s = err.what();
            ShowError("Edit preset error: " + s);
        }

    #endif // DEBUG

    return ok;

}

//---------------------------------------------------------------------------------------

bool FFQPresetMgr::Execute(wxChoice* forChoice)
{

    m_Changes = 0;
    m_LastMod = NULL;

    Presets->DeselectAll();
    LPFFQ_PRESET pst = NULL;

    if ((forChoice != NULL) && forChoice->HasClientData()) try {
        pst = (LPFFQ_PRESET)forChoice->GetClientData(forChoice->GetSelection());
        Presets->SetSelection(IndexOf(pst));
    } catch (...) {}

    UpdateControls();
    CenterOnParent();
    ShowModal();

    if (m_Changes > 0)
    {

        FFQCFG()->SaveConfig();

        if (forChoice != NULL)
        {

            LPFFQ_PRESET lmod = GetLastModified();
            if (lmod != NULL) pst = lmod;
            UNIQUE_ID pid = (pst == NULL) ? UNIQUE_ID() : pst->preset_id;
            FillChoice(forChoice, pid.ToString());

        }

        return true;

    }

    return false;

}

//---------------------------------------------------------------------------------------

void FFQPresetMgr::FillChoice(wxChoice* choice, wxString selectId)
{
    choice->Freeze();
    choice->Clear();
    unsigned int sel = 0;
    for (unsigned int i = 0; i < Presets->GetCount(); i++)
    {
        LPFFQ_PRESET pst = (LPFFQ_PRESET)Presets->GetClientData(i);
        if (selectId == pst->preset_id.ToString()) sel = choice->GetCount();
        choice->Append(Presets->GetString(i), pst);
    }
    choice->SetSelection(sel);
    choice->Thaw();
}

//---------------------------------------------------------------------------------------

long FFQPresetMgr::FindPreset(wxString name, bool matchCase)
{
    if (!matchCase) name.LowerCase();
    for (unsigned int i = 0; i < Presets->GetCount(); i++)
    {
        LPFFQ_PRESET pst = (LPFFQ_PRESET)Presets->GetClientData(i);
        if (matchCase && (pst->preset_name == name)) return i;
        else if ((!matchCase) && (pst->preset_name.Lower() == name)) return i;
    }
    return -1;
}

//---------------------------------------------------------------------------------------

LPFFQ_PRESET FFQPresetMgr::GetLastModified()
{
    if (IndexOf(m_LastMod) < 0) return NULL;
    return m_LastMod;
}

//---------------------------------------------------------------------------------------

unsigned int FFQPresetMgr::GetPresetCount()
{
    return Presets->GetCount();
}

//---------------------------------------------------------------------------------------

LPFFQ_PRESET FFQPresetMgr::GetPreset(unsigned int pstIndex)
{
    if (pstIndex >= Presets->GetCount()) return NULL;
    return (LPFFQ_PRESET)Presets->GetClientData(pstIndex);
}

//---------------------------------------------------------------------------------------

LPFFQ_PRESET FFQPresetMgr::GetPreset(wxString pstId)
{
    for (unsigned int i = 0; i < Presets->GetCount(); i++)
    {
        LPFFQ_PRESET pst = (LPFFQ_PRESET)Presets->GetClientData(i);
        if (pst->preset_id.ToString() == pstId) return pst;
    }
    return NULL;
}

//---------------------------------------------------------------------------------------

LPFFQ_PRESET FFQPresetMgr::GetPresetByFingerPrint(wxString pstFp)
{
    for (unsigned int i = 0; i < Presets->GetCount(); i++)
    {
        LPFFQ_PRESET pst = (LPFFQ_PRESET)Presets->GetClientData(i);
        if (pst->finger_print == pstFp) return pst;
    }
    return NULL;
}

//---------------------------------------------------------------------------------------

long FFQPresetMgr::IndexOf(LPFFQ_PRESET pst)
{
    for (unsigned int i = 0; i < Presets->GetCount(); i++) if (Presets->GetClientData(i) == pst) return i;
    return -1;
}

//---------------------------------------------------------------------------------------

unsigned int FFQPresetMgr::SetListItem(long idx, LPFFQ_PRESET pst, bool select)
{
    Presets->Freeze();
    if (idx < 0) idx = Presets->Append(pst->preset_name, pst);
    else
    {
        Presets->SetString(idx, pst->preset_name);
        Presets->SetClientData(idx, pst);
    }
    if (select) Presets->SetSelection(idx);
    Presets->Thaw();
    m_Changes++;
    m_LastMod = pst;
    return idx;
}

//---------------------------------------------------------------------------------------

void AddSorted(std::vector<LPFFQ_PRESET> &addTo, LPFFQ_PRESET add)
{
    wxString s = add->preset_name.Lower();
    for (std::vector<LPFFQ_PRESET>::iterator ite = addTo.begin(); ite < addTo.end(); ite++)
    {
        if (s < (*ite)->preset_name.Lower())
        {
            addTo.insert(ite, add);
            return;
        }
    }
    addTo.push_back(add);
}

void FFQPresetMgr::SortPresets()
{
    Presets->Freeze();
    std::vector<LPFFQ_PRESET> temp;
    for (unsigned int i = 0; i < Presets->GetCount(); i++) AddSorted(temp, (LPFFQ_PRESET)Presets->GetClientData(i));
    for (unsigned int i = 0; i < Presets->GetCount(); i++) SetListItem(i, temp.at(i));
    Presets->Thaw();
}

//---------------------------------------------------------------------------------------

void FFQPresetMgr::UpdateControls()
{
    wxArrayInt ints;
    int sel = Presets->GetSelections(ints);
    CopyButton->Enable(sel > 0);
    EditButton->Enable(sel == 1);
    DeleteButton->Enable(sel > 0);
    UpButton->Enable((sel > 0) && (ints[0] > 0));
    DownButton->Enable((sel > 0) && ((unsigned int)ints[sel-1] < Presets->GetCount() - 1));
}

//---------------------------------------------------------------------------------------

bool openClipBrd()
{
    if (wxTheClipboard->Open()) return true;
    return ShowError(NULL, FFQS(SID_OPEN_CLIPBOARD_ERROR));
}

//---------------------------------------------------------------------------------------

void FFQPresetMgr::OnButtonClick(wxCommandEvent& event)
{

    int evtId = event.GetId();

    if ((evtId == ID_COPYBUTTON) && openClipBrd())
    {

        wxString s = "";
        unsigned int cnt = 0;

        for (unsigned int i = 0; i < Presets->GetCount(); i++) if (Presets->IsSelected(i))
        {
            LPFFQ_PRESET pst = GetPreset(i);
            s += CFG_PRESET + CRLF + pst->ToString() + CRLF + CRLF;
            cnt++;
        }

        s.Trim();
        CompressAndBase64(s, 50);
        bool ok = wxTheClipboard->SetData( new wxTextDataObject(s) );
        if (ok) wxTheClipboard->Flush();
        wxTheClipboard->Close();

        if (ok) ShowInfo(Presets, FFQSF(SID_COPY_PRESET_SUCCESS, cnt));
        else ShowError(Presets, FFQS(SID_COPY_PRESET_ERROR));

    }

    else if ((evtId == ID_PASTEBUTTON) && openClipBrd())
    {

        wxString s = "", t, dup = "";
        unsigned int cnt = 0, first = Presets->GetCount();
        wxTextDataObject data;
        if (wxTheClipboard->GetData(data))
        {
            s = data.GetText();
            DecompressFromBase64(s);
        }
        wxTheClipboard->Close();

        while (s.Len() > 0)
        {

            t = StrTrim(GetLine(s));

            if (t == CFG_PRESET)
            {

                LPFFQ_PRESET pst = new FFQ_PRESET(s);

                if (pst->preset_name.Len() > 0)
                {

                    //Ensure unique name
                    if (FindPreset(pst->preset_name, false) >= 0)
                    {
                        unsigned int i = 0;
                        do
                        {
                            i++;
                            t = pst->preset_name + "_" + ToStr(i);
                        } while (FindPreset(t, false) >= 0);
                        pst->preset_name = t;
                    }

                    //Ensure unique id
                    while (GetPreset(pst->preset_id.ToString()) != NULL) pst->preset_id = UNIQUE_ID();

                    //Add preset
                    if (cnt == 0) Presets->DeselectAll();
                    AddPreset(pst, true);
                    cnt++;

                }

            }

        }

        //Select new presets
        if (cnt > 0)
        {
            Presets->DeselectAll();
            for (unsigned int i = 0; i < cnt; i++) Presets->SetSelection(first + i);
            ShowInfo(Presets, FFQSF(SID_PASTE_PRESET_SUCCESS, cnt));
        }
        else ShowInfo(Presets, FFQS(SID_PASTE_PRESET_NONE));

    }

    else if (evtId == ID_DELETEBUTTON)
    {

        if (DoConfirm(Presets, FFQS(SID_CONFIRM_DELETE_PRESETS)))
        {

            unsigned int i = 0, ch = m_Changes;

            while (i < Presets->GetCount())
            {

                //Check if preset can be deleted
                bool can_del = !((FFQMain*)GetParent())->IsPresetActive((LPFFQ_PRESET)Presets->GetClientData(i));

                //Delete if possible
                if (can_del && Presets->IsSelected(i)) DeletePreset(i);
                else i++;

            }

            //Inform main frame about changes or alert no changes to user
            if (ch != m_Changes) ((FFQMain*)GetParent())->PresetChanged(NULL);
            else ShowError(Presets, FFQS(SID_PRESET_MODIFY_ACTIVE_ERROR));

        }

    }

    else if ((evtId == ID_EDITBUTTON) || (evtId == ID_PRESETS))
    {

        for (unsigned int i = 0; i < Presets->GetCount(); i++) if (Presets->IsSelected(i))
        {
            LPFFQ_PRESET pst = (LPFFQ_PRESET)Presets->GetClientData(i);
            if (EditPreset(pst))
            {
                SetListItem(i, pst, true);
                ((FFQMain*)GetParent())->PresetChanged(pst);
            }
            break;
        }

    }

    else if (evtId == ID_NEWBUTTON)
    {

        LPFFQ_PRESET pst = new FFQ_PRESET();
        pst->thumbs = THUMBS_AND_TILES(FFQCFG()->thumb_settings);
        pst->thumbs.make_thumbs = false;
        pst->thumbs.make_tiles = false;
        if (EditPreset(pst))
        {
            Presets->DeselectAll();
            SetListItem(-1, pst, true);
        }
        else delete pst;

    }

    else if (evtId == ID_UPBUTTON) ListBoxMoveSelectedItems(Presets, true, false);// MoveSelectedPresets(-1);

    else if (evtId == ID_DOWNBUTTON) ListBoxMoveSelectedItems(Presets, false, false);// MoveSelectedPresets(1);

    else if (evtId == ID_SORTBUTTON) SortPresets();

    else if (evtId == ID_CLOSEBUTTON)
    {

        EndModal(wxID_OK);
        return;

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQPresetMgr::OnPresetsSelect(wxCommandEvent& event)
{

    UpdateControls();

}
