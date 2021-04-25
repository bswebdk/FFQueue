/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConst.cpp                                                    *
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

#include "FFQConst.h"

/*bool STRBOOLDEF(wxString sval, bool def)
{
    return sval.Len() == 0 ? def : STRBOOL(sval);
}*/

//---------------------------------------------------------------------------------------

uint32_t COLOR_BLACK  = 0x01000000;
uint32_t COLOR_BLUE   = 0x02000000;
uint32_t COLOR_GRAY   = 0x03000000;
uint32_t COLOR_GREEN  = 0x04000000;
uint32_t COLOR_ORANGE = 0x05000000;
uint32_t COLOR_RED    = 0x06000000;

uint32_t DEFAULT_COLORS[COLOR_COUNT]      = { 0x000000, 0xff0000, 0x777777, 0x007700, 0x0066ff, 0x0000ff };
uint32_t DEFAULT_COLORS_DARK[COLOR_COUNT] = { 0xffffff, 0xff5555, 0x888888, 0x337733, 0x0066ff, 0x3333ff };

//---------------------------------------------------------------------------------------

