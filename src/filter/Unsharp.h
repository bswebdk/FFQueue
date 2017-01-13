/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: Unsharp.h                                                       *
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

#ifndef UNSHARP_H
#define UNSHARP_H

#include "FilterBasePanel.h"

class Unsharp : public FilterBasePanel
{

    public:

        Unsharp(wxWindow* parent);
        virtual ~Unsharp();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        wxSlider *m_LumaX, *m_LumaY, *m_LumaP, *m_ChromaX, *m_ChromaY, *m_ChromaP;
        wxCheckBox *m_LumaLock, *m_ChromaLock, *m_PowerLock, *m_OpenCL;
        wxString *m_Labels;

        void OnCtrlChange(wxCommandEvent &event);
        void MakeSizeGroup(wxSizer *sizer, wxString caption, wxSlider **sX, wxSlider **sY, wxCheckBox **cL, bool power);

};

#endif // UNSHARP_H
