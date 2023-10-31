/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPreset.h                                                     *
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

#ifndef FFQPRESET_H
#define FFQPRESET_H

#include <wx/string.h>
#include <wx/arrstr.h>
#include "FFQUniqueID.h"
#include "FFQThumbs.h"

//The FFQ_PRESET structure
typedef struct FFQ_PRESET
{

    UNIQUE_ID preset_id; //The id that identifies the preset

    bool is_temporary, //If true the preset is not saved on app exit
         two_pass, //Two pass encoding
         two_pass_null, //Use null target file for fist pass in two pass encoding
         af_complex, //True if audio filters should be added to "-filter_complex" rather than "-af"
         mf_faststart, //True if -movflags faststart should be declared
         keep_filetime, //True if the output file shall get the source file's time
         shortest; //True if -shortest is used

    uint8_t skip_encode_same; //Skip encoding to same codec; 0 = off, 1 = for audio, 2 = for video, 3 = for both

    wxString preset_name, //Name of the preset
             video_codec, //The video codec
             video_codec_id, //The identifier of the video codec (eg. h264, hevc, mpeg)
             video_rate, //The video bit rate settings
             frame_rate, //Video frame rate
             video_sync, //video synchronization
             video_quality, //Video quality settings
             key_frames, //Key frame detection settings
             hw_decode, //Settings for hardware decoding of video
             audio_codec, //Audio codec
             audio_codec_id, //The identifier of the audio codec (eg. mp2, mp3, ac3)
             audio_rate, //Audio bit rate settings
             audio_quality, //Audio quality settings
             audio_channels, //Number of audio channels
             audio_profile, //Audio profile
             meta_data, //Global meta data to embed in target file
             meta_data_v, //Video specific meta data
             meta_data_a, //Audio specific meta data
             meta_data_s, //Subtitle specific meta data
             pixel_format, //The pixel format to use
             trellis, //Trellis settings
             threads, //Number of threads to use
             speed_preset, //The video speed preset (x264)
             video_tuning, //The tuning to be used (x264)
             aspect_ratio, //Video aspect ratio
             output_format, //Output format
             custom_args_1, //Custom command line arguments for 1st pass
             custom_args_2, //As "custom_args_1" but for second pass
             disposition, //Video,Audio,Subtitle disposition
             finger_print, //Used to auto detect preset (see FFQJobEditAdv::GetPresetFingerPrint)
             fullspec_vid, //Storing any full spec settings for video
             fullspec_aud; //Storing any full spec settings for audio

    wxArrayString filters; //The video and audio filters

    //Audio and video FOURCC for AVI files
    struct fourcc {
        wxString vids, auds;
    } fourcc;

    //Subtitle settings
    struct subtitles {

        bool bitmap; //True if subtitles are bitmap based
        short size_type, //0=Default, 1=PAL, 2=custom
              scale; //Scale subtitles (only for bitmap based subs)
        wxString width, //Custom width (size_type == 2)
                 height, //Custom height (size_type == 2)
                 codec, //Subtitle codec
                 charenc; //Character encoding

        //wxString ToString();

    } subtitles;

    //Segmenting settings
    struct segmenting {
        unsigned int length; //Length of each segment, 0 = segmenting disabled
        uint8_t length_type; //0=Seconds, 1=Minutes, 2=Hours
        uint8_t list_file; //0=None, 1=Flat, 2=CSV, 3=FFconcat, 4=M3U8
        bool break_bframes; //Break B-frames
        bool incremental_tc; //Use incremental time code
        bool streaming; //Make streaming format
        bool reset_ts; //Reset time stamps
    } segmenting;

    //Settings for thumbs
    THUMBS_AND_TILES thumbs;

    FFQ_PRESET();
    FFQ_PRESET(wxString &preset);

    wxString GetFilters(wxUniChar separator = '\n');
    wxString GetFormatExtension(wxString current);
    void Reset(bool newPreset);
    wxString SubsString();
    wxString ToString();

} FFQ_PRESET, *LPFFQ_PRESET;

#endif // FFQPRESET_H
