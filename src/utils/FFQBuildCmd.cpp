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
#include "FFQConsole.h"
//#include "FFQProbing.h"
#include <wx/filename.h>
#include <wx/regex.h>

//---------------------------------------------------------------------------------------

//Constant that declares the name used for multipass log files
const wxString FILENAME_MULTIPASSLOG = "ffq_multipass_log_%u"; //ffqueue_ffmpeg_multipass

//---------------------------------------------------------------------------------------

//extern declarations

const wxString CODEC_TYPE_VIDEO = "video";
const wxString CODEC_TYPE_AUDIO = "audio";
const wxString CODEC_TYPE_SUBTITLE = "subtitle";
const wxString CODEC_TYPE_DATA = "data";
const wxString CODEC_TYPE_ATTACHMENT = "attachment";
const wxString CODEC_COPY = "copy";
const wxString CODEC_SUBS_BURNIN = "BURN-IN";
const wxString X265_PARAMS = "-x265-params";
const wxString TWO_PASS_INSERT_POS = "$$TPIP$$";

const unsigned int ENCODER_PRESET_COUNT = 9;
const wxString ENCODER_PRESETS[ENCODER_PRESET_COUNT] = {

    "ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow"

};

const unsigned int ENCODER_TUNINGS_COUNT = 6;
const wxString ENCODER_TUNINGS[ENCODER_TUNINGS_COUNT] = {

    "film", "animation", "grain", "stillimage", "fastdecode", "zerolatency"

};

const unsigned int DISPOSITION_COUNT = 15;
const wxString DISPOSITIONS[DISPOSITION_COUNT] = {

    "default", "dub", "original", "comment", "lyrics", "karaoke", "forced", "hearing_impaired", "visual_impaired",
    "clean_effects", "attached_pic", "captions", "descriptions", "dependent", "metadata"

};

bool IsPreviewSafe = true;
static unsigned int multipass_log_counter = 0; //To ensure a new file name for each job

//---------------------------------------------------------------------------------------
//Private helper functions
//---------------------------------------------------------------------------------------

wxString BitRateToArg(long bit_rate, long br_type, wxString ff_arg, bool video, wxString stream_tag = wxEmptyString)
{

    //This function converts a single bit rate setting into a command line argument

    //Zero bit_rate means unused arg
    if (bit_rate <= 0) return "";

    //Prepare stream specifier
    wxString av = (video ? "v" : "a") + stream_tag, rate;

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
    return "-" + ff_arg + COLON + av + SPACE + rate + SPACE;

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
        while ((delp < cmd.Len()) && (cmd.GetChar(delp) == SPACE)) cmd.Remove(delp, 1);

        //Replace the old with the new
        if (idx > 0) cmd.Replace(old_str, SPACE + new_str); //Ensure a single blank character
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

    bool b = GetToken(from, COMMA) == STR_YES; //Use?
    float v = Str2Long(GetToken(from, COMMA), INT_MIN); //Percent

    if (b && (v > INT_MIN) && (max != min)) //Validate
    {

        qval = ConvertPctToMinMaxFloat(v, min, max); //Convert

        //Float or int command line argument?
        if (is_float) return "-" + cmd + " " + wxString::Format("%.2f", qval) + SPACE; //Float
        else return "-" + cmd + " " + wxString::Format("%i", froundi(qval)) + SPACE; //Int

    }

    return ""; //Nope, not used!

}

//---------------------------------------------------------------------------------------

