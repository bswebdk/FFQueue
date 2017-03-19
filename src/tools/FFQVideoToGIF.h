/*************************************************************************
*                                                                        *
*  Copyright (c) 2017 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQVideo2GIF.h                                                  *
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

#ifndef FFQVIDEOTOGIF_H
#define FFQVIDEOTOGIF_H

#include "../FFQTimeEdit.h"
#include "../utils/FFQToolJobs.h"
#include "../utils/FFQProbing.h"
#include <wx/hyperlink.h>

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQVideoToGIF)
	#include <wx/checkbox.h>
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/button.h>
	#include <wx/hyperlink.h>
	#include <wx/filedlg.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQVideoToGIF)
//*)

class FFQVideoToGIF: public wxDialog
{
	public:

		FFQVideoToGIF(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FFQVideoToGIF();

		bool Execute(LPFFQ_VID2GIF_JOB job);

		//(*Declarations(FFQVideoToGIF)
		wxHyperlinkCtrl* LimitLen;
		wxCheckBox* PreciseCuts;
		wxTextCtrl* Source;
		wxTextCtrl* Dest;
		wxStaticText* ST7;
		wxTextCtrl* Width;
		wxFlexGridSizer* TopSizer;
		wxStaticText* ST8;
		wxFileDialog* OpenFile;
		wxCheckBox* SaveLog;
		wxButton* OkBtn;
		wxCheckBox* TwoPass;
		wxTextCtrl* Height;
		wxStaticText* ST9;
		wxStaticText* FileInfo;
		wxTextCtrl* FrameRate;
		wxFlexGridSizer* MidSizer;
		wxFileDialog* SaveFile;
		wxHyperlinkCtrl* StartTime;
		wxButton* NoBtn;
		wxButton* BrowseDst;
		wxButton* BrowseSrc;
		//*)

	protected:

		//(*Identifiers(FFQVideoToGIF)
		static const long ID_SOURCE;
		static const long ID_BROWSESRC;
		static const long ID_FILEINFO;
		static const long ID_STARTTIME;
		static const long ID_WIDTH;
		static const long ID_HEIGHT;
		static const long ID_FRAMERATE;
		static const long ID_TWOPASS;
		static const long ID_PRECISECUTS;
		static const long ID_LIMITLEN;
		static const long ID_DEST;
		static const long ID_BROWSEDST;
		static const long ID_SAVELOG;
		static const long ID_OKBTN;
		static const long ID_NOBTN;
		//*)

	private:

		FFQTimeEdit* m_TimeEdit;
		TIME_VALUE m_StartTime, m_LimitLen, m_ProbeDuration;
		unsigned int m_ProbeW, m_ProbeH;
		double m_ProbeFPS;
		LPFFQ_VID2GIF_JOB m_Job;
		wxString m_LastProbed;
		FFProbeInfoParser* m_PIP;


		bool EditTime(TIME_VALUE &t);
		void UpdateLinks();

		void OnIdle(wxIdleEvent &event);

		//(*Handlers(FFQVideoToGIF)
		void OnAction(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
