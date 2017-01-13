/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQJobEditAdv.h                                                 *
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

#ifndef FFQJOBEDITADV_H
#define FFQJOBEDITADV_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQJobEditAdv)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/checklst.h>
	#include <wx/checkbox.h>
	#include <wx/filedlg.h>
	#include <wx/hyperlink.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(FFQJobEditAdv)
#include <wx/notebook.h>
#include "FFQPresetPanel.h"
//*)

#include <wx/hyperlink.h>

#include "utils/FFQJob.h"
#include "utils/FFQProbing.h"
#include "FFQTimeEdit.h"
#include "FFQShowText.h"
#include "FFQJobEditAdvPopup.h"
#include "FFQJobEditAdvCuts.h"

//Structure used to hold the controls and values of
//one input file panel / tab
typedef struct INPUT_CTRLS
{

    wxPanel *panel;
    wxTextCtrl *input;//, *itsoffset, *framerate;
    wxFlexGridSizer *sizer;
    wxString validated_path, itsoffset, framerate;
    bool valid, discard_corrupt, genpts, igndts, ignidx, can_cut;
    wxButton *browse;
    wxHyperlinkCtrl *start, *cuts, *more;
    TIME_VALUE start_val;
    FFQ_CUTS cut_cfg;
    //wxCheckBox *discard_corrupt, *genpts, *igndts, *ignidx;
    FFProbeInfoParser probe;

} INPUT_CTRLS, *LPINPUT_CTRLS;

class FFQJobEditAdv: public wxDialog
{
	public:

		FFQJobEditAdv(wxWindow* parent);
		virtual ~FFQJobEditAdv();
		bool Execute(LPFFQ_JOB job);
		void ShowPopupCtrls(bool show = true, bool cancel = false);

		//(*Declarations(FFQJobEditAdv)
		wxCheckBox* SaveLog;
		wxButton* BrowseOutput;
		wxButton* SaveJob;
		wxButton* DelInput;
		wxButton* AddInput;
		wxButton* StreamRefresh;
		wxButton* StreamUp;
		wxHyperlinkCtrl* OutputLength;
		wxButton* CancelDlg;
		wxStaticText* ST5;
		wxStaticText* ST4;
		wxStaticText* ST3;
		wxTextCtrl* Output;
		wxButton* AddSecondary;
		wxButton* StreamDown;
		wxFileDialog* OpenFile;
		wxCheckListBox* StreamList;
		wxNotebook* Inputs;
		wxTextCtrl* CmdLine;
		FFQPresetPanel* Preset;
		wxFileDialog* SaveFile;
		wxFlexGridSizer* OutputSizer;
		//*)

	protected:

		//(*Identifiers(FFQJobEditAdv)
		static const long ID_INPUTS;
		static const long ID_ADDINPUT;
		static const long ID_DELINPUT;
		static const long ID_ADDSECONDARY;
		static const long ID_STREAMLIST;
		static const long ID_STREAMUP;
		static const long ID_STREAMDOWN;
		static const long ID_STREAMREFRESH;
		static const long ID_OUTPUT;
		static const long ID_BROWSEOUTPUT;
		static const long ID_OUTPUTLENGTH;
		static const long ID_STATICTEXT3;
		static const long ID_CMDLINE;
		static const long ID_STATICTEXT4;
		static const long ID_PRESET;
		static const long ID_SAVELOG;
		static const long ID_SAVEJOB;
		static const long ID_CANCELDLG;
		//*)

		static const long ID_BROWSE;
		static const long ID_START;
		static const long ID_CUTS;
		static const long ID_MORE;

	private:

		wxArrayPtrVoid m_CtrlData; //Holds the INPUT_FILE_CONTROLS for the individual tabs
		FFQTimeEdit *m_TimeEdit;
		FFQShowText *m_ViewText;
		FFQJobEditAdvCuts *m_CutEdit;
		FFQJobEditAdvPopup *m_PopupCtrls;
		bool m_FindSecondary, m_PopupValidate, m_CanCut, m_AutoPreset;
		TIME_VALUE m_OutLen;
		//FFQProcess *m_Process;
		wxString m_StreamMap;
		LPFFQ_JOB m_Job;

		void AddInputFile(LPFFQ_INPUT_FILE in_file, bool select = true);
		int AddSecondaryFiles(wxString &for_file);
		bool EditTime(TIME_VALUE &time, bool subtract);
		int FindInvalidInput(bool select, bool frame_rate = false);
		int FindInputByPath(wxString path);
		LPINPUT_CTRLS GetCtrlData(int index = -1);
		wxString GetPresetFingerPrint(int file_index = 0);
		wxString GetStreamMapping();
		int RefreshStreamList(bool force_all = false);
		void RemoveInputFile(int index);
		void SetStreamMapping(wxString mapping);
		void UpdateControls();
		void UpdateLink(int index);
		void UpdateToolTip(LPINPUT_CTRLS ctrls = NULL);
		bool ValidateJob();

		void OnIdle(wxIdleEvent &event);

		//(*Handlers(FFQJobEditAdv)
		void OnAction(wxCommandEvent& event);
		void OnStreamListChange(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif // FFQJOBEDITADV_H
