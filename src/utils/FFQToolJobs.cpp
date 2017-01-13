/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQToolJobs.cpp                                                 *
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

#include "FFQToolJobs.h"
#include "FFQBuildCmd.h"
#include "FFQMisc.h"
#include "FFQConst.h"
#include "FFQProbing.h"
#include "FFQConfig.h"
#include "FFQJob.h"
#include "FFQConsole.h"
#include "../FFQPresetMgr.h"
#include <wx/filename.h>
#include <wx/file.h>

//---------------------------------------------------------------------------------------

//Constants for loading and saving job values
const wxString THUMB_SETTINGS = "thumbs";

const wxString VIDSTAB_PASS1 = "pass1";
const wxString VIDSTAB_PASS2 = "pass2";
const wxString VIDSTAB_OTHER = "other";
const wxString VIDSTAB_DURATION = "duration";
const wxString VIDSTAB_OUTPUT = "out";
const wxString VIDSTAB_PRESET = "preset";
const wxString VIDSTAB_STREAM_MAP = "stream_map";
const wxString VIDSTAB_TRANSFORM_EXT = ".trf";

const wxString CONCAT_SETTINGS = "concat_settings";
const wxString CONCAT_IMG_PATTERN = "img_pattern";
const wxString CONCAT_MERGE_FILTER = "merge_filter";
const wxString CONCAT_MERGE_PADDING = "merge_padding";
const wxString CONCAT_FLTR_KEY1 = "$F1$";
const wxString CONCAT_FLTR_KEY2 = "$F2$";
const wxString CONCAT_FIT_AND_PAD = "scale='if(gt(iw,ih),$W,-1)':'if(gt(ih,iw),$H,-1)',pad=$W:$H:(ow-iw)/2:(oh-ih)/2:$C";

//VIDSTAB constants
const wxString OPT_ALGOS[VIDSTAB_OPT_ALGO_COUNT] = { "gauss", "avg" };
const wxString INTERPOLATION_VALUES[VIDSTAB_INTERPOLATION_COUNT] = {
    "no", "linear", "bilinear", "bicubic"
};

//---------------------------------------------------------------------------------------

FFQ_THUMB_JOB::FFQ_THUMB_JOB() : FFQ_QUEUE_ITEM()
{

    //Default constructor - reset only
    Reset(false);

}

//---------------------------------------------------------------------------------------

FFQ_THUMB_JOB::FFQ_THUMB_JOB(const FFQ_THUMB_JOB &copy_from) : FFQ_QUEUE_ITEM(copy_from)
{

    //Constructor to clone another job
    Reset(false);

    thumbs = THUMBS_AND_TILES(thumbs);
    //save_log = copy_from.save_log;

}

//---------------------------------------------------------------------------------------

FFQ_THUMB_JOB::FFQ_THUMB_JOB(wxString from) : FFQ_QUEUE_ITEM(from)
{

    //Constructor that parses values from a string
    Reset(true);

}

//---------------------------------------------------------------------------------------

wxString FFQ_THUMB_JOB::ToString()
{

    //Save values
    SetValue(THUMB_SETTINGS, thumbs.ToString());
    //SetValue(SAVE_LOG, BOOLSTR(save_log));

    return FFQ_QUEUE_ITEM::ToString();

}

//---------------------------------------------------------------------------------------

wxString FFQ_THUMB_JOB::GetCommandAtIndex(int index)
{

    //Return next command

    if (index == 0)
    {

        //Probe input file and make the commands
        FFProbeInfoParser *pip = new FFProbeInfoParser();
        bool ok = pip->RunFFProbe(GetInput(0).path, NULL, true);

        if (ok)
        {

            //Get duration of out
            TIME_VALUE tv;
            pip->GetDuration(tv);

            //Build command(s)
            m_Commands = BuildThumbsAndTilesCommand(thumbs, GetInput(0).path, tv, true);


        }
        delete pip;

        if (!ok)
        {

            //Set failed and return empty string
            status = qsFAILED;
            return "";

        }

    }

    //Check if more commands are queued
    if (m_Commands.Len() > 0)
    {

        //Set to active and return next command
        status = qsTHUMBS;
        return GetLine(m_Commands, true);

    }

    //Set status to done
    status = qsDONE;

    //Return empty string; No command
    return wxEmptyString;

}

