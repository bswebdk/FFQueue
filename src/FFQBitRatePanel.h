/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQBitRatePanel.h                                               *
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

#ifndef FFQBITRATEPANEL_H
#define FFQBITRATEPANEL_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQBitRatePanel)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/panel.h>
	#include <wx/choice.h>
	//*)
#endif
//(*Headers(FFQBitRatePanel)
//*)

class FFQBitRatePanel: public wxPanel
{
	public:

		FFQBitRatePanel(wxWindow* parent);
		virtual ~FFQBitRatePanel();

		void SetValues(wxString values);
        bool GetValues(wxString &values, bool showErr);

		//(*Declarations(FFQBitRatePanel)
		wxStaticText* BST1;
		wxFlexGridSizer* MainSizer;
		wxChoice* BitRateType;
		wxStaticText* ST5;
		wxStaticText* ST2;
		wxTextCtrl* BufSize;
		wxStaticText* ST4;
		wxStaticText* ST3;
		wxStaticText* BST2;
		wxStaticText* BST4;
		wxStaticText* ST1;
		wxTextCtrl* MinRate;
		wxTextCtrl* MaxRate;
		wxStaticText* BST3;
		wxTextCtrl* BitRate;
		//*)

	protected:

		//(*Identifiers(FFQBitRatePanel)
		static const long ID_ST1;
		static const long ID_BITRATETYPE;
		static const long ID_ST2;
		static const long ID_BITRATE;
		static const long ID_BST1;
		static const long ID_ST3;
		static const long ID_MINRATE;
		static const long ID_BST2;
		static const long ID_ST5;
		static const long ID_BUFSIZE;
		static const long ID_BST4;
		static const long ID_ST4;
		static const long ID_MAXRATE;
		static const long ID_BST3;
		//*)

	private:

		//(*Handlers(FFQBitRatePanel)
		void OnChoiceChange(wxCommandEvent& event);
		//*)

		void UpdateLabels();

		DECLARE_EVENT_TABLE()
};

#endif // FFQBITRATEPANEL_H
