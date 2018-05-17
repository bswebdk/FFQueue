/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQBuildCmd.cpp                                                 *
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

#include "FFQBuildCmd.h"
#include "FFQConfig.h"
#include "FFQFilter.h"
#include "FFQConst.h"
#include "FFQMisc.h"
#include "FFQStreamMapping.h"
#include "../FFQPresetMgr.h"
//#include "FFQProbing.h"
#include <wx/filename.h>

//---------------------------------------------------------------------------------------

//Constant that declares the name used for multipass log files
const wxString FILENAME_MULTIPASSLOG = "ffqueue_ffmpeg_multipass";

//---------------------------------------------------------------------------------------

//extern declarations

const wxString CODEC_TYPE_VIDEO = "video";
const wxString CODEC_TYPE_AUDIO = "audio";
const wxString CODEC_TYPE_SUBTITLE = "subtitle";
const wxString CODEC_COPY = "copy";
const wxString CODEC_SUBS_BURNIN = "BURN-IN";

const unsigned int ENCODER_PRESET_COUNT = 9;
const wxString ENCODER_PRESETS[ENCODER_PRESET_COUNT] = {

    "ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow"

};

//---------------------------------------------------------------------------------------
//Private helper functions
//---------------------------------------------------------------------------------------

wxString BitRateToArg(long bit_rate, long br_type, wxString ff_arg, bool video)
{

    //This function converts a single bit rate setting into a command line argument

    //Zero bit_rate means unused arg
    if (bit_rate <= 0) return "";

    //Prepare stream specifier
    wxString av = video ? "v" : "a", rate;

    //Convert bit_rate to a string that is defined by br_type
    if (br_type == 0) rate << bit_rate; // bits/sec

    else if (br_type == 1) rate << bit_rate << 'k'; // kbits/sec

    else if (br_type == 2) rate << (bit_rate * 1000) << 'k'; // mbits/sec

    else if (br_type == 3) rate << (long)(bit_rate * 8); // bytes/sec

    else if (br_type == 4) rate << (long)(bit_rate * 8) << 'k'; // kbytes/sec

    else if (br_type == 5) rate << (long)(bit_rate * 8000) << 'k'; // mbytes/sec

    else if (br_type == 6)
    {

        //This is not used anymore but kept for backwards compatibility
        if (ff_arg == "b")
        {
            rate << bit_rate;
            ff_arg = "qscale";
        }

        else return "";

    }

    else return ""; //Bad bit rate type

    //Return the command line argument
    return "-" + ff_arg + ":" + av + " " + rate + " ";

}

//---------------------------------------------------------------------------------------

void CommandLineReplace(wxString &cmd, const wxString old_str, const wxString new_str)
{

    //Replaces one of the FFQ command line variables
    //with other content and ensures that blank spaces
    //are inserted before and after

    //Find the old string
    int idx = cmd.Lower().Find(old_str);

    if (idx >= 0)
    {

        //Remove any blank chars before the old string
        while ((idx > 0) && (cmd.GetChar(idx-1) == ' '))
        {
            cmd.Remove(idx - 1, 1);
            idx--;
        }

        //Remove any blank chars after the old string
        unsigned int delp = idx + old_str.Len();
        while ((delp < cmd.Len()) && (cmd.GetChar(delp) == ' ')) cmd.Remove(delp, 1);

        //Replace the old with the new
        if (idx > 0) cmd.Replace(old_str, " " + new_str); //Ensure a single blank character
        else cmd.Replace(old_str, new_str);

        //Trim blanks
        cmd.Trim(true);

    }

}

//---------------------------------------------------------------------------------------

wxString FormatSingleQualityArg(wxString &from, const wxString cmd, float min, float max, float &qval, bool is_float)
{

    //Used by the two following functions to convert a single
    //setting to a command line argument by mapping a percentage (0..100)
    //to the range defined by min..max. The min/max values are usually
    //defined in CODEC_INFO's

    bool b = GetToken(from, ",") == STR_YES; //Use?
    float v = Str2Long(GetToken(from, ","), INT_MIN); //Percent

    if (b && (v > INT_MIN) && (max != min)) //Validate
    {

        qval = ConvertPctToMinMaxFloat(v, min, max); //Convert

        //Float or int command line argument?
        if (is_float) return "-" + cmd + " " + wxString::Format("%.2f", qval) + " "; //Float
        else return "-" + cmd + " " + wxString::Format("%i", froundi(qval)) + " "; //Int

    }

    return ""; //Nope, not used!

}

//---------------------------------------------------------------------------------------

bool FormatAudioQuality(wxString &aq, const wxString &codec)
{

    //Convert audio quality settings to command line arguments
    wxString res;

    //Get codec information
    CODEC_INFO *ci = FFQCFG()->GetCodecInfo()->Find(codec);
    if (ci == NULL) ci = FFQCFG()->GetCodecInfo()->Find(DEFAULT_AUDIO_CODEC_INFO);
    float val;

    //Oh yeah! More formatting!
    res  = FormatSingleQualityArg(aq, "q:a", ci->min_qscale, ci->max_qscale, val, ci->qscale_float);

    //Heeeeeeeere's JOHNNY!!
    aq = res;
    return aq.Len() > 0;

}

//---------------------------------------------------------------------------------------