//---------------------------------------------------------------------------------------

void FFQ_THUMB_JOB::Reset(bool load)
{

    //Reset the values of the job
    thumbs = THUMBS_AND_TILES();

    //save_log = true;
    m_Commands.Clear();

    if (load)
    {

        thumbs = THUMBS_AND_TILES( GetValue(THUMB_SETTINGS, thumbs.ToString()) );
        //save_log = STRBOOL( GetValue(SAVE_LOG, BOOLSTR(save_log)) );

    }

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQ_VIDSTAB_JOB::FFQ_VIDSTAB_JOB() : FFQ_QUEUE_ITEM()
{

    //Default ctor
    Reset(false);

}

//---------------------------------------------------------------------------------------

FFQ_VIDSTAB_JOB::FFQ_VIDSTAB_JOB(const FFQ_VIDSTAB_JOB &copy_from) : FFQ_QUEUE_ITEM(copy_from)
{

    //Copy ctor

    //First pass values
    shakiness = copy_from.shakiness;
    accuracy = copy_from.accuracy;
    step_size = copy_from.step_size;
    tripod_frame = copy_from.tripod_frame;
    min_contrast = copy_from.min_contrast;

    //Second pass values
    smoothing = copy_from.smoothing;
    max_shift = copy_from.max_shift;
    max_angle = copy_from.max_angle;
    zoom = copy_from.zoom;
    zoom_speed = copy_from.zoom_speed;
    opt_algo = copy_from.opt_algo;
    interpolation = copy_from.interpolation;
    opt_zoom = copy_from.opt_zoom;

    crop_black = copy_from.crop_black;
    invert_trf = copy_from.invert_trf;
    relative_trf = copy_from.relative_trf;
    virtual_tripod = copy_from.virtual_tripod;

    //Other options
    only_1st = copy_from.only_1st;
    verbose = copy_from.verbose;
    only_2nd = copy_from.only_2nd;
    keep_trf = copy_from.keep_trf;

    //Destination and preset
    duration = copy_from.duration;
    out = copy_from.out;
    preset = copy_from.preset;
    stream_map = copy_from.stream_map;

}

//---------------------------------------------------------------------------------------

FFQ_VIDSTAB_JOB::FFQ_VIDSTAB_JOB(wxString from) : FFQ_QUEUE_ITEM(from)
{

    //Parse ctor
    Reset(true);

}

//---------------------------------------------------------------------------------------

void FFQ_VIDSTAB_JOB::Cleanup()
{

    //Remove any temporary transformation file
    if ((m_TempFile.Len() > 0) && (wxFileExists(m_TempFile))) wxRemoveFile(m_TempFile);
    m_TempFile.Clear();

    //Copy the file time
    CpFileTime(FFQPresetMgr::Get()->GetPreset(preset.ToString()), out);


}

//---------------------------------------------------------------------------------------

bool FFQ_VIDSTAB_JOB::GetLogFileName(wxString &name)
{

    //Set correct log file name
    name = save_log ? MakeLogFileName(out) : "";
    return name.Len() > 0;

}

//---------------------------------------------------------------------------------------

wxString FFQ_VIDSTAB_JOB::ToString()
{

    //Save values
    wxString s;

    //Pass 1
    s.Printf("%i,%i,%i,%i,%i", shakiness, accuracy, step_size, tripod_frame, min_contrast);
    SetValue(VIDSTAB_PASS1, s);

    //Pass 2
    s.Printf("%i,%i,%i,%i,%i,%i,%i,%i,%s,%s,%s,%s",
             smoothing, max_shift, max_angle, zoom, zoom_speed,
             opt_algo, interpolation, opt_zoom,
             BOOLSTR(crop_black), BOOLSTR(invert_trf),
             BOOLSTR(relative_trf), BOOLSTR(virtual_tripod)
             );
    SetValue(VIDSTAB_PASS2, s);

    //Other options
    s.Printf("%s,%s,%s,%s",
             BOOLSTR(only_1st), BOOLSTR(verbose),
             BOOLSTR(only_2nd), BOOLSTR(keep_trf)
            );
    SetValue(VIDSTAB_OTHER, s);

    //Destination and preset
    SetValue(VIDSTAB_DURATION, duration.ToString());
    SetValue(VIDSTAB_OUTPUT, out);
    SetValue(VIDSTAB_PRESET, preset.ToString());
    SetValue(VIDSTAB_STREAM_MAP, stream_map);

    //Return the whole sh'bang!
    return FFQ_QUEUE_ITEM::ToString();

}

//---------------------------------------------------------------------------------------

wxString FFQ_VIDSTAB_JOB::GetCommandAtIndex(int index)
{

    if ((index == 0) && (!MakeCommands()))
    {
        //Commands could not be created
        status = qsFAILED;
        return wxEmptyString;
    }

    //Update status
    //status = (m_Commands.Len() == 0) ? qsDONE : qsACTIVE;

    //Return next command and set status
    wxString res;

    if (m_Commands.Len() == 0) status = qsDONE;

    else
    {

        res = GetLine(m_Commands, true);
        status = (res.Find("vidstabdetect=") == wxNOT_FOUND) ? qsPASS2 : qsPASS1;

    }

    //Return command
    return res;

}

//---------------------------------------------------------------------------------------

bool FFQ_VIDSTAB_JOB::MakeCommands()
{

    //Clear commands
    m_Commands.Clear();
    m_TempFile.Clear();

    //A job for easier command build
    FFQ_JOB job = FFQ_JOB();
    job.cmd_line = CMD_DEFAULT;// CMD_INPUTS + CMD_PRESET + "-y" + CMD_OUTPUT;

    //Get preset
    LPFFQ_PRESET pst_ptr = FFQPresetMgr::Get()->GetPreset(preset.ToString());
    if ((pst_ptr == NULL) || (pst_ptr->video_codec == CODEC_COPY) || pst_ptr->two_pass) return false;

    //Copy preset
    wxString s = pst_ptr->ToString();
    FFQ_PRESET pst = FFQ_PRESET(s);
    job.preset_ptr = &pst;

    //Copy input file
    FFQ_INPUT_FILE inf = GetInput(0);
    job.inputs.Add(inf.ToString());

    //Set stream mapping
    job.stream_map = stream_map;

    //Get name of transformations file
    wxString trf_file = (only_2nd || keep_trf) ? inf.path + VIDSTAB_TRANSFORM_EXT :
                       FFQCFG()->GetTmpPath(inf.path.BeforeLast(wxFileName::GetPathSeparator()), false, VIDSTAB_TRANSFORM_EXT);
    if (!keep_trf) m_TempFile = trf_file;
    trf_file = EscapeFilterString(trf_file, false);

    //Variables for passes
    wxString p1, p2;

    //Find optimal index for filters
    FFMPEG_FILTER fltr;
    int fidx = 0;
    if (pst.filters.Count() > 0)
    {
        fltr = FFMPEG_FILTER(pst.filters.Item(0));
        if (fltr.type == ftDEINTERLACE) fidx++;
    }
    pst.filters.Insert("", fidx);
    fltr.type = ftUNKNOWN;

    long pass = 0;

    if (!only_2nd)
    {

        //Make 1st pass filter
        p1.Printf("vidstabdetect=result=%s:shakiness=%u:accuracy=%u:stepsize=%u:mincontrast=%g:tripod=%u",
                  trf_file, shakiness, accuracy, step_size, ((double)min_contrast / 100.0), tripod_frame);
        if (only_1st) p1 = p1 + ":show=" + (verbose ? "2" : "1");

        fltr.ff_filter.Printf("%s%s%s", FILTER_VIDEO_IN, p1, FILTER_VIDEO_OUT);

        pst.filters[fidx] = fltr.ToString();
        p1 = BuildCommandLine(&job, pass, false, false);

        //No output file name - must be added here
        p1.Remove(p1.Len() - 2, 2);
        if (only_1st) p1 += "\"" + out + "\"";
        else p1 += "-f null " + NULL_FILENAME;

    }

    if (!only_1st)
    {

        //Make 2nd pass filter
        double ang = (double)max_angle * (3.14159265359 / 180.0);
        p2.Printf("vidstabtransform=input=%s:smoothing=%u:optalgo=%s:maxshift=%i:maxangle=%g:crop=%s:invert=%u:" \
                  "relative=%u:zoom=%i:optzoom=%u:zoomspeed=%g:interpol=%s:tripod=%u",
                  trf_file, smoothing, OPT_ALGOS[opt_algo], max_shift, ang, crop_black ? "black" : "keep", invert_trf ? 1 : 0,
                  relative_trf ? 1 : 0, zoom, opt_zoom, ((double)zoom_speed / 100.0 * 5.0), INTERPOLATION_VALUES[interpolation],
                  virtual_tripod ? 1 : 0);

        fltr.ff_filter.Printf("%s%s%s", FILTER_VIDEO_IN, p2, FILTER_VIDEO_OUT);

        pst.filters[fidx] = fltr.ToString();
        job.out = out;
        p2 = BuildCommandLine(&job, pass, false, false);

    }

    if (p1.Len() > 0) m_Commands += duration.ToString() + "," + p1 + CRLF;
    if (p2.Len() > 0) m_Commands += duration.ToString() + "," + p2 + CRLF;

    m_Commands.RemoveLast(CRLF.Len());

    return true;

}

//---------------------------------------------------------------------------------------

void FFQ_VIDSTAB_JOB::Reset(bool load)
{

    //Reset values
    //First pass values
    shakiness = 5;
    accuracy = 15;
    step_size = 6;
    tripod_frame = 0;
    min_contrast = 30;

    //Second pass values
    smoothing = 10;
    max_shift = -1;
    max_angle = -1;
    zoom = 0;
    zoom_speed = 5;
    opt_algo = 0;
    interpolation = 2;
    opt_zoom = 1;

    crop_black = false;
    invert_trf = false;
    relative_trf = false;
    virtual_tripod = false;

    //Other options
    only_1st = false;
    verbose = false;
    only_2nd = false;
    keep_trf = false;

    //Destination and preset
    duration = TIME_VALUE();
    out.Clear();
    preset.Reset();
    stream_map.Clear();
    m_Commands.Clear();

    if (load)
    {

        //Load values
        wxString s;

        //Pass 1
        s = GetValue(VIDSTAB_PASS1);
        shakiness = Str2Long(GetToken(s, ',', true), shakiness);
        accuracy = Str2Long(GetToken(s, ',', true), accuracy);
        step_size = Str2Long(GetToken(s, ',', true), step_size);
        tripod_frame = Str2Long(GetToken(s, ',', true), tripod_frame);
        min_contrast = Str2Long(GetToken(s, ',', true), min_contrast);

        //Pass 2
        s = GetValue(VIDSTAB_PASS2);
        smoothing = Str2Long(GetToken(s, ',', true), smoothing);
        max_shift = Str2Long(GetToken(s, ',', true), max_shift);
        max_angle = Str2Long(GetToken(s, ',', true), max_angle);
        zoom = Str2Long(GetToken(s, ',', true), zoom);
        zoom_speed = Str2Long(GetToken(s, ',', true), zoom_speed);
        opt_algo = Str2Long(GetToken(s, ',', true), opt_algo);
        interpolation = Str2Long(GetToken(s, ',', true), interpolation);
        opt_zoom = Str2Long(GetToken(s, ',', true), opt_zoom);

        crop_black = STRBOOLDEF(GetToken(s, ',', true), crop_black);
        invert_trf = STRBOOLDEF(GetToken(s, ',', true), invert_trf);
        relative_trf = STRBOOLDEF(GetToken(s, ',', true), relative_trf);
        virtual_tripod = STRBOOLDEF(GetToken(s, ',', true), virtual_tripod);

        //Other options
        s = GetValue(VIDSTAB_OTHER);
        only_1st = STRBOOLDEF(GetToken(s, ',', true), only_1st);
        verbose = STRBOOLDEF(GetToken(s, ',', true), verbose);
        only_2nd = STRBOOLDEF(GetToken(s, ',', true), only_2nd);
        keep_trf = STRBOOLDEF(GetToken(s, ',', true), keep_trf);

        //Destination and preset
        duration = TIME_VALUE(GetValue(VIDSTAB_DURATION, duration.ToString()));
        out = GetValue(VIDSTAB_OUTPUT, out);
        preset = UNIQUE_ID(GetValue(VIDSTAB_PRESET, preset.ToString()));
        stream_map = GetValue(VIDSTAB_STREAM_MAP, stream_map);

    }

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------


FFQ_CONCAT_JOB::FFQ_CONCAT_JOB() : FFQ_QUEUE_ITEM()
{

    //Default constructor - reset only
    Reset(false);

}

//---------------------------------------------------------------------------------------

FFQ_CONCAT_JOB::FFQ_CONCAT_JOB(const FFQ_CONCAT_JOB &copy_from) : FFQ_QUEUE_ITEM(copy_from)
{

    //Constructor to clone another job
    Reset(false);

    slideshow = copy_from.slideshow;
    scale_pad = copy_from.scale_pad;
    force = copy_from.force;
    loop_frames = copy_from.loop_frames;
    simple_concat = copy_from.simple_concat;
    concat_vid = copy_from.concat_vid;
    concat_aud = copy_from.concat_aud;
    concat_subs = copy_from.concat_subs;

    frame_time = copy_from.frame_time;

    pad_color = copy_from.pad_color;
    img_first = copy_from.img_first;
    img_count = copy_from.img_count;

    out = copy_from.out;
    img_pattern = copy_from.img_pattern;
    merge_filter = copy_from.merge_filter;
    merge_smap = copy_from.merge_smap;
    merge_padding = copy_from.merge_padding;

    preset = copy_from.preset;

    limit_len = copy_from.limit_len;

}

//---------------------------------------------------------------------------------------

FFQ_CONCAT_JOB::FFQ_CONCAT_JOB(wxString from) : FFQ_QUEUE_ITEM(from)
{

    //Constructor that parses values from a string
    Reset(true);

}

//---------------------------------------------------------------------------------------

void FFQ_CONCAT_JOB::Cleanup()
{

    //Cleanup any temporary files and folders

    if (m_TempPath.Len() > 0)
    {

        wxArrayString *tpl = new wxArrayString();
        tpl->Add(m_TempPath);
        DeletePaths(tpl);
        delete tpl;

    }

    //Copy the file time
    CpFileTime(FFQPresetMgr::Get()->GetPreset(preset.ToString()), out);

}

//---------------------------------------------------------------------------------------

bool FFQ_CONCAT_JOB::GetLogFileName(wxString &name)
{

    //Set correct log file name
    name = save_log ? MakeLogFileName(out) : "";
    return name.Len() > 0;

}

//---------------------------------------------------------------------------------------

wxString FFQ_CONCAT_JOB::ToString()
{

    //Store values

    wxString s;

    s.Printf("%s,%s,%s,%s,%s,%s,%s,%s,%g,%u,%u,%u",

             BOOLSTR(slideshow), BOOLSTR(scale_pad), BOOLSTR(force), BOOLSTR(loop_frames),
             BOOLSTR(simple_concat), BOOLSTR(concat_vid), BOOLSTR(concat_aud), BOOLSTR(concat_subs),

             frame_time,

             pad_color, img_first, img_count

            );


    SetValue(CONCAT_SETTINGS, s);
    SetValue(VIDSTAB_OUTPUT, out);
    SetValue(CONCAT_IMG_PATTERN, img_pattern);
    SetValue(CONCAT_MERGE_FILTER, merge_filter);
    SetValue(CONCAT_MERGE_PADDING, merge_padding);
    SetValue(VIDSTAB_STREAM_MAP, merge_smap);
    SetValue(VIDSTAB_PRESET, preset.ToString());
    SetValue(VIDSTAB_DURATION, limit_len.ToString());

    return FFQ_QUEUE_ITEM::ToString();

}

//---------------------------------------------------------------------------------------

wxString FFQ_CONCAT_JOB::GetCommandAtIndex(int index)
{

    /*if (index > 0)
    {
        //Always one command only
        status = qsDONE;
        return wxEmptyString;
    }*/

    if (index == 0)
    {

        //Validate preset
        if (slideshow || (!simple_concat))
        {

            LPFFQ_PRESET pst = FFQPresetMgr::Get()->GetPreset(preset.ToString());
            if ((pst == NULL) || (pst->video_codec == CODEC_COPY) || pst->two_pass) return wxEmptyString;

            //Set ptr to preset
            m_PresetPtr = pst;

        }

        //Slide show?
        if (slideshow) m_CmdList = MakeSlideshowCmd();

        //Simple concat?
        else if (simple_concat) m_CmdList = MakeConcatCmd();

        //Merge!
        else m_CmdList = MakeMergeCmd();

        if (m_CmdList.Len() == 0)
        {

            //Command creation failed
            status = qsFAILED;
            return wxEmptyString;

        }

    }

    //Command was created, update status
    status = (m_CmdList.Len() == 0) ? qsDONE : qsACTIVE;

    //Return next command (if any)
    return GetLine(m_CmdList, true);

}

//---------------------------------------------------------------------------------------

wxString FFQ_CONCAT_JOB::MakeConcatCmd()
{

    //Make a simple concat command

    wxString s, fn;
    uint64_t cmd_len = 0;

    for (size_t i = 0; i < inputs.Count(); i++)
    {

        //Get input file
        FFQ_INPUT_FILE inf = GetInput(i);

        //Add duration
        cmd_len += inf.duration.ToMilliseconds();

        //Get short file name
        wxFileName fname(inf.path);
        fn = fname.GetShortPath();
        fn.Replace("'", "'\\''"); //Might not be necessary

        //Add file to source list
        s += "file '" + fn + "'" + CRLF;

        //If custom defined add duration to list
        if ((inf.start != 0) && (inf.start != inf.duration)) s += "duration " + inf.start.ToString() + CRLF;

    }

    //Create the temporary list file
    m_TempPath = FFQCFG()->GetTmpPath(out.BeforeLast(wxFileName::GetPathSeparator()), false, "txt");

    wxFile *lf = new wxFile(m_TempPath, wxFile::write);
    if (!lf->IsOpened())
    {

        //Log error
        delete lf;
        FFQConsole::Get()->AppendLine(FFQSF(SID_CREATE_WRITE_FILE_ERROR, m_TempPath), COLOR_RED);
        return wxEmptyString;

    }

    //Save the list as ascii
    wxScopedCharBuffer scb = s.ToAscii();
    bool ok = lf->Write((void*)scb.data(), scb.length());
    lf->Close();
    delete lf;

    if (!ok)
    {

        //Log error
        FFQConsole::Get()->AppendLine(FFQSF(SID_CREATE_WRITE_FILE_ERROR, m_TempPath), COLOR_RED);
        return wxEmptyString;

    }

    //Create the command
    s = TIME_VALUE(cmd_len).ToString() + ",-f concat -safe 0 -i \"" + FormatFileName(m_TempPath) + "\" -c:v copy -c:a copy ";
    if (!limit_len.IsUndefined()) s += "-t " + limit_len.ToString() + " ";
    s += "-y \"" + FormatFileName(out) + "\"";

    //Return it
    return s;

}

//---------------------------------------------------------------------------------------

wxString FFQ_CONCAT_JOB::MakeMergeCmd()
{

    //Make input list and duration
    uint64_t cmd_len = 0;
    wxString input_list = "";

    for (int i = 0; i < (int)inputs.Count(); i++)
    {

        FFQ_INPUT_FILE inf = GetInput(i);

        //Add duration
        cmd_len += inf.duration.ToMilliseconds();

        //Add input file
        input_list += "-i \"" + FormatFileName(inf.path) + "\" ";

    }

    //Get name of streams
    wxString ca = merge_smap, cv;
    cv = GetToken(ca, ",", true);

    //Create a job for building the command
    FFQ_JOB job = FFQ_JOB();
    job.skip_in_files = inputs.Count();
    job.cmd_line = CMD_DEFAULT;
    job.preset_ptr = m_PresetPtr;
    job.stream_map = "";
    job.out = out;

    //Create a simple stream mapping with search keys
    if (cv.Len() > 0) job.stream_map += CONCAT_FLTR_KEY1 + cv;
    job.stream_map += ",";
    if (ca.Len() > 0) job.stream_map += CONCAT_FLTR_KEY2 + ca;

    //Build the command
    long dummy = 0;
    wxString res = BuildCommandLine(&job, dummy, false, true);

    //Get filter
    wxString fltr = merge_filter;

    //Insert the filter to the command
    bool fk1 = (res.Find(CONCAT_FLTR_KEY1) >= 0), //Search key for video filters
         fk2 = (res.Find(CONCAT_FLTR_KEY2) >= 0); //Search key for audio filters

    if ((!fk1) && (!fk2))
    {

        //Remove specifiers since they are not used by other filters
        if (ca.Len() > 0) fltr = fltr.BeforeLast('[');
        if (cv.Len() > 0) fltr = fltr.BeforeLast('[');

        //Add filter to inputs
        input_list += "-filter_complex \"" + fltr + "\" ";

    }
    else
    {

        //At least one of the stream specifiers are used - figure out how to deal with it
        if (fk1)
        {

            //Video specifier used; make sure that audio stream specifier is either used or directed to null
            if (fk2) res.Replace(CONCAT_FLTR_KEY2, ""); //Used
            else if (ca.Len() > 0) fltr += ";" + ca + "anull"; //Unused

            //Insert filter to chain
            res.Replace(CONCAT_FLTR_KEY1, fltr + ";");

        }
        else
        {

            //If video stream exists it must be set to null since it is unused
            if (cv.Len() > 0) fltr += ";" + cv + "null";

            //Insert filter to chain
            res.Replace(CONCAT_FLTR_KEY2, fltr + ";");

        }

    }

    //Insert inputs to command
    CommandLineReplace(res, CMD_INPUTS, input_list);
    //res.Replace(CMD_INPUTS, input_list);

    //Insert output to command
    wxString dst = "-y \"" + FormatFileName(out) + "\"";
    if (!limit_len.IsUndefined())
    {
        dst = "-t " + limit_len.ToString() + " " + dst;
        cmd_len = limit_len.ToMilliseconds();
    }

    //Disable audio to prevent issues with some files have audio and others don't
    if (!concat_aud) dst = "-an " + dst;

    //Insert destination to command
    res.Replace(CMD_OUTPUT, dst);

    //Apply the command length and return
    return TIME_VALUE(cmd_len).ToString() + "," + res;

}

//---------------------------------------------------------------------------------------

wxString FFQ_CONCAT_JOB::MakeSlideshowCmd()
{

    //Ensure that the destination is available and preset is compatible
    int img1st = img_first;
    wxString res, tmp, imgptn = img_pattern;
    LPFFQ_PRESET pst = (LPFFQ_PRESET)m_PresetPtr;

    if (scale_pad)
    {

        //Create the first command used for creating uniform dimensions
        res = CONCAT_FIT_AND_PAD;
        res.Replace("$W", ToStr(GetInput(0).width));
        res.Replace("$H", ToStr(GetInput(0).height));
        res.Replace("$C", wxColour((unsigned long)pad_color).GetAsString(wxC2S_HTML_SYNTAX));

        //Create a temporary folder for the fitted images
        m_TempPath = FFQCFG()->GetTmpPath(out.BeforeLast(wxFileName::GetPathSeparator()), true);
        if (m_TempPath.Len() == 0)
        {
            FFQConsole::Get()->AppendLine(FFQS(SID_CONCAT_TEMP_FOLDER_ERROR), COLOR_RED);
            return wxEmptyString;
        }

        tmp = m_TempPath + wxFileName::GetPathSeparator() + imgptn.AfterLast(wxFileName::GetPathSeparator());
        res = ToStr(img_count) + ",-hide_banner -start_number " + ToStr(img1st) + " -i \"" + FormatFileName(imgptn) +
              "\" -vf \"" + res + "\" -qscale:v 1 \"" + FormatFileName(tmp) + "\"" + CRLF;

        imgptn = tmp;
        img1st = 1;

    }
    else res = "";

    wxString aud = GetInput(1).path;
    TIME_VALUE aud_len = (aud.Len() > 0) ? GetInput(1).duration : TIME_VALUE(0);

    FFQ_JOB job = FFQ_JOB();
    job.cmd_line = CMD_DEFAULT;
    job.preset_id = pst->preset_id;
    job.stream_map = "[0:v]" + CONCAT_FLTR_KEY1;
    job.out = out;
    if (aud.Len() > 0)
    {
        job.stream_map += ",[1:a]";
        job.skip_in_files = 2;
    }
    else job.skip_in_files = 1;

    long dummy = 0;
    tmp = BuildCommandLine(&job, dummy);

    wxString loop = /*((aud.Len() > 0) && loop_frames)*/ loop_frames ? "-loop 1 " : "";
    wxString in = "-framerate 1/" + ToStr(frame_time);
    if (img_count > 1) in += " -start_number " + ToStr(img1st);
    in += " " + loop + "-i \"" + FormatFileName(imgptn) + "\"";
    if (aud.Len() > 0) in += " -i \"" + FormatFileName(aud) + "\"";

    tmp.Replace(CMD_INPUTS, in);

    wxString dst = "-shortest -y \"" + out + "\"";
    //if (aud.Len() > 0) dst = "-shortest " + dst;

    if (pst->frame_rate.Len() == 0) dst = "-r 25 " + dst; //Framerate of output video MUST be specified to make it work
    if (pst->pixel_format.Len() == 0) dst = "-pix_fmt yuv420p " + dst;
    if (!limit_len.IsUndefined()) dst = "-t " + limit_len.ToString() + " " + dst;

    //Make setpts filter and insert it
    wxString fltr = force ? "setpts=PTS+(" + ToStr(frame_time) + "/TB)" : "";
    if ((tmp.Find(CONCAT_FLTR_KEY1) < 0) && (fltr.Len() > 0)) dst = "-vf \"" + fltr + "\" " + dst;
    else
    {

        if (fltr.Len() > 0) fltr += ",";
        tmp.Replace(CONCAT_FLTR_KEY1, fltr);

    }

    tmp.Replace(CMD_OUTPUT, dst);

    //Figure out the length of the command and add it to command
    TIME_VALUE cmd_len = TIME_VALUE((uint64_t)(frame_time * 1000 * img_count));
    if ((!aud_len.IsUndefined()) && (cmd_len.IsUndefined() || loop_frames || (aud_len.ToMilliseconds() < cmd_len.ToMilliseconds()))) cmd_len = aud_len;
    if ((!limit_len.IsUndefined()) && (cmd_len.IsUndefined() || (limit_len.ToMilliseconds() < cmd_len.ToMilliseconds()))) cmd_len = limit_len;
    res += cmd_len.ToString() + "," + tmp;

    return res;

}

//---------------------------------------------------------------------------------------

void FFQ_CONCAT_JOB::Reset(bool load)
{

    //Reset to default
    slideshow = false;
    scale_pad = true;
    force = false;
    loop_frames = true;
    simple_concat = false;
    concat_vid = true;
    concat_aud = true;
    concat_subs = false;

    frame_time = 5;
    pad_color = 0;
    img_first = 0;
    img_count = 0;

    out.Clear();
    img_pattern.Clear();
    merge_filter.Clear();
    merge_padding.Clear();
    merge_smap.Clear();
    preset.Reset();
    limit_len = TIME_VALUE(0);

    m_TempPath.Clear();

    if (load)
    {

        //Load values
        wxString s = GetValue(CONCAT_SETTINGS);

        slideshow = STRBOOLDEF(GetToken(s, ",", true), slideshow);
        scale_pad = STRBOOLDEF(GetToken(s, ",", true), scale_pad);
        force = STRBOOLDEF(GetToken(s, ",", true), force);
        loop_frames = STRBOOLDEF(GetToken(s, ",", true), loop_frames);
        simple_concat = STRBOOLDEF(GetToken(s, ",", true), simple_concat);
        concat_vid = STRBOOLDEF(GetToken(s, ",", true), concat_vid);
        concat_aud = STRBOOLDEF(GetToken(s, ",", true), concat_aud);
        concat_subs = STRBOOLDEF(GetToken(s, ",", true), concat_subs);

        frame_time = Str2Float(GetToken(s, ",", true), frame_time);

        pad_color = Str2Long(GetToken(s, ",", true), pad_color);
        img_first = Str2Long(GetToken(s, ",", true), img_count);
        img_count = Str2Long(GetToken(s, ",", true), img_count);

        out = GetValue(VIDSTAB_OUTPUT, out);
        img_pattern = GetValue(CONCAT_IMG_PATTERN, img_pattern);
        merge_filter = GetValue(CONCAT_MERGE_FILTER, merge_filter);
        merge_padding = GetValue(CONCAT_MERGE_PADDING, merge_padding);
        merge_smap = GetValue(VIDSTAB_STREAM_MAP, merge_smap);
        preset = UNIQUE_ID(GetValue(VIDSTAB_PRESET, preset.ToString()));
        limit_len = TIME_VALUE(GetValue(VIDSTAB_DURATION, limit_len.ToString()));

    }

}
