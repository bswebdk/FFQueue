/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPresetEdit.h                                                 *
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

#ifndef FFQPRESETEDIT_H
#define FFQPRESETEDIT_H

//#include "wx_pch.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQPresetEdit)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/menu.h>
	#include <wx/textctrl.h>
	#include <wx/checkbox.h>
	#include <wx/listbox.h>
	#include <wx/radiobut.h>
	#include <wx/slider.h>
	#include <wx/panel.h>
	#include <wx/filedlg.h>
	#include <wx/choice.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/combobox.h>
	//*)
#endif
//(*Headers(FFQPresetEdit)
#include <wx/notebook.h>
#include "tools/FFQThumbPanel.h"
#include "FFQBitRatePanel.h"
#include <wx/grid.h>
//*)

#include "FFQFilterEdit.h"
#include "utils/FFQPreset.h"
//#include <wx/valnum.h>
//#include <wx/menu.h>

class FFQPresetEdit: public wxDialog
{
	public:

		FFQPresetEdit(wxWindow* parent);
		virtual ~FFQPresetEdit();

		bool Execute(LPFFQ_PRESET preset);

		//(*Declarations(FFQPresetEdit)
		wxTextCtrl* SubsHeight;
		wxTextCtrl* KeyFrameMax;
		wxChoice* AudioCodec;
		wxButton* EditFilterButton;
		wxCheckBox* UseConstRate;
		wxStaticText* ConstRateVal;
		wxStaticText* CST1;
		wxStaticText* QDiffVal;
		wxCheckBox* PresetTemp;
		wxCheckBox* KeepFileTime;
		wxPanel* FilterPage;
		wxStaticText* QMaxVal;
		wxPanel* BitRatePage;
		wxStaticText* OST4;
		wxTextCtrl* CustomArgs1;
		wxButton* OKButton;
		wxSlider* AudioQScale;
		wxStaticText* FilterTip;
		wxChoice* VideoCodec;
		wxStaticBoxSizer* ASBS3;
		wxCheckBox* UseAudioQScale;
		wxSlider* SceneChangeSens;
		wxTextCtrl* KeyFrameMin;
		wxStaticBoxSizer* FourCCSizer;
		wxRadioButton* SubsSizeDef;
		wxStaticText* KST5;
		wxStaticText* OST2;
		wxFlexGridSizer* AudioQScaleSizer;
		wxFlexGridSizer* KeyFrameSizer;
		wxCheckBox* TwoPass;
		wxRadioButton* SubsSizePAL;
		wxStaticText* OST6;
		wxStaticText* KST3;
		wxPanel* MetaDataPage;
		wxCheckBox* AudioFiltersComplex;
		wxCheckBox* UseQDiff;
		wxPanel* KeyFramesPage;
		wxFlexGridSizer* VideoQualitySizer;
		FFQBitRatePanel* VideoBitRate;
		wxCheckBox* TwoPassNull;
		wxPanel* OtherPage;
		wxMenu AddFilterMenu;
		wxStaticText* CST2;
		wxStaticText* KST1;
		wxStaticBoxSizer* ASBS2;
		wxTextCtrl* FOURCC_Vid;
		wxComboBox* AudioProfile;
		wxStaticText* AST5;
		wxStaticText* ST55;
		wxNotebook* VideoPages;
		FFQBitRatePanel* AudioBitRate;
		wxChoice* VideoSync;
		wxChoice* KeyFrameMaxP;
		wxStaticText* QST2;
		wxButton* AddFilterButton;
		wxStaticText* MinConstRateVal;
		wxComboBox* VideoThreads;
		wxSlider* SubsScale;
		wxCheckBox* UseSceneChangeSens;
		wxTextCtrl* AudioChannels;
		wxStaticText* ST53;
		wxFlexGridSizer* SceneChangeSizer;
		wxCheckBox* MF_FastStart;
		wxChoice* SubsCodec;
		wxStaticText* ScSensInfo;
		wxComboBox* PixFmt;
		wxSlider* QMin;
		wxButton* RemoveFilterButton;
		wxFlexGridSizer* FilterSizer;
		wxStaticText* ST51;
		wxStaticText* KST8;
		wxPanel* ThumbsPage;
		wxListBox* FilterList;
		wxGrid* MetaData;
		wxPanel* QualityPage;
		wxStaticText* QMinVal;
		wxCheckBox* UseMinConstRate;
		wxStaticText* KST9;
		wxComboBox* Aspect;
		wxStaticText* KST4;
		wxStaticText* AST1;
		wxFlexGridSizer* VidSizer1;
		wxButton* FilterUpButton;
		wxFlexGridSizer* SubsSizer3;
		wxTextCtrl* PresetName;
		wxStaticText* SubsLab2;
		wxRadioButton* SubsText;
		wxStaticText* AST4;
		wxPanel* VideoPage;
		wxStaticBoxSizer* ASBS4;
		wxStaticText* AudioQScaleVal;
		wxStaticText* OST1;
		wxButton* FilterDownButton;
		wxChoice* Trellis;
		wxPanel* MiscPage;
		wxSlider* VideoQScale;
		wxChoice* KeyFrameMaxB;
		wxStaticText* ST56;
		wxRadioButton* SubsBitmap;
		wxCheckBox* UseVideoQScale;
		wxComboBox* OutputFormat;
		wxStaticText* KST2;
		wxStaticText* KST7;
		wxStaticText* OST7;
		wxComboBox* MotionEstimation;
		wxTextCtrl* CustomArgs2;
		wxStaticText* KST6;
		wxStaticText* OST5;
		wxStaticText* SubsLab3;
		wxStaticText* OST3;
		wxStaticText* SubsScaleLab;
		wxSlider* QDiff;
		wxStaticText* AST2;
		wxFileDialog* OpenFileDlg;
		FFQThumbPanel* ThumbsPanel;
		wxSlider* QMax;
		wxFlexGridSizer* VideoBitRateSizer;
		wxButton* CancelButton;
		wxTextCtrl* FrameRate;
		wxTextCtrl* SubsWidth;
		wxRadioButton* SubsSizeCust;
		wxStaticText* VideoQScaleVal;
		wxTextCtrl* FOURCC_Aud;
		wxStaticBoxSizer* SSBS2;
		wxStaticText* ST52;
		wxComboBox* SpeedPreset;
		wxStaticText* QST1;
		wxCheckBox* UseQMax;
		wxFlexGridSizer* SubsSizer2;
		wxPanel* AudioPage;
		wxCheckBox* UseQMin;
		wxSlider* MinConstRate;
		wxPanel* SubtitlesPage;
		wxPanel* SubsSizePan;
		wxFlexGridSizer* VideoOtherSizer;
		wxButton* FilterPreviewButton;
		wxSlider* ConstRate;
		wxNotebook* Pages;
		wxStaticText* SubsLab1;
		//*)

