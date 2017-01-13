/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQCompress.h                                                   *
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

#ifndef FFQCOMPRESS_H
#define FFQCOMPRESS_H

#include <cstddef>
#include <wx/string.h>

void CompressAndBase64(const void* src_buf, const size_t src_len, wxString &base64, size_t wrap_len = 0);
void CompressAndBase64(wxString &src_and_dst, size_t wrap_len = 0);
bool DecompressFromBase64(wxString &base64, void* dest_buf, size_t* dest_len);
bool DecompressFromBase64(wxString &base64);


#endif // FFQCOMPRESS_H
