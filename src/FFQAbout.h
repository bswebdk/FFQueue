/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQAbout.h                                                      *
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

#ifndef FFQABOUT_H
#define FFQABOUT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQAbout)
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/button.h>
	#include <wx/hyperlink.h>
	#include <wx/panel.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/statbmp.h>
	//*)
#endif
//(*Headers(FFQAbout)
//*)

#include <wx/animate.h>

#define ABOUT_KEY "d\te\rv"

class FFQAbout: public wxDialog
{
	public:

		FFQAbout(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~FFQAbout();
		void Execute();

		//(*Declarations(FFQAbout)
		wxStaticText* Info;
		wxPanel* AnimPanel;
		wxTextCtrl* TextCtrl;
		wxButton* CloseButton;
		wxStaticText* Hdr;
		wxStaticText* Bacon;
		wxStaticBitmap* StaticBitmap;
		//*)

	protected:

		//(*Identifiers(FFQAbout)
		static const long ID_STATICBITMAP;
		static const long ID_ANIMPANEL;
		static const long ID_TEXTCTRL;
		static const long ID_CLOSEBUTTON;
		//*)

	private:

		//(*Handlers(FFQAbout)
		void OnButtonCloseClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnTimerTrigger(wxTimerEvent& event);
		//*)

		wxAnimationCtrl *m_Anim;

		#ifdef DEBUG
		int cc;
		wxTimer Timer;
		void OnTimer(wxTimerEvent& event);
		#endif // DEBUG

		DECLARE_EVENT_TABLE()
};

#endif // FFQABOUT_H
