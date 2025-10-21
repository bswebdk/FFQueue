/*************************************************************************
*                                                                        *
*  Copyright (c) 2014 Torben Bruchhaus                                   *
*  http://ffqueue.bruchhaus.dk/                                          *
*  File: FFQDebugUtils.cpp                                               *
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

#include "FFQDebugUtils.h"
#include "../FFQAbout.h"
#include "../utils/FFQLang.h"
#include "FFQConst.h"
#include "FFQMisc.h"
#include "FFQConsole.h"
#include "FFQCompress.h"
#include "../FFQFullSpec.h"
#include "version.h"
#include <wx/ffile.h>
#include <wx/file.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/xml/xml.h>
//#include <zlib.h>

wxString MakeArrayName(wxString array_name, int array_len, bool for_header)
{

    wxString len;

    if (array_len >= 0)
    {
        //Array size
        len += "const uint32_t " + array_name + "_SIZE";
        if (for_header) len = "extern " + len + ";" + CRLF;
        else len += wxString::Format(" = %i;", array_len) + CRLF;
    }

    //Array declaration
    wxString cnt = "const unsigned char " + array_name + "[]";
    if (for_header) cnt = "extern " + cnt + ";";
    else cnt = cnt + " = {" + CRLF;

    return len + cnt + CRLF;

}

//---------------------------------------------------------------------------------------

wxString LoadString(wxString filename)
{
    wxFFile ff(filename, "rb");
    if (!ff.IsOpened()) return "";
    wxString res;
    bool ok = ff.ReadAll(&res);
    ff.Close();
    return ok ? res : "";
}

//---------------------------------------------------------------------------------------

bool SaveString(wxString filename, wxString &str)
{
    wxFFile ff(filename, "wb");
    bool ok = ff.Write(str);
    ff.Close();
    return ok;
}

//---------------------------------------------------------------------------------------

wxString GetCopyHeader()
{

    //Extract the copyright header from FFQApp.h
    wxString src = LoadString("src/FFQApp.h");
    if (src.Len() == 0) return wxEmptyString;
    if (!src.StartsWith("/*")) return wxEmptyString;
    int endp = src.Find("*/");
    if (endp < 2) return wxEmptyString;
    return src.SubString(0, endp + 1);

}
//---------------------------------------------------------------------------------------

wxString BufToArray(unsigned char *buf, int len, wxString array_name, unsigned int indent)
{

    //Converts a byte buffer into a C-style array of unsigned
    //char's using the given array name.

    //Array declaration
    wxString idt, res = MakeArrayName(array_name, len, false);// "const unsigned char " + array_name + "[] = {" + CRLF, idt;//, s;

    //Make indentation
    if (indent > 0) while (idt.Len() < indent - 1) idt += " ";

    //Add line break
    idt = CRLF + idt;

    //Add indent to result
    res += idt;

    //Line length counter
    int line = 0;

    for (int i = 0; i < len; i++)
    {

        if (line == 16)
        {

            //16 bytes pr line in the array
            res += idt;

            //Reset line counter
            line = 0;
        }

        //Add a byte to the array
        //s.Printf(" 0x%02x,", buf[i]);
        //res += s;
        res += wxString::Format(" 0x%02x,", buf[i]);

        //Increment line counter
        line++;

    }

    //Remove last ","
    res.RemoveLast();

    //Return the terminated array string
    return res + CRLF + CRLF + "};";

}

//---------------------------------------------------------------------------------------

wxString FileToArray(wxString filename, wxString array_name, unsigned int indent)
{

    //This does the same as "BufToArray" above but instead of using a buffer
    //the bytes are read from a file


    wxFile f;
    if (!f.Open(filename, wxFile::read)) return "";

    wxString idt, res = MakeArrayName(array_name, (int)f.Length(), false);// "const unsigned char " + array_name + "[] = {" + CRLF + CRLF;

    if (indent > 0) while (idt.Len() < indent - 1) idt += " ";
    res += idt;

    unsigned char c;
    int line = 0;

    while (!f.Eof())
    {

        f.Read(&c, 1);
        if (line == 16)
        {
            res += CRLF + idt;
            line = 0;
        }
        //s.Printf(" 0x%02x,", c);
        //res += s;
        res += wxString::Format(" 0x%02x,", c);
        line++;

    }

    f.Close();

    //Remove last ","
    res.RemoveLast();

    return res + CRLF + CRLF + "};";

}

//---------------------------------------------------------------------------------------

