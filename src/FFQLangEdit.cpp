/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQLangEdit.cpp                                                 *
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

#include "FFQLangEdit.h"
#include "utils/FFQMisc.h"
#include "utils/FFQConst.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FFQLangEdit)
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FFQLangEdit)
//*)


//(*IdInit(FFQLangEdit)
const long FFQLangEdit::ID_ST2 = wxNewId();
const long FFQLangEdit::ID_DESCRIPTION = wxNewId();
const long FFQLangEdit::ID_ST3 = wxNewId();
const long FFQLangEdit::ID_PASSWORD1 = wxNewId();
const long FFQLangEdit::ID_ST4 = wxNewId();
const long FFQLangEdit::ID_PASSWORD2 = wxNewId();
const long FFQLangEdit::ID_LISTVIEW = wxNewId();
const long FFQLangEdit::ID_STREDIT = wxNewId();
const long FFQLangEdit::ID_INFO = wxNewId();
const long FFQLangEdit::ID_TABPANEL = wxNewId();
const long FFQLangEdit::ID_SB = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQLangEdit,wxFrame)
	//(*EventTable(FFQLangEdit)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

const wxString DUMMY_PASSWORD = "\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02";

//---------------------------------------------------------------------------------------

const wxString STRING_STATUS[4] = { "New", "Internal", "Translated", "Modified" };

//---------------------------------------------------------------------------------------

FFQLangEdit* FFQLangEdit::m_Instance = NULL;

//---------------------------------------------------------------------------------------

