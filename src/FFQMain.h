/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQMain.h                                                       *
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

#ifndef FFQMAIN_H
#define FFQMAIN_H

//#include "wx_pch.h"

//(*Headers(FFQMain)
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

#include "FFQAbout.h"
#include "FFQJobEditAdv.h"
#include "FFQPresetEdit.h"
#include "FFQConfigEdit.h"
#include "FFQBatchMake.h"
#include "tools/FFQThumbMake.h"
#include "tools/FFQConcat.h"
#include "tools/FFQVidStab.h"
#include "tools/FFQVideoToGIF.h"
#include "utils/FFQConsole.h"
#include "utils/FFQProcess.h"
#include "utils/FFQQueueItem.h"

const int TIMER_INTERVAL = 10;

const unsigned int LOG_COLOR_COUNT = 6;
const unsigned int LOG_COLORS[LOG_COLOR_COUNT] = { COLOR_GRAY, COLOR_BLUE, COLOR_RED, COLOR_BLACK, COLOR_GREEN, COLOR_ORANGE };

typedef enum DEFINE_SELECT {dsNOSELECT, dsUNIQUE, dsAPPEND} DEFINE_SELECT;

void PtrToBitmap(void* ptr, unsigned int len, wxBitmap &bmp, wxBitmapType type = wxBITMAP_TYPE_PNG, wxSize fit = wxSize(-1,-1));

typedef struct ENCODING_SLOT
{
    int index;                 //The index of the slot in the array
    FFQProcess process;        //Encoding process
    FFQConsole console;        //Console for logging
    wxGauge* gauge;            //Gauge for status
    LPFFQ_QUEUE_ITEM item;     //Item being encoded
    wxString command;          //The currently running command
    TIME_VALUE duration;       //The expected duration of the encoding
    uint64_t started;          //The tick count when the encoding was started
    long frames;               //The expected number of frames to be encoded
    double pct_done;           //The last percent done being reported

    ENCODING_SLOT() : process(), console(), gauge(0), item(0) {}// { process = FFQProcess(); console = FFQConsole(); gauge = 0; item = 0; }

} ENCODING_SLOT, *LPENCODING_SLOT;

class FFQMain: public wxFrame
{
    public:

        static FFQMain* GetInstance() { return m_Instance; }

        FFQMain(wxWindow* parent, wxWindowID id = -1);
        virtual ~FFQMain();

        //Public item handling
        long FindItemForFile(wxString file_name, long start_index = 0, bool output_file = false);
        LPFFQ_QUEUE_ITEM GetItemAtIndex(long index);
        LPFFQ_QUEUE_ITEM GetSelectedItem(long *index = NULL);
        long IndexOfItem(LPFFQ_QUEUE_ITEM item);

        //Public preset handling
        bool IsPresetActive(LPFFQ_PRESET pst, bool *can_modify = NULL);
        void PresetChanged(LPFFQ_PRESET pst);

        //Public access to previewing commands
        bool PreviewCommand(wxString cmd, bool add_to_console);

        //Public access to select a console
        void SelectConsole(FFQConsole* by_pointer, int by_index = -1);

    private:

        #ifdef DEBUG
        static const long ID_DEBUG_MAKEFILES;
        static const long ID_DEBUG_BINRES;
        static const long ID_DEBUG_TESTING;
        static const long ID_DEBUG_FILTERS;
        static const long ID_DEBUG_ABOUT;
        wxMenu *m_DebugPopupMenu;
        #endif

        static FFQMain* m_Instance;

        wxImageList *m_ListIcons, *m_ConsoleIcons;
        wxBitmap *m_ToolBitmaps;
        wxArrayPtrVoid *m_PtrList;
        FFQAbout* AboutBox; //About box - created when required
        FFQBatchMake* BatchMaker; //Used to batch-make jobs
        FFQThumbMake* ThumbMaker;
        FFQJobEditAdv* JobEdit; //The new job editor - created when used
        FFQConfigEdit* OptionsDlg; //Options editor - created when needed
        FFQConcat* ConcatDlg;
        FFQVidStab* VidStabDlg;
        FFQVideoToGIF* Video2Gif;
        FFQConsole* Console;
        wxString m_JobsFileName;
        wxRect m_RestoredRect;
        float m_LastColumnPct;
        int m_StatusColumnWidth; //Width of the status column, calculated with a screen DC
        bool m_AllowEvents; //Used to avoid event-filtering for programmatically triggered events
        bool m_FirstIdle; //True if OnIdle is entered for the first time
        bool m_FirstShow; //True when frame receives focus for the first time
        bool m_Closed; //Has the frame been closed?

