/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Noise.h                                                         *
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

#ifndef NOISE_H
#define NOISE_H

#include "FilterBasePanel.h"

#define COMPONENT_COUNT 5

typedef struct NOISE_SEED
{

    wxTextCtrl *val;
    wxCheckBox *use;

} NOISE_SEED;

//---------------------------------------------------------------------------------------

typedef struct NOISE_STRENGTH
{

    wxSlider *val;
    wxCheckBox *use;
    wxStaticText *show;

} NOISE_STRENGTH;

//---------------------------------------------------------------------------------------

typedef struct NOISE_FLAGS
{

    wxCheckBox *a, *p, *t, *u, *use;

} NOISE_FLAGS;

//---------------------------------------------------------------------------------------

class Noise : public FilterBasePanel
{

    public:

        Noise(wxWindow* parent);
        virtual ~Noise();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        NOISE_SEED m_Seed[COMPONENT_COUNT];
        NOISE_STRENGTH m_Strength[COMPONENT_COUNT];
        NOISE_FLAGS m_Flags[COMPONENT_COUNT];

        void UpdateControls();
        void OnCtrlChange(wxCommandEvent &event);


};

#endif // NOISE_H
