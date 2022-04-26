/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQThumbMake.h                                                  *
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

#ifndef FFQTHUMBMAKE_H
#define FFQTHUMBMAKE_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQThumbMake)
	#include <wx/button.h>
	#include <wx/checkbox.h>
	#include <wx/dialog.h>
	#include <wx/filedlg.h>
	#include <wx/sizer.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQThumbMake)
#include "FFQThumbPanel.h"
//*)

#include "../utils/FFQToolJobs.h"

class FFQThumbMake: public wxDialog
{
	public:

		FFQThumbMake(wxWindow* parent);
		virtual ~FFQThumbMake();
		bool Execute(LPFFQ_THUMB_JOB job);
		//bool Execute(THUMBS_AND_TILES &tnt, wxString &forFile, bool &saveLog);

		//(*Declarations(FFQThumbMake)
		FFQThumbPanel* ThumbPanel;
		wxButton* BrowseButton;
		wxButton* CancelButton;
		wxButton* MakeButton;
		wxCheckBox* SaveLog;
		wxFileDialog* FileDlg;
		wxTextCtrl* InFile;
		//*)

	protected:

		//(*Identifiers(FFQThumbMake)
		static const long ID_INFILE;
		static const long ID_BROWSEBUTTON;
		static const long ID_ThumbPanel;
		static const long ID_SAVELOG;
		static const long ID_MAKEBUTTON;
		static const long ID_CANCELBUTTON;
		//*)

	private:

		//LPTHUMBS_AND_TILES m_EditPtr;
		LPFFQ_THUMB_JOB m_EditJob;
		bool m_Reset;

		//(*Handlers(FFQThumbMake)
		void OnAction(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

};

#endif // FFQTHUMBMAKE_H