        ENCODING_SLOT* m_EncodingSlots;
        int m_NumEncodingSlots;
        bool m_EncodingActive;
        int m_EncodingAborted;
        int m_EncodingFailed;
        int m_EncodingSuccess;
        uint64_t m_QueueStarted;

        //Handlers for encoding slots / consoles
        int  FindEncodingSlot(LPFFQ_QUEUE_ITEM item);
        void InitEncodingSlots();

        //Item processing
        void AfterItemProcessing(LPENCODING_SLOT slot); //Called when an item has been processed
        bool BeforeItemProcessing(LPENCODING_SLOT slot); //Called when an item is about to be processed
        void DeleteProcessedItems(); //Delete the jobs marked as qsDONE after queue has been processed
        LPFFQ_QUEUE_ITEM FindNextItemToProcess(LPFFQ_QUEUE_ITEM from_item); //Return the next queued item in the list
        void LogItemStatus(LPENCODING_SLOT slot, bool first_command); //Log the status of the item
        bool MustBeQueued(LPFFQ_QUEUE_ITEM item, long item_index, bool selected_only); //Checks if the items status allows queuing
        bool OverwritePrompt(bool selected_only); //Asks if the user wants to overwrite existing files before starting the queue
        bool ProcessNext(LPENCODING_SLOT slot); //Process the next item/command
        void ProcessReadOutput(); //Reads output from the current job

        //Queue handling
        void FinishQueue(); //Called when the queue finishes or is aborted
        void StartQueue(bool selected_only); //Starts processing of the requested items
        void StopQueue(bool selected_only); //Stops processing of the requested items


        //Item handling in the list
        void DefineItem(long index, LPFFQ_QUEUE_ITEM item, DEFINE_SELECT select, bool save_if_required); //Uused to modify items in the ListView
        void DeleteItem(long index); //Delete job and attached data
        void EditJob(long index, wxString forFileName = "", LPFFQ_QUEUE_ITEM clone = NULL); //Edit the job at idx
        void LoadItems(); //Load previously stored items
        void SaveItems(bool prompt = false); //Save items in queue
        void ValidateItems(); //Validate all items in the list for missing file(s)
        void MoveItems(bool up); //Used to move items up or down in the list
        void SwapItems(int a, int b); //Used to swap 2 items


        //UI related stuff
        void ResizeColumns(bool dragging = false); //Resizes the columns in the ListView when size of frame changes
        void UpdateControls(); //Used to enable/disable buttons according to the current state of query and listview
        void UpdateProgress(LPENCODING_SLOT slot, unsigned int pos); //Used to update progress in gauge and task bar
        void UpdateStatus(); //Used to update statusbar and current job progress

        //Window position get and set
        wxString GetWindowPos();
        void SetWindowPos(wxString &wp);

        //FFmpeg handling
        void ShowFFMpegVersion(bool langInfo); //Displays FFMpeg version in the TextCtrl used as console
        void ShowFFProbeInfo(LPFFQ_QUEUE_ITEM item = NULL); //Display ffprobe info for an item

        //Tools and such
        void BatchMakeJobs(wxArrayString *files, bool releaseFilesPtr);
        bool LaunchTool(short ToolID, long edit_index = -1, LPFFQ_QUEUE_ITEM edit_item = NULL); //Launches a tool to create commands

        //Event handlers
        void OnChar(wxKeyEvent &event);
        void OnDropFiles(wxDropFilesEvent& event);
        void OnIdle(wxIdleEvent &event);
        void OnMaximize(wxMaximizeEvent &event);
        void OnMove(wxMoveEvent &event);
        void OnShow(wxShowEvent &event);

