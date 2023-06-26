/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQTaskBar.cpp                                                  *
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

#include "FFQTaskBar.h"

//---------------------------------------------------------------------------------------

#ifdef __WINDOWS__

//Following line is a hack:
//const DWORD SHGFP_TYPE_DEFAULT = 0;
//An alternate way is to uncomment the following line (and comment the above):
//#define _WIN32_IE 0x0500
//which will cause the typedef enum to be available in shlobj.h

#include <objbase.h>
#include <shobjidl.h>
#include <shlobj.h>

const GUID CLSID_FFQ_TaskbarList = {0x56fdf344, 0xfd6d, 0x11d0, { 0x95, 0x8a, 0x00, 0x60, 0x97, 0xc9, 0xa0, 0x90 }};

#endif // __WINDOWS__

//---------------------------------------------------------------------------------------

wxString GetProgramFolder(bool x86)
{

    wxString res;

    #ifdef __WINDOWS__

    wchar_t *buf = new wchar_t[MAX_PATH + 1];
    if (SHGetFolderPathW(0, x86 ? CSIDL_PROGRAM_FILESX86 : CSIDL_PROGRAM_FILES, NULL, /*SHGFP_TYPE_DEFAULT*/0, buf) == S_OK)
        res = wxString(buf);
    delete[] buf;

    #endif // __WINDOWS__

    return res;

}

//---------------------------------------------------------------------------------------

FFQTaskBar::FFQTaskBar()
{

    m_WndHandle = 0;
    m_State = tbsNORMAL;

#ifdef __WINDOWS__

    m_CoUninit = (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) == S_OK);
    m_TaskbarList = NULL;
    CoCreateInstance(CLSID_FFQ_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (LPVOID*)&m_TaskbarList);

#endif // __WINDOWS__
}

//---------------------------------------------------------------------------------------

FFQTaskBar::~FFQTaskBar()
{

    SetTaskBarState(tbsNORMAL);

#ifdef __WINDOWS__

    if (m_TaskbarList != NULL)
    {
        ((ITaskbarList3*)m_TaskbarList)->Release();
        m_TaskbarList = NULL;
    }

    if (m_CoUninit)
    {
        CoUninitialize();
        m_CoUninit = false;
    }

#endif // __WINDOWS__
}

//---------------------------------------------------------------------------------------

void FFQTaskBar::SetWindowHandle(WXWidget handle)
{
    m_WndHandle = handle;
}

//---------------------------------------------------------------------------------------

TASKBAR_STATE FFQTaskBar::GetTaskBarState()
{
    return m_State;
}

//---------------------------------------------------------------------------------------

void FFQTaskBar::SetTaskBarState(TASKBAR_STATE state)
{

    if (m_State == state) return;

#ifdef __WINDOWS__

    if (m_TaskbarList != NULL) ((ITaskbarList3*)m_TaskbarList)->SetProgressState((HWND)m_WndHandle, (TBPFLAG)state);

#endif // __WINDOWS__

    m_State = state;

}

//---------------------------------------------------------------------------------------

void FFQTaskBar::SetTaskBarProgress(unsigned char percent_done)
{
    SetTaskBarProgress(percent_done, 100);
}

//---------------------------------------------------------------------------------------

void FFQTaskBar::SetTaskBarProgress(unsigned int position, unsigned int total)
{
#ifdef __WINDOWS__

    if (m_TaskbarList != NULL)
    {
        ((ITaskbarList3*)m_TaskbarList)->SetProgressValue((HWND)m_WndHandle, position, total);
        m_State = tbsPROGRESS;
    }
    if (position == 0) SetTaskBarState(tbsNORMAL);

#endif // __WINDOWS__
}
