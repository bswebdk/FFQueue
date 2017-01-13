/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Deinterlace.h                                                   *
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

#ifndef DEINTERLACE_H
#define DEINTERLACE_H

#include "FilterBasePanel.h"

class Deinterlace : public FilterBasePanel
{

    public:

        Deinterlace(wxWindow* parent);
        virtual ~Deinterlace();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        wxPanel *m_YadifPan, *m_KernDeintPan;
        wxRadioButton *m_Yadif, *m_KernDeint;

        //Yadif
        wxChoice *m_DeintMode;
        wxRadioButton *m_ParityAuto, *m_ParityTop, *m_ParityBottom, *m_FramesAll, *m_FramesInterlaced;

        //Kerndeint
        wxSlider *m_Threshold;
        wxStaticText *m_ThreshLab;
        wxCheckBox *m_MapToWhite, *m_SwapOrder, *m_AddSharp, *m_TwoWaySharp;

        void OnAction(wxCommandEvent &event);
        void SwapPanels(wxPanel *select = NULL);

};

#endif // DEINTERLACE_H
