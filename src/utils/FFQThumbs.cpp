/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQThumbs.cpp                                                   *
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

#include "FFQThumbs.h"
#include "FFQConfig.h"
#include "FFQConst.h"
#include "FFQMisc.h"
#include <wx/filename.h>

THUMBS_AND_TILES::THUMBS_AND_TILES()
{

    //Default constructor - reset values
    Reset();

}

//---------------------------------------------------------------------------------------

THUMBS_AND_TILES::THUMBS_AND_TILES(wxString from)
{

    //Constructor that parses values from a string

    //Reset first
    Reset();

    //If the string given is empty we exit here
    if (from.Len() == 0) return;

    //Parse values
    make_thumbs = GetToken(from, ',') == STR_YES;
    make_tiles = GetToken(from, ',') == STR_YES;
    every_scene = GetToken(from, ',') == STR_YES;

    img_format = GetToken(from, ',');

    img_dest = Str2Long(GetToken(from, ','));
    freq_type = Str2Long(GetToken(from, ','));
    thumb_width = Str2Long(GetToken(from, ','));
    thumb_count = Str2Long(GetToken(from, ','));
    tile_rows = Str2Long(GetToken(from, ','));
    tile_cols = Str2Long(GetToken(from, ','));
    tile_count = Str2Long(GetToken(from, ','));

    thumb_freq = Str2Float(GetToken(from, ','));

    start_time = TIME_VALUE(GetToken(from, ','));

}

//---------------------------------------------------------------------------------------

void THUMBS_AND_TILES::Reset()
{

    //Reset values to default
    make_thumbs = false;
    make_tiles = false;
    every_scene = true;
    img_format = "png";
    img_dest = 0;
    freq_type = 1;
    thumb_width = 100;
    thumb_count = 20;
    tile_rows = 5;
    tile_cols = 4;
    tile_count = 5;
    thumb_freq = 10;
    start_time = TIME_VALUE(0);

}

//---------------------------------------------------------------------------------------

wxString THUMBS_AND_TILES::ToString()
{

    //Pack values to a string
    wxString res;
    res.Printf(
        "%s,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u,%g,%s",
        BOOLSTR(make_thumbs), BOOLSTR(make_tiles), BOOLSTR(every_scene),
        img_format, img_dest, freq_type,
        thumb_width, thumb_count, tile_rows, tile_cols, tile_count,
        thumb_freq, start_time.ToString()
    );

    //Return the packed values
    return res;

}

//---------------------------------------------------------------------------------------

