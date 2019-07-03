/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQConfig.cpp                                                   *
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

#include "FFQConfig.h"
#include "FFQProcess.h"
#include "FFQConst.h"
#include "../FFQPresetMgr.h"
#include "../bin_res.h"
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/ffile.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>

//---------------------------------------------------------------------------------------

//Initialize static FFQConfig
FFQConfig* FFQConfig::m_Instance = NULL;

//---------------------------------------------------------------------------------------

//extern declarations
//const wxString FILENAME_CONFIG = "ffqueue.cfg";

const wxString CFG_PRESET = "[preset]";
const wxString CFG_CODEC_INFO = "[codec_info]";

const wxString TEMP_PATH_SYST = "<sys>";
const wxString TEMP_PATH_DEST = "<dest>";

const wxString PATTERN_VAR_FILENAME = "<file_name>";
const wxString PATTERN_VAR_FILEFMT = "<file_format>";
const wxString PATTERN_VAR_PREFFMT = "<preferred_format>";
const wxString PATTERN_VAR_PRESET = "<preset_name>";

//---------------------------------------------------------------------------------------

//Private constants for saving and loading config
const wxString CFG_HEADER_10 = "FFQUEUE_CONFIG_VERSION_10";
const wxString CFG_FFMPEG_PATH = "ffmpeg_path";
const wxString CFG_TEMP_PATH = "temp_path";
const wxString CFG_THUMB_SETTINGS = "thumb_settings";
const wxString CFG_SECOND_FILE_EXTS = "second_file_exts";
const wxString CFG_AUTO_REMOVE = "auto_remove";
const wxString CFG_FONTSCONF_CHECKED = "fonts_conf_checked";
const wxString CFG_SAVE_ON_EXIT = "save_on_exit";
const wxString CFG_LIMIT_STATS = "limit_stats";
const wxString CFG_HIDE_BANNER = "hide_banner";
const wxString CFG_BATCH_CFG = "batch_config";
const wxString CFG_CUSTOM_PLAYER = "custom_player";
const wxString CFG_PREFERRED_FMT = "preferred_fmt";
const wxString CFG_PREFERRED_PATH = "preferred_path";
const wxString CFG_PREFERRED_UNIQUE = "preferred_unique";
const wxString CFG_OUTPUT_NAME_PATTERN = "output_name_pattern";
const wxString CFG_KEEP_CONSOLE = "keep_console";
const wxString CFG_SAVE_WINDOW_POS = "save_pos";
const wxString CFG_LIST_COLUMNS = "list_columns";
const wxString CFG_FULL_CODEC_LIST = "full_codec_list";
const wxString CFG_VIDSTAB_SETTINGS = "vidstab_settings";
const wxString CFG_CONSOLE_CMD = "console_cmd";
const wxString CFG_SAVE_LOG = "save_log";
const wxString CFG_SILENT_QFINISH = "silent_qfinish";
const wxString CFG_SAVED_COMMANDS = "saved_commands";
const wxString CFG_SAVE_ON_MODIFY = "save_on_modify";
const wxString CFG_VALIDATE_ON_LOAD = "validate_on_load";
const wxString CFG_CONFIRM_DELETE_JOBS = "confirm_delete_jobs";
const wxString CFG_SUBS_CHARENC = "subs_charenc";
const wxString CFG_LOCALE = "locale";

//---------------------------------------------------------------------------------------

//Private helper functions

CODEC_TYPE ParseCodec(wxString &c, bool encoders)
{

    if (c.GetChar(0) == ' ') c.Remove(0, 1); //Remove first blank

    if (!encoders)
    {

        //This is purely for backwards compatibility with old ffmpeg's
        for (unsigned int i = 0; i < 6; i++) if ( (i < c.Len()) && (c.GetChar(i) == ' ') ) c.SetChar(i, '.');

    }

    CODEC_TYPE res;
    wxString s1, s2;
    s1 = GetToken(c, ' '); //Descriptive characters: encoders=[V|A|S]FSXBD ,codecs=DE[V|A|S]SDT
    if (s1.Len() != 6) return ctINVALID;

    c = StrTrim(c);
    s2 = GetToken(c, ' '); //Codec name for command line
    c = StrTrim(c); //c is now codec description

    //Index of A,V,S character
    int idx = encoders ? 0 : 2;

    if (s1.GetChar(idx) == 'V') res = ctVIDEO;
    else if (s1.GetChar(idx) == 'A') res = ctAUDIO;
    else if (s1.GetChar(idx) == 'S') res = ctSUBTITLE;
    else return ctINVALID;

    if (encoders)
    {

        //Retrieved with -encoders option
        if (s1.GetChar(3) == 'X') c += " (EXPERIMENTAL)";
        if (c.Find(s2 + " ") == 0) c.Remove(0, s2.Len() + 1);
        c = s2 + " - " + c + "\n";

    }

    else
    {

        //Retrieved with -codecs option

        if (s1.GetChar(1) != 'E') return ctINVALID; //Not an encoder

        //Extract encoders - if available
        wxString s3 = GetToken(c, " (encoders:", true);

        //Remove "(decoders:" if present
        s3 = GetToken(s3, " (decoders:", true) + "\n";

        if (c.Len() > 0)
        {

            //c contains encoders separated by space
            c.RemoveLast(); //Remove trailing ")"
            c = StrTrim(c); //Trim blanks

            //Make list of encoders
            s1.Clear();
            while (c.Len() > 0) s1 += GetToken(c, " ", true) + " - " + s3;

            //Set result
            c = s1;

        }

        //No encoder list - use s2
        else c = s2 + " - " + s3;

    }

    return res;

}

//---------------------------------------------------------------------------------------

