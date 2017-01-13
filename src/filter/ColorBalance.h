/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: ColorBalance.h                                                  *
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

#ifndef COLORBALANCE_H
#define COLORBALANCE_H

#include "FilterBasePanel.h"

class ColorBalance : public FilterBasePanel
{

    public:

        ColorBalance(wxWindow* parent);
        virtual ~ColorBalance();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        int m_AddID;

        wxSlider *m_ShadowR, *m_ShadowG, *m_ShadowB,
                 *m_MidtoneR, *m_MidtoneG, *m_MidtoneB,
                 *m_LightR, *m_LightG, *m_LightB;

        wxCheckBox *m_LockGroups, *m_LockColors;

        void MakeGroup(wxSizer *sizer, wxString caption, wxSlider **red, wxSlider **green, wxSlider **blue);
        void MakeRow(wxSizer *sizer, wxString caption, wxSlider **slider);

        void OnCtrlChange(wxCommandEvent &event);

};

#endif // COLORBALANCE_H
