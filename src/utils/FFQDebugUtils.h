/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQDebugUtils.h                                                 *
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

#ifndef FFQDEBUGUTILS_H
#define FFQDEBUGUTILS_H

#include <wx/string.h>

//Convert files and buffera to arrays - only used in the debug version
wxString BufToArray(unsigned char *buf, int len, wxString array_name, unsigned int indent = 8);
wxString FileToArray(wxString filename, wxString array_name, unsigned int indent = 8);

//Make binary resources ./bin_res.*
bool MakeBinaryResources();

//Create Makefile
wxString CreateMakefiles(wxString cbp_path = wxEmptyString);

#endif // FFQDEBUGUTILS_H
