/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQPreset.cpp                                                   *
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

#include "FFQPreset.h"
#include "../utils/FFQLang.h"
#include "FFQFilter.h"
#include "FFQConfig.h"
#include "FFQMisc.h"
#include "FFQConst.h"

//Preset constants for saving / loading
const wxString PRESET_ID = "id";
const wxString PRESET_NAME = "name";
const wxString PRESET_VIDEO_CODEC = "video_codec";
const wxString PRESET_VIDEO_RATE = "video_rate";
const wxString PRESET_VIDEO_SYNC = "video_sync";
const wxString PRESET_VIDEO_QUALITY = "video_quality";
const wxString PRESET_FRAME_RATE = "frame_rate";
const wxString PRESET_KEY_FRAMES = "key_frames";
const wxString PRESET_HW_DECODE = "hw_decode";
const wxString PRESET_TWO_PASS = "two_pass";
const wxString PRESET_TWO_PASS_NULL = "two_pass_null";
const wxString PRESET_TRELLIS = "trellis";
const wxString PRESET_THREADS = "threads";
const wxString PRESET_SPEED_PRESET = "speed_preset";
const wxString PRESET_AUDIO_CODEC = "audio_codec";
const wxString PRESET_AUDIO_RATE = "audio_rate";
const wxString PRESET_AUDIO_QUALITY = "audio_quality";
const wxString PRESET_AUDIO_CHANNELS = "audio_channels";
const wxString PRESET_AUDIO_PROFILE = "audio_profile";
const wxString PRESET_AUDIO_RESAMPLE = "audio_resample";
const wxString PRESET_AUDIO_FILTERS_COMPLEX = "af_complex";
const wxString PRESET_FASTSTART = "faststart";
const wxString PRESET_KEEP_FILETIME = "keep_filetime";
const wxString PRESET_METADATA = "metadata";
const wxString PRESET_PIXEL_FMT = "pixel_fmt";
const wxString PRESET_ASPECT = "aspect_ratio";
const wxString PRESET_OUTPUT_FORMAT = "output_format";
const wxString PRESET_CUSTOM_ARGS = "custom_args";
const wxString PRESET_CUSTOM_ARGS_2 = "custom_args_2";
const wxString PRESET_SUBTITLE = "subtitle";
const wxString PRESET_FOURCC = "fourcc";
const wxString PRESET_THUMBS = "thumbs";
const wxString PRESET_FILTER = "filter";
const wxString PRESET_FINGER_PRINT = "finger_print";
const wxString PRESET_SEGMENTING = "segmenting";


//---------------------------------------------------------------------------------------

FFQ_PRESET::FFQ_PRESET()
{

    //Default constructor - reset values to new preset with default values
    Reset(true);

}

//---------------------------------------------------------------------------------------

