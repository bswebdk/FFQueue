/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQCodecInfo.h                                                  *
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

#ifndef FFQCODECINFO_H
#define FFQCODECINFO_H

#include <wx/string.h>

//Minimum and maximum quantizer values
extern const int QUANTIZER_BEST_QUALITY;
extern const int QUANTIZER_WORST_QUALITY;

//Default codec constants
extern const wxString DEFAULT_VIDEO_CODEC_INFO;
extern const wxString DEFAULT_AUDIO_CODEC_INFO;

//The CODEC_INFo structure
typedef struct CODEC_INFO
{

    bool friendly, qscale_float, crf_float;
    wxString codec;
    float min_qscale, max_qscale, min_crf, max_crf;
    CODEC_INFO *next;

    CODEC_INFO();
    CODEC_INFO(wxString &from);
    ~CODEC_INFO();

    void Append(wxString &from);
    void CopyTo(CODEC_INFO &dst);
    CODEC_INFO* Find(const wxString find_codec);
    bool GetConstRate(int pct, float *result);
    wxString GetConstRateStr(int pct);
    bool GetQScale(int pct, float *result);
    wxString GetQScaleStr(int pct);
    bool IsDefault(wxString *packed = NULL);
    void Parse(wxString &from);
    void Reset();
    wxString ToString();

} CODEC_INFO, *LPCODEC_INFO;

//---------------------------------------------------------------------------------------

//Helper functions
LPCODEC_INFO DeleteCodecInfo(LPCODEC_INFO root, LPCODEC_INFO del);
LPCODEC_INFO MakeDefaultCodecInfo(LPCODEC_INFO root = nullptr);

//---------------------------------------------------------------------------------------

#endif // FFQCODECINFO_H