bool MakeBinaryResources()
{

    wxString cpy = GetCopyHeader();
    if (cpy.Len() > 0) cpy += CRLF + CRLF;
    cpy += "// BINARY RESOURCES GENERATED BY FFQUEUE DEBUG BUILD!" + CRLF + CRLF;

    //Make the contents of the header
    wxString s = cpy;
    s.Replace("FFQApp.h ", "bin_res.h");
    s += "#ifndef BIN_RES_H" + CRLF +
         "#define BIN_RES_H" + CRLF +
         "#include \"utils/FFQConst.h\"" + CRLF +
         //"#ifndef __WINDOWS__" + CRLF +
         //"#include <cstdint>" + CRLF +
         //"#endif" + CRLF +
         CRLF;

    s += MakeArrayName("TOOL_ADD", 0, true) + CRLF;
    s += MakeArrayName("TOOL_BATCH", 0, true) + CRLF;
    s += MakeArrayName("TOOL_EDIT", 0, true) + CRLF;
    s += MakeArrayName("TOOL_DELETE", 0, true) + CRLF;
    s += MakeArrayName("TOOL_START", 0, true) + CRLF;
    s += MakeArrayName("TOOL_STOP", 0, true) + CRLF;
    s += MakeArrayName("TOOL_CONFIG", 0, true) + CRLF;
    s += MakeArrayName("TOOL_ABOUT", 0, true) + CRLF;
    s += MakeArrayName("TOOL_TOOLS", 0, true) + CRLF;
    s += MakeArrayName("TOOL_PREVIEW", 0, true) + CRLF;
    s += MakeArrayName("TOOL_PRESETS", 0, true) + CRLF;
    s += MakeArrayName("ICON_JOB", 0, true) + CRLF;
    s += MakeArrayName("ICON_TOOL", 0, true) + CRLF;
    s += MakeArrayName("ICON_PLAY_DARK", 0, true) + CRLF;
    s += MakeArrayName("ICON_PLAY_LIGHT", 0, true) + CRLF;
    s += MakeArrayName("ICON_TAB_MSG", 0, true) + CRLF;
    s += MakeArrayName("ICON_TAB_ON", 0, true) + CRLF;
    s += MakeArrayName("ICON_TAB_OFF", 0, true) + CRLF;
    s += MakeArrayName("FLAG_ANIM", 0, true) + CRLF;
    s += MakeArrayName("MAIN_LOGO", 0, true) + CRLF;
    s += MakeArrayName("MAD_CLIPPY", 0, true) + CRLF;
    s += MakeArrayName("FONTS_CONF", 0, true) + CRLF;
    s += MakeArrayName("STR_ABOUT", 0, true) + CRLF;
    s += MakeArrayName("STR_X264", 0, true) + CRLF;
    s += MakeArrayName("STR_X265", 0, true) + CRLF;
    s += MakeArrayName("STR_AOMAV1", 0, true) + CRLF;
    s += MakeArrayName("STR_RAV1E", 0, true) + CRLF;
    s += "#endif //BIN_RES_H";

    //Update the header as required
    wxString t = LoadString("src/bin_res.h");
    if (s != t) SaveString("src/bin_res.h", s);

    //Encrypt the about strings
    t = ABOUT_KEY;
    STR_HASH key;
    HashString(t, key);
    t = LoadString("res/about.txt");
    t.Replace("\r", "");
    char *about = (char*)t.ToUTF8().data();// .mb_str() t.char_str().data();// .AsChar();
    int about_len = strlen(about);
    //wxMessageBox(wxString::Format("%i", about_len), "about_len");
    //wxMessageBox(about, "about");
    EncBuf((uint8_t*)about, (uint32_t)about_len, key);

    //Create the implementation
    s = cpy;
    s.Replace("FFQApp.h   ", "bin_res.cpp");

    s += "#include \"bin_res.h\"" + CRLF + CRLF;
    s += FileToArray("res/add.png", "TOOL_ADD") + CRLF + CRLF;
    s += FileToArray("res/add_batch.png", "TOOL_BATCH") + CRLF + CRLF;
    s += FileToArray("res/pencil.png", "TOOL_EDIT") + CRLF + CRLF;
    s += FileToArray("res/delete.png", "TOOL_DELETE") + CRLF + CRLF;
    s += FileToArray("res/start.png", "TOOL_START") + CRLF + CRLF;
    s += FileToArray("res/stop.png", "TOOL_STOP") + CRLF + CRLF;
    s += FileToArray("res/config.png", "TOOL_CONFIG") + CRLF + CRLF;
    s += FileToArray("res/info.png", "TOOL_ABOUT") + CRLF + CRLF;
    s += FileToArray("res/tools.png", "TOOL_TOOLS") + CRLF + CRLF;
    s += FileToArray("res/preview.png", "TOOL_PREVIEW") + CRLF + CRLF;
    s += FileToArray("res/preset.png", "TOOL_PRESETS") + CRLF + CRLF;
    s += FileToArray("res/icon_job.png", "ICON_JOB") + CRLF + CRLF;
    s += FileToArray("res/icon_tool.png", "ICON_TOOL") + CRLF + CRLF;
    s += FileToArray("res/play_dark.png", "ICON_PLAY_DARK") + CRLF + CRLF;
    s += FileToArray("res/play_light.png", "ICON_PLAY_LIGHT") + CRLF + CRLF;
    s += FileToArray("res/tab_msg.png", "ICON_TAB_MSG") + CRLF + CRLF;
    s += FileToArray("res/tab_on.png", "ICON_TAB_ON") + CRLF + CRLF;
    s += FileToArray("res/tab_off.png", "ICON_TAB_OFF") + CRLF + CRLF;
    s += FileToArray("res/DK_Flag_Anim_Hjerte.gif", "FLAG_ANIM") + CRLF + CRLF;
    s += FileToArray("res/MainLogo.png", "MAIN_LOGO") + CRLF + CRLF;
    s += FileToArray("res/mad_clippy.png", "MAD_CLIPPY") + CRLF + CRLF;
    s += FileToArray("res/fonts.conf", "FONTS_CONF") + CRLF + CRLF;
    s += BufToArray((unsigned char*)about, about_len, "STR_ABOUT") + CRLF + CRLF;

    //Compress x264 full spec
    wxString fs = LoadString("res/x264" + FULLSPEC_FILE_EXTENSION);
    uint32_t fs_len = 0;
    uint8_t *fs_buf = CompressString(fs, &fs_len);
    s += BufToArray((unsigned char*)fs_buf, fs_len /*x264size + sizeof(x264len)*/, "STR_X264") + CRLF + CRLF;
    delete[] fs_buf;

    //Compress x265 full spec
    fs = LoadString("res/x265" + FULLSPEC_FILE_EXTENSION);
    fs_len = 0;
    fs_buf = CompressString(fs, &fs_len);
    s += BufToArray((unsigned char*)fs_buf, fs_len /*x264size + sizeof(x264len)*/, "STR_X265") + CRLF + CRLF;
    delete[] fs_buf;

    //Compress AOM AV1 full spec
    fs = LoadString("res/aomav1" + FULLSPEC_FILE_EXTENSION);
    fs_len = 0;
    fs_buf = CompressString(fs, &fs_len);
    s += BufToArray((unsigned char*)fs_buf, fs_len /*x264size + sizeof(x264len)*/, "STR_AOMAV1") + CRLF + CRLF;
    delete[] fs_buf;

    //Compress Rust AV1 Encoder full spec
    fs = LoadString("res/rav1e" + FULLSPEC_FILE_EXTENSION);
    fs_len = 0;
    fs_buf = CompressString(fs, &fs_len);
    s += BufToArray((unsigned char*)fs_buf, fs_len /*x264size + sizeof(x264len)*/, "STR_RAV1E") + CRLF + CRLF;
    delete[] fs_buf;

    //Update the implementation as required
    t = LoadString("src/bin_res.cpp");
    if (s != t) SaveString("src/bin_res.cpp", s);

    return true;

}