FFQ_PRESET::FFQ_PRESET(wxString &preset)
{

    //Constructor that parses values from a string

    //Reset to empty preset
    Reset(false);

    //Handy variables
    wxString n, v, a_resample = "";

    while (preset.Len() > 0)
    {

        //Get a line
        v = StrTrim(GetToken(preset, '\n'));

        //Because there might be another preset in the "preset" argument
        //we exit when a blank line is discovered
        if (v.Len() == 0) return;

        //Split line into name and value
        n = GetToken(v, '=');

        //Check the name and set the correct value
        if (n == PRESET_ID) preset_id = UNIQUE_ID(v);
        else if (n == PRESET_NAME) preset_name = v;
        else if (n == PRESET_VIDEO_CODEC) video_codec = v;
        else if (n == PRESET_VIDEO_RATE) video_rate = v;
        else if (n == PRESET_FRAME_RATE) frame_rate = v;
        else if (n == PRESET_KEY_FRAMES) key_frames = v;
        else if (n == PRESET_HW_DECODE) hw_decode = v;
        else if (n == PRESET_VIDEO_SYNC) video_sync = v;
        else if (n == PRESET_VIDEO_QUALITY) video_quality = v;
        else if (n == PRESET_TWO_PASS) two_pass = (v == STR_YES);
        else if (n == PRESET_TWO_PASS_NULL) two_pass_null = (v == STR_YES);
        else if (n == PRESET_AUDIO_CODEC) audio_codec = v;
        else if (n == PRESET_AUDIO_RATE) audio_rate = v;
        else if (n == PRESET_AUDIO_QUALITY) audio_quality = v;
        else if (n == PRESET_AUDIO_CHANNELS) audio_channels = v;
        else if (n == PRESET_AUDIO_PROFILE) audio_profile = v;
        else if (n == PRESET_AUDIO_RESAMPLE) a_resample = v; //This is for backwards compatibility
        else if (n == PRESET_AUDIO_FILTERS_COMPLEX) af_complex = (v == STR_YES);
        else if (n == PRESET_FASTSTART) mf_faststart = (v == STR_YES);
        else if (n == PRESET_KEEP_FILETIME) keep_filetime = (v == STR_YES);
        else if (n == PRESET_METADATA) meta_data = v;
        else if (n == PRESET_PIXEL_FMT) pixel_format = v;
        else if (n == PRESET_TRELLIS) trellis = v;
        else if (n == PRESET_THREADS) threads = v;
        else if (n == PRESET_SPEED_PRESET) speed_preset = v;
        else if (n == PRESET_ASPECT) aspect_ratio = v;
        else if (n == PRESET_OUTPUT_FORMAT) output_format = v;
        else if (n == PRESET_CUSTOM_ARGS) custom_args_1 = v;
        else if (n == PRESET_CUSTOM_ARGS_2) custom_args_2 = v;
        else if (n == PRESET_FINGER_PRINT) finger_print = v;
        else if (n == PRESET_SUBTITLE)
        {

            //Subtitle settings
            subtitles.bitmap = GetToken(v, ',') == STR_YES;
            subtitles.size_type = Str2Long(GetToken(v, ','), 0);
            subtitles.scale = Str2Long(GetToken(v, ','), 100);
            subtitles.width = GetToken(v, ',');
            if (Str2Long(subtitles.width, -1) < 0) subtitles.width.Clear(); //Validate bad-values (might have been set as codec)
            subtitles.height = GetToken(v, ',');
            if (Str2Long(subtitles.height, -1) < 0) subtitles.height.Clear(); //Same as width
            subtitles.codec = GetToken(v, ',');
            subtitles.charenc = v;

        }
        else if (n == PRESET_FOURCC)
        {

            //FourCC settings
            fourcc.vids = GetToken(v, ',');
            fourcc.auds = v;

        }
        else if (n == PRESET_SEGMENTING)
        {

            //Segmenting settings
            segmenting.length = Str2Long(GetToken(v, ','), 0);
            segmenting.length_type = Str2Long(GetToken(v, ','), 0);
            segmenting.list_file = Str2Long(GetToken(v, ','), 0);
            segmenting.break_bframes = GetToken(v, ',') == STR_YES;
            segmenting.incremental_tc = GetToken(v, ',') == STR_YES;
            segmenting.streaming = GetToken(v, ',') == STR_YES;
            segmenting.reset_ts = GetToken(v, ',') == STR_YES;

        }
        else if (n == PRESET_THUMBS) thumbs = THUMBS_AND_TILES(v);
        else if (n == PRESET_FILTER) filters.Add(v);
        //else ThrowError("Invalid preset NVP: \""+n+"\"=\""+v+"\"");
    }

    if (a_resample.Len() > 0)
    {

        //Convert old audio_resample to filter
        FFMPEG_FILTER f = FFMPEG_FILTER();
        f.type = ftRESAMPLE;
        f.ff_filter = FILTER_AUDIO_IN + "aresample=" + a_resample + FILTER_AUDIO_OUT;
        f.editable = a_resample;
        f.friendly = FFQL()->FILTER_NAMES[ftRESAMPLE] + "=" + a_resample;
        filters.Add(f.ToString());

    }

    if ( (video_quality.Len() == 0) && (video_rate.Freq(',') == 1) && (video_rate.AfterLast(',') == "6") )
    {

        //Convert old qscale
        float qs = Str2Long(video_rate.BeforeFirst(','), 5);
        video_quality = "1," + ToStr(froundi(qs / 31 * 100.0));
        video_rate = "1000,1";

    }

}