bool FormatVideoQuality(wxString &vq, const wxString &codec)
{

    //Convert the video quality settings to command line arguments
    wxString res;

    //Get codec information
    CODEC_INFO *ci = FFQCFG()->GetCodecInfo()->Find(codec);
    if (ci == NULL) ci = FFQCFG()->GetCodecInfo()->Find(DEFAULT_VIDEO_CODEC_INFO);
    float qmin, qmax;

    //Format da shit!
    res  = FormatSingleQualityArg(vq, "q:v", ci->min_qscale, ci->max_qscale, qmin, ci->qscale_float);
    res += FormatSingleQualityArg(vq, "crf", ci->min_crf, ci->max_crf, qmin, ci->crf_float);
    res += FormatSingleQualityArg(vq, "crf_max", ci->min_crf, ci->max_crf, qmin, ci->crf_float);
    res += FormatSingleQualityArg(vq, "qmin", QUANTIZER_WORST_QUALITY, QUANTIZER_BEST_QUALITY, qmin, false);
    res += FormatSingleQualityArg(vq, "qmax", QUANTIZER_WORST_QUALITY, QUANTIZER_BEST_QUALITY, qmax, false);
    res += FormatSingleQualityArg(vq, "qdiff", 0, qmax - qmin, qmin, false);

    //Booya!
    vq = res;
    return vq.Len() > 0;

}

//---------------------------------------------------------------------------------------

void GetPassLogFileName(LPFFQ_JOB job, wxString &plf)
{

    //For convenience
    wxUniChar psep = wxFileName::GetPathSeparator();

    //Get temp path
    plf = FFQCFG()->GetTmpPath(job->out.BeforeLast(psep));

    //Make sure it end with a path-separator
    if ((plf.Len() > 0) && (plf.Right(1) != psep)) plf += psep;

    //Add name of passlogfile
    plf += FILENAME_MULTIPASSLOG;

}

//---------------------------------------------------------------------------------------

bool MakeKeyframeArguments(wxString &kf)
{

    //Converts the comma separated values from the preset editors
    //key frame page to command line arguments

    //Handy variables
    wxString res = "", s, t;
    int v;

    //Min key frame dist
    s = GetToken(kf, ",");
    if (s.Len() > 0) res += "-keyint_min " + s + " ";

    //Max key frame dist
    s = GetToken(kf, ",");
    if (s.Len() > 0) res += "-g " + s + " ";

    //Motion estimation method
    s = GetToken(kf, ",");
    if (s.Len() > 0) res += "-me_method " + s + " ";

    //Scene change sensibility.
    //NOTE: Some websites claims that this value has the range from
    //-1000000000 to 1000000000 (higher value, less sensitivity) but
    //I have experimented with it and found the actual range to be in
    //percent meaning 0 = disabled, 1..100 is the percent of image change
    //(higher value is again less sensitivity) that triggers a scene change.
    s = GetToken(kf, ",");
    t = GetToken(kf, ",");
    if (s == STR_YES)
    {
        v = Str2Long(t, -1);
        if (v >= 0) res += "-sc_threshold " + ( v == 0 ? "0" : ToStr(100-v) ) + " ";
    }

    //Maximum consecutive B-frames
    v = Str2Long(GetToken(kf, ","), 0);
    if (v > 0) res += "-bf " + ToStr(v - 2) + " ";

    //Maximum reference frames for P-frames
    v = Str2Long(GetToken(kf, ","), 0);
    if (v > 0) res += "-refs " + ToStr(v - 1) + " ";

    //Use closed GOP
    if (GetToken(kf, ",") == STR_YES) res += "-flags +cgop ";

    //Set result and return success
    kf = res;
    return kf.Len() > 0;

}

//---------------------------------------------------------------------------------------

wxString MakeSubtitleBurninFilter(wxString sub_in, int sub_stream_idx, wxString sub_src_file, LPFFQ_PRESET preset)
{

    //Return empty string if the filter is not used
    if ((preset->subtitles.codec != CODEC_SUBS_BURNIN) || (sub_in == "")) return "";

    //Handy variables
    wxString res, s, sw, sh;

    if (preset->subtitles.size_type != 0)
    {

        //Subtitle size
        sw = preset->subtitles.size_type == 1 ? "720" : preset->subtitles.width;
        sh = preset->subtitles.size_type == 1 ? "576" : preset->subtitles.height;

    }

    //If size is undefined, use ffmpeg variables for input width / height
    if (sw.Len() == 0) sw = "iw";
    if (sh.Len() == 0) sh = "ih";

    if (preset->subtitles.bitmap)
    {

        //Overlay bitmap type subtitles
        if (preset->subtitles.size_type != 0)
        {

            //Crop to selected size before scaling / overlaying
            s.Printf("crop=%s:%s:0:0[SUBF1];", sw, sh);
            res = sub_in + s;
            sub_in = "[SUBF1]";

        }

        if (preset->subtitles.scale < 100)
        {

            //Scale subtitles
            float scale = preset->subtitles.scale / 100.0;
            s.Printf("%sscale=%s*%g:-1[SUBF2];", sub_in, sw, scale);
            sub_in = "[SUBF2]";
            res += s;

        }

        //Overlay subtitles
        s.Printf("%s%soverlay=main_w/2-overlay_w/2:main_h-overlay_h%s", FILTER_VIDEO_IN, sub_in, FILTER_VIDEO_OUT);

        //Return the filter
        return res + s;

    }
    else
    {

        //Burn in text type subtitles, ALERT: if font.conf is not available to ffmpeg, this will fail!

        //Get subtitle extension
        wxString ext = sub_src_file.AfterLast('.').Lower();

        //Escape subtitle file name
        sub_src_file = EscapeFilterString(sub_src_file);

        //Resize required?
        bool rs = (sw != "iw") || (sh != "ih");

        //Choose the correct filter type
        if ( ((ext == "ssa") || (ext == "ass")) && (sub_stream_idx == 0) && (!rs) ) res.Printf("ass=%s", sub_src_file);
        else
        {
            res.Printf("subtitles=%s", sub_src_file);

            //Character encoding - subtitles filter only!
            if (preset->subtitles.charenc.Len() > 0) res += ":charenc=" + preset->subtitles.charenc;
        }

        if (rs)
        {

            //Resize
            s.Printf(":original_size=%sx%s", sw, sh);
            res += s;

        }

        if (sub_stream_idx > 0)
        {

            //Select correct stream identifier
            s.Printf(":si=%i", sub_stream_idx);
            res += s;

        }

        //Return the filter
        return FILTER_VIDEO_IN + res + FILTER_VIDEO_OUT;

    }

}