bool FormatAudioQuality(wxString &aq, const wxString &codec, const wxString stream_tag = wxEmptyString)
{

    //Convert audio quality settings to command line arguments
    wxString res;

    //Get codec information
    CODEC_INFO *ci = FFQCFG()->GetCodecInfo()->Find(codec);
    if (ci == NULL) ci = FFQCFG()->GetCodecInfo()->Find(DEFAULT_AUDIO_CODEC_INFO);
    float val;

    //Oh yeah! More formatting!
    res  = FormatSingleQualityArg(aq, "q:a" + stream_tag, ci->min_qscale, ci->max_qscale, val, ci->qscale_float);

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

void GetTwoPassLog(LPFFQ_JOB job, wxString &path)
{

    if (job->twopass_log.Len() == 0)
    {

        //For convenience
        wxUniChar psep = wxFileName::GetPathSeparator();

        //Get temp path
        job->twopass_log = FFQCFG()->GetTmpPath(job->out.BeforeLast(psep));

        //Make sure it ends with a path-separator
        if ((job->twopass_log.Len() > 0) && (job->twopass_log.Right(1) != psep)) job->twopass_log += psep;

        //Find a unique name for the file
        do
        {
            path = wxString::Format(FILENAME_MULTIPASSLOG, ++multipass_log_counter);
        } while (wxFileExists(job->twopass_log + path));

        //Assign the log file name
        job->twopass_log += path;

    }

    //Get the file name
    path = job->twopass_log;

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
    s = GetToken(kf, COMMA);
    if (s.Len() > 0) res += "-keyint_min " + s + SPACE;

    //Max key frame dist
    s = GetToken(kf, COMMA);
    if (s.Len() > 0) res += "-g " + s + SPACE;

    //Motion estimation method
    s = GetToken(kf, COMMA);
    if (s.Len() > 0) res += "-me_method " + s + SPACE;

    //Scene change sensibility.
    //NOTE: Some websites claims that this value has the range from
    //-1000000000 to 1000000000 (higher value, less sensitivity) but
    //I have experimented with it and found the actual range to be in
    //percent meaning 0 = disabled, 1..100 is the percent of image change
    //(higher value is again less sensitivity) that triggers a scene change.
    s = GetToken(kf, COMMA);
    t = GetToken(kf, COMMA);
    if (s == STR_YES)
    {
        v = Str2Long(t, -1);
        if (v >= 0) res += "-sc_threshold " + ( v == 0 ? "0" : ToStr(100-v) ) + SPACE;
    }

    //Maximum consecutive B-frames
    v = Str2Long(GetToken(kf, COMMA), 0);
    if (v > 0) res += "-bf " + ToStr(v - 2) + SPACE;

    //Maximum reference frames for P-frames
    v = Str2Long(GetToken(kf, COMMA), 0);
    if (v > 0) res += "-refs " + ToStr(v - 1) + SPACE;

    //Use closed GOP
    if (GetToken(kf, COMMA) == STR_YES) res += "-flags +cgop ";

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
        wxString ext = sub_src_file.AfterLast(DOT).Lower();

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
    //if (stream_tag.Len() == 0) res.Printf("[FID%i]", fid);
    //else res.Printf("[FID%i:%s]", fid, stream_tag);
    if (stream_tag.Len() == 0) res.Printf("[F%i]", fid);
    else res.Printf("[F%i:%s]", fid, stream_tag);
    fid++;
    return res;

}

//---------------------------------------------------------------------------------------

wxString MakeAudioEncoding(LPFFQ_PRESET pst, int stream_id)
{

    //Create a stream tag
    wxString tag = stream_id >= 0 ? wxString::Format(":%d", stream_id) : "";

    //Audio codec
    wxString res = pst->audio_codec.Len() > 0 ? "-c:a" + tag + SPACE + pst->audio_codec + SPACE : "";

    if (pst->audio_codec != CODEC_COPY)
    {

        //Audio rate
        wxString s = pst->audio_rate;
        if (FormatStreamRate(s, false, tag)) res += s;

        //Audio quality
        s = pst->audio_quality;
        if (FormatAudioQuality(s, pst->audio_codec, tag)) res += s;

        //Audio channels
        if (pst->audio_channels.Len() > 0) res += "-ac" + tag + SPACE + pst->audio_channels + SPACE;

        //Audio profile
        if (pst->audio_profile.Len() > 0) res += "-profile:a" + tag + SPACE + pst->audio_profile + SPACE;

        //Audio fullspec
        if (pst->fullspec_aud.Len() > 0) res += pst->fullspec_aud + SPACE;

    }

    return res;

}

//---------------------------------------------------------------------------------------

int GetKeepParts(FFQ_INPUT_FILE &inf, wxString *keep = NULL, bool non_quick = false)
{
    //The return value will be the number of times the input file
    //needs to be added to the command line. "quick_cuts" will
    //be set to the parts of the file that should be kept
    wxString qc = (non_quick || inf.cuts.quick) ? inf.cuts.KeepParts(inf.duration) : "";
    int qcc = qc.Len() > 0 ? ((qc.Freq(SCOLON) + 1) / 2) : 1;
    if (keep) *keep = qc;
    return qcc;
}

//---------------------------------------------------------------------------------------

void MakeInputFileArgs(wxArrayString &src, wxArrayString &dst, bool cuts_allowed)
{

    //This method will convert a list of packed FFQ_INPUT_FILE's
    //to a list of -i arguments to use with ffmpeg
    wxString s, t;

    for (unsigned int i = 0; i < src.Count(); i++)
    {

        //Get FFQ_INPUT_FILE
        FFQ_INPUT_FILE inf(src[i]);

        //Get quick cuts
        wxString cuts;
        int remain = cuts_allowed ? GetKeepParts(inf, &cuts) : 1;

        while (remain-- > 0)
        {

            //Stream start
            if (cuts.Len() > 0)
            {

                //If quick cuts are used, the default stream start is ignored
                //and a from-to span is defined instead
                s = "-ss " + GetToken(cuts, SCOLON, true) + SPACE;
                if (cuts.Len() > 0) s += "-to " + GetToken(cuts, SCOLON, true) + SPACE; //To may be empty for the last cut!

            }
            else s = inf.start.IsUndefined() ? "" : "-ss " + inf.start.ToString() + SPACE;

            //Frame rate
            if (inf.framerate.Len() > 0)
            {
                t = inf.framerate;
                if (t.StartsWith('~'))
                {
                    t.Remove(0, 1);
                    if (t.Len() > 0) s += "-r " + t + SPACE;
                }
                else s += "-framerate " + t + SPACE;
            }

            //Input Time Stamp Offset
            if (inf.itsoffset != 0)
            {
                t = TIME_VALUE(labs(inf.itsoffset)).ToShortString();
                if (inf.itsoffset < 0) t = "-" + t;
                s += "-itsoffset " + t + " ";
            }

            if (inf.loop != 0) s += "-stream_loop " + ToStr(inf.loop) + SPACE;

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

}

//---------------------------------------------------------------------------------------

wxString MakeHWDecodeArgs(wxString from)
{
    wxString hwd = from, res = wxEmptyString, t;
    t = GetToken(hwd, COMMA, true);
    if (t.Len() == 0) return wxEmptyString;
    res = "-hwaccel " + t;
    t = GetToken(hwd, COMMA, true);
    if (t.Len() > 0) res += " -c:v " + t;
    t = GetToken(hwd, COMMA, true);
    if (t.Len() > 0) res = "-hwaccel_device " + t + " " + res;
    if (hwd.Len() > 0) res = "-init_hw_device " + hwd + " " + res;
    res += " ";
    return res;
}

//---------------------------------------------------------------------------------------

wxString MakeMetaData(wxString meta_data, wxString stream_tag)
{
    //Make a list of -metadata "meta info" tags for the specified stream
    wxString res;
    if (stream_tag.Len() > 0) stream_tag = ":s:" + stream_tag;
    while (meta_data.Len() > 0)
    {

        wxString v = GetToken(meta_data, FILTER_SEPARATOR); //Get the metadata token
        wxString n = GetToken(v, EQUAL); //Get name:stream_tag part
        wxString st; //Additional stream tag(s)

        if (n.Find(PIPE) > 0) st = COLON + GetLastToken(n, PIPE, true);
        else
        {

            wxRegEx rx(":[cgps](:|\\z)", wxRE_EXTENDED);
            if (rx.Matches(n))
            {

                size_t pos = 0;
                if (rx.GetMatch(&pos, 0, 0))
                {

                    st = n.SubString(pos, n.Len());
                    n.Remove(pos, n.Len());

                }

            }

        }

        res += "-metadata" + stream_tag + st + SPACE + n + EQUAL + v + SPACE;//GetToken(meta_data, FILTER_SEPARATOR) + SPACE;

    }
    return res;
}

//---------------------------------------------------------------------------------------

wxString MakeDisposition(wxString from, int index, wxString stream_tag)
{
    while ((from.Len() > 0) && (index-- > 0)) from = from.AfterFirst(',');
    int disp = Str2Long(from.BeforeFirst(','), -1);
    if (disp >= 0)
    {
        if (stream_tag.Len() > 0) stream_tag = COLON + stream_tag;
        return "-disposition" + stream_tag + SPACE + (disp > 0 ? DISPOSITIONS[disp-1] : "0") + SPACE;
    }
    return wxEmptyString;
}

//---------------------------------------------------------------------------------------

bool IsCommandSafe(wxString cmd)
{

    //This will test if any malicious code may be present in the command / list of arguments
    size_t pos = 0;
    while (pos < cmd.Len())
    {
        #ifdef __WINDOWS__

            //Code required for windows?

        #else

            wxUniChar quote = '.';
            wxUniChar cc = cmd.at(pos);
            wxUniChar nc = (pos < cmd.Len() - 1) ? cmd.at(pos + 1) : '\0';

            if (cc == '\\') pos++; //Skip escaped character
            else if ((cc == '\'') || (cc == '\"'))
            {
                //In or out of single / double quotes
                if (quote == DOT) quote = cc;
                else if (quote == cc) quote = DOT;
            }
            else if ((quote == DOT) && ((cc == '&') || (cc == '|') || (cc == '`') || ((cc == '$') && (nc == '('))))
            {
                //Unsafe code may be present
                return false;
            }

        #endif // __WINDOWS__

        pos++;

    }

    //Command seems safe
    return true;
}

//---------------------------------------------------------------------------------------

void MakeMappedFileIndicies(LPFFQ_JOB job, wxArrayInt &indicies, bool cuts_allowed)
{
    //Some files may be added multiple times to the command line in order to perform quick cuts
    //which causes the indices for the following files to be wrong. This will make a list of
    //adjusted file indices to prevent this.
    indicies.Add(0);
    int cur = 0;
    for (int i = 0; i < (int)job->inputs.Count(); i++)
    {
        FFQ_INPUT_FILE inf(job->inputs[i]);
        cur += (cuts_allowed && inf.cuts.quick) ? GetKeepParts(inf) : 1;
        indicies.Add(cur);
    }
}

//---------------------------------------------------------------------------------------
//Public functions
//---------------------------------------------------------------------------------------

//Structure for temporarily storing info about streams (Stream Build INFO)
typedef struct SBINFO
{
    int type, //Stream type 0=vid, 1=aud, 2=subs
        file_id, //The mapped file id to which the stream belongs
        stream_id, //The index of the stream in the file
        split; //How many times the stream must be split
    wxString tag; //The stream tag used for filtering [X:Y]
    wxString data; //Used by FormatCuts
    wxString codec; //Used to identify the source codec
    TIME_VALUE dur; //Stream duration
    FFQ_CUTS cuts; //A list of cuts to perform

    SBINFO(int type, int file_id, int stream_id, wxString tag, FFQ_CUTS cuts, TIME_VALUE dur, wxString codec)
    : type(type), file_id(file_id), stream_id(stream_id), split(0), tag(tag), data(wxEmptyString), codec(codec), dur(dur), cuts(cuts)
    {};

    SBINFO(wxString tag, const int no_value = std::numeric_limits<int>::max()) //This constructor is used as a placeholder for stream mappings / tags only
    : type(no_value), file_id(no_value), stream_id(no_value), split(0), tag(tag), data(wxEmptyString), codec(wxEmptyString), dur(TIME_VALUE(0)), cuts(FFQ_CUTS())
    {};

} SBINFO, *LPSBINFO;

//---------------------------------------------------------------------------------------

void SelectStreams(wxArrayPtrVoid &src, wxArrayPtrVoid &dst, int file_id, int stream_type)
{
    //This will copy the wanted stream info's from "src" to "dst"
    dst.Clear();
    for (size_t i = 0; i < src.Count(); i++)
    {
        LPSBINFO si = (LPSBINFO)src[i];
        if ((si->file_id == file_id) || (si->type == stream_type)) dst.Add(si);
    }
}

//---------------------------------------------------------------------------------------

wxString MakeCutsForStreams(wxArrayPtrVoid &streams, int &filter_id, bool first, wxArrayInt *cutted_files = 0)
{

    //This is used to make cuts for the streams in the list, minding
    wxString res;

    for (size_t i = 0; i < streams.GetCount(); i++)
    {
        //Get the stream info
        LPSBINFO sbi = (LPSBINFO)streams[i];

        //Skip to next stream if no cuts are available or if they are placed last
        if (sbi->cuts.cuts.Len() > 0)
        {

            //Skip if filters are not at the right position
            if (first != (sbi->cuts.quick || sbi->cuts.filter_first)) continue;

            //Select the streams
            wxArrayPtrVoid sel_streams;
            SelectStreams(streams, sel_streams, sbi->file_id, -1);

            //Make the cuts
            res += FormatCuts(sel_streams, filter_id, cutted_files);

        }

    }

    return res;

}

//---------------------------------------------------------------------------------------

wxString MakeStreamSplits(wxArrayPtrVoid &streams, int &filter_id)
{

    //Create split filters for streams
    wxString res;

    for (size_t i = 0; i < streams.Count(); i++)
    {

        LPSBINFO sbi = (LPSBINFO)streams[i];

        //Must the stream be split?
        if (sbi->split > 0)
        {
            sbi->split++; //1=2, 2=3 etc.
            res += sbi->tag + wxString::Format("%ssplit=%d", sbi->type == 0 ? "" : "a", sbi->split);
            sbi->tag.Clear();
            for (int ii = 0; ii < sbi->split; ii++)
            {
                wxString s = NextFilterID(filter_id);
                sbi->tag += s + SPACE;
                res += s;
            }
            res += SCOLON;
            sbi->tag.RemoveLast();
        }

    }

    return res;

}

//---------------------------------------------------------------------------------------

/*void TerminateFilterTags(wxArrayPtrVoid &streams, wxString &filters)
{

    //This will make sure that here are no output tags in the filters
    //that would require to mapped with "-map" to the output file

    //If either "concat" or "split" is present, the tags must be
    //terminated with a copy filter in order to maintain the correct
    //position in the output file. If none are present, just remove
    //the tags..
    bool use_copy = (filters.Find("]concat=") > 0) || (filters.Find("]split=") > 0);

    for (size_t i = 0; i < streams.Count(); i++)
    {

        LPSBINFO sbi = (LPSBINFO)streams[i];
        wxString tags = sbi->tag, tag; //tags may be a space-separated list
        sbi->tag.Clear();

        while (tags.Len() > 0)
        {

            //Terminate the tag if present, else add it back to the streams tag list
            tag = GetToken(tags, SPACE);
            if (use_copy)
            {
                if (filters.Find(tag) > 0) filters+= tag + (sbi->type == 0 ? "" : "a") + "copy;";
                else sbi->tag += tag + SPACE;
            }
            else if (filters.Replace(tag, "") <= 0) sbi->tag += tag + SPACE;

        }

        sbi->tag.RemoveLast(); //Remove last SPACE

    }

}*/

//---------------------------------------------------------------------------------------

wxString MakeOutputMapping(wxArrayPtrVoid &streams)
{
    wxString res;
    for (size_t i = 0; i < streams.Count(); i++)
    {
        LPSBINFO sbi = (LPSBINFO)streams[i];

        #ifdef DEBUG
        //FFQConsole::Get()->AppendLine(wxString::Format("%u:%u = %s", sbi->file_id, sbi->stream_id, sbi->tag), COLOR_BLUE);
        #endif // DEBUG

        if (sbi->tag.Len() == 0) res += wxString::Format("-map %u:%u ", sbi->file_id, sbi->stream_id);
        else
        {
            wxString tags = sbi->tag;
            while (tags.Len() > 0)
            {
                wxString tag = GetToken(tags, SPACE);
                if (tag.StartsWith('[') && (tag.Find(COLON) > 0)) res += "-map " + tag.AfterFirst('[').BeforeLast(']') + " ";
                else res += "-map \"" + tag + "\" ";
            }
        }
    }
    return res;
}

//---------------------------------------------------------------------------------------

int IndexOfStreamByType(wxString stream_map, wxString codec_type, int file_id, int stream_id)
{

    //Used to find the index of the stream "stream_id" in file "file_id" based
    //on the codec type (aud, vis, subs etc).
    //This may seem klonky but the user may have changed the order of the streams,
    //so this is the only reliable way of getting the per-type index of a stream

    STREAM_MAPPING smap;
    wxArrayInt sids;
    while (stream_map.Len() > 0)
    {
        smap.Parse(stream_map);
        //Subtract 1 from file_id since they start by 1 and not 0
        if ((smap.file_id - 1 == file_id) && (smap.codec_type == codec_type)) sids.Add(smap.stream_id);
    }

    //Restore the correct stream order
    sids.Sort([](int a, int b) { return a - b; });

    //Return the per-type index or wxNOT_FOUND
    return sids.Index(stream_id);

}

//---------------------------------------------------------------------------------------

wxString BuildCommandLine(LPFFQ_JOB job, long &encoding_pass, bool for_preview, bool audio_filters_complex)
{

    //Is the preset the only thing to make?
    bool preset_only = job->skip_in_files >= 0; //job->in1.Len() == 0;

    //Handy variables
    wxString s, t, input_args;

    //List of input files and their arguments
    wxArrayString input_list;
    //if (!preset_only) MakeInputFileArgs(job->inputs, input_list, true); //Moved to stream map handling

    //Array that hold a list of SBINFO for each audio &video stream in the job
    wxArrayPtrVoid streams, sel_streams;

    //Integers that define the first entry of each stream type in the list
    int first_vid_idx = -1, first_aud_idx = -1, vid_count = 0, aud_count = 0;

    //Integers that define the current entry of each stream type in the list
    int cur_vid_idx = -1, cur_aud_idx = -1, cur_sub_idx = -1;

    //Variables used for required inputs for filters, subtitle filter, mapping & preset
    wxString sub_in, req_in, sub_filter, mapping, preset, aud_extra, subs_extra, metadata_extra, disposition;

    //Target extension
    wxString target_ext = job->out.AfterLast(DOT).Lower();

    //Find the preset to be used for this job
    LPFFQ_PRESET pst = (job->preset_ptr != NULL) ? (LPFFQ_PRESET)job->preset_ptr : FFQPresetMgr::Get()->GetPreset(job->preset_id.ToString());

    //Create input arguments for HW decoding
    input_args = (pst == NULL) ? "" : MakeHWDecodeArgs(pst->hw_decode);

    //If preset is used and preset uses two-pass EncodingPass must be incremented (0 for new job)
    if ((!for_preview) && (pst != NULL) && pst->two_pass) encoding_pass++;

    //Do we need to add segmenting arguments?
    //Segmenting is only performed when not previewing and for single pass / second pass
    bool segment = (!for_preview) && ((encoding_pass==0) || (encoding_pass==2)) && (pst != NULL) && (pst->segmenting.length > 0);

    //Get the stream mapping from the job
    wxString sm = job->stream_map;

    //Variables used to determine how the command line must be constructed
    bool subs_burn_in = pst && (pst->subtitles.codec == CODEC_SUBS_BURNIN);
    uint8_t skip_encode_same = pst ? pst->skip_encode_same : 0;
    //bool mapped_subs = false;
    bool cuts_allowed = true;

    if (preset_only)
    {

        //When preset only is being build the streamMap only contains a
        //simple [VIDID],[AUDID] mapping

        streams.Add(new SBINFO(0, 0, 0, /*0,*/ GetToken(sm, COMMA), FFQ_CUTS(), 0, wxEmptyString));
        first_vid_idx = 0;

        if (sm.Len() > 0)
        {
            streams.Add(new SBINFO(1, 0, 0, /*0,*/ GetToken(sm, COMMA), FFQ_CUTS(), 0, wxEmptyString));
            first_aud_idx = 1;
        }


        //vid_in = GetToken(sm, ',');
        //aud_in = GetToken(sm, ',');

        //Add empty strings to the list of input files in order to compensate
        //for the amount of input files that should be skipped in order to
        //maintain valid use of requested input files by filters (req_in)
        long in_count = job->skip_in_files;
        while (in_count-- > 0) input_list.Add("");
        /*{
            input_list.Add("");
            in_count--;
        }*/

    }
    else
    {

        //Create stream mapping for command line
        STREAM_MAPPING smap;
        //int sub_stream_idx = -1;//, file_id_add = 0;

        //Before we can convert the stream mapping to a stream list, we need to know whether there are
        //any mapped subtitles, which would disable cuts, and whether there are any cuts, which would
        //disable conditional encoding (skip encode to same format)

        //NOTE: Mapping subtitles will not cause errors, but they may produce files which
        //seems broken if the length (time) of the subtitles are exceeding the length (time)
        //of the cut streams. For now, this is left to the user to handle
        int /*num_subs = 0,*/ num_cuts = 0;
        while (sm.Len() > 0)
        {
            smap.Parse(sm);
            if (!smap.checked) continue;
            //if (smap.codec_type == CODEC_TYPE_SUBTITLE) num_subs++;
            FFQ_INPUT_FILE inf = job->GetInput(smap.file_id - 1);
            if (inf.cuts.cuts.Len() > 0) num_cuts++;
        }
        //bool mapped_subs = subs_burn_in ? num_subs > 1 : num_subs > 0; // = num_subs > (int)subs_burn_in;
        //if (mapped_subs) cuts_allowed = false; //If mapped subs, disable cuts
        if (cuts_allowed && (num_cuts > 0)) skip_encode_same = 0; //If cuts, disable conditional encoding

        //Make input file arguments..
        MakeInputFileArgs(job->inputs, input_list, cuts_allowed);

        //Make mapped file indices
        wxArrayInt file_idc;
        MakeMappedFileIndicies(job, file_idc, cuts_allowed);

        //Now we are ready to feast on all them delicious streams of mappings..
        sm = job->stream_map;
        while (sm.Len() > 0)
        {

            //This is an actual command line build and therefore the stream
            //mapping is fully specified

            smap.Parse(sm);
            smap.file_id--; //Decrement file_id since it starts from 1 in FFQ and 0 in FFM
            int map_id = file_idc[smap.file_id];

            if (smap.checked)
            {

                #ifdef DEBUG
                //FFQConsole::Get()->AppendLine("SMAP=" + smap.ToString(), COLOR_BLACK);
                #endif

                //Get file info
                FFQ_INPUT_FILE inf = job->GetInput(smap.file_id);

                //Make stream tag(s)
                wxString smcur = wxString::Format("%d:%d", map_id, smap.stream_id), smxtra = "";

                //Stream info for the current stream
                LPSBINFO sbi = nullptr;

                //What type of stream is it?
                bool is_video    = smap.codec_type == CODEC_TYPE_VIDEO,
                     is_audio    = smap.codec_type == CODEC_TYPE_AUDIO,
                     is_subtitle = smap.codec_type == CODEC_TYPE_SUBTITLE;

                //If a file is added more than once to the command line (for quick
                //cutting etc.) there must be added extra mappings for audio and video
                int num_keep_parts = 0;
                if (cuts_allowed && (is_video || is_audio))
                {

                    num_keep_parts = GetKeepParts(inf);

                    //If quick cuts, add extra mappings
                    if (inf.cuts.quick) for (int add = 1; add < num_keep_parts; add++) smxtra += wxString::Format(" %d:%d", map_id + add, smap.stream_id);

                }

                if (is_video)
                {

                    //Add stream
                    sbi = new SBINFO(0, map_id, smap.stream_id, /*++cur_vid_idx,*/ "[" + smcur + "]", inf.cuts, inf.duration, smap.codec_name);
                    streams.Add(sbi);
                    cur_vid_idx++;

                    //Set index of first video stream
                    if (first_vid_idx < 0) first_vid_idx = cur_vid_idx;

                    //Increase number of video streams
                    vid_count++;

                }

                else if (is_audio)
                {

                    //As with video
                    sbi = new SBINFO(1, map_id, smap.stream_id, /*++cur_aud_idx,*/ "[" + smcur + "]", inf.cuts, inf.duration, smap.codec_name);
                    streams.Add(sbi);
                    cur_aud_idx++;
                    if (first_aud_idx < 0) first_aud_idx = cur_aud_idx;

                    //Prevent audio from being mapped into two pass encode
                    if (encoding_pass == 1) continue;

                    //Since one audio-stream can be mapped multiple times with
                    //different codecs, we must handle that here:
                    while (smap.preset_list.Len() > 0)
                    {

                        LPFFQ_PRESET p = FFQPresetMgr::Get()->GetPreset(GetToken(smap.preset_list, SPACE));
                        if (p)
                        {

                            //Make sure that we do not re-encode to same format if this is unwanted
                            if (((skip_encode_same & 1) == 0) || (p->audio_codec_id != smap.codec_name))
                            {

                                if ((inf.cuts.cuts.Len() == 0) && ((pst == NULL) || (pst->GetFilters().Find(FILTER_AUDIO_IN) == wxNOT_FOUND)))
                                {

                                    //Extra mappings must only be added if the stream is not being filtered
                                    for (int add = 0; add < num_keep_parts; add++) smxtra += wxString::Format(" %d:%d", map_id + add, smap.stream_id);

                                }
                                else sbi->split++;//Otherwise split with filter

                                aud_extra += MakeAudioEncoding(p, ++cur_aud_idx);
                                s = wxString::Format("a:%d", cur_aud_idx);
                                metadata_extra += MakeMetaData(p->meta_data_a, s);
                                disposition += MakeDisposition(p->disposition, 1, s);

                            }

                        }
                        //else handle invalid preset error

                    }

                    //Increase number of sudio streams
                    aud_count++;


                }

                else if (is_subtitle)
                {

                    if (sub_in.Len() == 0)
                    {

                        //We are only interested in the first subtitle stream since
                        //that is the one required for subtitle burn in
                        sub_in = "[" + smcur + "]";
                        //sub_stream_idx++;

                        //Map or burn-in?
                        if (subs_burn_in)// (pst != NULL) && (pst->subtitles.codec == CODEC_SUBS_BURNIN))
                        {

                            //Get the index of the subtitle stream within the source file based on type
                            int subs_idx = IndexOfStreamByType(job->stream_map, smap.codec_type, smap.file_id, smap.stream_id);

                            //Create the subtitle burn in filter
                            sub_filter = MakeSubtitleBurninFilter(sub_in, subs_idx, job->GetInput(smap.file_id).path, pst);

                            //Prevent burned-in subtitles from being mapped
                            continue;

                        }
                        else cur_sub_idx++; //else map

                    }

                    //Mapped subtitles does not work well with the preview function
                    //so if not explicitly dictated by the user they are not mapped
                    if (for_preview && !FFQCFG()->preview_map_subs) continue;

                    //As with audio, subtitles may be mapped multiple times with different codecs
                    while (smap.preset_list.Len() > 0)
                    {

                        LPFFQ_PRESET p = FFQPresetMgr::Get()->GetPreset(GetToken(smap.preset_list, SPACE));
                        if (p != nullptr)// && (p->subtitles.codec != CODEC_SUBS_BURNIN))
                        {
                            smxtra += wxString::Format(" %d:%d", map_id, smap.stream_id);
                            s = wxString::Format("s:%d", ++cur_sub_idx);
                            subs_extra += wxString::Format("-c:%s %s ", s, p->subtitles.codec);
                            metadata_extra += MakeMetaData(p->meta_data_s, s);
                            disposition += MakeDisposition(p->disposition, 2, s);
                        }
                        //else handle invalid preset error

                    }

                    //Prevent cuts if subtitles are mapped
                    //mapped_subs = true;

                }

                //If the stream has not been added, it must be added here to ensure a valid
                //output stream mapping for subtitles and attachments
                if (sbi == nullptr) streams.Add(new SBINFO(smcur + smxtra));

                //Add stream(s) to (input) mapping: Obsolete?
                smcur += smxtra;
                while (smcur.Len() > 0) mapping += "-map " + GetToken(smcur, SPACE) + SPACE;

            }

            //else if (smap.codec_type == CODEC_TYPE_SUBTITLE) sub_stream_idx++; //Beware of teletext!

        }

    }

    //Handy vars for detecting included content
    bool has_video = (first_vid_idx >= 0),

         //Audio skipped in first pass
         has_audio = (first_aud_idx >= 0) && (encoding_pass != 1),

         //Subtitles skipped in first pass if not burned in
         has_subs = (sub_in.Len() > 0) && ( (encoding_pass != 1) || (sub_filter.Len() > 0) ),

         //Are we performing a pass in a multi-pass encoding?
         two_pass = (!preset_only) && (!for_preview) && (encoding_pass > 0);

    //Format preset if available and required by the command line
    if ((pst != NULL) && (job->cmd_line.Find(CMD_PRESET) >= 0))
    {

        //Used to prevent switches that are useless for stream copy
        bool copy_vid = (pst->video_codec == CODEC_COPY),
             copy_aud = (pst->audio_codec == CODEC_COPY);

        //Used to prevent cuts from being added if they cannot be performed
        bool can_cut  = cuts_allowed /*(!mapped_subs)*/ && ((!has_video) || (!copy_vid)) && ((!has_audio) || (!copy_aud));

        //filterID is a counter used to create unique filter identifiers
        int filter_id = 1;

        //Adjust audio_filter_complex according to preset
        if (pst->af_complex) audio_filters_complex = true;

        //To prevent the streams in one file to be cut twice
        wxArrayInt cutted_files;

        //Make filters for all streams
        wxString vf, af; //Video filters and audio filters
        wxArrayString acodecs; //Used to keep track of which audio codecs have been added to the output
        SBINFO dummy(0, 0, 0, /*0,*/ "", FFQ_CUTS(), 0, wxEmptyString); //Dummy used as empty pointer
        LPSBINFO first_vid = (has_video ? (LPSBINFO)streams[first_vid_idx] : &dummy),
                 first_aud = (has_audio ? (LPSBINFO)streams[first_aud_idx] : &dummy);

        //Make cuts that are placed first in the filtergraph
        if (can_cut) vf += MakeCutsForStreams(streams, filter_id, true, &cutted_files);

        for (size_t sidx = 0; sidx < streams.GetCount(); sidx++)
        {

            //Get the stream info
            LPSBINFO sbi = (LPSBINFO)streams[sidx];

            //Select all streams in the same file for cutting
            //if (can_cut) SelectStreams(streams, sel_streams, sbi->file_id, -1);
            //else sel_streams.Clear();

            //Can video filters be applied?
            if ((!copy_vid) && (sbi->type == 0))
            {

                //If the video stream is already the desired type, copy codec is appended to aud_extra
                if ( ((skip_encode_same & 2) != 0) && (sbi->codec == pst->video_codec_id) )
                {

                    if (vid_count > 1) aud_extra += wxString::Format("-c:%d %s ", (int)sidx, CODEC_COPY);
                    else
                    {
                        //two_pass = false; //Two pass will not work for copy video
                        copy_vid = true; //Do not apply video encoding settings
                    }

                }
                else
                {

                    //Since subtitles can be burned in to multiple streams,
                    //we need a var for a copy of the subs_filter
                    wxString subs = sub_filter.Clone();

                    //Add cuts first?
                    //if (can_cut && (sbi->cuts.filter_first || sbi->cuts.quick)) vf += FormatCuts(sel_streams, filter_id, &cutted_files);

                    //Apply all other video filters from the presets filter list
                    for (size_t fidx = 0; fidx < pst->filters.Count(); fidx++)
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
                                if (fltr.required_file.loop.Len() > 0) s = "-loop " + fltr.required_file.loop + SPACE + s;
                                wxString tag = fltr.required_file.tag;
                                if (tag.Len() > 0) tag = COLON + tag;
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
                    //if (can_cut && (!sbi->cuts.filter_first) && (!sbi->cuts.quick)) vf += FormatCuts(sel_streams, filter_id, &cutted_files);

                }

            }

            else if (/*(!copy_aud) &&*/ (sbi->type == 1))
            {

                //Can audio filters be applied?
                if (!copy_aud)
                {

                    //Skip re-encoding if codec is the same?
                    if ((skip_encode_same & 1) && (pst->audio_codec_id == sbi->codec))
                    {

                        if (aud_count > 1) aud_extra += wxString::Format("-c:%d %s ", (int)sidx, CODEC_COPY);
                        else copy_aud = true;

                    }
                    else
                    {

                        //Prepare audio cuts
                        //wxString cuts = (can_cut && FormatCuts(sbi->cuts, sbi->tag, false, sbi->dur, filter_id)) ? sbi->cuts.cuts : "";
                        //bool has_cuts = sbi->cuts.cuts.Len() > 0;
                        bool has_cuts = can_cut && (sbi->cuts.cuts.Len() > 0);

                        //Cuts requires complex audio filtering
                        if (has_cuts || (sbi->split > 0)) audio_filters_complex = true;

                        //Apply audio cutting first?
                        /*if (has_cuts && (sbi->cuts.filter_first || sbi->cuts.quick))
                        {
                            af += FormatCuts(sel_streams, filter_id, &cutted_files);
                            has_cuts = false; //Prevent add last
                        }*/

                        //Process audio filters from the list
                        for (size_t fidx = 0; fidx < pst->filters.Count(); fidx++)
                        {

                            FFMPEG_FILTER fltr = FFMPEG_FILTER(pst->filters[fidx]);
                            if (fltr.IsAudio() && FormatFilter(fltr.ff_filter, first_vid->tag, sbi->tag, sub_in, req_in, filter_id))
                                af += fltr.ff_filter;

                        }

                        //Apply audio cutting last?
                        //if (has_cuts && (!sbi->cuts.filter_first)) af += FormatCuts(sel_streams, filter_id, &cutted_files);

                    }

                }

                //Must the stream be split?
                /*if (sbi->split > 0)
                {
                    sbi->split++; //1=2, 2=3 etc.
                    af += sbi->tag + wxString::Format("asplit=%d", sbi->split);
                    sbi->tag.Clear();
                    for (int tidx = 0; tidx < sbi->split; tidx++)
                    {
                        s = NextFilterID(filter_id);
                        sbi->tag += s + SPACE;
                        af += s;
                    }
                    af += SCOLON;
                    sbi->tag.RemoveLast();
                }*/

                //See comment for the same thing done on vf above
                //if (af.Right(1) == SCOLON) af = af.BeforeLast('[') + SCOLON;

            }

        }

        //Adding filters with -af is currently borked, so audio filters must be passed as complex
        //if (audio_filters_complex)
        //{

            //If audio filters are to be added to -filter_complex they are appended to the video filters
            vf += af;
            af.Clear();

        //}

        //Make cuts that are placed last in the filtergraph
        if (can_cut) vf += MakeCutsForStreams(streams, filter_id, false, &cutted_files);

        vf += MakeStreamSplits(streams, filter_id);

        //Terminate output tags
        //TerminateFilterTags(streams, vf);
        //TerminateFilterTags(streams, af);

        //Now we need to add the filters to the preset
        if (vf.Len() > 0)
        {

            //Remove last ";" and add as complex filters
            vf.RemoveLast();
            preset += "-filter_complex \"" + vf + "\" ";

            //Since -filter_complex is used, we must map the pads / tags to the output
            preset += MakeOutputMapping(streams);

            //And prevent the input mapping from being used
            mapping.Clear();

            /*

            ALL THIS JUNK is not necessary anymore, is it? Better ask Sherlock..

            //Remove any output tags
            //RemoveOutputTags(streams, vf);

            //Set video filters, always complex
            preset = "-filter_complex \"" + vf;

            //Add complex audio filters?
            if (audio_filters_complex && (af.Len() > 0))
            {

                //Remove any output tags
                //RemoveOutputTags(streams, af);
                preset += af;
                af.Clear(); //Prevent from being re-added

            }

            //Remove trailing ";" and close filter_complex
            preset.RemoveLast();
            preset += "\" ";
            */

        }

        if (af.Len() > 0)
        {

            //Audio filters have not been added using complex filters,
            //so we must remove the beginning and ending pads / tags
            af = af.AfterFirst(']').BeforeLast('[');

            //And all the tags within the chain must be replaced with comma
            int pp = af.Find("];[");
            while (pp >= 0)
            {
                af = af.substr(0, pp).BeforeLast('[') + COMMA + af.substr(pp+3).AfterFirst(']');
                pp = af.Find("];[");
            }

            //Add filter(s) with -af - note that input mapping is preserved
            preset += "-af \"" + af + "\" ";

        }

        //Release memory, this cannot be done in the loop above, since it might
        //result in first_vid and / or first_aud as being bad memory
        for (size_t sidx = 0; sidx < streams.GetCount(); sidx++) delete (LPSBINFO)streams[sidx];
        streams.Clear();

        //Video codec
        if (has_video && (pst->video_codec.Len() > 0)) preset += "-c:v " + (copy_vid ? CODEC_COPY : pst->video_codec) + SPACE;

        if ((!copy_vid) && has_video)
        {

            //Video rate
            s = pst->video_rate;
            if (FormatStreamRate(s, true)) preset += s;//(pst->videoVBR ? "-qscale:v " : "-b:v ") + s + " ";

            //Video quality
            s = pst->video_quality;
            if (FormatVideoQuality(s, pst->video_codec)) preset += s;

            //Trellis
            if (pst->trellis.Len() > 0) preset += "-trellis " + pst->trellis + SPACE;

            //Threads
            if (pst->threads.Len() > 0) preset += "-threads " + pst->threads + SPACE;

            //Video speed preset
            if (pst->speed_preset.Len() > 0) preset += "-preset:v " + pst->speed_preset + SPACE;

            //Video tuning
            if (pst->video_tuning.Len() > 0) preset += "-tune " + pst->video_tuning + SPACE;

            //Key frame, motion and scene change arguments
            s = pst->key_frames;
            if (MakeKeyframeArguments(s)) preset += s;

            //Frames per second
            if (pst->frame_rate.Len() > 0) preset += "-r " + pst->frame_rate + SPACE;

            //Pixel format
            if (pst->pixel_format.Len() > 0) preset += "-pix_fmt " + pst->pixel_format + SPACE;

            //Video sync
            if (pst->video_sync.Len() > 0) preset += "-vsync " + pst->video_sync + SPACE;

            //Closed GOP when segmenting - this should be a user choice in a future release
            //if (segment) preset += "-flags +cgop ";

            //Full specification video arguments
            if (pst->fullspec_vid.Len() > 0)
            {

                wxString fsv = pst->fullspec_vid;

                //Make it possible to integrate two-pass encoding into full-spec settings
                if (two_pass && fsv.StartsWith(X265_PARAMS)) fsv = fsv.BeforeFirst(DQUOTE) + DQUOTE + TWO_PASS_INSERT_POS + fsv.AfterFirst(DQUOTE);

                //Apply full spec to preset
                preset += fsv + SPACE;

            }

        }

        //Audio encoding
        if (has_audio) preset += copy_aud ? ("-c:a " + CODEC_COPY + SPACE) : MakeAudioEncoding(pst, -1);
        preset += aud_extra;

        //Subtitle encoding
        if (has_subs && (pst->subtitles.codec.Len() > 0) && (pst->subtitles.codec != CODEC_SUBS_BURNIN))
            preset += "-c:s " + pst->subtitles.codec + SPACE;
        preset += subs_extra;

        //Segmenting - note that "faststart" needs to be set different when segmenting
        if (segment)
        {
            uint64_t st = pst->segmenting.length * 1000;
            if (pst->segmenting.length_type > 0) st *= 60;
            if (pst->segmenting.length_type > 1) st *= 60;
            TIME_VALUE sl(st);
            preset += "-segment_time " + sl.ToShortString() + SPACE;
            preset += "-break_non_keyframes " + wxString(pst->segmenting.break_bframes ? "1" : "0") + SPACE;
            preset += "-increment_tc " + wxString(pst->segmenting.incremental_tc ? "1" : "0") + SPACE;
            preset += "-reset_timestamps " + wxString(pst->segmenting.reset_ts ? "1" : "0") + SPACE;
            if (pst->mf_faststart) preset += "-segment_format_options movflags=+faststart ";
        }

        //No segmenting, use default faststart
        else if (pst->mf_faststart) preset += "-movflags faststart ";

        if (target_ext == "avi")
        {
            //FourCC - only for AVI containers
            if (pst->fourcc.vids.Len() == 4) preset += "-vtag " + pst->fourcc.vids + SPACE;
            if (pst->fourcc.auds.Len() == 4) preset += "-atag " + pst->fourcc.auds + SPACE;
        }

        //Disposition
        preset += MakeDisposition(pst->disposition, 0, "v");
        preset += MakeDisposition(pst->disposition, 1, "a");
        preset += MakeDisposition(pst->disposition, 2, "s");
        preset += disposition;

        //Meta data
        preset += MakeMetaData(pst->meta_data, "");
        preset += MakeMetaData(pst->meta_data_v, "v");
        preset += MakeMetaData(pst->meta_data_a, "a");
        preset += MakeMetaData(pst->meta_data_s, "s");
        preset += metadata_extra;

        /*if (pst->meta_data.Len() > 0)
        {
            s = pst->meta_data;
            while (s.Len() > 0) preset += "-metadata " + GetToken(s, FILTER_SEPARATOR) + " ";
        }*/

        //Aspect ratio - does not need encoding - just video
        if (has_video && (pst->aspect_ratio.Len() > 0)) preset += "-aspect " + pst->aspect_ratio + SPACE;

        //Error detection
        if (pst->error_detect.Len() > 0) preset += "-err_detect " + pst->error_detect + SPACE;

        //End after shortest stream ends?
        if (pst->shortest) preset += "-shortest ";

        //Append FullSpec from user extended GUI
        if (pst->fullspec_user.Len() > 0) preset += pst->fullspec_user + SPACE;

        //Append the proper custom arguments
        if (for_preview && (pst->custom_args_1.Len() > 0)) s = pst->custom_args_1;
        else if ((encoding_pass < 2) && (pst->custom_args_1.Len() > 0)) s = pst->custom_args_1;
        else if ((encoding_pass > 1) && (pst->custom_args_2.Len() > 0)) s = pst->custom_args_2;
        else s = wxEmptyString;

        //Split into input_args and args added to preset
        int p = s.Find("||");
        if (p == 0) s.Remove(0, 2);
        else if (p >= 0)
        {
            input_args += s.SubString(0, p - 1) + SPACE;
            s.Remove(0, p + 2);
        }

        preset += s + SPACE;

    }

    //String for formatting the resulting command line
    s = job->cmd_line;

    //Check if banner must be hidden
    if (FFQCFG()->hide_banner) s = "-hide_banner " + s;

    //Convert inputList to string and insert inputs & mapping to command line
    t = "";
    for (size_t i = 0; i < input_list.GetCount(); i++) if (input_list[i].Len() > 0) t += input_list[i];

    #ifdef DEBUG
    //ShowInfo(t);
    #endif // DEBUG

    //Make sure that CMD_INPUTS is not removed if presetOnly
    if (preset_only) t = CMD_INPUTS + t;

    //Insert inputs & mapping to command line
    CommandLineReplace(s, CMD_INPUTS, input_args + t + mapping);

    //Insert preset
    CommandLineReplace(s, CMD_PRESET, preset);

    //If only the preset is being build - we are done
    if (preset_only) return s;

    //Create output-file - depending on multipass (and what pass) or singlepass
    if (two_pass) //(!for_preview) && (encoding_pass > 0))
    {

        //Get name of two-pass log file
        GetTwoPassLog(job, t);

        //Add pass number and passlogfile
        if (pst && (pst->video_codec.Find("x265") != wxNOT_FOUND))
        {

            //x265 handles multiple passes different, so we need to handle this here
            t = "pass=" + ToStr(encoding_pass)+ ":stats='" + FormatFileName(t) + "'";
            if (s.Replace(TWO_PASS_INSERT_POS, t + COLON, false) == 1) t = wxEmptyString;
            else
            {
                //t = "pass=" + ToStr(encoding_pass) + ":stats=\"'" + FormatFileName(t) + "'\" ";
                t = X265_PARAMS + SPACE + "\"" + t + "\"" + SPACE;
            }

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

    if ((!for_preview) && (encoding_pass == 1) && two_pass)
    {

        two_pass_null = (pst != NULL) && pst->two_pass_null; //pst should never be NULL here - but just in case..
        t += "-an "; //No audio for first pass
        if (two_pass_null) fmt = "null"; //Override output format to null for first pass

    }

    //Add time-limit to encode
    if (!job->out_len.IsUndefined()) t += "-t " + job->out_len.ToString() + SPACE;

    if (for_preview)
    {

        //Is the command safe for previewing?
        IsPreviewSafe = IsCommandSafe(preset);

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
            if (fmt.Len() > 0) t += "-segment_format " + fmt + SPACE;

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
        else if (fmt.Len() > 0) t += "-f " + fmt + SPACE;

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
    GetTwoPassLog(job, plf);

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
        _U(s, SCOLON);
        _U(s, COMMA);
        _U(s, ']');
        _U(s, '[');
        _U(s, '\'');
        _U(s, '\\');

        //First level
        _U(s, '\'');
        _U(s, COLON);
        _U(s, '\\');

    }
    else
    {

        //First level
        _E(s, '\\');
        _E(s, COLON);
        _E(s, '\'');
        //Second level
        _E(s, '\\');
        _E(s, '\'');
        _E(s, '[');
        _E(s, ']');
        _E(s, COMMA);
        _E(s, SCOLON);

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

wxString FormatCuts(wxArrayPtrVoid &streams, int &filter_id, wxArrayInt *unique_files_only)
{

    //Converts a list of from_time;to_time[..] to a filter
    //that utilizes the trim & concat filters for video
    //and audio

    //Exit if streams is empty
    if (streams.Count() == 0) return wxEmptyString;

    //Get info for the first stream
    LPSBINFO sbi = (LPSBINFO)streams[0];

    //Check if the file has already been processed
    if (unique_files_only)
    {
        //Exit if processed
        if (unique_files_only->Index(sbi->file_id) > -1) return wxEmptyString;

        //Add if not
        unique_files_only->Add(sbi->file_id);
    }

    //Used to correct time stamps after trimming
    const wxString SETPTS = "%ssetpts=PTS-STARTPTS";

    //Prepare the cuts with initial vars
    wxString cuts = sbi->cuts.KeepParts(sbi->dur), s, t, res;
    if (cuts.Len() == 0) return "";
    bool quick = sbi->cuts.quick, use_split;
    int num_parts = (cuts.Freq(SCOLON) + 1) / 2;
    size_t i;

    //Make splits and input tags
    if (num_parts == 1)
    {

        //No splitting necessary
        for (i = 0; i < streams.Count(); i++)
        {
            sbi = (LPSBINFO)streams[i];
            sbi->data = sbi->tag;
        }

    }
    else
    {

        sbi->data.Clear(); //Used to store stream tags
        sbi->cuts.Reset(); //Prevent cuts from being re-created for other streams

        for (i = 0; i < streams.Count(); i++)
        {

            //Get stream info and create a split filter if required
            sbi = (LPSBINFO)streams[i];

            //If the stream has been filtered, it must be split
            use_split = sbi->tag.Freq(COLON) == 0;

            if ((!quick) && use_split) res += wxString::Format("%s%ssplit=%d", sbi->tag, sbi->type==0 ? "" : "a", num_parts);

            for (int p = 0; p < num_parts; p++)
            {

                //If quick then we have to use the mapped tags
                if (quick) sbi->data += wxString::Format("[%d:%d] ", sbi->file_id + p, sbi->stream_id);

                else if (use_split)
                {

                    //Split has to be used if the input tag is the output from another filter
                    //which may be the case if cuts are placed last
                    s = NextFilterID(filter_id);
                    sbi->data += s + SPACE;
                    res += s;

                }

                //If neither quick nor split we just add the tag
                else sbi->data += sbi->tag + SPACE;

            }

            if ((res.Len() > 0) && (res.Right(1) != SCOLON)) res += SCOLON;

        }

    }


    //Array to store concat tags
    TIME_VALUE from, to;
    wxArrayString cc_tags;
    cc_tags.SetCount(num_parts);

    //Create the trim filters for all streams
    for (i = 0; i < streams.Count(); i++)
    {

        sbi = (LPSBINFO)streams[i];
        wxString cc = cuts, tag, pts;
        int part = 0;

        //Create the filters
        while (sbi->data.Len() > 0)
        {

            from = TIME_VALUE(GetToken(cc, SCOLON));
            to = cc.Len() == 0 ? sbi->dur : TIME_VALUE(GetToken(cc, SCOLON));
            t = sbi->type == 0 ? "" : "a"; //Type
            tag = GetToken(sbi->data, SPACE, true);
            pts = (from.ToMilliseconds() > 0) ? wxString::Format(SETPTS, t) : ""; //If the setpts filter is used for offset 0, audio comes out of sync
            s = (!quick) || (pts.Len() > 0) ? NextFilterID(filter_id) : tag;
            cc_tags[part++] += s;

            if (quick && (pts.Len() > 0)) res += tag + pts + s + SCOLON;
            else if (!quick)
            {
                if (pts.Len() > 0) pts = COMMA + pts;
                res += wxString::Format("%s%strim=%s:%s%s%s;", tag, t, from.ToShortString(), to.ToShortString(), pts, s);
            }

        }

    }


    //Create the concat filter for multiple parts
    for (size_t i = 0; i < cc_tags.Count(); i++) res += cc_tags[i];
    int vids = 0, auds = 0;
    t = "";
    for (size_t i = 0; i < streams.Count(); i++)
    {
        LPSBINFO sbi = (LPSBINFO)streams[i];
        sbi->tag = NextFilterID(filter_id); //Output stream tag for the concat filter
        if (sbi->type == 0) vids++;
        else auds++;
        t += sbi->tag;
    }

    //Clear streams to prevent re-processing
    //streams.Clear();

    return res + wxString::Format("concat=n=%d:v=%d:a=%d%s;", num_parts, vids, auds, t);

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
    if (filter.Right(1) != SCOLON) filter += SCOLON;

    //All green
    return true;

}

//---------------------------------------------------------------------------------------

bool FormatStreamRate(wxString &rate, bool video, wxString stream_tag)
{

    //Converts the settings from the FFQBitRatePanel into command line arguments

    //Extract values
    long abr = Str2Long(GetToken(rate, COMMA), 0),
         brt = Str2Long(GetToken(rate, COMMA), 1),
         min = Str2Long(GetToken(rate, COMMA), 0),
         max = Str2Long(GetToken(rate, COMMA), 0),
         buf = Str2Long(GetToken(rate, COMMA), 0);

    //Convert values to command line arguments
    rate = BitRateToArg(abr, brt, "b", video, stream_tag) +
           BitRateToArg(min, brt, "minrate", video, stream_tag) +
           BitRateToArg(max, brt, "maxrate", video, stream_tag) +
           BitRateToArg(buf, brt, "bufsize", video, stream_tag);

    //Return if any values are available
    return rate.Len() > 0;

}