//---------------------------------------------------------------------------------------

wxXmlNode* FindXmlNode(wxXmlNode* root, wxString node_name, wxString find_attr, wxString find_value)
{

    if (root == NULL) return NULL;

    //Test root
    if (root->GetName() == node_name)
    {

        if (find_attr.Len() > 0)
        {

            wxString av = root->GetAttribute(find_attr);

            if (find_value.Len() > 0)
            {

                if (find_value == av) return root;

            }

            else if (av.Len() > 0) return root;

        }

        else return root;

    }

    //Children
    wxXmlNode *fc = FindXmlNode(root->GetChildren(), node_name, find_attr, find_value);
    if (fc) return fc;

    //Next
    return FindXmlNode(root->GetNext(), node_name, find_attr, find_value);

}

//---------------------------------------------------------------------------------------

//Used to hold variables parsed from code blocks project
wxArrayString wcomp_vars, wcomp_dirs, wlink_vars, wlink_dirs, wlink_libs,
              lcomp_vars, lcomp_dirs, llink_vars, llink_dirs, llink_libs,
              units;

//---------------------------------------------------------------------------------------

wxString FormatShellCommand(wxString s)
{
    //Convert `shell command` into $(shell shell command)
    if ( s.StartsWith("`") && s.EndsWith("`") )
    {
        //s.Remove(0, 1);
        //s.Remove(s.Len() - 1, 1);
        s.Replace("$(#wx)", "$(WX)");
        //s = "$(shell " + s + ")";
    }
    return s;
}

const int TARGET_BOTH = 0;
const int TARGET_MSW = 1;
const int TARGET_NIX = 2;

void AnalyzeNode(wxXmlNode *n, bool comp, int target)
{

    wxString s, t, tt;

    while (n)
    {

        s = n->GetName();

        if (s == "Add")
        {

            s.Clear();

            if (n->GetAttribute("option", &s))
            {

                if ((s != "-DWX_PRECOMP"))// && (!s.EndsWith("-pch")) && (!s.EndsWith("wx_pch.h")))
                {

                    if (target == TARGET_NIX) s.Replace("/gtk-build/", "/");

                    if (comp) {

                        if ((target == TARGET_MSW) || (target == TARGET_BOTH)) wcomp_vars.Add(FormatShellCommand(s));
                        if ((target == TARGET_NIX) || (target == TARGET_BOTH)) lcomp_vars.Add(FormatShellCommand(s));

                    } else {

                        if ((target == TARGET_MSW) || (target == TARGET_BOTH)) wlink_vars.Add(FormatShellCommand(s));
                        if ((target == TARGET_NIX) || (target == TARGET_BOTH)) llink_vars.Add(FormatShellCommand(s));

                    }

                }

            }

            else if (n->GetAttribute("directory", &s))
            {

                s.Replace("$(#wx)", "$(WX)");
                s = "\"" + s + "\"";

                if (comp) {

                    if ((target == TARGET_MSW) || (target == TARGET_BOTH)) wcomp_dirs.Add(s);
                    if ((target == TARGET_NIX) || (target == TARGET_BOTH)) lcomp_dirs.Add(s);

                } else {

                    if ((target == TARGET_MSW) || (target == TARGET_BOTH)) wlink_dirs.Add(s);
                    if ((target == TARGET_NIX) || (target == TARGET_BOTH)) llink_dirs.Add(s);

                }

            }

            else if (n->GetAttribute("library", &s))
            {

                //if (s.StartsWith("lib")) s.Remove(0, 3);
                if ((target == TARGET_MSW) || (target == TARGET_BOTH)) wlink_libs.Add(s.Remove(0, 3).BeforeLast('.'));
                if ((target == TARGET_NIX) || (target == TARGET_BOTH)) llink_libs.Add(s.Remove(0, 3).BeforeLast('.'));

            }

            else FFQConsole::Get()->AppendLine("Bad Add attribute = " + n->GetAttributes()->GetName(), COLOR_RED);

        }

        else if (s == "Compiler") AnalyzeNode(n->GetChildren(), true, target);

        else if (s == "Linker") AnalyzeNode(n->GetChildren(), false, target);

        else if (s == "Unit")
        {

            //Units must be targeted for release..
            wxXmlNode *cn = n->GetChildren();
            bool dbg = false, rls = false;
            while (cn)
            {
                if (cn->GetName() == "Option")
                {
                    dbg = dbg || cn->GetAttribute("target").StartsWith("Debug_");
                    rls = rls || cn->GetAttribute("target").StartsWith("Release_");
                }
                cn = cn->GetNext();
            }

            /*bool dbg = (FindXmlNode(n->GetChildren(), "Option", "target", "Debug_Win") != NULL) ||
                       (FindXmlNode(n->GetChildren(), "Option", "target", "Release_Win") != NULL);

            if (dbg) dbg = (FindXmlNode(n->GetChildren(), "Option", "target", "Release_Win") == NULL) &&
                           (FindXmlNode(n->GetChildren(), "Option", "target", "Release_Lin") == NULL);*/

            if ( rls || (!dbg) )
            {

                s = n->GetAttribute("filename");
                t = s.AfterLast('.');

                if (t == "cpp") units.Add(s);
                else if (t == "rc") units.Add("$" + s);

            }

        }

        n = n->GetNext();

    }

}

