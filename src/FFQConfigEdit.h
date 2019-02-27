/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConfigEdit.h                                                 *
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

#ifndef FFQCONFIGEDIT_H
#define FFQCONFIGEDIT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQConfigEdit)
	#include <wx/button.h>
	#include <wx/checklst.h>
	#include <wx/dialog.h>
	#include <wx/dirdlg.h>
	#include <wx/filedlg.h>
	#include <wx/panel.h>
	#include <wx/radiobut.h>
	#include <wx/sizer.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQConfigEdit)
//*)

class FFQConfigEdit: public wxDialog
{
	public:

		FFQConfigEdit(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~FFQConfigEdit();
		bool Execute();

		//(*Declarations(FFQConfigEdit)
		wxButton* BrowseCustPlayer;
		wxButton* BrowseFFMpeg;
		wxButton* BrowseTemp;
		wxButton* CancelButton;
		wxButton* LangButton;
		wxButton* OKButton;
		wxCheckListBox* CheckList;
		wxDirDialog* DirDlg;
		wxFileDialog* OpenDialog;
		wxPanel* TempPathRadioPanel;
		wxRadioButton* TP_Custom;
		wxRadioButton* TP_Dest;
		wxRadioButton* TP_System;
		wxTextCtrl* ConsoleCmd;
		wxTextCtrl* CustPlayer;
		wxTextCtrl* CustomTemp;
		wxTextCtrl* FFMpegPath;
		wxTextCtrl* SecondFileExts;
		//*)

	protected:

		//(*Identifiers(FFQConfigEdit)
		static const long ID_FFMPEGPATH;
		static const long ID_BROWSEFFMPEG;
		static const long ID_CUSTPLAYER;
		static const long ID_BROWSECUSTPLAYER;
		static const long ID_TP_SYSTEM;
		static const long ID_TP_DEST;
		static const long ID_TP_CUSTOM;
		static const long ID_TPRPANEL;
		static const long ID_CUSTOMTEMP;
		static const long ID_BROWSETEMP;
		static const long ID_SECONDFILEEXTS;
		static const long ID_CONSOLECMD;
		static const long ID_CHECKLIST;
		static const long ID_LANGBUTTON;
		static const long ID_OKBUTTON;
		static const long ID_CANCELBUTTON;
		//*)

	private:


		//(*Handlers(FFQConfigEdit)
		void OnButtonClick(wxCommandEvent& event);
		void OnCheckListToggled(wxCommandEvent& event);
		//*)

		void UpdateControls();

		DECLARE_EVENT_TABLE()
};

#endif // FFQCONFIGEDIT_H