//---------------------------------------------------------------------------------------

wxString NextFilterID(int &fid, wxString stream_tag = "")
{

    //Generates a new filter identification token for use in the filter chain
    //in the format [FID(n)(:stream_tag)] and increments the identification counter
    //to ensure unique filter tokens
    wxString res;
    if (stream_tag.Len() == 0) res.Printf("[FID%i]", fid);
    else res.Printf("[FID%i:%s]", fid, stream_tag);
    fid++;
    return res;

}

//---------------------------------------------------------------------------------------

void MakeInputFileArgs(wxArrayString &src, wxArrayString &dst)
{

    //This method will convert a list of packed FFQ_INPUT_FILE's
    //to a list of -i arguments to use with ffmpeg
    wxString s, t;

    for (unsigned int i = 0; i < src.Count(); i++)
    {

        //Get FFQ_INPUT_FILE
        FFQ_INPUT_FILE inf(src[i]);

        //Stream start
        s = inf.start.IsUndefined() ? "" : "-ss " + inf.start.ToString() + " ";

        //Frame rate
        if (inf.framerate.Len() > 0)
        {
            t = inf.framerate;
            if (t.StartsWith('~'))
            {
                t.Remove(0, 1);
                if (t.Len() > 0) s += "-r " + t + " ";
            }
            else s += "-framerate " + t + " ";
        }

        //Input Time Stamp Offset
        if (inf.itsoffset != 0)
        {
            t = TIME_VALUE(labs(inf.itsoffset)).ToShortString();
            if (inf.itsoffset < 0) t = "-" + t;
            s += "-itsoffset " + t + " ";
        }

        //fflags
        t = "";
        if (inf.fflags_discardcorrupt) t+= "+discardcorrupt";
        if (inf.fflags_genpts) t+= "+genpts";
        if (inf.fflags_igndts) t+= "+igndts";
        if (inf.fflags_ignidx) t+= "+ignidx";
        if (inf.fflags_sortdts) t+= "+sortdts";
        //if (inf.fflags_fastseek) t+= "+fastseek";
        //if (inf.fflags_bitexact) t+= "+bitexact";

        //Add to s
        if (t.Len() > 0) s+= "-fflags \"" + t + "\" ";

        //Add filename
        s += "-i \"" + FormatFileName(inf.path) + "\" ";

        //Add to input list
        dst.Add(s);

    }

}

//---------------------------------------------------------------------------------------
//Public functions
//---------------------------------------------------------------------------------------

//Structure for temporarily storing info about streams (Stream Build INFO)
typedef struct SBINFO
{
    int type; //Stream type 0=vid, 1=aud, 2=subs
    wxString tag; //The stream tag used for filtering [X:Y]
    TIME_VALUE dur; //Stream duration
    FFQ_CUTS cuts; //A list of cuts to perform

    SBINFO(int type, wxString tag, FFQ_CUTS cuts, TIME_VALUE dur)
    {
        this->type = type;
        this->tag = tag;
        this->cuts = cuts;
        this->dur = dur;
    }

} SBINFO, *LPSBINFO;

//---------------------------------------------------------------------------------------

