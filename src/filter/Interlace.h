/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Interlace.h                                                     *
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

#ifndef INTERLACE_H
#define INTERLACE_H

#include "FilterBasePanel.h"

class Interlace : public FilterBasePanel
{

    public:

        Interlace(wxWindow* parent);
        virtual ~Interlace();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:


        //Type selection
        wxRadioButton *m_Default, *m_Temporal;
        wxPanel *m_DefaultPan, *m_TemporalPan;

        //Default
        wxRadioButton *m_ScanEven, *m_ScanOdd;
        wxCheckBox *m_LowPass; //Used for both

        //Temporal
        wxChoice *m_TemporalMode;

        void OnAction(wxCommandEvent &event);
        void SwapPanels(wxPanel *select);


};

#endif // INTERLACE_H
