/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQFullSpec.h                                                   *
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

#ifndef FFQFULLSPEC_H
#define FFQFULLSPEC_H
#include <vector>
#include <wx/regex.h>
#include "utils/FFQStaticJob.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(FFQFullSpec)
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/filedlg.h>
	#include <wx/scrolwin.h>
	#include <wx/sizer.h>
	//*)
#endif
//(*Headers(FFQFullSpec)
//*)

extern const wxString FULLSPEC_FILE_EXTENSION;

enum FULLSPEC_FIELD_TYPE { ftHEADER, ftCHECK, ftCHOICE, ftSTRING, ftINTEGER, ftFLOAT, ftCOMBO, ftCHECKLIST };

struct FULLSPEC_FIELD
{
    FULLSPEC_FIELD_TYPE type;
    wxString name, text, value, range, def, require;
    int defidx;
    bool hide;
    wxCheckBox *check;
    wxWindow *ctrl;
    FULLSPEC_FIELD *req_field;

    FULLSPEC_FIELD():
         type(ftHEADER), name(""), text(""), value(""), range(""), def(""), defidx(0/*wxNOT_FOUND*/), hide(false), check(NULL), ctrl(NULL), req_field(NULL) {};

    FULLSPEC_FIELD(FULLSPEC_FIELD_TYPE t, wxString n, wxString x, wxString v, wxString r, wxString d, wxString q, bool h):
         type(t), name(n), text(x), value(v), range(r), def(d), require(q), defidx(0/*wxNOT_FOUND*/), hide(h), check(NULL), ctrl(NULL), req_field(NULL) {};

    FULLSPEC_FIELD(wxString x):
         type(ftHEADER), name(""), text(x), value(""), range(""), def(""), require(""), defidx(0/*wxNOT_FOUND*/), hide(false), check(NULL), ctrl(NULL), req_field(NULL) {};
};

struct FULLSPEC_FILE
{
    uint8_t version;
    wxString id, checkval, composite, display, matches, prefix, separator, test;
    wxString *body;
    wxRegEx *rx_match;
    std::vector<FULLSPEC_FIELD> fields;
    //FULLSPEC_FILE() : version({0,0}), composite(""), display(""), matches(""), prefix(""), separator(""), body(NULL), rx_match(), fields() {};
    //bool valid() { return }
};

class FFQFullSpec: public wxDialog
{
	public:

		static int FindFullSpec(wxString for_codec);
		static int FindFullSpecID(wxString for_codec);
		static void Finalize();
		static void Initialize();

		FFQFullSpec(wxWindow* parent, wxWindowID id = wxID_ANY);
		virtual ~FFQFullSpec();
		bool Execute(int full_spec_index, wxString &cmd);
		//FFQ_STATIC_JOB* GetTestJob();


		//(*Declarations(FFQFullSpec)
		wxBoxSizer* BtnSizer;
		wxButton* CancelBtn;
		wxButton* OkBtn;
		wxButton* TestBtn;
		wxFileDialog* FileDlg;
		wxFlexGridSizer* MainSizer;
		wxScrolledWindow* ScrollWin;
		//*)

	protected:

		//(*Identifiers(FFQFullSpec)
		static const long ID_SCROLLWIN;
		static const long ID_TESTBTN;
		static const long ID_OKBTN;
		static const long ID_CANCELBTN;
		//*)

	private:

		static bool s_Initialized;
		static std::vector<FULLSPEC_FILE> s_Files;
		static int FullSpecFileFromString(wxString &spec);
		static void FullSpecsFromDir(wxString dir);

		FULLSPEC_FILE *m_File;
		wxString m_CmdLine, m_TestFile;
		wxFlexGridSizer *m_CtrlSizer;
		wxArrayString m_UpdateStack;
		wxFont m_HeaderFont;
		bool m_CtrlsCreated;
		bool m_FirstIdle;
		bool m_FirstShow;

		void AddFieldCtrl(FULLSPEC_FIELD &field);
		bool GetCmdLine(wxString &res);
		wxString GetFieldValue(FULLSPEC_FIELD &field);
		int  IndexOfField(wxString field_name);
		void MakeControls();
		void ParseCmd(wxString cmd);
		void ParseSpecBody();
		void UpdateControls();
		void UpdateField(FULLSPEC_FIELD &field);
		bool ValidateRequires();

		void OnCommand(wxCommandEvent &evt);
		void OnIdle(wxIdleEvent &evt);
		void OnShow(wxShowEvent &evt);

		//(*Handlers(FFQFullSpec)
		void ActionClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

wxString GetCmdToken(wxString &from);

#endif