wxString BuildCommandLine(LPFFQ_JOB job, long &encoding_pass, bool for_preview, bool audio_filters_complex)
{

    //Is the preset the only thing to make?
    bool preset_only = job->skip_in_files >= 0; //job->in1.Len() == 0;

    //Handy variables
    wxString s, t;

    //List of input files and their arguments
    wxArrayString input_list;
    if (!preset_only) MakeInputFileArgs(job->inputs, input_list);

    //Array that hold a list of SBINFO for each audio &video stream in the job
    wxArrayPtrVoid streams;

    //Integers that define the first entry of each stream type in the list
    int vid_idx = -1, aud_idx = -1;

    //Variables used for required inputs for filters, subtitle filter, mapping & preset
    wxString sub_in, req_in, sub_filter, mapping, preset;

    //Target extension
    wxString target_ext = job->out.AfterLast('.').Lower();

    //Find the preset to be used for this job
    LPFFQ_PRESET pst = (job->preset_ptr != NULL) ? (LPFFQ_PRESET)job->preset_ptr : FFQPresetMgr::Get()->GetPreset(job->preset_id.ToString());

    //If preset is used and preset uses two-pass EncodingPass must be incremented (0 for new job)
    if ((!for_preview) && (pst != NULL) && pst->two_pass) encoding_pass++;

    //Do we need to add segmenting arguments?
    //Segmenting is only performed when not previewing and for single pass / second pass
    bool segment = (!for_preview) && ((encoding_pass==0) || (encoding_pass==2)) && (pst != NULL) && (pst->segmenting.length > 0);

    //Create stream mapping for command line
    STREAM_MAPPING smap;
    wxString sm = job->stream_map, smcur;
    int sub_stream_idx = -1;
    bool mapped_subs = false;

    if (preset_only)
    {

        //When preset only is being build the streamMap only contains a
        //simple [VIDID],[AUDID] mapping

        streams.Add(new SBINFO(0, GetToken(sm, ','), FFQ_CUTS(), 0));
        vid_idx = 0;

        streams.Add(new SBINFO(1, GetToken(sm, ','), FFQ_CUTS(), 0));
        aud_idx = 1;

        //vid_in = GetToken(sm, ',');
        //aud_in = GetToken(sm, ',');

        //Add empty strings to the list of input files in order to compensate
        //for the amount of input files that should be skipped in order to
        //maintain valid use of requested input files by filters (req_in)
        long in_count = job->skip_in_files;
        while (in_count > 0)
        {
            input_list.Add("");
            in_count--;
        }

    }
    else while (sm.Len() > 0)
    {

        //This is an actual command line build and therefore the stream
        //mapping is fully specified

        smap.Parse(sm);
        smap.file_id--; //Decrement file_id since it starts from 1 in FFQ and 0 in FFM

        if (smap.checked)
        {

            smcur = "";
            smcur << smap.file_id << ":" << smap.stream_id;

            if (smap.codec_type == CODEC_TYPE_VIDEO)
            {

                //Get file info
                FFQ_INPUT_FILE inf = job->GetInput(smap.file_id);

                //Add stream
                streams.Add(new SBINFO(0, "[" + smcur + "]", inf.cuts, inf.duration));

                //Set index of first video stream
                if (vid_idx < 0) vid_idx = streams.GetCount() - 1;

            }

            else if (smap.codec_type == CODEC_TYPE_AUDIO)
            {

                //As with video
                FFQ_INPUT_FILE inf = job->GetInput(smap.file_id);
                streams.Add(new SBINFO(1, "[" + smcur + "]", inf.cuts, inf.duration));
                if (aud_idx < 0) aud_idx = streams.GetCount() - 1;

                //Prevent audio from being mapped into two pass encode
                if (encoding_pass == 1) continue;

            }

            else if (smap.codec_type == CODEC_TYPE_SUBTITLE)
            {

                if (sub_in.Len() == 0)
                {

                    //We are only interested in the first subtitle stream since
                    //that is the one required for subtitle burn in
                    sub_in = "[" + smcur + "]";
                    sub_stream_idx++;

                    //Map or burn-in?
                    if ((pst != NULL) && (pst->subtitles.codec == CODEC_SUBS_BURNIN))
                    {

                        //Create the subtitle burn in filter
                        sub_filter = MakeSubtitleBurninFilter(sub_in, sub_stream_idx, job->GetInput(smap.file_id).path, pst);

                        //Prevent burned-in subtitles from being mapped
                        continue;

                    } //else map

                }

                //Prevent cuts if subtitles are mapped
                mapped_subs = true;

            }

            //Add stream to mapping
            mapping += "-map " + smcur + " ";

        }

        else if (smap.codec_type == CODEC_TYPE_SUBTITLE) sub_stream_idx++; //Beware of teletext!

    }

    //Handy vars for detecting included content
    bool has_video = (vid_idx >= 0),

         //Audio skipped in first pass
         has_audio = (aud_idx >= 0) && (encoding_pass != 1),

         //Subtitles skipped in first pass if not burned in
         has_subs = (sub_in.Len() > 0) && ( (encoding_pass != 1) || (sub_filter.Len() > 0) );

    //Format preset if available and required by the command line
    if ((pst != NULL) && (job->cmd_line.Find(CMD_PRESET) >= 0))
    {

        //Used to prevent switches that are useless for stream copy
        bool copy_vid = (pst->video_codec == CODEC_COPY),
             copy_aud = (pst->audio_codec == CODEC_COPY),
             can_cut = /*(!has_subs) &&*/ (!mapped_subs) && ((!has_video) || (!copy_vid)) && ((!has_audio) || (!copy_aud));
             //copySubs = (pst->SUBS.codec == CODEC_COPY); //Not used, removed until usefull

        //filterID is a counter used to create unique filter identifiers
        int filter_id = 1;

        //Adjust audio_filter_complex according to preset
        if (pst->af_complex) audio_filters_complex = true;

        //Make filters for all streams
        wxString vf, af; //Video filters and audio filters
        SBINFO dummy(0, "", FFQ_CUTS(),0); //Dummy used as empty pointer
        LPSBINFO first_vid = (has_video ? (LPSBINFO)streams[vid_idx] : &dummy),
                 first_aud = (has_audio ? (LPSBINFO)streams[aud_idx] : &dummy);

        for (unsigned int sidx = 0; sidx < streams.GetCount(); sidx++)
        {

            //Get the stream info
            LPSBINFO sbi = (LPSBINFO)streams[sidx];

            //Can video filters be applied?
            if ((!copy_vid) && (sbi->type == 0))
            {

                //Since subtitles can be burned in to multiple streams,
                //we need a var for a copy of the subs_filter
                wxString subs = sub_filter.Clone();

                //Add cuts first?
                if (can_cut && sbi->cuts.filter_first && FormatCuts(sbi->cuts, sbi->tag, true, sbi->dur, filter_id))
                    vf += sbi->cuts.cuts;

                //Apply all other video filters from the presets filter list
                for (unsigned int fidx = 0; fidx < pst->filters.Count(); fidx++)
                {

                    //Get filter
                    FFMPEG_FILTER fltr = FFMPEG_FILTER(pst->filters[fidx]);

                    if (fltr.IsVideo())
                    {

                        //Check if the filter has a required input file
                        if (fltr.required_file.IsValid())
                        {

                            //Yup, add it to inputs and make a tag for it
                            s = "-i \"" + fltr.required_file.path + "\" ";
                            if (fltr.required_file.loop.Len() > 0) s = "-loop " + fltr.required_file.loop + " " + s;
                            wxString tag = fltr.required_file.tag;
                            if (tag.Len() > 0) tag = ":" + tag;
                            req_in.Printf("[%u%s]", SIZEFMT(input_list.GetCount()), tag);
                            input_list.Add(s);

                        }

                        //Format the filter
                        if (FormatFilter(fltr.ff_filter, sbi->tag, first_aud->tag, sub_in, req_in, filter_id)) vf += fltr.ff_filter;

                    }

                    else if (fltr.type == ftSUBSBURNIN)
                    {

                        //Subtitle dummy-filter for placement in the filter chain found
                        if (FormatFilter(subs, sbi->tag, first_aud->tag, sub_in, req_in, filter_id))
                        {

                            vf += subs;
                            subs = ""; //Prevent it from being re-added later

                        }

                    }

                }

                //If subtitle-filter has not been applied - do it now
                if (FormatFilter(subs, sbi->tag, first_aud->tag, sub_in, req_in, filter_id)) vf += subs;

                //Add cuts last?
                if (can_cut && (!sbi->cuts.filter_first) && FormatCuts(sbi->cuts, sbi->tag, true, sbi->dur, filter_id))
                    vf += sbi->cuts.cuts;

                //If the last filter tag is not removed from the filter string
                //it must be mapped into the output with "-map [tag] output_file"
                //Therefore we remove it here but we preserve the filter separator
                //in order to be able to add filters for more streams
                if (vf.Right(1) == ";") vf = vf.BeforeLast('[') + ";";

            }

            //Can audio filters be applied?
            else if ((!copy_aud) && (sbi->type == 1))
            {

                //Prepare audio cuts
                wxString cuts = (can_cut && FormatCuts(sbi->cuts, sbi->tag, false, sbi->dur, filter_id)) ? sbi->cuts.cuts : "";
                bool has_cuts = (cuts.Len() > 0);

                //Cuts requires complex audio filtering
                if (has_cuts) audio_filters_complex = true;

                //Apply audio cutting first?
                if (has_cuts && sbi->cuts.filter_first) af += cuts;

                //Process audio filters from the list
                for (unsigned int fidx = 0; fidx < pst->filters.Count(); fidx++)
                {

                    FFMPEG_FILTER fltr = FFMPEG_FILTER(pst->filters[fidx]);
                    if (fltr.IsAudio() && FormatFilter(fltr.ff_filter, first_vid->tag, sbi->tag, sub_in, req_in, filter_id))
                        af += fltr.ff_filter;

                }

                //Apply audio cutting last?
                if (has_cuts && (!sbi->cuts.filter_first)) af += cuts;

                //See comment for the same thing done on vf above
                if (af.Right(1) == ";") af = af.BeforeLast('[') + ";";

            }

        }

        //Now we need to add the filters to the preset
        if (vf.Len() > 0)
        {

            //Set video filters, always complex
            preset = "-filter_complex \"" + vf;

            //Add complex audio filters?
            if (audio_filters_complex && af.Len() > 0)
            {
                preset += af;
                af.Clear(); //Prevent from being re-added
            }

            //Remove trailing ";" and close filter_complex
            preset.RemoveLast();
            preset += "\" ";

        }

        //Add audio filters (if not already done above)
        if (af.Len() > 0)
        {

            //Remove trailing ";"
            af.RemoveLast();

            //Figure out the correct argument
            preset += audio_filters_complex ? "-filter_complex" : "-af";

            //Add filters to preset
            preset += " \"" + af + "\" ";

        }

        //Release memory, this cannot be done in the loop above, since it might
        //result in first_vid and / or first_aud as being bad memory
        for (unsigned int sidx = 0; sidx < streams.GetCount(); sidx++) delete (LPSBINFO)streams[sidx];
        streams.Clear();

        //Video codec
        if (has_video && (pst->video_codec.Len() > 0)) preset += "-c:v " + pst->video_codec + " ";

        if ((!copy_vid) && has_video)
        {

            //Video rate
            s = pst->video_rate;
            if (FormatStreamRate(s, true)) preset += s;//(pst->videoVBR ? "-qscale:v " : "-b:v ") + s + " ";

            //Video quality
            s = pst->video_quality;
            if (FormatVideoQuality(s, pst->video_codec)) preset += s;

            //Trellis
            if (pst->trellis.Len() > 0) preset += "-trellis " + pst->trellis + " ";

            //Threads
            if (pst->threads.Len() > 0) preset += "-threads " + pst->threads + " ";

            //Video speed preset
            if (pst->speed_preset.Len() > 0) preset += "-preset:v " + pst->speed_preset + " ";

            //Key frame, motion and scene change arguments
            s = pst->key_frames;
            if (MakeKeyframeArguments(s)) preset += s;

            //Frames per second
            if (pst->frame_rate.Len() > 0) preset += "-r " + pst->frame_rate + " ";

            //Pixel format
            if (pst->pixel_format.Len() > 0) preset += "-pix_fmt " + pst->pixel_format + " ";

            //Video sync
            if (pst->video_sync.Len() > 0) preset += "-vsync " + pst->video_sync + " ";

            //Closed GOP when segmenting - this should be a user choice in a future release
            //if (segment) preset += "-flags +cgop ";

        }

        //Audio codec
        if (has_audio && (pst->audio_codec.Len() > 0)) preset += "-c:a " + pst->audio_codec + " ";

        if ((!copy_aud) && has_audio)
        {

            //Audio rate
            s = pst->audio_rate;
            if (FormatStreamRate(s, false)) preset += s;//(pst->audioVBR ? "-qscale:a " : "-b:a ") + s + " ";

            //Audio quality
            s = pst->audio_quality;
            if (FormatAudioQuality(s, pst->audio_codec)) preset += s;

            //Audio channels
            if (pst->audio_channels.Len() > 0) preset += "-ac " + pst->audio_channels + " ";

            //Audio profile
            if (pst->audio_profile.Len() > 0) preset += "-profile:a " + pst->audio_profile + " ";

        }

        //Subtitle codec
        if (has_subs && (pst->subtitles.codec.Len() > 0) && (pst->subtitles.codec != CODEC_SUBS_BURNIN))
            preset += "-c:s " + pst->subtitles.codec + " ";

        //Segmenting - note that "faststart" needs to be set different when segmenting
        if (segment)
        {
            uint64_t st = pst->segmenting.length * 1000;
            if (pst->segmenting.length_type > 0) st *= 60;
            if (pst->segmenting.length_type > 1) st *= 60;
            TIME_VALUE sl(st);
            preset += "-segment_time " + sl.ToShortString() + " ";
            preset += "-break_non_keyframes " + wxString(pst->segmenting.break_bframes ? "1" : "0") + " ";
            preset += "-increment_tc " + wxString(pst->segmenting.incremental_tc ? "1" : "0") + " ";
            preset += "-reset_timestamps " + wxString(pst->segmenting.reset_ts ? "1" : "0") + " ";
            if (pst->mf_faststart) preset += "-segment_format_options movflags=+faststart ";
        }

        //No segmenting, use default faststart
        else if (pst->mf_faststart) preset += "-movflags faststart ";

        if (target_ext == "avi")
        {
            //FourCC - only for AVI containers
            if (pst->fourcc.vids.Len() == 4) preset += "-vtag " + pst->fourcc.vids + " ";
            if (pst->fourcc.auds.Len() == 4) preset += "-atag " + pst->fourcc.auds + " ";
        }

        if (pst->meta_data.Len() > 0)
        {
            s = pst->meta_data;
            while (s.Len() > 0) preset += "-metadata " + GetToken(s, FILTER_SEPARATOR) + " ";
        }

        //Aspect ratio - does not need encoding - just video
        if (has_video && (pst->aspect_ratio.Len() > 0)) preset += "-aspect " + pst->aspect_ratio + " ";

        //Append the proper custom arguments
        if (for_preview && (pst->custom_args_1.Len() > 0)) preset += pst->custom_args_1 + " ";
        else if ((encoding_pass < 2) && (pst->custom_args_1.Len() > 0)) preset += pst->custom_args_1 + " ";
        else if ((encoding_pass > 1) && (pst->custom_args_2.Len() > 0)) preset += pst->custom_args_2 + " ";

    }

    //String for formatting the resulting command line
    s = job->cmd_line;

    //Check if banner must be hidden
    if (FFQCFG()->hide_banner) s = "-hide_banner " + s;

    //Convert inputList to string and insert inputs & mapping to command line
    t = "";
    for (unsigned int i = 0; i < input_list.GetCount(); i++) if (input_list[i].Len() > 0) t += input_list[i];

    #ifdef DEBUG
    //ShowInfo(t);
    #endif // DEBUG

    //Make sure that CMD_INPUTS is not removed if presetOnly
    if (preset_only) t = CMD_INPUTS + t;

    //Insert inputs & mapping to command line
    CommandLineReplace(s, CMD_INPUTS, t + mapping);

    //Insert preset
    CommandLineReplace(s, CMD_PRESET, preset);

    //If only the preset is being build - we are done
    if (preset_only) return s;

    //Create output-file - depending on multipass (and what pass) or singlepass
    if ((!for_preview) && (encoding_pass > 0))
    {

        //Get name of passlogfile
        GetPassLogFileName(job, t);

        //Add pass number and passlogfile
        if (pst && (pst->video_codec.Find("x265") != wxNOT_FOUND))
        {

            //x265 handles multiple passes different - here's how
            t = "-x265-params pass=" + ToStr(encoding_pass) + ":stats=\"'" + FormatFileName(t) + "'\" ";

        }

        else
        {

            //Use default multi-pass parameters
            t = "-pass " + ToStr(encoding_pass) + " -passlogfile \"" + FormatFileName(t) + "\" ";

        }

    }

    else t = ""; //No multi-pass encoding = empty string

    //For convenience
    bool two_pass_null = false;
    wxString fmt = pst ? pst->output_format : "";

    if ((!for_preview) && (encoding_pass == 1))
    {

        two_pass_null = (pst != NULL) && pst->two_pass_null; //pst should never be NULL here - but just in case..
        t+= "-an "; //No audio for first pass
        if (two_pass_null) fmt = "null"; //Override output format to null for first pass

    }

    //Add time-limit to encode
    if (!job->out_len.IsUndefined()) t += "-t " + job->out_len.ToString() + " ";

    if (for_preview)
    {

        //Add the pipe commands for ffmpeg > ffplay using the matroska container
        PLAYER_TYPE ptype;
        wxString pcmd = FFQCFG()->GetFFPlayCommand(true, &ptype);
        if (pcmd.Len() == 0) return pcmd;
        wxString pargs = "";
        if (ptype == ptFFPLAY) pargs += "-autoexit ";
        else if (ptype == ptVLC) pargs += "--play-and-exit ";
        t += "-f matroska - | \"" + pcmd + "\" " + pargs + "-";

    }
    else
    {

        //Get output name
        wxString out_name = job->out;

        //Output format - different arguments when segmenting
        if (segment)
        {

            //Segmenting format
            t += "-f ";
            if (pst->segmenting.streaming) t += "s";
            t += "segment ";

            //Container format
            if (fmt.Len() > 0) t += "-segment_format " + fmt + " ";

            //Segmenting list file name
            if (pst->segmenting.list_file > 0)
            {
                const wxString lft[] = {"flat", "csv", "ffconcat", "m3u8"};
                wxString list_file = out_name.BeforeLast('.') + "." + lft[pst->segmenting.list_file - 1];
                EnsureUniquePath(list_file);
                t += "-segment_list \"" + FormatFileName(list_file) + "\" ";
            }

            //Segmenting numbering - this probably need a makeover at some point
            out_name = out_name.BeforeLast('.') + " %03d." + out_name.AfterLast('.');

        }

        //Regular container format selection
        else if (fmt.Len() > 0) t += "-f " + fmt + " ";

        //Add either NULL_FILENAME or output filename
        t += two_pass_null ? NULL_FILENAME : "-y \"" + FormatFileName(out_name) + "\"";

    }

    //Merge output into command line
    CommandLineReplace(s, CMD_OUTPUT, t);

    //Return da mess!
    return s;

}