//---------------------------------------------------------------------------------------

void FFQ_PRESET::Reset(bool new_preset)
{

    //Reset the values of the preset to either a new preset with
    //default values (when a new preset is created) or an empty,
    //undeclared preset (for parsing a preset from config file)

    preset_id = UNIQUE_ID();

    is_temporary = false;
    two_pass = false;
    two_pass_null = false;
    af_complex = false;
    mf_faststart = false;
    keep_filetime = false;

    preset_name = "";
    video_codec = new_preset ? "libx264" : "";
    video_rate = new_preset ? "1000,1" : "";
    video_sync = "";
    video_quality = "";
    key_frames = "";
    hw_decode = "";
    audio_codec = new_preset ? "mp3" : "";
    audio_rate = new_preset ? "128,1" : "";
    audio_quality = "";
    audio_channels = "";
    audio_profile = "";
    frame_rate = "";
    meta_data = "";
    pixel_format = "";
    trellis = "";
    threads = "";
    speed_preset = "";
    aspect_ratio = "";
    output_format = "";
    custom_args_1 = "";
    custom_args_2 = "";
    finger_print = "";

    fourcc.vids = "";
    fourcc.auds = "";

    //Changing default values for SUBS might cause problems!
    subtitles.bitmap = false;
    subtitles.size_type = 0;
    subtitles.scale = 100;
    subtitles.width = "";
    subtitles.height = "";
    subtitles.codec = "copy";
    subtitles.charenc = "";

    //Nullify segmenting
    memset(&segmenting, 0, sizeof(segmenting));

    thumbs = THUMBS_AND_TILES();

}

//---------------------------------------------------------------------------------------

wxString FFQ_PRESET::SubsString()
{

    //Pack subtitle values to string
    wxString res;
    res.Printf(
        "%s,%i,%i,%s,%s,%s,%s",
        BOOLSTR(subtitles.bitmap), (int)subtitles.size_type, (int)subtitles.scale,
        subtitles.width, subtitles.height, subtitles.codec, subtitles.charenc
    );

    //Return the packed values
    return res;
}

//---------------------------------------------------------------------------------------

