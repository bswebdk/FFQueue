/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQFilter.h                                                     *
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

#ifndef FFQFILTER_H
#define FFQFILTER_H

#include <wx/string.h>

//Used to separate filter strings
extern const wxString FILTER_SEPARATOR;// = "[|]";

//Used for formatting filters
extern const wxString FILTER_VIDEO_IN;// = "<VIDIN>";
extern const wxString FILTER_VIDEO_OUT;// = "<VIDOUT>";
extern const wxString FILTER_AUDIO_IN;// = "<AUDIN>";
extern const wxString FILTER_AUDIO_OUT;// = "<AUDOUT>";
extern const wxString FILTER_SUBTITLE_IN;// = "<SUBIN>";
extern const wxString FILTER_SUBTITLE_OUT;// = "<SUBOUT>";
extern const wxString FILTER_REQUIRED_IN;// = "<REQIN>";
extern const wxString FILTER_UNIQUE_ID;// = "<UID%u>";

//Number of integrated filters
extern const unsigned int FILTER_COUNT;// = 44;

//Enum of all integrated filters
typedef enum FILTER_TYPE { ftCOLOROVERLAY,
                           ftDEINTERLACE,
                           ftCROP,
                           ftSCALE,
                           ftSUBSBURNIN,
                           ftDELOGO,
                           ftWATERMARK,
                           ftFLIP,
                           ftDEJUDDER,
                           ftDESHAKE,
                           ftBOXBLUR,
                           ftCOLORBALANCE,
                           ftPAD,
                           ftROTATE,
                           ftUNSHARP,
                           ftVIGNETTE,
                           ftVOLUME_AND_TONE,
                           ftECHO,
                           ftPHASER,
                           ftFLANGER,
                           ftHIGHPASS,
                           ftLOWPASS,
                           ftBANDPASS,
                           ftBANDREJECT,
                           ftEQUALIZER,
                           ftCOLORMATRIX,
                           ftCURVES,
                           ftCHANNELMAP,
                           ftCUSTOM,
                           ftRESAMPLE,
                           ftEARWAX,
                           ftNEGATE,
                           ftELBG,
                           ftADELAY,
                           ftATEMPO,
                           ftDECIMATE,
                           ftINTERLACE,
                           ftFRAMESTEP,
                           ftHQDN3D,
                           ftHUE,
                           ftLENSCORRECTION,
                           ftNOISE,
                           ftSWAPUV,
                           ftPERSPECTIVE,
                           ftTRANSPOSE,
                           ftDYNAUDNORM,
                           ftUNKNOWN=0xffff } FILTER_TYPE;


//The actual filter names used to verify support for the individual filters
extern const wxString FILTER_NAMES[];

//The order in which the filters appear in the menu. A better way of sorting
//filters should be established in the future.
extern const unsigned int FILTER_ORDER_COUNT;
extern const int FILTER_ORDER[];


//Unused functions for now
//bool IsFilterVideo(FILTER_TYPE f);
//bool IsFilterAudio(FILTER_TYPE f);


//Structure to define a file required by a filter
typedef struct REQUIRED_FILE
{

    wxString path, tag, loop;

    REQUIRED_FILE();
    REQUIRED_FILE(wxString from);
    bool IsValid();
    void Reset();
    wxString ToString();

} REQUIRED_FILE;


//Structure to define a filter
typedef struct FFMPEG_FILTER
{

    FILTER_TYPE type;
    wxString friendly, ff_filter, editable;
    REQUIRED_FILE required_file;

    FFMPEG_FILTER();
    FFMPEG_FILTER(wxString fltr);

    bool IsAudio();
    bool IsComplex();
    bool IsEditable();
    bool IsSubtitle();
    bool IsVideo();
    void Reset();
    wxString ToString();

} FFMPEG_FILTER, *LPFFMPEG_FILTER;

#endif // FFQFILTER_H