//---------------------------------------------------------------------------------------

const wxString PASSLOG_CUTREE = ".cutree";

//---------------------------------------------------------------------------------------

void CleanupFinishedJob(LPFFQ_JOB job)
{

    //Clean up any temporary files or other garbage created by a job
    wxString plf, cf;
    GetPassLogFileName(job, plf);

    //x265 files
    if (wxFileExists(plf)) wxRemoveFile(plf);
    if (wxFileExists(plf + PASSLOG_CUTREE)) wxRemoveFile(plf + PASSLOG_CUTREE);

    //Other files
    for (int i = 0; i < 1000; i++)
    {

        cf.Printf("%s-%i.log", plf, i);

        if (wxFileExists(cf))
        {

            wxRemoveFile(cf); //Delete log
            cf += ".mbtree";
            if (wxFileExists(cf)) wxRemoveFile(cf); //Delete mbtree

        } else return; //No more stream-files exists

    }

}

//---------------------------------------------------------------------------------------

#define _E(s, c) s.Replace(wxString(c), "\\" + wxString(c))
#define _U(s, c) s.Replace("\\" + wxString(c), wxString(c))

//---------------------------------------------------------------------------------------

wxString EscapeFilterString(wxString s, bool unEscape)
{

    //Escapes (or un-escapes) a string as declared in the ffmpeg docs here:
    //https://www.ffmpeg.org/ffmpeg-filters.html#Notes-on-filtergraph-escaping

    if (unEscape)
    {

        //Third level (the shell level - not required in windows)
        //There must be made a conditional compiler statement to
        //properly enable this level where required
        //_E(s, '\\');

        //Second level
        _U(s, ';');
        _U(s, ',');
        _U(s, ']');
        _U(s, '[');
        _U(s, '\'');
        _U(s, '\\');

        //First level
        _U(s, '\'');
        _U(s, ':');
        _U(s, '\\');

    }
    else
    {

        //First level
        _E(s, '\\');
        _E(s, ':');
        _E(s, '\'');
        //Second level
        _E(s, '\\');
        _E(s, '\'');
        _E(s, '[');
        _E(s, ']');
        _E(s, ',');
        _E(s, ';');

        //Third level (the shell level - not required in windows)
        //There must be made a conditional compiler statement to
        //properly enable this level where required
        #ifndef __WINDOWS__
        _E(s, '\\');
        #endif

    }

    //Return the result
    return s;

}

