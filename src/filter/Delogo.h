/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Delogo.h                                                        *
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

#ifndef DELOGO_H
#define DELOGO_H

#include "FilterBasePanel.h"

class Delogo : public FilterBasePanel
{

    public:

        Delogo(wxWindow* parent);
        virtual ~Delogo();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        wxFlexGridSizer *m_DelogoCtrls;
        wxCheckBox *m_UseImgMask;
        wxTextCtrl *m_Blur, *m_ImgMask;
        wxButton *m_BrowseImg;

        void OnAction(wxCommandEvent &event);
        void UpdateControls();

};

#endif // DELOGO_H
