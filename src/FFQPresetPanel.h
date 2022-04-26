/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPresetPanel.h                                                *
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

#ifndef FFQPRESETPANEL_H
#define FFQPRESETPANEL_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQPresetPanel)
	#include <wx/button.h>
	#include <wx/choice.h>
	#include <wx/panel.h>
	#include <wx/sizer.h>
	//*)
#endif
//(*Headers(FFQPresetPanel)
//*)

#include "utils/FFQPreset.h"

class FFQPresetPanel: public wxPanel
{
	public:

		FFQPresetPanel(wxWindow* parent);
		virtual ~FFQPresetPanel();
		void FillAndSelect(wxString selectId);
		void SelectPreset(LPFFQ_PRESET pst);
		void SetFixedFirstItem(wxString item);
		LPFFQ_PRESET GetSelectedPreset();

		//(*Declarations(FFQPresetPanel)
		wxButton* MgrButton;
		wxChoice* Presets;
		//*)

	protected:

		//(*Identifiers(FFQPresetPanel)
		static const long ID_PRESETS;
		static const long ID_MGRBUTTON;
		//*)

	private:

		wxString m_FixedFirst;

		//(*Handlers(FFQPresetPanel)
		void OnAction(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif // FFQPRESETPANEL_H
