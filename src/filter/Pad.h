/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Pad.h                                                           *
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

#ifndef PAD_H
#define PAD_H

#include "FilterBasePanel.h"
#include <wx/clrpicker.h>

class Pad : public FilterBasePanel
{

    public:

        Pad(wxWindow* parent);
        virtual ~Pad();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:
        wxColourPickerCtrl *m_Color;
        wxRadioButton *m_UseColor, *m_UseBlur;
        wxFlexGridSizer *m_BlurSizer;
        wxSlider *m_Blur;
        bool m_BlurOK;

        void OnRadioClick(wxCommandEvent &event);
        void UpdateControls();

};

#endif // PAD_H
