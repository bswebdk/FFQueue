/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQMain.cpp                                                     *
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

#include "FFQMain.h"
#include "utils/FFQLang.h"
#include "FFQLangEdit.h"
#include "FFQPresetMgr.h"
#include "FFQFullSpec.h"
#include "utils/FFQConfig.h"
#include "utils/FFQConst.h"
#include "utils/FFQMisc.h"
#include "utils/FFQBuildCmd.h"
#include "utils/FFQStaticJob.h"
#include "utils/FFQToolJobs.h"
#include "bin_res.h"

#ifdef DEBUG
    #include "utils/FFQDebugUtils.h"
    #include "utils/FFQCodecInfo.h"
    #include "utils/FFQCompress.h"
    #include "utils/FFQParsing.h"
    #include <wx/cmdline.h>
    #include <wx/ffile.h>
    #include <wx/regex.h>
    #include <wx/dir.h>
    #include <zlib.h>
    //#include "utils/FFQNvpList.h"
    //#include "FFQFilterEdit.h"
#endif // DEBUG

#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/mstream.h>
//#include <wx/app.h>

#ifdef __WINDOWS__
    #include <windows.h>
#else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"
    #include "../res/MainIcon32.xpm"
    //It may be nescessary to uncomment the following lines if 16x16 and/or 64x64 icons are required
    //#include "res/MainIcon16.xpm"
    //#include "res/MainIcon64.xpm"
    #pragma GCC diagnostic pop
#endif // __WINDOWS__

//(*InternalHeaders(FFQMain)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(FFQMain)
const long FFQMain::ID_LISTVIEW = wxNewId();
const long FFQMain::ID_TEXTCTRL = wxNewId();
const long FFQMain::ID_PANEL1 = wxNewId();
const long FFQMain::ID_CONSOLES = wxNewId();
const long FFQMain::ID_SPLITTERWINDOW = wxNewId();
const long FFQMain::ID_STATUSBAR = wxNewId();
const long FFQMain::ID_TIMER = wxNewId();
const long FFQMain::ID_MENU_MOVEUP = wxNewId();
const long FFQMain::ID_MENU_MOVEDOWN = wxNewId();
const long FFQMain::ID_MENU_EDIT = wxNewId();
const long FFQMain::ID_MENU_CLONE = wxNewId();
const long FFQMain::ID_MENU_PRESET = wxNewId();
const long FFQMain::ID_MENU_REMOVE = wxNewId();
const long FFQMain::ID_MENU_FFPROBE = wxNewId();
const long FFQMain::ID_MENU_FFCMD = wxNewId();
const long FFQMain::ID_MENU_STARTALL = wxNewId();
const long FFQMain::ID_MENU_STARTSEL = wxNewId();
const long FFQMain::ID_TOOLTHUMBS = wxNewId();
const long FFQMain::ID_TOOLSLIDESHOW = wxNewId();
const long FFQMain::ID_TOOLCONCAT = wxNewId();
const long FFQMain::ID_TOOLVIDSTAB = wxNewId();
const long FFQMain::ID_TOOLVID2GIF = wxNewId();
//*)

#ifdef DEBUG
const long FFQMain::ID_DEBUG_MAKEFILES = wxNewId();
const long FFQMain::ID_DEBUG_BINRES = wxNewId();
const long FFQMain::ID_DEBUG_TESTING = wxNewId();
const long FFQMain::ID_DEBUG_ABOUT = wxNewId();
const long FFQMain::ID_DEBUG_FILTERS = wxNewId();
#endif

const long FFQMain::ID_TOOLBAR = wxNewId();
const long FFQMain::ID_TOOLBARADD = wxNewId();
const long FFQMain::ID_TOOLBARBATCH = wxNewId();
const long FFQMain::ID_TOOLBARREMOVE = wxNewId();
const long FFQMain::ID_TOOLBAREDIT = wxNewId();
const long FFQMain::ID_TOOLBARPREVIEW = wxNewId();
const long FFQMain::ID_TOOLBARSTART = wxNewId();
const long FFQMain::ID_TOOLBARSTOP = wxNewId();
const long FFQMain::ID_TOOLBARTOOLS = wxNewId();
const long FFQMain::ID_TOOLBARPRESETS = wxNewId();
const long FFQMain::ID_TOOLBAROPTIONS = wxNewId();
const long FFQMain::ID_TOOLBARABOUT = wxNewId();
const long FFQMain::ID_MENU_STOPSEL = wxNewId();

FFQMain* FFQMain::m_Instance = NULL;

BEGIN_EVENT_TABLE(FFQMain,wxFrame)
    //(*EventTable(FFQMain)
    //*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

const double GAUGE_MAX = 10000.0;

//---------------------------------------------------------------------------------------

void PtrToBitmap(void* ptr, unsigned int len, wxBitmap &bmp, wxBitmapType type, wxSize fit)
{
    wxMemoryInputStream *ms = new wxMemoryInputStream(ptr, len);
    wxImage img = wxImage(*ms, type);
    if ((fit.GetHeight() > 0) && (fit.GetWidth() > 0)) img = img.Scale(fit.GetWidth(), fit.GetHeight());
	bmp = wxBitmap(img);
	delete ms;
}

//---------------------------------------------------------------------------------------

#define GETQUEUEFLAG(flags, flag) (flags & flag) != 0
#define SETQUEUEFLAG(flags, flag, on) if (on) { flags |= flag; } else { flags &= ~flag; }

#define CONSOLE_IMAGE_MSG 0
#define CONSOLE_IMAGE_OFF 1
#define CONSOLE_IMAGE_ON 2

//---------------------------------------------------------------------------------------


