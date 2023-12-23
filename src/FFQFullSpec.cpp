/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQFullSpec.cpp                                                 *
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

#include "FFQFullSpec.h"
#include "utils/FFQLang.h"
#include "utils/FFQMisc.h"
#include "utils/FFQConfig.h"
#include "utils/FFQConsole.h"
#include "utils/FFQParsing.h"
#include "utils/FFQCompress.h"
#include "bin_res.h"
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/display.h>
#include <wx/valnum.h>
#include <wx/regex.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/ffile.h>
#include <algorithm>

#ifndef WX_PRECOMP
	//  (*InternalHeadersPCH(FFQFullSpec)
	#include <wx/string.h>
	//  *)
#endif
//  (*InternalHeaders(FFQFullSpec)
//  *)

//  (*IdInit(FFQFullSpec)
const long FFQFullSpec::ID_OKBTN = wxNewId();
const long FFQFullSpec::ID_CANCELBTN = wxNewId();
//  *)

//BEGIN_EVENT_TABLE(FFQFullSpec,wxDialog)
	//  (*EventTable(FFQFullSpec)
	//  *)
//END_EVENT_TABLE()

//---------------------------------------------------------------------------------------

const wxString FULLSPEC_FILE_EXTENSION = ".ffqs";
const wxString FULLSPEC_PRESET = "#preset";
const wxString FULLSPEC_FILTER = "#filter";
const wxString RANGE_MIN = "min";
const wxString RANGE_MAX = "max";
const wxString FFQ_FULLSPEC_HEADER_1 = "FFQ_FULLSPEC_HEADER_1";
const wxString NUM_VAL_DEFAULT = "-10000 10000";
const wxString FS_INVALID_SPEC = "FFQFullSpec: The specification is missing information";
const wxString FS_INVALID = "FFQFullSpec: Invalid %s in \"%s\"";
const wxString FS_CYCLIC_REQUIRE = "FFQFullSpec: Cyclic require in field \"%s\"";
const wxString FS_LOG_TYPE[] = {"type", "field", "value", "header", "regex", "property", "augment"};
const wxString LFLF = "\n\n";
const uint8_t  FS_LOG_BAD_TYPE = 0;
const uint8_t  FS_LOG_BAD_FIELD = 1;
const uint8_t  FS_LOG_BAD_VALUE = 2;
const uint8_t  FS_LOG_BAD_HEADER = 3;
const uint8_t  FS_LOG_BAD_REGEX = 4;
const uint8_t  FS_LOG_BAD_PROPERTY = 5;
const uint8_t  FS_LOG_BAD_AUGMENT = 6;

//---------------------------------------------------------------------------------------

wxString GetCmdToken(wxString &from)
{
   size_t p = 0;
   wxString res = wxEmptyString;
   bool quote = false;
   while (p < from.Len())
   {
       wxUniChar cc = from.at(p++);
       if ((!quote) && ((cc == SPACE) || (cc == COLON) || (cc == EQUAL))) break;
       else if (cc == '"') quote = !quote;
       else res += cc;
   }
   from = from.Remove(0, p).Trim(false);
   return res;
}

//---------------------------------------------------------------------------------------

wxString GetFieldValue(FULLSPEC_FILE &file, FULLSPEC_FIELD &field)
{
    //if (!field.check->IsChecked()) return wxEmptyString;
    wxString res;
    wxTextEntry *te = dynamic_cast<wxTextEntry*>(field.ctrl);

    if (te) res = te->GetValue(); //string (wxTextEntry) entry or combo

    else if (field.type == ftCHOICE)
    {

        //Get the value from a choice
        wxChoice *ch = dynamic_cast<wxChoice*>(field.ctrl);
        int sel_idx = ch->GetSelection();
        FFQTokenParser tp(field.value, file.separator);
        while (sel_idx-- > 0) tp.next();
        if (tp.has_more()) res = tp.next().BeforeFirst(EQUAL);

    }

    else if (field.type == ftCHECKLIST)
    {

        //Get the values from a checklist
        wxCheckListBox *cl = dynamic_cast<wxCheckListBox*>(field.ctrl);
        FFQTokenParser tp(field.value, file.separator);
        for (int i = 0; i < (int)cl->GetCount(); i++)
        {
            wxString tok = tp.next();
            if (cl->IsChecked(i)) res += tok.BeforeFirst(EQUAL) + COMMA;
        }
        if (res.Len() > 0) res = res.RemoveLast();

    }

    #ifdef DEBUG
    else FFQConsole::Get()->AppendLine("FFQFullSpec: Bad field control \"" + field.text + "\"", COLOR_RED);
    #endif

    return res;

}

//---------------------------------------------------------------------------------------

int IndexOfField(FULLSPEC_FILE &file, wxString field_name)
{
    //Return the index of the field with field_name
    for (size_t i = 0; i < file.fields.size(); i++)
    {
        FULLSPEC_FIELD &field = file.fields[i];
        if (field.name == QUESTION)
        {
            FFQTokenParser tp(field.value, file.separator);
            while (tp.has_more()) if (tp.next().StartsWith(field_name + EQUAL)) return (int)i;
        }
        else if (file.fields[i].name == field_name) return (int)i;
    }
    //Field not found
    return wxNOT_FOUND;
}

//---------------------------------------------------------------------------------------

void LogSpecError(const uint8_t log_type, const wxString &line)//, const unsigned int line)
{
    FFQConsole::Get()->AppendLine(wxString::Format(FS_INVALID, FS_LOG_TYPE[log_type], line), COLOR_RED);
}

//---------------------------------------------------------------------------------------

