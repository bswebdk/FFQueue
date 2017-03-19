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
	#include <wx/combobox.h>
	#include <wx/checkbox.h>
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/button.h>
	#include <wx/menu.h>
	#include <wx/radiobut.h>
	#include <wx/filedlg.h>
	#include <wx/panel.h>
	#include <wx/slider.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/choice.h>
	#include <wx/listbox.h>
	//*)
#endif
//(*Headers(FFQPresetEdit)
#include <wx/grid.h>
#include "tools/FFQThumbPanel.h"
#include "FFQBitRatePanel.h"
#include <wx/notebook.h>
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
		wxCheckBox* UseAudioQScale;
		wxStaticText* SubsLab3;
		wxRadioButton* SubsSizeCust;
		wxButton* CancelButton;
		wxListBox* FilterList;
		wxSlider* QDiff;
		wxPanel* SubsSizePan;
		wxStaticText* KST1;
		wxStaticBoxSizer* ASBS4;
		wxPanel* BitRatePage;
		wxStaticBoxSizer* ASBS3;
		wxComboBox* VideoThreads;
		wxComboBox* OutputFormat;
		wxStaticText* KST8;
		wxStaticText* QMaxVal;
		wxFlexGridSizer* VideoOtherSizer;
		wxStaticText* CST1;
		wxStaticBoxSizer* SSBS2;
		wxButton* RemoveFilterButton;
		wxPanel* SubtitlesPage;
		wxStaticText* SubsLab1;
		wxFlexGridSizer* VidSizer1;
		wxSlider* QMax;
		wxStaticText* AST1;
		wxFlexGridSizer* KeyFrameSizer;
		wxStaticText* FilterTip;
		wxStaticText* OST1;
		wxTextCtrl* CustomArgs1;
		wxGrid* MetaData;
		wxCheckBox* AudioFiltersComplex;
		wxStaticText* OST2;
		wxSlider* SceneChangeSens;
		wxStaticText* OST5;
		wxTextCtrl* FOURCC_Vid;
		wxButton* FilterDownButton;
		wxFlexGridSizer* SubsSizer2;
		wxStaticText* OST7;
		wxStaticText* KST3;
		wxTextCtrl* KeyFrameMin;
		wxTextCtrl* KeyFrameMax;
		wxFileDialog* OpenFileDlg;
		wxTextCtrl* SubsWidth;
		wxChoice* AudioCodec;
		wxStaticText* QDiffVal;
		wxCheckBox* PresetTemp;
		wxStaticText* ST55;
		wxRadioButton* SubsSizeDef;
		wxStaticText* SubsLab2;
		wxComboBox* AudioProfile;
		wxChoice* Trellis;
		wxRadioButton* SubsText;
		wxStaticText* QMinVal;
		wxStaticText* KST2;
		wxNotebook* Pages;
		wxRadioButton* SubsBitmap;
		wxTextCtrl* FOURCC_Aud;
		wxCheckBox* UseQDiff;
		FFQBitRatePanel* VideoBitRate;
		wxStaticText* OST6;
		wxPanel* FilterPage;
		wxChoice* VideoCodec;
		wxStaticText* KST4;
		wxStaticText* ConstRateVal;
		wxStaticText* QST2;
		wxStaticText* SubsScaleLab;
		wxButton* FilterPreviewButton;
		wxFlexGridSizer* SceneChangeSizer;
		wxStaticText* ST53;
		wxStaticText* QST1;
		wxStaticText* OST4;
		wxComboBox* SpeedPreset;
		wxFlexGridSizer* AudioQScaleSizer;
		wxFlexGridSizer* VideoBitRateSizer;
		wxStaticText* AudioQScaleVal;
		wxStaticText* ScSensInfo;
		wxCheckBox* UseVideoQScale;
		wxStaticText* AST4;
		wxChoice* KeyFrameMaxB;
		wxPanel* MetaDataPage;
		wxCheckBox* UseQMin;
		wxComboBox* MotionEstimation;
		wxSlider* VideoQScale;
		wxPanel* MiscPage;
		wxStaticText* KST6;
		wxPanel* AudioPage;
		wxStaticText* KST7;
		wxStaticText* ST56;
		wxCheckBox* UseQMax;
		wxFlexGridSizer* VideoQualitySizer;
		wxStaticText* ST51;
		wxPanel* QualityPage;
		wxStaticText* KST5;
		wxTextCtrl* PresetName;
		wxTextCtrl* AudioChannels;
		wxStaticText* CST2;
		wxSlider* QMin;
		wxStaticText* MinConstRateVal;
		wxCheckBox* TwoPass;
		wxChoice* VideoSync;
		wxCheckBox* TwoPassNull;
		wxSlider* MinConstRate;
		wxSlider* ConstRate;
		wxFlexGridSizer* SubsSizer3;
		wxCheckBox* UseMinConstRate;
		wxCheckBox* KeepFileTime;
		wxComboBox* Aspect;
		wxButton* AddFilterButton;
		wxPanel* KeyFramesPage;
		wxMenu AddFilterMenu;
		wxStaticText* OST3;
		wxStaticText* AST2;
		wxStaticBoxSizer* FourCCSizer;
		wxButton* EditFilterButton;
		wxPanel* VideoPage;
		wxStaticText* VideoQScaleVal;
		wxStaticText* ST52;
		wxButton* OKButton;
		wxFlexGridSizer* FilterSizer;
		wxPanel* OtherPage;
		wxChoice* SubsCodec;
		wxTextCtrl* FrameRate;
		FFQThumbPanel* ThumbsPanel;
		wxSlider* AudioQScale;
		wxStaticBoxSizer* ASBS2;
		wxCheckBox* UseConstRate;
		wxTextCtrl* CustomArgs2;
		wxTextCtrl* SubsHeight;
		FFQBitRatePanel* AudioBitRate;
		wxCheckBox* UseSceneChangeSens;
		wxPanel* ThumbsPage;
		wxSlider* SubsScale;
		wxRadioButton* SubsSizePAL;
		wxStaticText* AST5;
		wxChoice* KeyFrameMaxP;
		wxStaticText* KST9;
		wxCheckBox* MF_FastStart;
		wxButton* FilterUpButton;
		wxComboBox* PixFmt;
		wxNotebook* VideoPages;
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