//---------------------------------------------------------------------------------------

wxString FormatFileName(wxString fn)
{

    //Used to make sure that a file name is valid for the command line
    fn.Replace("\"", "\\\"");
    //fn.Replace("%", "%%");
    //fn.Replace("\'", "\\\'");

    return fn;

}

//---------------------------------------------------------------------------------------

const wxString TRIM_FILTER = "{$%i}%strim=%s:%s,%ssetpts=PTS-STARTPTS";

//---------------------------------------------------------------------------------------

void AppendTrim(wxString &fltr, wxString &concat, int &fid, int &tid, bool video, TIME_VALUE from, TIME_VALUE to, TIME_VALUE duration, unsigned int compensate = 0)
{

    //Compensation required?
    if (compensate > 0)
    {

        //When removing parts the time values must be compensated with the
        //duration of one frame in order to be accurate - this we do now
        if (from.ToMilliseconds() > 0) from = TIME_VALUE(from.ToMilliseconds() + compensate);
        if (to.ToMilliseconds() < duration.ToMilliseconds()) to = TIME_VALUE(to.ToMilliseconds() - compensate);

    }

    //Append filter - notice that we use an input tag id instead
    //of a stream tag in order to be able to prepend a split filter
    //if required
    fltr += wxString::Format(TRIM_FILTER, tid++, video?"":"a", from.ToShortString(), to.ToShortString(), video?"":"a");

    //Get an output tag
    wxString tag = NextFilterID(fid);

    //Append output tag to filter and concat
    fltr += tag + ";";
    concat += tag;

}

