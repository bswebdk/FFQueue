/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Scale.h                                                         *
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

#ifndef SCALE_H
#define SCALE_H

#include "FilterBasePanel.h"

class Scale : public FilterBasePanel
{

    public:

        Scale(wxWindow* parent);
        virtual ~Scale();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        wxCheckBox *m_Percent, *m_FitRect, *m_AccurateRnd, *m_FullChromaInt, *m_FullChromaInp, *m_BitExact, *m_NoValid;
        wxChoice *m_Algorithm, *m_Dither, *m_Interlace, *m_ClrSpaceIn, *m_ClrSpaceOut, *m_SampleRngIn, *m_SampleRngOut;

        void OnCheckBox(wxCommandEvent &event);

};

#endif // COLOR_H