	protected:

		//(*Identifiers(FFQPresetEdit)
		static const long ID_PRESETNAME;
		static const long ID_PRESETTEMP;
		static const long ID_VIDEOCODEC;
		static const long ID_TWOPASS;
		static const long ID_TWOPASSNULL;
		static const long ID_VIDEOBITRATE;
		static const long ID_BITRATEPAGE;
		static const long ID_QST1;
		static const long ID_QST2;
		static const long ID_USEVIDEOQSCALE;
		static const long ID_VIDEOQSCALE;
		static const long ID_VIDEOQSCALEVAL;
		static const long ID_USECONSTRATE;
		static const long ID_CONSTRATE;
		static const long ID_CONSTRATEVAL;
		static const long ID_USEMINCONSTRATE;
		static const long ID_MINCONSTRATE;
		static const long ID_MINCONSTRATEVAL;
		static const long ID_USEQMIN;
		static const long ID_QMIN;
		static const long ID_QMINVAL;
		static const long ID_USEQMAX;
		static const long ID_QMAX;
		static const long ID_QMAXVAL;
		static const long ID_USEQDIFF;
		static const long ID_QDIFF;
		static const long ID_QDIFFVAL;
		static const long ID_QUALITYPAGE;
		static const long ID_KEYFRAMEMIN;
		static const long ID_KEYFRAMEMAX;
		static const long ID_KEYFRAMEMAXB;
		static const long ID_KEYFRAMEMAXP;
		static const long ID_MOTIONESTIMATION;
		static const long ID_USESCENECHANGESENSITIVITY;
		static const long ID_SCENECHANGESENS;
		static const long ID_KEYFRAMESPAGE;
		static const long ID_OST3;
		static const long ID_VIDEOSYNC;
		static const long ID_OST1;
		static const long ID_FRAMERATE;
		static const long ID_OST2;
		static const long ID_OST4;
		static const long ID_PIXFMT;
		static const long ID_OST6;
		static const long ID_TRELLIS;
		static const long ID_OST7;
		static const long ID_SPEEDPRESET;
		static const long ID_OST5;
		static const long ID_VIDEOTHREADS;
		static const long ID_OTHERPAGE;
		static const long ID_VIDEOPAGES;
		static const long ID_VIDEOPAGE;
		static const long ID_AUDIOCODEC;
		static const long ID_AUDIOBITRATE;
		static const long ID_USEAUDIOQSCALE;
		static const long ID_AST1;
		static const long ID_AUDIOQSCALE;
		static const long ID_AST2;
		static const long ID_AUDIOQSCALEVAL;
		static const long ID_AST4;
		static const long ID_AUDIOCHANNELS;
		static const long ID_AST5;
		static const long ID_AUDIOPROFILE;
		static const long ID_AUDIOPAGE;
		static const long ID_SUBSCODEC;
		static const long ID_ST53;
		static const long ID_SUBSTEXT;
		static const long ID_SUBSBITMAP;
		static const long ID_SUBSLAB2;
		static const long ID_SUBSSIZEDEF;
		static const long ID_SUBSSIZEPAL;
		static const long ID_SUBSSIZECUST;
		static const long ID_SUBSWIDTH;
		static const long ID_STATICTEXT11;
		static const long ID_SUBSHEIGHT;
		static const long ID_SUBSSIZEPAN;
		static const long ID_SUBSLAB1;
		static const long ID_SUBSSCALE;
		static const long ID_STATICTEXT8;
		static const long ID_SUBTITLESPAGE;
		static const long ID_FILTERLIST;
		static const long ID_ADDFILTERBUTTON;
		static const long ID_EDITFILTERBUTTON;
		static const long ID_REMOVEFILTERBUTTON;
		static const long ID_FILTERUPBUTTON;
		static const long ID_FILTERDOWNBUTTON;
		static const long ID_FILTERPREVIEWBUTTON;
		static const long ID_AUDIOFILTERSCOMPLEX;
		static const long ID_FILTERTIP;
		static const long ID_FILTERPAGE;
		static const long ID_METADATA;
		static const long ID_METADATAPAGE;
		static const long ID_THUMBSPANEL;
		static const long ID_THUMBSPAGE;
		static const long ID_ST51;
		static const long ID_FOURCC_VID;
		static const long ID_ST52;
		static const long ID_FOURCC_AUD;
		static const long ID_CST1;
		static const long ID_CUSTOMARGS;
		static const long ID_CST2;
		static const long ID_CUSTOMARGS2;
		static const long ID_STATICTEXT13;
		static const long ID_ASPECT;
		static const long ID_OUTPUTFORMAT;
		static const long ID_MF_FASTSTART;
		static const long ID_KEEPFILETIME;
		static const long ID_MISCPANEL;
		static const long ID_PAGES;
		static const long ID_OKBUTTON;
		static const long ID_CANCELBUTTON;
		//*)