//---------------------------------------------------------------------------------------

bool FormatCuts(FFQ_CUTS &cuts, wxString &stream_tag, bool video, TIME_VALUE duration, int &filter_id)
{

    //Converts a list of from_time;to_time[..] to a filter
    //that utilizes the trim & concat filters for video
    //and audio

    //Return false if no stream tag
    if (stream_tag.Len() == 0) return false;

    //Handy vars
    bool beginning = true;
    wxString res, concat, save_tag = stream_tag;
    TIME_VALUE from, to, prev_to(0);
    int tag_id = 0;

    //Make filter
    while (cuts.cuts.Len() > 0)
    {

        //Get trim times
        from = TIME_VALUE(GetToken(cuts.cuts, ";", true));
        to = TIME_VALUE(GetToken(cuts.cuts, ";", true));

        if (cuts.keep)
        {

            //Add simple trim to keep the part
            AppendTrim(res, concat, filter_id, tag_id, video, from, to, duration);

        }

        else
        {

            //If we are not at the beginning or from > 0 then
            //we must add filters to trim prev_to => from
            if ((!beginning) || (from.ToMilliseconds() > 0))
                AppendTrim(res, concat, filter_id, tag_id, video, prev_to, from, duration, cuts.frame_time);

            //We are no longer at the beginning
            beginning = false;

            //If we are at the end (cuts is empty) then we must check if
            //something there must be kept
            if ((cuts.cuts.Len() == 0) && (to.ToMilliseconds() < duration.ToMilliseconds()))
                AppendTrim(res, concat, filter_id, tag_id, video, to, duration, duration, cuts.frame_time);

            //Store to for next iteration
            prev_to = to;

        }

    }

    //Count number of tags in concat
    int cf = concat.Freq('[');

    if (cf > 1)
    {

        //More than one tag = create output stream tag and concat filter
        stream_tag = NextFilterID(filter_id);
        res += concat + wxString::Format("concat=n=%i:v=%i:a=%i%s;", cf, video?1:0, video?0:1, stream_tag);

    }
    else if (cf > 0)
    {

        //Only one tag = get output stream tag and don't concat
        filter_id--;
        stream_tag = NextFilterID(filter_id);

    }

    //If tag_id is above one and the supplied stream_tag if not
    //a [file:stream] tag, then we need to prepend a split filter
    //for each trim performed
    if ((tag_id > 1) && (save_tag.Find(':') == wxNOT_FOUND))
    {

        //Yes we do, create and replace tags
        wxString split, tag;
        for (int i = 0; i < tag_id; i++)
        {
            tag = NextFilterID(filter_id);
            split += tag;
            res.Replace(wxString::Format("{$%i}", i), tag);
        }

        //Prepend split filter
        res = wxString::Format("%s%ssplit=%i%s;", save_tag, video?"":"a", tag_id, split) + res;

    }

    //Nope, we do not need to use a split filter, insert
    //saved stream tag instead
    else for (int i = 0; i < tag_id; i++)
        res.Replace(wxString::Format("{$%i}", i), save_tag);

    //Set cuts and return success
    cuts.cuts = res;
    return res.Len() > 0;

}

