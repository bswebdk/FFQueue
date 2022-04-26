/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FilterBasePanel.h                                               *
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

#ifndef FILTERBASEPANEL_H
#define FILTERBASEPANEL_H

#include "../utils/FFQLang.h"
#include "../FFQTimeEdit.h"
#include "../utils/FFQConfig.h"
#include "../utils/FFQMisc.h"
#include "../utils/FFQConst.h"
#include "../utils/FFQFilter.h"

#include <wx/string.h>
#include <wx/panel.h>
#include <wx/window.h>
#include <wx/filedlg.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/hyperlink.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/combobox.h>
#include <wx/valnum.h>

class FilterBasePanel : public wxPanel
{

    public:

        FilterBasePanel(wxWindow* parent);
        virtual ~FilterBasePanel();

        virtual void SetFilter(LPFFMPEG_FILTER fltr) = 0; //Abstract
        virtual bool GetFilter(LPFFMPEG_FILTER fltr) = 0; //Abstract

        static FilterBasePanel* GetFilterPanel(wxWindow* parent, FILTER_TYPE ft);

    protected:

        //Creates the next unique filter id <UID[n]>
        wxString NextFilterUID();

        //Change control padding
        void SetControlPadding(unsigned int pad);
        void SetControlParent(wxWindow *parent);

        //Used to browse for a file
        bool BrowseForFile(wxString &file);

        //Fast label-creator
        wxStaticText* MakeLabel(wxString text, wxSizer *addToSizer = NULL, int addSizerFlags = 0);
        wxFlexGridSizer* MakeSliderLabels(wxSizer *addToSizer, wxString l1, wxString l2, wxString l3, bool vert);

        //Functionality for enabling and disabling stock controls
        void EnableLeftAndTop(bool enable);
        void EnableWidthAndHeight(bool enable);

        //Functionality for time limit links
        bool EditTimeValue(TIME_VALUE &tv, bool allowMillis = false);
        wxSizer* GetTimeLimitControls(bool AddLabelThis = false);
        wxSizer* GetTimeLimitControls(wxSizer *AppendToSizer, int AddSizerFlags = 0, bool SetAsSizer = false);
        bool GetTimeLimitFilter(wxString &str, bool colonFirst = true);
        void GetTimeLimitFriendly(wxString &from, wxString &to);
        void GetTimeLimitFriendly(wxString &fromAndTo);
        bool GetTimeLimitValues(TIME_VALUE &from, TIME_VALUE &to);
        bool GetTimeLimitValues(uint64_t &from, uint64_t &to, bool asSeconds = true);
        void SetTimeLimitValues(TIME_VALUE &from, TIME_VALUE &to);
        void SetTimeLimitValues(uint64_t &from, uint64_t &to, bool isSeconds = true);
        void SetTimeLimitValues(wxString &str, bool isSeconds = true);
        void OnTimeLinkClick(wxHyperlinkEvent &event);
        void UpdateTimeLimitLinks();

        //Functionality for position editing
        wxSizer* GetLeftAndTopControls(wxValidator &val);
        void GetLeftAndTopValues(int &left, int &top);
        void SetLeftAndTopValues(int left, int top);
        void SetLeftAndTopValues(wxString &from);

        //Functionality for dimension editing
        wxSizer* GetWidthAndHeightControls(wxValidator &val);
        bool GetWidthAndHeightValues(int &width, int &height, bool required = false);
        bool GetWidthAndHeightValues(wxString &width, wxString &height, bool required = false);
        void SetWidthAndHeightValues(int width, int height);
        void SetWidthAndHeightValues(wxString &from);
        void SetWidthAndHeightValues(wxString width, wxString height);

        //Combined functionality for position and size
        //void GetLeftTopWidthHeightValues(int &left, int &top, int &width, int &height);
        //void SetLeftTopWidthHeightValues(int left, int top, int width, int height);
        //void SetLeftTopWidthHeightValues(wxString &from);

    private:

        //NextFilterID()
        unsigned int m_FilterUID, m_Padding;

        //Control parent
        wxWindow *m_CtrlParent;

        //Open file handling
        wxFileDialog *m_FileDlg;

        //Time limit functions
        FFQTimeEdit* m_TimeEditor;
        wxGenericHyperlinkCtrl *m_ShowTime1, *m_ShowTime2;
        wxFlexGridSizer *m_TimeSizer;
        TIME_VALUE m_Time1, m_Time2;

        //Left and top functions
        wxFlexGridSizer *m_PosSizer;
        wxTextCtrl *m_Left, *m_Top;

        //Width and height functions
        wxFlexGridSizer *m_DimSizer;
        wxTextCtrl *m_Width, *m_Height;

};

#endif // FILTERBASEPANEL_H
