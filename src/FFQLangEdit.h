/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQLangEdit.h                                                   *
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

#ifndef FFQLANGEDIT_H
#define FFQLANGEDIT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQLangEdit)
	#include <wx/frame.h>
	#include <wx/listctrl.h>
	#include <wx/panel.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/statusbr.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQLangEdit)
//*)

#include "utils/FFQLang.h"
#include <wx/listctrl.h>

//const wxString DUMMY_PASSWORD = "\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02\t\x02";

class FFQLangEdit: public wxFrame
{
	public:

		static FFQLangEdit* Get(wxWindow *parent);
		static void Release();

		FFQLangEdit(wxWindow* parent);
		virtual ~FFQLangEdit();
		void Execute();
		void LoadList();
        void SetListItem(long idx, LPFFQ_STRING ffqs);


		//(*Declarations(FFQLangEdit)
		wxListView* ListView;
		wxPanel* TabPanel;
		wxStaticText* Info;
		wxStaticText* ST2;
		wxStaticText* ST3;
		wxStaticText* ST4;
		wxStatusBar* SB;
		wxTextCtrl* Description;
		wxTextCtrl* Password1;
		wxTextCtrl* Password2;
		wxTextCtrl* StrEdit;
		//*)

	protected:

		//(*Identifiers(FFQLangEdit)
		static const long ID_ST2;
		static const long ID_DESCRIPTION;
		static const long ID_ST3;
		static const long ID_PASSWORD1;
		static const long ID_ST4;
		static const long ID_PASSWORD2;
		static const long ID_LISTVIEW;
		static const long ID_STREDIT;
		static const long ID_INFO;
		static const long ID_TABPANEL;
		static const long ID_SB;
		//*)

		static FFQLangEdit *m_Instance;

	private:

		//(*Handlers(FFQLangEdit)
		void OnClose(wxCloseEvent& event);
		void OnListViewItemSelect(wxListEvent& event);
		//*)

		wxString m_Password;
		FFQLang *m_EditLang, *m_OrgLang;
		LPFFQ_STRING m_EditStr, m_OrgStr;
		long m_EditIndex, m_Modified;
		bool m_SkipEvents;
		unsigned int m_EditListSize;

		void ExportTranslationFile();
		void ImportTranslationFile(wxString file_name = wxEmptyString);

		void LoadEditStr(bool internal = false);
		bool SaveEditStr(bool force, bool clear = true);
		bool SavePwdAndDesc();
		bool SkipItemChange(wxString reason);

		void UpdateStatus();

		void OnIdle(wxIdleEvent &event);
		void OnKeyDown(wxKeyEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif // FFQLANGEDIT_H