//---------------------------------------------------------------------------------------

void DumpArray(wxString name, wxArrayString &arr)
{
    FFQConsole::Get()->AppendLine("[" + name + "]", COLOR_BLUE);
    for (unsigned int i = 0; i < arr.Count(); i++) FFQConsole::Get()->AppendLine(arr[i], COLOR_BLUE);
    FFQConsole::Get()->AppendLine("", COLOR_BLUE);
}

//---------------------------------------------------------------------------------------

wxString ArrayToStr(wxArrayString &arr, wxString prefix, wxString sep = " ")
{

    wxString res;
    for (unsigned int i = 0; i < arr.Count(); i++) res += prefix + arr[i] + sep;
    if (res.Len() > 0) res.RemoveLast(sep.Len());
    return res;

}

//---------------------------------------------------------------------------------------

void AddSubDir(wxString path, wxArrayString &dirs, wxUniChar PSEP)
{
    if (path.Find(PSEP) < 0) return;
    path = path.BeforeLast(PSEP);
    if (dirs.Index(path) < 0) dirs.Add(path);
}

//---------------------------------------------------------------------------------------

const wxString MKDIR_WIN = "if not exist \"$(MAKEROOT)%s\" mkdir \"$(MAKEROOT)%s\"";
const wxString MKDIR_LIN = "mkdir -p \"$(MAKEROOT)%s\"";
const wxString PHONY = ".PHONY: ";
const wxString RCP_ALL = PHONY + "all%sall: paths $(MAKEBINARY)";


//---------------------------------------------------------------------------------------

#define CHEXT(fn, ext) ( fn.BeforeLast('.') + "." + ext )
#define ADD_ICONS(tostr) for(uint8_t i = 0; i < ICON_COUNT; i++) tostr += StrReplace(tmp, "#", ICON_SIZES[i])

//---------------------------------------------------------------------------------------