        //(*Handlers(FFQMain)
        void OnFrameResize(wxSizeEvent& event);
        void OnListViewColumnBeginDrag(wxListEvent& event);
        void OnToolBarButtonClick(wxCommandEvent& event);
        void OnTimerTrigger(wxTimerEvent& event);
        void OnListViewItemSelection(wxListEvent& event);
        void OnListViewItemActivated(wxListEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnListViewItemRightClick(wxListEvent& event);
        void OnListViewColumnEndDrag(wxListEvent& event);
        void OnConsolesPageChanged(wxNotebookEvent& event);
        //*)

        //(*Identifiers(FFQMain)
        static const long ID_LISTVIEW;
        static const long ID_TEXTCTRL;
        static const long ID_PANEL1;
        static const long ID_CONSOLES;
        static const long ID_SPLITTERWINDOW;
        static const long ID_STATUSBAR;
        static const long ID_TIMER;
        static const long ID_MENU_MOVEUP;
        static const long ID_MENU_MOVEDOWN;
        static const long ID_MENU_EDIT;
        static const long ID_MENU_CLONE;
        static const long ID_MENU_PRESET;
        static const long ID_MENU_REMOVE;
        static const long ID_MENU_FFPROBE;
        static const long ID_MENU_FFCMD;
        static const long ID_MENU_STARTALL;
        static const long ID_MENU_STARTSEL;
        static const long ID_TOOLTHUMBS;
        static const long ID_TOOLSLIDESHOW;
        static const long ID_TOOLCONCAT;
        static const long ID_TOOLVIDSTAB;
        static const long ID_TOOLVID2GIF;
        //*)

        //(*Declarations(FFQMain)
        wxFileDialog* OpenFilesDlg;
        wxListView* ListView;
        wxMenu ListMenu;
        wxMenu ToolsMenu;
        wxMenuItem* ConcatItem;
        wxMenuItem* MenuClone;
        wxMenuItem* MenuEdit;
        wxMenuItem* MenuFFCmd;
        wxMenuItem* MenuFFProbe;
        wxMenuItem* MenuMoveDown;
        wxMenuItem* MenuMoveUp;
        wxMenuItem* MenuPreset;
        wxMenuItem* MenuRemove;
        wxMenuItem* MenuStartAll;
        wxMenuItem* MenuStartSel;
        wxMenuItem* SlideshowItem;
        wxMenuItem* ThumbsItem;
        wxMenuItem* Vid2GifItem;
        wxMenuItem* VidStabItem;
        wxNotebook* Consoles;
        wxSplitterWindow* SplitterWindow;
        wxStatusBar* StatusBar;
        wxTextCtrl* TextCtrl;
        wxTimer Timer;
        //*)

        static const long ID_TOOLBAR;
        static const long ID_TOOLBARADD;
        static const long ID_TOOLBARBATCH;
        static const long ID_TOOLBARREMOVE;
        static const long ID_TOOLBAREDIT;
        static const long ID_TOOLBARPREVIEW;
        static const long ID_TOOLBARSTART;
        static const long ID_TOOLBARSTOP;
        static const long ID_TOOLBARTOOLS;
        static const long ID_TOOLBARPRESETS;
        static const long ID_TOOLBAROPTIONS;
        static const long ID_TOOLBARABOUT;
        static const long ID_MENU_STOPSEL;

        wxToolBar* ToolBar;
        wxToolBarToolBase* ToolBarAbout;
        wxToolBarToolBase* ToolBarAdd;
        wxToolBarToolBase* ToolBarBatch;
        wxToolBarToolBase* ToolBarEdit;
        wxToolBarToolBase* ToolBarOptions;
        wxToolBarToolBase* ToolBarPresets;
        wxToolBarToolBase* ToolBarPreview;
        wxToolBarToolBase* ToolBarRemove;
        wxToolBarToolBase* ToolBarStart;
        wxToolBarToolBase* ToolBarStop;
        wxToolBarToolBase* ToolBarTools;

        wxMenuItem* MenuStopSel;
        wxMenuItem* MenuStopAll;

        DECLARE_EVENT_TABLE()
};

#endif // FFQMAIN_H
