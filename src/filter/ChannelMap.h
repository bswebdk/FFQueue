/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: ChannelMap.h                                                    *
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

#ifndef CHANNELMAP_H
#define CHANNELMAP_H

#include "FilterBasePanel.h"

class ChannelMap : public FilterBasePanel
{

    public:

        ChannelMap(wxWindow* parent);
        virtual ~ChannelMap();

        virtual void SetFilter(LPFFMPEG_FILTER fltr);
        virtual bool GetFilter(LPFFMPEG_FILTER fltr);

    protected:

    private:

        wxFlexGridSizer *m_Remappings;
        wxChoice *m_Layout;

        void EnableRemappings(int mapId);
        void EnableRemappings(wxString channelList);
        bool FindChannelCtrls(unsigned int channelId, wxStaticText **label, wxChoice **choice);
        unsigned int GetChannelCount();
        void GetChannelCtrls(unsigned int channelIndex, wxStaticText **label, wxChoice **choice);
        wxString GetRemappings(bool forFFMpeg);
        void OnChoiceChange(wxCommandEvent& event);


};

#endif // CHANNELMAP_H
