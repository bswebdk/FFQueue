/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQThumbs.h                                                     *
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

#ifndef FFQTHUMBS_H
#define FFQTHUMBS_H

#include <wx/string.h>
#include "FFQTimeValue.h"

//Values used with THUMBS_AND_TILES::img_dest
#define THUMB_IMG_DEST_NEW_UNIQUE_SUBFOLDER 0
#define THUMB_IMG_DEST_SAME_AS_SOURCE 1
#define THUMB_IMG_DEST_THUMBS_SUBFOLDER 2

//Values used with THUMBS_AND_TILES::freq_type
#define THUMB_FREQ_TYPE_FRAMES 0
#define THUMB_FREQ_TYPE_SECOND 1
#define THUMB_FREQ_TYPE_MINUTE 2

//The THUMBS_AND_TILES structure
typedef struct THUMBS_AND_TILES
{

    bool make_thumbs, //True if thumbs should be made
         make_tiles, //True if tiles / mosaics should be made
         every_scene; //True if each image is made on a scene change (else a fixed frequency is used)
    wxString img_format; //The image format to use for images
    unsigned int img_dest, //The destination type for images (declared with THUMB_IMG_DEST_* above)
                 freq_type, //The type of fixed frequency (declared with THUMB_FREQ_TYPE_* above)
                 thumb_width, //Width of thumbs and individual tiles in mosaics
                 thumb_count, //Number of thumbs to create
                 tile_rows, //Number of thumb rows in a mosaic
                 tile_cols, //Number of thumb columns in a mosaic
                 tile_count; //Number of mosaics (which is tile_coun * tile_rows * tile_cols individual thumbs)
    double thumb_freq; //The frequency of thumbs (in freq_type units)
    TIME_VALUE start_time; //The amount of time that should be skipped from the start of source (to skip intro's)

    THUMBS_AND_TILES();
    THUMBS_AND_TILES(wxString from);

    void Reset();
    wxString ToString();

} THUMBS_AND_TILES, *LPTHUMBS_AND_TILES;


//Function to build command lines for making thumbs and tiles
wxString BuildThumbsAndTilesCommand(THUMBS_AND_TILES &tnt, wxString &for_file, TIME_VALUE &for_file_duration, bool make_dirs);

#endif // FFQTHUMBS_H
