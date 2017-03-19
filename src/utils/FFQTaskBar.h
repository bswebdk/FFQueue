/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQTaskBar.h                                                    *
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

#ifndef FFQTASKBAR_H
#define FFQTASKBAR_H

#include <wx/string.h>
#include <wx/window.h>

//#ifdef __WINDOWS__
//#include <objbase.h>
//#include <shobjidl.h>
//#include <shlobj.h>
//#endif // __WINDOWS__

typedef enum TASKBAR_STATE
{
    tbsNORMAL = 0x00,
    tbsMARQUEE = 0x01,
    tbsPROGRESS = 0x02,
    tbsERRORED = 0x04,
    //tbsMARQUEE_ERRORED = 0x05,
    tbsPAUSED = 0x08,
    //tbsMARQUEE_PAUSED = 0x09,
} TASKBAR_STATE;

wxString GetProgramFolder(bool x86);

class FFQTaskBar
{

    public:

        FFQTaskBar();
        ~FFQTaskBar();

        void SetWindowHandle(WXWidget handle);
        TASKBAR_STATE GetTaskBarState();
        void SetTaskBarState(TASKBAR_STATE state);
        void SetTaskBarProgress(unsigned char percent_done);
        void SetTaskBarProgress(unsigned int position, unsigned int total);

    protected:

    private:

        WXWidget m_WndHandle;
        TASKBAR_STATE m_State;

#ifdef __WINDOWS__
        bool m_CoUninit;
        void* m_TaskbarList;
#endif // __WINDOWS__

};

#endif // FFQTASKBAR_H
