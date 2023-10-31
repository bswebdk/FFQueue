/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: VolumeAndTone.h                                                 *
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

#ifndef VOLUMEANDTONE_H
#define VOLUMEANDTONE_H

#include "FilterBasePanel.h"

class VolumeAndTone : public FilterBasePanel
{

    public:

        VolumeAndTone(wxWindow* parent);
        virtual ~VolumeAndTone();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:
        wxSlider *m_Volume, *m_Bass, *m_Treble;
        wxStaticText *m_LVolume, *m_LBass, *m_LTreble;

        void OnSliderChange(wxCommandEvent &event);
        void UpdateLabels();

};

#endif // VOLUMEANDTONE_H
