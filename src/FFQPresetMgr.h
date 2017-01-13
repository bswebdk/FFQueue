/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPresetMgr.h                                                  *
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

#ifndef FFQPRESETMGR_H
#define FFQPRESETMGR_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQPresetMgr)
	#include <wx/sizer.h>
	#include <wx/listbox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(FFQPresetMgr)
//*)

#include "FFQPresetEdit.h"

class FFQPresetMgr: public wxDialog
{
	public:

		static FFQPresetMgr* Get();

		FFQPresetMgr(wxWindow* parent, wxWindowID id=wxID_ANY);
		virtual ~FFQPresetMgr();

		size_t AddPreset(LPFFQ_PRESET pst, bool select = false);
		void ClearPresets();
		void DeletePreset(LPFFQ_PRESET pst);
		void DeletePreset(size_t pstIndex);
		bool EditPreset(LPFFQ_PRESET pst);
		bool Execute(wxChoice* forChoice = NULL);
		void FillChoice(wxChoice* choice, wxString selectId);
		long FindPreset(wxString name, bool matchCase = false);
		LPFFQ_PRESET GetLastModified();
		size_t GetPresetCount();
		LPFFQ_PRESET GetPreset(size_t pstIndex);
		LPFFQ_PRESET GetPreset(wxString pstId);
		LPFFQ_PRESET GetPresetByFingerPrint(wxString pstFp);
		long IndexOf(LPFFQ_PRESET pst);

		//(*Declarations(FFQPresetMgr)
		wxButton* UpButton;
		wxListBox* Presets;
		wxButton* CopyButton;
		wxButton* CloseButton;
		wxButton* SortButton;
		wxButton* PasteButton;
		wxButton* DownButton;
		wxButton* NewButton;
		wxButton* DeleteButton;
		wxButton* EditButton;
		//*)

	protected:

		//(*Identifiers(FFQPresetMgr)
		static const long ID_PRESETS;
		static const long ID_NEWBUTTON;
		static const long ID_EDITBUTTON;
		static const long ID_DELETEBUTTON;
		static const long ID_UPBUTTON;
		static const long ID_DOWNBUTTON;
		static const long ID_SORTBUTTON;
		static const long ID_COPYBUTTON;
		static const long ID_PASTEBUTTON;
		static const long ID_CLOSEBUTTON;
		//*)

	private:

		//(*Handlers(FFQPresetMgr)
		void OnButtonClick(wxCommandEvent& event);
		void OnPresetsSelect(wxCommandEvent& event);
		//*)

		FFQPresetEdit *m_Editor;
		LPFFQ_PRESET m_LastMod;
		size_t m_Changes;

		//void MoveSelectedPresets(int by);
		size_t SetListItem(long idx, LPFFQ_PRESET pst, bool select = false);
		void SortPresets();
		//void SwapPresets(size_t idx1, size_t idx2);
		void UpdateControls();

		DECLARE_EVENT_TABLE()
};

#endif // FFQPRESETMGR_H
