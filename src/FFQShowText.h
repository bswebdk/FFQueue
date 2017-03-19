/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQShowText.h                                                   *
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

#ifndef FFQSHOWTEXT_H
#define FFQSHOWTEXT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQShowText)
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQShowText)
//*)

#include "utils/FFQNvpList.h"

class FFQShowText: public wxDialog
{
	public:

		FFQShowText(wxWindow* parent);
		virtual ~FFQShowText();

		//(*Declarations(FFQShowText)
		wxTextCtrl* Text;
		//*)

		void Execute(const wxString &title, wxString text);
		void Execute(const wxString &title, LPFFQ_NVP_LIST nvp);

	protected:

		//(*Identifiers(FFQShowText)
		static const long ID_TEXT;
		//*)

	private:

		//(*Handlers(FFQShowText)
		//*)

		wxFont fNormal, fBold;
		void SetTitleAndShow(const wxString &title);

		DECLARE_EVENT_TABLE()
};

#endif // FFQSHOWTEXT_H