FFQMain::FFQMain(wxWindow* parent, wxWindowID id)
{

    //Init cfg and str
    if (FFQCFG()->use_libav) FFQL()->LibAVify();

    //(*Initialize(FFQMain)
    wxFlexGridSizer* FlexGridSizer2;
    wxPanel* Panel1;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(800,500));
    SplitterWindow = new wxSplitterWindow(this, ID_SPLITTERWINDOW, wxPoint(184,256), wxDefaultSize, wxSP_3D|wxALWAYS_SHOW_SB, _T("ID_SPLITTERWINDOW"));
    SplitterWindow->SetMinSize(wxSize(100,100));
    SplitterWindow->SetSashGravity(0.5);
    ListView = new wxListView(SplitterWindow, ID_LISTVIEW, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_NO_SORT_HEADER|wxBORDER_NONE, wxDefaultValidator, _T("ID_LISTVIEW"));
    ListView->SetMinSize(wxDLG_UNIT(SplitterWindow,wxSize(800,150)));
    Consoles = new wxNotebook(SplitterWindow, ID_CONSOLES, wxDefaultPosition, wxDefaultSize, 0, _T("ID_CONSOLES"));
    Consoles->SetMinSize(wxSize(800,150));
    Panel1 = new wxPanel(Consoles, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    FlexGridSizer2 = new wxFlexGridSizer(1, 1, 0, 0);
    FlexGridSizer2->AddGrowableCol(0);
    FlexGridSizer2->AddGrowableRow(0);
    TextCtrl = new wxTextCtrl(Panel1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_DONTWRAP|wxBORDER_NONE, wxDefaultValidator, _T("ID_TEXTCTRL"));
    TextCtrl->SetMinSize(wxSize(800,100));
    wxFont TextCtrlFont(10,wxFONTFAMILY_TELETYPE,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Monospace"),wxFONTENCODING_DEFAULT);
    TextCtrl->SetFont(TextCtrlFont);
    FlexGridSizer2->Add(TextCtrl, 0, wxEXPAND, 5);
    Panel1->SetSizer(FlexGridSizer2);
    FlexGridSizer2->Fit(Panel1);
    FlexGridSizer2->SetSizeHints(Panel1);
    Consoles->AddPage(Panel1, _("Sys"), false);
    SplitterWindow->SplitHorizontally(ListView, Consoles);
    StatusBar = new wxStatusBar(this, ID_STATUSBAR, wxST_SIZEGRIP|wxBORDER_NONE, _T("ID_STATUSBAR"));
    int __wxStatusBarWidths_1[3] = { 1, 250, -300 };
    int __wxStatusBarStyles_1[3] = { wxSB_FLAT, wxSB_NORMAL, wxSB_NORMAL };
    StatusBar->SetFieldsCount(3,__wxStatusBarWidths_1);
    StatusBar->SetStatusStyles(3,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar);
    Timer.SetOwner(this, ID_TIMER);
    Timer.Start(10, false);
    MenuMoveUp = new wxMenuItem((&ListMenu), ID_MENU_MOVEUP, _("1"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuMoveUp);
    MenuMoveDown = new wxMenuItem((&ListMenu), ID_MENU_MOVEDOWN, _("2"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuMoveDown);
    ListMenu.AppendSeparator();
    MenuEdit = new wxMenuItem((&ListMenu), ID_MENU_EDIT, _("3"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuEdit);
    MenuClone = new wxMenuItem((&ListMenu), ID_MENU_CLONE, _("4"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuClone);
    MenuPreset = new wxMenuItem((&ListMenu), ID_MENU_PRESET, _("5"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuPreset);
    MenuRemove = new wxMenuItem((&ListMenu), ID_MENU_REMOVE, _("6"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuRemove);
    ListMenu.AppendSeparator();
    MenuFFProbe = new wxMenuItem((&ListMenu), ID_MENU_FFPROBE, _("7"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuFFProbe);
    MenuFFCmd = new wxMenuItem((&ListMenu), ID_MENU_FFCMD, _("8"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuFFCmd);
    ListMenu.AppendSeparator();
    MenuStartAll = new wxMenuItem((&ListMenu), ID_MENU_STARTALL, _("9"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuStartAll);
    MenuStartSel = new wxMenuItem((&ListMenu), ID_MENU_STARTSEL, _("10"), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuStartSel);
    OpenFilesDlg = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
    ThumbsItem = new wxMenuItem((&ToolsMenu), ID_TOOLTHUMBS, _("1"), wxEmptyString, wxITEM_NORMAL);
    ToolsMenu.Append(ThumbsItem);
    SlideshowItem = new wxMenuItem((&ToolsMenu), ID_TOOLSLIDESHOW, _("2"), wxEmptyString, wxITEM_NORMAL);
    ToolsMenu.Append(SlideshowItem);
    ConcatItem = new wxMenuItem((&ToolsMenu), ID_TOOLCONCAT, _("3"), wxEmptyString, wxITEM_NORMAL);
    ToolsMenu.Append(ConcatItem);
    VidStabItem = new wxMenuItem((&ToolsMenu), ID_TOOLVIDSTAB, _("4"), wxEmptyString, wxITEM_NORMAL);
    ToolsMenu.Append(VidStabItem);
    Vid2GifItem = new wxMenuItem((&ToolsMenu), ID_TOOLVID2GIF, _("5"), wxEmptyString, wxITEM_NORMAL);
    ToolsMenu.Append(Vid2GifItem);
    Center();

    Connect(ID_LISTVIEW,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&FFQMain::OnListViewItemSelection);
    Connect(ID_LISTVIEW,wxEVT_COMMAND_LIST_ITEM_DESELECTED,(wxObjectEventFunction)&FFQMain::OnListViewItemSelection);
    Connect(ID_LISTVIEW,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&FFQMain::OnListViewItemActivated);
    Connect(ID_LISTVIEW,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&FFQMain::OnListViewItemRightClick);
    Connect(ID_LISTVIEW,wxEVT_COMMAND_LIST_COL_BEGIN_DRAG,(wxObjectEventFunction)&FFQMain::OnListViewColumnBeginDrag);
    Connect(ID_LISTVIEW,wxEVT_COMMAND_LIST_COL_END_DRAG,(wxObjectEventFunction)&FFQMain::OnListViewColumnEndDrag);
    Connect(ID_CONSOLES,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&FFQMain::OnConsolesPageChanged);
    Connect(ID_TIMER,wxEVT_TIMER,(wxObjectEventFunction)&FFQMain::OnTimerTrigger);
    Connect(ID_MENU_MOVEUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_MOVEDOWN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_EDIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_CLONE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_PRESET,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_REMOVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_FFPROBE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_FFCMD,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_STARTALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_MENU_STARTSEL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_TOOLTHUMBS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_TOOLSLIDESHOW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_TOOLCONCAT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_TOOLVIDSTAB,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(ID_TOOLVID2GIF,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&FFQMain::OnClose);
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&FFQMain::OnFrameResize);
    //*)

    //SetClientSize(wxDLG_UNIT(this, wxSize(800, 600)));

    m_ToolBitmaps = new wxBitmap[11];
    PtrToBitmap((void*)&TOOL_ADD, TOOL_ADD_SIZE, m_ToolBitmaps[0]);
    PtrToBitmap((void*)&TOOL_DELETE, TOOL_DELETE_SIZE, m_ToolBitmaps[1]);
    PtrToBitmap((void*)&TOOL_EDIT, TOOL_EDIT_SIZE, m_ToolBitmaps[2]);
    PtrToBitmap((void*)&TOOL_START, TOOL_START_SIZE, m_ToolBitmaps[3]);
    PtrToBitmap((void*)&TOOL_STOP, TOOL_STOP_SIZE, m_ToolBitmaps[4]);
    PtrToBitmap((void*)&TOOL_CONFIG, TOOL_CONFIG_SIZE, m_ToolBitmaps[5]);
    PtrToBitmap((void*)&TOOL_ABOUT, TOOL_ABOUT_SIZE, m_ToolBitmaps[6]);
    PtrToBitmap((void*)&TOOL_BATCH, TOOL_BATCH_SIZE, m_ToolBitmaps[7]);
    PtrToBitmap((void*)&TOOL_TOOLS, TOOL_TOOLS_SIZE, m_ToolBitmaps[8]);
    PtrToBitmap((void*)&TOOL_PREVIEW, TOOL_PREVIEW_SIZE, m_ToolBitmaps[9]);
    PtrToBitmap((void*)&TOOL_PRESETS, TOOL_PRESETS_SIZE, m_ToolBitmaps[10]);

    ToolBar = new wxToolBar(this, ID_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_TEXT|wxTB_NODIVIDER|wxTB_TOP, wxEmptyString);
    ToolBarAdd = ToolBar->AddTool(ID_TOOLBARADD, FFQS(SID_COMMON_ADD), m_ToolBitmaps[0], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarBatch = ToolBar->AddTool(ID_TOOLBARBATCH, FFQS(SID_MAINFRAME_TB_BATCH), m_ToolBitmaps[7], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarRemove = ToolBar->AddTool(ID_TOOLBARREMOVE, FFQS(SID_COMMON_REMOVE), m_ToolBitmaps[1], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarEdit = ToolBar->AddTool(ID_TOOLBAREDIT, FFQS(SID_COMMON_EDIT), m_ToolBitmaps[2], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarPreview = ToolBar->AddTool(ID_TOOLBARPREVIEW, FFQS(SID_COMMON_PREVIEW), m_ToolBitmaps[9], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarStart = ToolBar->AddTool(ID_TOOLBARSTART, FFQS(SID_MAINFRAME_TB_START), m_ToolBitmaps[3], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarStop = ToolBar->AddTool(ID_TOOLBARSTOP, FFQS(SID_MAINFRAME_TB_STOP), m_ToolBitmaps[4], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarTools = ToolBar->AddTool(ID_TOOLBARTOOLS, FFQS(SID_MAINFRAME_TB_TOOLS), m_ToolBitmaps[8], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarPresets = ToolBar->AddTool(ID_TOOLBARPRESETS, FFQS(SID_MAINFRAME_TB_PRESETS), m_ToolBitmaps[10], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarOptions = ToolBar->AddTool(ID_TOOLBAROPTIONS, FFQS(SID_MAINFRAME_TB_OPTIONS), m_ToolBitmaps[5], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarAbout = ToolBar->AddTool(ID_TOOLBARABOUT, FFQS(SID_MAINFRAME_TB_ABOUT), m_ToolBitmaps[6], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->Realize();
    SetToolBar(ToolBar);
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &FFQMain::OnToolBarButtonClick, this);

    #ifdef DEBUG
    m_DebugPopupMenu = new wxMenu();
    m_DebugPopupMenu->Append(new wxMenuItem(m_DebugPopupMenu, ID_DEBUG_TESTING, "Testing"));
    m_DebugPopupMenu->Append(new wxMenuItem(m_DebugPopupMenu, ID_DEBUG_FILTERS, "Filters"));
    m_DebugPopupMenu->Append(new wxMenuItem(m_DebugPopupMenu, ID_DEBUG_ABOUT, "About"));
    m_DebugPopupMenu->Append(new wxMenuItem(m_DebugPopupMenu, ID_DEBUG_MAKEFILES, "Create makefiles"));
    m_DebugPopupMenu->Append(new wxMenuItem(m_DebugPopupMenu, ID_DEBUG_BINRES, "Create binary resources"));
    Bind(wxEVT_COMMAND_MENU_SELECTED, &FFQMain::OnToolBarButtonClick, this);
    #endif // DEBUG

    OpenFilesDlg->SetMessage(FFQS(SID_COMMON_SELECT_FILES));

    MenuMoveUp->SetItemLabel(FFQS(SID_COMMON_MOVE_UP));
    MenuMoveDown->SetItemLabel(FFQS(SID_COMMON_MOVE_DOWN));
    MenuEdit->SetItemLabel(FFQS(SID_MAINFRAME_LVM_EDIT_JOB));
    MenuPreset->SetItemLabel(FFQS(SID_MAINFRAME_LVM_EDIT_PRESET));
    MenuClone->SetItemLabel(FFQS(SID_MAINFRAME_LVM_CLONE_JOB));
    MenuRemove->SetItemLabel(FFQS(SID_MAINFRAME_LVM_REMOVE_JOB));
    MenuFFProbe->SetItemLabel(FFQS(SID_MAINFRAME_LVM_RUN_FFPROBE));
    MenuFFCmd->SetItemLabel(FFQS(SID_MAINFRAME_LVM_SHOW_COMMAND));
    MenuStartAll->SetItemLabel(FFQS(SID_MAINFRAME_LVM_START_ALL));
    MenuStartSel->SetItemLabel(FFQS(SID_MAINFRAME_LVM_START_SEL));

    //This should be moved to wxSmith resource
    ListMenu.AppendSeparator();
    MenuStopAll = new wxMenuItem((&ListMenu), ID_TOOLBARSTOP, FFQS(SID_MAINFRAME_LVM_STOP_ALL), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuStopAll);
    MenuStopSel = new wxMenuItem((&ListMenu), ID_MENU_STOPSEL, FFQS(SID_MAINFRAME_LVM_STOP_SEL), wxEmptyString, wxITEM_NORMAL);
    ListMenu.Append(MenuStopSel);
    Connect(ID_MENU_STOPSEL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FFQMain::OnToolBarButtonClick);

    ListView->AppendColumn(FFQS(SID_MAINFRAME_LVC_FILE), wxLIST_FORMAT_LEFT);
    ListView->AppendColumn(FFQS(SID_MAINFRAME_LVC_COMMAND), wxLIST_FORMAT_LEFT);
    ListView->AppendColumn(FFQS(SID_MAINFRAME_LVC_STATUS), wxLIST_FORMAT_CENTER);

    wxSize sz = wxSize(22, 22); //wxDLG_UNIT(this, wxSize(10, 8));
    m_ListIcons = new wxImageList(sz.GetWidth(), sz.GetHeight());
    wxBitmap bmp;
    PtrToBitmap((void*)&ICON_JOB, ICON_JOB_SIZE, bmp, wxBITMAP_TYPE_PNG, sz);
    m_ListIcons->Add(bmp);
    //PtrToBitmap((void*)&TOOL_TOOLS, TOOL_TOOLS_SIZE, bmp, wxBITMAP_TYPE_PNG, sz);
    PtrToBitmap((void*)&ICON_TOOL, ICON_TOOL_SIZE, bmp, wxBITMAP_TYPE_PNG, sz);
    m_ListIcons->Add(bmp);
    ListView->SetImageList(m_ListIcons, wxIMAGE_LIST_SMALL);

    sz = wxSize(16, 16);
    m_ConsoleIcons = new wxImageList(sz.GetWidth(), sz.GetHeight());
    PtrToBitmap((void*)&ICON_TAB_MSG, ICON_TAB_MSG_SIZE, bmp);//, wxBITMAP_TYPE_PNG, sz);
    m_ConsoleIcons->Add(bmp);
    PtrToBitmap((void*)&ICON_TAB_OFF, ICON_TAB_OFF_SIZE, bmp);//, wxBITMAP_TYPE_PNG, sz);
    m_ConsoleIcons->Add(bmp);
    PtrToBitmap((void*)&ICON_TAB_ON, ICON_TAB_ON_SIZE, bmp);//, wxBITMAP_TYPE_PNG, sz);
    m_ConsoleIcons->Add(bmp);
    Consoles->SetImageList(m_ConsoleIcons);
    Consoles->SetPageImage(0, CONSOLE_IMAGE_MSG);

    ThumbsItem->SetItemLabel(FFQS(SID_MAINFRAME_TM_THUMBTOOL));
    SlideshowItem->SetItemLabel(FFQS(SID_MAINFRAME_TM_SLIDESHOWTOOL));
    ConcatItem->SetItemLabel(FFQS(SID_MAINFRAME_TM_CONCATTOOL));
    VidStabItem->SetItemLabel(FFQS(SID_VIDSTAB_TITLE) + "...");
    Vid2GifItem->SetItemLabel(FFQS(SID_VIDEO2GIF_TITLE) + "...");

    Bind(wxEVT_CHAR_HOOK, &FFQMain::OnChar, this);
    Bind(wxEVT_IDLE, &FFQMain::OnIdle, this);
    Bind(wxEVT_MAXIMIZE, &FFQMain::OnMaximize, this);
    Bind(wxEVT_MOVE, &FFQMain::OnMove, this);
    Bind(wxEVT_SHOW, &FFQMain::OnShow, this);

    //Connect(wxEVT_MAXIMIZE,(wxObjectEventFunction)&FFQMain::OnMaximize);
    //Connect(wxEVT_MOVE,(wxObjectEventFunction)&FFQMain::OnMove);
    //Connect(wxEVT_SHOW,(wxObjectEventFunction)&FFQMain::OnShow);

    ListView->DragAcceptFiles(true);
    ListView->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(FFQMain::OnDropFiles), NULL, this);

    if (Timer.IsRunning()) Timer.Stop();

    AboutBox = NULL;
    BatchMaker = NULL;
    ThumbMaker = NULL;
    JobEdit = NULL;
    OptionsDlg = NULL;
    ConcatDlg = NULL;
    VidStabDlg = NULL;
    Video2Gif = NULL;
    Console = new FFQConsole();
    Console->SetTextCtrl(TextCtrl);

    m_NumEncodingSlots = 0;
    m_EncodingActive = false;
    m_EncodingSlots = NULL;

    m_PtrList = new wxArrayPtrVoid();
    m_AllowEvents = false;
    m_FirstIdle = true;
    m_FirstShow = true;
    m_Closed = false;
    m_RestoredRect.SetPosition(GetPosition());
    m_RestoredRect.SetSize(GetSize());
    m_LastColumnPct = 0;
    //m_LastPercentDone = 0;

    //m_EncodingProcess = NULL;
    //m_CurrentItem = NULL;
    //m_ItemDurationTime = 0;
    //m_ItemDurationFrames = -1;

    Consoles->SetPageText(0, FFQS(SID_MAINFRAME_NB_DEFAULT));

	FFQCFG()->SetBrowseRootFor(OpenFilesDlg);

    m_JobsFileName = FFQCFG()->app_name.Lower() + ".job";
    #ifdef DEBUG
    SetTitle(FFQCFG()->app_name + " - DEBUG");
    #else
    SetTitle(FFQCFG()->app_name);
    #endif // DEBUG

    FFQMain::m_Instance = this;

    #ifdef __WINDOWS__
    SetIcon(wxIcon("aaaa", wxBITMAP_TYPE_ICO_RESOURCE));
    #else
    SetIcon(wxIcon(MainIcon32_XPM));
    #endif // __WINDOWS__

}

//---------------------------------------------------------------------------------------

FFQMain::~FFQMain()
{

    //(*Destroy(FFQMain)
    //*)

    if (AboutBox)
    {
        delete AboutBox;
        AboutBox = NULL;
    }

    if (BatchMaker)
    {
        delete BatchMaker;
        BatchMaker = NULL;
    }

    if (ThumbMaker)
    {
        delete ThumbMaker;
        ThumbMaker = NULL;
    }

    if (JobEdit)
    {
        delete JobEdit;
        JobEdit = NULL;
    }

    if (OptionsDlg)
    {
        delete OptionsDlg;
        OptionsDlg = NULL;
    }

    if (ConcatDlg)
    {
        delete ConcatDlg;
        ConcatDlg = NULL;
    }

    if (VidStabDlg)
    {
        delete VidStabDlg;
        VidStabDlg = NULL;
    }

    if (m_EncodingSlots)
    {
        delete[] m_EncodingSlots;
        m_EncodingSlots = NULL;
    }

    delete Console;
    Console = NULL;

    delete m_ListIcons;
    m_ListIcons = NULL;

    delete m_ConsoleIcons;
    m_ConsoleIcons = NULL;

    delete[] m_ToolBitmaps;
    m_ToolBitmaps = NULL;

    delete m_PtrList;
    m_PtrList = NULL;

    FFQConfig::Release();
    FFQLangEdit::Release();

}

//---------------------------------------------------------------------------------------

long FFQMain::FindItemForFile(wxString file_name, long start_index, bool output_file)
{

    //Searches for a queue item that has "file_name" as input or output
    wxString p;

    for (long i = start_index; i < ListView->GetItemCount(); i++)
    {

        LPFFQ_QUEUE_ITEM item = GetItemAtIndex(i);

        if (output_file)
        {
            //Get output path
            if (item->GetItemType() == qtJOB) p = ((LPFFQ_JOB)item)->out;
            else if (item->GetItemType() == qtVIDSTAB_JOB) p = ((LPFFQ_VIDSTAB_JOB)item)->out;
            else if (item->GetItemType() == qtCONCAT_JOB) p = ((LPFFQ_CONCAT_JOB)item)->out;
            else p.Clear();
        }

        //Get first source path
        else p = item->GetInput(0).path;

        //Compare if found
        if ((p.Len() > 0) && CompareFileNames(p, file_name)) return i;

    }

    return -1;

}

//---------------------------------------------------------------------------------------

LPFFQ_QUEUE_ITEM FFQMain::GetItemAtIndex(long index)
{

    //Returns the queue item at the given index (or NULL if out of bounds)
    //if ((index < 0) || (index >= ListView->GetItemCount())) return NULL;
    return (LPFFQ_QUEUE_ITEM)ListView->GetItemData(index);

}

//---------------------------------------------------------------------------------------

LPFFQ_QUEUE_ITEM FFQMain::GetSelectedItem(long *index)
{

    //Returns the selected queue item
    long sel = ListView->GetFirstSelected();
    if (index != NULL) *index = sel;
    return (sel < 0) ? NULL : GetItemAtIndex(sel);

}

//---------------------------------------------------------------------------------------

long FFQMain::IndexOfItem(LPFFQ_QUEUE_ITEM item)
{

    //Returns the index of the item in the list
    for (long i = 0; i < ListView->GetItemCount(); i++) if (GetItemAtIndex(i) == item) return i;

    //Not found
    return -1;

}

//---------------------------------------------------------------------------------------

bool FFQMain::IsPresetActive(LPFFQ_PRESET pst, bool *can_modify)
{

    //Used to determine if a preset is used by a job that is
    //active or queued and optionally if the preset can be
    //modified (because it is queued, nor active)

    bool res = false;

    for (long i = 0; i < ListView->GetItemCount(); i++)
    {

        LPFFQ_QUEUE_ITEM item = GetItemAtIndex(i);

        if ( (item->IsActive() || (item->status == qsQUEUED)) && item->UsesPreset(pst->preset_id.ToString()) )
        {

            //If can_modify is defined, set value
            if (can_modify) *can_modify = *can_modify && (item->status == qsQUEUED);

            //If not defined, we do not need to examine more items
            else return true;

            //Set res to true for the remaining iterations
            res = true;

        }

    }

    //Return result
    return res;

}

//---------------------------------------------------------------------------------------

void FFQMain::PresetChanged(LPFFQ_PRESET pst)
{

    //The preset has changed and all the queue items that depends on it must be updated

    //If a preset is (or more presets are) deleted, all must be updated
    bool all = pst == NULL;
    wxString pid = all ? "" : pst->preset_id.ToString();

    ListView->Freeze();

    for (long i = 0; i < ListView->GetItemCount(); i++)
    {

        LPFFQ_QUEUE_ITEM item = GetItemAtIndex(i);
        if (all || item->UsesPreset(pid))
        {

            //Set flag to indicate that the preset was changed
            item->queue_flags |= QUEUE_FLAG_PRESET_CHANGED;

            //Define
            DefineItem(i, item, dsNOSELECT, false);

        }

    }

    ListView->Thaw();

}

//---------------------------------------------------------------------------------------

bool FFQMain::PreviewCommand(wxString cmd, bool add_to_console)
{

    /*
    # ifdef __WINDOWS__
    wxString prm = FFQCFG()->keep_console ? "k" : "c";
    cmd = "cmd /" + prm + " \"\"" + FFQCFG()->GetFFMpegCommand() + "\" " + cmd + "\"";
    # else
    wxString prm = FFQCFG()->keep_console ? "-hold" : "+hold";
    cmd = "xterm " + prm + " -e '\"" + FFQCFG()->GetFFMpegCommand() + "\" " + cmd + "'";
    # endif
    */

    cmd = FFQCFG()->GetConsoleCommand(FFQCFG()->GetFFMpegCommand(), cmd);
    if (cmd.Len() == 0) return false;

    //Only add command to console when queue is passive
    if (add_to_console) Console->AppendLine(CRLF + FFQSF(SID_LOG_EXEC_PREVIEW_CMD, cmd), COLOR_GRAY);

    if (wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER) == 0)
    {

        if (add_to_console) Console->AppendLine(FFQS(SID_LOG_PREVIEW_CMD_FAIL), COLOR_RED);
        return false;

    }

    return true;

}

//---------------------------------------------------------------------------------------

void FFQMain::SelectConsole(FFQConsole* by_pointer, int by_index)
{
    if (m_EncodingActive && (by_index > 0)) Consoles->ChangeSelection(by_index);
    else if ((!m_EncodingActive) && (by_pointer == this->Console) && (Consoles->GetSelection() != 0)) Consoles->ChangeSelection(0);
}

//---------------------------------------------------------------------------------------

int FFQMain::FindEncodingSlot(LPFFQ_QUEUE_ITEM item)
{

    //Find the index of the processing slot belonging to "item".
    //If item is NULL, the index of the first unused slot is returned
    for (int i = 0; i < m_NumEncodingSlots; i++) if (m_EncodingSlots[i].item == item) return i;
    return -1;

}

//---------------------------------------------------------------------------------------

void FFQMain::InitEncodingSlots()
{

    //Initialize encoding slots..
    int nes = FFQCFG()->num_encode_slots < 1 ? 1 : FFQCFG()->num_encode_slots;

    if (nes > m_NumEncodingSlots)
    {

        //Increasing the number of encoding slots always works..
        LPENCODING_SLOT slots = new ENCODING_SLOT[nes];
        wxFont fnt(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Monospace"), wxFONTENCODING_DEFAULT);
        for (int i = 0; i < nes; i++)
        {
            LPENCODING_SLOT slot = &slots[i];
            slot->index = i;
            if (i < m_NumEncodingSlots)
            {
                //Copy existing slot
                slot->gauge = m_EncodingSlots[i].gauge;
                slot->console.SetTextCtrl(m_EncodingSlots[i].console.GetTextCtrl());
            }
            else
            {
                wxPanel *pan = new wxPanel(Consoles, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
                wxFlexGridSizer *fgs = new wxFlexGridSizer(2, 1, 0, 0);
                fgs->AddGrowableCol(0);
                fgs->AddGrowableRow(0);
                wxTextCtrl *tc = new wxTextCtrl(pan, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_DONTWRAP|wxBORDER_NONE, wxDefaultValidator);
                tc->SetFont(fnt);
                fgs->Add(tc, 0, wxEXPAND, 5);
                slot->gauge = new wxGauge(pan, wxID_ANY, GAUGE_MAX);
                //slot->gauge->SetValue(GAUGE_MAX / 2);
                fgs->Add(slot->gauge, 1, wxALL|wxEXPAND, 0);
                pan->SetSizer(fgs);
                fgs->Fit(pan);
                fgs->SetSizeHints(pan);
                Consoles->AddPage(pan, FFQSF(SID_MAINFRAME_NB_FOR_JOB, (i+1)), false);
                Consoles->SetPageImage(i + 1, CONSOLE_IMAGE_OFF);//i == 0 ? CONSOLE_IMAGE_OFF : CONSOLE_IMAGE_ON);
                slot->console.SetTextCtrl(tc);
            }
        }

        if (m_EncodingSlots) delete[] m_EncodingSlots;
        m_EncodingSlots = slots;
        m_NumEncodingSlots = nes;

    }
    else if ((!m_EncodingActive) && (nes < m_NumEncodingSlots))
    {

        //Decreasing number of encoding slots only works if not encoding and the
        //slots to remove have empty text controls..
        int keep = m_NumEncodingSlots;
        for (int i = m_NumEncodingSlots - 1; i >= nes; i--)
        {
            if (m_EncodingSlots[i].console.GetTextCtrl()->IsEmpty())
            {
                Consoles->DeletePage(i+1);
                keep--;
            }
            else break;
        }

        if (keep < m_NumEncodingSlots)
        {
            LPENCODING_SLOT slots = new ENCODING_SLOT[keep];
            for (int i = 0; i < keep; i++)
            {
                //Copy existing slot
                slots[i].gauge = m_EncodingSlots[i].gauge;
                slots[i].console.SetTextCtrl(m_EncodingSlots[i].console.GetTextCtrl());
            }
            delete[] m_EncodingSlots;
            m_EncodingSlots = slots;
            m_NumEncodingSlots = keep;
        }
    }

}

//---------------------------------------------------------------------------------------

void FFQMain::AfterItemProcessing(LPENCODING_SLOT slot)
{

    //The method is called when the processing of a queued item has finished

    if (slot->item->status != qsFAILED)
    {

        //Update status
        if (slot->process.WasAborted())
        {
            slot->item->status = qsABORTED;
            m_EncodingAborted++;
        }
        else if (slot->console.GetStatisticsTotal() == 0)
        {
            slot->item->status = qsFAILED;
            m_EncodingFailed++;
        }
        else m_EncodingSuccess++;

    }

    //Update list
    DefineItem(IndexOfItem(slot->item), slot->item, dsNOSELECT, false);

    //Log status
    LogItemStatus(slot, false);

    //Save log as required
    wxString ln;
    if (slot->item->GetLogFileName(ln)) slot->console.SaveAsHtml(ln);

    //Cleanup
    slot->item->Cleanup();

    //Clear current item & command
    slot->item = NULL;
    slot->command.Clear();

    //Update console icon
    Consoles->SetPageImage(slot->index+1, CONSOLE_IMAGE_OFF);


}

//---------------------------------------------------------------------------------------

bool FFQMain::BeforeItemProcessing(LPENCODING_SLOT slot)
{

    //The method is called when a queued item is about to be processed

    //Store starting tick count
    slot->started = GetTimeTickCount();

    //Clear console
    slot->console.Clear(true);

    //Check if output file can be overwritten
    wxString out = (slot->item->GetItemType() == qtJOB) ? ((LPFFQ_JOB)slot->item)->out : "";

    //Check if it exists
    bool exists = (out.Len() > 0) && wxFileExists(out), start = true;

    if ( exists && ((slot->item->queue_flags & QUEUE_FLAG_OVERWRITE) != 0) )
    {

        //File exists and it is allowed to be overwritten - try to delete it
        if (!wxRemoveFile(out))
        {

            //Failed to delete = job failed
            slot->item->status = qsFAILED;
            m_EncodingFailed++;
            slot->console.AppendLine(FFQS(SID_LOG_WRITE_DEST_ERROR), COLOR_RED);
            start = false;

        }

    }

    else if (exists)
    {

        //Overwrite not accepted = job skipped
        slot->item->status = qsSKIPPED;
        start = false;

    }

    //Is item about to be started?
    if (start)
    {

        //Clear all flags other than the overwrite one
        slot->item->queue_flags &= QUEUE_FLAG_OVERWRITE;

        //Prepare processing
        slot->item->PrepareProcessing();

        //Update console icon
        Consoles->SetPageImage(slot->index+1, CONSOLE_IMAGE_ON);

    }
    //Not started: Update list
    else DefineItem(IndexOfItem(slot->item), slot->item, dsNOSELECT, false);

    //Return success|not
    return start;

}

//---------------------------------------------------------------------------------------

void FFQMain::DeleteProcessedItems()
{

    if (!FFQCFG()->auto_remove_jobs) return;

    ListView->Freeze();
    int idx = 0;
    while (idx < ListView->GetItemCount())
    {
        if (GetItemAtIndex(idx)->status == qsDONE) DeleteItem(idx);
        else idx++;
    }
    ListView->Thaw();

}

//---------------------------------------------------------------------------------------

LPFFQ_QUEUE_ITEM FFQMain::FindNextItemToProcess(LPFFQ_QUEUE_ITEM from_item)
{

    //Find next queued item

    //Get number of items in the list
    long num_items = ListView->GetItemCount();

    //Get the wrapping point and set first index to check
    long start = (from_item == NULL) ? 0 : IndexOfItem(from_item) + 1, index = start;

    //Search the list
    while (true)
    {

        //Wrap index?
        if (index >= num_items) index = 0;
        else
        {

            //Check item at index
            LPFFQ_QUEUE_ITEM item = GetItemAtIndex(index);
            if (item->status == qsQUEUED) return item; //Queued item found

            //Increment index
            index++;

        }

        //Wrapping point reached?
        if (index == start) return NULL;

    }

    //No queued items found
    return NULL;

}

//---------------------------------------------------------------------------------------

void FFQMain::LogItemStatus(LPENCODING_SLOT slot, bool first_command)
{

    //Used to log the status of the current item during processing
    //qsACTIVE, qsPASS1, qsPASS2, qsTHUMBS, qsDONE, qsFAILED, qsABORTED, qsSKIPPED, qsDORMANT

    if (!first_command)
    {

        //A command has finished

        //Force gauge to finished - just in case
        slot->gauge->SetValue(slot->gauge->GetRange());

        //Append empty line
        slot->console.AppendBlankLine();

        //Flush messages
        slot->console.FlushMessages();

        //Log ffmpeg running time
        TIME_VALUE t = TIME_VALUE(slot->process.GetRunningTimeMillis());
        slot->console.AppendLine(FFQSF(SID_LOG_FFMPEG_RUNTIME, FFQDT(), t.ToString()), COLOR_GRAY);

        //Log statistics
        if (slot->console.GetStatisticsTotal() == 0) slot->console.AppendLine(FFQS(SID_LOG_COMPLETED_IRREG), COLOR_RED);
        else slot->console.AppendLine(FFQSF(SID_LOG_COMPLETED_OK, slot->console.GetStatisticsTotal()), COLOR_BLACK);

    }

    const auto logstart = [this, slot]() {
        Console->AppendWithTime(FFQSF(SID_LOG_JOB_TAB_STARTED, Consoles->GetPageText(FindEncodingSlot(slot->item) + 1),  FFQ_INPUT_FILE(slot->item->inputs[0]).path /*slot->command*/), COLOR_BLUE);
    };

    const auto logend = [this, slot](FFQ_SID sid, uint32_t color) {
        Console->AppendWithTime(FFQSF(sid, Consoles->GetPageText(FindEncodingSlot(slot->item) + 1)), color);
    };

    bool total_time = false;

    switch (slot->item->status)
    {

        case qsACTIVE:
        case qsTHUMBS:
            //Command start
            logstart();
            if (!first_command) slot->console.AppendBlankLine(2);
            slot->console.AppendLine(FFQSF(SID_LOG_STARTING_FFMPEG, "", FFQDT(), slot->command), COLOR_GRAY);
            break;

        case qsPASS1:
            //First pass
            logstart();
            slot->console.AppendLine(FFQSF(SID_LOG_STARTING_FFMPEG, SPACE + FFQS(SID_LOG_FOR_FIRST_PASS), FFQDT(), slot->command), COLOR_GRAY);
            break;

        case qsPASS2:
            //Second pass
            logstart();
            slot->console.AppendBlankLine(2);
            slot->console.AppendLine(FFQSF(SID_LOG_STARTING_FFMPEG, SPACE + FFQS(SID_LOG_FOR_SECOND_PASS), FFQDT(), slot->command), COLOR_GRAY);
            break;

        case qsDONE:
            logend(SID_LOG_JOB_TAB_ENDED, COLOR_BLUE);
            //Console->AppendLine(FFQSF(SID_LOG_JOB_TAB_ENDED, GETLOGTIME(), Consoles->GetPageText(FindEncodingSlot(slot->item))), COLOR_BLUE);
            total_time = true;
            break;

        case qsFAILED:
            //Finished or failed (handled above)
            logend(SID_LOG_JOB_TAB_FAILED, COLOR_RED);
            //Console->AppendLine(FFQSF(SID_LOG_JOB_TAB_FAILED, GETLOGTIME(), Consoles->GetPageText(FindEncodingSlot(slot->item))), COLOR_RED);
            total_time = true;
            break;

        case qsABORTED:
            //Aborted
            logend(SID_LOG_JOB_TAB_ABORTED, COLOR_RED);
            //Console->AppendLine(FFQSF(SID_LOG_JOB_TAB_ABORTED, Consoles->GetPageText(FindEncodingSlot(slot->item)), GETLOGTIME()), COLOR_RED);
            slot->console.AppendLine(FFQS((slot->item->GetItemType() != qtJOB) ? SID_LOG_COMMAND_ABORTED : SID_LOG_JOB_ABORTED), COLOR_RED);
            total_time = true;
            break;

        default: break;

    }

    if (total_time)
    {

        //Log if preset was changed during encoding
        if ((slot->item->queue_flags & QUEUE_FLAG_PRESET_CHANGED) != 0)
        {
            slot->console.AppendBlankLine();
            slot->console.AppendLine(FFQS(SID_LOG_PRESET_WAS_CHANGED), COLOR_ORANGE);
        }

        //Log the total item time
        TIME_VALUE tv = TIME_VALUE(GetTimeTickCount() - slot->started);
        slot->console.AppendBlankLine(2);
        slot->console.AppendLine(FFQSF(SID_LOG_TOTAL_JOB_TIME, tv.ToString()), COLOR_BLACK);

    }

}

//---------------------------------------------------------------------------------------

bool FFQMain::MustBeQueued(LPFFQ_QUEUE_ITEM item, long item_index, bool selected_only)
{

    //Returns true if the item must be queued

    //Check selection
    if (selected_only && (!ListView->IsSelected(item_index))) return false;

    //Get item if NULL
    if (item == NULL) item = GetItemAtIndex(item_index);

    //Check status for selected items
    //if (selected_only) return  (item->status == qsABORTED) || (item->status == qsDONE) || (item->status == qsDORMANT) ||
    //                          (item->status == qsFAILED) || (item->status == qsSKIPPED);
    if (selected_only) return (!item->IsActive()) && (item->status != qsQUEUED);

    //Check status for all items
    return (item->status == qsDORMANT) || (item->status == qsABORTED);

}

//---------------------------------------------------------------------------------------

bool FFQMain::OverwritePrompt(bool selected_only)
{

    //Ask the user if any existing output files can be overwritten

    //Set to false
    //m_OverwriteAllowed = false;

    //Counters and lists
    unsigned int cnt = 0;
    wxString files = "", s;

    //Clear pointers
    m_PtrList->Clear();

    //Count files to overwrite and make a list of their names
    for (int i = 0; i < ListView->GetItemCount(); i++)
    {

        LPFFQ_QUEUE_ITEM item = GetItemAtIndex(i);

        if (MustBeQueued(item, i, selected_only))
        {

            wxString fn;

            switch (item->GetItemType())
            {

                case qtJOB: fn = ((LPFFQ_JOB)item)->out; break;
                case qtCONCAT_JOB: fn = ((LPFFQ_CONCAT_JOB)item)->out; break;
                case qtVIDSTAB_JOB: fn = ((LPFFQ_VIDSTAB_JOB)item)->out; break;
                case qtVID2GIF_JOB: fn = ((LPFFQ_VID2GIF_JOB)item)->out; break;
                case qtSTATIC_JOB:
                case qtTHUMB_JOB: break;
                default:
                    #ifdef DEBUG
                    ShowError("OverwritePrompt: Invalid item type");
                    #endif // DEBUG
                    break;

            }

            if ((fn.Len() > 0) && wxFileExists(fn))
            {

                files += fn + CRLF;
                cnt++;

            }

            //Add item to list of pointers that must be queued
            m_PtrList->Add(item);

        }

    }

    if (cnt > 0)
    {

        int res = wxMessageBox(FFQSF(SID_OVERWRITE_OUTPUT_FILES, cnt) + CRLF + CRLF + files, FFQS(SID_CONFIRM),
                               wxYES_NO | wxCANCEL | wxCANCEL_DEFAULT | wxICON_QUESTION);

        //If the user cancels or says no to all items we do not need to
        //put them in the queue
        if ((res == wxCANCEL) || ((res == wxNO) && (cnt == m_PtrList->Count()))) return false;

        //Update overwrite allowed
        //m_OverwriteAllowed = (res == wxYES);

        for (unsigned int i = 0; i < m_PtrList->Count(); i++)
        {

            //Set / unset individual overwrite allowed
            LPFFQ_QUEUE_ITEM item = (LPFFQ_QUEUE_ITEM)m_PtrList->Item(i);
            if (res == wxYES) item->queue_flags |= QUEUE_FLAG_OVERWRITE;
            else item->queue_flags &= ~QUEUE_FLAG_OVERWRITE;


        }

    }

    return true;

}

//---------------------------------------------------------------------------------------

bool FFQMain::ProcessNext(LPENCODING_SLOT slot)
{

    //Process next command from the current item (if available)

    bool first_cmd = (slot->command.Len() == 0);

    if ((slot->item != NULL) && slot->item->GetNextCommand(slot->command))
    {

        //Console->AppendLine("Command=" + m_CurrentCommand, COLOR_GRAY);

        //A command was received, extract the duration
        wxString len = GetToken(slot->command, ",", true);
        slot->frames = Str2Long(len, -1); //Convert to frames
        slot->duration = TIME_VALUE(len); //Convert to time value

        //Update item in the list
        long item_index = IndexOfItem(slot->item);
        DefineItem(item_index, slot->item, dsNOSELECT, false);

        //Log what is about to be done
        LogItemStatus(slot, first_cmd);

        //Update controls: status of selection might have changed
        UpdateControls();

        try
        {

            //Reset console counters for next command
            slot->console.Clear(false);

            //Blank line as separation
            slot->console.AppendBlankLine();

            //Run the command
            slot->process.SetCommand(false, slot->command);
            slot->process.Execute(false, true);

            //Return success
            return true;

        }
        catch (std::exception &err)
        {

            slot->item->status = qsFAILED;
            m_EncodingFailed++;
            slot->console.AppendLine(FFQSF(SID_LOG_EXECUTE_ERROR, err.what()), COLOR_RED);

        }

    }

    //The following code is only reached if:
    //1) no item is started
    //2) an item has finished
    //3) a command has errored

    //If an item has finished, do after processing stuff
    if (slot->item != NULL) AfterItemProcessing(slot);

    //Find the next item that can be processed
    do {

        slot->item = FindNextItemToProcess(slot->item);
        //Console->AppendLine("FindNext=" + wxString(m_CurrentItem ? "ITEM" : "NULL"), COLOR_GRAY);
        if (slot->item == NULL) return false; //No more items to process = done!

    } while (!BeforeItemProcessing(slot));

    //Console->AppendLine("RecurseProcessNext", COLOR_GRAY);

    //Recurse call to run first command
    return ProcessNext(slot); //Should always return true here

}

//---------------------------------------------------------------------------------------

void FFQMain::ProcessReadOutput()
{

    //Reads output from ffmpeg, and adds it to the TextCtrl

    wxString s;
    bool ok, update = false;

    for (int i = 0; i < m_NumEncodingSlots; i++)
    {

        LPENCODING_SLOT slot = &m_EncodingSlots[i];

        //Read stdout
        do {

            s = slot->process.GetProcessOutputLine(false);
            ok = s.Len() > 0;
            if (ok) slot->console.AppendFFOutput(s, true);

        } while (ok);

        //Read errout
        do {

            s = slot->process.GetProcessOutputLine(true);
            ok = s.Len() > 0;
            if (ok && (slot->console.AppendFFOutput(s, false) == mtSTATS)) update = true;

        } while (ok);

    }

    //Update status if needed
    if (update) UpdateStatus();

}

//---------------------------------------------------------------------------------------

void FFQMain::FinishQueue()
{

    //Finish the queue after processing items
    m_EncodingActive = false;

    //If nothing was processed, we bail out here
    //if (m_EncodingAborted + m_EncodingFailed + m_EncodingSuccess == 0) return;

    //Delete processed jobs
    DeleteProcessedItems();

    //Update number of encoding slots
    InitEncodingSlots();

    //Update controls
    UpdateControls();

    //Prompt the user if not silent finish
    if (!FFQCFG()->silent_qfinish)
    {

        if (m_EncodingAborted > 0) ShowInfo(ListView, FFQS(SID_TASK_ABORTED));
        else ShowInfo(ListView, FFQS(SID_QUEUE_COMPLETED));

    }

    TIME_VALUE elapsed(GetTimeTickCount() - m_QueueStarted);
    Console->AppendWithTime(FFQSF(SID_LOG_QUEUE_ENDED,
        elapsed.ToReadableString(),
        m_EncodingAborted + m_EncodingFailed + m_EncodingSuccess,
        m_EncodingSuccess,
        m_EncodingFailed,
        m_EncodingAborted),
        COLOR_BLUE);

}

//---------------------------------------------------------------------------------------

void FFQMain::StartQueue(bool selected_only)
{

    //Check fonts conf when starting queue
    if (!m_EncodingActive) FFQCFG()->CheckFontsConf();

    //Prompt if files are about to be overwritten
    if (!OverwritePrompt(selected_only)) return;

    //Check if any items must be queued
    if (m_PtrList->Count() == 0)
    {

        //Warn about nothing queued for good measures
        //before returning
        ShowInfo(this, FFQS(SID_NOTHING_QUEUED));
        return;

    }

    //Prepare items
    ListView->Freeze();
    for (unsigned int i = 0; i < m_PtrList->Count(); i++)
    {

        LPFFQ_QUEUE_ITEM item = (LPFFQ_QUEUE_ITEM)m_PtrList->Item(i);
        item->status = qsQUEUED;
        DefineItem(IndexOfItem(item), item, dsNOSELECT, false);

    }
    ListView->Thaw();

    //Set encoding active and start the timer
    if (!m_EncodingActive)
    {
        Console->AppendWithTime(FFQS(SID_LOG_QUEUE_STARTED), COLOR_BLUE);
        m_QueueStarted = GetTimeTickCount();
        m_EncodingAborted = 0;
        m_EncodingFailed = 0;
        m_EncodingSuccess = 0;
        m_EncodingActive = true;
        Timer.Start(0, true);
    }

    //Update controls
    //UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQMain::StopQueue(bool selected_only)
{

    //Un-queue items and abort as needed
    bool update = true;

    ListView->Freeze();
    for (int i = 0; i < ListView->GetItemCount(); i++) if ((!selected_only) || ListView->IsSelected(i))
    {
        LPFFQ_QUEUE_ITEM item = GetItemAtIndex(i);
        if (item->status == qsQUEUED)
        {
            item->status = qsDORMANT;
            DefineItem(i, item, dsNOSELECT, false);
        }
        else if (item->IsActive())
        {
            update = false;
            m_EncodingSlots[FindEncodingSlot(item)].process.Abort(true);
        }
    }
    ListView->Thaw();

    if (update) UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQMain::DefineItem(long index, LPFFQ_QUEUE_ITEM item, DEFINE_SELECT select, bool save_if_required)
{

    //Reset input
    item->GetInput(-1);

    if (index < 0) index = ListView->InsertItem(ListView->GetItemCount(), item->DisplayName()); //New item
    else ListView->SetItemText(index, item->DisplayName()); //Existing item

    ListView->SetItemPtrData(index, (wxUIntPtr)item);

    if (item->GetItemType() == qtJOB)
    {

        ListView->SetItemImage(index, 0);

        //Set info for a basic job
        LPFFQ_JOB job = (LPFFQ_JOB)item;

        if (job->preset_id == UNIQUE_ID_NULL) ListView->SetItem(index, 1, FFQSF(SID_COMMAND, job->cmd_line));
        else
        {

            LPFFQ_PRESET pst = FFQPresetMgr::Get()->GetPreset(job->preset_id.ToString());
            ListView->SetItem(index, 1, FFQSF(SID_PRESET, (pst == NULL) ?  FFQS(SID_INVALID_PRESET) : pst->preset_name));

        }

    }
    else
    {

        ListView->SetItemImage(index, 1);

        wxString s;

        if (item->GetItemType() == qtTHUMB_JOB) s = FFQSF(SID_TOOL_JOB, FFQS(SID_MAINFRAME_TM_THUMBTOOL));

        else if (item->GetItemType() == qtVIDSTAB_JOB) s = FFQSF(SID_TOOL_JOB, FFQS(SID_VIDSTAB_TITLE));

        else if (item->GetItemType() == qtVID2GIF_JOB) s = FFQSF(SID_TOOL_JOB, FFQS(SID_VIDEO2GIF_TITLE));

        else if (item->GetItemType() == qtCONCAT_JOB)
        {

            if (((LPFFQ_CONCAT_JOB)item)->slideshow) s = FFQSF(SID_TOOL_JOB, FFQS(SID_CONCAT_VIDEO_FROM_IMAGES));

            else if (((LPFFQ_CONCAT_JOB)item)->simple_concat) s = FFQSF(SID_TOOL_JOB, FFQS(SID_CONCAT_SIMPLE));

            else s = FFQSF(SID_TOOL_JOB, FFQS(SID_CONCAT_MERGE_FILES));

        }

        else if (item->GetItemType() == qtSTATIC_JOB) s = m_EncodingSlots[FindEncodingSlot(item)].command;// s = m_CurrentCommand;

        while (s.Right(1) == ".") s.RemoveLast(); //Remove "..." if present
        ListView->SetItem(index, 1, s);

    }

    ListView->SetItem(index, 2, FFQL()->QUEUE_STATUS_NAMES[item->status]);

    if (select == dsUNIQUE) for (long i = 0; i < ListView->GetItemCount(); i++) ListView->Select(i, i == index);
    else if (select == dsAPPEND) ListView->Select(index, true);

    if (select != dsNOSELECT)
    {
        ListView->EnsureVisible(index);
        ListView->Focus(index);
    }

    //Save items if required
    //if (save_if_required && (!FFQCFG()->save_on_modify)) SaveItems();
    if (save_if_required && FFQCFG()->save_on_modify) SaveItems();

}

//---------------------------------------------------------------------------------------

void FFQMain::DeleteItem(long index)
{
    ListView->Freeze();

    if (index < 0) //Delete all
    {

        for (long i = 0; i < ListView->GetItemCount(); i++) delete (LPFFQ_QUEUE_ITEM)ListView->GetItemData(i);
        ListView->ClearAll();

    }
    else //Delete single
    {

        delete (LPFFQ_QUEUE_ITEM)ListView->GetItemData(index);
        ListView->DeleteItem(index);

    }

    ListView->Thaw();
}

//---------------------------------------------------------------------------------------

void FFQMain::EditJob(long index, wxString forFileName, LPFFQ_QUEUE_ITEM clone)
{

    //Make sure the job editor is available
    if (JobEdit == NULL) JobEdit = new FFQJobEditAdv(this);

    if (index == -1)
    {

        //Create a new job - make sure not to define both "forFileName" and "clone"
        LPFFQ_JOB job = (clone && (clone->GetItemType() == qtJOB)) ? new FFQ_JOB(*((LPFFQ_JOB)clone)) : new FFQ_JOB();

        if (forFileName.Len() > 0)
        {

            //Initialize the new job
            FFQ_INPUT_FILE inf;

            //Input 1
            inf.path = forFileName;
            job->inputs.Add(inf.ToString());

            //Input 2
            inf.path = FFQCFG()->FindSecondaryInputFile(forFileName);;
            if (inf.path.Len() > 0) job->inputs.Add(inf.ToString());

            //Output
            job->out = FFQCFG()->GetPreferredOutputName(forFileName);

        }

        //Edit the job
        if (JobEdit->Execute(job)) DefineItem(-1, job, dsUNIQUE, true); //Success
        else delete job; //Canceled, delete the job

    }

    else
    {

        //Edit existing job
        LPFFQ_QUEUE_ITEM item = GetItemAtIndex(index);

        //Ensure that item is a job
        if (item->GetItemType() != qtJOB) return;

        //Launch editor and define as required
        if (JobEdit->Execute((LPFFQ_JOB)item)) DefineItem(index, item, dsNOSELECT, true);

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQMain::LoadItems()
{

    //Loads items from an array of base64 encoded strings
    //where each line is a single item

    wxArrayString* as = FFQCFG()->LoadArrayString(m_JobsFileName);

    if (as != NULL)
    {

        ListView->Freeze();

        for (unsigned int i = 0; i < as->Count(); i++)
        {

            LPFFQ_QUEUE_ITEM item = FFQ_QUEUE_ITEM::Parse(StrFromBase64(as->Item(i)));
            DefineItem(-1, item, dsNOSELECT, false);

        }

        ListView->EnsureVisible(0);
        ListView->Select(0, true);
        ListView->Focus(0);
        ListView->Thaw();

        delete as;

    }

}

//---------------------------------------------------------------------------------------

void FFQMain::SaveItems(bool prompt)
{

    //Saves items in the queue to file as a list of base64 encoded strings

    wxArrayString* as = new wxArrayString();
    for (int i = 0; i < ListView->GetItemCount(); i++)
    {
        wxString itemstr = GetItemAtIndex(i)->ToString();
        as->Add(StrToBase64(itemstr));
    }
    bool ok = FFQCFG()->SaveArrayString(m_JobsFileName, as->Count() > 0 ? as : NULL); //NULL will delete any existing file
    delete as;
    if (!ok) ShowError(this, FFQS(SID_SAVE_JOBS_ERROR));
    else if (prompt) ShowInfo(this, FFQS(SID_SAVE_JOBS_SUCCESS));

}

//---------------------------------------------------------------------------------------

void FFQMain::ValidateItems()
{

    long idx = 0, del = 0;
    wxString s, missing;

    Console->AppendLine("", COLOR_BLACK, false);

    while (idx < ListView->GetItemCount())
    {

        LPFFQ_QUEUE_ITEM item = GetItemAtIndex(idx);

        //Check all input files for the job
        missing = "";
        for (unsigned int fidx = 0; fidx < item->inputs.Count(); fidx++)
        {

            s = item->GetInput(fidx).path;
            if (!wxFileExists(s)) missing += "\t" + s + CRLF;

        }

        if (missing.Len() > 0)
        {

            Console->AppendLine(FFQSF(SID_LOG_JOB_REMOVED, item->GetInput(0).path.AfterLast(wxFileName::GetPathSeparator()), missing), COLOR_RED, false);
            ListView->DeleteItem(idx);
            delete item;
            del++;

        }

        else idx++;

    }

    if (del > 0) ShowWarning(this, FFQS(SID_JOBS_REMOVED));

}

//---------------------------------------------------------------------------------------

void FFQMain::MoveItems(bool up)
{

    int cnt = ListView->GetItemCount();
    if (cnt < 2) return;

    ListView->Freeze();
    if (up)
    {
        if (ListView->IsSelected(0)) return;
        for (int i = 1; i < cnt; i++) if (ListView->IsSelected(i)) SwapItems(i, i-1);
    }
    else
    {
        if (ListView->IsSelected(cnt-1)) return;
        for (int i = cnt-2; i >= 0; i--) if (ListView->IsSelected(i)) SwapItems(i, i+1);
    }
    ListView->Thaw();

    //Save if required
    if (FFQCFG()->save_on_modify) SaveItems();

}

//---------------------------------------------------------------------------------------

void FFQMain::SwapItems(int a, int b)
{

    ListView->Freeze();
    LPFFQ_QUEUE_ITEM qia = (LPFFQ_QUEUE_ITEM)ListView->GetItemData(a),
                     qib = (LPFFQ_QUEUE_ITEM)ListView->GetItemData(b);
    bool sa = ListView->IsSelected(a),
         sb = ListView->IsSelected(b);
    DefineItem(a, qib, dsNOSELECT, false);
    ListView->Select(a, sb);
    DefineItem(b, qia, dsNOSELECT, false);
    ListView->Select(b, sa);
    ListView->Thaw();

}

//---------------------------------------------------------------------------------------

void FFQMain::ResizeColumns(bool dragging)
{
    //Resize the columns in the listview
    m_AllowEvents = true;
    float cliw = ListView->GetClientSize().GetX() - m_StatusColumnWidth, col1pct = 40, dummy;
    if (m_FirstShow)//(int)m_LastColumnPct == 0)
    {
        //Get the column percentage from config
        dummy = Str2Float(FFQCFG()->list_columns, 0);
        if (dummy != 0) col1pct = dummy;
    }
    else if (dragging)
    {
        col1pct = (float)(ListView->GetColumnWidth(0) * 100) / cliw;
        FFQCFG()->list_columns = wxString::Format("%.2f", col1pct);
        //FFQCFG()->SaveConfig();
    }
    else col1pct = m_LastColumnPct;
    m_LastColumnPct = col1pct;
    dummy = (cliw * col1pct) / 100.0f;
    if (ListView->GetColumnWidth(0) != (int)dummy)
    {
        ListView->Freeze();
        ListView->SetColumnWidth(0, (int)dummy);
        ListView->SetColumnWidth(1, (int)cliw - (int)dummy);
        ListView->SetColumnWidth(2, m_StatusColumnWidth);
        ListView->Thaw();
    }
    m_AllowEvents = false;
}

//---------------------------------------------------------------------------------------

void FFQMain::UpdateControls()
{

    //Disable controls that cannot be used in the current state
    //{ qsQUEUED, qsACTIVE, qsPASS1, qsPASS2, qsTHUMBS, qsDONE, qsFAILED, qsABORTED, qsSKIPPED, qsDORMANT }

    bool sel_active = false,
         can_start = false, sel_can_start = false,
         sel_can_stop = false,
         sel_can_preview = false;

    int sel_count = 0, all_count = ListView->GetItemCount();

    for (long i = 0; i < all_count; i++)
    {

        LPFFQ_QUEUE_ITEM cur = GetItemAtIndex(i);
        can_start = can_start || (cur->status == qsDORMANT) || (cur->status == qsABORTED);

        if (ListView->IsSelected(i))
        {

            sel_count++;
            bool is_active = cur->IsActive();
            sel_active = sel_active || is_active;
            sel_can_start = sel_can_start || ((!is_active) && (cur->status != qsQUEUED));
            sel_can_stop = sel_can_stop || is_active || (cur->status == qsQUEUED);
            sel_can_preview = sel_can_start && cur->CanPreview();

        }

    }

    ToolBar->EnableTool(ID_TOOLBARREMOVE, (sel_count > 0) && (!sel_active));
    ToolBar->EnableTool(ID_TOOLBAREDIT, (sel_count == 1) && (!sel_active));

    ToolBar->EnableTool(ID_TOOLBARPREVIEW, (sel_count == 1) && sel_can_preview);

    ToolBar->EnableTool(ID_TOOLBARSTART, can_start);
    ToolBar->EnableTool(ID_TOOLBARSTOP, m_EncodingActive);// m_EncodingProcess != NULL);

    MenuMoveUp->Enable((sel_count > 0) && (!ListView->IsSelected(0)));
    MenuMoveDown->Enable((sel_count > 0) && (!ListView->IsSelected(all_count - 1)));

    MenuEdit->Enable(ToolBarEdit->IsEnabled());
    MenuClone->Enable(sel_count == 1);
    MenuPreset->Enable((sel_count == 1) && (!sel_active));
    MenuRemove->Enable(ToolBarRemove->IsEnabled());

    MenuFFProbe->Enable((sel_count == 1) && (!m_EncodingActive));//(m_EncodingProcess == NULL));
    MenuFFCmd->Enable(MenuFFProbe->IsEnabled());

    MenuStartAll->Enable(ToolBarStart->IsEnabled());
    MenuStopAll->Enable(ToolBarStop->IsEnabled());

    MenuStartSel->Enable(sel_can_start);
    MenuStopSel->Enable(sel_can_stop);

    //Always update status as well
    UpdateStatus();

}

//---------------------------------------------------------------------------------------

void FFQMain::UpdateProgress(LPENCODING_SLOT slot, unsigned int pos)
{
    if (slot)
    {
        slot->gauge->SetValue(pos);
        if (pos == 0) slot->gauge->UnsetToolTip();
        else slot->gauge->SetToolTip(ToStr((unsigned int)round(pos / GAUGE_MAX * 100.0)) + "%");
    }
    FFQCFG()->GetTaskBar()->SetTaskBarProgress(pos, (unsigned int)GAUGE_MAX);
}

//---------------------------------------------------------------------------------------

void FFQMain::UpdateStatus()
{

    //Get selected page and slot
    int sel_page = Consoles->GetSelection();
    LPENCODING_SLOT slot = sel_page <= 0 ? NULL : &m_EncodingSlots[sel_page - 1];

    TIME_VALUE time = slot ? slot->duration : 0;//m_ItemDurationTime;
    double frames = slot ? slot->frames : 0;//m_ItemDurationFrames;
    wxString s, total_time = "???";
    bool tooltip = (slot != NULL);

    if (!m_EncodingActive)
    {

        //Not encoding
        StatusBar->SetStatusText(FFQS(SID_FFMPEG_INACTIVE), 1);
        tooltip = false;
        UpdateProgress(NULL, 0);

    }
    else if ((slot == NULL) || (time.IsUndefined() && (frames <= 0)))
    {

        //Unknown job duration
        StatusBar->SetStatusText(FFQS(SID_UNKNOWN_TIME_REMAINS), 1);
        UpdateProgress(NULL, 0);

    }
    else
    {

        double elapsed = slot->process.GetRunningTimeMillis(), pct_done, total;

        pct_done = (frames <= 0) ? (slot->console.GetEncodedTime().ToDouble() / time.ToDouble()) : ((double)slot->console.GetEncodedFrames() / frames);
        if (pct_done == 0) pct_done = 0.01;
        pct_done *= GAUGE_MAX;
        if (pct_done > GAUGE_MAX) pct_done = GAUGE_MAX;

        total = elapsed / pct_done * GAUGE_MAX;

        UpdateProgress(slot, round(pct_done));

        if (pct_done != slot->pct_done)
        {
            time = TIME_VALUE(round(total - elapsed)); //Remains
            StatusBar->SetStatusText(FFQSF(SID_TIME_REMAINS, time.ToReadableString()), 1);
            slot->pct_done = pct_done;
        }

        time = TIME_VALUE(round(total));
        total_time = time.ToReadableString();

    }

    //Set or clear tool tip for elapsed time
    if (tooltip)
    {

        time = TIME_VALUE(slot->process.GetRunningTimeMillis());
        StatusBar->SetToolTip(FFQSF(SID_TIME_ELAPSED_TOTAL, time.ToReadableString(), total_time));

    }
    else StatusBar->UnsetToolTip();


    //typedef enum QUEUE_STATUS { qsQUEUED, qsACTIVE, qsPASS1, qsPASS2, qsDONE, qsFAILED, qsABORTED, qsSKIPPED } QUEUE_STATUS;
    unsigned int cnt[QUEUE_STATUS_COUNT], items = ListView->GetItemCount();
    memset(&cnt, 0, sizeof(cnt));

    for (unsigned int i = 0; i < items; i++)
    {
        LPFFQ_JOB job = (LPFFQ_JOB)ListView->GetItemData(i);
        cnt[job->status]++;
    }

    s.Printf(FFQS(SID_STATUS_MESSAGE),
        items,
        cnt[qsQUEUED],
        cnt[qsACTIVE] + cnt[qsPASS1] + cnt[qsPASS2] + cnt[qsTHUMBS],
        cnt[qsDONE],
        cnt[qsFAILED],
        cnt[qsABORTED],
        cnt[qsSKIPPED]
    );
    StatusBar->SetStatusText(s, 2);

}

//---------------------------------------------------------------------------------------

wxString FFQMain::GetWindowPos()
{

    //Get the restored window position as a string that can be saved to cfg

    return wxString::Format("%i,%i,%i,%i,%i,%i",
                            m_RestoredRect.x, m_RestoredRect.y, m_RestoredRect.width, m_RestoredRect.height,
                            (int)IsMaximized(), SplitterWindow->GetSashPosition()
                           );

}

//---------------------------------------------------------------------------------------

void FFQMain::SetWindowPos(wxString &wp)
{

    //Set the window position from a string
    wxRect r;
    r.SetPosition(GetPosition());
    r.SetSize(GetSize());
    int m = (int)IsMaximized(), sp = SplitterWindow->GetSashPosition();

    r.x = Str2Long(GetToken(wp, ",", true), r.x);
    r.y = Str2Long(GetToken(wp, ",", true), r.y);
    r.width = Str2Long(GetToken(wp, ",", true), r.width);
    r.height = Str2Long(GetToken(wp, ",", true), r.height);
    m = Str2Long(GetToken(wp, ",", true), m);
    sp = Str2Long(GetToken(wp, ",", true), sp);

    SetSize(r);
    if (m == 1) Maximize();
    SplitterWindow->SetSashPosition(sp);

}

//---------------------------------------------------------------------------------------

void FFQMain::ShowFFMpegVersion(bool langInfo)
{
    if (FFQCFG()->ffmpeg_ok)
    {

        Console->AppendLine(FFQSF(SID_LOG_FFMPEG_FOUND, FFQCFG()->GetFFMpegVersion(true)), COLOR_BLUE);//, true);

        //Enable supported tools
        ThumbsItem->Enable(FFQCFG()->AreFiltersAvailable("select,fps,scale,tile"));
        SlideshowItem->Enable(FFQCFG()->AreFiltersAvailable("concat"));
        ConcatItem->Enable(FFQCFG()->AreFiltersAvailable("concat"));
        VidStabItem->Enable(FFQCFG()->AreFiltersAvailable("vidstabdetect,vidstabtransform"));

    }

    else Console->AppendLine(FFQS(SID_LOG_FFMPEG_NOT_FOUND), COLOR_RED);//, true);

    //Console->AppendLine("DUMMY", COLOR_BLUE);
    //ShowInfo(BOOLSTR(langInfo));

    if (langInfo)
    {
        FFQLang *l = FFQL();
        unsigned int skip = l->GetNumberOfSkippedStringsInFile(),
                     scnt = l->GetCount(),
                     snew = scnt - l->GetFlagCount(SF_STORED),
                     smod = l->GetFlagCount(SF_MODIFIED);
        //if ( (skip == 0) && (snew == scnt) ) //Check if internal language is used
        //{
            //Not internal
            if ( (snew != scnt) && (skip + snew + smod > 0) )
                Console->AppendLine(CRLF + FFQSF(SID_LANGUAGE_OUTDATED, snew, skip, smod), COLOR_ORANGE);
        //}
    }

    #ifdef DEBUG


        //Show a list of unsupported filters
        wxString sl = ",", fl = FFQCFG()->GetFFMpegFilters(), cur, res = "";
        for (unsigned int i = 0; i < FILTER_COUNT; i++) sl += FILTER_NAMES[i] + ",";
        while (fl.Len() > 0)
        {
            cur = GetToken(fl, ",", true);
            if ((cur.Len() > 0) && (sl.Find("," + cur + ",") == wxNOT_FOUND)) res += cur + CRLF;
        }
        Console->AppendLine(CRLF + "Unsupported filters:" + CRLF + res, COLOR_BLUE);
        res = FFQCFG()->GetFFMpegFormats();
        res.Replace("\n", CRLF);
        Console->AppendLine(CRLF + "Muxers" + CRLF + res + CRLF, COLOR_RED);
        TextCtrl->SetSelection(0, 0);
        TextCtrl->ShowPosition(0);

    #endif // DEBUG

}

//---------------------------------------------------------------------------------------

void FFQMain::ShowFFProbeInfo(LPFFQ_QUEUE_ITEM item)
{

    //Show ffprobe info for the selected item or the item given as argument

    if (item == NULL)
    {

        item = GetSelectedItem();
        if (item == NULL) return;

    }

    TextCtrl->Freeze();
    //Console->Clear();

    wxString fn;
    FFQProcess *p = new FFQProcess();

    try
    {

        for (unsigned int fidx = 0; fidx < item->inputs.Count(); fidx++)
        {

            fn = item->GetInput(fidx).path;
            if (wxFileExists(fn))
            {

                Console->AppendLine(FFQSF(SID_LOG_FFPROBE_INFO_FOR, fn)/* + CRLF*/, COLOR_BLACK);
                p->FFProbe(fn);
                Console->AppendLine(p->GetProcessOutput(false) + CRLF, COLOR_BLUE);

            }

        }

    } catch (std::exception &err)
    {

        Console->AppendLine(FFQSF(SID_LOG_FFPROBE_ERROR, fn, err.what()), COLOR_RED);

    }

    delete p;

    TextCtrl->Thaw();

}

//---------------------------------------------------------------------------------------

void FFQMain::BatchMakeJobs(wxArrayString *files, bool releaseFilesPtr)
{

    if ((files->GetCount() == 1) && wxFileExists(files->Item(0)))
        EditJob(-1, files->Item(0)); //If only one file is being batched - use the job editor

    else
    {
        if (BatchMaker == NULL) BatchMaker = new FFQBatchMake(this);
        BatchMaker->SetFiles(files);
        if (BatchMaker->Execute())
        {

            ListView->Freeze();
            for (long i = 0; i < ListView->GetItemCount(); i++) ListView->Select(i, false);
            for (unsigned int i = 0; i < BatchMaker->GetJobCount(); i++) DefineItem(-1, (LPFFQ_JOB)BatchMaker->GetJob(i), dsAPPEND, false);
            ListView->Thaw();
            BatchMaker->Clear();

            //Save items if required
            if (FFQCFG()->save_on_modify) SaveItems();

        }
    }

    if (releaseFilesPtr) delete files;

    UpdateControls();

}

//---------------------------------------------------------------------------------------

bool FFQMain::LaunchTool(short ToolID, long edit_index, LPFFQ_QUEUE_ITEM edit_item)
{

    wxString CommandSaveLog;

    if (ToolID == ID_TOOLTHUMBS)
    {

        //Make a thumbs and tiles command

        //Validate editable
        if ((edit_item != NULL) && (edit_item->GetItemType() != qtTHUMB_JOB)) return false;

        //Create tool
        if (ThumbMaker == NULL) ThumbMaker = new FFQThumbMake(this);

        //Set job pointer
        LPFFQ_THUMB_JOB job = (edit_item == NULL) ? new FFQ_THUMB_JOB() : (LPFFQ_THUMB_JOB)edit_item;

        //Launch tool
        if (ThumbMaker->Execute(job))
        {

            //Define the item
            DefineItem(edit_index, job, dsUNIQUE, true);
            return true;

        }

        //Tool canceled, delete job if it is new
        if (edit_item == NULL) delete job;

    }

    else if ((ToolID == ID_TOOLSLIDESHOW) || (ToolID == ID_TOOLCONCAT))
    {

        //Slide show / concatenation
        if ((edit_item != NULL) && (edit_item->GetItemType() != qtCONCAT_JOB)) return false;
        if (!ConcatDlg) ConcatDlg = new FFQConcat(this);

        LPFFQ_CONCAT_JOB job = (LPFFQ_CONCAT_JOB)edit_item;
        if (job == NULL)
        {
            job = new FFQ_CONCAT_JOB();
            job->slideshow = (ToolID == ID_TOOLSLIDESHOW);
        }
        if (ConcatDlg->Execute(job))
        {
            DefineItem(edit_index, job, dsUNIQUE, true);
            return true;
        }
        if (edit_item == NULL) delete job;

    }

    else if (ToolID == ID_TOOLVIDSTAB)
    {

        //VidStab
        if ((edit_item != NULL) && (edit_item->GetItemType() != qtVIDSTAB_JOB)) return false;
        if (!VidStabDlg) VidStabDlg = new FFQVidStab(this);
        LPFFQ_VIDSTAB_JOB job = (edit_item == NULL) ? new FFQ_VIDSTAB_JOB() : (LPFFQ_VIDSTAB_JOB)edit_item;
        if (VidStabDlg->Execute(job))
        {
            DefineItem(edit_index, job, dsUNIQUE, true);
            return true;
        }
        if (edit_item == NULL) delete job;


    }

    else if (ToolID == ID_TOOLVID2GIF)
    {

        //Video to GIF
        if ((edit_item != NULL) && (edit_item->GetItemType() != qtVID2GIF_JOB)) return false;
        if (!Video2Gif) Video2Gif = new FFQVideoToGIF(this);
        LPFFQ_VID2GIF_JOB job = (edit_item == NULL) ? new FFQ_VID2GIF_JOB() : (LPFFQ_VID2GIF_JOB)edit_item;
        if (Video2Gif->Execute(job))
        {
            DefineItem(edit_index, job, dsUNIQUE, true);
            return true;
        }
        if (edit_item == NULL) delete job;


    }

    return false;

}

//---------------------------------------------------------------------------------------

void FFQMain::OnChar(wxKeyEvent &event)
{

    //Convert a key event to a button or menu command event
    int cmd = -1, key = event.GetKeyCode();

    if ( ((key == WXK_ADD) || (key == WXK_INSERT) || (key == WXK_NUMPAD_ADD)) && ToolBarAdd->IsEnabled() ) cmd = ID_TOOLBARADD;

    else if ( ((key == WXK_SUBTRACT) || (key == WXK_DELETE) || (key == WXK_NUMPAD_SUBTRACT)) && ToolBarRemove->IsEnabled() ) cmd = ID_TOOLBARREMOVE;

    else if (event.ControlDown())
    {

        if ( ((key == WXK_RETURN) || (key == WXK_SPACE)) && ToolBarPreview->IsEnabled()) cmd = ID_TOOLBARPREVIEW;
        else if ( (key == WXK_UP) && MenuMoveUp->IsEnabled() ) cmd = ID_MENU_MOVEUP;
        else if ( (key == WXK_DOWN) && MenuMoveDown->IsEnabled() ) cmd = ID_MENU_MOVEDOWN;
        else if ( (wxChar(key) == 'Q') && MenuStartSel->IsEnabled() ) cmd = ID_MENU_STARTSEL;
        else if ( (wxChar(key) == 'A') && MenuStopSel->IsEnabled() ) cmd = ID_MENU_STOPSEL;
        else if ( (wxChar(key) == 'B') && ToolBarBatch->IsEnabled() ) cmd = ID_TOOLBARBATCH;
        else if ( (wxChar(key) == 'O') && ToolBarOptions->IsEnabled() ) cmd = ID_TOOLBAROPTIONS;
        else if ( (wxChar(key) == 'P') && ToolBarPresets->IsEnabled() ) cmd = ID_TOOLBARPRESETS;
        else if ( (wxChar(key) == 'T') && ToolBarTools->IsEnabled() ) cmd = ID_TOOLBARTOOLS;
        else if ( wxChar(key) == 'S' )
        {

            SaveItems(true);
            return;

        }

    }

    if (cmd < 0) event.Skip(); //No command, skip key event
    else
    {

        //Execute the converted command
        wxCommandEvent evt = wxCommandEvent(wxEVT_BUTTON);
        evt.SetId(cmd);
        OnToolBarButtonClick(evt);

    }


}

//---------------------------------------------------------------------------------------

void FFQMain::OnDropFiles(wxDropFilesEvent& event)
{

    //Files have been dropped onto the list, raise the frame
    Raise();

    //Get the dropped files
    wxString* fl = event.GetFiles();//, s;
    if (fl == NULL) return;

    //Convert files to an array
    wxArrayString* files = new wxArrayString();
    for (int i = 0; i < event.GetNumberOfFiles(); i++)
    {
        //Let batch-maker expand the paths
        files->Add(fl[i]);
    }

    //Launch the batch maker
    BatchMakeJobs(files, true);

}

//---------------------------------------------------------------------------------------

void FFQMain::OnIdle(wxIdleEvent &event)
{

    //This is not entirely ready yet, so it is not implemented in V1.7.62 yet!

    if ((!m_FirstShow) && m_FirstIdle)
    {

        m_FirstIdle = false; //This should only be done once after the window is shown..
        int fs_idx = -1;

        //Check command line for important messages from the local submarine
        for (int i = 1; i < wxTheApp->argc; i++)
        {
            wxString arg = wxTheApp->argv[i];
            if (arg.StartsWith("--test-fs="))
            {
                arg = arg.AfterFirst(EQUAL);
                FFQFullSpec::Initialize(this);
                fs_idx = FFQFullSpec::FindFullSpecID(arg);
                if (fs_idx < 0) Console->AppendLine(FFQSF(SID_FULLSPEC_BAD_ID, arg), COLOR_RED);
            }
            else Console->AppendLine(FFQSF(SID_BAD_COMMAND_LINE_ARG, arg), COLOR_RED);
        }

        //Respond to those submarine whisperings
        if (fs_idx >= 0)
        {

            bool fs_ok = true;
            wxString cmd;

            while (fs_ok)
            {

                //Run the spec's UI
                FFQFullSpec *fs = new FFQFullSpec(this);
                fs_ok = fs->Execute(fs_idx, cmd);
                delete fs;

                //Show the result
                Console->AppendLine(cmd, COLOR_BLUE);
                wxTheApp->Yield(true);

                if (fs_ok)
                {
                    //Forcefully reload Full Specs for next iteration
                    FFQFullSpec::Finalize();
                    FFQFullSpec::Initialize(this);
                }

            }

        }

    }

}

//---------------------------------------------------------------------------------------

void FFQMain::OnMaximize(wxMaximizeEvent &event)
{

    //Skip to let base class do the handling
    event.Skip();

    //Resize columns in list
    ResizeColumns();

}

//---------------------------------------------------------------------------------------

void FFQMain::OnMove(wxMoveEvent &event)
{

    //Skip to let base class do the handling
    event.Skip();

    //Save restored rect
    if ((!IsMaximized()) && (!IsIconized())) m_RestoredRect = GetScreenRect();

}

//---------------------------------------------------------------------------------------

#define NO_GETTEXTEXTENT

void FFQMain::OnShow(wxShowEvent &event)
{

    //Skip to let base class do the handling
    event.Skip();

    if (m_FirstShow)
    {

        try
        {
            //Do initial stuff for the first appearance
            FFQCFG()->GetTaskBar()->SetWindowHandle(GetHandle());
            //ResizeColumns();
            FFQCFG()->InitPresetManager(this);
            FFQCFG()->LoadConfig();
            InitEncodingSlots();
            ShowFFMpegVersion(true);
            LoadItems();
            if (FFQCFG()->validate_on_load) ValidateItems();
            UpdateControls();

            //wxString path;
            //if (wxGetEnv("PATH", &path)) Console->AppendLine(path, 0);

            /*Console->AppendLine("AppDocumentsDir    = " + wxStandardPaths::Get().GetAppDocumentsDir(), 0);
            Console->AppendLine("ConfigDir          = " + wxStandardPaths::Get().GetConfigDir(), 0);
            Console->AppendLine("DataDir            = " + wxStandardPaths::Get().GetDataDir(), 0);
            Console->AppendLine("DocumentsDir       = " + wxStandardPaths::Get().GetDocumentsDir(), 0);
            Console->AppendLine("ExecutablePath     = " + wxStandardPaths::Get().GetExecutablePath(), 0);
            Console->AppendLine("LocalDataDir       = " + wxStandardPaths::Get().GetLocalDataDir(), 0);
            Console->AppendLine("PluginsDir         = " + wxStandardPaths::Get().GetPluginsDir(), 0);
            Console->AppendLine("ResourcesDir       = " + wxStandardPaths::Get().GetResourcesDir(), 0);
            Console->AppendLine("TempDir            = " + wxStandardPaths::Get().GetTempDir(), 0);
            Console->AppendLine("UserConfigDir      = " + wxStandardPaths::Get().GetUserConfigDir(), 0);
            Console->AppendLine("UserDataDir        = " + wxStandardPaths::Get().GetUserDataDir(), 0);
            Console->AppendLine("UserLocalDataDir   = " + wxStandardPaths::Get().GetUserLocalDataDir(), 0);
            */

            /* SNAP
                AppDocumentsDir    = /home/tnb/
                ConfigDir          = /etc
                DataDir            = /snap/ffqueue/x2/usr/local/share/ffqueue
                DocumentsDir       = /home/tnb/
                ExecutablePath     = /snap/ffqueue/x2/usr/local/bin/ffqueue
                LocalDataDir       = /etc/ffqueue
                PluginsDir         = /snap/ffqueue/x2/usr/local/lib/ffqueue
                ResourcesDir       = /snap/ffqueue/x2/usr/local/share/ffqueue
                TempDir            = /tmp
                UserConfigDir      = /home/tnb/snap/ffqueue/x2
                UserDataDir        = /home/tnb/snap/ffqueue/x2/.ffqueue
                UserLocalDataDir   = /home/tnb/snap/ffqueue/x2/.ffqueue
            */


            //Console->AppendLine("MakeConfigFileName = " + wxStandardPaths::Get().MakeConfigFileName("cfg-file-name"), 0);

        #ifdef NO_GETTEXTEXTENT

            //Calculate the size of the status column, the dirty way - but it seems to work
            m_StatusColumnWidth = 0;
            for (unsigned int i = 0; i < QUEUE_STATUS_COUNT; i++)
            {
                int len = (int)FFQL()->QUEUE_STATUS_NAMES[i].Len();
                if (len > m_StatusColumnWidth) m_StatusColumnWidth = len;
            }
            m_StatusColumnWidth += 3; //Padding
            m_StatusColumnWidth *= ListView->GetFont().GetPixelSize().GetX();

            //Make sure that the column is at least an inch wide on screen
            wxClientDC dc(this);
            if (m_StatusColumnWidth < dc.GetPPI().GetX()) m_StatusColumnWidth = dc.GetPPI().GetX();

        #else

            //Using a DC to calculate the actual string width is borked - or at least
            //on Linux it is not working as expected since GetTextExtent returns string
            //widths which are much smaller than the actual size shown on screen. If
            //anyone has a fix for this, please say so.
            wxClientDC dc(this);
            dc.SetFont(ListView->GetFont());
            m_StatusColumnWidth = 0;
            for (unsigned int i = 0; i < QUEUE_STATUS_COUNT; i++)
            {
                wxSize s = dc.GetTextExtent(FFQL()->QUEUE_STATUS_NAMES[i]);
                if (s.GetWidth() > m_StatusColumnWidth) m_StatusColumnWidth = s.GetWidth();
            }
            m_StatusColumnWidth *= (m_StatusColumnWidth / 20); //Padding

        #endif

            if (FFQCFG()->save_window_pos) SetWindowPos(FFQCFG()->window_position);
            else ResizeColumns();
        }
        catch (...)
        {
        }

        //Make sure this is only done once
        m_FirstShow = false;

    }

}

//---------------------------------------------------------------------------------------

void FFQMain::OnFrameResize(wxSizeEvent& event)
{

    //Let the base class do the sizing
    wxFrame::OnSize(event);

    //Yield_App();

    //If the window is in a restored state, snatch the rect of it
    if ((!IsMaximized()) && (!IsIconized())) m_RestoredRect = GetScreenRect(); //Get restored size

    //Resize columns
    ResizeColumns();

}

//---------------------------------------------------------------------------------------

void FFQMain::OnListViewColumnBeginDrag(wxListEvent& event)
{

    //Dragging is only allowed for the first columns
    if ((!m_AllowEvents) && (event.GetColumn() > 0)) event.Veto();

    //Veto the resize if the resize is not cased by code
    //if (!m_AllowEvents) ResizeColumns(true);
    //if (!m_AllowEvents) event.Veto();
    //else event.Skip();

}

//---------------------------------------------------------------------------------------

void FFQMain::OnListViewColumnEndDrag(wxListEvent& event)
{
    //Re-calculate the remaining columns after dragging has ended
    if (!m_AllowEvents) ResizeColumns(true);
}

//---------------------------------------------------------------------------------------

void FFQMain::OnToolBarButtonClick(wxCommandEvent& event)
{

    //Get the ID of the tool button or menu item clicked
    int evtId = event.GetId();

    //Handle the event
    if (evtId == ID_TOOLBARADD)
    {

        //Edit a new job
        EditJob(-1);

    }

    else if (evtId == ID_TOOLBARBATCH)
    {

        if (OpenFilesDlg->ShowModal() != wxID_CANCEL)
        {

            wxArrayString *files = new wxArrayString();
            OpenFilesDlg->GetPaths(*files);
            BatchMakeJobs(files, true);

        }

    }

    else if ((evtId == ID_TOOLBARREMOVE) || (evtId == ID_MENU_REMOVE))
    {

        if ((!FFQCFG()->confirm_delete_jobs) || DoConfirm(ListView, FFQS(SID_DELETE_SELECTED)))
        {

            ListView->Freeze();

            long idx = 0;
            while (idx < ListView->GetItemCount())
            {
                if (ListView->IsSelected(idx) && (FindEncodingSlot(GetItemAtIndex(idx)) < 0)) DeleteItem(idx);// idx != IndexOfItem(m_CurrentItem))) DeleteItem(idx);
                else idx++;
            }

            ListView->Thaw();

            //Save items if required
            if (!FFQCFG()->save_on_modify) SaveItems();

        }

    }

    else if ((evtId == ID_TOOLBAREDIT) || (evtId == ID_MENU_EDIT))
    {

        LPFFQ_QUEUE_ITEM item = GetSelectedItem();

        switch (item->GetItemType())
        {

            case qtJOB: EditJob(ListView->GetFirstSelected()); break;
            case qtCONCAT_JOB: LaunchTool(ID_TOOLCONCAT, ListView->GetFirstSelected(), item); break;
            case qtTHUMB_JOB: LaunchTool(ID_TOOLTHUMBS, ListView->GetFirstSelected(), item); break;
            case qtVIDSTAB_JOB: LaunchTool(ID_TOOLVIDSTAB, ListView->GetFirstSelected(), item); break;
            case qtVID2GIF_JOB: LaunchTool(ID_TOOLVID2GIF, ListView->GetFirstSelected(), item); break;
            default: ShowError(FFQS(SID_ITEM_CANNOT_BE_EDITED)); return;

        }

    }

    else if ((evtId == ID_MENU_CLONE))
    {


        LPFFQ_QUEUE_ITEM item = GetSelectedItem();

        if (item->GetItemType() == qtJOB) EditJob(-1, "", GetSelectedItem());

        else
        {

            item = FFQ_QUEUE_ITEM::Clone(item);

            switch (item->GetItemType())
            {

                case qtCONCAT_JOB: if (!LaunchTool(ID_TOOLCONCAT, -1, item)) delete item; break;
                case qtTHUMB_JOB: if (!LaunchTool(ID_TOOLTHUMBS, -1, item)) delete item; break;
                case qtVIDSTAB_JOB: if (!LaunchTool(ID_TOOLVIDSTAB, -1, item)) delete item; break;
                case qtVID2GIF_JOB: if (!LaunchTool(ID_TOOLVID2GIF, -1, item)) delete item; break;
                default: ShowError(FFQS(SID_ITEM_CANNOT_BE_CLONED)); delete item; return;

            }

        }

    }

    else if (evtId == ID_MENU_PRESET)
    {

        long idx = ListView->GetFirstSelected();
        LPFFQ_JOB job = (LPFFQ_JOB)ListView->GetItemData(idx);
        LPFFQ_PRESET pst = FFQPresetMgr::Get()->GetPreset(job->preset_id.ToString());
        if ((pst != NULL) && FFQPresetMgr::Get()->EditPreset(pst)) PresetChanged(pst);// DefineItem(idx, job, dsNOSELECT);
        else if (pst == NULL) ShowError(ListView, FFQS(SID_JOB_HAS_NO_PRESET));

    }

    else if (evtId == ID_TOOLBARPREVIEW)
    {

        LPFFQ_QUEUE_ITEM item = GetSelectedItem();
        wxString cmd = item->GetPreviewCommand();
        if (cmd.Len() > 0)
        {
            if (IsPreviewSafe || (wxMessageBox(FFQS(SID_INLINE_SHELL_CODE_WARNING), FFQS(SID_WARNING), wxICON_WARNING|wxYES_NO) == wxYES))
                PreviewCommand(cmd, true);
        }

    }

    else if ((evtId == ID_TOOLBARSTART) || (evtId == ID_MENU_STARTALL) || (evtId == ID_MENU_STARTSEL))
    {

        StartQueue(evtId == ID_MENU_STARTSEL);

    }

    else if ((evtId == ID_TOOLBARSTOP) || (evtId == ID_MENU_STOPSEL))
    {

        ToolBar->EnableTool(ID_TOOLBARSTOP, false);
        MenuStopAll->Enable(false);
        MenuStopSel->Enable(false);
        StopQueue(evtId == ID_MENU_STOPSEL);
        return;

    }

    else if (evtId == ID_TOOLBARTOOLS)
    {

        #ifdef __WINDOWS__
        wxSize ts = ToolBar->GetToolSize(); //This will only return a proper tool size in Windows
        wxPoint mp = wxPoint((ts.GetWidth() * 7) + (ToolBar->GetToolSeparation() * 2), ts.GetHeight());
        #else
        wxPoint mp = ToolBar->ScreenToClient(wxGetMousePosition());
        #endif
        ToolBar->PopupMenu(&ToolsMenu, mp);

    }
    else if (evtId == ID_TOOLTHUMBS) LaunchTool(ID_TOOLTHUMBS);
    else if (evtId == ID_TOOLSLIDESHOW) LaunchTool(ID_TOOLSLIDESHOW);
    else if (evtId == ID_TOOLCONCAT) LaunchTool(ID_TOOLCONCAT);
    else if (evtId == ID_TOOLVIDSTAB) LaunchTool(ID_TOOLVIDSTAB);
    else if (evtId == ID_TOOLVID2GIF) LaunchTool(ID_TOOLVID2GIF);

    else if (evtId == ID_TOOLBARPRESETS) FFQPresetMgr::Get()->Execute();

    else if (evtId == ID_TOOLBAROPTIONS)
    {

        if (OptionsDlg == NULL) OptionsDlg = new FFQConfigEdit(this);

        if (OptionsDlg->Execute())
        {
            ShowFFMpegVersion(true);
            InitEncodingSlots();
        }

    }

    else if (evtId == ID_TOOLBARABOUT)
    {

        #ifdef DEBUG //This will be terminated in the "if (evtId == ID_DEBUG_ABOUT)" block, so watch out!

        #ifdef __WINDOWS__
        wxSize ts = ToolBar->GetToolSize();
        wxPoint mp = wxPoint((ts.GetWidth() * 10) + (ToolBar->GetToolSeparation() * 5), ts.GetHeight());
        #else
        wxPoint mp = ToolBar->ScreenToClient(wxGetMousePosition());
        #endif
        ToolBar->PopupMenu(m_DebugPopupMenu, mp);
        return;

    }

    else if (evtId == ID_DEBUG_MAKEFILES) //DEBUG ONLY!!
    {

        wxString result = CreateMakefiles();
        if (result.Len() != 0) Console->AppendLine(result, COLOR_RED);

    }

    else if (evtId == ID_DEBUG_BINRES) //DEBUG ONLY!!
    {

        if (MakeBinaryResources()) Console->AppendLine("Binary resources created", COLOR_GREEN);
        else Console->AppendLine("Binary resources not created properly", COLOR_RED);

    }

    else if (evtId == ID_DEBUG_TESTING) //DEBUG ONLY!!
    {

        //Handler used to test all sorts of madness

        /*const int ITEM_COUNT = 9;
        wxString DISPLAY[ITEM_COUNT] = {
            "Vacation 2023.mpg", //Job - preset
            "Night Recording.mkv", //Job - command
            "TV recording.mp4", //Thumbs
            "Shaking Stevens.mp4", //VidStab
            "Family Meme.mkv", //Vid2GIF
            "Image Serie A1.jpg", //Concat: Images To Video
            "List of Images.txt", //Concat: Images To Video
            "Segment 1.avi", //Concat: Concat
            "Paragliding 1.m4v", //Concat: Merge
        };
        wxString s;
        for (int i = 0; i < ITEM_COUNT; i++)
        {
            ListView->InsertItem(i, DISPLAY[i]);
            ListView->SetItemImage(i, i < 2 ? 0 : 1);
            QUEUE_STATUS qs;
            switch (i) {
                case 0: s = FFQSF(SID_PRESET, "BluRay compatible FullHD"); qs = qsPASS1; break;
                case 1: s = FFQSF(SID_COMMAND, "<%inputs%> -c:v copy -c:a aac -b:a 128k <%output%>"); qs = qsQUEUED; break;
                case 2: s = FFQSF(SID_TOOL_JOB, FFQS(SID_MAINFRAME_TM_THUMBTOOL)); qs = qsTHUMBS; break;
                case 3: s = FFQSF(SID_TOOL_JOB, FFQS(SID_VIDSTAB_TITLE)); qs = qsACTIVE; break;
                case 4: s = FFQSF(SID_TOOL_JOB, FFQS(SID_VIDEO2GIF_TITLE)); qs = qsDONE; break;
                case 5: s = FFQSF(SID_TOOL_JOB, FFQS(SID_CONCAT_VIDEO_FROM_IMAGES)); qs = qsFAILED; break;
                case 6: s = FFQSF(SID_TOOL_JOB, FFQS(SID_CONCAT_VIDEO_FROM_IMAGES)); qs = qsDORMANT; break;
                case 7: s = FFQSF(SID_TOOL_JOB, FFQS(SID_CONCAT_SIMPLE)); qs = qsABORTED; break;
                case 8: s = FFQSF(SID_TOOL_JOB, FFQS(SID_CONCAT_MERGE_FILES)); qs = qsQUEUED; break;
            }
            ListView->SetItem(i, 1, s);
            ListView->SetItem(i, 2, FFQL()->QUEUE_STATUS_NAMES[qs]);

        }
        SetSize(wxSize(1920, 1080));
        Console->Clear();
        Console->AppendWithTime(FFQSF(SID_LOG_JOB_TAB_STARTED, Consoles->GetPageText(2),  DISPLAY[0]), COLOR_BLUE);
        Console->AppendWithTime(FFQSF(SID_LOG_JOB_TAB_FAILED, Consoles->GetPageText(2)), COLOR_RED);
        Console->AppendWithTime(FFQSF(SID_LOG_JOB_TAB_STARTED, Consoles->GetPageText(1),  DISPLAY[1]), COLOR_BLUE);
        Console->AppendWithTime(FFQSF(SID_LOG_JOB_TAB_STARTED, Consoles->GetPageText(2),  DISPLAY[2]), COLOR_BLUE);
        Console->AppendWithTime(FFQSF(SID_LOG_JOB_TAB_ENDED, Consoles->GetPageText(1)), COLOR_BLUE);
        Console->AppendWithTime(FFQSF(SID_LOG_JOB_TAB_STARTED, Consoles->GetPageText(1),  DISPLAY[3]), COLOR_BLUE);
        Consoles->SetPageImage(1, CONSOLE_IMAGE_ON);
        SetTitle("FFQueue");
        s.Printf(FFQS(SID_STATUS_MESSAGE), ITEM_COUNT, 2, 3, 1, 1, 1, 0);
        StatusBar->SetStatusText(s, 2);
        StatusBar->SetStatusText(FFQS(SID_UNKNOWN_TIME_REMAINS), 1);
        return;*/

        /*wxString str = wxString::FromUTF8("abc\rdef\nght\nyui\r\nå\næ\n\n\n");
        FFQLineParser lp(str);
        while (lp.has_more()) Console->AppendLine("\"" + lp.next() + "\"", 0);
        //return;


        str = wxString::FromUTF8("abc|*|def|*|ght|*|yui|*|å|*|æ|*||*|");
        FFQTokenParser tp(str, "|*|");
        while (!tp.done()) Console->AppendLine("\"" + tp.next() + "\"", 0);
        return;*/

        /*LPFFQ_QUEUE_ITEM sel = GetSelectedItem();
        Console->AppendLine(sel->ToString(), 0);
        return;*/

        /*wxString test = "-Hello Dude! Hello Dude! Hello Dude! Hello Dude! Hello Dude! Hello Dude!-";
        uint32_t len = 0;
        uint8_t *buf = CompressString(test, &len, 7);
        Console->AppendLine("Compressed = " + ToStr(len), 0);
        test = DecompressString(buf, len, 7);
        Console->AppendLine("Decompressed = \"" + test + "\"", 0);
        delete[] buf;
        return;*/

        /*uint32_t buflen;
        memcpy(&buflen, STR_X264, sizeof(buflen));
        uint8_t *buf = new uint8_t[buflen];
        long unsigned int zs = buflen;
        uncompress(buf, &zs, &STR_X264[sizeof(buflen)], STR_X264_SIZE - sizeof(buflen));
        wxString spec(buf, buflen);
        delete[] buf;*/

        /*wxString test = "libx264 - H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10 (codec h264)";
        wxRegEx re("[[:blank:]][hx]264)$", wxRE_BASIC);
        if (!re.IsValid()) Console->AppendLine("Invalid regex", 0);
        else if (re.Matches(test)) Console->AppendLine("Matches: " + re.GetMatch(test), 0);
        else Console->AppendLine("No match", 0);*/

        //profile=main:keyint=25:no-cabac:vbv-init=0.99000:psy-rd=1.0,0.1
        //profile=main:preset=veryslow:tune=animation:keyint=25:b-pyramid=strict:no-cabac:pulldown=64:vbv-init=0.99000:aq-mode=1:direct=auto:weightp=1:me=umh:subme=5:psy-rd=1.0,0.1:trellis=1:overscan=show:videoformat=secam:fullrange=off:colorprim=bt470bg:transfer=linear:colormatrix=bt470bg:nal-hdr=vbr
        wxString cmd = "asm=SSE4,SSE4.1,SSE4.2";//-x264opts \"no-fast-intra:profile=main:preset=veryslow:tune=animation:keyint=25:b-pyramid=strict:no-cabac:pulldown=64:vbv-init=0.99000:aq-mode=1:direct=auto:weightp=1:me=umh:subme=5:psy-rd=1.0,0.1:trellis=1:overscan=show:videoformat=secam:fullrange=off:colorprim=bt470bg:transfer=linear:colormatrix=bt470bg:nal-hdr=vbr\"", spec;

        FFQFullSpec::Finalize();
        FFQFullSpec::Initialize(this);
        int idx = FFQFullSpec::FindFullSpec("gdg fsdfg x264)");
        Console->AppendLine("x264=" + ToStr(idx), 0);
        //int idx = FFQFullSpec::FindFullSpec("gdg fsdfg hevc)");
        //Console->AppendLine("x265=" + ToStr(idx), 0);
        if (idx >= 0)
        {
            FFQFullSpec *fs = new FFQFullSpec(this);
            if (fs->Execute(idx, cmd)) Console->AppendLine(cmd, 0);
            delete fs;
        }


        /*wxRect rct = m_RestoredRect;// GetScreenRect();
        wxSize siz = GetSize();
        Console->AppendLine(wxString::Format("%i %i %i %i | %i %i", rct.GetLeft(), rct.GetTop(), rct.GetWidth(), rct.GetHeight(), siz.GetWidth(), siz.GetHeight()), 0);
        wxString wp = GetWindowPos();
        Console->AppendLine(wp, 0);

        wxSize sz = wxDLG_UNIT(this, wxSize(10, 10));
        Console->AppendLine(wxString::Format("DLG_UNIT = %d, %d", sz.GetWidth(), sz.GetHeight()), 0);*/


        /*wxArrayString as;
        as.Add("kjh");
        as.Add("kjh");
        FFQBatchMake *bm = new FFQBatchMake(this);
        bm->SetFiles(&as);
        bm->Execute();
        delete bm;*/

        /*FFQ_VID2GIF_JOB v2gj;
        FFQVideoToGIF *v2g = new FFQVideoToGIF(this);
        while (v2g->Execute(&v2gj))
        {

            //Console->AppendLine(v2gj.ToString(), COLOR_BLACK);

            v2gj.PrepareProcessing();
            wxString cmd;
            while (v2gj.GetNextCommand(cmd)) Console->AppendLine(cmd, COLOR_BLACK);
            Console->AppendBlankLine();

        }
        delete v2g;*/

        /*FFQ_INPUT_FILE inf;
        inf.path = "/path/to/video.flv";
        FFQ_VID2GIF_JOB v2g;
        v2g.inputs.Add(inf.ToString());
        v2g.width = 200;
        v2g.height = 0;
        v2g.fps = 10;
        v2g.start_time = TIME_VALUE(85743);
        v2g.limit_len = TIME_VALUE(5000);
        v2g.out = "/path/to/result.gif";
        v2g.PrepareProcessing();
        wxString cmd;
        while (v2g.GetNextCommand(cmd)) Console->AppendLine(cmd, COLOR_BLACK);
        */


        /*wxString tag = "[0:0]";
        FFQ_CUTS cuts("1,40;0:0:10;0:0:20;0:0:30;0:0:39");
        int fid = 0;
        FormatCuts(cuts, tag, true, TIME_VALUE("0:0:40"), fid);
        Console->AppendLine(cuts.ToString(), COLOR_BLACK);*/

        /*
        LPFFQ_QUEUE_ITEM item = GetSelectedItem();
        if (item)
        {
            item->PrepareProcessing();
            wxString cmd;
            while (item->GetNextCommand(cmd)) Console->AppendLine(cmd, COLOR_BLACK);
        }
        item->status = qsDORMANT;
        */

        //FFQLangEdit::Get(this)->Execute();

        /*
        FFQJobEditAdv *jea = new FFQJobEditAdv(this);
        LPFFQ_JOB job = new FFQ_JOB();
        while (jea->Execute(job)) ;
        delete job;
        delete jea;
        */


        /*FFQFilterEdit *fe = new FFQFilterEdit(this);
        FFMPEG_FILTER fltr = FFMPEG_FILTER();
        fltr.type = ftVOLUME_AND_TONE;
        bool ok = true;
        while (ok)
        {
            ok = fe->Execute(&fltr);
            if (ok) ok = (wxMessageBox(fltr.ToString(), "Info", wxYES_NO) == wxYES);
        }
        delete fe;*/


    }

    else if (evtId == ID_DEBUG_FILTERS) //DEBUG ONLY!!
    {
        bool ok = true;
        unsigned int ftyp = 0;
        FFQFilterEdit *fe = new FFQFilterEdit(this);
        do
        {
            if ((ftyp == 4) || (ftyp == 30) || (ftyp == 42)) ftyp++;
            FFMPEG_FILTER fltr;
            fltr.type = (FILTER_TYPE)ftyp;
            try
            {
                ok = fe->Execute(&fltr);
            }
            catch (...)
            {
                ok = true;
            }
        }
        while (ok && (++ftyp < FILTER_COUNT));
        delete fe;
    }

    else if (evtId == ID_DEBUG_ABOUT) //DEBUG ONLY!!
    {

        #endif // DEBUG

        if (AboutBox == NULL) AboutBox = new FFQAbout(this);
        AboutBox->Execute();

    }

    else if (evtId == ID_MENU_MOVEUP) MoveItems(true);

    else if (evtId == ID_MENU_MOVEDOWN) MoveItems(false);

    else if (evtId == ID_MENU_FFPROBE) ShowFFProbeInfo();

    else if (evtId == ID_MENU_FFCMD)
    {

        //Dump the commands from the selected item to the console
        Console->AppendBlankLine();
        LPFFQ_QUEUE_ITEM item = GetSelectedItem();
        QUEUE_STATUS ss = item->status;
        item->PrepareProcessing();
        wxString cmd;
        while (item->GetNextCommand(cmd, false))
        {
            GetToken(cmd, ",", true); //Remove command length
            Console->AppendLine(cmd, COLOR_BLACK);
        }
        item->status = ss;

    }

    UpdateControls();

}

//---------------------------------------------------------------------------------------

void FFQMain::OnTimerTrigger(wxTimerEvent& event)
{

    //Queue timer

    //Always read process output & update status
    ProcessReadOutput();

    int restart = 0;

    for (int i = 0; i < m_NumEncodingSlots; i++)
    {
        LPENCODING_SLOT slot = &m_EncodingSlots[i];
        if (slot->item == NULL)
        {
            if (ProcessNext(slot)) restart++;
        }
        else if (!slot->process.IsProcessRunning())
        {
            if (slot->process.WasAborted()) AfterItemProcessing(slot);
            else if (ProcessNext(slot)) restart++;
        }
        else restart++;
    }

    //Restart timer or finish queue?
    if (restart > 0) Timer.Start(TIMER_INTERVAL, true); //Restart timer
    else FinishQueue(); //Finish queue

}

//---------------------------------------------------------------------------------------

void FFQMain::OnListViewItemSelection(wxListEvent& event)
{

    if (!ListView->IsFrozen())
    {
        if (ListView->GetSelectedItemCount() == 1) SelectConsole(NULL, 1 + FindEncodingSlot((LPFFQ_QUEUE_ITEM)event.GetData()));
        UpdateControls();
    }

}

//---------------------------------------------------------------------------------------

void FFQMain::OnListViewItemActivated(wxListEvent& event)
{

    //Relay edit event if item can be edited
    if (ToolBarEdit->IsEnabled())
    {

        wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED);
        evt.SetId(ID_TOOLBAREDIT);
        OnToolBarButtonClick(evt);

    }

}

//---------------------------------------------------------------------------------------

void FFQMain::OnClose(wxCloseEvent& event)
{

    if (m_EncodingActive)
    {

        ShowError(ListView, FFQS(SID_PLEASE_STOP_TASK));
        event.Veto();
        return;

    }

    //For some reason FFQ sometimes hangs on close, possibly
    //to a rouge yield call. To prevent this "hanging" from
    //wrecking settings and jobs we must ensure that the code
    //below is only executed once using the m_Closed variable:
    if (m_Closed) return;
    m_Closed = true;

    if (FFQCFG()->save_window_pos)
    {

        wxString wp = GetWindowPos();

        if (FFQCFG()->window_position != wp)
        {

            FFQCFG()->window_position = wp;
            FFQCFG()->SaveConfig();

        }

    }


    //Save jobs?
    if (FFQCFG()->save_on_exit) SaveItems();

    //Delete all items
    DeleteItem(-1);

    event.Skip();
    Destroy();

}

//---------------------------------------------------------------------------------------

void FFQMain::OnListViewItemRightClick(wxListEvent& event)
{

    //Show popup menu
    ListView->PopupMenu(&ListMenu);

}


void FFQMain::OnConsolesPageChanged(wxNotebookEvent& event)
{
    if (m_EncodingActive && (ListView->GetSelectedItemCount() < 2))
    {
        ListView->Freeze(); //To prevent the list change event
        LPFFQ_QUEUE_ITEM item = m_EncodingSlots[event.GetSelection() - 1].item;
        for (int i = 0; i < ListView->GetItemCount() - 1; i++)
        {
            bool sel = (LPFFQ_QUEUE_ITEM)ListView->GetItemData(i) == item;
            ListView->Select(i, sel);
            if (sel) ListView->EnsureVisible(sel);
        }
        ListView->Thaw();
    }
}
