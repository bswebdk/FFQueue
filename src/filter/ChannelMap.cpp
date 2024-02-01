/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: ChannelMap.cpp                                                  *
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

#include "ChannelMap.h"

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

const unsigned int CHANNEL_COUNT = 25;

//---------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------

//Channels layout strings are [layout name] space [2-digit channel id numbers from above]
const unsigned int CHANNEL_LAYOUT_COUNT = 26;
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

//---------------------------------------------------------------------------------------

wxChoice* MakeChSel(wxWindow *parent, size_t srcChannel)
{
    wxChoice *res = new wxChoice(parent, wxID_ANY);
    res->Freeze();
    res->SetClientData((void*)srcChannel);
    for (unsigned int i = 0; i < CHANNEL_COUNT; i++) res->Append(CHANNEL_NAMES[i].AfterFirst(' '));
    res->SetSelection(srcChannel);
    res->Thaw();
    return res;
}

//---------------------------------------------------------------------------------------

ChannelMap::ChannelMap(wxWindow* parent) : FilterBasePanel(parent)
{

    //Find used channels
    bool *usedChannels = new bool[CHANNEL_COUNT];
    memset(usedChannels, 0, sizeof(bool) * CHANNEL_COUNT);
    int usedCount = 0, idx;

    for (unsigned int i = 0; i < CHANNEL_LAYOUT_COUNT; i++)
    {
        wxString s = CHANNEL_LAYOUTS[i];
        GetToken(s, SPACE); //Remove layout name
        while (s.Len() > 0)
        {
            idx = Str2Long(s.Left(2), 0);
            if (!usedChannels[idx])
            {
                usedCount++;
                usedChannels[idx] = true;
            }
            s.Remove(0, 2);
        }
    }

    wxFlexGridSizer *fgs1 = new wxFlexGridSizer(1, 2, 0, 0), *fgs2;
    wxStaticBoxSizer *sbs;

    fgs1 = new wxFlexGridSizer(2, 1, 0, 0);
    fgs1->AddGrowableCol(0);

        fgs2 = new wxFlexGridSizer(1, 2, 0, 0);
        fgs2->AddGrowableCol(1);

        MakeLabel(FFQS(SID_CHMAP_CHANNEL_LAYOUT), fgs2);
        m_Layout = new wxChoice(this, 1);
        for (unsigned int i = 0; i < CHANNEL_LAYOUT_COUNT; i++)
        {
            wxString s = CHANNEL_LAYOUTS[i];
            m_Layout->Append(GetToken(s, ' '));
        }
        fgs2->Add(m_Layout, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

    fgs1->Add(fgs2, 1, wxALL|wxEXPAND, 3);

        m_Remappings = new wxFlexGridSizer(usedCount+2, 3, 0, 0);
        m_Remappings->AddGrowableCol(2);

        MakeLabel(FFQS(SID_CHMAP_DESTINATION_CHANNEL), m_Remappings);
        m_Remappings->Add(10, -1, 1);
        MakeLabel(FFQS(SID_CHMAP_SOURCE_CHANNEL), m_Remappings);

        m_Remappings->Add(-1, 5, 1);
        m_Remappings->Add(-1, 5, 1);
        m_Remappings->Add(-1, 5, 1);

        for (unsigned int i = 0; i < CHANNEL_COUNT; i++) if (usedChannels[i])
        {
            MakeLabel(CHANNEL_NAMES[i].AfterFirst(' '), m_Remappings);
            m_Remappings->Add(10, -1, 1);
            m_Remappings->Add(MakeChSel(this, i), 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 2);
        }

        sbs = new wxStaticBoxSizer(wxVERTICAL, this, FFQS(SID_CHMAP_CHANNEL_REMAPPINGS));
        sbs->Add(m_Remappings, 1, wxALL|wxEXPAND, 3);

    fgs1->Add(sbs, 1, wxALL|wxEXPAND, 3);

    SetSizer(fgs1);
    fgs1->Fit(this);
    //fgs1->SetSizeHints(this);

    Connect(1, wxEVT_CHOICE, (wxObjectEventFunction)&ChannelMap::OnChoiceChange);
    //Connect(2, wxEVT_CHOICE, (wxObjectEventFunction)&Curves::OnChoiceChange);

}

//---------------------------------------------------------------------------------------

ChannelMap::~ChannelMap()
{

}

//---------------------------------------------------------------------------------------

void ChannelMap::SetFilter(LPFFMPEG_FILTER fltr)
{

    wxString fs = fltr->editable;

    if (fs.Len() > 0) m_Layout->SetSelection(Str2Long(GetToken(fs, ','), 0)); //Stored value

    else m_Layout->SetSelection(0); //Default value

    EnableRemappings(m_Layout->GetSelection());

    wxChoice *ch;
    unsigned int chId, mapTo;

    while (fs.Len() > 0)
    {
        chId = Str2Long(GetToken(fs, ','), 0);
        mapTo = Str2Long(GetToken(fs, ','), 0);
        if (FindChannelCtrls(chId, NULL, &ch)) ch->SetSelection(mapTo);
    }

}

//---------------------------------------------------------------------------------------

bool ChannelMap::GetFilter(LPFFMPEG_FILTER fltr)
{

    fltr->type = ftCHANNELMAP;

    wxString ffmpeg = GetRemappings(true), user = ffmpeg;

    user.Replace("-", "->");
    user.Replace("|", ", ");

    fltr->friendly = FFQSF(SID_CHMAP_USERFRIENDLY, FFQL()->FILTER_NAMES[fltr->type], user);
    fltr->ff_filter.Printf("%schannelmap=map=%s:channel_layout=%s%s", FILTER_AUDIO_IN, ffmpeg, m_Layout->GetStringSelection().Lower(), FILTER_AUDIO_OUT);
    fltr->editable.Printf("%i,%s", m_Layout->GetSelection(), GetRemappings(false));

    return true;

}

//---------------------------------------------------------------------------------------

void ChannelMap::EnableRemappings(int mapId)
{
    if ((mapId < 0) || (mapId >= (int)CHANNEL_LAYOUT_COUNT)) mapId = 0;
    EnableRemappings(CHANNEL_LAYOUTS[mapId].AfterFirst(' '));
}

//---------------------------------------------------------------------------------------

void ChannelMap::EnableRemappings(wxString channelList)
{

    Freeze();

    wxChoice *ch;
    wxStaticText *st;

    for (unsigned int i = 0; i < GetChannelCount(); i++)
    {
        GetChannelCtrls(i, &st, &ch);
        st->Enable(false);
        ch->Enable(false);
    }

    while (channelList.Len() > 0)
    {
        if (FindChannelCtrls(Str2Long(channelList.Left(2), 0), &st, &ch))
        {
            st->Enable(true);
            ch->Enable(true);
        }
        channelList.Remove(0, 2);
    }

    Thaw();

}

//---------------------------------------------------------------------------------------

bool ChannelMap::FindChannelCtrls(unsigned int channelId, wxStaticText **label, wxChoice **choice)
{
    wxChoice *ch;
    wxStaticText *st;
    for (unsigned int i = 0; i < GetChannelCount(); i++)
    {
        GetChannelCtrls(i, &st, &ch);
        if ((size_t)ch->GetClientData() == channelId)
        {
            if (label) *label = st;
            if (choice) *choice = ch;
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------

unsigned int ChannelMap::GetChannelCount()
{
    return m_Remappings->GetRows() - 2; //Headers and spacers subtracted
}

//---------------------------------------------------------------------------------------

void ChannelMap::GetChannelCtrls(unsigned int channelIndex, wxStaticText **label, wxChoice **choice)
{
    unsigned int ctrlOffs = (channelIndex + 2) * 3;
    if (label) *label = (wxStaticText*)m_Remappings->GetItem(ctrlOffs)->GetWindow();
    if (choice) *choice = (wxChoice*)m_Remappings->GetItem(ctrlOffs + 2)->GetWindow();
}

//---------------------------------------------------------------------------------------

wxString ChannelMap::GetRemappings(bool forFFMpeg)
{

    wxString res = "";
    wxChoice *ch;

    for (unsigned int i = 0; i < GetChannelCount(); i++)
    {

        GetChannelCtrls(i, NULL, &ch);

        if (ch->IsEnabled())
        {

            unsigned int chId = (size_t)ch->GetClientData(), mapTo = (unsigned int)ch->GetSelection();

            if (forFFMpeg)
            {
                res += CHANNEL_NAMES[mapTo].BeforeFirst(' ');
                /*if (chId != mapTo)*/ res += "-" + CHANNEL_NAMES[chId].BeforeFirst(' ');
                res += "|";
            }

            else res += ToStr(chId) + "," + ToStr(mapTo) + ",";

        }

    }

    if (res.Len() > 0) res.Remove(res.Len() - 1, 1);
    return res;

}

//---------------------------------------------------------------------------------------

void ChannelMap::OnChoiceChange(wxCommandEvent& event)
{
    event.Skip();
    if (event.GetEventObject() == m_Layout) EnableRemappings(m_Layout->GetSelection());
}
