/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQBatchMake.h                                                  *
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

#ifndef FFQBATCHMAKE_H
#define FFQBATCHMAKE_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQBatchMake)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/checkbox.h>
	#include <wx/button.h>
	#include <wx/dirdlg.h>
	#include <wx/dialog.h>
	#include <wx/combobox.h>
	#include <wx/gauge.h>
	//*)
#endif
//(*Headers(FFQBatchMake)
#include <src/FFQPresetPanel.h>
//*)

#include "utils/FFQProbing.h"
#include "utils/FFQStreamMapping.h"

const size_t PIP_COUNT = 2;

class FFQBatchMake: public wxDialog
{
	public:

		FFQBatchMake(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~FFQBatchMake();

		void Clear(bool releaseJobPointers = false);
		bool Execute();
		size_t GetJobCount();
		void* GetJob(size_t index);
		void SetFiles(wxArrayString *files);

		//(*Declarations(FFQBatchMake)
		wxButton* BrowseButton;
		wxFlexGridSizer* MainSizer;
		wxDirDialog* DestPathDlg;
		wxTextCtrl* OutputPath;
		wxCheckBox* IncludeVideo;
		wxComboBox* PrefAudio;
		wxStaticText* ST5;
		wxStaticText* ST2;
		wxStaticText* ST4;
		wxStaticText* ST3;
		wxStaticText* JobInfo;
		wxStaticText* ST1;
		wxGauge* Gauge;
		wxCheckBox* IncludeSubs;
		wxComboBox* OutputFormat;
		wxButton* CancelButton;
		FFQPresetPanel* Preset;
		wxCheckBox* IncludeAudio;
		wxButton* MakeButton;
		//*)

	protected:

		//(*Identifiers(FFQBatchMake)
		static const long ID_JOBINFO;
		static const long ID_ST1;
		static const long ID_INCLUDEVIDEO;
		static const long ID_INCLUDEAUDIO;
		static const long ID_INCLUDESUBS;
		static const long ID_ST2;
		static const long ID_PREFAUDIO;
		static const long ID_ST3;
		static const long ID_OUTPUTFORMAT;
		static const long ID_ST4;
		static const long ID_OUTPUTPATH;
		static const long ID_BROWSEBUTTON;
		static const long ID_ST5;
		static const long ID_PRESET;
		static const long ID_MAKEBUTTON;
		static const long ID_CANCELBUTTON;
		static const long ID_GAUGE;
		//*)

	private:

		//(*Handlers(FFQBatchMake)
		void OnAction(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		wxArrayString* m_Files;
		wxVector<void*> m_Jobs;
		bool m_ClearLog, m_Making, m_Abort, m_DoIdleTask;
		FFProbeInfoParser m_PIPS[PIP_COUNT];

		bool FindJobForDest(wxString dest);
		void LogLine(wxString line, size_t color);
		void MakeJobs();
		void SaveConfig();

		void OnIdle(wxIdleEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif // FFQBATCHMAKE_H
