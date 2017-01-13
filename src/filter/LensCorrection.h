/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: LensCorrection.h                                                *
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

#ifndef LENSCORRECTION_H
#define LENSCORRECTION_H

#include "FilterBasePanel.h"

class LensCorrection : public FilterBasePanel
{

    public:

        LensCorrection(wxWindow* parent);
        virtual ~LensCorrection();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        wxSlider *m_CX, *m_CY, *m_K1, *m_K2;
        wxStaticText *m_CXText, *m_CYText, *m_K1Text, *m_K2Text;

        void OnSliderChange(wxCommandEvent &event);

};

#endif // LENSCORRECTION_H