wxString ParseHWDecoders(wxString decoders, wxString hwaccels)
{

    //For comparison
    const wxString VAAPI = "vaapi";

    //Local constant which is a list of codecs most likely supported by vaapi
    const wxString VAAPI_CODECS = ",h264,mpeg2video,mpeg4,vc1,hevc,";

    wxString l, c, s, hwa, res = "";

    while (decoders.Len() > 0)
    {

        l = StrTrim(GetLine(decoders)); //Get a codec line

        c = GetToken(l, " ", true); //V.....
        c = GetToken(l, " ", true); //Codec name, "l" is now codec description

        //Test if the decoder matches any hardware accelerator
        s = hwaccels;
        while (s.Len() > 0)
        {

            hwa = GetLine(s); //Get accelerators
            int p = c.Find(hwa); //Test if the decoder includes the name of the accelerator

            if ((p > 0) || ((hwa == VAAPI) && (VAAPI_CODECS.Find("," + c + ",") >= 0)))
            {

                //The codec either includes the name of an accelerator OR it
                //is known to be supported by vaapi, if the latter is the case
                //we include vaapi in the name to hint the user where it belongs.
                if (p < 0) c += " (" + VAAPI + ")";

                //Append to result
                res += c + " - " + StrTrim(l) + "\n";

                //Clear to prevent further testing with this codec
                s.Clear();

            }

        }

    }

    //Return the mess!
    if (res.Len() > 0) res.RemoveLast();
    return res;

}

//---------------------------------------------------------------------------------------

const wxString FilterDescriptionChars = ".TSC";
bool IsFilterDescription(wxString &s)
{
    if (s.Len() != 3) return false;
    for (int i = 0; i < 3; i++) if (FilterDescriptionChars.Find(s.at(i)) < 0) return false;
    return true;
}

//---------------------------------------------------------------------------------------

/*wxString FFQConfig::GetAppName(wxString *path)
{

    //Get path to executable
    wxString p = wxStandardPaths::Get().GetExecutablePath();

    //Extract app name
    wxString a = p.AfterLast(wxFileName::GetPathSeparator());

    //Remove any extension
    if (a.Find('.') > 0) a = a.BeforeLast('.');

    //Extract path only if required
    if (path) *path = p.BeforeLast(wxFileName::GetPathSeparator());

    //Return the app name
    return a;

}*/

//---------------------------------------------------------------------------------------

FFQConfig* FFQConfig::GetInstance()
{

    //Returns a singleton instance of FFQConfig, instantiating as required
    if (m_Instance == NULL) FFQConfig::m_Instance = new FFQConfig();
    return FFQConfig::m_Instance;

}

//---------------------------------------------------------------------------------------

/*wxString FFQConfig::GetUserDataDir()
{
    //Return path to users data directory
    #ifdef __LINUX__
        //On Linux we honor the ~/.config/appname way of life
        wxString res = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + ".config";
        if (wxFileName::DirExists(res))
        {
            res += wxFileName::GetPathSeparator();
            return res + GetAppName().Lower();
        }
    #endif // __LINUX__
    return wxStandardPaths::Get().GetUserDataDir();
}*/

//---------------------------------------------------------------------------------------

void FFQConfig::Release()
{

    //Releases the singleton instance
    if (FFQConfig::m_Instance) delete FFQConfig::m_Instance;
    FFQConfig::m_Instance = NULL;

}

//---------------------------------------------------------------------------------------

FFQConfig::~FFQConfig()
{

    //Destructor - release allocated stuff

    delete m_TaskBar;
    m_TaskBar = NULL;

    if (m_PresetManager)
    {

        delete m_PresetManager;
        m_PresetManager = NULL;

    }

    if (m_CodecInfo)
    {

        delete m_CodecInfo;
        m_CodecInfo = NULL;

    }

    if (m_PixelFormats)
    {

        delete m_PixelFormats;
        m_PixelFormats = NULL;

    }

}

//---------------------------------------------------------------------------------------

bool FFQConfig::AreFiltersAvailable(wxString filter_list)
{

    //Checks if the filters in the filter_list are available (comma separated)

    if (filter_list == "-") return true; //Separator is skipped

    while (filter_list.Len() > 0)
        //Check if a filter from the list is supported - return false if not
        if (m_Filters.Find("," + GetToken(filter_list, ",") + ",") < 0) return false;

    //All supported
    return true;

}

//---------------------------------------------------------------------------------------

void FFQConfig::CheckFontsConf()
{

    //Checks if fonts.conf is available. If not the user is asked to create it

    if (fonts_conf_checked || use_libav) return; //Already checked or libav = no further action

    //Set checked to true and save config
    fonts_conf_checked = true;
    SaveConfig();

    #ifdef __WINDOWS__ //Only perform this test in Windows

    //Make path to folder where fonts.conf should be located
    wxString fc_path = m_FFPath.BeforeLast(wxFileName::GetPathSeparator()) + wxFileName::GetPathSeparator() + "fonts",

    //And a full filename to fonts.conf
    fc_file = fc_path + wxFileName::GetPathSeparator() + "fonts.conf";

    if (!wxFileExists(fc_file))
    {

        //fonts.conf does not exist - ask the user to create it
        if (wxMessageBox(FFQSF(SID_FONT_CONF_NOT_FOUND, fc_file), FFQS(SID_WARNING), wxYES_NO|wxICON_WARNING) == wxYES)
        {

            //Yup, must be created
            wxFFile *ff = new wxFFile();
            //wxTextFile *tf = new wxTextFile(fc_file);
            try
            {

                //Make dir - no error check
                wxFileName::Mkdir(fc_path);

                //Create file with error check
                if (!ff->Open(fc_file, "wb")) ThrowError(FFQSF(SID_CREATE_WRITE_FILE_ERROR, fc_file));
                //if (!tf->Create()) ThrowError(FFQSF(SID_CREATE_WRITE_FILE_ERROR, fc_file));

                //Add the contents
                if (!ff->Write(&FONTS_CONF, FONTS_CONF_SIZE)) ThrowError(FFQSF(SID_CREATE_WRITE_FILE_ERROR, fc_file));
                //tf->AddLine((char*)FONTS_CONF);

                //Write to file with error check
                //if (!tf->Write()) ThrowError(FFQSF(SID_CREATE_WRITE_FILE_ERROR, fc_file));

                //Show success
                else ShowInfo(NULL, FFQS(SID_FONT_CONF_CREATE_SUCCESS));


            } catch (std::exception &err)
            {

                //Show what happened
                ShowError(NULL, _(err.what()));

            }

            //Release (and close) the file
            delete ff;
            //delete tf;

        }

    }

    #endif // __WINDOWS__

}

