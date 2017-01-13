/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Equalizer.h                                                     *
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

#ifndef EQUALIZER_H
#define EQUALIZER_H

#include "FilterBasePanel.h"

typedef struct EQ_BAND
{

    wxSlider *gain;
    wxStaticText *label;
    int freq;

    EQ_BAND() { gain=NULL; freq=0; }

} EQ_BAND, *LPEQ_BAND;

//---------------------------------------------------------------------------------------

class Equalizer : public FilterBasePanel
{

    public:

        Equalizer(wxWindow* parent);
        virtual ~Equalizer();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        LPEQ_BAND m_EqBands;
        wxTextCtrl *m_BandWidth;
        wxChoice *m_WidthType;
        void MakeEqBand(wxSizer *addToSizer, EQ_BAND &band, int f, float w);
        void OnSliderChange(wxCommandEvent &event);
        void UpdateLabels(wxSlider *forSlider = NULL);

};

#endif // EQUALIZER_H