void SetItemContainer(FULLSPEC_FILE &file, FULLSPEC_FIELD &field)
{
    wxItemContainer *ic = dynamic_cast<wxItemContainer*>(field.ctrl);
    wxString cur, val;
    FFQTokenParser tp(field.value, file.separator);
    while (tp.has_more())
    {
        cur = tp.next(); //Get an item
        if (cur.Find(EQUAL) > 0)
        {
            //The item has a value and a text
            val = GetToken(cur, EQUAL);
            if (val == field.def) field.defidx = tp.index() - 1;//ch->GetCount(); //Preselected index
        }
        else if (cur == field.def) field.defidx = tp.index() - 1;//ch->GetCount(); //Preselected index
        ic->Append(cur); //Add the items text
    }
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

bool FFQFullSpec::s_Initialized = false;
std::vector<FULLSPEC_FILE> FFQFullSpec::s_Files = std::vector<FULLSPEC_FILE>();
wxFont FFQFullSpec::s_HeaderFont = wxFont();

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQFullSpecEvtHandler::FFQFullSpecEvtHandler(wxWindow *parent, FULLSPEC_FILE *file) : wxEvtHandler()
{

    m_File = file;
    m_Parent = parent;
    parent->PushEventHandler(this);

	Bind(wxEVT_CHECKBOX, &FFQFullSpecEvtHandler::OnCommand, this);
	Bind(wxEVT_CHECKLISTBOX, &FFQFullSpecEvtHandler::OnCommand, this);
	Bind(wxEVT_CHOICE, &FFQFullSpecEvtHandler::OnCommand, this);
	Bind(wxEVT_TEXT, &FFQFullSpecEvtHandler::OnCommand, this);

}

//---------------------------------------------------------------------------------------

FFQFullSpecEvtHandler::~FFQFullSpecEvtHandler()
{
    m_Parent->RemoveEventHandler(this);
}

//---------------------------------------------------------------------------------------

wxWindow* FFQFullSpecEvtHandler::GetParent()
{
    return m_Parent;
}

//---------------------------------------------------------------------------------------


void FFQFullSpecEvtHandler::UpdateControls()
{

    //Enable all control with their checkboxes ticked
    //if (!m_CtrlsCreated) return;
    m_Parent->Freeze();
    for (int i = 0; i < (int)m_File->fields.size(); i++)
    {
        FULLSPEC_FIELD &field = m_File->fields[i];
        if (field.type != ftHEADER) UpdateField(field);
    }
    m_Parent->Thaw();

}

//---------------------------------------------------------------------------------------

void FFQFullSpecEvtHandler::OnCommand(wxCommandEvent &event)
{

    //if (event.GetEventType() == wxEVT_CHECKBOX) FFQConsole::Get()->AppendLine("Checkbox intercepted", COLOR_BLUE);
    //else if (event.GetEventType() == wxEVT_BUTTON) FFQConsole::Get()->AppendLine("Button intercepted", COLOR_BLUE);
    //else FFQConsole::Get()->AppendLine("Event: " + ToStr(event.GetEventType()) + " intercepted", COLOR_BLUE);

    //Some control has changed its state
    if (event.GetEventType() == wxEVT_CHECKLISTBOX)
    {
        //Make sure that at least one item is checked
        wxCheckListBox *cl = dynamic_cast<wxCheckListBox*>(event.GetEventObject());
        if (cl)
        {
            wxArrayInt aint;
            cl->GetCheckedItems(aint);
            if (aint.GetCount() == 0) cl->Check(event.GetInt(), true);
        }
    }

    UpdateControls();

    //event.Skip();
}

//---------------------------------------------------------------------------------------

void FFQFullSpecEvtHandler::UpdateField(FULLSPEC_FIELD &field)
{

    //Make sure that we are not running in circles..
    if (m_UpdateStack.Index(field.name) >= 0)
    {

        FFQConsole::Get()->AppendLine(wxString::Format(FS_CYCLIC_REQUIRE, field.name), COLOR_RED);
        return;

    }
    else if (field.require == EXCLAM)
    {

        //Required field
        //field.label.Enable(true);
        //field.ctrl.Enable(true);

    }
    else
    {

        //Prevent circles..
        m_UpdateStack.Add(field.name);

        bool enable = true;
        FFQTokenParser tp(field.require, m_File->separator);

        while (enable && tp.has_more())
        {

            //Get required field name and value
            wxString v = tp.next(), n = GetToken(v, EQUAL, true);

            //Get required check state
            bool inverse = n.StartsWith(EXCLAM);
            if (inverse) n = n.AfterFirst(EXCLAM);

            //Get index of field
            int idx = IndexOfField(*m_File, n);

            //If the field is not found, it is ignored
            if (idx < 0) continue;

            FULLSPEC_FIELD &ff = m_File->fields[idx];

            //Make sure that the required field has its requirements met
            UpdateField(ff);

            if (ff.is_enabled()) //ff.check->IsEnabled())
            {

                //Validate the field value if applied
                if (v.Len() > 0)
                {

                    if (ff.is_checked()) //ff.check->IsChecked())
                    {
                        if (!LogicCompare(GetFieldValue(*m_File, ff), v)) enable = false;
                        //if (inverse == (v == GetFieldValue(ff))) enable = false;
                    }
                    else enable = false;

                }

                //Else validate checkbox state
                else if (ff.is_checked() /*check->IsChecked()*/ == inverse) enable = false;

            }

            //Field is not available
            else enable = false;

        }

        if (field.check == nullptr) field.label->Enable(enable);
        else field.check->Enable(enable);
        if (field.ctrl) field.ctrl->Enable(enable && field.is_checked()); //field.check->IsChecked());

        //Remove field from update stack
        m_UpdateStack.RemoveAt(m_UpdateStack.Count() - 1);

    }

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void FFQFullSpec::ClearControlsFor(FULLSPEC_FILE &file, bool delete_evt_handler)
{

    //Remove all pointers to controls which are about to be destroyed
    for (size_t idx = 0; idx < file.fields.size(); idx++)
    {
        FULLSPEC_FIELD &field = file.fields[idx];
        field.check = nullptr;
        field.ctrl = nullptr;
        field.label = nullptr;
    }

    //Delete the event handler as well?
    if (delete_evt_handler && file.evt_handler)
    {
        delete file.evt_handler;
        file.evt_handler = nullptr;
    }

}

//---------------------------------------------------------------------------------------

int FFQFullSpec::FindFullSpec(wxString for_codec)
{
    //Find codec full spec by regex match
    for (size_t i = 0; i < s_Files.size(); i++)
    {
        FULLSPEC_FILE &file = s_Files[i];
        if ((file.rx_match != nullptr) && file.rx_match->Matches(for_codec)) return (int)i;
    }
    return wxNOT_FOUND;
}

//---------------------------------------------------------------------------------------

int FFQFullSpec::FindFullSpecFilter(int from_index)
{
    //Find the next full spec filter
    for (size_t i = from_index; i < s_Files.size(); i++) if (s_Files[i].id == FULLSPEC_FILTER) return (int)i;
    return wxNOT_FOUND;
}

//---------------------------------------------------------------------------------------

int FFQFullSpec::FindFullSpecFilter(wxString filter_name)
{
    //Find full spec for a filter name
    for (size_t i = 0; i < s_Files.size(); i++)
    {
        FULLSPEC_FILE &file = s_Files[i];
        if ((file.id == FULLSPEC_FILTER) && (file.composite == filter_name)) return (int)i;
    }
    return wxNOT_FOUND;
}

//---------------------------------------------------------------------------------------

int FFQFullSpec::FindFullSpecID(wxString for_id)
{
    for (size_t i = 0; i < s_Files.size(); i++) if (s_Files[i].id == for_id) return (int)i;
    return wxNOT_FOUND;
}

//---------------------------------------------------------------------------------------

void FFQFullSpec::Finalize()
{
    if (s_Initialized)
    {
        for (size_t i = 0; i < s_Files.size(); i++)
        {
            FULLSPEC_FILE &file = s_Files[i];
            delete file.rx_match;
            delete file.body;
            file.fields.clear();
        }
        /*std::for_each(s_Files.begin(), s_Files.end(), [](const FULLSPEC_FILE &file) {
            delete file.rx_match;
            delete file.body;
            file.fields.clear();
        });*/
        s_Files.clear();
        s_Initialized = false;
    }
}

//---------------------------------------------------------------------------------------

bool FFQFullSpec::GetCommandLine(FULLSPEC_FILE &file, wxString &cmd, wxUniChar command_quote, wxUniChar value_quote)
{

    //Build a command line from the fields..

    bool composite = file.composite.Len() > 0;
    cmd.Clear();

    for (size_t i = 0; i < file.fields.size(); i++)
    {
        //Get a reference to the field and continue if the field is a header or un-checked
        FULLSPEC_FIELD &field = file.fields[i];
        if ((field.type == ftHEADER) || (!field.is_enabled()/* check->IsEnabled()*/) || (!field.is_checked() /*check->IsChecked()*/)) continue;

        //Get the value from the field, set to checkbox value if no control
        wxString val = field.ctrl ? GetFieldValue(file, field) : file.checkval;

        //Add field name to result
        if (field.name == QUESTION)
        {

            cmd += (val.Len() > 0) ? file.prefix + val : "";
            if (file.checkval.Len() > 0) cmd += EQUAL + file.checkval;

        }
        else
        {

            cmd += file.prefix + field.name;
            //val = StrTrim(val);

            if (val.Len() > 0)
            {

                //If a value exists, add the formatted value and the proper separators
                cmd += composite ? EQUAL : SPACE;
                if ((value_quote != 0) && (val.Find(SPACE) >= 0)) cmd += value_quote + val + value_quote;
                else cmd += val;

            }

        }

        //Terminate this field with a separator
        cmd += composite ? COLON : SPACE;

    }

    if (cmd.Len() > 0)
    {

        //Remove last separator
        cmd.RemoveLast();

        //Add the composite argument and the value list
        if (composite)
        {
            if (command_quote == 0) cmd = file.composite + SPACE + cmd;
            else cmd = file.composite + SPACE + command_quote + cmd + command_quote;
        }

    }

    return true;

}

//---------------------------------------------------------------------------------------

FULLSPEC_FILE* FFQFullSpec::GetFullSpec(int index)
{
    return ((index < 0) || (index >= (int)s_Files.size())) ? nullptr : &s_Files[index];
}

//---------------------------------------------------------------------------------------


void FFQFullSpec::Initialize(wxWindow *wnd)
{
    if (s_Initialized) return;
    s_Initialized = true;
    s_HeaderFont = wxFont(wnd->GetFont()).MakeLarger().MakeBold();

    wxString fn;

    #ifdef DEBUG

    //Load the specs as resource files
    fn = "res/x264" + FULLSPEC_FILE_EXTENSION;
    if (!wxFileExists(fn)) fn = "../../" + fn;
    FullSpecsFromDir(fn.BeforeLast(wxFileName::GetPathSeparator()));

    /*ff.Open(fn, FILE_MODE);
    ff.ReadAll(&spec);
    ff.Close();
    FullSpecFileFromString(spec); //264

    fn.Replace("x264", "x265");
    ff.Open(fn, FILE_MODE);
    //spec.Clear();
    ff.ReadAll(&spec);
    ff.Close();
    FullSpecFileFromString(spec); //265*/

    #else

    //Load the specs from internal resources

    wxString spec = DecompressString((unsigned char*)STR_X264, STR_X264_SIZE);
    FullSpecFileFromString(spec);

    spec = DecompressString((unsigned char*)STR_X265, STR_X265_SIZE);
    FullSpecFileFromString(spec);

    #endif // DEBUG

    //Load all specs from the program directory
    fn = wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator());
    //FFQConsole::Get()->AppendLine(fn, COLOR_BLUE);
    FullSpecsFromDir(fn);

    //Load all specs from the config directory
    wxString cfg = FFQConfig::GetInstance()->GetConfigPath();
    //FFQConsole::Get()->AppendLine(cfg, COLOR_BLUE);
    if (cfg.compare(fn) != 0) FullSpecsFromDir(cfg);

}

//---------------------------------------------------------------------------------------


/*

    //Create the controls used to define the fields
    if (m_CtrlsCreated) return;

    m_CtrlSizer = new wxFlexGridSizer(m_File->fields.size(), 1, 0, 0);
    m_HeaderFont = wxFont(GetFont()).MakeLarger().MakeBold();
    std::for_each(m_File->fields.begin(), m_File->fields.end(), [this](FULLSPEC_FIELD &field) { AddFieldCtrl(field); });

    ScrollWin->SetSizerAndFit(m_CtrlSizer);
	MainSizer->Fit(this);

    //Update title and scroll bars
    //SetTitle(FFQSF(SID_FULLSPEC_TITLE, m_File->display));

    //Update scroll bars based on the height of a checkbox
    for (size_t i = 0; i < m_File->fields.size(); i++)
    {

        FULLSPEC_FIELD &field = m_File->fields[i];
        if (field.check)
        {

            wxSize sz = field.check->GetSize();
            ScrollWin->SetScrollbars(sz.GetHeight(), sz.GetHeight(), 3, 3, 0, 0);
            break;

        }

    }

    m_CtrlsCreated = true;

*/

bool FFQFullSpec::MakeControlsFor(FULLSPEC_FILE &file, wxWindow *parent, wxSize *max_size)
{

    //Do not create controls if an event handler is already bound
    if (file.evt_handler) return false;

    //Make sure that the body has been parsed
    ParseSpecBody(file);

    //FFQConsole::Get()->AppendLine(ToStr(file.fields.size()), 0);

    //Common controls
    wxFlexGridSizer *fgs = nullptr;
    wxScrolledWindow *swin = nullptr;
    wxNotebook *nbook = nullptr;

    //Used to adjust sizes
	wxDisplay d(wxDisplay::GetFromWindow(parent));
	wxRect sz = d.GetClientArea();
	wxSize swmax = max_size ? *max_size : wxSize(int(float(sz.GetWidth()) * 0.75), int(float(sz.GetHeight()) * (file.notebook ? 0.65 : 0.75)));

    auto new_swin = [&swin, /*&nbook,*/ swmax](wxWindow *parent)
    {
        //Used to uniformly create a ScrolledWindow
        swin = new wxScrolledWindow(parent);//, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL);
        swin->SetMaxSize(swmax);
        //swin->ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
        swin->SetScrollbars(25, 25, 3, 3, 0, 0); ///TODO: Use height of a checkbox!
    };

    if (file.notebook)
    {

        //Calculate number of controls per page
        wxArrayInt page_sizes;
        int page_size = 0;
        for (size_t idx = 0; idx < file.fields.size(); idx++)
        {
            FULLSPEC_FIELD &field = file.fields[idx];
            if (field.type == ftHEADER)
            {
                if (page_size > 0) page_sizes.Add(page_size);
                page_size = 0;
            }
            else page_size++;
        }
        if (page_size > 0) page_sizes.Add(page_size);

        //Create the notebook control
        nbook = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
        wxString page_name;
        int page_idx = 0;

        auto add_page = [&page_name, &swin, &nbook, &fgs]()
        {
            //Add the current page to the notebook control
            swin->SetSizerAndFit(fgs);
            fgs->SetSizeHints(swin);
            nbook->AddPage(swin, page_name.Len() == 0 ? "???" : page_name);
            fgs = nullptr;
        };

        for (size_t idx = 0; idx < file.fields.size(); idx++)
        {

            FULLSPEC_FIELD &field = file.fields[idx];

            if (field.type == ftHEADER)
            {

                if (fgs != nullptr) add_page();
                page_name = field.text;

            }
            else
            {

                //Create containers for the page?
                if (fgs == nullptr)
                {

                    new_swin(nbook);
                    fgs = new wxFlexGridSizer(page_sizes[page_idx++], 1, 0, 0);
                    fgs->AddGrowableCol(0);

                }

                MakeControlFor(file, field, swin, fgs);

            }


        }

        if (fgs != nullptr) add_page();
        file.evt_handler = new FFQFullSpecEvtHandler(nbook, &file);

    }
    else
    {

        new_swin(parent);

        fgs = new wxFlexGridSizer(file.fields.size(), 1, 0, 0);
        fgs->AddGrowableCol(0);

        //Create the controls
        for (size_t idx = 0; idx < file.fields.size(); idx++)
        {
            FULLSPEC_FIELD &field = file.fields[idx];
            MakeControlFor(file, field, swin, fgs);
        }

        swin->SetSizerAndFit(fgs);
        fgs->SetSizeHints(swin);
        file.evt_handler = new FFQFullSpecEvtHandler(swin, &file);

    }

    return true;

}

//---------------------------------------------------------------------------------------

int FFQFullSpec::FullSpecFileFromString(wxString &spec)
{

    FULLSPEC_FILE file;
    FFQLineParser lp(spec);
    wxString v = lp.first();
    if (v == FFQ_FULLSPEC_HEADER_1) file.version = 1;
    else
    {
        LogSpecError(FS_LOG_BAD_HEADER, v);
        return -1;
    }
    bool done = false;
    wxString augment;
    while ((!done) && lp.has_more())
    {
        v = StrTrim(lp.next());
        if (v.Len() == 0) done = true;
        else if (v[0] != HASH)
        {
            wxString k; //= GetToken(v, EQUAL, true);
            if (StrSplit(v, k, EQUAL))
            {
                if (k == "augment") augment = v;
                else if (k == "checkval") file.checkval = v;
                else if (k == "composite") file.composite = v;
                else if (k == "display") file.display = v;
                else if (k == "extra") file.extra = v;
                else if (k == "id") file.id = v;
                else if (k == "matches") file.matches = v;
                else if (k == "prefix") file.prefix = v;
                else if (k == "separator") file.separator = v;
                else if (k == "notebook") file.notebook = (v == STR_YES);
                else LogSpecError(FS_LOG_BAD_PROPERTY, lp.last());
            }
            else LogSpecError(FS_LOG_BAD_VALUE, lp.last());
        }
    }

    if (!done)
    {
        FFQConsole::Get()->AppendLine(FS_INVALID_SPEC, COLOR_RED);
        return -1; //End of header not found
    }

    //Prepare for special ID's
    bool filter = (file.id == FULLSPEC_FILTER);
    bool preset = filter ? false : (file.id == FULLSPEC_PRESET);

    if (filter) file.prefix = file.separator = wxEmptyString;
    else if (preset) file.matches = wxEmptyString;

    //Find the index of a full spec that must be augmented
    int idx = wxNOT_FOUND;
    if (augment.Len() > 0)
    {
        if (filter) idx = FindFullSpecFilter(augment);
        else idx = FindFullSpecID(augment);
        if (idx == wxNOT_FOUND)
        {
            LogSpecError(FS_LOG_BAD_AUGMENT, augment);
            return wxNOT_FOUND;
        }
    }

    //We must either have an ID match for an existing specification to
    //augment or a specification with valid required fields
    bool spec_ok = (file.id.Len() > 0) && (file.display.Len() > 0) && (preset || (file.matches.Len() > 0));

    if ((idx >= 0) || spec_ok)
    {

        if ((!filter) && (file.matches.Len() > 0))
        {

            //Validate RegEx for matching if not filter
            file.rx_match = new wxRegEx(file.matches, wxRE_EXTENDED);

            if (!file.rx_match->IsValid())
            {
                LogSpecError(FS_LOG_BAD_REGEX, file.matches);
                delete file.rx_match;
                return wxNOT_FOUND;
            }

        }

        //Check separator
        if (file.separator.Len() == 0) file.separator = COLON;

        if (idx >= 0)
        {

            //Override existing spec
            FULLSPEC_FILE &ff = s_Files[idx];
            if (file.checkval.Len() > 0) ff.checkval = file.checkval;
            if ((!filter) && (file.composite.Len() > 0)) ff.composite = file.composite;
            if (file.display.Len() > 0) ff.display = file.display;
            if (file.extra.Len() > 0) ff.extra = file.extra;
            if (file.matches.Len() > 0)
            {
                if (ff.rx_match) delete ff.rx_match;
                ff.matches = file.matches;
                ff.rx_match = file.rx_match;
            }
            if (file.prefix.Len() > 0) ff.prefix = file.prefix;
            ff.separator = file.separator;

            //Append the remaining spec to existing body
            *ff.body += LFLF + lp.rest() + LFLF + LFLF;
            return idx;

        }
        else
        {

            //Check if we need to replace an existing spec
            idx = filter ? FindFullSpecFilter(file.composite) : FindFullSpecID(file.id);
            if (idx >= 0)
            {
                FULLSPEC_FILE &del = s_Files[idx];
                delete del.body;
                if (del.rx_match) delete del.rx_match;
                del.fields.clear();
                s_Files.erase(s_Files.begin() + idx);
            }

            //Add the new spec
            file.body = new wxString(lp.rest() + LFLF + LFLF); //Save the rest for later
            file.evt_handler = nullptr;
            s_Files.push_back(file);
            return (int)s_Files.size() - 1;

        }
    }
    FFQConsole::Get()->AppendLine(FS_INVALID_SPEC, COLOR_RED);
    return -1;
}

//---------------------------------------------------------------------------------------

void FFQFullSpec::FullSpecsFromDir(wxString dir)
{

    //Enum and load all specs from a direcotry
    wxArrayString files;
    wxDir::GetAllFiles(dir, &files, "*" + FULLSPEC_FILE_EXTENSION, wxDIR_FILES);

    #ifdef DEBUG
    //FFQConsole::Get()->AppendLine("FullSpecsFromDir:", COLOR_BLUE);
    #endif // DEBUG

    for (size_t i = 0; i < files.Count(); i++)
    {

        #ifdef DEBUG
        //FFQConsole::Get()->AppendLine(files[i], COLOR_BLUE);
        #endif // DEBUG

        wxString spec;
        wxFFile ff;
        try
        {

            ff.Open(files[i], "r");
            ff.ReadAll(&spec);
            FullSpecFileFromString(spec);

        }
        catch (std::exception &err)
        {

            FFQConsole::Get()->AppendLine(err.what(), COLOR_RED);

        }

    }

}

//---------------------------------------------------------------------------------------

void FFQFullSpec::MakeControlFor(FULLSPEC_FILE &file, FULLSPEC_FIELD &field, wxWindow *parent, wxSizer *sizer)
{

    if (field.type == ftHEADER)
    {

        //Header
        wxStaticText *st = new wxStaticText(parent, wxID_ANY, field.text);
        st->SetFont(s_HeaderFont);
        //sizer->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
        sizer->Add(st, 1, wxALL|wxEXPAND, 5);
        field.ctrl = st;

    }
    else
    {

        //Create a tool tip for the check / label
        wxString tt = field.name;
        if (field.range.Len() > 0) tt += ", range: " + field.range.BeforeFirst(SPACE) + ".." + field.range.AfterFirst(SPACE);
        if (field.def.Len() > 0) tt += ", default: " + field.def;

        if ((field.type == ftCHECK) || (field.require != EXCLAM))
        {
            //Type is check or field is not required
            field.check = new wxCheckBox(parent, wxID_ANY, field.text);
            field.check->SetToolTip(tt);
            //if (field.hide) field.check->SetForegroundColour(*wxRED);
        }
        else
        {
            //Field is not check and required
            field.label = new wxStaticText(parent, wxID_ANY, field.text);
            field.label->SetToolTip(tt);
            //if (field.hide) field.label->SetForegroundColour(*wxRED);
        }


        if (field.type == ftCHECK)
        {
            sizer->Add(field.check, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
            return;
        }

        else if (field.type == ftCHOICE)
        {

            //Create a choice control and parse the values for it
            wxChoice *ch = new wxChoice(parent, wxID_ANY);
            field.ctrl = ch;
            SetItemContainer(file, field);
            ch->SetSelection(field.defidx);

        }
        else if (field.type == ftFLOAT)
        {

            //Create a text control
            wxTextCtrl *tc = new wxTextCtrl(parent, wxID_ANY);
            field.ctrl = tc;

            //Attach a floating point validator
            wxFloatingPointValidator<float> val(5, NULL);//, wxNUM_VAL_ZERO_AS_BLANK);
            tc->SetValidator(val);

            //Set a valid input range
            wxString r = field.range.Len() > 0 ? field.range : NUM_VAL_DEFAULT;
            wxString m = GetToken(r, SPACE);
            val.SetMin(m == RANGE_MIN ? std::numeric_limits<float>::min() : Str2Float(m, 0));
            val.SetMax(r == RANGE_MAX ? std::numeric_limits<float>::max() : Str2Float(r, 0));

            //Set default value
            tc->ChangeValue(field.def);

        }

        else if (field.type == ftINTEGER)
        {

            //Same as with float, just for integers..
            wxTextCtrl *tc = new wxTextCtrl(parent, wxID_ANY);
            field.ctrl = tc;
            wxIntegerValidator<int> val(NULL);//, wxNUM_VAL_ZERO_AS_BLANK);
            tc->SetValidator(val);

            //Set a valid input range
            wxString r = field.range.Len() > 0 ? field.range : NUM_VAL_DEFAULT;
            wxString m = GetToken(r, SPACE);
            val.SetMin(m == RANGE_MIN ? std::numeric_limits<int>::min() : Str2Long(m, 0));
            val.SetMax(r == RANGE_MAX ? std::numeric_limits<int>::max() : Str2Long(r, 0));

            //Set default value
            tc->ChangeValue(field.def);

        }

        else if (field.type == ftSTRING)
        {

            //Simple text control / input - validated in ActionClick()
            wxTextCtrl *tc = new wxTextCtrl(parent, wxID_ANY);
            field.ctrl = tc;
            tc->ChangeValue(field.def);

        }

        else if (field.type == ftCOMBO)
        {

            //Create a combobox control and parse the values for it
            wxComboBox *cb = new wxComboBox(parent, wxID_ANY);
            field.ctrl = cb;
            SetItemContainer(file, field);
            cb->SetSelection(field.defidx);

        }

        else if (field.type == ftCHECKLIST)
        {

            //Create a checklist control and parse the values for it
            wxCheckListBox *cl = new wxCheckListBox(parent, wxID_ANY);

            ///TODO: Fiks dette..!!
            //cl->SetMaxSize(wxSize(-1, OkBtn->GetSize().GetHeight() * 3));

            field.ctrl = cl;
            SetItemContainer(file, field);
            if (field.def == "*") for (int i = 0; i < (int)cl->GetCount(); i++) cl->Check(i, true);
            else cl->Check(field.defidx, true);
        }

        else return; //Something went sour..

        //Create a sizer and add the control before adding the sizer to the control sizer..
        wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 2, 0, 0);
        fgs->AddGrowableCol(0);
        if (field.check == nullptr) fgs->Add(field.label, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        else fgs->Add(field.check, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        fgs->Add(field.ctrl, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
        //fgs->Add(field.ctrl, wxALL|wxEXPAND, 0);
        sizer->Add(fgs, 1, wxALL|wxEXPAND, 3);

    }

}

//---------------------------------------------------------------------------------------

void FFQFullSpec::ParseSpecBody(FULLSPEC_FILE &file)
{

    //This will parse the body of the specification text
    //and convert it to a list of fields

    if (file.body == nullptr) return;

    //FFQConsole::Get()->AppendLine(*file.body, 0);

    FULLSPEC_FIELD_TYPE type = ftHEADER;
    wxString key, val, name, text, value, range, def, require, needed, hide;

    FFQLineParser lp(*file.body);

    do
    {

        //Get a line
        val = StrTrim(lp.next());

        //Process if not empty
        if (val.Len() > 0)
        {

            //Skip comment?
            if (val[0] == HASH) continue;

            //Section header?
            if (val[0] == COLON) file.fields.push_back(FULLSPEC_FIELD(StrTrim(val.SubString(1, val.Len()))));
            else
            {

                //Split key / value
                if (StrSplit(val, key, EQUAL))
                {

                    if (key == "text")
                    {
                        text = val;
                        type = ftCHECK; //The default field type
                    }
                    else if (key == "type")
                    {

                        if (val == "check") type = ftCHECK;
                        else if (val == "choice") type = ftCHOICE;
                        else if (val == "float") type = ftFLOAT;
                        else if (val == "integer") type = ftINTEGER;
                        else if (val == "string") type = ftSTRING;
                        else if (val == "combo") type = ftCOMBO;
                        else if (val == "checklist") type = ftCHECKLIST;
                        else LogSpecError(FS_LOG_BAD_TYPE, lp.last());

                    }
                    else if (key == "value") value = val;
                    else if (key == "range") range = val;
                    else if (key == "default") def = val;
                    else if (key == "require") require = val;
                    else if (key == "needed") needed = val;
                    else if (key == "hide") hide = val;
                    else LogSpecError(FS_LOG_BAD_FIELD, lp.last());

                }
                else
                {

                    name = StrTrim(val);
                    if (name.Len() == 0) LogSpecError(FS_LOG_BAD_FIELD, lp.last());

                }

            }

        }

        else if (name.Len() > 0)
        {

            int idx = IndexOfField(file, name);
            if (idx >= 0)
            {

                //Modifying an existing field
                FULLSPEC_FIELD &field = file.fields[idx];
                if (type != ftHEADER) field.type = type;
                if (text.Len() > 0) field.text = text;
                if (value.Len() > 0) field.value = value;
                if (range.Len() > 0) field.range = range;
                if (def.Len() > 0) field.def = def;
                if (require.Len() > 0) field.require = require;
                //if (needed.Len() > 0) field.needed = Str2Long(needed, field.needed);
                if (hide.Len() > 0) field.hide = (hide == STR_YES);

            }
            else if ((text.Len() > 0) && (type != ftHEADER))
            {

                //Creating a new field
                file.fields.push_back(FULLSPEC_FIELD(type, name, text, value, range, def, require, /*Str2Long(needed, NEEDED),*/ hide == STR_YES));

            }

            //Prepare next field
            name = text = value = range = def = require = /*needed =*/ hide = wxEmptyString;
            type = ftHEADER;

        }

    } while (lp.has_more());

    delete file.body;
    file.body = nullptr;

    #ifdef DEBUG
    //Oh! Please don't look at me, I'm not dressed yet..
    /*FFQConsole::Get()->AppendLine(m_File->display + SPACE + m_File->composite + SPACE + m_File->separator + SPACE + m_File->matches, 0);
    std::for_each(m_File->fields.begin(), m_File->fields.end(), [](const FULLSPEC_FIELD &field) {
                  FFQConsole::Get()->AppendLine(ToStr(field.type) + " | " + field.name + " | " + field.text + " | " + field.value + " | " + field.def + " | " + field.range, 0);
    });*/
    #endif

}

//---------------------------------------------------------------------------------------

void FFQFullSpec::SetCommandLine(FULLSPEC_FILE &file, wxString cmd)
{

    //This will parse the command line and set the
    //recovered values to the corresponding controls

    if ((file.composite.Len() > 0) && cmd.StartsWith(file.composite))
    {

        //Remove composite + SPACE
        cmd.Remove(0, file.composite.Len() + 1);

        //Remove quotes, if present
        wxUniChar c = cmd.Len() > 0 ? cmd[0] : 0;
        if ((c == QUOTE) || (c == DQUOTE))
        {
            //Ending QUOTE
            cmd.Remove(0, 1);
            cmd.RemoveLast();
        }

    }

    while (cmd.Len() > 0)
    {

        //Get field name and its index
        wxString cur = GetCmdToken(cmd);
        if (cur.StartsWith(file.prefix)) cur.Remove(0, file.prefix.Len());
        int idx = IndexOfField(file, cur);

        if (idx >= 0)
        {

            //Get a reference to the field and skip if it is a header
            FULLSPEC_FIELD &field = file.fields[idx];
            if (field.type == ftHEADER) continue;

            //The checkbox must always be checked for fields in the command line
            if (field.check != nullptr) field.check->SetValue(true);

            if (field.type != ftCHECK)
            {

                //Everything that is not a check box must have a value
                wxString val = (field.name == QUESTION) ? cur : GetCmdToken(cmd);

                wxTextEntry *te = dynamic_cast<wxTextEntry*>(field.ctrl);
                if (te) te->ChangeValue(val); //wxTextEntry or wxComboBox?

                else if (field.type == ftCHOICE)
                {

                    //Select the value in the choice
                    wxChoice *ch = dynamic_cast<wxChoice*>(field.ctrl);
                    FFQTokenParser tp(field.value, file.separator);
                    for (idx = 0; (size_t)idx < ch->GetCount(); idx++)
                    {
                        if (val == tp.next().BeforeFirst(EQUAL))
                        {
                            ch->SetSelection(idx);
                            break;
                        }
                    }

                }

                else if (field.type == ftCHECKLIST)
                {

                    //Check the items in the list
                    wxCheckListBox *cl = dynamic_cast<wxCheckListBox*>(field.ctrl);
                    FFQTokenParser tp(field.value, file.separator);
                    val = COMMA + val + COMMA;
                    int idx = 0;
                    while (tp.has_more()) cl->Check(idx++, val.Find(COMMA + tp.next().BeforeFirst(EQUAL) + COMMA) > -1);

                }

            }

        }

        //else LOG ROUGE FIELD?

    }

}

//---------------------------------------------------------------------------------------

bool FFQFullSpec::ValidateCtrls(FULLSPEC_FILE &file, bool show_message)
{

    int section = 0; //Used to detect the section / notebook page in which the control exists
    wxNotebook *nbook = file.notebook ? dynamic_cast<wxNotebook*>(file.evt_handler->GetParent()) : nullptr;

    //Qualidate da textos controles!
    for (size_t i = 0; i < file.fields.size(); i++)
    {

        FULLSPEC_FIELD &field = file.fields[i];
        if (field.type == ftHEADER)
        {
            if (i > 0) section++;
            continue;
        }
        else if ((!field.is_enabled()) || (!field.is_checked())) continue; //((field.check == NULL) || (!field.check->IsChecked())) continue;

        wxTextEntry *te = dynamic_cast<wxTextEntry*>(field.ctrl);

        if (te != NULL)
        {

            if (te->GetValue().Len() == 0)
            {

                if (show_message)
                {
                    if (nbook) nbook->SetSelection(section);
                    ShowError(field.ctrl, FFQSF(SID_FULLSPEC_REQUIRED_FIELD, field.text));
                }
                return false;

            }

            else if (te->GetValue().Find(COLON) != wxNOT_FOUND)
            {

                if (show_message)
                {
                    if (nbook) nbook->SetSelection(section);
                    ShowError(field.ctrl, FFQS(SID_FULLSPEC_COLON_NOT_ALLOWED));
                }
                return false;

            }

        }

    }

    return true;

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQFullSpec::FFQFullSpec(wxWindow* parent, wxWindowID id)
{
	//  (*Initialize(FFQFullSpec)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	MainSizer = new wxFlexGridSizer(2, 1, 0, 0);
	//ScrollWin = new wxScrolledWindow(this, ID_SCROLLWIN, wxDefaultPosition, wxDefaultSize, wxVSCROLL, _T("ID_SCROLLWIN"));
	//MainSizer->Add(ScrollWin, 1, wxALL|wxEXPAND, 5);
	BtnSizer = new wxBoxSizer(wxHORIZONTAL);
	//TestBtn = new wxButton(this, ID_TESTBTN, _T("T"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TESTBTN"));
	//TestBtn->SetLabel(FFQS(SID_FULLSPEC_TEST));
	//BtnSizer->Add(TestBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BtnSizer->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	BtnSizer->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	OkBtn = new wxButton(this, ID_OKBTN, _T("O"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBTN"));
	OkBtn->SetDefault();
	OkBtn->SetLabel(FFQS(SID_COMMON_OK));
	BtnSizer->Add(OkBtn, 1, wxALL|wxEXPAND, 5);
	CancelBtn = new wxButton(this, ID_CANCELBTN, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBTN"));
	CancelBtn->SetLabel(FFQS(SID_COMMON_CANCEL));
	BtnSizer->Add(CancelBtn, 1, wxALL|wxEXPAND, 5);
	MainSizer->Add(BtnSizer, 1, wxALL|wxEXPAND, 0);
	SetSizer(MainSizer);
	MainSizer->Fit(this);
	MainSizer->SetSizeHints(this);
	Center();

	Connect(ID_OKBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQFullSpec::ActionClick);
	Connect(ID_CANCELBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQFullSpec::ActionClick);
	//  *)

	/*ScrollWin->SetScrollbars(25, 25, 3, 3, 0, 0);
	wxDisplay d(wxDisplay::GetFromWindow(this));
	wxRect sz = d.GetClientArea();
	ScrollWin->SetMaxSize(wxSize(sz.GetWidth(), int((float)sz.GetHeight() * 0.75)));*/
	//FileDlg->SetMessage(FFQS(SID_FULLSPEC_SELECT_FILE));
    //FileDlg->SetDirectory(FFQCFG()->GetBrowseRoot());

	m_CtrlSizer = nullptr;
	m_File = nullptr;
    m_FirstIdle = true;
    m_FirstShow = true;
	m_CtrlsCreated = false;

	//m_EvtHandler = nullptr;

	//Bind(wxEVT_BUTTON, &FFQFullSpec::OnCommand, this);
	//Bind(wxEVT_CHECKBOX, &FFQFullSpec::OnCommand, this);
	//Bind(wxEVT_CHECKLISTBOX, &FFQFullSpec::OnCommand, this);
	//Bind(wxEVT_CHOICE, &FFQFullSpec::OnCommand, this);
	//Bind(wxEVT_TEXT, &FFQFullSpec::OnCommand, this);

	Bind(wxEVT_IDLE, &FFQFullSpec::OnIdle, this);
	Bind(wxEVT_SHOW, &FFQFullSpec::OnShow, this);

}

//---------------------------------------------------------------------------------------

FFQFullSpec::~FFQFullSpec()
{

	//  (*Destroy(FFQFullSpec)
	//  *)

	if (m_File) ClearControlsFor(*m_File, true);

	//if (m_EvtHandler) delete m_EvtHandler;
	//m_EvtHandler = nullptr;

    //if (m_File) std::for_each(m_File->fields.begin(), m_File->fields.end(), [](FULLSPEC_FIELD &field) { field.check = NULL; field.ctrl = NULL; });
    //m_File = nullptr;

}

//---------------------------------------------------------------------------------------

bool FFQFullSpec::Execute(int full_spec_index, wxString &cmd)
{
    m_File = &s_Files[full_spec_index];
    m_CmdLine = cmd;
    m_TestFile = wxEmptyString;
    //ParseSpecBody(*m_File);
    MakeControlsFor(*m_File, this);
	MainSizer->Insert(0, m_File->evt_handler->GetParent(), 1, wxALL|wxEXPAND, 5);
	MainSizer->Fit(this);
	//Layout();

    //MakeControls();
    SetCommandLine(*m_File, cmd);
    m_File->evt_handler->UpdateControls();
    //m_EvtHandler = new FFQFullSpecEvtHandler(ScrollWin, m_File);
    //m_EvtHandler->UpdateControls();
    SetTitle(FFQSF(SID_FULLSPEC_TITLE, m_File->display));
    bool res = (ShowModal() == wxID_OK);
    if (res) res = GetCommandLine(*m_File, cmd);
    return res;
}

//---------------------------------------------------------------------------------------

/*FFQ_STATIC_JOB* FFQFullSpec::GetTestJob()
{
    if (m_TestFile.Len() == 0) return 0;

    //Should we only test checked items or everything?
    bool chk_only = false;
    for (int i = 0; (!chk_only) && (i < (int)m_File->fields.size()); i++)
    {
        FULLSPEC_FIELD &field = m_File->fields[i];
        chk_only = field.check && field.check->IsChecked();
    }

    //Prepare the job
    FFQ_STATIC_JOB *res = new FFQ_STATIC_JOB();
    wxString outfile = FFQCFG()->GetTmpPath(m_TestFile.BeforeLast(wxFileName::GetPathSeparator()), false, ".mkv");
    res->inputs.Add(m_TestFile);
    res->del_paths.Add(outfile);
    //res->save_log_for = outfile;
    //res->save_log = true;

    const wxString cmd = "-i \"%s\" -map 0:v:0 -b:v 1000 -c:v %s %s -an -t 5000 -y \"%s\"";

    //Make the commands
    return 0;

}*/

//---------------------------------------------------------------------------------------

/*void FFQFullSpec::AddFieldCtrl(FULLSPEC_FIELD &field)
{

    if (field.type == ftHEADER)
    {

        //Header
        wxStaticText *st = new wxStaticText(ScrollWin, wxID_ANY, field.text);
        st->SetFont(m_HeaderFont);
        m_CtrlSizer->Add(st, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
        field.ctrl = st;

    }
    else
    {

        //Field always have a checketty checkbox
        field.check = new wxCheckBox(ScrollWin, wxID_ANY, field.text);// + (field.hide ? " (!)" : ""));
        wxString tt = field.name;
        if (field.range.Len() > 0) tt += ", range: " + field.range.BeforeFirst(SPACE) + ".." + field.range.AfterFirst(SPACE);
        if (field.def.Len() > 0) tt += ", default: " + field.def;
        field.check->SetToolTip(tt);

        //if (field.hide) field.check->SetForegroundColour(*wxRED);

        if (field.type == ftCHECK)
        {
            //If only a checkbox, add it directly and resign from further processing
            //field.check->SetToolTip(field.name);
            m_CtrlSizer->Add(field.check, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
            return;
        }

        else if (field.type == ftCHOICE)
        {

            //Create a choice control and parse the values for it
            wxChoice *ch = new wxChoice(ScrollWin, wxID_ANY);
            field.ctrl = ch;
            SetItemContainer(*m_File, field);
            ch->SetSelection(field.defidx);

        }
        else if (field.type == ftFLOAT)
        {

            //Create a text control
            wxTextCtrl *tc = new wxTextCtrl(ScrollWin, wxID_ANY);
            field.ctrl = tc;

            //Attach a floating point validator
            wxFloatingPointValidator<float> val(5, NULL);//, wxNUM_VAL_ZERO_AS_BLANK);
            tc->SetValidator(val);

            //Set a valid input range
            wxString r = field.range.Len() > 0 ? field.range : NUM_VAL_DEFAULT;
            wxString m = GetToken(r, SPACE);
            val.SetMin(m == RANGE_MIN ? std::numeric_limits<float>::min() : Str2Float(m, 0));
            val.SetMax(r == RANGE_MAX ? std::numeric_limits<float>::max() : Str2Float(r, 0));

            //Set default value
            tc->ChangeValue(field.def);

        }

        else if (field.type == ftINTEGER)
        {

            //Same as with float, just for integers..
            wxTextCtrl *tc = new wxTextCtrl(ScrollWin, wxID_ANY);
            field.ctrl = tc;
            wxIntegerValidator<int> val(NULL);//, wxNUM_VAL_ZERO_AS_BLANK);
            tc->SetValidator(val);

            //Set a valid input range
            wxString r = field.range.Len() > 0 ? field.range : NUM_VAL_DEFAULT;
            wxString m = GetToken(r, SPACE);
            val.SetMin(m == RANGE_MIN ? std::numeric_limits<int>::min() : Str2Long(m, 0));
            val.SetMax(r == RANGE_MAX ? std::numeric_limits<int>::max() : Str2Long(r, 0));

            //Set default value
            tc->ChangeValue(field.def);

        }

        else if (field.type == ftSTRING)
        {

            //Simple text control / input - validated in ActionClick()
            wxTextCtrl *tc = new wxTextCtrl(ScrollWin, wxID_ANY);
            field.ctrl = tc;
            tc->ChangeValue(field.def);

        }

        else if (field.type == ftCOMBO)
        {

            //Create a combobox control and parse the values for it
            wxComboBox *cb = new wxComboBox(ScrollWin, wxID_ANY);
            field.ctrl = cb;
            SetItemContainer(*m_File, field);
            cb->SetSelection(field.defidx);

        }

        else if (field.type == ftCHECKLIST)
        {

            //Create a checklist control and parse the values for it
            wxCheckListBox *cl = new wxCheckListBox(ScrollWin, wxID_ANY);
            cl->SetMaxSize(wxSize(-1, OkBtn->GetSize().GetHeight() * 3));
            field.ctrl = cl;
            SetItemContainer(*m_File, field);
            if (field.def == "*") for (int i = 0; i < (int)cl->GetCount(); i++) cl->Check(i, true);
            else cl->Check(field.defidx, true);
        }

        else return; //Something went sour..

        //Create a sizer and add the control before adding the sizer to the control sizer..
        wxFlexGridSizer *fgs = new wxFlexGridSizer(1, 2, 0, 0);
        fgs->AddGrowableCol(0);
        fgs->Add(field.check, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        fgs->Add(field.ctrl, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
        m_CtrlSizer->Add(fgs, 1, wxALL|wxEXPAND, 3);

    }

}*/

//---------------------------------------------------------------------------------------

/*bool FFQFullSpec::GetCmdLine(wxString &res)
{

    //Build a command line from the fields..

    bool composite = m_File->composite.Len() > 0;
    res.Clear();

    for (size_t i = 0; i < m_File->fields.size(); i++)
    {
        //Get a reference to the field and continue if the field is a header or un-checked
        FULLSPEC_FIELD &field = m_File->fields[i];
        if ((field.type == ftHEADER) || (!field.check->IsEnabled()) || (!field.check->IsChecked())) continue;

        //Get the value from the field, set to checkbox value if no control
        wxString val = field.ctrl ? GetFieldValue(field) : m_File->checkval;

        //Add field name to result
        if (field.name == QUESTION)
        {

            res += (val.Len() > 0) ? m_File->prefix + val : "";
            if (m_File->checkval.Len() > 0) res += EQUAL + m_File->checkval;

        }
        else
        {

            res += m_File->prefix + field.name;
            //val = StrTrim(val);

            if (val.Len() > 0)
            {

                //If a value exists, add the formatted value and the proper separators
                res += composite ? EQUAL : SPACE;
                //val.Replace(COLON, COMMA);
                if (val.Find(SPACE) >= 0) res += '"' + val + '"';
                else res += val;

            }

        }

        //Terminate this field with a separator
        res += composite ? COLON : SPACE;

    }
    if (res.Len() > 0)
    {

        //Remove last separator
        res.RemoveLast();

        //Add the composite argument and the value list
        if (composite) res = m_File->composite + SPACE + '"' + res + '"';

    }
    return true;
}*/

//---------------------------------------------------------------------------------------

/*wxString FFQFullSpec::GetFieldValue(FULLSPEC_FIELD &field)
{
    //if (!field.check->IsChecked()) return wxEmptyString;
    wxString res;
    wxTextEntry *te = dynamic_cast<wxTextEntry*>(field.ctrl);
    if (te) res = te->GetValue(); //string (wxTextEntry) entry or combo
    else if (field.type == ftCHOICE)
    {
        //Get the value from a choice
        wxChoice *ch = dynamic_cast<wxChoice*>(field.ctrl);
        int sel_idx = ch->GetSelection();
        FFQTokenParser tp(field.value, m_File->separator);
        while (sel_idx-- > 0) tp.next();
        if (tp.has_more()) res = tp.next().BeforeFirst(EQUAL);
    }
    else if (field.type == ftCHECKLIST)
    {
        //Get the values from a checklist
        wxCheckListBox *cl = dynamic_cast<wxCheckListBox*>(field.ctrl);
        FFQTokenParser tp(field.value, m_File->separator);
        for (int i = 0; i < (int)cl->GetCount(); i++)
        {
            wxString tok = tp.next();
            if (cl->IsChecked(i)) res += tok.BeforeFirst(EQUAL) + COMMA;
        }
        if (res.Len() > 0) res = res.RemoveLast();
    }
    #ifdef DEBUG
    else FFQConsole::Get()->AppendLine("FFQFullSpec: Bad field control \"" + field.text + "\"", COLOR_RED);
    #endif
    return res;
}*/

//---------------------------------------------------------------------------------------

/*int FFQFullSpec::IndexOfField(wxString field_name)
{

    //Return the index of the field with field_name
    for (size_t i = 0; i < m_File->fields.size(); i++)
    {
        FULLSPEC_FIELD &field = m_File->fields[i];
        if (field.name == QUESTION)
        {
            FFQTokenParser tp(field.value, m_File->separator);
            while (tp.has_more()) if (tp.next().StartsWith(field_name + EQUAL)) return (int)i;
        }
        else if (m_File->fields[i].name == field_name) return (int)i;
    }
    //Field not found
    return wxNOT_FOUND;

}*/

//---------------------------------------------------------------------------------------

/*void FFQFullSpec::MakeControls()
{

    //Create the controls used to define the fields
    if (m_CtrlsCreated) return;

    m_CtrlSizer = new wxFlexGridSizer(m_File->fields.size(), 1, 0, 0);
    //m_HeaderFont = wxFont(GetFont()).MakeLarger().MakeBold();

    for (size_t idx = 0; idx < m_File->fields.size(); idx++) MakeControlFor(*m_File, m_File->fields[idx], ScrollWin, m_CtrlSizer);

    ScrollWin->SetSizerAndFit(m_CtrlSizer);
	MainSizer->Fit(this);

    //Update title and scroll bars
    //SetTitle(FFQSF(SID_FULLSPEC_TITLE, m_File->display));

    //Update scroll bars based on the height of a checkbox
    for (size_t i = 0; i < m_File->fields.size(); i++)
    {

        FULLSPEC_FIELD &field = m_File->fields[i];
        if (field.check)
        {

            wxSize sz = field.check->GetSize();
            ScrollWin->SetScrollbars(sz.GetHeight(), sz.GetHeight(), 3, 3, 0, 0);
            break;

        }

    }

    m_CtrlsCreated = true;

}*/

//---------------------------------------------------------------------------------------

/*void FFQFullSpec::ParseCmd(wxString cmd)
{

    //This will parse the command line and set the
    //recovered values to the corresponding controls

    if ((m_File->composite.Len() > 0) && cmd.StartsWith(m_File->composite))
    {

        //Remove composite + SPACE + QUOTE
        cmd.Remove(0, m_File->composite.Len() + 2);

        //Ending QUOTE
        cmd.Remove(cmd.Len() - 1, 1);

    }

    while (cmd.Len() > 0)
    {

        //Get field name and its index
        wxString cur = GetCmdToken(cmd);
        if (cur.StartsWith(m_File->prefix)) cur.Remove(0, m_File->prefix.Len());
        int idx = IndexOfField(*m_File, cur);

        if (idx >= 0)
        {

            //Get a reference to the field and skip if it is a header
            FULLSPEC_FIELD &field = m_File->fields[idx];
            if (field.type == ftHEADER) continue;

            //The checkbox must always be checked for fields in the command line
            field.check->SetValue(true);

            if (field.type != ftCHECK)
            {

                //Everything that is not a check box must have a value
                wxString val = (field.name == QUESTION) ? cur : GetCmdToken(cmd);

                wxTextEntry *te = dynamic_cast<wxTextEntry*>(field.ctrl);
                if (te) te->ChangeValue(val); //wxTextEntry or wxComboBox?

                else if (field.type == ftCHOICE)
                {

                    //Select the value in the choice
                    wxChoice *ch = dynamic_cast<wxChoice*>(field.ctrl);
                    FFQTokenParser tp(field.value, m_File->separator);
                    for (idx = 0; (size_t)idx < ch->GetCount(); idx++)
                    {
                        if (val == tp.next().BeforeFirst(EQUAL))
                        {
                            ch->SetSelection(idx);
                            break;
                        }
                    }

                }

                else if (field.type == ftCHECKLIST)
                {

                    //Check the items in the list
                    wxCheckListBox *cl = dynamic_cast<wxCheckListBox*>(field.ctrl);
                    FFQTokenParser tp(field.value, m_File->separator);
                    val = COMMA + val + COMMA;
                    int idx = 0;
                    while (tp.has_more()) cl->Check(idx++, val.Find(COMMA + tp.next().BeforeFirst(EQUAL) + COMMA) > -1);

                }

            }

        }

        //else LOG ROUGE FIELD?

    }

}*/

//---------------------------------------------------------------------------------------

/*void FFQFullSpec::ParseSpecBody()
{

    //This will parse the body of the specification text
    //and convert it to a list of fields

    if (m_File->body == NULL) return;

    FULLSPEC_FIELD_TYPE type = ftHEADER;
    wxString key, val, name, text, value, range, def, require, required, hide;

    FFQLineParser lp(*m_File->body);

    do
    {

        //Get a line
        val = StrTrim(lp.next());

        //Process if not empty
        if (val.Len() > 0)
        {

            //Skip comment?
            if (val[0] == HASH) continue;

            //Section header?
            if (val[0] == COLON) m_File->fields.push_back(FULLSPEC_FIELD(StrTrim(val.SubString(1, val.Len()))));
            else
            {

                //Split key / value
                if (StrSplit(val, key, EQUAL))
                {

                    if (key == "text")
                    {
                        text = val;
                        type = ftCHECK; //The default field type
                    }
                    else if (key == "type")
                    {

                        if (val == "check") type = ftCHECK;
                        else if (val == "choice") type = ftCHOICE;
                        else if (val == "float") type = ftFLOAT;
                        else if (val == "integer") type = ftINTEGER;
                        else if (val == "string") type = ftSTRING;
                        else if (val == "combo") type = ftCOMBO;
                        else if (val == "checklist") type = ftCHECKLIST;
                        else LogSpecError(FS_LOG_BAD_TYPE, lp.last());

                    }
                    else if (key == "value") value = val;
                    else if (key == "range") range = val;
                    else if (key == "default") def = val;
                    else if (key == "require") require = val;
                    else if (key == "required") required = val;
                    else if (key == "hide") hide = val;
                    else LogSpecError(FS_LOG_BAD_FIELD, lp.last());

                }
                else
                {

                    name = StrTrim(val);
                    if (name.Len() == 0) LogSpecError(FS_LOG_BAD_FIELD, lp.last());

                }

            }

        }

        else if (name.Len() > 0)
        {

            int idx = IndexOfField(name);
            if (idx >= 0)
            {

                //Modifying an existing field
                FULLSPEC_FIELD &field = m_File->fields[idx];
                if (type != ftHEADER) field.type = type;
                if (text.Len() > 0) field.text = text;
                if (value.Len() > 0) field.value = value;
                if (range.Len() > 0) field.range = range;
                if (def.Len() > 0) field.def = def;
                if (require.Len() > 0) field.require = require;
                if (required.Len() > 0) field.required = (require == STR_YES);
                if (hide.Len() > 0) field.hide = (hide == STR_YES);

            }
            else if ((text.Len() > 0) && (type != ftHEADER))
            {

                //Creating a new field
                m_File->fields.push_back(FULLSPEC_FIELD(type, name, text, value, range, def, require, hide == STR_YES, required == STR_YES));

            }

            //Prepare next field
            name = text = value = range = def = require = hide = required = wxEmptyString;
            type = ftHEADER;

        }

    } while (lp.has_more());

    delete m_File->body;
    m_File->body = NULL;

    SetTitle(FFQSF(SID_FULLSPEC_TITLE, m_File->display));

    #ifdef DEBUG
    //Oh! Please don't look at me, I'm not dressed yet..
    / *FFQConsole::Get()->AppendLine(m_File->display + SPACE + m_File->composite + SPACE + m_File->separator + SPACE + m_File->matches, 0);
    std::for_each(m_File->fields.begin(), m_File->fields.end(), [](const FULLSPEC_FIELD &field) {
                  FFQConsole::Get()->AppendLine(ToStr(field.type) + " | " + field.name + " | " + field.text + " | " + field.value + " | " + field.def + " | " + field.range, 0);
    });* /
    #endif
}*/

//---------------------------------------------------------------------------------------

/*void FFQFullSpec::UpdateControls()
{

    //Enable all control with their checkboxes ticked
    if (!m_CtrlsCreated) return;
    ScrollWin->Freeze();
    for (int i = 0; i < (int)m_File->fields.size(); i++)
    {
        FULLSPEC_FIELD &field = m_File->fields[i];
        if (field.type != ftHEADER) UpdateField(field);
    }
    ScrollWin->Thaw();

}*/

//---------------------------------------------------------------------------------------

/*void FFQFullSpec::UpdateField(FULLSPEC_FIELD &field)
{

    //Make sure that we are not running in circles..
    if (m_UpdateStack.Index(field.name) >= 0)
    {

        //WARNING: Cyclic field requirements for: field.name
        //#ifdef DEBUG
        FFQConsole::Get()->AppendLine(wxString::Format(FS_CYCLIC_REQUIRE, field.name), COLOR_RED);
        //#endif // DEBUG
        return;

    }
    m_UpdateStack.Add(field.name);

    bool enable = true;
    FFQTokenParser tp(field.require, m_File->separator);
    while (enable && tp.has_more())
    {

        //Get required field name and value
        wxString v = tp.next(), n = GetToken(v, EQUAL, true);

        //Get required check state
        bool inverse = n.StartsWith(EXCLAM);
        if (inverse) n = n.AfterFirst(EXCLAM);

        //Get index of field
        int idx = IndexOfField(*m_File, n);

        //If the field is not found, it is ignored
        if (idx < 0) continue;

        FULLSPEC_FIELD &ff = m_File->fields[idx];

        //Make sure that the required field has its requirements met
        UpdateField(ff);

        if (ff.check->IsEnabled())
        {

            //Validate the field value if applied
            if (v.Len() > 0)
            {

                if (ff.check->IsChecked())
                {
                    if (!LogicCompare(GetFieldValue(*m_File, ff), v)) enable = false;
                    //if (inverse == (v == GetFieldValue(ff))) enable = false;
                }
                else enable = false;

            }

            //Else validate checkbox state
            else if (ff.check->IsChecked() == inverse) enable = false;

        }

        //Field is not available
        else enable = false;

    }

    field.check->Enable(enable);
    if (field.ctrl) field.ctrl->Enable(enable && field.check->IsChecked());

    //Remove field from update stack
    m_UpdateStack.RemoveAt(m_UpdateStack.Count() - 1);

}*/

//---------------------------------------------------------------------------------------

/*bool FFQFullSpec::ValidateRequires()
{
    return false;
}*/

//---------------------------------------------------------------------------------------

/*void FFQFullSpec::OnCommand(wxCommandEvent &evt)
{

    //Some control has changed its state
    if (evt.GetEventType() == wxEVT_CHECKLISTBOX)
    {
        //Make sure that at least one item is checked
        wxCheckListBox *cl = dynamic_cast<wxCheckListBox*>(evt.GetEventObject());
        if (cl)
        {
            wxArrayInt aint;
            cl->GetCheckedItems(aint);
            if (aint.GetCount() == 0) cl->Check(evt.GetInt(), true);
        }
    }

    UpdateControls();

}*/


//---------------------------------------------------------------------------------------

void FFQFullSpec::OnIdle(wxIdleEvent &evt)
{
    if ((!m_FirstShow) && m_FirstIdle)
    {
        m_FirstIdle = false;
        Center();//OnParent();
    }
}

//---------------------------------------------------------------------------------------

void FFQFullSpec::OnShow(wxShowEvent &evt)
{
    evt.Skip();
    m_FirstShow = false;
}

//---------------------------------------------------------------------------------------

void FFQFullSpec::ActionClick(wxCommandEvent& event)
{

    //A button was clicked..
    int evtId = event.GetId();
    if (evtId == ID_CANCELBTN) EndModal(wxID_CANCEL);
    else if ((evtId == ID_OKBTN) && ValidateCtrls(*m_File)) EndModal(wxID_OK);

}
