/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQTimeEdit.h                                                   *
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

#ifndef FFQTIMEEDIT_H
#define FFQTIMEEDIT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQTimeEdit)
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQTimeEdit)
//*)

#include "utils/FFQTimeValue.h"

typedef struct SUBTRACT_TIME {

    wxString name;
    TIME_VALUE time;

} SUBTRACT_TIME;

class FFQTimeEdit: public wxDialog
{
	public:

		FFQTimeEdit(wxWindow* parent);
		virtual ~FFQTimeEdit();

		//(*Declarations(FFQTimeEdit)
		wxButton* CancelButton;
		wxButton* ClearButton;
		wxButton* OKButton;
		wxFlexGridSizer* MainSizer;
		wxStaticBoxSizer* SubtractSizer;
		wxStaticText* MillisLabel;
		wxStaticText* ST1;
		wxStaticText* ST2;
		wxStaticText* ST3;
		wxTextCtrl* Hours;
		wxTextCtrl* Millis;
		wxTextCtrl* Minutes;
		wxTextCtrl* Seconds;
		//*)

		bool Execute(TIME_VALUE &timeVal, bool allowMilliseconds = true);
		bool Execute(TIME_VALUE &timeVal, LPTIME_VALUE subtractVal1, LPTIME_VALUE subtractVal2, bool allowMilliseconds = true);
		bool Execute(TIME_VALUE &timeVal, SUBTRACT_TIME subtractVals[], int subtractCount, bool allowMilliseconds = true);
		TIME_VALUE GetTimeValue();
		void SetTimeValue(TIME_VALUE &val);


	protected:

		//(*Identifiers(FFQTimeEdit)
		static const long ID_ST1;
		static const long ID_HOURS;
		static const long ID_ST2;
		static const long ID_MINUTES;
		static const long ID_ST3;
		static const long ID_SECONDS;
		static const long ID_MILLISLABEL;
		static const long ID_MILLIS;
		static const long ID_CLEARBUTTON;
		static const long ID_OKBUTTON;
		static const long ID_CANCELBUTTON;
		//*)

		static const long ID_SUBTRACT;
	private:

		//(*Handlers(FFQTimeEdit)
		void OnButtonClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		SUBTRACT_TIME *m_Subtract;

		int ButtonIndex(wxButton *button);
		bool DoExecute(TIME_VALUE &time, SUBTRACT_TIME subtract[], int count, bool allowMillis);
		void SubtractTimeValue(LPTIME_VALUE time);
		void UpdateControls();


		DECLARE_EVENT_TABLE()
};

#endif // FFQTIMEEDIT_H
