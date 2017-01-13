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

/*
//Channel identification numbers
const unsigned short CH_FL      = 0; //Front left
const unsigned short CH_FR      = 1; //Front right
const unsigned short CH_FC      = 2; //Front center
const unsigned short CH_LFE     = 3; //Low frequency
const unsigned short CH_BL      = 4; //Back left
const unsigned short CH_BR      = 5; //Back right
const unsigned short CH_FLC     = 6; //Front left-of-center
const unsigned short CH_FRC     = 7; //Front right-of-center
const unsigned short CH_BC      = 8; //Back center
const unsigned short CH_SL      = 9; //Side left
const unsigned short CH_SR      = 10; //Side right
const unsigned short CH_TC      = 11; //Top center
const unsigned short CH_TFL     = 12; //Top front left
const unsigned short CH_TFC     = 13; //Top front center
const unsigned short CH_TFR     = 14; //Top front right
const unsigned short CH_TBL     = 15; //Top back left
const unsigned short CH_TBC     = 16; //Top back center
const unsigned short CH_TBR     = 17; //Top back right
const unsigned short CH_DL      = 18; //Downmix left
const unsigned short CH_DR      = 19; //Downmix right
const unsigned short CH_WL      = 20; //Wide left
const unsigned short CH_WR      = 21; //Wide right
const unsigned short CH_SDL     = 22; //Surround direct left
const unsigned short CH_SDR     = 23; //Surround direct right
const unsigned short CH_LFE2    = 24; //Low frequency 2

const size_t CHANNEL_COUNT = 25;

const wxString CHANNEL_NAMES[CHANNEL_COUNT] = {
    "FL Front Left",
    "FR Front Right",
    "FC Front Center",
    "LFE Low Frequency Effects",
    "BL Back Left",
    "BR Back Right",
    "FLC Front Left-of-Center",
    "FRC Front Right-of-Center",
    "BC Back Center",
    "SL Side Left",
    "SR Side Right",
    "TC Top Center",
    "TFL Top Front Left",
    "TFC Top Front Center",
    "TFR Top Front Right",
    "TBL Top Back Left",
    "TBC Top Back Center",
    "TBR Top Back Right",
    "DL Downmix Left",
    "DR Downmix Right",
    "WL Wide Left",
    "WR Wide Right",
    "SDL Surround Direct Left",
    "SDR Surround Direct Right",
    "LFE2 Low Frequency Effects 2"
};

//Channels layout strings are [layout name] space [2-digit channel id numbers from above]
const size_t CHANNEL_LAYOUT_COUNT = 26;
const wxString CHANNEL_LAYOUTS[CHANNEL_LAYOUT_COUNT] = {
                "Mono 02", //FC
                "Stereo 0001", //FL+FR
                "2.1 000103", //FL+FR+LFE
                "3.0 000102", //FL+FR+FC
                "3.0(back) 000108", //FL+FR+BC
                "4.0 00010208", //FL+FR+FC+BC
                "Quad 00010405", //FL+FR+BL+BR
                "Quad(side) 00010910", //FL+FR+SL+SR
                "3.1 00010203", //FL+FR+FC+LFE
                "5.0 0001020405", //FL+FR+FC+BL+BR
                "5.0(side) 0001020910", //FL+FR+FC+SL+SR
                "4.1 0001020308", //FL+FR+FC+LFE+BC
                "5.1 000102030405", //FL+FR+FC+LFE+BL+BR
                "5.1(side) 000102030910", //FL+FR+FC+LFE+SL+SR
                "6.0 000102080910", //FL+FR+FC+BC+SL+SR
                "6.0(front) 000106070910", //FL+FR+FLC+FRC+SL+SR
                "Hexagonal 000102040508", //FL+FR+FC+BL+BR+BC
                "6.1 00010203080910", //FL+FR+FC+LFE+BC+SL+SR
                "6.1(back) 00010203040508", //FL+FR+FC+LFE+BL+BR+BC
                "7.0 00010204050910", //FL+FR+FC+BL+BR+SL+SR
                "7.0(front) 00010206070910", //FL+FR+FC+FLC+FRC+SL+SR
                "7.1 0001020304050910", //FL+FR+FC+LFE+BL+BR+SL+SR
                "7.1(wide) 0001020304050607", //FL+FR+FC+LFE+BL+BR+FLC+FRC
                "7.1(wide-side) 0001020306070910", //FL+FR+FC+LFE+FLC+FRC+SL+SR
                "Octagonal 0001020405080910", //FL+FR+FC+BL+BR+BC+SL+SR
                "Downmix 1819" //DL+DR
};
*/


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
        bool FindChannelCtrls(size_t channelId, wxStaticText **label, wxChoice **choice);
        size_t GetChannelCount();
        void GetChannelCtrls(size_t channelIndex, wxStaticText **label, wxChoice **choice);
        wxString GetRemappings(bool forFFMpeg);
        void OnChoiceChange(wxCommandEvent& event);


};

#endif // CHANNELMAP_H
