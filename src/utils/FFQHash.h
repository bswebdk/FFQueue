/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQHash.h                                                       *
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

#ifndef FFQHASH_H
#define FFQHASH_H

#include <wx/string.h>

//SHA1 context
typedef struct SHA1_Context {

 uint32_t hash[5], hi, lo, index;
 char  buffer[64];

} SHA1_Context;

//Self test to ensure correct byte alignment
bool SHA1_SelfTest();

//Initialize a context
void SHA1_Init(SHA1_Context* ctxt);

//Append a buffer of data to the context
void SHA1_Append(SHA1_Context* ctxt, void* buf, uint32_t len);

//Flush the context to finalize hashing
void SHA1_Finish(SHA1_Context* ctxt);

//Hash a file
bool SHA1_File(wxString filename, SHA1_Context* sha1);

//Hash a buffer
void SHA1_Buffer(SHA1_Context* sha1, void* buf, uint32_t len);

//Convert a context to a hex string
void SHA1_ToHex(SHA1_Context* sha1, wxString &hex);

#endif //FFQHASH_H