wxString CreateMakefiles(wxString cbp_path)
{

    //This method converts the FFQueue.cbp to makefiles for Windows and Linux

    const uint8_t ICON_COUNT = 6;
    const wxString ICON_SIZES[ICON_COUNT] = {"16","32","64","128","256","512"};

    wxString wxver = wxString::Format("%d%d", wxMAJOR_VERSION,  wxMINOR_VERSION);
    wxUniChar WSEP = '\\', LSEP = '/', CSEP = wxFileName::GetPathSeparator();
    wxString WBR = "\r\n", LBR = "\n", tmp, tmp2;

    //Make paths
    wxString root = cbp_path;
    if (root.Len() == 0)
    {
        root = wxStandardPaths::Get().GetExecutablePath();
        root = root.BeforeLast(CSEP).BeforeLast(CSEP).BeforeLast(CSEP);
        cbp_path = root + CSEP + "FFQueue.cbp";
    }
    else root = root.BeforeLast(CSEP);

    wxXmlDocument xml;
    if (!xml.Load(cbp_path)) return "Unable to load FFQueue.cbp from \"" + root + "\"";

    //EnumXmlNode(xml.GetRoot(), 0);

    wxXmlNode *cn = FindXmlNode(xml.GetRoot(), "Build", "", "");

    if (cn == NULL) return "\"Build\" section not found in project";

    //Clear values
    wcomp_vars.Clear();
    wcomp_dirs.Clear();
    wlink_vars.Clear();
    wlink_dirs.Clear();
    wlink_libs.Clear();
    lcomp_vars.Clear();
    lcomp_dirs.Clear();
    llink_vars.Clear();
    llink_dirs.Clear();
    llink_libs.Clear();
    units.Clear();

    //Message
    tmp = " build target not found in project";

    //Parse release build for windows
    wxXmlNode *rt = FindXmlNode(cn, "Target", "title", "Release_Win");
    if (rt == NULL) return "Release_Win" + tmp;
    AnalyzeNode(rt->GetChildren(), false, TARGET_MSW);

    //Repace version number in all wx libraries with a shell var
    wxString wxv;
    for (size_t i = 0; i < wlink_libs.Count(); i++)
    {
        wxv = wlink_libs[i];
        if (wxv.StartsWith("wx") && (wxv.Replace(wxver, "$(WXVER)") > 0)) wlink_libs[i] = wxv;
    }

    //Parse release build for linux
    rt = FindXmlNode(cn, "Target", "title", "Release_Lin");
    if (rt == NULL) return "Release_Lin64" + tmp;
    AnalyzeNode(rt->GetChildren(), false, TARGET_NIX);

    //Parse rest: units
    cn = cn->GetNext();
    AnalyzeNode(cn, false, TARGET_BOTH);

    wxString autoconf_ac = "", //# AUTOCONF FILES GENERATED BY FFQUEUE DEBUG BUILD" + LBR + LBR,
             makefile_am = autoconf_ac,
             src_makefile_am = autoconf_ac,
             mfx;

    autoconf_ac += "AC_PREREQ([2.69])" + LBR;
    autoconf_ac += "AC_INIT([ffqueue], [" + wxString::Format("%li.%li.%li", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD) + "], [https://ffqueue.bruchhaus.dk/])" + LBR;
    autoconf_ac += "AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])" + LBR;
    autoconf_ac += "AC_CONFIG_SRCDIR([src/FFQApp.cpp])" + LBR;
    //autoconf_ac += "AM_INIT_AUTOMAKE" + LBR;
    autoconf_ac += "AC_PROG_CXX" + LBR;
    autoconf_ac += "WXVER=`wx-config --version | sed -e 's/\\.//g'`" + LBR;
    autoconf_ac += "AS_IF([test -z $WXVER], [ AC_MSG_ERROR([\"wx-config\" was not found. To correct this you ";
    autoconf_ac += "must either install the development version of wxWidgets (usual package name \"libwxgtk3.0-dev\" ";
    autoconf_ac += "or alike) OR if you have built your own version of wxWidgets you must ensure that \"wx-config\" ";
    autoconf_ac += "is added to the PATH environment variable.]) ], [])" + LBR;
    autoconf_ac += "AS_IF([test ${WXVER::3} -lt 302], [ AC_MSG_ERROR([wxWidgets must be at least version 3.0.2]) ], [])" + LBR;
    autoconf_ac += "AC_CONFIG_FILES([Makefile src/Makefile])" + LBR;
    autoconf_ac += "AC_CHECK_LIB([z], [compress2], [], [AC_MSG_ERROR([ZLib (libz) was not found!])])" + LBR;
    autoconf_ac += "AM_CONDITIONAL([FFQ_BASE_DIR],[test -f \"res/MainLogo.png\"])" + LBR;
    //autoconf_ac += "AM_CONDITIONAL([FFQ_AS_ROOT],[test `whoami` = root])" + LBR;
    //autoconf_ac += "AM_CONDITIONAL([FFQ_SNAP],[test -n \"$(SNAPCRAFT_PROJECT_NAME)\"])" + LBR;
    autoconf_ac += "LIBS=\"$LIBS `wx-config --libs` -lz\"" + LBR;
    autoconf_ac += "AC_OUTPUT" + LBR;

    makefile_am += "SUBDIRS=src" + LBR;
    makefile_am += "FFQ_BINARY=ffqueue" + LBR;
    makefile_am += "FFQ_XDGVER=`xdg-icon-resource --version 2> /dev/null`" + LBR;
    makefile_am += "if FFQ_BASE_DIR" + LBR;
    makefile_am += "FFQ_RES=res" + LBR;
    makefile_am += "else" + LBR;
    makefile_am += "FFQ_RES=../res" + LBR;
    makefile_am += "endif" + LBR;

    makefile_am += "ifeq ($(DESTDIR),)" + LBR + "FFQ_DEST=$(prefix)/share" + LBR + "else" + LBR + "FFQ_DEST=$(DESTDIR)/share" + LBR + "endif" + LBR;
    //makefile_am += "FFQ_DEST=$(DESTDIR)$(prefix)/share" + LBR;
    makefile_am += "FFQ_ICONDEST=$(FFQ_DEST)/icons/hicolor" + LBR;
    //makefile_am += "FFQ_LOCALE=../locale" + LBR;

    makefile_am += LBR + ".ONESHELL:" + LBR;
    makefile_am += "install-data-hook:" + LBR;
    //makefile_am += "\t @if [ \"$$(whoami)\" != \"root\" ]; then FFQ_DEST=`realpath ~/`/.local/share; fi" + LBR;
    //makefile_am += "\t @if [ ! -d \"$(FFQ_RES)\" ]; then FFQ_RES=res; fi" + LBR;
    makefile_am += "\t @if [ -z \"$(FFQ_XDGVER)\" ]; then" + LBR;
    tmp = "\t   @install -Dm644 $(FFQ_RES)/MainLogo#.png $(FFQ_ICONDEST)/#x#/apps/$(FFQ_BINARY).png" + LBR;
    ADD_ICONS(makefile_am);
    makefile_am += "\t   @install -Dm755 $(FFQ_RES)/$(FFQ_BINARY).desktop $(FFQ_DEST)/applications/$(FFQ_BINARY).desktop" + LBR;
    makefile_am += "\t @else" + LBR;
    tmp = "\t   @xdg-icon-resource install --novendor --size # $(FFQ_RES)/MainLogo#.png $(FFQ_BINARY)" + LBR;
    ADD_ICONS(makefile_am);
    makefile_am += "\t   @xdg-desktop-menu install --novendor $(FFQ_RES)/$(FFQ_BINARY).desktop" + LBR;
    makefile_am += "\t   @FFQ_DF=$(FFQ_DEST)/applications/$(FFQ_BINARY).desktop" + LBR;
    makefile_am += "\t   @if [ -f \"$$FFQ_DF\" ]; then chmod 755 \"$$FFQ_DF\"; fi" + LBR;
    makefile_am += "\t @fi" + LBR;

    makefile_am += LBR + ".ONESHELL:" + LBR;
    makefile_am += "uninstall-hook:" + LBR;
    makefile_am += "\t @if [ -z \"$(FFQ_XDGVER)\" ]; then" + LBR;
    tmp = "\t   @rm -f $(FFQ_ICONDEST)/#x#/apps/$(FFQ_BINARY).png" + LBR;
    ADD_ICONS(makefile_am);
    makefile_am += "\t   @rm -f $(FFQ_DEST)/applications/$(FFQ_BINARY).desktop" + LBR;
    makefile_am += "\t @else" + LBR;
    tmp = "\t   @xdg-icon-resource uninstall --novendor --size # $(FFQ_BINARY)" + LBR;
    ADD_ICONS(makefile_am);
    makefile_am += "\t   @xdg-desktop-menu uninstall --novendor $(FFQ_BINARY).desktop" + LBR;
    makefile_am += "\t @fi" + LBR;

    src_makefile_am += "bin_PROGRAMS=ffqueue" + LBR;
    src_makefile_am += "ffqueue_CXXFLAGS=`wx-config --debug=no --cxxflags` -std=c++11 -include ../wx_pch.h" + LBR;
    src_makefile_am += "ffqueue_LDFLAGS=-s" + LBR;
    src_makefile_am += "ffqueue_LDADD=@LIBS@" + LBR;
    src_makefile_am += "ffqueue_SOURCES=";

    wxString wmf = "# MAKEFILE GENERATED BY FFQUEUE DEBUG BUILD!", lmf = wmf;
    wmf += WBR + WBR;
    lmf += LBR + LBR;

    //Compiler directories
    wxString lcdirs = ArrayToStr(lcomp_dirs, "-I"), wcdirs = ArrayToStr(wcomp_dirs, "-I");
    wcdirs.Replace(LSEP, WSEP);
    lcdirs.Replace(WSEP, LSEP);

    //Linker directories
    wxString lldirs = ArrayToStr(llink_dirs, "-L"), wldirs = ArrayToStr(wlink_dirs, "-L");
    wldirs.Replace(LSEP, WSEP);
    lldirs.Replace(WSEP, LSEP);

    //Message
    tmp = "# The following line must point to the location of the GCC (g++) compiler on your system:";

    //Compilers and linkers
    wmf += tmp + WBR + "CXX:=g++.exe" + WBR + WBR;
    lmf += tmp + LBR + "CXX:=g++" + LBR + LBR;

    //Message
    tmp = "# If the linker does not equal the compiler declared above, you must modify the following line:";

    wmf += tmp + WBR + "LINKER:=$(CXX)" + WBR + WBR;
    lmf += tmp + LBR + "LINKER:=$(CXX)" + LBR + LBR;

    wmf += "# You may need to specify the full path to windres.exe in the following line:" + WBR + "WINDRES:=windres.exe" + WBR + WBR;
    //lmf += "WINDRES=??" + LBR + LBR; //UNUSED!

    //Message
    tmp = "# You may need to change the location to wxWidgets root / build folder in the following line:";

    //wxWidgets
    wmf += tmp + WBR + "WX:=..\\wxWidgets" + WBR + "WXVER:=" + wxver + WBR + WBR;
    lmf += tmp + LBR + "WX:=../wxWidgets/gtk-build" + LBR + "WXVER:=" + wxver + LBR + LBR;

    //Message
    tmp = "# If you want to change the default build / output folder you can do it in the following line:";

    //Make root
    wmf += tmp + WBR + "MAKEROOT:=build" + WBR + WBR;
    lmf += tmp + LBR + "MAKEROOT:=build" + LBR + LBR;

    //Message
    tmp = "# If you want to change the name of the linked binary, you can do it in the following line:";

    //Binary name
    wmf += tmp + WBR + "MAKEBINARY:=$(MAKEROOT)\\FFQueue.exe" + WBR + WBR;
    lmf += tmp + LBR + "FFQ_BINARY:=FFQueue" + LBR + "MAKEBINARY:=$(MAKEROOT)/$(FFQ_BINARY)" + LBR + LBR;

    //Message
    tmp = "# Compiler settings";
    tmp2 = "";//, you may need to change \"/gtk_build/\" to the folder you used to build wxWidgets:";

    //Compiler vars and dirs
    wmf += tmp + WBR + "COMPVARS:=-mwindows " + ArrayToStr(wcomp_vars, "") + WBR + WBR;
    lmf += tmp + tmp2 + LBR + "COMPVARS:=" + ArrayToStr(lcomp_vars, "") + LBR + LBR;

    tmp = "# Compiler search directories";
    wmf += tmp + WBR + "COMPDIRS:=-I. " + wcdirs + WBR + WBR;
    lmf += tmp + LBR + "COMPDIRS:=-I." + LBR + LBR;

    //Message
    tmp = "# Linker settings";

    //Linker vars, dirs and libs
    wmf += tmp + WBR + "LINKVARS:=-mwindows " + ArrayToStr(wlink_vars, "") + WBR + WBR;
    lmf += tmp + tmp2 + LBR + "LINKVARS:=" + ArrayToStr(llink_vars, "") + LBR + LBR;

    //Message
    tmp = "# Linker search directories";

    wmf += tmp + WBR + "LINKDIRS:=" + wldirs + WBR + WBR;
    lmf += tmp + LBR + "LINKDIRS:=" + LBR + LBR;

    //Message
    tmp = "# Linker libraries";

    wmf += tmp + WBR + "LINKLIBS:=" + ArrayToStr(wlink_libs, "-l", "\\" + WBR + "\t") + WBR + WBR;
    lmf += tmp + LBR + "LINKLIBS:=" + ArrayToStr(llink_libs, "-l", "\\" + LBR + "\t") + LBR + LBR;

    //The all rule
    //wmf += WBR + "all: paths $(MAKEBINARY)" + WBR + WBR + "$(MAKEBINARY):";
    //lmf += LBR + "all: paths $(MAKEBINARY)" + LBR + LBR + "$(MAKEBINARY):";

    //Message
    tmp = "# List of objects to link";

    wmf += tmp + WBR + "OBJECTS:=";
    lmf += tmp + LBR + "OBJECTS:=";

    wxString wmrs = "$(MAKEROOT)\\", lmrs = "$(MAKEROOT)/", wrecipes, lrecipes, wcur, lcur, /*wlink_list, llink_list,*/ ws, ls;
    wxArrayString subdirs;

    //Make recipes and rules
    for (unsigned int i = 0; i < units.Count(); i++)
    {

        //Get unit names
        lcur = units[i];
        wcur = lcur;

        //Path separators
        lcur.Replace(WSEP, LSEP);
        wcur.Replace(LSEP, WSEP);

        //Link names
        wxString wadd_link, ladd_link;

        if (wcur.at(0) == '$')
        {

            //Resource file: Windows only
            wcur.Remove(0, 1);
            AddSubDir(wcur, subdirs, WSEP);

            wadd_link = wmrs + CHEXT(wcur, "res");
            wadd_link.Replace("src", "obj");
            wrecipes += wadd_link + ":" + WBR +
                        "\t$(WINDRES) $(COMPDIRS) -J rc -O coff -DwxUSE_NO_MANIFEST -i " + wcur + " -o $@" + WBR + WBR;
                        //"\t$(WINDRES) $(COMPDIRS) -J rc -O coff -i " + wcur + " -o " + wmrs + wadd_link + WBR + WBR;

            ladd_link.Clear();

        }
        else
        {

            //Object files
            AddSubDir(wcur, subdirs, WSEP);

            wadd_link = wmrs + CHEXT(wcur, "o");
            wadd_link.Replace("src", "obj");

            ladd_link = lmrs + CHEXT(lcur, "o");
            ladd_link.Replace("src", "obj");

            wrecipes += wadd_link + ":" + WBR +
                        "\t$(CXX) $(COMPVARS) $(COMPDIRS) -c \"" + wcur + "\" -o $@" + WBR + WBR;
                        //"\t$(CXX) $(COMPVARS) $(COMPDIRS) -c \"" + wcur + "\" -o \"" + wmrs + wadd_link + "\"" + WBR + WBR;

            lrecipes += ladd_link + ":" + LBR +
                        "\t$(CXX) $(COMPVARS) $(COMPDIRS) -c \"" + lcur + "\" -o $@" + LBR + LBR;
                        //"\t$(CXX) $(COMPVARS) $(COMPDIRS) -c \"" + lcur + "\" -o \"" + lmrs + ladd_link + "\"" + LBR + LBR;

            if (i > 0) src_makefile_am += "\\" + LBR + "\t";
            lcur.Replace("src/","");
            src_makefile_am += lcur;

        }

        if (wadd_link.Len() > 0)
        {
            //Add to win
            wmf += "\\" + WBR + "\t" + wadd_link;
            //wlink_list += "\"" + wadd_link + "\"\\" + WBR + "\t";
        }

        if (ladd_link.Len() > 0)
        {
            //Add to lin
            lmf += "\\" + LBR + "\t" + ladd_link;
            //llink_list += "\"" + ladd_link + "\"\\" + LBR + "\t";
        }

    }

    //Create the clean and paths recipes
    wxString wdel = "\tdel \"" + wmrs, ldel = "\trm \"" + lmrs;

    tmp2 = ".PHONY: clean";
    wxString wclean = tmp2 + WBR + "clean:" + WBR + wdel + "*.o\"" + WBR + wdel + "*.res\"" + WBR,
             lclean = tmp2 + LBR + "clean:" + LBR + ldel + "*.o\"" + LBR;

    //Message
    tmp = "# The all recipe for a complete make";

    //All recipes
    wmf += WBR + WBR + tmp + WBR + wxString::Format(RCP_ALL, WBR) + WBR + WBR;
    lmf += LBR + LBR + tmp + LBR + wxString::Format(RCP_ALL, LBR) + LBR + LBR;

    //Message
    tmp = "# Recipe for creating required paths";

    //Paths recipe
    tmp2 = PHONY + "paths";
    wmf += tmp + WBR + tmp2 + WBR + "paths:" + WBR + "\t" + wxString::Format(MKDIR_WIN, "", "") + WBR;
    lmf += tmp + LBR + tmp2 + LBR + "paths:" + LBR + "\t" + wxString::Format(MKDIR_LIN, "") + LBR;

    //Clean recipes
    for (unsigned int sid = 0; sid < subdirs.Count(); sid++)
    {

        tmp = subdirs[sid];
        tmp.Replace("src", "obj");

        tmp.Replace(LSEP, WSEP);
        wmf += "\t" + wxString::Format(MKDIR_WIN, WSEP + tmp, WSEP + tmp) + WBR;
        wclean += wdel + tmp + WSEP + "*.o\"" + WBR;
        wclean += wdel + tmp + WSEP + "*.res\"" + WBR;

        tmp.Replace(WSEP, LSEP);
        lmf += "\t" + wxString::Format(MKDIR_LIN, LSEP + tmp) + LBR;

        lclean += ldel + tmp + LSEP + "*.o\"" + LBR;

    }

    //Message
    tmp = "# Recipes used for cleanup";

    //Add to makefiles
    wmf += WBR + tmp + WBR + wclean;
    lmf += LBR + tmp + LBR + lclean;

    //Install and uninstall, Linux only
    lmf += LBR + "# Install & uninstall targets" + LBR;
    lmf += "ifeq ($(DESTDIR),)" + LBR + "DESTDIR:=/usr" + LBR + "endif" + LBR + LBR;
    lmf += PHONY + "install" + LBR + "install:" + LBR;
    lmf += "\t@echo Installing to $(DESTDIR)" + LBR;
    lmf += "\t@install -Dm755 $(MAKEBINARY) $(DESTDIR)/bin/$(FFQ_BINARY)" + LBR;
    lmf += "\t@install -Dm755 res/ffqueue.desktop $(DESTDIR)/share/applications/$(FFQ_BINARY).desktop" + LBR;
    lmf += "\t@sed -ie 's/^Exec=.*$/Exec='\"$(FFQ_BINARY)\"'/' $(DESTDIR)/share/applications/$(FFQ_BINARY).desktop" + LBR;
    lmf += "\t@sed -ie 's/^Icon=.*$/Icon='\"$(FFQ_BINARY)\"'/' $(DESTDIR)/share/applications/$(FFQ_BINARY).desktop" + LBR;
    tmp = "\t@install -Dm644 res/MainLogo#.png $(DESTDIR)/share/icons/hicolor/#x#/apps/$(FFQ_BINARY).png" + LBR;
    ADD_ICONS(lmf);

    lmf += LBR + PHONY + "uninstall" + LBR + "uninstall:" + LBR;
    lmf += "\t@echo Uninstalling from $(DESTDIR)" + LBR;
    lmf += "\t@rm -f $(DESTDIR)/bin/$(FFQ_BINARY)" + LBR;
    lmf += "\t@rm -f $(DESTDIR)/share/applications/$(FFQ_BINARY).desktop" + LBR;
    tmp = "\t@rm -f $(DESTDIR)/share/icons/hicolor/#x#/apps/$(FFQ_BINARY).png" + LBR;
    ADD_ICONS(lmf);

    //Message
    tmp = "# Receipe to compile the objects and link the binary";

    //Add makebinary recipe
    wmf += WBR + WBR + tmp + WBR + "$(MAKEBINARY): $(OBJECTS)" + WBR +
           "\t$(LINKER) $(LINKDIRS) -o $(MAKEBINARY) $(OBJECTS) $(LINKVARS) $(LINKLIBS)";

    lmf += LBR + LBR + tmp + LBR + "$(MAKEBINARY): $(OBJECTS)" + LBR +
           "\t$(LINKER) $(LINKDIRS) -o $(MAKEBINARY) $(OBJECTS) $(LINKVARS) $(LINKLIBS)";

    //Message
    tmp = "# Recipes to create the individual objects and resources";

    //Add the recipes
    wmf += WBR + WBR + tmp + LBR + wrecipes;
    lmf += LBR + LBR + tmp + LBR + lrecipes;

    //Save the makefiles
    SaveString(root + CSEP + "Makefile.win", wmf);
    SaveString(root + CSEP + "Makefile.lin", lmf);
    SaveString(root + CSEP + "configure.ac", autoconf_ac);
    SaveString(root + CSEP + "Makefile.am", makefile_am);
    SaveString(root + CSEP + "src" + CSEP + "Makefile.am", src_makefile_am);

    FFQConsole::Get()->AppendLine(wmf + "\n\n" + lmf + "\n\n" + autoconf_ac, COLOR_BLUE);
    FFQConsole::Get()->AppendLine(wmf + "\n\n" + lmf + "\n\n" + makefile_am, COLOR_BLUE);
    FFQConsole::Get()->AppendLine(wmf + "\n\n" + lmf + "\n\n" + src_makefile_am, COLOR_BLUE);

    /*DumpArray("comp_vars", comp_vars);
    DumpArray("comp_dirs", comp_dirs);
    DumpArray("link_vars", link_vars);
    DumpArray("link_dirs", link_dirs);
    DumpArray("link_libs", link_libs);
    DumpArray("units", units);*/

    return "";

}