//---------------------------------------------------------------------------------------

bool FormatFilter(wxString &filter, wxString &vid_in, wxString &aud_in, wxString &sub_in, wxString &req_in, int &filter_id)
{

    //Formats the filter by replacing the <*IN> tokens with the actual
    //stream identification tokens that are generated with a counter
    //by the function NextFilterID

    //Empty filter = no bother
    if (filter.Len() == 0) return false;

    if (filter.Find(FILTER_VIDEO_IN) >= 0)
    {

        //Handle video filter - if no video token is available the
        //filter is invalid and can therefore not be implemented in
        //the filter chain
        if (vid_in.Len() == 0) return false;

        //Filter and token is OK - replace
        filter.Replace(FILTER_VIDEO_IN, vid_in);

        //Generate a new filter id for video
        vid_in = NextFilterID(filter_id);

    }

    if (filter.Find(FILTER_AUDIO_IN) >= 0)
    {

        //Same procedure as with video
        if (aud_in.Len() == 0) return false;
        filter.Replace(FILTER_AUDIO_IN, aud_in);
        aud_in = NextFilterID(filter_id);

    }

    if (filter.Find(FILTER_SUBTITLE_IN) >= 0)
    {

        //Same procedure as with video
        if (sub_in.Len() == 0) return false;
        filter.Replace(FILTER_SUBTITLE_IN, sub_in);
        sub_in = NextFilterID(filter_id);

    }

    if (filter.Find(FILTER_REQUIRED_IN) >= 0)
    {

        //If a filter requires a file (FFQFilter->REQUIRED_FILE) the input
        //token for the file must be available
        if (req_in.Len() == 0) return false;
        filter.Replace(FILTER_REQUIRED_IN, req_in);
        req_in = ""; //No output for required in = use only once

    }

    //Now we need to replace the output tokens
    //with the newly created tokens
    filter.Replace(FILTER_VIDEO_OUT, vid_in);
    filter.Replace(FILTER_AUDIO_OUT, aud_in);
    filter.Replace(FILTER_SUBTITLE_OUT, sub_in);

    //Handle unique ID's used by filters that have multiple parts
    unsigned int uid = 1, rep;
    do { rep = filter.Replace(wxString::Format(FILTER_UNIQUE_ID, uid), NextFilterID(filter_id)); uid++; } while (rep > 0);

    //Make sure that the filter end with semicolon
    if (filter.Right(1) != ";") filter += ";";

    //All green
    return true;

}

//---------------------------------------------------------------------------------------

bool FormatStreamRate(wxString &rate, bool video)
{

    //Converts the settings from the FFQBitRatePanel into command line arguments

    //Extract values
    long abr = Str2Long(GetToken(rate, ','), 0),
         brt = Str2Long(GetToken(rate, ','), 1),
         min = Str2Long(GetToken(rate, ','), 0),
         max = Str2Long(GetToken(rate, ','), 0),
         buf = Str2Long(GetToken(rate, ','), 0);

    //Convert values to command line arguments
    rate = BitRateToArg(abr, brt, "b", video) +
           BitRateToArg(min, brt, "minrate", video) +
           BitRateToArg(max, brt, "maxrate", video) +
           BitRateToArg(buf, brt, "bufsize", video);

    //Return if any values are available
    return rate.Len() > 0;

}
