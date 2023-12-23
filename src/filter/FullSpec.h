/*************************************************************************
*                                                                        *
*  Copyright (c) 2023 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FullSpec.h                                                    *
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

#ifndef FULLSPEC_H
#define FULLSPEC_H

#include "FilterBasePanel.h"
#include "../FFQFullSpec.h"

class FullSpec : public FilterBasePanel
{

    public:

        FullSpec(wxWindow* parent, void *data);
        virtual ~FullSpec();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:
        FULLSPEC_FILE *m_FullSpec;
        bool m_HasTime;

};

#endif // AUDIODELAY_H
