/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQToolJobs.h                                                   *
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

#ifndef FFQTOOLJOBS_H
#define FFQTOOLJOBS_H

#include <wx/string.h>
#include "FFQQueueItem.h"
#include "FFQThumbs.h"
#include "FFQUniqueID.h"

//Structure to define a job for creating thumbs and tiles
typedef struct FFQ_THUMB_JOB : FFQ_QUEUE_ITEM
{

    THUMBS_AND_TILES thumbs;

    FFQ_THUMB_JOB();
    FFQ_THUMB_JOB(const FFQ_THUMB_JOB &copy_from);
    FFQ_THUMB_JOB(wxString from);

    virtual QUEUE_ITEM_TYPE GetItemType() { return qtTHUMB_JOB; };
    virtual wxString ToString();

protected:

    virtual wxString GetCommandAtIndex(int index);

private:

    wxString m_Commands;
    void Reset(bool load);

} FFQ_THUMB_JOB, *LPFFQ_THUMB_JOB;


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

//Structure used to define a job for two-pass video stabilization
const unsigned int VIDSTAB_OPT_ALGO_COUNT = 2;
const unsigned int VIDSTAB_OPT_ZOOM_COUNT = 3;
const unsigned int VIDSTAB_INTERPOLATION_COUNT = 4;
extern const wxString VIDSTAB_TRANSFORM_EXT;

typedef struct FFQ_VIDSTAB_JOB : FFQ_QUEUE_ITEM
{

    //First pass values
    int shakiness, accuracy, step_size, tripod_frame, min_contrast;

    //Second pass values
    int  smoothing, max_shift, max_angle, zoom, zoom_speed, opt_algo,
         interpolation, opt_zoom;
    bool crop_black, invert_trf, relative_trf, virtual_tripod;

    //Other options
    bool only_1st, verbose, only_2nd, keep_trf;

    TIME_VALUE duration; //Duration of the job
    wxString out; //Destination file
    UNIQUE_ID preset; //Preset to use
    wxString stream_map; //Stream mapping

    FFQ_VIDSTAB_JOB();
    FFQ_VIDSTAB_JOB(const FFQ_VIDSTAB_JOB &copy_from);
    FFQ_VIDSTAB_JOB(wxString from);

    virtual void Cleanup();
    virtual QUEUE_ITEM_TYPE GetItemType() { return qtVIDSTAB_JOB; };
    virtual bool GetLogFileName(wxString &name);
    virtual wxString ToString();

protected:

    virtual wxString GetCommandAtIndex(int index);

private:

    wxString m_Commands, m_TempFile;
    short m_Passes;

    bool MakeCommands();
    void Reset(bool load);

} FFQ_VIDSTAB_JOB, *LPFFQ_VIDSTAB_JOB;


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

//Structure to define a slideshow or concat job
typedef struct FFQ_CONCAT_JOB : FFQ_QUEUE_ITEM
{

    bool slideshow, //True if this is a slide show job - otherwise it's a concat job
         scale_pad, //Should slide show be scaled and padded?
         force, //True if following should be forced: PTS for slide show, SAR for concat
         loop_frames, //Should frames be looped until audio track finishes?
         simple_concat, //True if a simple concat should be performed
         concat_vid, //Concat video?
         concat_aud, //Concat audio?
         concat_subs; //Concat subtitles?

    double frame_time; //How long should each image be shown?

    unsigned int pad_color, //Padding color for slide show
                 img_first, //Start number of the first slide show image
                 img_count; //Number of images in a slide show

    wxString out, //Destination file
             img_pattern, //The image pattern to use for slide show
             merge_filter, //Pre-build merge filter for audio + video
             merge_smap, //A simple [audio],[video] stream mapping
             merge_padding; //A filter used for padding

    UNIQUE_ID preset; //Preset for encoding

    TIME_VALUE limit_len; //The duration limit for out

    FFQ_CONCAT_JOB();
    FFQ_CONCAT_JOB(const FFQ_CONCAT_JOB &copy_from);
    FFQ_CONCAT_JOB(wxString from);

    virtual void Cleanup();
    virtual QUEUE_ITEM_TYPE GetItemType() { return qtCONCAT_JOB; };
    virtual bool GetLogFileName(wxString &name);
    virtual wxString ToString();

protected:

    virtual wxString GetCommandAtIndex(int index);

private:

    wxString m_TempPath, m_CmdList;
    void *m_PresetPtr;

    wxString MakeConcatCmd();
    wxString MakeMergeCmd();
    wxString MakeSlideshowCmd();

    void Reset(bool load);

} FFQ_CONCAT_JOB, *LPFFQ_CONCAT_JOB;

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

//Structure to define a video-to-GIF job
typedef struct FFQ_VID2GIF_JOB : FFQ_QUEUE_ITEM
{

    wxString out;
    unsigned short width, height, fps;
    bool two_pass, precise_cuts;
    TIME_VALUE start_time, limit_len;

    FFQ_VID2GIF_JOB();
    FFQ_VID2GIF_JOB(const FFQ_VID2GIF_JOB &copy_from);
    FFQ_VID2GIF_JOB(wxString from);

    virtual void Cleanup();
    virtual QUEUE_ITEM_TYPE GetItemType() { return qtVID2GIF_JOB; };
    virtual bool GetLogFileName(wxString &name);
    virtual wxString ToString();

protected:

    virtual wxString GetCommandAtIndex(int index);

private:

    wxString m_PaletteFile;

    void Reset(bool load);

} FFQ_VID2GIF_JOB, *LPFFQ_VID2GIF_JOB;

#endif // FFQTOOLJOBS_H
