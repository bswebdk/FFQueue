/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQLang.cpp                                                     *
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

#include "FFQLang.h"
#include "FFQMisc.h"
#include "FFQFilter.h"
#include "FFQConst.h"
#include "FFQConfig.h"
#include "FFQJob.h"
#include "FFQHash.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>

//---------------------------------------------------------------------------------------

//Initialize statics
FFQLang* FFQLang::m_Instance = NULL;
const unsigned short FFQLang::TIME_VALUE_COUNT = 6;
const unsigned short FFQLang::AUDIO_CHANNEL_COUNT = 25;

//---------------------------------------------------------------------------------------

//Version 1.0 header of the language file
const char LANG_FILE_HEADER_10[20] = {
    'F','F','Q','U','E','U','E','_','L','A','N','G','_','1','0','\x00','\xFF','\x66','\xAB','\x3E'
};

//---------------------------------------------------------------------------------------

//SHA1 hash used as "no password" which is basicly a hash of:
//char buf[] = { '\x01', '\xFF', '\x08', '\x0A', '\x0F' };

const STR_HASH NO_PASSWORD_HASH = {
    0xff, 0xc0, 0x80, 0xba, 0x2c, 0x2d, 0xc1, 0xc0, 0x77, 0x9e,
    0x95, 0x01, 0xd8, 0xd2, 0x5d, 0x17, 0x29, 0x00, 0x9b, 0x14
};

//---------------------------------------------------------------------------------------

//Buffer size used for loading and saving
const unsigned int BUFFER_SIZE = 20480;

//---------------------------------------------------------------------------------------

const wxString DEFAULT_LANGUAGE_EXT = ".lng";

//---------------------------------------------------------------------------------------

void HashString(const wxString &str, STR_HASH &hash)
{

    //SHA1 hash a string
    SHA1_Context sha1;
    wxScopedCharBuffer cb = str.utf8_str();
    SHA1_Buffer(&sha1, (void*)cb.data(), cb.length());
    memcpy(&hash, &sha1.hash, sizeof(hash));

}

//---------------------------------------------------------------------------------------

void EncBuf(uint8_t *buf, uint32_t len, STR_HASH &key)
{

    //Crude encryption / decryption of a buffer that uses
    //the bytes in the key to XOR the data. First time done
    //it will encrypt the buffer and second time done it
    //will restore the buffer to unencrypted

    uint8_t key_idx = 0;

    for (uint32_t i = 0; i < len; i++)
    {
        buf[i] = buf[i] ^ key[key_idx];
        key_idx++;
        if (key_idx == sizeof(key)) key_idx = 0;
    }


}

//---------------------------------------------------------------------------------------

void SaveString(wxFile *file, wxString &str, uint8_t *buf, STR_HASH &key)
{

    //Saves an encrypted string to a file

    //Get string as UTF-8
    wxScopedCharBuffer cb = str.utf8_str();

    //Get length as 32 bit unsigned integer
    uint32_t cl = cb.length();

    //Save the length of the string
    file->Write((void*)&cl, sizeof(cl));

    if (cl > 0)
    {
        //String is not empty, copy the contents to the buffer
        memcpy((void*)buf, (void*)cb.data(), cl);

        //Encrypt the buffer
        EncBuf(buf, cl, key);

        //Store the buffer
        file->Write(buf, cl);

    }

}

//---------------------------------------------------------------------------------------

void LoadString(wxFile *file, wxString &str, uint8_t *buf, STR_HASH &key)
{

    //Loads a string from file and decrypts it

    //First load the length of the string
    uint32_t cl;
    file->Read((void*)&cl, sizeof(cl));

    if (cl > 0)
    {
        //If the string is not empty - start by null-termination the buffer
        buf[cl] = '\0';
        buf[cl + 1] = '\0';

        //Load the data
        file->Read((void*)buf, cl);

        //Decrypt it
        EncBuf(buf, cl, key);

        //Set the decrypted data to the string variable
        str = wxString::FromUTF8((char*)buf);

    }

    //If string is empty - clear the variable
    else str.clear();

}

//---------------------------------------------------------------------------------------

