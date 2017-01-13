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
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/checkbox.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
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
		wxCheckBox* IgnIdx;
		wxButton* NoBtn;
		wxCheckBox* DiscardCorrupt;
		wxStaticLine* Line;
		wxCheckBox* FrameRateSwitch;
		wxPanel* Panel1;
		wxStaticText* ST0;
		wxCheckBox* IgnDTS;
		wxStaticText* ST2;
		wxStaticText* ST4;
		wxStaticText* ST3;
		wxStaticLine* StaticLine1;
		wxButton* ClearBtn;
		wxStaticText* ST1;
		wxTextCtrl* ItsOffset;
		wxButton* OkBtn;
		wxTextCtrl* FrameRate;
		wxCheckBox* GenPTS;
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