wxString BuildThumbsAndTilesCommand(THUMBS_AND_TILES &tnt, wxString &for_file, TIME_VALUE &for_file_duration, bool make_dirs)
{

    //Make command for thumbs and tiles

    //Return empty if disabled
    if ((!tnt.make_thumbs) && (!tnt.make_tiles)) return "";

    //Hide banner or not?
    wxString cmd = FFQCFG()->hide_banner ? "-hide_banner " : "";

    //Start time of first thumb
    if (!tnt.start_time.IsUndefined()) cmd += "-ss " + tnt.start_time.ToString() + " ";

    //Source file (audio none, start video filters)
    cmd += "-i \"" + for_file + "\" -an -vf \"";

    //Create thumbs for each scene change (50% image change)
    if (tnt.every_scene) cmd += "select='gt(scene\\,0.5)'";

    //Fixed frequency - per frame
    else if (tnt.freq_type == THUMB_FREQ_TYPE_FRAMES) cmd += "select='not(mod(n\\," + ToStr((size_t)tnt.thumb_freq) + "))'";

    else
    {

        //Fixed frequency - per second
        double fps = tnt.thumb_freq;

        //Make minutes to seconds as required
        if (tnt.freq_type == THUMB_FREQ_TYPE_MINUTE) fps *= 60;

        //Turn into fractions
        fps = 1.0 / fps;

        //Add to command
        cmd += "fps=fps=" + ToStr(fps);

    }

    //Add scaling features, vsync and forced overwrite to command ($T, $F explained later)
    cmd += ",scale=" + ToStr(tnt.thumb_width) + ":-1$T\" -vsync vfr -y$F ";

    //Path and name for output file(s)
    wxString path = for_file.BeforeLast(wxFileName::GetPathSeparator()) + wxFileName::GetPathSeparator(), fn;

    //File name
    fn = for_file.AfterLast(wxFileName::GetPathSeparator());

    //Remove extension
    if (fn.Find('.') > 0) fn = fn.BeforeLast('.');

    if (tnt.img_dest == THUMB_IMG_DEST_NEW_UNIQUE_SUBFOLDER)
    {
        //Destination is a new, unique sub folder

        //Counter
        int i = 1;

        //Find non-exiting folder name
        while (wxDirExists(path + fn + "_TB" + ToStr(i))) i++;

        //Concat the values to path
        path += fn + "_TB" + ToStr(i);

        //Make directory as needed
        if (make_dirs) wxFileName::Mkdir(path);

        //Add separator to path
        path += wxFileName::GetPathSeparator();

        //File name pattern
        fn = "$_%03d";

    }
    else if (tnt.img_dest == THUMB_IMG_DEST_SAME_AS_SOURCE)
    {

        //Destination is same folder as input file

        //Pattern for file name
        fn += "_$_%03d";

    }
    else if (tnt.img_dest == THUMB_IMG_DEST_THUMBS_SUBFOLDER)
    {

        //Destination is the sub folder "Thumbs" (created if needed)

        //Add folder name
        path += "Thumbs";

        //Create as needed
        if ((!wxDirExists(path)) && make_dirs) wxFileName::Mkdir(path);

        //Add separator
        path += wxFileName::GetPathSeparator();

        //Pattern for file name
        fn += "_$_%03d";

    }

    //Add extension to file name pattern
    fn += "." + tnt.img_format;

    //Handy variables
    wxString res = "", tmp;

    if (tnt.make_thumbs)
    {

        //Thumb command must be created

        //Set tmp to cmd (so that cmd can be reused later)
        tmp = cmd;

        //Remove the $T from earlier; not used here
        tmp.Replace("$T", "");

        //Thumb count of 0 means unlimited - remove the $F
        if (tnt.thumb_count == 0) tmp.Replace("$F", "");

        //Thumb count is limited, insert as number of frames where $F is in the command line
        else tmp.Replace("$F", " -frames:v " + ToStr(tnt.thumb_count));

        //Now we need to tell FFQMain how long the command takes
        //In number of frames to make:
        if (tnt.thumb_count > 0) res += ToStr(tnt.thumb_count) + ",";

        //In amount of time to process
        else res += for_file_duration.ToString() + ",";

        //Add command to result including path beginning
        res += tmp + "\"" + path;

        //Insert "Thumb" to file name pattern
        tmp = fn;
        tmp.Replace("$", "Thumb");

        //And add the file name to the result and close the path
        res += tmp + "\"\n";

    }

    if (tnt.make_tiles)
    {

        //Tile / Mosaic command must be created

        //Copy cmd
        tmp = cmd;

        //Insert the tile filter with rows & cols where $T is
        tmp.Replace("$T", ",tile=" + ToStr(tnt.tile_cols) + "x" + ToStr(tnt.tile_rows));

        //Set mosaic count (As with thumbs above)
        if (tnt.tile_count == 0) tmp.Replace("$F", "");
        else tmp.Replace("$F", " -frames:v " + ToStr(tnt.tile_count));

        //Command length (same as above with thumbs)
        if (tnt.tile_count > 0) res += ToStr(tnt.tile_count) + ",";
        else res += for_file_duration.ToString() + ",";

        //Add the path
        res += tmp + "\"" + path;

        //And the file name - now with "Mosaic" instead of "Thumb"
        tmp = fn;
        tmp.Replace("$", "Mosaic");

        //Add to result
        res += tmp + "\"\n";

    }

    //DONE! return the command list
    return res;

}