bool DoConfirm(wxString msg) { return DoConfirm(NULL, msg); }
bool DoConfirm(wxWindow *focus, wxString msg, unsigned int icon)
{

    //If icon is undefined, use a question mark
    if (icon == 0) icon = wxICON_QUESTION;

    //Show an OK/Cancel dialog with question mark icon
    bool res = (wxMessageBox(msg, FFQS(SID_CONFIRM), wxYES_NO | icon, focus) == wxYES);

    //Focus window as needed
    if (focus) focus->SetFocus();

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

bool ShowError(wxString msg) { return ShowError(NULL, msg); }
bool ShowError(wxWindow *focus, wxString msg)
{

    //Show a dialog with error icon
    wxMessageBox(msg, FFQS(SID_ERROR), wxOK|wxICON_ERROR, focus);

    //Focus window as needed
    if (focus) focus->SetFocus();

    //Always return false
    return false;

}

//---------------------------------------------------------------------------------------

bool ShowInfo(wxString msg) { return ShowInfo(NULL, msg); }
bool ShowInfo(wxWindow *focus, wxString msg)
{

    //Show a dialog with information icon
    wxMessageBox(msg, FFQS(SID_INFORMATION), wxOK|wxICON_INFORMATION, focus);

    //Focus window as needed
    if (focus) focus->SetFocus();

    //Always return false
    return false;

}

//---------------------------------------------------------------------------------------

bool ShowWarning(wxString msg) { return ShowWarning(NULL, msg); }
bool ShowWarning(wxWindow *focus, wxString msg)
{

    //Show a dialog with warning icon
    wxMessageBox(msg, FFQS(SID_WARNING), wxOK|wxICON_WARNING, focus);

    //Focus window as needed
    if (focus) focus->SetFocus();

    //Always return false
    return false;

}

//---------------------------------------------------------------------------------------

FFQLang* FFQLang::GetInstance()
{

    //Create the singleton if NULL
    if (m_Instance == NULL) m_Instance = new FFQLang();

    //Return singleton instance
    return m_Instance;

}

//---------------------------------------------------------------------------------------

FFQLang::FFQLang(bool loadFile)
{

    //Default constructor

    //Language file name
    //m_LoadName = wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator()) + wxFileName::GetPathSeparator() + "FFQueue.lng";

    //Create default strings
    m_Strings = new wxVector<LPFFQ_STRING>();

    //Basic strings
    SetString(SID_ERROR,                        "Error");
    SetString(SID_WARNING,                      "Warning");
    SetString(SID_INFORMATION,                  "Information");
    SetString(SID_CONFIRM,                      "Confirm");
    SetString(SID_BAD_STRING_ID,                "Invalid string ID");
    SetString(SID_DATE_TIME_FORMAT,             "%Y/%d/%m %H:%M:%S");
    SetString(SID_FFPROBE_BAD_OUTPUT,           "FFProbe output malformed, maybe unsupported file format");
    SetString(SID_FFMPEG_INACTIVE,              "FFMpeg not active");
    SetString(SID_UNKNOWN_TIME_REMAINS,         "Unknown time remains");
    SetString(SID_TIME_REMAINS,                 "%s remains");
    SetString(SID_TIME_ELAPSED_TOTAL,           "%s elapsed, %s estimated total");
    SetString(SID_STATUS_MESSAGE,               "%u jobs (%u queued, %u active, %u done, %u failed, %u aborted, %u skipped)");
    SetString(SID_COMMAND,                      "Command: %s");
    SetString(SID_PRESET,                       "Preset: %s");
    SetString(SID_INVALID_PRESET,               "Invalid preset!");
    SetString(SID_SAVE_JOBS_ERROR,              "Unable to save queued jobs to file!");
    SetString(SID_JOBS_REMOVED,                 "Jobs where removed from queue, please review the console for further info");
    SetString(SID_OVERWRITE_OUTPUT_FILE,        "Output file already exists, overwrite it?");
    SetString(SID_OVERWRITE_OUTPUT_FILES,       "%u output file(s) already exists. Do you wish to overwrite the files?<br>Yes = overwrite, No = skip existing, Cancel = abort queue.");
    SetString(SID_DELETE_SELECTED,              "Delete selected job(s)?");
    SetString(SID_JOB_HAS_NO_PRESET,            "No preset is available for the job");
    SetString(SID_TASK_ABORTED,                 "Task has been aborted");
    SetString(SID_COMMAND_COMPLETED,            "Command execution has finished");
    SetString(SID_QUEUE_COMPLETED,              "Queue has been finished");
    SetString(SID_PLEASE_STOP_TASK,             "Please stop the current task before closing");
    SetString(SID_NO_FILES_OR_NO_PRESET,        "No files supplied or no presets available");
    SetString(SID_NO_CONTENT_SELECTED,          "No content (video, audio or subtitles) selected");
    SetString(SID_OUTPUT_FORMAT_REQUIRED,       "Output format must be specified");
    SetString(SID_OUTPUT_PATH_EXIST_OR_EMPTY,   "Destination folder must be an existing folder or left empty");
    SetString(SID_START_POSITION,               "Process stream(s) in %s from %s");
    SetString(SID_LIMIT_LENGTH,                 "Limit length of output to %s");
    SetString(SID_THE_BEGINNING,                "the beginning");
    SetString(SID_NO_LIMIT,                     "no limit");
    SetString(SID_INPUT_FILE_MUST_EXIST,        "Input file %s must exist");
    SetString(SID_INPUT_FILE_NOT_FOUND,         "Input file does not exist");
    SetString(SID_SELECT_AT_LEAST_ONE_STREAM,   "You must select at least one stream to include in the output file");
    SetString(SID_OUTPUT_FILE_REQUIRED,         "Output file must be specified");
    SetString(SID_OUTPUT_FILE_NOT_EQUAL_INPUT,  "Output file cannot be equal to any of the input files");
    SetString(SID_COMMAND_LINE_INVALID,         "Command line is invalid because following parts are missing: %s");
    SetString(SID_JOB_FOR_FILE_ALREADY_QUEUED,  "Another job for input %s is already queued. Continue anyway?");
    SetString(SID_STREAM_INFORMATION,           "Stream information");
    SetString(SID_FFPROBE_FAILED_ON_FILES,      "FFProbe failed on the following files:");
    SetString(SID_PRESET_REQUIRED,              "A preset is required for the encoding process");
    SetString(SID_COPY_VIDEO_NOT_ALLOWED,       "Presets using \"copy\" as video codec are not allowed");
    SetString(SID_TWO_PASS_NOT_ALLOWED,         "Presets with two pass encoding are not allowed");
    SetString(SID_FF_BINARIES_NOT_FOUND,        "FF-binaries not found in the specified location");
    SetString(SID_CUSTOM_TEMP_PATH_NOT_FOUND,   "The custom temporary path does not exist");
    SetString(SID_OPEN_CLIPBOARD_ERROR,         "Unable to open clipboard");
    SetString(SID_COPY_PRESET_SUCCESS,          "Successfully copied %u preset(s)");
    SetString(SID_COPY_PRESET_ERROR,            "Unable to copy presets to clipboard");
    SetString(SID_PASTE_PRESET_SUCCESS,         "Successfully pasted %u preset(s)");
    SetString(SID_PASTE_PRESET_NONE,            "No presets where pasted");
    SetString(SID_CONFIRM_DELETE_PRESETS,       "Do you want to delete the selected preset(s)?");
    SetString(SID_THUMBS_OR_MOSAICS_REQUIRED,   "Either thumbs or mosaics must be selected");
    SetString(SID_IMAGE_FORMAT_REQUIRED,        "Image format must be specified");
    SetString(SID_MUST_BE_A_POSITIVE_NUMBER,    "%s must be a positive number");
    SetString(SID_THUMB_WIDTH,                  "Thumb width");
    SetString(SID_THUMB_INTERVAL,               "Thumb interval");
    SetString(SID_ROW_AND_COLUMN_TOO_SMALL,     "Row and column count must multiply to at least 2");
    SetString(SID_CREATE_FIRST_THUMB_FROM,      "Create first thumb from %s");
    SetString(SID_TIME_VALUE_NAMES,             "%u second|%u seconds|%u minute|%u minutes|%u hour|%u hours");
    SetString(SID_AND,                          "and");
    SetString(SID_INVALID_STREAM_MAP_ENDODING,  "Invalid stream map encoding");
    SetString(SID_STREAM_INDEX_OUT_OF_BOUNDS,   "Stream index (%u) out of bounds");
    SetString(SID_PROBE_INFO_IS_INVALID,        "Probe info is not valid");
    SetString(SID_KILL_FFMPEG_FAILED,           "FFMpegProcess::Abort failed to kill process");
    SetString(SID_ACTIVE_CANNOT_SET_COMMAND,    "Cannot set command while process is running");
    SetString(SID_PROCESS_ALREADY_RUNNING,      "Process is already running");
    SetString(SID_EXECUTE_COMMAND_ERROR,        "Unable to execute command: %s");
    SetString(SID_FONT_CONF_NOT_FOUND,          "No font configuration found (%s). The configuration might be needed in order to use textual filters like subtitle burn-in. Do you wish to attempt to create a default configuration file?");
    SetString(SID_CREATE_WRITE_FILE_ERROR,      "Cannot create or write file \"%s\"");
    SetString(SID_FONT_CONF_CREATE_SUCCESS,     "Successfully created font.conf");
    SetString(SID_FFPLAY_NOT_FOUND,             "FFPlay was not found (needs to be in same location as FFMpeg)");
    SetString(SID_PRESET_MGR_NOT_INITIALIZED,   "Preset manager not initialized");
    SetString(SID_LOAD_CONFIG_ERROR,            "Unable to load configuration from file, error: %s");
    SetString(SID_SAVE_CONFIG_ERROR,            "Unable to save configuration to file, error: %s");
    SetString(SID_INVALID_CONFIG_HEADER,        "Invalid configuration file header");
    SetString(SID_INVALID_PRESET_IN_CONFIG,     "Invalid preset found in configurations file:<br><br>%s");
    SetString(SID_OPEN_FILE_ERROR,              "Unable to open file \"%s\"");
    SetString(SID_CREATE_DIRECTORY_ERROR,       "Unable to create directory \"%s\"");
    SetString(SID_INVALID_STRING_ARRAY_SIZE,    "String array with id %u does not have required size of %u items");
    SetString(SID_LANGUAGE_OUTDATED,            "The following inconsistencies has been found in the language file: %u untranslated, %u deprecated and %u modified!");
    SetString(SID_DISABLED,                     "Disabled");
    SetString(SID_ITEM_CANNOT_BE_EDITED,        "This item cannot be edited");
    SetString(SID_ITEM_CANNOT_BE_CLONED,        "This item cannot be cloned");
    SetString(SID_TOOL_JOB,                     "Tool: %s");
    SetString(SID_DUPLICATE_JOB_FOR_OUTPUT,     "A job with destination \"%s\" already queued");
    SetString(SID_SOURCE_EQUALS_OUTPUT,         "The input path equals the output path (%s)");
    SetString(SID_PATH_NOT_FOUND,               "Path not found: \"%s\"");
    SetString(SID_SAVE_JOBS_SUCCESS,            "Jobs has been saved");
    SetString(SID_INVALID_INPUT_NOT_ALLOWED,    "The input file is not valid; file not found or no streams in it. Fix this to do what you tried.");
    SetString(SID_NOTHING_QUEUED,               "Nothing was queued; If you want to force jobs to be queued you must right click and start selected.");
    SetString(SID_INVALID_CONSOLE_COMMAND,      "The console command seems invalid, following would be valid: executable -arg1 '%s'||executable -arg1 -arg2 '%s'");
    SetString(SID_CANNOT_SAVE_ACTIVE_QUEUE_ITEM,"The item cannot be saved because it is active!");
    SetString(SID_INLINE_SHELL_CODE_WARNING,    "The command you are about to execute seems to contain inline shell code which may be harmful for your system. Are you sure that you want to take the risk?");
    SetString(SID_ADVANCED_CUTS_WARNING,        "You have combined advanced cuts with remapping (copy codec) and this will cause the job to fail. Continue anyway?");
    SetString(SID_CONFIRM_SORT_PRESETS,         "This will re-arrange all presets alphabetically, do you want to continue?");


    //Log related messages
    SetString(SID_LOG_FILE_ERROR,               "Unable to create log file");
    SetString(SID_LOG_CLIPPING_WARNING,         "FFMpeg reported that audio clipping was performed %I64d times (distortion awareness)!");
    SetString(SID_LOG_FFMPEG_RUNTIME,           "FFMpeg exited %s, running time %s");
    SetString(SID_LOG_JOB_ABORTED,              "The job was aborted!");
    SetString(SID_LOG_COMMAND_ABORTED,          "The command was aborted!");
    SetString(SID_LOG_FFPROBE_ERROR,            "Unable to examine (FFProbe) file \"%s\". Error \"%s\"");
    SetString(SID_LOG_EXEC_COMMAND,             "Executing command: %s");
    SetString(SID_LOG_JOB_REMOVED,              "Job for \"%s\" removed because the following file(s) does not exists:<br>%s");
    SetString(SID_LOG_STARTING_FFMPEG,          "Starting FFMpeg%s %s with command: %s");
    SetString(SID_LOG_FOR_FIRST_PASS,           "for first pass");
    SetString(SID_LOG_FOR_SECOND_PASS,          "for second pass");
    SetString(SID_LOG_WRITE_DEST_ERROR,         "The destination file could not be overwritten!");
    SetString(SID_LOG_EXECUTE_ERROR,            "Error \"%s\" while trying to execute command!");
    SetString(SID_LOG_SECOND_PASS_FAIL,         "Second pass failed - please check output above for detailed information!");
    SetString(SID_LOG_COMPLETED_OK,             "Successfully completed with %I64d lines of statistics!");
    SetString(SID_LOG_COMPLETED_IRREG,          "Job completed irregularly - please check output above for detailed information!");
    SetString(SID_LOG_FFMPEG_FOUND,             "FFMpeg found: %s");
    SetString(SID_LOG_FFMPEG_NOT_FOUND,         "FFMpeg and/or FFProbe not found; Please update path in options!");
    SetString(SID_LOG_FFPROBE_INFO_FOR,         "Displaying FFProbe output for \"%s\":");
    SetString(SID_LOG_EXEC_PREVIEW_CMD,         "Executing preview command: %s");
    SetString(SID_LOG_PREVIEW_CMD_FAIL,         "Preview command failed!");
    SetString(SID_LOG_ALREADY_QUEUED,           "Already queued: %s");
    SetString(SID_LOG_MAKE_JOB_FOR,             "Make job for: %s");
    SetString(SID_LOG_CONTENT_NOT_FOUND,        "%s : not found");
    SetString(SID_LOG_LOW_CONTRAST_WARNING,     "Video stabilization reported no translations due to too low contrast in frames %u => %u");
    SetString(SID_LOG_TOTAL_JOB_TIME,           "Total processing time was %s");
    SetString(SID_LOG_PRESET_WAS_CHANGED,       "WARNING! The preset used by this job was modified during processing!");


    //Common UI strings
    SetString(SID_COMMON_MOVE_UP,               "Move up");
    SetString(SID_COMMON_MOVE_DOWN,             "Move down");
    SetString(SID_COMMON_SELECT_INPUT_FILE,     "Select input file");
    SetString(SID_COMMON_SELECT_OUTPUT_FILE,    "Select output file");
    SetString(SID_COMMON_SELECT_FILE,           "Select file");
    SetString(SID_COMMON_SELECT_FILES,          "Select files");
    SetString(SID_COMMON_VIDEO,                 "Video");
    SetString(SID_COMMON_AUDIO,                 "Audio");
    SetString(SID_COMMON_SUBTITLES,             "Subtitles");
    SetString(SID_COMMON_CANCEL,                "Cancel");
    SetString(SID_COMMON_CLOSE,                 "Close");
    SetString(SID_COMMON_OK,                    "OK");
    SetString(SID_COMMON_REFRESH,               "Refresh");
    SetString(SID_COMMON_SAVE_LOG,              "Save log file to *_log.htm");
    SetString(SID_COMMON_SAVE_JOB,              "Save job");
    SetString(SID_COMMON_ADD,                   "Add...");
    SetString(SID_COMMON_NEW,                   "New...");
    SetString(SID_COMMON_REMOVE,                "Remove");
    SetString(SID_COMMON_DELETE,                "Delete");
    SetString(SID_COMMON_EDIT,                  "Edit...");
    SetString(SID_COMMON_PREVIEW,               "Preview");
    SetString(SID_COMMON_SORT,                  "Sort");
    SetString(SID_COMMON_COPY,                  "Copy");
    SetString(SID_COMMON_PASTE,                 "Paste");
    SetString(SID_COMMON_DEST_FOLDER,           "Destination folder:");
    SetString(SID_COMMON_CLEAR,                 "Clear");
    SetString(SID_COMMON_UNDEFINED,             "Undefined");
    SetString(SID_COMMON_INPUT,                 "Input");
    SetString(SID_COMMON_OUTPUT,                "Output");
    SetString(SID_COMMON_DEFAULTS,              "Defaults");
    SetString(SID_COMMON_UPDATE,                "Update");
    SetString(SID_COMMON_DRY_RUN,               "Dry run...");
    SetString(SID_COMMON_ATTACHMENT,            "Attachment");
    SetString(SID_COMMON_DATA,                  "Data");


    //Main frame UI strings
    SetString(SID_MAINFRAME_QUEUE_STATUS_NAMES,  "Queued|Active|1st pass|2nd pass|Thumbs|Done|Failed|Aborted!|Skipped!|Dormant");



    SetString(SID_MAINFRAME_TB_BATCH,         "Batch...");
    SetString(SID_MAINFRAME_TB_START,         "Start");
    SetString(SID_MAINFRAME_TB_STOP,          "Stop");
    SetString(SID_MAINFRAME_TB_TOOLS,         "Tools");
    SetString(SID_MAINFRAME_TB_OPTIONS,       "Options...");
    SetString(SID_MAINFRAME_TB_ABOUT,         "About...");
    SetString(SID_MAINFRAME_TB_PRESETS,       "Presets...");

    SetString(SID_MAINFRAME_LVM_EDIT_JOB,     "Edit job...");
    SetString(SID_MAINFRAME_LVM_EDIT_PRESET,  "Edit preset...");
    SetString(SID_MAINFRAME_LVM_REMOVE_JOB,   "Remove job");
    SetString(SID_MAINFRAME_LVM_RUN_FFPROBE,  "Run FFProbe");
    SetString(SID_MAINFRAME_LVM_START_ALL,    "Start all");
    SetString(SID_MAINFRAME_LVM_START_SEL,    "Start selected");
    SetString(SID_MAINFRAME_LVM_CLONE_JOB,    "Clone job...");
    SetString(SID_MAINFRAME_LVM_STOP_ALL,     "Stop all");
    SetString(SID_MAINFRAME_LVM_STOP_SEL,     "Stop selected");
    SetString(SID_MAINFRAME_LVM_SHOW_COMMAND, "Show command(s)");

    SetString(SID_MAINFRAME_LVC_FILE,         "File");
    SetString(SID_MAINFRAME_LVC_COMMAND,      "Command");
    SetString(SID_MAINFRAME_LVC_STATUS,       "Status");

    SetString(SID_MAINFRAME_TM_THUMBTOOL,     "Thumbnails and mosaics...");
    SetString(SID_MAINFRAME_TM_SLIDESHOWTOOL, "Make video from images...");
    SetString(SID_MAINFRAME_TM_CONCATTOOL,    "Merge multiple sources into one...");

    //Batch make UI strings
    SetString(SID_BATCHMAKE_TITLE,             "Batch make");
    SetString(SID_BATCHMAKE_DEFINE_PROPERTIES, "Please define properties for the jobs");
    SetString(SID_BATCHMAKE_SEL_CONTENT,       "Select the content to include:");
    SetString(SID_BATCHMAKE_PREF_AUDIO_CODEC,  "Preferred audio codec(s) / language(s):");
    SetString(SID_BATCHMAKE_DEST_FORMAT,       "Destination format for output files:");
    SetString(SID_BATCHMAKE_SELECT_PRESET,     "Select a preset for the jobs:");
    SetString(SID_BATCHMAKE_MAKE_JOBS,         "Make jobs...");
    SetString(SID_BATCHMAKE_MAKE_JOBS_FOR,     "Make jobs for %u files");
    SetString(SID_BATCHMAKE_MATCH_INVERSE,     "Inverse matching: Exclude matching streams");
    SetString(SID_BATCHMAKE_PREF_SUBS_CODEC,   "Preferred subtitle codec(s) / language(s):");
    SetString(SID_BATCHMAKE_NOFAIL_CONTENT,    "Do not fail on missing:");
    SetString(SID_BATCHMAKE_ALSO_INCLUDE,      "If available, also include:");

    //Job editor UI strings
    SetString(SID_JOBEDIT_TITLE,                "Edit job");
    SetString(SID_JOBEDIT_INPUT_FILES,          "Input files");
    SetString(SID_JOBEDIT_INPUT_NO_1,           "#1");
    SetString(SID_JOBEDIT_INPUT_NO_2,           "#2");
    SetString(SID_JOBEDIT_SELECT_STREAMS,       "Select stream(s) to include in output file");
    SetString(SID_JOBEDIT_DBLCLK_FOR_INFO,      "INFO: Double click on a stream to get detailed information...");
    SetString(SID_JOBEDIT_OUTPUT_FILE,          "Output file");
    SetString(SID_JOBEDIT_OUTPUT_ENCODING,      "Encoding of output file");
    SetString(SID_JOBEDIT_COMMAND_LINE,         "FFMpeg command line");
    SetString(SID_JOBEDIT_PRESET,               "Preset command lines");
    SetString(SID_JOBEDIT_NO_PRESET,            "No preset selected (use command line)");

    //Job editor advanced UI (used in combination with items above)
    SetString(SID_JOBEDIT_ADV_ITSOFFSET,        "Offset / synchronize streams by");
    SetString(SID_JOBEDIT_ADV_MILLISECONDS,     "milliseconds");
    SetString(SID_JOBEDIT_ADV_FRAMERATE,        "Force frame rate to");
    SetString(SID_JOBEDIT_ADV_FPS,              "frames per second");
    SetString(SID_JOBEDIT_ADV_DISCARDCORRUPT,   "Discard corrupt frames");
    SetString(SID_JOBEDIT_ADV_GENPTS,           "Generate presentation time stamps");
    SetString(SID_JOBEDIT_ADV_IGNOREDTS,        "Ignore decoder time stamps");
    SetString(SID_JOBEDIT_ADV_IGNOREIDX,        "Ignore index");
    SetString(SID_JOBEDIT_ADV_ADD_INPUT,        "Add input file");
    SetString(SID_JOBEDIT_ADV_DEL_INPUT,        "Remove input file");
    SetString(SID_JOBEDIT_ADV_ADD_SECONDARY,    "Find secondary files");
    SetString(SID_JOBEDIT_ADV_MORE,             "[MORE]");
    SetString(SID_JOBEDIT_ADV_INPUT_SETTINGS,   "Per file input settings");
    SetString(SID_JOBEDIT_ADV_MORE_TOOLTIP,     "offset=%s\nframe rate=%s\nflags=%s");
    SetString(SID_JOBEDIT_ADV_FRAMERATE_SWITCH, "Force frame rate with -r rather than -framerate");

    SetString(SID_JOBEDIT_ADV_CUTS,             "[Adv. cuts]");
    SetString(SID_JOBEDIT_ADV_CUTS_TITLE,       "Advanced cutting");
    SetString(SID_JOBEDIT_ADV_CUTS_LIST_TITLE,  "Time spans to process");
    SetString(SID_JOBEDIT_ADV_CUTS_TOOLS_TITLE, "Time span");
    SetString(SID_JOBEDIT_ADV_CUTS_FROM,        "From:");
    SetString(SID_JOBEDIT_ADV_CUTS_TO,          "To:");
    SetString(SID_JOBEDIT_ADV_CUTS_BAD_TIME,    "The specified time is not valid (hh:mm:ss.ms) or From is greater or equal to To");
    SetString(SID_JOBEDIT_ADV_CUTS_OVERLAP_TIME,"The specified cut overlaps with another cut");
    SetString(SID_JOBEDIT_ADV_CUTS_DURATION,    "Duration of input file: %s (%s seconds @ %s FPS)");
    SetString(SID_JOBEDIT_ADV_CUTS_FRAME_CONV,  "Convert frame index to time value (calculated using FPS)");
    SetString(SID_JOBEDIT_ADV_CUTS_BAD_FRAME,   "Frame index is not a numeric value or From is greater or equal to To");
    SetString(SID_JOBEDIT_ADV_CUTS_TIME_DETAILS,"Show adv. time for period around:");
    SetString(SID_JOBEDIT_ADV_CUTS_BAD_TIME_2,  "Invalid time, hh:mm:ss requested (milliseconds unused here)");
    SetString(SID_JOBEDIT_ADV_CUTS_TRIM_MODE,   "The time spans should be:|Kept|Removed");
    SetString(SID_JOBEDIT_ADV_CUTS_SET_FROM,    "Set from");
    SetString(SID_JOBEDIT_ADV_CUTS_SET_TO,      "Set to");
    SetString(SID_JOBEDIT_ADV_CUTS_INPUT_INFO,  "Input file info");
    SetString(SID_JOBEDIT_ADV_CUTS_PREVIEW,     "Frame preview");
    SetString(SID_JOBEDIT_ADV_CUTS_PREVIEW_CFG, "Preview settings");
    SetString(SID_JOBEDIT_ADV_CUTS_CFG_ACCURACY,"Frame accuracy:|Low (fast)|High (slow)");
    SetString(SID_JOBEDIT_ADV_CUTS_CFG_DELAY,   "Load delay:|Fast|Slow");
    SetString(SID_JOBEDIT_ADV_CUTS_CFG_PLACEMENT,"Trim filter placement:|First|Last");
    SetString(SID_JOBEDIT_ADV_CUTS_QUICK_CUT,   "Perform quick cuts (may be less accurate)");

    //Concat tool UI strings
    SetString(SID_CONCAT_TITLE,                 "Make slideshow / concat");
    SetString(SID_CONCAT_VIDEO_FROM_IMAGES,     "Make video from images");
    SetString(SID_CONCAT_MERGE_FILES,           "Merge multiple files");
    SetString(SID_CONCAT_SOURCE_FRAMES,         "Source images / frames for the video");
    SetString(SID_CONCAT_FIRST_IMAGE,           "First image:");
    SetString(SID_CONCAT_SCALE_AND_PAD,         "Scale and pad images to fit video dimensions:");
    SetString(SID_CONCAT_SIZE,                  "Size:");
    SetString(SID_CONCAT_FILL_COLOR,            "Fill color:");
    SetString(SID_CONCAT_SCALE_INFORMATION,     "INFO: If images are NOT of identical dimensions they MUST be scaled and padded!");
    SetString(SID_CONCAT_FRAME_TIME,            "Show each frame for:");
    SetString(SID_CONCAT_SECONDS,               "seconds");
    SetString(SID_CONCAT_FORCE_PTS,             "Force presentation timestamp to prevent \"first frame missing\"");
    SetString(SID_CONCAT_AUDIO_TRACK,           "Audio track");
    SetString(SID_CONCAT_LOOP_FRAMES,           "Loop frames until audio track finishes");
    SetString(SID_CONCAT_DESTINATION,           "Destination");
    SetString(SID_CONCAT_FILE_NAME,             "File name:");
    SetString(SID_CONCAT_PRESET,                "Preset:");
    SetString(SID_CONCAT_SOURCE_FILES,          "Source files");
    SetString(SID_CONCAT_CONTENT_TYPES,         "Content types:");
    SetString(SID_CONCAT_FORCE_ASPECT,          "Force sample aspect ratios (SAR) to video aspect (W/H)");
    SetString(SID_CONCAT_IMAGES_FOUND,          "%s image(s) found (click to refresh)");
    SetString(SID_CONCAT_VIDEO_ITEM,            "%s (Video: %ux%u %s)");
    SetString(SID_CONCAT_AUDIO_ITEM,            "%s (Audio: %s)");
    SetString(SID_CONCAT_TWO_FILES_MINIMUM,     "At least two files must be available for concatenation");
    SetString(SID_CONCAT_CONTENT_MISSING,       "Required content (video / audio / subtitles) not found in selected file");
    SetString(SID_CONCAT_NO_IMAGES_FOUND,       "No images found");
    SetString(SID_CONCAT_MIN_WIDTH_AND_EVEN,    "Width must be >= 50 and even");
    SetString(SID_CONCAT_MIN_HEIGHT_AND_EVEN,   "Height must be >= 50 and even");
    SetString(SID_CONCAT_TEMP_FOLDER_ERROR,     "Unable to create temporary folder for fitted images");
    SetString(SID_CONCAT_FRAME_TIME_POSITIVE,   "Frame time must be a positive number");
    SetString(SID_CONCAT_AUDIO_TRACK_ERROR,     "Audio track is not recognizable");
    SetString(SID_CONCAT_SIMPLE,                "Perform simple concatenation (remux content)");
    SetString(SID_CONCAT_UNEQUAL_FILES,         "Simple concatenation requires the source files to be of identical format and video dimensions");
    SetString(SID_CONCAT_PADDING,               "Use padding rather than stretching");
    SetString(SID_CONCAT_LOOP_ERROR,            "This job will encode forever; you must disable loop frames, specify an audio track or limit the length of the output to prevent this.");
    SetString(SID_CONCAT_EXPLICIT_MAP,          "Explicitly map all streams");


    //Preset editor UI strings
    SetString(SID_PRESET_TITLE,                         "Edit preset");
    SetString(SID_PRESET_SELECT_PREVIEW_FILE,           "Select file to use for preview");
    SetString(SID_PRESET_PRESET_NAME,                   "Preset name");
    SetString(SID_PRESET_TEMPORARY_PRESET,              "This preset is temporary (will not be stored, thus unavailable after this session)");
    //SetString(SID_PRESET_ENCODING,                      "Encoding");
    SetString(SID_PRESET_FILTERS,                       "Filters");
    SetString(SID_PRESET_METADATA,                      "Metadata");
    SetString(SID_PRESET_THUMBS,                        "Thumbs");
    SetString(SID_PRESET_MISCELLANEOUS,                 "Miscellaneous");
    //SetString(SID_PRESET_VIDEO_ENCODING,                "Video encoding");
    SetString(SID_PRESET_CODEC,                         "Codec");
    SetString(SID_PRESET_BITRATE,                       "Bit rate");
    SetString(SID_PRESET_FRAMERATE,                     "Frame rate:");
    SetString(SID_PRESET_FPS,                           "Frames per second");
    SetString(SID_PRESET_TWO_PASS_ENCODING,             "Two-pass encoding (using temp path in options)");
    SetString(SID_PRESET_FIRST_PASS_NULL_TARGET,        "No output file for first pass (faster)");
    SetString(SID_PRESET_VIDEO_SYNC,                    "Video sync:");
    //SetString(SID_PRESET_AUDIO_ENCODING,                "Audio encoding");
    SetString(SID_PRESET_NUM_AUDIO_CHANNELS,            "Number of audio channels:");
    //SetString(SID_PRESET_SUBTITLE_ENCODING,             "Subtitle encoding");
    SetString(SID_PRESET_FILTER_INFO,                   "INFO: Filters are applied in the order they appear in the list above..");
    SetString(SID_PRESET_METADATA_DEFINE,               "Please define key-value-pairs for metadata");
    SetString(SID_PRESET_METADATA_KEY,                  "Key");
    SetString(SID_PRESET_METADATA_VALUE,                "Value");
    SetString(SID_PRESET_DEFINE_FOURCC,                 "Change FOURCC for AVI files");
    SetString(SID_PRESET_FOURCC_VIDS,                   "Video (vids):");
    SetString(SID_PRESET_FOURCC_AUDS,                   "Audio (auds):");
    SetString(SID_PRESET_BURN_IN_SUBTITLES,             "Burn in subtitles (requires video encoding and \"BURN-IN\" subtitle-codec selected)");
    SetString(SID_PRESET_SUBTITLE_TYPE,                 "Subtitles type:");
    SetString(SID_PRESET_SUBTITLE_TYPE_TEXT,            "Text (SRT / SubRip, ASS / Advanced SSA etc.)");
    SetString(SID_PRESET_SUBTITLE_TYPE_BITMAP,          "Bitmap (DVB Subtitles, XSUB etc.)");
    SetString(SID_PRESET_SUBTITLE_SCALE,                "Scale subtitles:");
    SetString(SID_PRESET_SUBTITLE_SIZE,                 "Subtitles size:");
    SetString(SID_PRESET_SUBTITLE_SIZE_DEFAULT,         "Default");
    SetString(SID_PRESET_SUBTITLE_SIZE_PAL,             "PAL (DVB_Subs)");
    SetString(SID_PRESET_SUBTITLE_SIZE_CUSTOM,          "Custom:");
    //SetString(SID_PRESET_PIXELS_AND_ASPECTS,            "Pixels and aspects");
    SetString(SID_PRESET_PIXEL_FORMAT,                  "Pixel format:");
    SetString(SID_PRESET_ASPECT_RATIO,                  "Aspect ratio:");
    SetString(SID_PRESET_CUSTOM_CMDLINE_ARGS,           "Custom command line arguments");
    SetString(SID_PRESET_MANAGER_TITLE,                 "Preset manager");
    SetString(SID_PRESET_DELETE_FILTERS,                "Remove selected filter(s)?");
    SetString(SID_PRESET_ONLY_ONE_DEINTERLACE,          "Only one deinterlace filter can be added");
    SetString(SID_PRESET_NAME_IS_REQUIRED,              "You must specify a name for the preset");
    SetString(SID_PRESET_INVALID_FRAME_RATE,            "Frame rate is invalid: integers, floats and nominator/denominator values only");
    SetString(SID_PRESET_INVALID_FILTERS_ADDED,         "You have added video and/or audio filters that cannot be applied when \"copy\" is used as codec. Continue anyway?");
    SetString(SID_PRESET_INVALID_METADATA,              "Metadata contains illegal value(s)");
    SetString(SID_PRESET_NO_PREVIEWABLE_FILTERS,        "No preview-able filters are in the chain, preview aborted");
    SetString(SID_PRESET_PREVIEW_FAILED,                "Unable to start FFPlay with command: %s");
    SetString(SID_PRESET_INVALID_QSCALE,                "Valid values for using QScale (VBR) are 1 (best quality) to 31 (lowest quality)");
    SetString(SID_PRESET_INVALID_SUBTITLE_SIZE,         "Custom subtitle size must be specified as a positive number");
    SetString(SID_PRESET_QUALITY,                       "Quality");
    SetString(SID_PRESET_QUALITY_SCALE,                 "Quality setting:");
    SetString(SID_PRESET_CONSTANT_RATE,                 "Best constant quality:");
    SetString(SID_PRESET_MIN_CONSTANT_RATE,             "Worst constant quality:");
    SetString(SID_PRESET_QUANTIZER_MIN,                 "Best quantizer quality:");
    SetString(SID_PRESET_QUANTIZER_MAX,                 "Worst quantizer quality:");
    SetString(SID_PRESET_QUANTIZER_MAX_DIFF,            "Max. quantizer difference:");
    SetString(SID_PRESET_BITRATE_DECLARED_AS,           "Bit rate declared as:");
    SetString(SID_PRESET_AVERAGE_BITRATE,               "Average bit rate:");
    SetString(SID_PRESET_MINIMUM_BITRATE,               "Minimum bit rate:");
    SetString(SID_PRESET_MAXIMUM_BITRATE,               "Maximum bit rate:");
    SetString(SID_PRESET_BUFFER_SIZE,                   "Buffer size:");
    SetString(SID_PRESET_BUFFER_SIZE_REQUIRED,          "Buffer size must be specified when using maximum rate");
    SetString(SID_PRESET_KEYFRAMES,                     "Key frames");
    SetString(SID_PRESET_KEYFRAME_MINIMUM,              "Shortest distance between key frames:");
    SetString(SID_PRESET_KEYFRAME_MAXIMUM,              "Longest distance between key frames:");
    SetString(SID_PRESET_KEYFRAME_INFO,                 "(in frames)");
    SetString(SID_PRESET_MOTION_EST_METHOD,             "Motion estimation method:");
    SetString(SID_PRESET_SCENE_CHANGE_SENSITIVITY,      "Scene change sensitivity:");
    SetString(SID_PRESET_SCENE_CHANGE_SENS_INFO,        "%i%% picture change");
    SetString(SID_PRESET_LOW,                           "Low");
    SetString(SID_PRESET_HIGH,                          "High");
    SetString(SID_PRESET_OTHER,                         "Other");
    SetString(SID_PRESET_THREADS,                       "Threads:");
    SetString(SID_PRESET_TRELLIS,                       "Trellis:");
    SetString(SID_PRESET_TRELLIS_LIST,                  "Undefined|Don't use trellis|Only final encode|Use always");
    SetString(SID_PRESET_SPEED_PRESET,                  "Speed preset:");
    SetString(SID_PRESET_CUSTOM_ARGS_PASS_1,            "First pass:");
    SetString(SID_PRESET_CUSTOM_ARGS_PASS_2,            "Second pass:");
    SetString(SID_PRESET_AUDIO_PROFILE,                 "Audio profile:");
    SetString(SID_PRESET_AUDIO_FILTERS_COMPLEX,         "Complex audio filters (use '-filter_complex' rather than '-af')");
    SetString(SID_PRESET_MOVFLAGS_FASTSTART,            "Enable faststart for better streaming");
    SetString(SID_PRESET_KEYFRAME_MAX_B_FRAMES,         "Maximum of consecutive B-frames:");
    SetString(SID_PRESET_KEYFRAME_MAX_B_FRAME_ITEMS,    "Undefined|Automatic|0 (disabled)|%u frames");
    SetString(SID_PRESET_KEYFRAME_MAX_P_REFS,           "Maximum reference frames for P-frames:");
    SetString(SID_PRESET_OUTPUT_FORMAT,                 "Output format:");
    SetString(SID_PRESET_MODIFY_ACTIVE_ERROR,           "This operation cannot be performed because a job depends on the preset!");
    SetString(SID_PRESET_KEEP_FILETIME,                 "Set file time of output file to file time of (first) input file");
    SetString(SID_PRESET_SUBTITLE_CHARENC,              "Character encoding:");
    SetString(SID_PRESET_SEGMENTING,                    "Segmenting");
    SetString(SID_PRESET_SEGMENTING_LEN,                "Make segments with the length:");
    SetString(SID_PRESET_SEGMENTING_LEN_TYPE,           "Seconds|Minutes|Hours");
    SetString(SID_PRESET_SEGMENTING_LIST_FILE,          "List file:");
    SetString(SID_PRESET_SEGMENTING_LIST_TYPE,          "None|Flat|CSV|FFconcat|M3U8");
    SetString(SID_PRESET_SEGMENTING_BREAK_BFRAMES,      "Break non-keyframes");
    SetString(SID_PRESET_SEGMENTING_INCREMENT_TIME,     "Incremental time code");
    SetString(SID_PRESET_SEGMENTING_STREAMING,          "For streaming");
    SetString(SID_PRESET_SEGMENTING_RESET_TIME,         "Reset timestamps");
    SetString(SID_PRESET_USE_CLOSED_GOP,                "Closed GOP (Group Of Pictures)");
    SetString(SID_PRESET_HWDECODE,                      "HW Decoding");
    SetString(SID_PRESET_HWDECODE_ACCEL,                "Accelerator:");
    SetString(SID_PRESET_HWDECODE_CODEC,                "Codec:");
    SetString(SID_PRESET_HWDECODE_DEVICE,               "Device:");
    SetString(SID_PRESET_HWDECODE_DEVICE_INIT,          "Device initializer:");
    SetString(SID_PRESET_ERROR_NO_COMMA_ALLOWED,        "Comma is not allowed in this field!");
    SetString(SID_PRESET_VIDEO_TUNING,                  "Video tuning:");
    SetString(SID_PRESET_METADATA_FOR,                  "Set metadata for:");
    SetString(SID_PRESET_METADATA_FOR_ITEMS,            "All (global)|Video|Audio|Subtitle");
    SetString(SID_PRESET_DISPOSITION,                   "Dispositions");
    SetString(SID_PRESET_DISPOSITION_UNSET,             "Un-set existing");


    //Video sync mode strings
    SetString(SID_PRESET_VIDEO_SYNC_AUTO,           "CFR or VFR depending on muxer");
    SetString(SID_PRESET_VIDEO_SYNC_PASSTHROUGH,    "timestamps are passed from demuxer to muxer");
    SetString(SID_PRESET_VIDEO_SYNC_CFR,            "duplicate or drop frames to maintain constant frame rate");
    SetString(SID_PRESET_VIDEO_SYNC_VFR,            "no duplicate but drop frames with same timestamp");
    SetString(SID_PRESET_VIDEO_SYNC_DROP,           "passthrogh but muxer creates new timestamps");

    //Bit rate mode strings
    SetString(SID_PRESET_BITRATE_BIT_SEC,           "bit/sec");
    SetString(SID_PRESET_BITRATE_KBIT_SEC,          "kbits/sec");
    SetString(SID_PRESET_BITRATE_MBIT_SEC,          "mbits/sec");
    SetString(SID_PRESET_BITRATE_BYTE_SEC,          "byte/sec");
    SetString(SID_PRESET_BITRATE_KBYTE_SEC,         "kbytes/sec");
    SetString(SID_PRESET_BITRATE_MBYTE_SEC,         "mbytes/sec");
    SetString(SID_PRESET_BITRATE_QSCALE,            "QScale (VBR)");


    //Options edit UI strings
    SetString(SID_OPTIONS_TITLE,                    "FFQueue options");
    SetString(SID_OPTIONS_FFMPEG_PATH,              "Path to ffmpeg and ffprobe binaries (executables)");
    SetString(SID_OPTIONS_TEMPORARY_PATH,           "Temporary path");
    SetString(SID_OPTIONS_TEMP_SYS_DEFAULT,         "Use system default");
    SetString(SID_OPTIONS_TEMP_DEST_PATH,           "Use destination path");
    SetString(SID_OPTIONS_TEMP_CUSTOM,              "Use custom path below:");
    SetString(SID_OPTIONS_FIND_2ND_INPUT_EXTS,      "Automatically find secondary file from these extensions");
    SetString(SID_OPTIONS_SAVE_JOBS_ON_EXIT,        "Save unprocessed jobs in the query list on exit (and reload on startup)");
    SetString(SID_OPTIONS_REMOVE_FINISHED_JOBS,     "Automatically remove processed jobs from query list");
    SetString(SID_OPTIONS_LIMIT_STATISTICS,         "Limit statistics from FFMpeg");
    SetString(SID_OPTIONS_SHOW_BANNER_INFO,         "Show banner-information from FFMpeg when processing jobs");
    SetString(SID_OPTIONS_EDIT_LANGUAGE,            "Language...");
    SetString(SID_OPTIONS_SAVE_WINDOW_POS,          "Save position of main window for next session");
    SetString(SID_OPTIONS_FULL_CODEC_LIST,          "Enable full codec listings (advanced users)");
    SetString(SID_OPTIONS_ALWAYS_UNIQUE_NAMES,      "Always suggest unique, non-existing file names");
    SetString(SID_OPTIONS_CUSTOM_PLAYER,            "Custom player for preview (must support piping, like VLC)");
    SetString(SID_OPTIONS_KEEP_CONSOLE_OPEN,        "Keep console open after preview finishes");
    SetString(SID_OPTIONS_CUSTOM_CONSOLE_CMD,       "Custom console command");
    SetString(SID_OPTIONS_SILENT_QUEUE_FINISH,      "Silent queue finish");
    SetString(SID_OPTIONS_SAVE_JOBS_ON_MODIFY,      "Save jobs whenever they are modified");
    SetString(SID_OPTIONS_VALIDATE_ON_LOAD,         "Remove jobs with non-existing input files during startup");
    SetString(SID_OPTIONS_OUTPUT_NAME_PATTERN,      "Pattern used to generate output names");
    SetString(SID_OPTIONS_CONFIRM_DELETE_JOBS,      "Confirm removal of jobs");
    SetString(SID_OPTIONS_PREVIEW_MAP_SUBS,         "Always map subtitles when previewing");


    //Thumb maker UI strings
    SetString(SID_THUMBS_TITLE,                     "Thumb maker");
    SetString(SID_THUMBS_INPUT_FILE,                "Input file");
    SetString(SID_THUMBS_MAKE,                      "Make...");
    SetString(SID_THUMBS_CREATE_THUMBS,             "Create thumbnails");
    SetString(SID_THUMBS_CREATE_MOSAICS,            "Create tiled mosaics");
    SetString(SID_THUMBS_DEST_NEW_UNIQUE,           "New, unique sub folder made from input name");
    SetString(SID_THUMBS_DEST_SAME,                 "Same folder as input file");
    SetString(SID_THUMBS_DEST_THUMBS,               "Sub folder \"Thumbs\" (created if needed)");
    SetString(SID_THUMBS_IMAGE_FORMAT,              "Image format");
    SetString(SID_THUMBS_WIDTH,                     "Thumbnail width");
    SetString(SID_THUMBS_HEIGHT_INFO,               "INFO: Height is being auto-calculated");
    SetString(SID_THUMBS_INTERVAL,                  "Create thumb every");
    SetString(SID_THUMBS_INTERVAL_FRAME,            "Frame");
    SetString(SID_THUMBS_INTERVAL_SECOND,           "Second");
    SetString(SID_THUMBS_INTERVAL_MINUTE,           "Minute");
    SetString(SID_THUMBS_SCENE_CHANGE,              "Create thumb for each scene change (estimated)");
    SetString(SID_THUMBS_MAX_THUMBS,                "Max. amount of: Thumbnails =");
    SetString(SID_THUMBS_MAX_MOSAICS,               "Mosaics =");
    SetString(SID_THUMBS_MOSAIC_ROWS,               "Mosaic rows =");
    SetString(SID_THUMBS_MOSAIC_COLS,               "columns =");


    //Time edit UI strings
    SetString(SID_TIMEEDIT_TITLE,                   "Edit stream time");
    SetString(SID_TIMEEDIT_ENTER_VALUE,             "Enter time value");
    SetString(SID_TIMEEDIT_HOURS,                   "Hours:");
    SetString(SID_TIMEEDIT_MINUTES,                 "Minutes:");
    SetString(SID_TIMEEDIT_SECONDS,                 "Seconds:");
    SetString(SID_TIMEEDIT_MILLISECONDS,            "Milliseconds:");
    SetString(SID_TIMEEDIT_SUBTRACT_VALUES,         "Subtract time values");
    SetString(SID_TIMEEDIT_SUBTRACT_TIME,           "Subtract start of %s (%s)");
    SetString(SID_TIMEEDIT_NOT_AVAILABLE,           "Not available");


    //About box UI strings
    SetString(SID_ABOUT_TITLE,                      "About");


    //FFQVidStab UI strings
    SetString(SID_VIDSTAB_TITLE,                    "Two pass video stabilization");
    SetString(SID_VIDSTAB_SHAKINESS,                "Shakiness");
    SetString(SID_VIDSTAB_ACCURACY,                 "Accuracy");
    SetString(SID_VIDSTAB_STEP_SIZE,                "Step size");
    SetString(SID_VIDSTAB_MIN_CONTRAST,             "Min. contrast");
    SetString(SID_VIDSTAB_TRIPOD_REF_FRAME,         "Tripod reference frame (0 = disabled)");
    SetString(SID_VIDSTAB_1ST_PASS,                 "First pass (detection)");
    SetString(SID_VIDSTAB_2ND_PASS,                 "Second pass (translation)");
    SetString(SID_VIDSTAB_IN_FILE,                  "Video file to stabilize");
    SetString(SID_VIDSTAB_DESTINATION,              "Destination");
    SetString(SID_VIDSTAB_SMOOTHING,                "Smoothing (number of frames used for movement filtering)");
    SetString(SID_VIDSTAB_MAX_SHIFT,                "Max. allowed number of pixels to shift (-1 = unlimited)");
    SetString(SID_VIDSTAB_MAX_ANGLE,                "Max. allowed number of degrees to rotate (-1 = unlimited)");
    SetString(SID_VIDSTAB_OPT_ALGO,                 "Camera path optimization algorithm");
    SetString(SID_VIDSTAB_OPT_ALGO_ITEMS,           "Gaussian kernel low-pass|Averaging on transforms");
    SetString(SID_VIDSTAB_CROP_FILL_BLACK,          "Use solid black rather than keeping pixels from previous frame for borders");
    SetString(SID_VIDSTAB_INVERT,                   "Invert transformations");
    SetString(SID_VIDSTAB_RELATIVE,                 "Consider transformations as relative to previous frame");
    SetString(SID_VIDSTAB_VIRTUAL_TRIPOD,           "Virtual tripod mode (same as Relative=0+Smoothing=0)");
    SetString(SID_VIDSTAB_INTERPOLATION,            "Interpolation");
    SetString(SID_VIDSTAB_INTERPOLATION_ITEMS,      "No interpolation|Linear only horizontal|Linear in both directions (default)|Cubic in both directions (slow)");
    SetString(SID_VIDSTAB_OPT_ZOOM,                 "Optimal zoom");
    SetString(SID_VIDSTAB_OPT_ZOOM_ITEMS,           "No zoom|Optimal static zoom|Optimal adaptive zoom");
    SetString(SID_VIDSTAB_ZOOM,                     "Zoom percentage (<0 = zoom out, >0 zoom in)");
    SetString(SID_VIDSTAB_ZOOM_SPEED,               "Max. zoom percent per frame (requires optimal adaptive zoom)");
    SetString(SID_VIDSTAB_ONLY_1ST_PASS,            "Only run first pass to visualize transformations in destination file");
    SetString(SID_VIDSTAB_ONLY_2ND_PASS,            "Only run second pass (requires transformations from a previous 1st pass)");
    SetString(SID_VIDSTAB_KEEP_TRANSFORMS,          "Keep transformations for later 2nd pass (stored as *.trf with the source file)");
    SetString(SID_VIDSTAB_VERBOSE,                  "Make detailed visualization");
    SetString(SID_VIDSTAB_OTHER_OPTIONS,            "Other options");
    SetString(SID_VIDSTAB_FILE_INFO,                "Duration:%s Video:%s %ix%i Audio:%i Subs:%i");


    //VideoToGIF UI strings
    SetString(SID_VIDEO2GIF_TITLE,                  "Convert video to GIF");
    SetString(SID_VIDEO2GIF_PROPERTIES,             "GIF properties");
    SetString(SID_VIDEO2GIF_WIDTH,                  "Width");
    SetString(SID_VIDEO2GIF_HEIGHT,                 "Height");
    SetString(SID_VIDEO2GIF_TWOPASS,                "2 passes for optimized palette / colors");
    SetString(SID_VIDEO2GIF_START_FROM,             "Start conversion from %s");
    SetString(SID_VIDEO2GIF_VIDEO_INFO,             "%ux%u @ %.2f fps, duration: %s");
    SetString(SID_VIDEO2GIF_NO_VIDEO,               "No video found in input file");
    SetString(SID_VIDEO2GIF_BEYOND_END_OF_VID,      "Start time and/or time length is beyond end of video");
    SetString(SID_VIDEO2GIF_PRECISE_CUTS,           "Use filters to increase cut precision");


    //Filter editor UI strings
    SetString(SID_FILTER_DEFINE_PROPERTIES,         "Define filter properties");
    SetString(SID_FILTER_INVALID_ID,                "Invalid FilterID: %u");


    //Common filter strings
    SetString(SID_FILTER_TIME_LIMIT,                "Time limit");
    SetString(SID_FILTER_TIME_LIMIT_FROM,           "filter is enabled from");
    SetString(SID_FILTER_TIME_LIMIT_TO,             "to");
    SetString(SID_FILTER_TIME_LIMIT_START,          "start");
    SetString(SID_FILTER_TIME_LIMIT_END,            "end");
    SetString(SID_FILTER_TIME_LIMIT_FRIENDLY,       "from:%s to:%s");
    SetString(SID_FILTER_TIME_LIMIT_ERROR,          "The enabled timespan is invalid; from is after to!");
    SetString(SID_FILTER_POSITION_LEFT,             "left:");
    SetString(SID_FILTER_POSITION_TOP,              "top:");
    SetString(SID_FILTER_POSITION_WIDTH,            "width:");
    SetString(SID_FILTER_POSITION_HEIGHT,           "height:");
    SetString(SID_FILTER_WIDTH_HEIGHT_REQUIRED,     "Width and height are required");
    SetString(SID_FILTER_THIS,                      "This");
    //Subtitle burn in
    SetString(SID_SUBSBURNIN_USERFRIENDLY,          "Subtitle BURN-IN (only for placement in filter chain)");
    SetString(SID_SUBSBURNIN_EDIT_DEL_INFO,         "The subtitle filter is only used for placement in the filter chain. To remove it, select another subtitle codec than \"BURN-IN\". To edit its properties, go to the Miscellaneous tab.");
    //Subtitle burn in end
    SetString(SID_FILTER_UNEDITABLE,                "This filter has no properties and cannot be edited");



    //BoxblurFilterPanel strings
    SetString(SID_BOXBLUR_USERFRIENDLY,             "%s=luma:%i,%i chroma:%i,%i alpha:%i,%i %s");
    SetString(SID_BOXBLUR_ALPHA,                    "Alpha");
    SetString(SID_BOXBLUR_CHROMA,                   "Chroma");
    SetString(SID_BOXBLUR_LUMA,                     "Luma");
    SetString(SID_BOXBLUR_RADIUS,                   "radius:");
    SetString(SID_BOXBLUR_POWER,                    "power:");
    SetString(SID_BOXBLUR_RADIUS_ERROR,             "%s radius cannot be 0");
    SetString(SID_BOXBLUR_POWER_ERROR,              "Either of the powers must be specified");


    //ChannelMapFilterPanel strings
    SetString(SID_CHMAP_USERFRIENDLY,               "%s=%s");
    SetString(SID_CHMAP_CHANNEL_LAYOUT,             "Channel layout");
    SetString(SID_CHMAP_CHANNEL_REMAPPINGS,         "Channel remappings");
    SetString(SID_CHMAP_SOURCE_CHANNEL,             "Source channel");
    SetString(SID_CHMAP_DESTINATION_CHANNEL,        "Destination channel");


    //ColorbalanceFilterPanel strings
    SetString(SID_CLRBAL_USERFRIENDLY,              "%s=shadow:%.2f,%.2f,%.2f midtone:%.2f,%.2f,%.2f highlight:%.2f,%.2f,%.2f %s");
    SetString(SID_CLRBAL_SHADOW_ADJUST,             "Shadow adjustments");
    SetString(SID_CLRBAL_HIGHLIGHT_ADJUST,          "Highlight adjustments");
    SetString(SID_CLRBAL_MIDTONE_ADJUST,            "Midtone adjustments");
    SetString(SID_CLRBAL_LOCK_IN_GROUPS,            "Lock red-green-blue inside groups");
    SetString(SID_CLRBAL_LOCK_ACROSS_GROUPS,        "Lock colors across groups");
    SetString(SID_CLRBAL_RED,                       "Red");
    SetString(SID_CLRBAL_GREEN,                     "Green");
    SetString(SID_CLRBAL_BLUE,                      "Blue");

    //ColorFilterPanel strings
    SetString(SID_COLOR_USERFRIENDLY,               "%s=%s pos:%u,%u size:%ux%u opacity:%.1f%% %s");
    SetString(SID_COLOR_PICK_COLOR,                 "Pick a color");
    SetString(SID_COLOR_VISIBILITY,                 "Visibility");
    SetString(SID_COLOR_VISIBILITY_NONE,            "none");
    SetString(SID_COLOR_VISIBILITY_OPAQUE,          "opaque");
    SetString(SID_COLOR_POSITION,                   "Color position");
    SetString(SID_COLOR_SIZE,                       "Color size");
    SetString(SID_COLOR_SIZE_ERROR,                 "Color dimensions must be specified");

    //ColorMatrixFilterPanel strings
    SetString(SID_CLRMATRIX_USERFRIENDLY,           "%s=%s change to %s");
    SetString(SID_CLRMATRIX_SRC_MATRIX,             "Source matrix");
    SetString(SID_CLRMATRIX_DST_MATRIX,             "Destination matrix");
    SetString(SID_CLRMATRIX_ERROR,                  "Source and destination cannot be equal");

    //CropFilterPanel strings
    SetString(SID_CROP_USERFRIENDLY,                "%s=left:%u top:%u right:%u bottom:%u");
    SetString(SID_CROP_LEFT,                        "Left");
    SetString(SID_CROP_TOP,                         "Top");
    SetString(SID_CROP_RIGHT,                       "Right");
    SetString(SID_CROP_BOTTOM,                      "Bottom");
    SetString(SID_CROP_ERROR,                       "Empty crop filter (0 values) cannot be applied");

    //CurvesFilterPanel strings
    SetString(SID_CURVES_USERFRIENDLY,              "%s=preset:%s master:%s red:%s green:%s blue:%s all:%s");
    SetString(SID_CURVES_PRESET,                    "Select preset");
    SetString(SID_CURVES_MASTER_KEYS,               "Master key points");
    SetString(SID_CURVES_RED_KEYS,                  "Red key points");
    SetString(SID_CURVES_GREEN_KEYS,                "Green key points");
    SetString(SID_CURVES_BLUE_KEYS,                 "Blue key points");
    SetString(SID_CURVES_ALL_KEYS,                  "All key points");
    SetString(SID_CURVES_ERROR,                     "Custom values cannot contain characters \",'");
    SetString(SID_CURVES_PRESET_BASE + 0,           "None (define custom properties below)");
    SetString(SID_CURVES_PRESET_BASE + 1,           "Color negative");
    SetString(SID_CURVES_PRESET_BASE + 2,           "Cross process");
    SetString(SID_CURVES_PRESET_BASE + 3,           "Darker");
    SetString(SID_CURVES_PRESET_BASE + 4,           "Increase contrast");
    SetString(SID_CURVES_PRESET_BASE + 5,           "Lighter");
    SetString(SID_CURVES_PRESET_BASE + 6,           "Linear contrast");
    SetString(SID_CURVES_PRESET_BASE + 7,           "Medium contrast");
    SetString(SID_CURVES_PRESET_BASE + 8,           "Negative");
    SetString(SID_CURVES_PRESET_BASE + 9,           "Strong contrast");
    SetString(SID_CURVES_PRESET_BASE + 10,          "Vintage");


    //CustomFilterPanel strings
    SetString(SID_CUSTFLTR_USERFRIENDLY,            "%s=%s %s");
    SetString(SID_CUSTFLTR_TYPE,                    "Filter type:");
    SetString(SID_CUSTFLTR_VALUE,                   "Filter:");
    SetString(SID_CUSTFLTR_ERROR,                   "Empty filter or illegal character sequence \"%s\"");

    //DeinterlaceFilterPanel strings
    SetString(SID_DEINTER_USERFRIENDLY,             "%s=yadif %s parity:%s frames:%s");
    SetString(SID_DEINTER_MODE_NAME,                "Mode");
    SetString(SID_DEINTER_MODE_VALUES,              "Output one frame for each frame|Output one frame for each field|Output one frame for each frame (skip spartial check)|Output one frame for each field (skip spartial check)");
    SetString(SID_DEINTER_MODE_FRIENDLY,            "per frame|per field|per frame (no-check)|per field (no-check)");
    SetString(SID_DEINTER_PARITY_NAME,              "Parity");
    SetString(SID_DEINTER_PARITY_VALUES,            "Auto detect|Top field first|Bottom field first");
    SetString(SID_DEINTER_PARITY_FRIENDLY,          "auto|top|bottom");
    SetString(SID_DEINTER_DEINTERLACE_NAME,         "Deinterlace");
    SetString(SID_DEINTER_DEINTERLACE_VALUES,       "All frames|Only interlaced frames");
    SetString(SID_DEINTER_DEINTERLACE_FRIENDLY,     "all|interlaced");
    SetString(SID_DEINTER_FILTER_TYPE,              "Select which de-interlace filter to use:");
    SetString(SID_DEINTER_FILTER_TYPE_NAMES,        "yadif|kerndeint");
    SetString(SID_DEINTER_KDEINT_USERFRIENDLY,      "%s=kerndeint thresh:%i map:%s swap:%s sharp:%s twoway:%s");
    SetString(SID_DEINTER_KDEINT_TRESH,             "Pixel processing threshold");
    SetString(SID_DEINTER_KDEINT_MAP,               "Map pixels to white if threshold is exceeded");
    SetString(SID_DEINTER_KDEINT_ORDER,             "Swap fields order");
    SetString(SID_DEINTER_KDEINT_SHARP,             "Enable additional sharpening");
    SetString(SID_DEINTER_KDEINT_TWOWAY,            "Enable two way sharpening");

    //DejudderFilterPanel strings
    SetString(SID_DEJUDDER_USERFRIENDLY,            "%s=cycle:%u");
    SetString(SID_DEJUDDER_FILM_2_NTSC,             "Film to NTSC (cycle = 4)");
    SetString(SID_DEJUDDER_PAL_2_NTSC,              "PAL to NTSC (cycle = 5)");
    SetString(SID_DEJUDDER_MIX_OF_BOTH,             "Mix of both above (cycle = 20)");
    SetString(SID_DEJUDDER_CUSTOM,                  "Custom cycle =");
    SetString(SID_DEJUDDER_CUSTOM_ERROR,            "Custom cycle must be at least 2");

    //DelogoFilterPanel strings
    SetString(SID_DELOGO_USERFRIENDLY,              "%s=%u,%u size:%ux%u blur:%s %s");
    SetString(SID_DELOGO_POSITION,                  "Logo position");
    SetString(SID_DELOGO_SIZE,                      "Logo size");
    SetString(SID_DELOGO_SIZE_ERROR,                "Logo size must be specified");
    SetString(SID_DELOGO_BLUR_EDGE,                 "Blur edge");
    SetString(SID_DELOGO_USE_IMG_MASK,              "Use an image mask to obscure logo:");
    SetString(SID_DELOGO_IMG_MASK_ERROR,            "The image mask file does not exist");

    //DeshakeFilterPanel strings
    SetString(SID_DESHAKE_USERFRIENDLY,             "%s=detect:%s shake:%ix%i fill:%s blocksize:%i contrast:%i search:%s opencl=%s");
    SetString(SID_DESHAKE_DETECT_FRAME_1,           "Detect whole frame");
    SetString(SID_DESHAKE_DETECT_FRAME_2,           "Use entire frame to detect shake");
    SetString(SID_DESHAKE_DETECT_POSITION,          "Detection box position");
    SetString(SID_DESHAKE_DETECT_SIZE,              "Detection box size");
    SetString(SID_DESHAKE_DETECT_ENTIRE_FRAME,      "entire frame");
    SetString(SID_DESHAKE_MAX_SHAKE,                "Maximum shake");
    SetString(SID_DESHAKE_HORZ,                     "horizontal:");
    SetString(SID_DESHAKE_VERT,                     "vertical:");
    SetString(SID_DESHAKE_FILL_EDGE,                "Fill blank edge");
    SetString(SID_DESHAKE_FILL_EDGE_VALUES,         "Fill with black|Keep original pixels|Extrude edge value|Mirrored original");
    SetString(SID_DESHAKE_FILL_EDGE_FRIENDLY,       "black|original|extrude|mirror");
    SetString(SID_DESHAKE_BLOCK_SIZE,               "Motion block size");
    SetString(SID_DESHAKE_BLOCK_SIZE_ERROR,         "Block size must be at least 4");
    SetString(SID_DESHAKE_MIN_CONTRAST,             "Minimum contrast");
    SetString(SID_DESHAKE_MIN_CONTRAST_ERROR,       "Contrast must be at least 1");
    SetString(SID_DESHAKE_SEARCH,                   "Search");
    SetString(SID_DESHAKE_SEARCH_VALUES,            "Simple|Exhaustive");
    SetString(SID_DESHAKE_OPENCL,                   "OpenCL");
    SetString(SID_DESHAKE_USE_OPENCL,               "Use OpenCL capabilities (might cause ffmpeg to fail)");


    //EchoFilterPanel strings
    SetString(SID_ECHO_USERFRIENDLY,                "%s=%.3g,%.3g delays:%s decays:%s");
    SetString(SID_ECHO_GAIN,                        "Gain");
    SetString(SID_ECHO_GAIN_INPUT,                  "input:");
    SetString(SID_ECHO_GAIN_OUTPUT,                 "output:");
    SetString(SID_ECHO_GAIN_ERROR,                  "Gain (input and output) must be above zero");
    SetString(SID_ECHO_DELAYS,                      "Delays");
    SetString(SID_ECHO_DECAYS,                      "Decays");
    SetString(SID_ECHO_DELAY_DECAY_ERROR,           "At least one delay and decay must be specified");

    //EqualizerFilterPanel strings
    SetString(SID_EQUALIZER_USERFRIENDLY,           "%s=%s");
    SetString(SID_EQUALIZER_ERROR,                  "All adjustments cannot be 0");
    SetString(SID_EQUALIZER_BAND_WIDTH,             "Define the width of each EQ-band");

    //FlangerFilterPanel strings
    SetString(SID_FLANGER_USERFRIENDLY,             "%s=%i depth:%i regen:%i width:%i speed:%g phase:%i %s %s");
    SetString(SID_FLANGER_DELAY,                    "Delay");
    SetString(SID_FLANGER_DEPTH,                    "Depth");
    SetString(SID_FLANGER_REGENERATION,             "Regeneration");
    SetString(SID_FLANGER_WIDTH,                    "Width");
    SetString(SID_FLANGER_SPEED,                    "Speed");
    SetString(SID_FLANGER_PHASE,                    "Phase");
    SetString(SID_FLANGER_SHAPE,                    "Shape");
    SetString(SID_FLANGER_SHAPE_SIN,                "Sinusoidal");
    SetString(SID_FLANGER_SHAPE_TRI,                "Triangular");
    SetString(SID_FLANGER_INTERPOLATION,            "Interpolation");
    SetString(SID_FLANGER_INTERPOLATION_LINE,       "Linear");
    SetString(SID_FLANGER_INTERPOLATION_QUAD,       "Quadratic");

    //FlipFilterPanel strings
    SetString(SID_FLIP_USERFRIENDLY,                "%s=horizontal:%s vertical:%s");
    SetString(SID_FLIP_HORZ,                        "Flip horizontally (mirror)");
    SetString(SID_FLIP_VERT,                        "Flip vertically (bottom up)");
    SetString(SID_FLIP_ERROR,                       "Either horizontal or vertical flip must be checked");

    //HighLowBandPassFilterPanel strings
    SetString(SID_BANDPASS_USERFRIENDLY,            "%s=%sHz%s width:%g%s");
    SetString(SID_BANDPASS_FREQUENCY,               "frequency:");
    SetString(SID_BANDPASS_HERZ,                    "Herz");
    SetString(SID_BANDPASS_SKIRT_GAIN,              "Constant skirt gain");
    SetString(SID_BANDPASS_SKIRT_GAIN_FRIENDLY,     "csg:");
    SetString(SID_BANDPASS_POLES,                   "poles:");
    SetString(SID_BANDPASS_BAND_WIDTH,              "band width:");
    SetString(SID_BANDPASS_BAND_WIDTH_TYPES,        "Herz|Q-Factor|Octave|Slope");
    SetString(SID_BANDPASS_BAD_FILTERTYPE,          "Invalid band pass/reject filter type: %u");

    //PadFilterPanel strings
    SetString(SID_PAD_USERFRIENDLY,                 "%s=%ix%i color:%s");
    SetString(SID_PAD_COLOR,                        "Padding color");
    SetString(SID_PAD_SIZE,                         "Padded size");
    SetString(SID_PAD_BLUR,                         "Padding blur");
    SetString(SID_PAD_USERFRIENDLY_BLUR,            "blur=%i");

    //PhaserFilterPanel strings
    SetString(SID_PHASER_USERFRIENDLY,              "%s=%.3g,%.3g delay:%.3g decay:%.3g speed:%.3g type:%s");
    SetString(SID_PHASER_DECAY,                     "Decay");
    SetString(SID_PHASER_TYPE,                      "Type");
    SetString(SID_PHASER_ERROR,                     "All values are required and must be above zero");

    //ResampleFilterPanel strings
    SetString(SID_RESAMPLE_USERFRIENDLY,            "%s=%u async:%s matrix:%s filter:%s dither:%s");
    SetString(SID_RESAMPLE_SAMPLE_RATE,             "Sample rate");
    SetString(SID_RESAMPLE_SAMPLE_RATE_ERROR,       "Sample rate is not valid");
    SetString(SID_RESAMPLE_AUDIO_SYNC,              "Audio sync");
    SetString(SID_RESAMPLE_MATRIX_ENCODING,         "Matrix encoding");
    SetString(SID_RESAMPLE_MATRIX_ENC_NAMES,        "Undefined|None|Dolby|Dolby Pro Logic II");
    SetString(SID_RESAMPLE_FILTER_TYPE,             "Filter type");
    SetString(SID_RESAMPLE_FILTER_TYPE_NAMES,       "Undefined|Cubic|Blackman Nuttall Windowed Sinc|Kaiser Windowed Sinc");
    SetString(SID_RESAMPLE_DITHER_METHOD,           "Dither method");
    SetString(SID_RESAMPLE_DITHER_METHOD_NAMES,     "Undefined|Rectangular|Triangular|Triangular w. high pass|Lipshitz noise shaping|Shibata noise shaping|Low shibata noise shaping|High shibata noise shaping|F-weighted noise shaping|Modified-e-weighted noise shaping|Improved-e-weighted noise shaping");
    SetString(SID_RESAMPLE_FORCE_RESAMPLE,          "Force resampling (even when input equals output)");
    SetString(SID_RESAMPLE_INTERPOLATION,           "Use linear interpolation");
    SetString(SID_RESAMPLE_CUSTOM_VALUES,           "Other values for the resampler:");
    SetString(SID_RESAMPLE_CUSTOM_VALUE_NAMES,      "Input channel count|Output channel count|Used channel count|Input sample rate|Output sample rate|Input sample format|Output sample format|Internal sample format|Input channel layout|Output channel layout|Center mix level|Surround mix level|LFE mix level|Rematrix volume|Rematrix max. value|Dither scale|Filter size|Phase shift|Cut off frequency|First PTS|Min. comp.|Min. hard comp.|Comp. duration|Max. soft comp.|Kaiser Window Beta|Output sample bits");
    SetString(SID_RESAMPLE_VALUE,                   "Value");
    SetString(SID_RESAMPLE_EMPTY_ERROR,             "Resample filter is empty");
    SetString(SID_RESAMPLE_KEEP_ORIGINAL_RATE,      "Do not change the sample rate");

    //RotateFilterPanel strings
    SetString(SID_ROTATE_USERFRIENDLY,              "%s=%.3f fill:%s bilinear:%s %s");
    SetString(SID_ROTATE_FILL_COLOR,                "Fill color");
    SetString(SID_ROTATE_ROTATE_ANGLE,              "Rotate angle");
    SetString(SID_ROTATE_ROTATE_ANGLE_ERROR,        "Angle must be a valid floating point value above zero");
    SetString(SID_ROTATE_BILINEAR_INTERPOLATE,      "Use bilinear interpolation");
    SetString(SID_ROTATE_ROTATE_CCW,                "Rotate counter-clockwise");

    //ScaleFilterPanel strings
    SetString(SID_SCALE_USERFRIENDLY,               "%s=width:%s height:%s algorithm:%s dither:%s interlace:%s color_space:%s=>%s sample_range:%s=>%s");
    SetString(SID_SCALE_EMPTY_INFO,                 "Empty field will be auto-calculated in order to maintain aspect ratio.");
    SetString(SID_SCALE_RESIZE_TO,                  "Resize to");
    SetString(SID_SCALE_PERCENTAGE,                 "Percentage scale (value of 50 = half size)");
    SetString(SID_SCALE_ALGORITHM,                  "Scaling algorithm");
    SetString(SID_SCALE_ALGORITHM_NAMES,            "Use default|Fast bilinear|Bilinear|Bicubic|Experimental|Nearest neighbor|Averaging area|Bicubic luma, bilinear chroma|Gaussian|Sinc|Lanczos|Natural bicubic spline");
    SetString(SID_SCALE_DITHER,                     "Dithering algorithm");
    SetString(SID_SCALE_DITHER_NAMES,               "Automatic|No dithering|Bayer|Error diffusion|Arithmetic dither (add)|Arithmetic dither (xor)");
    SetString(SID_SCALE_ACCURATE_ROUNDING,          "Accurate rounding");
    SetString(SID_SCALE_FULL_CHROMA_INTERPOLATE,    "Full chroma interpolation");
    SetString(SID_SCALE_FULL_CHROMA_INPUT,          "Full chroma input");
    SetString(SID_SCALE_BIT_EXACT_OUTPUT,           "Bit exact output");
    SetString(SID_SCALE_SIZE_ERROR,                 "Either width or height must be specified");
    SetString(SID_SCALE_NEGATIVE_NUMBER_ERROR,      "Negative numbers are not allowed");
    SetString(SID_SCALE_PERCENTAGE_ERROR,           "Percentage scale accepts only numeric values above zero");
    SetString(SID_SCALE_INTERLACE_AWARE,            "Interlaced aware");
    SetString(SID_SCALE_INTERLACE_AWARE_NAMES,      "No interlaced scaling|Forced on all frames|Only on interlaced frames");
    SetString(SID_SCALE_COLOR_SPACE,                "Color space");
    SetString(SID_SCALE_SAMPLE_RANGE,               "Sample range");
    SetString(SID_SCALE_FIT_RECT,                   "Keep aspect ratio but fit into rectangle");
    SetString(SID_SCALE_FIT_RECT_ERROR,             "Both width and height must be positive numbers in order to fit into rectangle");


    //UnsharpFilterPanel strings
    SetString(SID_UNSHARP_USERFRIENDLY,             "%s=%.3f/%.3f luma_matrix:%ix%i chroma_matrix:%ix%i %s");
    SetString(SID_UNSHARP_SHARPEN_AMOUNT,           "Sharpen amount");
    SetString(SID_UNSHARP_LUMA_SIZE,                "Luma matrix size");
    SetString(SID_UNSHARP_CHROMA_SIZE,              "Chroma matrix size");
    SetString(SID_UNSHARP_SHARPEN_ERROR,            "Sharpen amount cannot be \"None\" for both parameters");
    SetString(SID_UNSHARP_GROUP_LABELS,             "Softer|Sharper|Smaller|Larger|None|Luma|Chroma|X|Y|Lock values");

    //VignetteFilterPanel strings
    SetString(SID_VIGNETTE_USERFRIENDLY,            "%s=%g center:%s fwd:%s pr_frame:%s flicker:%s dither:%s %s");
    SetString(SID_VIGNETTE_ANGLE,                   "Angle");
    SetString(SID_VIGNETTE_ANGLE_ERROR,             "Angle must be a valid floating point value above zero");
    SetString(SID_VIGNETTE_ORIGIN,                  "Origin");
    SetString(SID_VIGNETTE_CALC_CENTER_COORDS,      "Calculate center coords (else define below:)");
    SetString(SID_VIGNETTE_FORWARD,                 "Forward vignette / Darken edge (otherwise lighten)");
    SetString(SID_VIGNETTE_CALC_EACH_FRAME,         "Calculate for each frame (slower encoding)");
    SetString(SID_VIGNETTE_APPLY_FLICKER,           "Apply flickering to vignette (requires pr. frame calc)");
    SetString(SID_VIGNETTE_REDUCE_BANDING,          "Reduce banding with dithering");

    //VolumeAndToneFilterPanel strings
    SetString(SID_VOLTONE_USERFRIENDLY,             "%s=%i bass=%i treble=%i");
    SetString(SID_VOLTONE_VOLUME,                   "Volume");
    SetString(SID_VOLTONE_BASS,                     "Bass");
    SetString(SID_VOLTONE_TREBLE,                   "Treble");
    SetString(SID_VOLTONE_ERROR,                    "All values cannot be 0 (no audio change)");

    //WatermarkFilterPanel strings
    SetString(SID_WATERMARK_USERFRIENDLY,           "%s=%u,%u scale:%s fade:%.2g %s file:%s");
    SetString(SID_WATERMARK_FILE,                   "Watermark file");
    SetString(SID_WATERMARK_FILE_ERROR,             "Watermark file must exist");
    SetString(SID_WATERMARK_POSITION,               "Position");
    SetString(SID_WATERMARK_SCALE_TO,               "Scale to");
    SetString(SID_WATERMARK_ASPECT_INFO,            "Use \"0\" as width or height to maintain aspect");
    SetString(SID_WATERMARK_FADE_IN_OUT,            "Fade in/out");
    SetString(SID_WATERMARK_FADE_INSTANT,           "instant");
    SetString(SID_WATERMARK_FADE_SLOW,              "slow (10 sec)");
    SetString(SID_WATERMARK_ALPHA_CHANNEL_INFO,     "Fade requires alpha channel in image");


    //NegateFilterPanel
    SetString(SID_NEGATE_USERFRIENDLY,              "%s=%s %s");
    SetString(SID_NEGATE_ALPHA_CHANNEL,             "Invert alpha channel (if available)");
    SetString(SID_NEGATE_ALPHA,                     "alpha");
    SetString(SID_NEGATE_IMAGE,                     "image");

    //ElbgFilterPanel
    SetString(SID_ELBG_USERFRIENDLY,                "%s=%u iterations:%u");
    SetString(SID_ELBG_COLORS,                      "Number of colors");
    SetString(SID_ELBG_ITERATIONS,                  "Maximum iterations");
    SetString(SID_ELBG_ERROR,                       "Number of colors and iterations must be positive");

    //AudioDelayFilterPanel
    SetString(SID_ADELAY_USERFRIENDLY,              "%s=%s %s");
    SetString(SID_ADELAY_CHANNEL_NUMBER,            "Channel number");
    SetString(SID_ADELAY_DELAY_MILLIS,              "Delay in milliseconds");
    SetString(SID_ADELAY_ERROR,                     "Delay for at least one channel must be specified");

    //AudioTempoFilterPanel
    SetString(SID_ATEMPO_USERFRIENDLY,              "%s=%s");
    SetString(SID_ATEMPO_HALF_SPEED,                "Half speed");
    SetString(SID_ATEMPO_DOUBLE_SPEED,              "Double speed");
    SetString(SID_ATEMPO_ERROR,                     "No tempo change selected");

    //DecimateFilterPanel
    SetString(SID_DECIMATE_USERFRIENDLY,            "%s=%u dup_thresh:%g sc_thresh:%u block:%ux%u preproc:%s chroma:%s");
    SetString(SID_DECIMATE_CYCLE,                   "Number of frames to drop one from");
    SetString(SID_DECIMATE_DUP_THRESHOLD,           "Threshold for duplicate detection");
    SetString(SID_DECIMATE_SC_THRESHOLD,            "Threshold for scene change detection");
    SetString(SID_DECIMATE_BLOCK_SIZE,              "Block size for metric calculations (W x H)");
    SetString(SID_DECIMATE_MAIN_PREPROCESSED,       "The main input is preprocessed");
    SetString(SID_DECIMATE_CHROMA_METRIC,           "Chroma is considered in metric calculations");
    SetString(SID_DECIMATE_CYCLE_ERROR,             "Number of frames must be above 1");
    SetString(SID_DECIMATE_BLOCK_SIZE_ERROR,        "Block size must be a power of 2 above zero");

    //InterlaceFilterPanel
    SetString(SID_INTERLACE_USERFRIENDLY,           "%s=%s %s low-pass:%s");
    SetString(SID_INTERLACE_SCAN,                   "Make interlaced frame from");
    SetString(SID_INTERLACE_EVEN,                   "Even lines");
    SetString(SID_INTERLACE_ODD,                    "Odd lines");
    SetString(SID_INTERLACE_LOWPASS,                "Enable vertical low-pass filter");
    SetString(SID_INTERLACE_FILTER_TYPE,            "Select which interlace filter to use:");
    SetString(SID_INTERLACE_FILTER_TYPE_NAMES,      "Default|Temporal");
    SetString(SID_INTERLACE_TINTRL_MODE,            "Mode");
    SetString(SID_INTERLACE_TINTRL_MODE_NAMES,      "Odd frames upper field, even frames lower field|" \
                                                    "Only output even frames|" \
                                                    "Only output odd frames|" \
                                                    "Pad alternate lines with black|" \
                                                    "Interleave odd top, even bottom|" \
                                                    "Interleave even top, odd bottom|" \
                                                    "Pad using previous and next temporal fields|" \
                                                    "Odd frames upper, even frames lower (maintain frame rate)");

    //FramestepFilterPanel
    SetString(SID_FRAMESTEP_USERFRIENDLY,           "%s=%u %s");
    SetString(SID_FRAMESTEP_SELECT,                 "Only include a frame for each");
    SetString(SID_FRAMESTEP_FRAMES,                 "frames");
    SetString(SID_FRAMESTEP_ERROR,                  "The number must be positive");

    //Hqdn3dFilterPanel
    SetString(SID_HQDN3D_USERFRIENDLY,              "%s=luma_spatial:%s luma_tmp:%s chroma_spatial:%s chroma_tmp:%s %s");
    SetString(SID_HQDN3D_LUMA_SPARTIAL,             "Luma spatial strength");
    SetString(SID_HQDN3D_CHROMA_SPARTIAL,           "Chroma spatial strength");
    SetString(SID_HQDN3D_LUMA_TEMP,                 "Luma temporal strength");
    SetString(SID_HQDN3D_CHROMA_TEMP,               "Chroma temporal strength");


    //HueFilterPanel
    SetString(SID_HUE_USERFRIENDLY,                 "%s=hue:%i saturation:%.2f brightness:%.2f %s");
    SetString(SID_HUE_HUE,                          "Hue degrees");
    SetString(SID_HUE_SATURATION,                   "Saturation");
    SetString(SID_HUE_BRIGHTNESS,                   "Brightness");
    SetString(SID_HUE_ERROR,                        "The filter does not do any changes");

    //LenscorrectionFilterPanel
    SetString(SID_LENSCORRECTION_USERFRIENDLY,      "%s=%.3fx%.3f k1:%.3f k2:%.3f");
    SetString(SID_LENSCORRECTION_CX,                "Relative horizontal origin");
    SetString(SID_LENSCORRECTION_CY,                "Relative vertical origin");
    SetString(SID_LENSCORRECTION_CX_CY_TEXT,        "Left|Right|Top|Bottom");
    SetString(SID_LENSCORRECTION_K1,                "Quadratic correction");
    SetString(SID_LENSCORRECTION_K2,                "Double quadratic correction");
    SetString(SID_LENSCORRECTION_K1_K2_TEXT,        "Negative|Positive");
    SetString(SID_LENSCORRECTION_ERROR,             "No correction has been selected");

    //NoiseFilterPanel
    SetString(SID_NOISE_USERFRIENDLY,               "%s=%s strength:%s flags:%s %s");
    SetString(SID_NOISE_SEED,                       "Seed");
    SetString(SID_NOISE_STRENGTH,                   "Strength");
    SetString(SID_NOISE_FLAGS,                      "Flags");
    SetString(SID_NOISE_COMPONENT_NAMES,            "All components|Red / Y'|Green / Cb|Blue / Cr|Alpha");
    SetString(SID_NOISE_FLAG_NAMES,                 "Averaged temporal|Mix random|Temporal|Uniform");
    SetString(SID_NOISE_SEED_ERROR,                 "The value of the seed is required");
    SetString(SID_NOISE_FLAGS_ERROR,                "No flags has been selected for the component");
    SetString(SID_NOISE_STRENGTH_ERROR,             "At least one strength value must be provided");


    //PerspectiveFilterPanel
    SetString(SID_PERSPECTIVE_USERFRIENDLY,         "%s=left:%s right:%s top:%s bottom:%s cubic:%s %s");
    SetString(SID_PERSPECTIVE_LEFT,                 "Left");
    SetString(SID_PERSPECTIVE_RIGHT,                "Right");
    SetString(SID_PERSPECTIVE_TOP,                  "Top");
    SetString(SID_PERSPECTIVE_BOTTOM,               "Bottom");
    SetString(SID_PERSPECTIVE_SQUEEZE,              "Squeeze");
    SetString(SID_PERSPECTIVE_STRETCH,              "Stretch");
    SetString(SID_PERSPECTIVE_INTERPOL_CUBIC,       "Use cubic interpolation (linear is default)");
    SetString(SID_PERSPECTIVE_ERROR,                "No change in perspective selected");


    //TransposeFilterPanel
    SetString(SID_TRANSPOSE_USERFRIENDLY,           "%s=%s flip:%s");
    SetString(SID_TRANSPOSE_DIRECTION,              "Direction");
    SetString(SID_TRANSPOSE_CLOCKWISE,              "Clockwise");
    SetString(SID_TRANSPOSE_COUNTER_CLOCKWISE,      "Counter-clockwise");
    SetString(SID_TRANSPOSE_FLIP,                   "Flip video");

    //DynaudnormFilterPanel
    SetString(SID_DYNAUDNORM_USERFRIENDLY,          "%s=%u gaussian:%u peak:%.2f gain:%.2f rms:%.2f compress:%.2f coupling:%s dc-bias:%s boundary:%s");
    SetString(SID_DYNAUDNORM_FRAME_LENGTH,          "Frame length");
    SetString(SID_DYNAUDNORM_GAUS_WND_SIZE,         "Gaussian window size");
    SetString(SID_DYNAUDNORM_TARGET_PEAK_VALUE,     "Target peak value");
    SetString(SID_DYNAUDNORM_MAX_GAIN_FACTOR,       "Maximum gain factor");
    SetString(SID_DYNAUDNORM_TARGET_RMS,            "Target RMS");
    SetString(SID_DYNAUDNORM_CHANNEL_COUPLING,      "Enable channel coupling");
    SetString(SID_DYNAUDNORM_DC_BIAS_CORRECT,       "DC bias correction");
    SetString(SID_DYNAUDNORM_BOUNDARY_MODE,         "Enable boundary mode");
    SetString(SID_DYNAUDNORM_COMPRESS_FACTOR,       "Compress factor");

    //DeflickerFilterPanel
    SetString(SID_DEFLICKER_USERFRIENDLY,           "%s=%u mode:%s");
    SetString(SID_DEFLICKER_SAMPLE_COUNT,           "Filter size (frames)");
    SetString(SID_DEFLICKER_AVG_MODE,               "Averaging mode");
    SetString(SID_DEFLICKER_AVG_MODES,              "Arithmetic mean|Geometric mean|Harmonic mean|Quadratic mean|Cubic mean|Power mean|Median");

    //SetString(, "");

    //Filter display names
    SetString(SID_FILTER_NAME_BASE + ftCOLOROVERLAY,        "Color overlay");
    SetString(SID_FILTER_NAME_BASE + ftDEINTERLACE,         "Deinterlace");
    SetString(SID_FILTER_NAME_BASE + ftCROP,                "Crop");
    SetString(SID_FILTER_NAME_BASE + ftSCALE,               "Scale");
    SetString(SID_FILTER_NAME_BASE + ftSUBSBURNIN,          "Subtitle burn-in");
    SetString(SID_FILTER_NAME_BASE + ftDELOGO,              "Remove logo");
    SetString(SID_FILTER_NAME_BASE + ftWATERMARK,           "Watermark");
    SetString(SID_FILTER_NAME_BASE + ftFLIP,                "Flip / Mirror");
    SetString(SID_FILTER_NAME_BASE + ftDEJUDDER,            "Dejudder");
    SetString(SID_FILTER_NAME_BASE + ftDESHAKE,             "Deshake");
    SetString(SID_FILTER_NAME_BASE + ftBOXBLUR,             "Box blur");
    SetString(SID_FILTER_NAME_BASE + ftCOLORBALANCE,        "Color balance");
    SetString(SID_FILTER_NAME_BASE + ftPAD,                 "Pad to size");
    SetString(SID_FILTER_NAME_BASE + ftROTATE,              "Rotate");
    SetString(SID_FILTER_NAME_BASE + ftUNSHARP,             "Sharpen");
    SetString(SID_FILTER_NAME_BASE + ftVIGNETTE,            "Vignette");
    SetString(SID_FILTER_NAME_BASE + ftVOLUME_AND_TONE,     "Volume and Tone");
    SetString(SID_FILTER_NAME_BASE + ftECHO,                "Echo");
    SetString(SID_FILTER_NAME_BASE + ftPHASER,              "Phaser");
    SetString(SID_FILTER_NAME_BASE + ftFLANGER,             "Flanger");
    SetString(SID_FILTER_NAME_BASE + ftHIGHPASS,            "Highpass");
    SetString(SID_FILTER_NAME_BASE + ftLOWPASS,             "Lowpass");
    SetString(SID_FILTER_NAME_BASE + ftBANDPASS,            "Bandpass");
    SetString(SID_FILTER_NAME_BASE + ftBANDREJECT,          "Bandreject");
    SetString(SID_FILTER_NAME_BASE + ftEQUALIZER,           "Equalizer");
    SetString(SID_FILTER_NAME_BASE + ftCOLORMATRIX,         "Color Matrix");
    SetString(SID_FILTER_NAME_BASE + ftCURVES,              "Curve adjust (color)");
    SetString(SID_FILTER_NAME_BASE + ftCHANNELMAP,          "Channel remapping");
    SetString(SID_FILTER_NAME_BASE + ftCUSTOM,              "Custom filter");
    SetString(SID_FILTER_NAME_BASE + ftRESAMPLE,            "Resample");
    SetString(SID_FILTER_NAME_BASE + ftEARWAX,              "Headphonize");
    SetString(SID_FILTER_NAME_BASE + ftNEGATE,              "Invert colors");
    SetString(SID_FILTER_NAME_BASE + ftELBG,                "Posterize");
    SetString(SID_FILTER_NAME_BASE + ftADELAY,              "Delay channels");
    SetString(SID_FILTER_NAME_BASE + ftATEMPO,              "Change tempo");
    SetString(SID_FILTER_NAME_BASE + ftDECIMATE,            "Drop duplicated frames");
    SetString(SID_FILTER_NAME_BASE + ftINTERLACE,           "Interlace");
    SetString(SID_FILTER_NAME_BASE + ftFRAMESTEP,           "Frame stepping");
    SetString(SID_FILTER_NAME_BASE + ftHQDN3D,              "HQ Denoise 3D");
    SetString(SID_FILTER_NAME_BASE + ftHUE,                 "Hue/Saturation/Brightness");
    SetString(SID_FILTER_NAME_BASE + ftLENSCORRECTION,      "Lens correction");
    SetString(SID_FILTER_NAME_BASE + ftNOISE,               "Noise");
    SetString(SID_FILTER_NAME_BASE + ftSWAPUV,              "Swap U and V plane");
    SetString(SID_FILTER_NAME_BASE + ftPERSPECTIVE,         "Perspective");
    SetString(SID_FILTER_NAME_BASE + ftTRANSPOSE,           "Transpose");
    SetString(SID_FILTER_NAME_BASE + ftDYNAUDNORM,          "Normalization");
    SetString(SID_FILTER_NAME_BASE + ftDEFLICKER,           "Remove flicker");


    m_Description = "FFQueue integrated English by Torben Bruchhaus";
    m_Temp = "";
    m_DateTimeFmt = "";
    m_BadStrID = "";
    m_SkipCount = 0;
    m_LoadName = "";
    memcpy(&m_PasswordHash, &NO_PASSWORD_HASH, sizeof(NO_PASSWORD_HASH));
    QUEUE_STATUS_NAMES = NULL;
    FILTER_NAMES = NULL;
    TIME_VALUE_NAMES = NULL;

    if (loadFile) LoadLanguage();
    else InitStringVars();

}

//---------------------------------------------------------------------------------------

FFQLang::~FFQLang()
{

    //Destructor - release all memory used
    for (wxVector<LPFFQ_STRING>::const_iterator ite = m_Strings->begin(); ite != m_Strings->end(); ite++) delete ((LPFFQ_STRING)*ite);
    delete m_Strings;
    m_Strings = NULL;

}

//---------------------------------------------------------------------------------------

void FFQLang::LibAVify()
{

    //This method replaces any instances of "ffmpeg", "ffprobe" and "ffplay"
    //with its equivalent "avconv", "avprobe" and "avplay"

    for (wxVector<LPFFQ_STRING>::const_iterator ite = m_Strings->begin(); ite != m_Strings->end(); ite++)
    {

        LibAVify((*ite)->str);

    }

}

//---------------------------------------------------------------------------------------

void FFQLang::LibAVify(wxString &str)
{

    //It's messy but it works!

    str.Replace("FFMpeg", "AVConv");
    str.Replace("FFmpeg", "AVconv");
    str.Replace("ffmpeg", "AVconv");

    str.Replace("FFProbe", "AVProbe");
    str.Replace("FFprobe", "AVprobe");
    str.Replace("ffprobe", "avprobe");

    str.Replace("FFPlay", "AVPlay");
    str.Replace("FFplay", "AVplay");
    str.Replace("ffplay", "avplay");

    str.Replace("FF-bin", "AV-bin");

}

//---------------------------------------------------------------------------------------

void FFQLang::ChangePassword(wxString pwd)
{

    //Change the password to the language file
    if (pwd.Len() == 0) memcpy(&m_PasswordHash, &NO_PASSWORD_HASH, sizeof(NO_PASSWORD_HASH));
    else HashString(pwd, m_PasswordHash);

}

//---------------------------------------------------------------------------------------

bool FFQLang::CheckPassword(wxString pwd)
{

    //Check the password of the language file

    if (HasPassword())
    {

        //Since the password is stored as a hash representation
        //we must hash the password given
        STR_HASH h;
        HashString(pwd, h);

        //And compare it to the one used in the language file
        return CMPHASH(&h, &m_PasswordHash);

    }

    //No password - return OK
    return true;
}

//---------------------------------------------------------------------------------------

LPFFQ_STRING FFQLang::FindString(FFQ_SID sid)
{

    //Find the string with the corresponding string id

    for (wxVector<LPFFQ_STRING>::const_iterator ite = m_Strings->begin(); ite != m_Strings->end(); ite++)
    {
        LPFFQ_STRING ffqs = *ite;
        if (ffqs->sid == sid) return ffqs; //Found - return it
    }

    //Not found - return NULL
    return NULL;

}

//---------------------------------------------------------------------------------------

const wxString& FFQLang::GetAtIndex(unsigned int index)
{

    //Return the string at the specified index
    return GetPtrAtIndex(index)->str;

}

//---------------------------------------------------------------------------------------

LPFFQ_STRING FFQLang::GetPtrAtIndex(unsigned int index)
{

    //Return pointer to the FFQ_STRING at the specified index

    if (index < GetCount())
    {
        return *((wxVector<LPFFQ_STRING>::const_iterator) m_Strings->begin() + index);
    }

    //String index out of bounds, throw an exception
    ThrowError(m_BadStrID);

    //Satisfy compiler
    return NULL;

}

//---------------------------------------------------------------------------------------

const unsigned int FFQLang::GetCount()
{

    //Return the number of strings in the vector
    return m_Strings->size();

}

//---------------------------------------------------------------------------------------

const wxString& FFQLang::GetDescription()
{

    //Get description of language file
    return m_Description;

}

//---------------------------------------------------------------------------------------

const unsigned int FFQLang::GetFlagCount(STR_FLAG flag)
{

    //Count the number of FFQ_STRINGS whose flag matches the flag given as argument
    //This is used to find out how many strings are translated, untranslated and modified

    unsigned int res = 0;
    for (wxVector<LPFFQ_STRING>::const_iterator ite = m_Strings->begin(); ite != m_Strings->end(); ite++)
    {
        //Test for match and increment as needed
        if (((*ite)->flags & flag) == flag) res++;
    }

    //Return the result
    return res;

}

//---------------------------------------------------------------------------------------

const wxString& FFQLang::GetLoadedFileName()
{
    return m_LoadName;
}

//---------------------------------------------------------------------------------------

const unsigned int FFQLang::GetNumberOfSkippedStringsInFile()
{

    //Returns the number of skipped strings which is the number
    //of strings where the original, untranslated string has been
    //accepted as translation

    return m_SkipCount;

}

//---------------------------------------------------------------------------------------

wxString* FFQLang::GetStringArray(FFQ_SID sid, unsigned int requiredSize, wxUniChar separator)
{

    //Converts a string to an array of strings

    wxString src = GetString(sid), //Get the string to convert
             *res = new wxString[requiredSize]; //Make an array for the items

    unsigned int idx = 0;
    while ((src.Len() > 0) && (idx < requiredSize))
    {

        //Get one token using the separator (usually "|")
        res[idx] = GetToken(src, separator);
        idx++;

    }

    //If the index matches the required size we're green
    if ((idx == requiredSize) && (src.Len() == 0)) return res;

    //We're not green, release the array
    delete[] res;

    //Throw exception
    ThrowError(FFQSF(SID_INVALID_STRING_ARRAY_SIZE, sid, requiredSize));

    //Satisfy compiler
    return NULL;

}

//---------------------------------------------------------------------------------------

const wxString& FFQLang::GetString(FFQ_SID sid)
{

    //Return the string with the given string id
    LPFFQ_STRING ffqs = FindString(sid);

    if (ffqs == NULL)
    {

        //The string was not found, show message while debugging and crash otherwise

        #ifdef DEBUG

            ShowError(m_BadStrID + " " + ToStr(sid));
            m_Temp.Clear();
            return m_Temp;

        #else

            ThrowError(m_BadStrID);

        #endif // DEBUG

    }

    //All good - return the string
    return ffqs->str;

}

//---------------------------------------------------------------------------------------

const wxString& FFQLang::GetDateTimeString()
{

    //Return the current date and time as a formatted string
    //using the format string from the language. Please note
    //that we are returning a variable and therefore it is
    //required to first store it to the temp string before
    //returning it

    m_Temp = wxDateTime::Now().Format(m_DateTimeFmt);
    return m_Temp;

}

//---------------------------------------------------------------------------------------

bool FFQLang::HasPassword()
{

    //Checks if a password is present by comparing the password hash
    //with the NO_PASSWORD_HASH. Password protection of the language
    //file was made to protect the translator from being discredited

    if (CMPHASH(&m_PasswordHash, &NO_PASSWORD_HASH)) return false;
    return true;

}

//---------------------------------------------------------------------------------------

bool FFQLang::LoadLanguage()
{

    //Try ConfigPath with lower-case
    m_LoadName = FFQCFG()->GetConfigPath(FFQCFG()->app_name.Lower() + DEFAULT_LANGUAGE_EXT);
    if (!wxFileExists(m_LoadName))
    {

        //Try ConfigPath with camel-case
        m_LoadName = FFQCFG()->GetConfigPath(FFQCFG()->app_name + DEFAULT_LANGUAGE_EXT);
        if (!wxFileExists(m_LoadName))
        {

            //Try the shared data directory, if available
            m_LoadName = FFQCFG()->share_data_dir;
            if (m_LoadName.Len() > 0)
            {

                //Try user selected locale
                if ((FFQCFG()->user_locale.Len() > 0) && wxFileExists(m_LoadName + FFQCFG()->user_locale))
                    m_LoadName += FFQCFG()->user_locale;

                //Try system locale
                else if (wxFileExists(m_LoadName + wxLocale::GetLanguageCanonicalName(wxLocale::GetSystemLanguage())))
                    m_LoadName += wxLocale::GetLanguageCanonicalName(wxLocale::GetSystemLanguage());

                //Nope, nothing available
                else m_LoadName.Clear();

            }

        }

    }

    m_SkipCount = 0;
    bool res = false;

    if (m_LoadName.Len() > 0) try
    {

        //Open the file
        wxFile *file = new wxFile(m_LoadName, wxFile::read);

        //Create a buffer
        uint8_t *buffer = new uint8_t[BUFFER_SIZE];

        //Check header
        file->Read((void*)buffer, sizeof(LANG_FILE_HEADER_10));
        if (memcmp(buffer, &LANG_FILE_HEADER_10, sizeof(LANG_FILE_HEADER_10)) != 0)
        {
            delete[] buffer;
            ThrowError("Invalid language file");
        }

        //Load garbage data and extract password hash
        file->Read((void*)buffer, 1024);
        unsigned int offs = 20 + (buffer[0] + buffer[10] + buffer[20]) % 200;
        for (unsigned int i = 0; i < sizeof(m_PasswordHash); i++) m_PasswordHash[i] = buffer[offs+(i*39)];

        //Load description
        LoadString(file, m_Description, buffer, m_PasswordHash);

        //Load number of strings in file
        uint32_t cnt;
        file->Read(&cnt, sizeof(cnt));

        //Handy variables
        LPFFQ_STRING ffqs, dummy = new FFQ_STRING(0, "");
        FFQ_SID sid;

        for (uint32_t i = 0; i < cnt; i++)
        {
            //Read String ID
            file->Read(&sid, sizeof(sid));

            //Get pointer to string
            ffqs = FindString(sid);
            if (ffqs == NULL)
            {
                //If the string is no longer used - load into dummy and increment skip-counter
                ffqs = dummy;
                m_SkipCount++;
            }

            //Load flags
            file->Read(&ffqs->flags, sizeof(ffqs->flags));

            //Load hash of last, internal string
            file->Read((void*)&ffqs->org_hash, sizeof(ffqs->org_hash));

            //Set the modified flag if hash has changed
            if (!CMPHASH(&ffqs->str_hash, &ffqs->org_hash)) ffqs->flags |= SF_MODIFIED;

            //If translation is available, load it
            if ((ffqs->flags & SF_TRANSLATED) != 0) LoadString(file, ffqs->str, buffer, m_PasswordHash);

        }

        //Release memory
        delete dummy;
        delete[] buffer;

        //Close and release file
        file->Close();
        delete file;

        //Set result
        res = true;

    }
    catch (std::exception &err1)
    {

        #ifdef DEBUG
        ShowError(err1.what());
        #endif // DEBUG

    }

    //Init common vars
    InitStringVars();

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

bool FFQLang::SaveLanguage()
{

    //Save the language to a file

    bool res = false;

    try
    {

        //Language is always stored in the same location as config files:
        wxString path = FFQCFG()->GetConfigPath(FFQCFG()->app_name.Lower() + DEFAULT_LANGUAGE_EXT);

        //Delete existing file and create new
        if (wxFileExists(path)) wxRemoveFile(path);

        //Get the number of stored strings
        uint32_t cnt = GetFlagCount(SF_STORED);

        //If there are no stored strings, we are finished
        if (cnt == 0) return true;

        wxFile *file = new wxFile(path, wxFile::write);

        //Write header
        file->Write((void*)&LANG_FILE_HEADER_10, sizeof(LANG_FILE_HEADER_10));

        //Create a buffer
        uint8_t *buffer = new uint8_t[BUFFER_SIZE];

        //Make garbage data that includes password hash and save it
        for (unsigned int i = 0; i < 1024; i++) buffer[i] = (rand() % 0xFF);
        unsigned int offs = 20 + (buffer[0] + buffer[10] + buffer[20]) % 200;
        for (unsigned int i = 0; i < sizeof(m_PasswordHash); i++) buffer[offs+(i*39)] = m_PasswordHash[i];
        file->Write((void*)buffer, 1024);

        //Save description
        SaveString(file, m_Description, buffer, m_PasswordHash);

        //Save number of strings in file
        file->Write(&cnt, sizeof(cnt));

        //Store all translated strings
        for (wxVector<LPFFQ_STRING>::const_iterator ite = m_Strings->begin(); ite != m_Strings->end(); ite++)
        {

            LPFFQ_STRING ffqs = *ite;

            if ((ffqs->flags & SF_STORED) != 0)
            {

                //Save String ID
                file->Write((void*)&ffqs->sid, sizeof(ffqs->sid));

                //Flags
                file->Write((void*)&ffqs->flags, sizeof(ffqs->flags));

                //Hash of original string value
                file->Write((void*)&ffqs->org_hash, sizeof(ffqs->org_hash));

                //If translated - save the translated string
                if ((ffqs->flags & SF_TRANSLATED) != 0) SaveString(file, ffqs->str, buffer, m_PasswordHash);

            }

        }

        //Release the buffer
        delete[] buffer;

        //Flush file and close / release it
        file->Flush();
        file->Close();
        delete file;

        //Set result
        res = true;

    } catch (std::exception &err1)
    {

        #ifdef DEBUG
        ShowError(err1.what());
        #endif // DEBUG

    }

    return res;

}

//---------------------------------------------------------------------------------------

void FFQLang::SetDescription(wxString &desc)
{

    //Get description of language
    m_Description = desc;

}

//---------------------------------------------------------------------------------------

void FFQLang::SetString(FFQ_SID sid, wxString str)
{

    //Replace everything that does not have to be formatted dynamically
    str.Replace("<br>", CRLF);

    //Find the string by its ID
    LPFFQ_STRING ffqs = FindString(sid);

    if (ffqs == NULL)
    {

        //Not found = create new
        LPFFQ_STRING ffqs = new FFQ_STRING(sid, str);

        //Hash the string and set the file hash to the current hash
        HashString(ffqs->str, ffqs->str_hash);
        memcpy(&ffqs->org_hash, &ffqs->str_hash, sizeof(ffqs->str_hash));

        //Append to vector and exit
        m_Strings->push_back(ffqs);
        return;

    }

    //Found = update string
    ffqs->str = str;
    ffqs->str.Shrink();

}

//---------------------------------------------------------------------------------------

void FFQLang::InitStringVars()
{

    //Initializes common string variables from the language

    //Validate first
    ValidateStrings();

    //Handy variable
    LPFFQ_STRING ffqs;

    //Initialize queue status names
    if (QUEUE_STATUS_NAMES)
    {
        //Release if already initialized
        delete[] QUEUE_STATUS_NAMES;
        QUEUE_STATUS_NAMES = NULL;
    }

    //Get from string array
    QUEUE_STATUS_NAMES = GetStringArray(SID_MAINFRAME_QUEUE_STATUS_NAMES, QUEUE_STATUS_COUNT);

    //Initialize time value format names
    if (TIME_VALUE_NAMES)
    {
        //Release if already initialized
        delete[] TIME_VALUE_NAMES;
        TIME_VALUE_NAMES = NULL;
    }

    //Get from string array
    TIME_VALUE_NAMES = GetStringArray(SID_TIME_VALUE_NAMES, TIME_VALUE_COUNT);

    //Initialize filter names
    if (FILTER_NAMES) delete[] FILTER_NAMES; //Release existing array

    //Make new filter name array
    FILTER_NAMES = new wxString[FILTER_COUNT];
    for (unsigned int i = 0; i < FILTER_COUNT; i++)
    {
        ffqs = FindString(SID_FILTER_NAME_BASE + i);
        FILTER_NAMES[i] = (ffqs == NULL) ? "???" : ffqs->str;
    }

    //Initialize other variables
    m_Temp = ""; //Set temp to empty
    m_DateTimeFmt = GetString(SID_DATE_TIME_FORMAT); //Date / time format string
    m_BadStrID = GetString(SID_BAD_STRING_ID); //Error message for bad string id

}

//---------------------------------------------------------------------------------------

void FFQLang::ValidateStrings()
{

    //This method is used to do search and replace in the language
    //strings. Originally %I64d was used for 64bit integers but that
    //formatter is not portable, so..

    //As of V1.7.52 %I64d is used on Windows and %lld is used on
    //everything else. Since language file may be saved with either
    //one of the formatters, we must replace both ways..
    #ifdef __WINDOWS__
      #define YEA_FMT "%I64d"
      #define NEI_FMT "%lld"
    #else
      #define YEA_FMT "%lld"
      #define NEI_FMT "%I64d"
    #endif // __WINDOWS__

    LPFFQ_STRING ffqs = FindString(SID_LOG_COMPLETED_OK);
    if (ffqs != NULL) ffqs->str.Replace(NEI_FMT, YEA_FMT);

    ffqs = FindString(SID_LOG_CLIPPING_WARNING);
    if (ffqs != NULL) ffqs->str.Replace(NEI_FMT, YEA_FMT);

}