//---------------------------------------------------------------------------------------

void FFQConfig::DefaultOptions()
{

	//Resets the config to its default state

	//Public config
    auto_remove_jobs = false;
    fonts_conf_checked = false;
    ffmpeg_ok = false;
    save_on_exit = true;
    limit_statistics = true;
    hide_banner = true;
    save_window_pos = false;
    full_codec_listings = false;
    preferred_unique = false;
    keep_console = false;
    batch_config = "1,1,0,ac3,mp4";
    preferred_format = "mp4";
    preferred_path = "";
    output_name_pattern = PATTERN_VAR_FILENAME + "." + PATTERN_VAR_FILEFMT;
    temp_path = TEMP_PATH_SYST;
    thumb_settings = "";
    cust_player = "";
    second_file_extensions = "srt,ass,ssa,xsub,sub,mp3,wav";
    window_position = "";
    list_columns = "";
    vidstab_settings = "";
    console_cmd = "";
    save_log = true;
    silent_qfinish = false;
    save_on_modify = false;
    validate_on_load = true;
    confirm_delete_jobs = true;
    saved_commands = "";
    subs_charenc = "";
    user_locale = "";

	//Private config
    if (m_CodecInfo) delete m_CodecInfo;
    m_CodecInfo = MakeDefaultCodecInfo();

	if (m_PixelFormats) delete m_PixelFormats;
	m_PixelFormats = NULL;

	if (m_PresetManager) ((FFQPresetMgr*)m_PresetManager)->ClearPresets();

    m_AudioCodecs = "";
    m_FFMpegLongVersion = "";
    m_FFMpegShortVersion = "";
    m_FFPath = "";
    m_Filters = "";
    m_Formats = "";
    m_SubtitleCodecs = "";
    m_VideoCodecs = "";
    m_HWAccels = "";
    m_HWDecoders = "";

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::FindSecondaryInputFile(wxString &for_filename)
{

    //Find only one secondary input file (for backwards compatibility)
    wxArrayString as;
    if (FindSecondaryInputFiles(for_filename, as, 1) == 1) return as[0];
    return "";

}

//---------------------------------------------------------------------------------------

unsigned int FFQConfig::FindSecondaryInputFiles(wxString &for_filename, wxArrayString &dest, unsigned int limit)
{

    //Find secondary input files based on the extensions in "second_file_extensions"
    //and return the number of file names added to "dest". The number of files to find might
    //be limited to "limit" (0 = unlimitd)

    wxString exts = second_file_extensions, ext, fn = for_filename.BeforeLast('.') + ".", p;

    unsigned int cnt = 0;

    while (exts.Len() > 0)
    {

        ext = StrTrim(GetToken(exts, ",", true));
        p = fn + ext;

        if ((p != for_filename) && wxFileExists(p))
        {

            dest.Add(p);
            cnt++;

            if ((limit > 0) && (cnt >= limit)) return cnt;

        }

    }

    return cnt;

}

//---------------------------------------------------------------------------------------

LPCODEC_INFO FFQConfig::GetCodecInfo()
{

    //Return the list of codec info's
    return m_CodecInfo;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetConfigPath(wxString append_filename)
{

    //Return the configuration path and append a file name if required
    wxString res = m_ConfigPath;
    if (append_filename.Len() > 0) res += wxFileName::GetPathSeparator() + append_filename;
    return res;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetConsoleCommand(wxString prog, wxString args)
{

    //Returns the console command used for previewing and other stuff
    wxString res = "";

    if (console_cmd.Len() == 0)
    {

        //Use the default console command if empty
        #ifdef __WINDOWS__
        res = "cmd /" + wxString(keep_console ? "k" : "c") + " \"%s\"";
        #else
        res = "xterm " + wxString(keep_console ? "-hold" : "+hold") + " -e '%s'";
        args.Replace("'", "\\'");
        #endif

    }
    else
    {

        //Custom console command declared in the format
        //console_command
        //or
        //close_console_command||keep_console_command

        res = console_cmd;
        if (res.Find("||") > 0)
        {

            wxString cc = GetToken(res, "||", true);
            if (!keep_console) res = cc;

        }

    }

    if ((prog.Len() > 0) && (args.Len() > 0))
    {

        //Insert arguments as "prog" args
        return wxString::Format(res, "\"" + prog + "\" " + args);

    }
    else if ((prog.Len() > 0) || (args.Len() > 0))
    {

        //Insert either prog or args as argument
        return wxString::Format(res, prog + args);

    }

    //Return unformatted
    return res;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetExecutableName(EXECUTABLE_TYPE exe_type, bool add_ext)
{

    //Return the wanted binary name

    wxString res = wxString(use_libav ? "av" : "ff");
    if (exe_type == etENCODER) res += wxString(use_libav ? "conv" : "mpeg");
    else if (exe_type == etPROBER) res += "probe";
    else if (exe_type == etPLAYER) res += "play";
    else ThrowError("Bad binary type");

    if (add_ext)
    {
        #ifdef __WINDOWS__
        res += ".exe";
        #endif // __WINDOWS__
    }

    return res;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetFFMpegCommand(bool ffprobe, wxString ffmpeg_path)
{

    //Get the location of ffmpeg or ffprobe using either the path
    //stored in config or the path specified as argument

    wxString res = ffmpeg_path.Len() > 0 ? ffmpeg_path : m_FFPath;

    if (ffprobe)
    {

        if (res.Find(wxFileName::GetPathSeparator()) >= 0)
        {
            res = res.BeforeLast(wxFileName::GetPathSeparator());
            res += wxFileName::GetPathSeparator();
        }
        else res.Clear();

        res += GetExecutableName(etPROBER);

    }

    return res;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetFFPlayCommand(bool show_error_if_not_found, PLAYER_TYPE *ptype)
{

    //Get the path to ffplay (or the custom player)
    wxString res = cust_player;

    if (res.Len() == 0)
    {

        //No custom player - get ffplay
        res = GetFFMpegCommand();
        if (res.Len() <= 0) return res; //Not possible
        res = res.BeforeLast(wxFileName::GetPathSeparator()) + wxFileName::GetPathSeparator() + GetExecutableName(etPLAYER);

    }

    bool found = wxFileExists(res);

    if (show_error_if_not_found && (!found))
    {

        //Show error if not found
        ShowError(NULL, FFQS(SID_FFPLAY_NOT_FOUND));
        res = "";

    }

    else if (found && ptype)
    {

        wxString pn = res.AfterLast(wxFileName::GetPathSeparator()).Lower();
        if (pn.Find('.') > 0) pn = pn.BeforeLast('.');
        if (pn == "ffplay") *ptype = ptFFPLAY;
        else if (pn == "vlc") *ptype = ptVLC;
        else *ptype = ptUNKNOWN;

    }

    return res;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetFFMpegCodecs(CODEC_TYPE ct)
{

    //Returns the codecs for the specified codec type
    if (ct == ctVIDEO) return m_VideoCodecs;
    else if (ct == ctAUDIO) return m_AudioCodecs;
    else if (ct == ctSUBTITLE) return m_SubtitleCodecs;

    //Bad codec type
    return "";

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetFFMpegFilters()
{

    //Return the list of supported filters
    return m_Filters;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetFFMpegFormats()
{

    //Return the list of supported formats
    return m_Formats;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetFFMpegVersion(bool short_version)
{

    //Returns either the long or the short ffmpeg version
    return short_version ? m_FFMpegShortVersion : m_FFMpegLongVersion;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetHWAccelerators()
{

    //Return the list of supported hardware accelerators
    return m_HWAccels;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetHWDecoders()
{
    //Return the list of supported hardware decoders
    return m_HWDecoders;
}
//---------------------------------------------------------------------------------------

LPPIXEL_FORMAT FFQConfig::GetPixelFormats()
{

    //Return the list of pixel formats
    return m_PixelFormats;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetPreferredOutputName(wxString for_input_file, LPFFQ_PRESET pst)
{

    //Returns the preferred (estimated) file name for output files
    //based on the name of the input file and the specified pattern

    wxUniChar psep = wxFileName::GetPathSeparator();
    wxString p = preferred_path, n = for_input_file.AfterLast(psep), e = n.AfterLast('.');
    n = n.BeforeLast('.');

    wxString ptn = output_name_pattern;
    ptn.Replace(PATTERN_VAR_FILEFMT, e);
    ptn.Replace(PATTERN_VAR_FILENAME, n);
    ptn.Replace(PATTERN_VAR_PREFFMT, preferred_format);
    ptn.Replace(PATTERN_VAR_PRESET, pst == NULL ? "" : pst->preset_name);

    if ((p.Len() == 0) || (!wxDirExists(p))) p = for_input_file.BeforeLast(psep);
    if (p.Right(1) != psep) p += psep;
    p += ptn;

    /*if ((p.Len() > 0) && wxDirExists(p))
    {

        //If the last used output path exists, use it again
        wxUniChar psep = wxFileName::GetPathSeparator();
        if (p.Right(1) != psep) p += psep;

        p += for_input_file.AfterLast(psep).BeforeLast('.') + "." + preferred_format;

    }

    else
    {

        //If last output path does not exists, use the input path
        p = for_input_file.BeforeLast('.') + "." + preferred_format;

    }*/

    //Ensure unique?
    if (preferred_unique) EnsureUniquePath(p);

    //return the result
    return p;

}

//---------------------------------------------------------------------------------------

wxWindow* FFQConfig::GetPresetManager()
{

    //Return instance of preset manager
    return m_PresetManager;

}

//---------------------------------------------------------------------------------------

FFQTaskBar* FFQConfig::GetTaskBar()
{

    //Return instance of task bar
    return m_TaskBar;

}

//---------------------------------------------------------------------------------------

wxString FFQConfig::GetTmpPath(wxString dest_path, bool make_sub_folder, wxString temp_file_extension)
{

    //Returns the path to a temporary folder

    //Get the path defined in options
    wxString res;
    if (temp_path == TEMP_PATH_SYST) res = wxStandardPaths::Get().GetTempDir();
    else if (temp_path == TEMP_PATH_DEST) res = dest_path;
    else res = temp_path;

    if (make_sub_folder || (temp_file_extension.Len() > 0))
    {

        //Append temp name for file or folder
        if (res.Right(1) != wxFileName::GetPathSeparator()) res += wxFileName::GetPathSeparator();
        res += app_name.Lower() +  "_tmp";
        int idx = 1;

        if (make_sub_folder)
        {

            //Make unique temp dir
            while (wxDirExists(res + ToStr(idx))) idx++;
            res += ToStr(idx);
            if (!wxFileName::Mkdir(res)) res = "";

        }
        else
        {

            //Make unique temp file
            if (temp_file_extension.at(0) != '.') temp_file_extension = "." + temp_file_extension;
            while (wxFileExists(res + ToStr(idx) + temp_file_extension)) idx++;
            res += ToStr(idx) + temp_file_extension;

        }

    }

    //Return the path
    return res;

}

//---------------------------------------------------------------------------------------

void FFQConfig::InitPresetManager(wxWindow *parent)
{

    //Initializes the preset manager using the parent specified
    if (m_PresetManager == NULL) m_PresetManager = new FFQPresetMgr(parent);

}

//---------------------------------------------------------------------------------------

wxArrayString* FFQConfig::LoadArrayString(wxString filename)
{

    //Loads an array of strings from the specified file name

    //If no path separator is found we assume that a name of
    //a file located in the config dir is wanted
    if (filename.Find(wxFileName::GetPathSeparator()) == wxNOT_FOUND) filename = GetConfigPath(filename);

    //Create text file
    wxTextFile txt(filename);

    if (txt.Exists())
    {

        try
        {

            //If the file exists we try to load it

            if (txt.Open())
            {

                //Opened successfully, read content into a array string pointer
                wxArrayString* res = new wxArrayString();
                for (unsigned int i = 0; i < txt.GetLineCount(); i++) res->Add(StrTrim(txt.GetLine(i)));

                //Close the file
                txt.Close();

                //Return the result
                return res;

            }

        } catch (...) { }

    }

    //Some error occurred - return NULL
    return NULL;

}

//---------------------------------------------------------------------------------------

void FFQConfig::LoadConfig()
{

    //Loads configurations from file

    //If preset manager is not available we fail
    if (m_PresetManager == NULL) ThrowError(FFQS(SID_PRESET_MGR_NOT_INITIALIZED));

    //Create a text file to load from
    wxTextFile cfg(m_ConfigFile);

    if (cfg.Exists())
    {

        //Config file exists, continue with loading

        wxString ff_path = "";
        bool opened = false;

        try
        {

            //Open configurations file and error out if not successful
            opened = cfg.Open();
            if (!opened) ThrowError(FFQSF(SID_OPEN_FILE_ERROR, m_ConfigFile));

            //Options file is OK, reset options to defaults prior to load
            DefaultOptions();

            //Test file header, error out if not valid
            wxString line, name;
            line = cfg.GetFirstLine();
            if (line != CFG_HEADER_10) ThrowError(FFQS(SID_INVALID_CONFIG_HEADER));

            while (!cfg.Eof())
            {

                //Read a line of config
                line = cfg.GetNextLine();

                if (line.Lower() == CFG_PRESET)
                {

                    //Preset header found, load preset into string
                    name = "";
                    bool done = false;
                    while ((!cfg.Eof()) && (!done))
                    {
                        line = StrTrim(cfg.GetNextLine());
                        done = (line.Len() == 0);
                        if (!done) name += line + CRLF;
                        //if (line.Len() == 0) break;
                        //name += line + CRLF;
                    }
                    try
                    {

                        //Parse the preset and add to manager
                        LPFFQ_PRESET pst = new FFQ_PRESET(name);
                        ((FFQPresetMgr*)m_PresetManager)->AddPreset(pst);

                    }
                    catch (std::exception &err)
                    {

                        //Invalid preset
                        ShowError(NULL, FFQSF(SID_INVALID_PRESET_IN_CONFIG, name));

                    }

                }

                else if (line.Lower() == CFG_CODEC_INFO)
                {

                    //Load codec info section
                    bool done = false;
                    while ((!cfg.Eof()) && (!done))
                    {
                        line = StrTrim(cfg.GetNextLine());
                        //if (line.Len() == 0) break;
                        done = (line.Len() == 0);
                        if (!done)
                        {
                            line.LowerCase(); //Ensure lowercase codec's
                            if (m_CodecInfo == NULL) m_CodecInfo = new CODEC_INFO(line);
                            else m_CodecInfo->Append(line);
                        }
                    }

                }

                else if (line.Find('=') > 0)
                {

                    //Load config value from a name value pair
                    name = GetToken(line, '=');
                    if (name == CFG_FFMPEG_PATH) m_FFPath = line;
                    else if (name == CFG_TEMP_PATH) temp_path = line;
                    else if (name == CFG_THUMB_SETTINGS) thumb_settings = line;
                    else if (name == CFG_SECOND_FILE_EXTS) second_file_extensions = line;
                    else if (name == CFG_BATCH_CFG) batch_config = line;
                    else if (name == CFG_CUSTOM_PLAYER) cust_player = line;
                    else if (name == CFG_PREFERRED_FMT) preferred_format = line;
                    else if (name == CFG_PREFERRED_PATH) preferred_path = line;
                    else if (name == CFG_PREFERRED_UNIQUE) preferred_unique = STRBOOL(line);
                    else if (name == CFG_OUTPUT_NAME_PATTERN) output_name_pattern = line;
                    else if (name == CFG_KEEP_CONSOLE) keep_console = STRBOOL(line);
                    else if (name == CFG_AUTO_REMOVE) auto_remove_jobs = STRBOOL(line);
                    else if (name == CFG_FONTSCONF_CHECKED) fonts_conf_checked = STRBOOL(line);
                    else if (name == CFG_SAVE_ON_EXIT) save_on_exit = STRBOOL(line);
                    else if (name == CFG_LIMIT_STATS) limit_statistics = STRBOOL(line);
                    else if (name == CFG_HIDE_BANNER) hide_banner = STRBOOL(line);
                    else if (name == CFG_SAVE_WINDOW_POS)
                    {

                        save_window_pos = STRBOOL(GetToken(line, "|", true));
                        window_position = line;

                    }
                    else if (name == CFG_LIST_COLUMNS) list_columns = line;
                    else if (name == CFG_VIDSTAB_SETTINGS) vidstab_settings = line;
                    else if (name == CFG_FULL_CODEC_LIST) full_codec_listings = STRBOOL(line);
                    else if (name == CFG_CONSOLE_CMD) console_cmd = line;
                    else if (name == CFG_SAVE_LOG) save_log = STRBOOL(line);
                    else if (name == CFG_SILENT_QFINISH) silent_qfinish = STRBOOL(line);
                    else if (name == CFG_SAVED_COMMANDS) saved_commands = line;
                    else if (name == CFG_SAVE_ON_MODIFY) save_on_modify = STRBOOL(line);
                    else if (name == CFG_VALIDATE_ON_LOAD) validate_on_load = STRBOOL(line);
                    else if (name == CFG_CONFIRM_DELETE_JOBS) confirm_delete_jobs = STRBOOL(line);
                    else if (name == CFG_SUBS_CHARENC) subs_charenc = line;
                    else if (name == CFG_LOCALE) user_locale = line;


                }

            }

        }
        catch (std::exception &err)
        {

            //Some naughty stuff happened
            ShowError(NULL, FFQSF(SID_LOAD_CONFIG_ERROR, _(err.what())));

        }

        //Validate the path to the FF-binaries
        if (m_FFPath.Len() > 0) ValidateFFMpegPath(m_FFPath, true);

        //Close configurations file if opened
        if (opened) cfg.Close();

    }

    //else
    if (!ffmpeg_ok)
    {

        //ShowInfo("Config not found: " + m_ConfigFile);
        //If no config file is available we try to automatically locate the FF-binaries
        TryLocateFFMpeg();

    }

}

//---------------------------------------------------------------------------------------

bool FFQConfig::SaveArrayString(wxString filename, wxArrayString* array_str)
{

    //Save an array string to a file (reverse of LoadArrayString)
    if (filename.Find(wxFileName::GetPathSeparator()) == wxNOT_FOUND) filename = GetConfigPath(filename);

    bool ok = false, exists = wxFileExists(filename);

    //Remove existing file if exists and array is NULL
    //and set result to success of file removal
    if ((array_str == NULL) && exists) ok = wxRemoveFile(filename);

    else if (array_str != NULL)
    {

        //Save the array
        try
        {
            //File object
            wxTextFile txt(filename);

            //Create or open - set result to success of either
            ok = exists ? txt.Open() : txt.Create();

            if (ok)
            {

                //Successfully created or opened - clear contents
                txt.Clear();

                //Add the strings in the array
                for (unsigned int i = 0; i < array_str->Count(); i++) txt.AddLine(array_str->Item(i));

                //Write to file and set result to success
                ok = txt.Write();

                //Close file
                txt.Close();

            }

        } catch (...) {}

    }

    //Set result to non existence of file
    else ok = (!exists);

    //Return result
    return ok;

}

//---------------------------------------------------------------------------------------

void FFQConfig::SaveConfig()
{

    //Save the config to file

    //Create a file object
    wxTextFile cfg(m_ConfigFile);
    bool opened = false;

    try
    {

        //Make sure the directory for config file exists, error
        //out if it does not and cannot be created
        bool exists = wxFileName::Exists(m_ConfigPath, wxFILE_EXISTS_DIR);
        if (!exists) exists = wxFileName::Mkdir(m_ConfigPath);
        if (!exists) ThrowError(FFQSF(SID_CREATE_DIRECTORY_ERROR, m_ConfigPath));

        //Open / Create the file
        opened = cfg.Exists() ? cfg.Open() : cfg.Create();

        //If failed to open file, error out
        if (!opened) ThrowError(FFQSF(SID_CREATE_WRITE_FILE_ERROR, m_ConfigFile));

        //Clear file if not empty
        if (!cfg.Eof()) cfg.Clear();

        //Write config to file
        cfg.AddLine(CFG_HEADER_10);
        cfg.AddLine(CFG_FFMPEG_PATH + "=" + m_FFPath);
        cfg.AddLine(CFG_TEMP_PATH + "=" + temp_path);
        cfg.AddLine(CFG_THUMB_SETTINGS + "=" + thumb_settings);
        cfg.AddLine(CFG_SECOND_FILE_EXTS + "=" + second_file_extensions);
        cfg.AddLine(CFG_BATCH_CFG + "=" + batch_config);
        cfg.AddLine(CFG_CUSTOM_PLAYER + "=" + cust_player);
        cfg.AddLine(CFG_PREFERRED_FMT + "=" + preferred_format);
        cfg.AddLine(CFG_PREFERRED_PATH + "=" + preferred_path);
        cfg.AddLine(CFG_PREFERRED_UNIQUE + "=" + BOOLSTR(preferred_unique));
        cfg.AddLine(CFG_OUTPUT_NAME_PATTERN + "=" + output_name_pattern);
        cfg.AddLine(CFG_KEEP_CONSOLE + "=" + BOOLSTR(keep_console));
        cfg.AddLine(CFG_AUTO_REMOVE + "=" + BOOLSTR(auto_remove_jobs));
        cfg.AddLine(CFG_FONTSCONF_CHECKED + "=" + BOOLSTR(fonts_conf_checked));
        cfg.AddLine(CFG_SAVE_ON_EXIT + "=" + BOOLSTR(save_on_exit));
        cfg.AddLine(CFG_LIMIT_STATS + "=" + BOOLSTR(limit_statistics));
        cfg.AddLine(CFG_HIDE_BANNER + "=" + BOOLSTR(hide_banner));
        if (save_window_pos) cfg.AddLine(CFG_SAVE_WINDOW_POS + "=" + STR_YES + "|" + window_position);
        else cfg.AddLine(CFG_SAVE_WINDOW_POS + "=" + STR_NO);
        cfg.AddLine(CFG_LIST_COLUMNS + "=" + list_columns);
        cfg.AddLine(CFG_VIDSTAB_SETTINGS + "=" + vidstab_settings);
        cfg.AddLine(CFG_FULL_CODEC_LIST + "=" + BOOLSTR(full_codec_listings));
        cfg.AddLine(CFG_CONSOLE_CMD + "=" + console_cmd);
        cfg.AddLine(CFG_SAVE_LOG + "=" + BOOLSTR(save_log));
        cfg.AddLine(CFG_SILENT_QFINISH + "=" + BOOLSTR(silent_qfinish));
        cfg.AddLine(CFG_SAVED_COMMANDS + "=" + saved_commands);
        cfg.AddLine(CFG_SAVE_ON_MODIFY + "=" + BOOLSTR(save_on_modify));
        cfg.AddLine(CFG_VALIDATE_ON_LOAD + "=" + BOOLSTR(validate_on_load));
        cfg.AddLine(CFG_CONFIRM_DELETE_JOBS + "=" + BOOLSTR(confirm_delete_jobs));
        cfg.AddLine(CFG_SUBS_CHARENC + "=" + subs_charenc);
        cfg.AddLine(CFG_LOCALE + "=" + user_locale);

        //Empty line to separate codec_info's
        cfg.AddLine("");

        //Write non-default codec_info's to file
        LPCODEC_INFO cinf = m_CodecInfo;
        wxString s, t;
        while (cinf)
        {

            if (!cinf->IsDefault(&t)) s += t + "\n";
            cinf = cinf->next;

        }

        if (s.Len() > 0)
        {

            cfg.AddLine(CFG_CODEC_INFO);
            cfg.AddLine(s); //Since "s" ends with "\n" there will be a blank line to end section

        }

        //Write presets to file
        FFQPresetMgr* pm = (FFQPresetMgr*)m_PresetManager;
        for (unsigned int i = 0; i < pm->GetPresetCount(); i++)
        {

            LPFFQ_PRESET p = pm->GetPreset(i);

            if (!p->is_temporary)
            {

                wxString pst = CFG_PRESET + CRLF + p->ToString() + CRLF;
                cfg.AddLine(pst);

            }

        }

        //Flush contents and error out is fails
        if (!cfg.Write()) ThrowError(FFQSF(SID_CREATE_WRITE_FILE_ERROR, m_ConfigFile));

    }
    catch (std::exception &err)
    {

        //Display error to user
        ShowError(NULL, FFQSF(SID_SAVE_CONFIG_ERROR, _(err.what())));

    }

    //Close configurations file if opened
    if (opened) cfg.Close();

}

//---------------------------------------------------------------------------------------

bool FFQConfig::SetSaveLog(bool log, bool save_config)
{

    //Convenient set save_log and save as required
    //used in FFQJobEditAdv, tools/*
    if (save_log != log)
    {
        save_log = log;
        if (save_config) SaveConfig();
        return true;
    }

    return false;

}

//---------------------------------------------------------------------------------------

void FFQConfig::SetPreferredAndSave(wxString fmt, wxString path, bool save_always)
{

    //Sets preferred format and path and saves if necessary
    bool save = save_always;

    if ((fmt.Len() > 0) && (fmt != preferred_format))
    {
        preferred_format = fmt;
        save = true;
    }

    if ((path.Len() > 0) && (path != preferred_path))
    {
        preferred_path = path;
        save = true;
    }

    if (save) SaveConfig();

}

//---------------------------------------------------------------------------------------

bool FFQConfig::ValidateFFMpegPath(wxString path, bool set_config_path_if_valid)
{

    //Used to initialize and verify that FFMpeg and FFProbe is available
    wxString ffmpeg = GetFFMpegCommand(false, path),
             ffprobe = GetFFMpegCommand(true, path);

    //Resturn false if ffmpeg executables does not exists
    //if ((!wxFileName::Exists(ffmpeg)) || (!wxFileName::Exists(ffprobe))) return false;
    if ((!wxFileExists(ffmpeg)) || (!wxFileExists(ffprobe))) return false;

    //Get version from FFMpeg
    FFQProcess* proc = new FFQProcess();
    wxString long_ver = StrTrim(proc->GetFFMpegVersion(path));

    //Validate the received version information
    wxString s = long_ver, t = GetLine(s), st = GetExecutableName(etENCODER, false) + " version";


    if (t.Lower().Left(st.Len()) != st)
    {

        #ifdef DEBUG
        ShowInfo("Bad encoder version output (\"" + st + "\" expected):\n\n" + t);
        #endif // DEBUG

        //Not valid
        delete proc;
        return false;

    }

    st = t.Right(t.Len() - st.Len() - 1);
    t = StrTrim(GetLine(s));
    //if (t.Left(8) != "built on")
    if ((t.Lower().Left(5) != "built") || (t.Lower().Find("gcc") == wxNOT_FOUND))
    {

        #ifdef DEBUG
        ShowInfo("Bad encoder built info:\n\n" + t);
        #endif // DEBUG

        //Not valid
        delete proc;
        return false;

    }

    //If the path to ffmpeg is not to be set as option, exit now with success
    if (!set_config_path_if_valid) return true;

    //Version information is valid, make short version
    wxString short_ver = st + " " + t, a_codecs = "", v_codecs = "", s_codecs = "", filters = "";

    //Extract supported codecs for encoding
    s = proc->GetFFMpegEncoders(path);

    //Skip all lines before "------"
    while ( (s.Len() > 0) && (StrTrim(GetLine(s)).Find("-----") != 0) ) ;

    bool encoders = (s.Len() > 0);

    if (!encoders)
    {

        //If -encoders fails, use -codecs
        s = proc->GetFFMpegCodecs(path);
        while ( (s.Len() > 0) && (StrTrim(GetLine(s)).Find("-----") != 0) ) ;


    }

    CODEC_TYPE ct;

    while (s.Len() > 0)
    {

        //t = StrTrim(GetLine(s));
        t = GetLine(s);

        if (t.Len() == 0) s.Clear(); //Stop at any empty line

        else
        {
            ct = ParseCodec(t, encoders);
            if (ct == ctVIDEO) v_codecs += t;
            else if (ct == ctAUDIO) a_codecs += t;
            else if (ct == ctSUBTITLE) s_codecs += t;
        }

    }

    //Hardware accelerators (for decoding)
    wxString hwacl = "";
    s = proc->GetFFMpegOther("-hwaccels", path);
    while (s.Len() > 0)
    {
        t = StrTrim(GetLine(s));
        if (t.Find(':') < 0) hwacl += t + "\n";
    }
    if (hwacl.Len() > 0) hwacl.RemoveLast();

    //Hardware decoders
    wxString hwdec = (hwacl.Len() > 0) ? ParseHWDecoders(proc->GetFFMpegOther("-decoders", path), hwacl) : "";

    //Create a list of supported filters
    s = proc->GetFFMpegFilters(path);

    while (s.Len() > 0)
    {

        t = StrTrim(GetLine(s));

        if (t.Find("->") > 0)
        {
            st = GetToken(t, " ");
            if (IsFilterDescription(st)) st = GetToken(t, " ");
            filters += "," + st;
        }

        else if (t.Find(" ") > 0)
        {

            //Old style "filtername filterdescription"
            st = StrTrim(GetToken(t, " "));

            //If "t" now starts with "=", it is a descriptive line that must be skipped
            if ( (st.Len() > 0) && (!t.StartsWith('=')) ) filters += "," + st;

        }

    }

    //Create a list of supported formats
    s = proc->GetFFMpegFormats(path);

    //Skip lines before "--"
    while ((s.Len() > 0) && (!StrTrim(GetLine(s, true)).StartsWith('-'))) ;

    //Get all supported muxers
    while (s.Len() > 0)
    {

        //Get a line
        t = StrTrim(GetLine(s, true));

        //Get muxer / demuxer field
        st = StrTrim(GetToken(t, " ", true));

        //Test if muxer
        if (st.Find('E') >= 0)
        {

            //Yup! Add to formats
            st = StrTrim(GetToken(t, " "));
            if (st.Find(',') > 0) st = st.BeforeFirst(','); //Remove alternate format names (eg. stream_segment,ssegment)
            m_Formats += st + " - " + StrTrim(t) + "\n";

        }

    }

    //Remove last ","
    if (m_Formats.Len() > 0) m_Formats.RemoveLast();

    //Create list of pixel formats
    LPPIXEL_FORMAT pixfmts = ParsePixelFormats(proc->GetFFMpegPixelFormats(path));

    //Release the process
    delete proc;

    //Ensure that filters are encapsulated in commas (for easier filter validation with AreFiltersAvailable)
    if (filters.Len() > 0 ) filters += ",";

    //Validation was successfull - store extracted values
    ffmpeg_ok = true;
    fonts_conf_checked = fonts_conf_checked && (m_FFPath == path);
    m_FFPath = path;
    m_FFMpegShortVersion = short_ver;
    m_FFMpegLongVersion = long_ver;
    m_AudioCodecs = a_codecs;
    m_VideoCodecs = v_codecs;
    m_Filters = filters;
    m_SubtitleCodecs = s_codecs;
    m_HWAccels = hwacl;
    m_HWDecoders = hwdec;
    if (m_PixelFormats) delete m_PixelFormats;
    m_PixelFormats = pixfmts;

    //Return success
    return true;

}

//---------------------------------------------------------------------------------------

FFQConfig::FFQConfig()
{

    //Default constructor

    //Initialize random numbers used by UNIQUE_ID
    srand(time(NULL));

    //Get path to executable
    wxString p = wxStandardPaths::Get().GetExecutablePath();

    //Extract app name
    app_name = p.AfterLast(wxFileName::GetPathSeparator());

    //Remove any extension
    if (app_name.Find('.') > 0) app_name = app_name.BeforeLast('.');

    //Extract path only if required
    p = p.BeforeLast(wxFileName::GetPathSeparator());

    //Check if we are using AVlib (avconv, avprobe, avplay)
    use_libav = app_name.Lower().Find("avqueue") >= 0;

    //Check if config exists in the same location as the program file
    wxString fn_cfg = app_name.Lower() + ".cfg";
    m_ConfigPath = p;
    m_ConfigFile = GetConfigPath(fn_cfg);
    bool cfg_ok = wxFileExists(m_ConfigFile);

    if (!cfg_ok)
    {

        //Check if config exists in the typical config location
        #ifdef __LINUX__
            //On Linux we honor the ~/.config/appname way of life if available
            wxString cfg_dir = wxStandardPaths::Get().GetUserConfigDir() + "/.config";
            if (wxFileName::DirExists(cfg_dir)) cfg_dir += "/" + app_name.Lower();
            else cfg_dir = wxStandardPaths::Get().GetUserDataDir();
        #else
            //For backwards compatibility we must check if ConfigDir exists and
            //is not, we use UserDataDir in order to support per-user config
            wxString cfg_dir = wxStandardPaths::Get().GetConfigDir();
            if (!wxFileName::DirExists(cfg_dir)) cfg_dir = wxStandardPaths::Get().GetUserDataDir();
        #endif // __LINUX__

        m_ConfigPath = cfg_dir;
        m_ConfigFile = GetConfigPath(fn_cfg);
        cfg_ok = wxFileExists(m_ConfigFile);

        if (!cfg_ok)
        {

            //Try to create config in same folder as program file
            m_ConfigPath = p;
            m_ConfigFile = GetConfigPath(fn_cfg);

            wxLogNull preventPeskyPopup;

            wxTextFile *tf = NULL;
            try
            {
                tf = new wxTextFile(m_ConfigFile);
                if (tf->Create())
                {
                    tf->AddLine(CFG_HEADER_10);
                    cfg_ok = tf->Write();
                    tf->Close();
                }

            } catch (...) { }

            if (tf) delete tf;

            if (!cfg_ok)
            {

                //Everything failed; revert to default config dir
                m_ConfigPath = cfg_dir;
                m_ConfigFile = GetConfigPath(fn_cfg);//FILENAME_CONFIG);

            }

        }

    }

    //Locate the directory for shared data
    #ifdef __LINUX__
        share_data_dir = "/usr/local/share/" + app_name.Lower();
        if (!wxFileName::DirExists(share_data_dir))
            share_data_dir = wxStandardPaths::Get().GetUserConfigDir() + "/.local/share/" + app_name.Lower();
    #else
        share_data_dir = wxStandardPaths::Get().GetConfigDir();
    #endif // __LINUX__

    //Either clear or terminate with separator
    if (!wxFileName::DirExists(share_data_dir)) share_data_dir.Clear();
    else share_data_dir += wxFileName::GetPathSeparator();

    //Set pointers to NULL
    m_PresetManager = NULL;
    m_CodecInfo = NULL;
    m_PixelFormats = NULL;

    //Create task bar object
    m_TaskBar = new FFQTaskBar();

    DefaultOptions();

}

//---------------------------------------------------------------------------------------

bool FFQConfig::TryLocateFFMpeg()
{

    //Create binary name
    wxString binary = GetExecutableName(etENCODER), s, t;

    //Same directory as FFQueue (all systems)
    s = wxStandardPaths::Get().GetExecutablePath();
    s = s.BeforeLast(wxFileName::GetPathSeparator()) + wxFileName::GetPathSeparator() + binary;
    if (ValidateFFMpegPath(s, true)) return true;

    //Without any path - searches the environment (all systems)
    if (ValidateFFMpegPath(binary, true)) return true;

    #ifdef __WINDOWS__

    //Programs x86 (windows)
    t = wxString("\\" + wxString(use_libav ? "libav\\usr" : "ffmpeg") + "\\bin\\" + binary);
    s = GetProgramFolder(true) + t;
    if (ValidateFFMpegPath(s, true)) return true;
    //Programs x64 if different from x86  (windows)
    t = GetProgramFolder(false) + t;
    if ((t != s) && ValidateFFMpegPath(t, true)) return true;

    #else

    //Try obvious Linux paths
    if (ValidateFFMpegPath("/usr/bin/" + binary, true)) return true;
    if (ValidateFFMpegPath("/bin/" + binary, true)) return true;
    if (ValidateFFMpegPath("./" + binary, true)) return true;
    if (ValidateFFMpegPath("/usr/local/bin/" + binary, true)) return true;

    #endif // __WINDOWS__

    //Location failed
    return false;

}
