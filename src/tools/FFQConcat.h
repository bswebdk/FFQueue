/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConcat.h                                                     *
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

#ifndef FFQCONCAT_H
#define FFQCONCAT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQConcat)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/checkbox.h>
	#include <wx/listbox.h>
	#include <wx/radiobut.h>
	#include <wx/slider.h>
	#include <wx/panel.h>
	#include <wx/filedlg.h>
	#include <wx/hyperlink.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(FFQConcat)
#include <wx/notebook.h>
#include "../FFQPresetPanel.h"
#include <wx/clrpicker.h>
//*)

#include "../FFQTimeEdit.h"
#include "../utils/FFQProbing.h"
#include "../utils/FFQToolJobs.h"
#include <wx/hyperlink.h>

typedef struct CONCAT_DATA {

    wxString path;
    TIME_VALUE duration, custDuration;
    unsigned int width, height;
    int videoID, audioID;

    CONCAT_DATA() { reset(); }
    CONCAT_DATA(wxString &aPath) { reset(); path=aPath; }
    void reset() { path=""; width=0; height=0; videoID=-1; audioID=-1; duration=TIME_VALUE(); custDuration=TIME_VALUE();  }

} CONCAT_DATA, *LPCONCAT_DATA;

class FFQConcat: public wxDialog
{
	public:

		FFQConcat(wxWindow* parent);
		virtual ~FFQConcat();
		//void CleanupLast();
		bool Execute(LPFFQ_CONCAT_JOB job);
		//bool Execute(wxString &command, wxString &saveLogFor, int PageID = 0);

		//(*Declarations(FFQConcat)
		wxCheckBox* SaveLog;
		wxFlexGridSizer* Sizer3;
		wxButton* CCUp;
		wxTextCtrl* SSAudio;
		wxHyperlinkCtrl* LimitDest;
		wxFlexGridSizer* Sizer4;
		wxRadioButton* CCUsePadBlur;
		wxFlexGridSizer* CCPadSizer;
		wxStaticText* StaticText2;
		wxStaticText* ST22;
		wxTextCtrl* SSWidth;
		wxCheckBox* CCSubtitles;
		wxFlexGridSizer* CCBlurSizer;
		wxFlexGridSizer* Sizer2;
		wxSlider* CCPadBlur;
		wxCheckBox* CCSimple;
		wxCheckBox* SSFit;
		wxStaticText* ST9;
		wxButton* CCAdd;
		wxCheckBox* CCSetSar;
		wxFlexGridSizer* SSSizer1;
		wxStaticText* StaticText1;
		wxButton* CCDown;
		wxButton* OkButton;
		wxStaticText* ST6;
		wxFileDialog* SaveFileDlg;
		wxPanel* MergePage;
		wxCheckBox* CCAudio;
		wxPanel* SlideshowPage;
		wxColourPickerCtrl* CCPadColor;
		wxRadioButton* CCUsePadColor;
		wxStaticText* ST8;
		wxTextCtrl* DestFile;
		wxStaticText* ST5;
		wxStaticText* ST2;
		wxButton* SSBrowseAudio;
		wxStaticText* ST4;
		wxTextCtrl* SSSource;
		wxStaticText* ST3;
		wxTextCtrl* SSFrameTime;
		wxCheckBox* SSSetPTS;
		wxStaticText* ST1;
		wxTextCtrl* SSHeight;
		wxListBox* CCSources;
		wxColourPickerCtrl* SSPadding;
		wxCheckBox* CCVideo;
		wxFileDialog* OpenFileDlg;
		wxButton* SSBrowseImg;
		wxButton* CancelButton;
		FFQPresetPanel* Preset;
		wxFlexGridSizer* Sizer1;
		wxButton* CCRemove;
		wxStaticText* ST7;
		wxCheckBox* CCPadding;
		wxHyperlinkCtrl* SSFrameStatus;
		wxCheckBox* SSLoopFrames;
		wxButton* BrowseDest;
		wxNotebook* Pages;
		//*)

	protected:

		//(*Identifiers(FFQConcat)
		static const long ID_ST1;
		static const long ID_IMGSRC;
		static const long ID_SSBROWSEIMG;
		static const long ID_SSFRAMESTATUS;
		static const long ID_SSFIT;
		static const long ID_ST2;
		static const long ID_TEXTCTRL1;
		static const long ID_ST22;
		static const long ID_TEXTCTRL2;
		static const long ID_ST3;
		static const long ID_SSPADDING;
		static const long ID_ST4;
		static const long ID_ST5;
		static const long ID_SSFRAMETIME;
		static const long ID_ST6;
		static const long ID_SSSETPTS;
		static const long ID_SSAUDIO;
		static const long ID_SSBROWSEAUDIO;
		static const long ID_SSLOOPFRAMES;
		static const long ID_SLIDESHOWPAGE;
		static const long ID_CCSOURCES;
		static const long ID_CCADD;
		static const long ID_CCREMOVE;
		static const long ID_CCUP;
		static const long ID_CCDOWN;
		static const long ID_CCSIMPLE;
		static const long ID_CCSETSAR;
		static const long ID_CCPADDING;
		static const long ID_CCUSEPADCOLOR;
		static const long ID_CCPADCOLOR;
		static const long ID_CCUSEPADBLUR;
		static const long ID_STATICTEXT1;
		static const long ID_CCPADBLUR;
		static const long ID_STATICTEXT2;
		static const long ID_ST9;
		static const long ID_CCVideo;
		static const long ID_CCAUDIO;
		static const long ID_CCSUBTITLES;
		static const long ID_MERGEPAGE;
		static const long ID_PAGES;
		static const long ID_ST7;
		static const long ID_DESTFILE;
		static const long ID_BROWSEDEST;
		static const long ID_ST8;
		static const long ID_PRESET;
		static const long ID_LIMITDEST;
		static const long ID_SAVELOG;
		static const long ID_OKBUTTON;
		static const long ID_CANCELBUTTON;
		//*)

	private:

		FFQTimeEdit *m_TimeEdit;
		FFProbeInfoParser *m_PIP;

		wxString m_ImgPatn, m_LastPst, m_Command, m_TempPath;
		bool m_DoIdleTask, m_BlurOK;
		size_t m_ImgCount, m_ImgFirst;
		TIME_VALUE m_LimitLen;
		LPFFQ_CONCAT_JOB m_EditJob;
		CONCAT_DATA m_AudInfo;

		void AddConcatSources(wxArrayString *paths);
		void BuildMergeFilter(wxString &filter, wxString &smap);
		bool EditTime(TIME_VALUE &tv);
		bool EnumSlideshowFrames();
		bool GetFileInfo(LPCONCAT_DATA cd, bool dimensionRequired);
		//bool MakeConcatCommand(wxString &cmd);
		//bool MakeMergeCommand(wxString &cmd);
		//bool MakeSlideshowCommand(wxString &cmd);
		//void SetValuesFrom(LPFFQ_CONCAT_JOB job);
		void SetDestFile(wxString based_on);
		bool UpdateAudioInfo();
		void UpdateControls();
		bool ValidateDialog();
		bool GetOutputFileAndPreset(wxString &file, LPFFQ_PRESET *pst, bool allowVideoCopyCodec, bool allowTwoPass);

		void OnIdle(wxIdleEvent &event);
        void OnDropFiles(wxDropFilesEvent& event);

		//(*Handlers(FFQConcat)
		void OnAction(wxCommandEvent& event);
		void OnCCSrcDblClick(wxCommandEvent& event);
		void OnPagesPageChanging(wxNotebookEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif // FFQCONCAT_H
