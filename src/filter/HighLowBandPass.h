/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: HighLowBandPass.h                                               *
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

#ifndef HIGHLOWBANDPASS_H
#define HIGHLOWBANDPASS_H

#include "FilterBasePanel.h"

extern const unsigned int BAND_WIDTH_TYPE_COUNT;
extern const wxString WIDTH_TYPES[];

class HighLowBandPass : public FilterBasePanel
{

    public:

        HighLowBandPass(wxWindow* parent);
        virtual ~HighLowBandPass();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        FILTER_TYPE m_FilterType;
        wxFlexGridSizer *m_Sizer;
        wxStaticText *m_MainLab, *m_PolesLab, *m_WidthLab;
        wxTextCtrl *m_Frequency, *m_Width;
        wxChoice *m_Poles, *m_WidthType;
        wxCheckBox *m_CSG;

        void OnChoiceChange(wxCommandEvent &event);
        void UpdateControls();

};

#endif // HIGHLOWBANDPASS_H
