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
	#include <wx/button.h>
	#include <wx/checkbox.h>
	#include <wx/combobox.h>
	#include <wx/dialog.h>
	#include <wx/dirdlg.h>
	#include <wx/gauge.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQBatchMake)
#include "FFQPresetPanel.h"
#include <wx/statline.h>
//*)

#include "utils/FFQProbing.h"
#include "utils/FFQStreamMapping.h"

const unsigned int PIP_COUNT = 2;

class FFQBatchMake: public wxDialog
{
	public:

		FFQBatchMake(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~FFQBatchMake();

		void Clear(bool releaseJobPointers = false);
		bool Execute();
		unsigned int GetJobCount();
		void* GetJob(unsigned int index);
		void SetFiles(wxArrayString *files);

		//(*Declarations(FFQBatchMake)
		FFQPresetPanel* Preset;
		wxButton* BrowseButton;
		wxButton* CancelButton;
		wxButton* DryRunButton;
		wxButton* MakeButton;
		wxCheckBox* AudInverse;
		wxCheckBox* IncludeAttachment;
		wxCheckBox* IncludeAudio;
		wxCheckBox* IncludeData;
		wxCheckBox* IncludeSubs;
		wxCheckBox* IncludeVideo;
		wxCheckBox* NoFailAudio;
		wxCheckBox* NoFailSubs;
		wxCheckBox* NoFailVideo;
		wxCheckBox* SaveLogs;
		wxCheckBox* SubsInverse;
		wxComboBox* OutputFormat;
		wxComboBox* PrefAudio;
		wxComboBox* PrefSubs;
		wxDirDialog* DestPathDlg;
		wxFlexGridSizer* MainSizer;
		wxGauge* Gauge;
		wxStaticLine* SL1;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticLine* StaticLine3;
		wxStaticText* JobInfo;
		wxStaticText* ST1;
		wxStaticText* ST2;
		wxStaticText* ST3;
		wxStaticText* ST4;
		wxStaticText* ST5;
		wxStaticText* ST6;
		wxStaticText* ST7;
		wxStaticText* ST8;
		wxTextCtrl* OutputPath;
		//*)

	protected:

		//(*Identifiers(FFQBatchMake)
		static const long ID_JOBINFO;
		static const long ID_ST1;
		static const long ID_INCLUDEVIDEO;
		static const long ID_INCLUDEAUDIO;
		static const long ID_INCLUDESUBS;
		static const long ID_NOFAILVIDEO;
		static const long ID_NOFAILAUDIO;
		static const long ID_NOFAILSUBS;
		static const long ID_STATICLINE1;
		static const long ID_ST2;
		static const long ID_PREFAUDIO;
		static const long ID_AUDINVERSE;
		static const long ID_STATICLINE2;
		static const long ID_ST6;
		static const long ID_PREFSUBS;
		static const long ID_SUBSINVERSE;
		static const long ID_STATICLINE3;
		static const long ID_ST3;
		static const long ID_OUTPUTFORMAT;
		static const long ID_ST4;
		static const long ID_OUTPUTPATH;
		static const long ID_BROWSEBUTTON;
		static const long ID_ST5;
		static const long ID_PRESET;
		static const long ID_MAKELOGS;
		static const long ID_DRYRUNBUTTON;
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
		long m_VidIdx, m_AudIdx, m_SubsIdx;
		std::vector<LPFFPROBE_STREAM_INFO> m_StreamInf;
		wxString m_FindAud, m_FindSubs;
		FFProbeInfoParser m_PIPS[PIP_COUNT];

		bool FindJobForDest(wxString dest);
		//wxString GetStreamMapping(FFProbeInfoParser *pip, unsigned int file_id);

		void GetStreamsFromParser(FFProbeInfoParser *pip, unsigned int file_id, wxString &stream_map);
		//bool HasStreamFromParser(LPFFPROBE_STREAM_INFO si);
        //bool IncludeAudioStream(LPFFPROBE_STREAM_INFO si);
        //bool IncludeSubtitleStream(LPFFPROBE_STREAM_INFO si);
        bool IncludeStream(LPFFPROBE_STREAM_INFO si, wxString search_for, bool find_language, bool invert_result);
		void LogLine(wxString line, unsigned int color);
		void MakeJobs(bool dry_run);
		void SaveConfig();
		void UpdateControls();

		void OnIdle(wxIdleEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif // FFQBATCHMAKE_H
