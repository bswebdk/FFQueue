/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConfig.h                                                     *
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

#ifndef FFQCONFIG_H
#define FFQCONFIG_H

#include <wx/string.h>
#include <wx/window.h>
#include <wx/hyperlink.h>
#include "FFQPixFmt.h"
#include "FFQPreset.h"
#include "FFQCodecInfo.h"
#include "FFQConst.h"
#include "../utils/FFQTaskBar.h"

//File name for configurations
//extern const wxString FILENAME_CONFIG;

//Public configuration strings
extern const wxString CFG_PRESET;
extern const wxString CFG_CODEC_INFO;

//Temporary path variables
extern const wxString TEMP_PATH_SYST;
extern const wxString TEMP_PATH_DEST;

//Used to define the variables used in output name pattern
extern const wxString PATTERN_VAR_FILENAME;
extern const wxString PATTERN_VAR_FILEFMT;
extern const wxString PATTERN_VAR_PREFFMT;
extern const wxString PATTERN_VAR_PRESET;

//Enumeration to define codec types (used in FFQConfig::GetFFMpegCodecs)
typedef enum CODEC_TYPE { ctVIDEO, ctAUDIO, ctSUBTITLE, ctINVALID = 0xFF } CODEC_TYPE;

//Executable identifiers (for encoder, prober and player)
typedef enum EXECUTABLE_TYPE { etENCODER, etPROBER, etPLAYER } EXECUTABLE_TYPE;

//Used to identify the type of player used for previews
typedef enum PLAYER_TYPE {ptUNKNOWN, ptFFPLAY, ptVLC} PLAYER_TYPE;

//The FFQConfig class used for configuration
class FFQConfig
{
    public:

        //static wxString GetAppName(wxString *path = NULL);
        static FFQConfig* GetInstance();
        //static wxString GetUserDataDir();
        static void Release();

        ~FFQConfig();

	    //Public variables
	    bool auto_remove_jobs;
	    bool fonts_conf_checked;
	    bool ffmpeg_ok;
	    bool save_on_exit;
	    bool limit_statistics;
	    bool hide_banner;
	    bool save_window_pos;
	    bool full_codec_listings;
	    bool use_libav;
	    bool preferred_unique;
	    bool keep_console;
	    bool save_log;
	    bool silent_qfinish;
	    bool save_on_modify;
	    bool validate_on_load;
	    bool confirm_delete_jobs;
	    bool preview_map_subs;
	    bool dont_save_ffmpeg;

	    bool is_snap;

	    wxString app_name;
	    wxString batch_config;
	    wxString console_cmd;
	    wxString cust_player;
	    wxString preferred_format;
	    wxString preferred_path;
	    wxString output_name_pattern;
	    wxString temp_path;
	    wxString thumb_settings;
	    wxString second_file_extensions;
	    wxString subs_charenc;
	    wxString window_position;
	    wxString list_columns;
	    wxString vidstab_settings;
	    wxString saved_commands;
	    wxString user_locale;
	    wxString share_data_dir;

	    int num_encode_slots;

	    bool dark_theme;
	    uint32_t colors[COLOR_COUNT];

		//Methods to access options not stored in public vars
		bool            AreFiltersAvailable(wxString filter_list);
		void            CheckFontsConf();
        void            DefaultOptions();
        wxString        FindSecondaryInputFile(wxString &for_filename);
        unsigned int    FindSecondaryInputFiles(wxString &for_filename, wxArrayString &dest, unsigned int limit = 0);
        LPCODEC_INFO    GetCodecInfo();
        uint32_t        GetColor(uint32_t color, bool find_index = false);
		wxString        GetConfigPath(wxString append_filename = "");
		wxString        GetConsoleCommand(wxString prog, wxString args);
        wxString        GetExecutableName(EXECUTABLE_TYPE exe_type, bool add_ext = true);
		wxString        GetFFMpegCommand(bool ffprobe = false, wxString ffmpeg_path = "");
		wxString        GetFFPlayCommand(bool show_error_if_not_found, PLAYER_TYPE *ptype = NULL);
		wxString        GetFFMpegCodecs(CODEC_TYPE ct);
		wxString        GetFFMpegFilters();
		wxString        GetFFMpegFormats();
		wxString        GetFFMpegVersion(bool shortVersion);
		wxString        GetHelpPath();
		wxString        GetHWAccelerators();
		wxString        GetHWDecoders();
		LPPIXEL_FORMAT  GetPixelFormats();
		wxString        GetPreferredOutputName(wxString for_input_file, LPFFQ_PRESET pst = NULL, bool use_preferred_path = true, bool use_preferred_format = true);
		wxWindow*       GetPresetManager();
		FFQTaskBar*     GetTaskBar();
		wxString        GetTmpPath(wxString dest_path, bool make_sub_folder = false, wxString temp_file_extension = "");
		void            InitPresetManager(wxWindow *parent);
	    wxArrayString*  LoadArrayString(wxString filename);
	    void            LoadConfig();
	    bool            SaveArrayString(wxString filename, wxArrayString* array_string);
	    void            SaveConfig();
        void            SetBrowseRootFor(wxFileDialog *dlg);
	    void            SetCtrlColors(wxGenericHyperlinkCtrl *ctrl);
	    bool            SetSaveLog(bool log, bool save_config = true);
	    void            SetPreferredAndSave(wxString format, wxString path, bool save_always = false);
	    bool            ValidateFFMpegPath(wxString path, bool set_config_path_if_valid);

    private:

        //Private variables for options

        static FFQConfig* m_Instance; //Static singleton reference

	    LPCODEC_INFO m_CodecInfo; //List with codec information
	    LPPIXEL_FORMAT m_PixelFormats; //List of supported pixel formats
	    wxWindow* m_PresetManager; //Instance of preset manager
	    FFQTaskBar* m_TaskBar; //Pointer to task bar object (windows only)

	    wxString m_AudioCodecs; //List of supported audio codecs
	    wxString m_ConfigFile; //Full path and name of configuration file
	    wxString m_ConfigPath; //Path where configuration is stored
	    wxString m_FFMpegLongVersion; //Long version of FFMpeg
	    wxString m_FFMpegShortVersion; //Short version of FFMpeg
	    wxString m_FFPath; //Path to the FF-binaries
	    wxString m_Filters; //Supported filters
	    wxString m_Formats; //Supported formats for muxing
	    wxString m_SubtitleCodecs; //List of supported subtitle codecs
	    wxString m_VideoCodecs; //List of supported video codecs
	    wxString m_HWAccels; //List of supported hardware accelerators
	    wxString m_HWDecoders; //List of hardware decoders
	    wxString m_CapsFile; //File name used for ffmpeg capabilities
	    wxString m_SnapRoot; //The root folder where the snap is mounted or empty if not snap

        FFQConfig(); //Private constructor to prevent instantiation.
        bool LoadFFmpegCapabilities();
        void SaveFFmpegCapabilities();
        bool TryLocateFFMpeg();

};

//Macro for easy access to config
#define FFQCFG() FFQConfig::GetInstance()

bool test_file_create(wxString file_name, wxString write_text = "");

#endif // FFQCONFIG_H