		static const long ID_ADD_FILTER_MENU_BASE;

	private:

		//(*Handlers(FFQPresetEdit)
		void OnClose(wxCloseEvent& event);
		void OnButtonClick(wxCommandEvent& event);
		void OnChoiceChange(wxCommandEvent& event);
		void OnSubsScaleChange(wxScrollEvent& event);
		void OnMetaDataCellChange(wxGridEvent& event);
		void OnNotebookPageChanged(wxNotebookEvent& event);
		//*)

		FFQFilterEdit *FilterEditor;
		wxString m_MetaData, m_PreviewFile;
		CODEC_INFO m_VidCodecInfo, m_AudCodecInfo;
		bool m_ShowPreviewDlg;
		LPFFQ_PRESET m_Preset;


		bool EditFilter(LPFFMPEG_FILTER filter);
		int FindFilter(FILTER_TYPE ft);
		void PreviewFilters();
		//void SwapFilters(size_t idx1, size_t idx2);


		//bool GetBitrate(wxTextCtrl* br, wxChoice* brt, wxString &rate, bool validate);
		//void SetBitrate(wxTextCtrl* br, wxChoice* brt, wxString &rate);
		void SetChoiceItems(wxChoice* ch, wxString &items, wxString &select);
		bool GetMetaData(wxString &data);
		void SetMetaData(wxString data);
		//void SetNumberValue(wxTextCtrl* ctrl, wxString val, wxString noVal = "0");
		void UpdateCodecInfo();
		void UpdateControls(bool sizers = false);
		void UpdateFilterMenu();
		void UpdateSliderLabels(int SliderId);
		void UpdateSubtitleFilter();
		void UpdateVideoPages(bool sizers = false);

		DECLARE_EVENT_TABLE()
};

#endif // FFQPRESETEDIT_H
