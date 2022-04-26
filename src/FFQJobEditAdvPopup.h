/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQJobEditAdvPopup.h                                            *
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

#ifndef FFQJOBEDITADVPOPUP_H
#define FFQJOBEDITADVPOPUP_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQJobEditAdvPopup)
	#include <wx/button.h>
	#include <wx/checkbox.h>
	#include <wx/dialog.h>
	#include <wx/panel.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQJobEditAdvPopup)
#include <wx/statline.h>
//*)

class FFQJobEditAdvPopup: public wxDialog
{
	public:

		FFQJobEditAdvPopup(wxWindow* parent);
		virtual ~FFQJobEditAdvPopup();
		bool Execute(void* data);

		//bool CheckFocus(wxWindow *focus = NULL);

		//(*Declarations(FFQJobEditAdvPopup)
		wxButton* ClearBtn;
		wxButton* NoBtn;
		wxButton* OkBtn;
		wxCheckBox* DiscardCorrupt;
		wxCheckBox* FrameRateSwitch;
		wxCheckBox* GenPTS;
		wxCheckBox* IgnDTS;
		wxCheckBox* IgnIdx;
		wxPanel* Panel1;
		wxStaticLine* Line;
		wxStaticLine* StaticLine1;
		wxStaticText* ST0;
		wxStaticText* ST1;
		wxStaticText* ST2;
		wxStaticText* ST3;
		wxStaticText* ST4;
		wxTextCtrl* FrameRate;
		wxTextCtrl* ItsOffset;
		//*)

	protected:

		//(*Identifiers(FFQJobEditAdvPopup)
		static const long ID_PANEL1;
		static const long ID_STATICLINE1;
		static const long ID_CLEARBTN;
		static const long ID_OKBTN;
		static const long ID_NOBTN;
		//*)

	private:

		void ResetCtrls();

		//(*Handlers(FFQJobEditAdvPopup)
		void OnAction(wxCommandEvent& event);
		void OnChar(wxKeyEvent& event);
		//*)

		DECLARE_EVENT_TABLE()


};

#endif // FFQJOBEDITADVPOPUP_H