wxString FFQ_PRESET::ToString()
{

    //Pack the values of the preset to a string

    //Handy variables
    wxString res, subs, segm, tbs;

    //Used to test the preset for default values
    FFQ_PRESET default_pst = FFQ_PRESET();

    //Get the subtitle settings
    subs = SubsString();
    //If subtitle settings are default they are dismissed
    if (subs == default_pst.SubsString()) subs.Clear();

    //Segmenting
    segm.Printf("%u,%u,%u,%s,%s,%s,%s",
        segmenting.length,
        segmenting.length_type,
        segmenting.list_file,
        BOOLSTR(segmenting.break_bframes),
        BOOLSTR(segmenting.incremental_tc),
        BOOLSTR(segmenting.streaming),
        BOOLSTR(segmenting.reset_ts)
    );

    //Get the thumb settings
    tbs = thumbs.ToString();
    //As with subs we discard default settings
    if (tbs == default_pst.thumbs.ToString()) tbs.Clear();

    //Make the lines of key value pairs that defines the preset
    res = PRESET_ID + "=" + preset_id.ToString() + CRLF +
          PRESET_NAME + "=" + preset_name + CRLF;

    //Empty values are not saved in order to reduce the size of the presets
    if (video_codec.Len() > 0) res += PRESET_VIDEO_CODEC + "=" + video_codec + CRLF;
    if (video_rate.Len() > 0) res += PRESET_VIDEO_RATE + "=" + video_rate + CRLF;
    if (frame_rate.Len() > 0) res += PRESET_FRAME_RATE + "=" + frame_rate + CRLF;
    if (key_frames.Len() > 0) res += PRESET_KEY_FRAMES + "=" + key_frames + CRLF;
    if (hw_decode.Len() > 0) res += PRESET_HW_DECODE + "=" + hw_decode + CRLF;
    if (video_sync.Len() > 0) res += PRESET_VIDEO_SYNC + "=" + video_sync + CRLF;
    if (video_quality.Len() > 0) res += PRESET_VIDEO_QUALITY + "=" + video_quality + CRLF;
    if (two_pass) res += PRESET_TWO_PASS + "=" + STR_YES + CRLF;
    if (two_pass_null) res += PRESET_TWO_PASS_NULL + "=" + STR_YES + CRLF;
    if (audio_codec.Len() > 0) res += PRESET_AUDIO_CODEC + "=" + audio_codec + CRLF;
    if (audio_rate.Len() > 0) res += PRESET_AUDIO_RATE + "=" + audio_rate + CRLF;
    if (audio_quality.Len() > 0) res += PRESET_AUDIO_QUALITY + "=" + audio_quality + CRLF;
    if (audio_channels.Len() > 0) res += PRESET_AUDIO_CHANNELS + "=" + audio_channels + CRLF;
    if (audio_profile.Len() > 0) res += PRESET_AUDIO_PROFILE + "=" + audio_profile + CRLF;
    if (af_complex) res += PRESET_AUDIO_FILTERS_COMPLEX + "=" + STR_YES + CRLF;
    if (mf_faststart) res += PRESET_FASTSTART + "=" + STR_YES + CRLF;
    if (keep_filetime) res += PRESET_KEEP_FILETIME + "=" + STR_YES + CRLF;
    if (meta_data.Len() > 0) res += PRESET_METADATA + "=" + meta_data + CRLF;
    if (pixel_format.Len() > 0) res += PRESET_PIXEL_FMT + "=" + pixel_format + CRLF;
    if (trellis.Len() > 0) res += PRESET_TRELLIS + "=" + trellis + CRLF;
    if (threads.Len() > 0) res += PRESET_THREADS + "=" + threads + CRLF;
    if (speed_preset.Len() > 0) res += PRESET_SPEED_PRESET + "=" + speed_preset + CRLF;
    if (aspect_ratio.Len() > 0) res += PRESET_ASPECT + "=" + aspect_ratio + CRLF;
    if (output_format.Len() > 0) res += PRESET_OUTPUT_FORMAT + "=" + output_format + CRLF;
    if (custom_args_1.Len() > 0) res += PRESET_CUSTOM_ARGS + "=" + custom_args_1 + CRLF;
    if (custom_args_2.Len() > 0) res += PRESET_CUSTOM_ARGS_2 + "=" + custom_args_2 + CRLF;
    if (finger_print.Len() > 0) res += PRESET_FINGER_PRINT + "=" + finger_print + CRLF;
    if (subs.Len() > 0) res += PRESET_SUBTITLE + "=" + subs + CRLF;
    if (segm.Len() > 0) res += PRESET_SEGMENTING + "=" + segm + CRLF;
    if (fourcc.vids.Len() + fourcc.auds.Len() > 0) res += PRESET_FOURCC + "=" + fourcc.vids + "," + fourcc.auds + CRLF;
    if (tbs.Len() > 0) res += PRESET_THUMBS + "=" + tbs + CRLF;

    //Remove last CRLF
    res.RemoveLast(CRLF.Len());

    //Append filters
    for (unsigned int i = 0; i < filters.size(); i++) res += CRLF + PRESET_FILTER + "=" + filters.Item(i);

    //Return the packed preset
    return res;

}