FFQLangEdit* FFQLangEdit::Get(wxWindow *parent)
{
    if (m_Instance == NULL) m_Instance = new FFQLangEdit(NULL);//parent);
    return m_Instance;
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::Release()
{
    if (m_Instance != NULL)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

//---------------------------------------------------------------------------------------

FFQLangEdit::FFQLangEdit(wxWindow* parent)
{
	//(*Initialize(FFQLangEdit)
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* SBS1;
	wxFlexGridSizer* FlexGridSizer2;
	wxStaticBoxSizer* SBS2;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer3;

	Create(parent, wxID_ANY, _T("FFQueue language editor"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetClientSize(wxSize(800,600));
	TabPanel = new wxPanel(this, ID_TABPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_TABPANEL"));
	FlexGridSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(1);
	SBS1 = new wxStaticBoxSizer(wxVERTICAL, TabPanel, _T("Language information"));
	FlexGridSizer2 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer2->AddGrowableCol(1);
	ST2 = new wxStaticText(TabPanel, ID_ST2, _T("Description:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST2"));
	FlexGridSizer2->Add(ST2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	Description = new wxTextCtrl(TabPanel, ID_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DESCRIPTION"));
	Description->SetMaxLength(100);
	FlexGridSizer2->Add(Description, 1, wxALL|wxEXPAND, 3);
	ST3 = new wxStaticText(TabPanel, ID_ST3, _T("Password:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST3"));
	FlexGridSizer2->Add(ST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer3 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	FlexGridSizer3->AddGrowableCol(2);
	Password1 = new wxTextCtrl(TabPanel, ID_PASSWORD1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD, wxDefaultValidator, _T("ID_PASSWORD1"));
	Password1->SetMaxLength(50);
	FlexGridSizer3->Add(Password1, 1, wxALL|wxEXPAND, 0);
	ST4 = new wxStaticText(TabPanel, ID_ST4, _T("Re-type password:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ST4"));
	FlexGridSizer3->Add(ST4, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Password2 = new wxTextCtrl(TabPanel, ID_PASSWORD2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD, wxDefaultValidator, _T("ID_PASSWORD2"));
	FlexGridSizer3->Add(Password2, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 3);
	SBS1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 3);
	FlexGridSizer1->Add(SBS1, 1, wxALL|wxEXPAND, 5);
	ListView = new wxListView(TabPanel, ID_LISTVIEW, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_NO_SORT_HEADER, wxDefaultValidator, _T("ID_LISTVIEW"));
	FlexGridSizer1->Add(ListView, 1, wxALL|wxEXPAND, 5);
	SBS2 = new wxStaticBoxSizer(wxHORIZONTAL, TabPanel, _T("Edit selected string"));
	FlexGridSizer4 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	FlexGridSizer4->AddGrowableRow(0);
	StrEdit = new wxTextCtrl(TabPanel, ID_STREDIT, wxEmptyString, wxDefaultPosition, wxSize(-1,200), wxTE_PROCESS_ENTER|wxTE_MULTILINE|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_STREDIT"));
	FlexGridSizer4->Add(StrEdit, 1, wxALL|wxEXPAND, 3);
	Info = new wxStaticText(TabPanel, ID_INFO, _T("Use [Ctrl]+[Enter] to store && next, [Ctrl]+[Up/Down] to navigate list (skipping changes), [Ctrl]+[S] to store changes, [Ctrl]+[I] to load integrated string."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_INFO"));
	Info->Disable();
	FlexGridSizer4->Add(Info, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SBS2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	FlexGridSizer1->Add(SBS2, 1, wxALL|wxEXPAND, 5);
	TabPanel->SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(TabPanel);
	FlexGridSizer1->SetSizeHints(TabPanel);
	SB = new wxStatusBar(this, ID_SB, wxST_SIZEGRIP, _T("ID_SB"));
	int __wxStatusBarWidths_1[3] = { 1, 200, 200 };
	int __wxStatusBarStyles_1[3] = { wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL };
	SB->SetFieldsCount(3,__wxStatusBarWidths_1);
	SB->SetStatusStyles(3,__wxStatusBarStyles_1);
	SetStatusBar(SB);

	Connect(ID_LISTVIEW,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&FFQLangEdit::OnListViewItemSelect);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&FFQLangEdit::OnClose);
	//*)

    ListView->AppendColumn("ID", wxLIST_FORMAT_LEFT, 100);
    ListView->AppendColumn("Status", wxLIST_FORMAT_LEFT, 100);
    ListView->AppendColumn("Content", wxLIST_FORMAT_LEFT, 550);

    m_Password = "";
    m_OrgLang = NULL;
    m_EditLang = NULL;
	m_EditStr = NULL;
	m_OrgStr = NULL;
	m_EditIndex = -1;
	m_SkipEvents = false;
	m_Modified = 0;

	Connect(wxID_ANY, wxEVT_IDLE, (wxObjectEventFunction)&FFQLangEdit::OnIdle);
	StrEdit->Bind(wxEVT_KEY_DOWN, (wxObjectEventFunction)&FFQLangEdit::OnKeyDown, this);

    CenterOnScreen();

}

//---------------------------------------------------------------------------------------

FFQLangEdit::~FFQLangEdit()
{
	//(*Destroy(FFQLangEdit)
	//*)
    if (m_OrgLang)
    {
        delete m_OrgLang;
        m_OrgLang = NULL;
    }
    if (m_EditLang)
    {
        if (m_Modified > 0) m_EditLang->SaveLanguage();
        delete m_EditLang;
        m_EditLang = NULL;
    }
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::Execute()
{

    //Ask for password (if necessary)
    if (FFQL()->HasPassword())
    {
        if (!FFQL()->CheckPassword(m_Password))
        {
            m_Password = wxGetPasswordFromUser("Please enter the password to the language file", "Authentication");
            if (!FFQL()->CheckPassword(m_Password))
            {
                ShowError("Bad password");
                return;
            }
        }
    }

    if (m_OrgLang == NULL)
    {
        //Prepare a copy of the internal language and a copy for edit
        m_OrgLang = new FFQLang(false);
        m_EditLang = new FFQLang(true);

        //Fill the list (only done once)
        LoadList();
    }

    //Display the window
    if (IsIconized()) Restore();
    if (IsVisible()) Raise();
    else Show();

}

//---------------------------------------------------------------------------------------

void FFQLangEdit::LoadList()
{
    Description->SetValue(m_EditLang->GetDescription());
    Description->SetModified(false);
    if (m_EditLang->HasPassword())
    {
        Password1->SetValue(DUMMY_PASSWORD);
        Password1->SetModified(false);
        Password2->SetValue(DUMMY_PASSWORD);
        Password2->SetModified(false);
    }
    ListView->Freeze();
    for (unsigned int i = 0; i < m_EditLang->GetCount(); i++) SetListItem(-1, m_EditLang->GetPtrAtIndex(i));
    ListView->Thaw();
    ListView->Select(0);
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::SetListItem(long idx, LPFFQ_STRING ffqs)
{
    if (idx < 0) idx = ListView->InsertItem(ListView->GetItemCount(), ToStr(ffqs->sid)); //New item
    else ListView->SetItemText(idx, ToStr(ffqs->sid)); //Existing item

    //Set status
    int status = 0; //New item
    if ((ffqs->flags & SF_STORED) != 0)
    {
        if ((ffqs->flags & SF_MODIFIED) != 0) status = 3; //Modified
        else if ((ffqs->flags & SF_TRANSLATED) != 0) status = 2; //Translated
        else status = 1; //Internal item
    }
    ListView->SetItem(idx, 1, STRING_STATUS[status]);
    wxString s = ffqs->str;
    s.Replace(CRLF, "<br>");
    ListView->SetItem(idx, 2, s);
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::LoadEditStr(bool internal)
{
    m_EditIndex = ListView->GetFirstSelected();
    if (m_EditIndex < 0) return;
    m_EditStr = m_EditLang->GetPtrAtIndex(m_EditIndex);
    m_OrgStr = m_OrgLang->GetPtrAtIndex(m_EditIndex);
    wxString s = internal ? m_OrgStr->str : m_EditStr->str;
    m_EditListSize = s.Replace("|", CRLF) + 1;
    StrEdit->ChangeValue(s);
    StrEdit->SetModified(false);
    ListView->EnsureVisible(m_EditIndex);
    UpdateStatus();
}

//---------------------------------------------------------------------------------------

bool FFQLangEdit::SaveEditStr(bool force, bool clear)
{

    if (m_EditStr != NULL)
    {

        if (force || StrEdit->IsModified())
        {

            //The content of the string has been modified - full verification is needed

            //Get value
            wxString s = StrTrim(StrEdit->GetValue());

            //Validate value
            if (s.Find("|") >= 0) return SkipItemChange("Illegal character \"|\" found in string");

            if ((s.Len() > 0) && (m_EditListSize > 1))
            {

                //Item is a list of items - validate the number of items
                wxString t, ls = "";
                unsigned int items = 0;

                while (s.Len() > 0)
                {
                    t = StrTrim(GetLine(s, true));
                    if (t.Len() > 0)
                    {
                        ls += t + "|";
                        items++;
                    }
                }

                if (items != m_EditListSize) return SkipItemChange("The number of items / lines cannot be changed");
                ls.Remove(ls.Len() - 1, 1); //Remove last |
                s = ls;

            }

            s.Shrink();

            if ((s.Len() == 0) || (s == m_OrgStr->str))
            {

                //Revert to internal value
                m_EditStr->str = m_OrgStr->str;
                m_EditStr->flags = SF_STORED;

            }

            else
            {

                //Set translated string value
                m_EditStr->str = s;
                m_EditStr->flags = SF_STORED | SF_TRANSLATED;

            }

            HashString(m_OrgStr->str, m_EditStr->org_hash);
            HashString(m_EditStr->str, m_EditStr->str_hash);

            //Update list item
            SetListItem(m_EditIndex, m_EditStr);

            //Increase modification counter to ensure file write
            m_Modified++;

        }

    }

    if (clear)
    {

        //Clear the item currently being edited
        m_EditStr = NULL;
        m_OrgStr = NULL;
        m_EditIndex = -1;
        StrEdit->Clear();

    }

    StrEdit->SetModified(false);
    UpdateStatus();
    return true;

}

//---------------------------------------------------------------------------------------

bool FFQLangEdit::SavePwdAndDesc()
{
    wxString s = StrTrim(Description->GetValue());
    unsigned int r;
    do { r = s.Replace("<br>", " "); } while (r > 0);
    do { r = s.Replace("  ", " "); } while (r > 0);
    s.Trim();
    if (s != m_EditLang->GetDescription())
    {
        m_EditLang->SetDescription(s);
        m_Modified++;
    }
    s = Password1->GetValue();
    if ((s == Password2->GetValue()) && (s != DUMMY_PASSWORD))
    {
        m_EditLang->ChangePassword(s);
        m_Modified++;
    }
    else if (s != Password2->GetValue())
        return ShowError(Password2, "The passwords are not equal and cannot be set");

    return true;
}

//---------------------------------------------------------------------------------------

bool FFQLangEdit::SkipItemChange(wxString reason)
{
    m_SkipEvents = true;
    ListView->Select(m_EditIndex, true);
    return ShowError(StrEdit, reason);
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::UpdateStatus()
{
    SB->SetStatusText("", 0);
    SB->SetStatusText(wxString::Format("Editing %i of %u", (int)(ListView->GetFirstSelected()+1), SIZEFMT(ListView->GetItemCount()), 1));
    int pct = (int)( (float)m_EditLang->GetFlagCount(SF_STORED) / (float)m_EditLang->GetCount() * 100.0  );
    SB->SetStatusText(wxString::Format("%i%% translated", pct), 2);
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::OnClose(wxCloseEvent& event)
{
    if (event.CanVeto())
    {
        event.Veto();
        if (!SavePwdAndDesc()) return;
        if (!SaveEditStr(false)) return;
        Show(false);
    }
    else
    {
        SavePwdAndDesc();
        SaveEditStr(false);
    }
    if (Destroy()) m_Instance = NULL;
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::OnListViewItemSelect(wxListEvent& event)
{
    if (m_SkipEvents || (event.GetIndex() == m_EditIndex)) return;
    if (SaveEditStr(false)) LoadEditStr();
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::OnIdle(wxIdleEvent &event)
{
    if ((m_EditIndex >= 0) && (ListView->GetFirstSelected() != m_EditIndex)) ListView->Select(m_EditIndex, true);
    else m_SkipEvents = false;
}

//---------------------------------------------------------------------------------------

void FFQLangEdit::OnKeyDown(wxKeyEvent &event)
{

    if (event.ControlDown())
    {

        //int key = (event.GetUnicodeKey() == WXK_NONE) ? event.GetKeyCode() : (int)event.GetUnicodeKey();
        int key = ((int)event.GetUnicodeKey() == 0) ? event.GetKeyCode() : (int)event.GetUnicodeKey();
        long sel = ListView->GetFirstSelected();

        if ((key == WXK_UP) && (sel > 0))
        {
            //if (StrEdit->IsModified()) LoadEditStr();
            StrEdit->SetModified(false);
            ListView->Select(sel-1, true);
        }

        else if ((key == WXK_DOWN) && (sel < ListView->GetItemCount() - 1))
        {
            //if (StrEdit->IsModified()) LoadEditStr();
            StrEdit->SetModified(false);
            ListView->Select(sel+1, true);
        }

        else if (key == WXK_RETURN)
        {

            SaveEditStr(true, false);

            while ((unsigned int)sel++ < m_EditLang->GetCount())
            {
                LPFFQ_STRING ffqs = m_EditLang->GetPtrAtIndex(sel);
                if ((ffqs->flags & SF_STORED) == 0)
                {
                     ListView->Select(sel, true);
                     return;
                }
            }

            ShowInfo(StrEdit, "The end has been reached, my friend :-)");

        }

        else if (key == 83) SaveEditStr(true, false); //S

        else if (key == 73) LoadEditStr(true); //I

        else event.Skip();

    } else event.Skip();

}

