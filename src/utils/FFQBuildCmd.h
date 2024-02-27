/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQBuildCmd.h                                                   *
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

#ifndef FFQBUILDCMD_H
#define FFQBUILDCMD_H

#include <wx/string.h>
#include "FFQJob.h"
//#include "FFQThumbs.h"

//Constants used for building command lines
extern const wxString CODEC_TYPE_VIDEO;
extern const wxString CODEC_TYPE_AUDIO;
extern const wxString CODEC_TYPE_SUBTITLE;
extern const wxString CODEC_TYPE_DATA;
extern const wxString CODEC_TYPE_ATTACHMENT;
extern const wxString CODEC_COPY;
extern const wxString CODEC_SUBS_BURNIN;

//Encoder presets
extern const unsigned int ENCODER_PRESET_COUNT;
extern const wxString ENCODER_PRESETS[];

//Video tuning
extern const unsigned int ENCODER_TUNINGS_COUNT;
extern const wxString ENCODER_TUNINGS[];

//Disposition
extern const unsigned int DISPOSITION_COUNT;
extern const wxString DISPOSITIONS[];

//Used to flag if previewing a command is safe
extern bool IsPreviewSafe;

//Function interfaces
void CommandLineReplace(wxString &cmd, const wxString old_str, const wxString new_str);
//int IndexOfStreamByType(wxString stream_map, wxString codec_type, int file_id, int stream_id);
wxString BuildCommandLine(LPFFQ_JOB job, long &encoding_pass, bool for_preview = false, bool audio_filters_complex = false);
void CleanupFinishedJob(LPFFQ_JOB job);
wxString EscapeFilterString(wxString s, bool un_escape = false);
wxString FormatFileName(wxString fn);
//bool FormatCuts(FFQ_CUTS &cuts, wxString &stream_tag, bool video, TIME_VALUE duration, int &filter_id);
wxString FormatCuts(wxArrayPtrVoid &streams, int &filter_id, wxArrayInt *unique_files_only = 0);
bool FormatFilter(wxString &filter, wxString &vid_in, wxString &aud_in, wxString &sub_in, wxString &req_in, int &filter_id);
bool FormatStreamRate(wxString &rate, bool video, wxString stream_tag = wxEmptyString);


#endif // FFQBUILDCMD_H
