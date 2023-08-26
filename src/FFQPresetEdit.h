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
	#include <wx/button.h>
	#include <wx/checkbox.h>
	#include <wx/choice.h>
	#include <wx/combobox.h>
	#include <wx/dialog.h>
	#include <wx/filedlg.h>
	#include <wx/listbox.h>
	#include <wx/menu.h>
	#include <wx/panel.h>
	#include <wx/radiobut.h>
	#include <wx/sizer.h>
	#include <wx/slider.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	//*)
#endif
//(*Headers(FFQPresetEdit)
#include "FFQBitRatePanel.h"
#include "tools/FFQThumbPanel.h"
#include <wx/grid.h>
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
		FFQBitRatePanel* AudioBitRate;
		FFQBitRatePanel* VideoBitRate;
		FFQThumbPanel* ThumbsPanel;
		wxButton* AddFilterButton;
		wxButton* CancelButton;
		wxButton* ClearMetaDataBtn;
		wxButton* DispositionsBtn;
		wxButton* EditFilterButton;
		wxButton* FilterDownButton;
		wxButton* FilterPreviewButton;
		wxButton* FilterUpButton;
		wxButton* FullSpecVidButton;
		wxButton* OKButton;
		wxButton* RemoveFilterButton;
		wxCheckBox* AudioFiltersComplex;
		wxCheckBox* KeepFileTime;
		wxCheckBox* MF_FastStart;
		wxCheckBox* PresetTemp;
		wxCheckBox* SegmentBreakB;
		wxCheckBox* SegmentIncTime;
		wxCheckBox* SegmentResetTS;
		wxCheckBox* SegmentStreaming;
		wxCheckBox* TwoPass;
		wxCheckBox* TwoPassNull;
		wxCheckBox* UseAudioQScale;
		wxCheckBox* UseClosedGOP;
		wxCheckBox* UseConstRate;
		wxCheckBox* UseMinConstRate;
		wxCheckBox* UseQDiff;
		wxCheckBox* UseQMax;
		wxCheckBox* UseQMin;
		wxCheckBox* UseSceneChangeSens;
		wxCheckBox* UseVideoQScale;
		wxChoice* AudioCodec;
		wxChoice* HWD_Accel;
		wxChoice* KeyFrameMaxB;
		wxChoice* KeyFrameMaxP;
		wxChoice* MetaDataFor;
		wxChoice* SegmentLenType;
		wxChoice* SegmentListType;
		wxChoice* SubsCodec;
		wxChoice* Trellis;
		wxChoice* VideoCodec;
		wxChoice* VideoSync;
		wxComboBox* Aspect;
		wxComboBox* AudioProfile;
		wxComboBox* HWD_Codec;
		wxComboBox* MotionEstimation;
		wxComboBox* OutputFormat;
		wxComboBox* PixFmt;
		wxComboBox* SpeedPreset;
		wxComboBox* SubsCharEnc;
		wxComboBox* VideoThreads;
		wxComboBox* VideoTuning;
		wxFileDialog* OpenFileDlg;
		wxFlexGridSizer* AudioQScaleSizer;
		wxFlexGridSizer* FilterSizer;
		wxFlexGridSizer* KeyFrameSizer;
		wxFlexGridSizer* PresetTuneSizer;
		wxFlexGridSizer* SceneChangeSizer;
		wxFlexGridSizer* SubsSizer2;
		wxFlexGridSizer* SubsSizer3;
		wxFlexGridSizer* VideoBitRateSizer;
		wxFlexGridSizer* VideoCodecSizer;
		wxFlexGridSizer* VideoHWDecodeSizer;
		wxFlexGridSizer* VideoOtherSizer;
		wxFlexGridSizer* VideoQualitySizer;
		wxFlexGridSizer* VideoTwoPassSizer;
		wxGrid* MetaData;
		wxListBox* FilterList;
		wxMenu AddFilterMenu;
		wxMenu DispositionsMenu;
		wxNotebook* Pages;
		wxNotebook* VideoPages;
		wxPanel* AudioPage;
		wxPanel* BitRatePage;
		wxPanel* FilterPage;
		wxPanel* HWDecodePage;
		wxPanel* KeyFramesPage;
		wxPanel* MetaDataPage;
		wxPanel* MiscPage;
		wxPanel* OtherPage;
		wxPanel* QualityPage;
		wxPanel* SubsSizePan;
		wxPanel* SubtitlesPage;
		wxPanel* ThumbsPage;
		wxPanel* VideoPage;
		wxRadioButton* SubsBitmap;
		wxRadioButton* SubsSizeCust;
		wxRadioButton* SubsSizeDef;
		wxRadioButton* SubsSizePAL;
		wxRadioButton* SubsText;
		wxSlider* AudioQScale;
		wxSlider* ConstRate;
		wxSlider* MinConstRate;
		wxSlider* QDiff;
		wxSlider* QMax;
		wxSlider* QMin;
		wxSlider* SceneChangeSens;
		wxSlider* SubsScale;
		wxSlider* VideoQScale;
		wxStaticBoxSizer* ASBS2;
		wxStaticBoxSizer* ASBS3;
		wxStaticBoxSizer* ASBS4;
		wxStaticBoxSizer* FourCCSizer;
		wxStaticBoxSizer* SSBS2;
		wxStaticText* AST1;
		wxStaticText* AST2;
		wxStaticText* AST4;
		wxStaticText* AST5;
		wxStaticText* AudioQScaleVal;
		wxStaticText* CST1;
		wxStaticText* CST2;
		wxStaticText* ConstRateVal;
		wxStaticText* FilterTip;
		wxStaticText* HWD1;
		wxStaticText* HWD2;
		wxStaticText* HWD3;
		wxStaticText* HWD4;
		wxStaticText* KST1;
		wxStaticText* KST2;
		wxStaticText* KST3;
		wxStaticText* KST4;
		wxStaticText* KST5;
		wxStaticText* KST6;
		wxStaticText* KST7;
		wxStaticText* KST8;
		wxStaticText* KST9;
		wxStaticText* MinConstRateVal;
		wxStaticText* OST1;
		wxStaticText* OST2;
		wxStaticText* OST3;
		wxStaticText* OST4;
		wxStaticText* OST5;
		wxStaticText* OST6;
		wxStaticText* OST7;
		wxStaticText* OST8;
		wxStaticText* QDiffVal;
		wxStaticText* QMaxVal;
		wxStaticText* QMinVal;
		wxStaticText* QST1;
		wxStaticText* QST2;
		wxStaticText* ST51;
		wxStaticText* ST52;
		wxStaticText* ST53;
		wxStaticText* ST55;
		wxStaticText* ST56;
		wxStaticText* STMD1;
		wxStaticText* ScSensInfo;
		wxStaticText* SegST1;
		wxStaticText* SegST2;
		wxStaticText* SubsLab1;
		wxStaticText* SubsLab2;
		wxStaticText* SubsLab3;
		wxStaticText* SubsLab4;
		wxStaticText* SubsScaleLab;
		wxStaticText* VideoQScaleVal;
		wxTextCtrl* AudioChannels;
		wxTextCtrl* CustomArgs1;
		wxTextCtrl* CustomArgs2;
		wxTextCtrl* FOURCC_Aud;
		wxTextCtrl* FOURCC_Vid;
		wxTextCtrl* FrameRate;
		wxTextCtrl* HWD_Device;
		wxTextCtrl* HWD_DeviceInit;
		wxTextCtrl* KeyFrameMax;
		wxTextCtrl* KeyFrameMin;
		wxTextCtrl* PresetName;
		wxTextCtrl* SegmentLen;
		wxTextCtrl* SubsHeight;
		wxTextCtrl* SubsWidth;
		//*)

	protected:

		//(*Identifiers(FFQPresetEdit)
		static const long ID_PRESETNAME;
		static const long ID_PRESETTEMP;
		static const long ID_VIDEOCODEC;
		static const long ID_FULLSPECVIDBUTTON;
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
		static const long ID_USECLOSEDGOP;
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
		static const long ID_OST8;
		static const long ID_VIDEOTUNING;
		static const long ID_OST5;
		static const long ID_VIDEOTHREADS;
		static const long ID_OTHERPAGE;
		static const long ID_HWD_ACCEL;
		static const long ID_HWD_CODEC;
		static const long ID_HWD_DEVICE;
		static const long ID_HWD_DEVICEINIT;
		static const long ID_HWDECODEPAGE;
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
		static const long wxID_NONE;
		static const long ID_SUBSCHARENC;
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
		static const long ID_CLEARMETADATABTN;
		static const long ID_STMD1;
		static const long ID_METADATAFOR;
		static const long ID_METADATAPAGE;
		static const long ID_THUMBSPANEL;
		static const long ID_THUMBSPAGE;
		static const long ID_ST51;
		static const long ID_FOURCC_VID;
		static const long ID_ST52;
		static const long ID_FOURCC_AUD;
		static const long ID_CST1;
		static const long ID_CUSTOM_ARGS_1;
		static const long ID_CST2;
		static const long ID_CUSTOM_ARGS_2;
		static const long ID_SEGMENTLEN;
		static const long ID_SEGMENTLENTYPE;
		static const long ID_SEGMENTLISTTYPE;
		static const long ID_SEGMENTRESETTS;
		static const long ID_SEGMENTINCTIME;
		static const long ID_SEGMENTSTREAMING;
		static const long ID_SEGMENTBREAKB;
		static const long ID_STATICTEXT13;
		static const long ID_ASPECT;
		static const long ID_DISPOSITIONSBTN;
		static const long ID_OUTPUTFORMAT;
		static const long ID_MF_FASTSTART;
		static const long ID_KEEPFILETIME;
		static const long ID_MISCPAGE;
		static const long ID_PAGES;
		static const long ID_OKBUTTON;
		static const long ID_CANCELBUTTON;
		//*)

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
		wxString m_PreviewFile, m_FullSpecVid;
		wxArrayString m_MetaData;
		int m_SelMetaData, m_FullSpecVidIdx;
		CODEC_INFO m_VidCodecInfo, m_AudCodecInfo;
		bool m_ShowPreviewDlg, m_CanSegment, m_CanSSegment;
		LPFFQ_PRESET m_Preset;
	    wxIntegerValidator<unsigned int> m_UIntVal;
	    wxMenu *m_DispositionMenus[3];
	    int m_DispositionSelection[3];



		bool EditFilter(LPFFMPEG_FILTER filter);
		int FindFilter(FILTER_TYPE ft);
		void PreviewFilters();
		//void SwapFilters(size_t idx1, size_t idx2);


		//bool GetBitrate(wxTextCtrl* br, wxChoice* brt, wxString &rate, bool validate);
		//void SetBitrate(wxTextCtrl* br, wxChoice* brt, wxString &rate);
		void SetItems(wxItemContainer* ch, wxString &items, wxString &select, bool is_codec = true);
		bool GetMetaData(wxString &data);
		void SetMetaData(wxString data);
		//void SetNumberValue(wxTextCtrl* ctrl, wxString val, wxString noVal = "0");
		void UpdateCodecInfo();
		void UpdateControls(bool sizers = false);
		void UpdateFilterMenu();
		void UpdateSliderLabels(int SliderId);
		void UpdateSubtitleFilter();
		void UpdateVideoPages(bool sizers = false);

		void OnMenuSelected(wxCommandEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif // FFQPRESETEDIT_H
