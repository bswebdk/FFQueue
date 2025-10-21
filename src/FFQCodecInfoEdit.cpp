#include "FFQCodecInfoEdit.h"
#include "utils/FFQLang.h"
#include "utils/FFQMisc.h"
#include "utils/FFQConfig.h"

#ifdef DEBUG
  #include "utils/FFQConsole.h"
#endif

#ifndef WX_PRECOMP
    //(*InternalHeadersPCH(FFQCodecInfoEdit)
    #include <wx/string.h>
    //*)
#endif
//(*InternalHeaders(FFQCodecInfoEdit)
//*)

//(*IdInit(FFQCodecInfoEdit)
const long FFQCodecInfoEdit::ID_QSMIN = wxNewId();
const long FFQCodecInfoEdit::ID_QSMAX = wxNewId();
const long FFQCodecInfoEdit::ID_QSFLOAT = wxNewId();
const long FFQCodecInfoEdit::ID_CRFMIN = wxNewId();
const long FFQCodecInfoEdit::ID_CRFMAX = wxNewId();
const long FFQCodecInfoEdit::ID_CRFFLOAT = wxNewId();
const long FFQCodecInfoEdit::ID_RESETBTN = wxNewId();
const long FFQCodecInfoEdit::ID_OKBTN = wxNewId();
const long FFQCodecInfoEdit::ID_CANCELBTN = wxNewId();
//*)

BEGIN_EVENT_TABLE(FFQCodecInfoEdit,wxDialog)
    //(*EventTable(FFQCodecInfoEdit)
    //*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

/*void UpdateValidator(wxTextCtrl *tc, bool is_float)
{
    if (is_float)
    {
        float v = Str2Long(tc->GetValue(), 0);
        wxFloatingPointValidator<float> fv(nullptr, wxNUM_VAL_NO_TRAILING_ZEROES);
        fv.SetRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
        tc->SetValidator(fv);
        tc->SetValue(ToStr(v));
    }
    else
    {
        int v = Str2Float(tc->GetValue(), 0);
        wxIntegerValidator<int> iv(nullptr, wxNUM_VAL_DEFAULT);
        iv.SetRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        tc->SetValidator(iv);
        tc->SetValue(ToStr(v));
    }
}

//---------------------------------------------------------------------------------------

void UpdateValidators(wxTextCtrl *lv, wxTextCtrl *hv, bool is_float)
{
    UpdateValidator(lv, is_float);
    UpdateValidator(hv, is_float);
}*/

//---------------------------------------------------------------------------------------

wxString n2str(float v, bool flt) { return flt ? wxString::Format("%.2f", v) : wxString::Format("%i", froundi(v)); }

//---------------------------------------------------------------------------------------

void SetCtrlValues(wxTextCtrl *tl, wxTextCtrl *th, wxCheckBox *cf, bool valid, bool flt, float lv, float hv)
{
    if (valid)
    {
        cf->SetValue(flt);
        //tl->SetValue(ToStr(flt ? lv : (int)lv));
        tl->SetValue(n2str(lv, flt));
        th->SetValue(n2str(hv, flt));
    }
    else
    {
        cf->SetValue(false);
        tl->SetValue("0");//Clear();
        th->SetValue("0");//Clear();
    }
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

FFQCodecInfoEdit::FFQCodecInfoEdit(wxWindow* parent, int id)
{
    //(*Initialize(FFQCodecInfoEdit)
    wxBoxSizer* BoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxStaticText* ST1;
    wxStaticText* ST2;
    wxStaticText* ST3;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxDefaultSize);
    Move(wxDefaultPosition);
    FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer1->AddGrowableRow(0);
    FlexGridSizer2 = new wxFlexGridSizer(3, 4, 0, 0);
    FlexGridSizer2->AddGrowableCol(0);
    FlexGridSizer2->Add(-1,-1,1, wxALL|wxEXPAND, 3);
    ST1 = new wxStaticText(this, wxID_ANY, _T("L"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
    ST1->SetLabel(FFQS(SID_PRESET_LOW));
    FlexGridSizer2->Add(ST1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    ST2 = new wxStaticText(this, wxID_ANY, _T("H"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
    ST2->SetLabel(FFQS(SID_PRESET_HIGH));
    FlexGridSizer2->Add(ST2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer2->Add(-1,-1,1, wxALL|wxEXPAND, 3);
    ST3 = new wxStaticText(this, wxID_ANY, _T("Qs"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
    ST3->SetLabel(FFQS(SID_PRESET_QUALITY_SCALE));
    FlexGridSizer2->Add(ST3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    QScaleMin = new wxTextCtrl(this, ID_QSMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_QSMIN"));
    FlexGridSizer2->Add(QScaleMin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    QScaleMax = new wxTextCtrl(this, ID_QSMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_QSMAX"));
    FlexGridSizer2->Add(QScaleMax, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    QScaleFloat = new wxCheckBox(this, ID_QSFLOAT, _T("F"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_QSFLOAT"));
    QScaleFloat->SetValue(false);
    QScaleFloat->SetLabel(FFQS(SID_PRESET_FLOATING_POINT));
    FlexGridSizer2->Add(QScaleFloat, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    ST4 = new wxStaticText(this, wxID_ANY, _T("Crf"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
    ST4->SetLabel(FFQS(SID_PRESET_CONSTANT_QUALITY));
    FlexGridSizer2->Add(ST4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 3);
    CRFMin = new wxTextCtrl(this, ID_CRFMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CRFMIN"));
    FlexGridSizer2->Add(CRFMin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    CRFMax = new wxTextCtrl(this, ID_CRFMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CRFMAX"));
    FlexGridSizer2->Add(CRFMax, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    CRFFloat = new wxCheckBox(this, ID_CRFFLOAT, _T("F"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CRFFLOAT"));
    CRFFloat->SetValue(false);
    CRFFloat->SetLabel(FFQS(SID_PRESET_FLOATING_POINT));
    FlexGridSizer2->Add(CRFFloat, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    ResetBtn = new wxButton(this, ID_RESETBTN, _T("R"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RESETBTN"));
    ResetBtn->SetLabel(FFQS(SID_COMMON_RESET));
    BoxSizer1->Add(ResetBtn, 1, wxALL|wxEXPAND, 3);
    BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    OkBtn = new wxButton(this, ID_OKBTN, _T("O"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OKBTN"));
    OkBtn->SetDefault();
    OkBtn->SetLabel(FFQS(SID_COMMON_OK));
    BoxSizer1->Add(OkBtn, 1, wxALL|wxEXPAND, 3);
    CancelBtn = new wxButton(this, ID_CANCELBTN, _T("C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBTN"));
    CancelBtn->SetLabel(FFQS(SID_COMMON_CANCEL));
    BoxSizer1->Add(CancelBtn, 1, wxALL|wxEXPAND, 3);
    FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_QSFLOAT,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQCodecInfoEdit::OnButtonClick);
    Connect(ID_CRFFLOAT,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FFQCodecInfoEdit::OnButtonClick);
    Connect(ID_RESETBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQCodecInfoEdit::OnButtonClick);
    Connect(ID_OKBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQCodecInfoEdit::OnButtonClick);
    Connect(ID_CANCELBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FFQCodecInfoEdit::OnButtonClick);
    //*)

    m_FloatVal = new wxFloatingPointValidator<float>(2, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES);
    m_FloatVal->SetRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
    m_IntVal = new wxIntegerValidator<int>(nullptr, wxNUM_VAL_DEFAULT);
    m_IntVal->SetRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    /*wxString s = "0123456789-";
    m_IntVal = new wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
    m_IntVal->AddCharIncludes(s);

    s += ".";
    m_FloatVal = new wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
    m_FloatVal->AddCharIncludes(s);*/

}

//---------------------------------------------------------------------------------------

FFQCodecInfoEdit::~FFQCodecInfoEdit()
{
    //(*Destroy(FFQCodecInfoEdit)
    //*)
}

//---------------------------------------------------------------------------------------

bool FFQCodecInfoEdit::Execute(LPCODEC_INFO ci, wxString codec, bool video)
{

    SetTitle((video ? FFQS(SID_COMMON_VIDEO) : FFQS(SID_COMMON_AUDIO)) + " - " + codec);

    ST4->Enable(video);
    CRFMin->Enable(video);
    CRFMax->Enable(video);
    CRFFloat->Enable(video);

    m_CodecInfo = ci;
    m_Codec = codec;
    SetControlValues();

    #ifdef DEBUG
    FFQConsole::Get()->AppendLine(wxString::Format("%s: %s qscale(%f %f %i) crf(%f %f %i)", video ? "vid" : "aud", ci->codec, ci->min_qscale, ci->max_qscale, (int)ci->qscale_float,  ci->min_crf, ci->max_crf, (int)ci->crf_float), COLOR_BLUE);
    #endif

    int res = ShowModal();

    if (res == wxID_OK)
    {

        //Update info
        ci->codec = codec;
        ci->qscale_float = QScaleFloat->GetValue();
        ci->min_qscale = Str2Float(QScaleMin->GetValue(), ci->min_qscale);
        ci->max_qscale = Str2Float(QScaleMax->GetValue(), ci->max_qscale);
        ci->crf_float = CRFFloat->GetValue();
        ci->min_crf = Str2Float(CRFMin->GetValue(), ci->min_crf);
        ci->max_crf = Str2Float(CRFMax->GetValue(), ci->max_crf);

        //Get as string
        wxString s = ci->ToString();

        //Update config and save
        FFQCFG()->GetCodecInfo()->Append(s);
        FFQCFG()->SaveConfig();

        //Return changes made
        return true;

    }
    else if (res == wxID_CLEAR)
    {

        //Prevent modification of defaults
        if ((video && (m_CodecInfo->codec != DEFAULT_VIDEO_CODEC_INFO)) || ((!video) && (m_CodecInfo->codec != DEFAULT_AUDIO_CODEC_INFO)))
        {

            //Get default info
            LPCODEC_INFO def_root = MakeDefaultCodecInfo();
            LPCODEC_INFO def = def_root->Find(m_CodecInfo->codec);

            //Get the info loaded from config
            LPCODEC_INFO cfg_root = FFQCFG()->GetCodecInfo();
            LPCODEC_INFO inf = cfg_root->Find(m_CodecInfo->codec);

            if (def)
            {
                //Reset to default and save config
                def->CopyTo(*inf);
                FFQCFG()->SaveConfig();
            }
            else
            {
                //Delete and update & save config
                FFQCFG()->SetCodecInfo(DeleteCodecInfo(cfg_root, inf));
            }

            //Release copy of defaults
            delete def_root;

            //Return changes made
            return true;

        }

    }

    //Return no changes made
    return false;

}

//---------------------------------------------------------------------------------------

void FFQCodecInfoEdit::SetControlValues(LPCODEC_INFO ci)
{
    if (!ci) ci = m_CodecInfo;
    SetCtrlValues(QScaleMin, QScaleMax, QScaleFloat, ci->GetQScale(0, nullptr), ci->qscale_float, ci->min_qscale, ci->max_qscale);
    //UpdateValidators(QScaleMin, QScaleMax, QScaleFloat->GetValue());
    SetCtrlValues(CRFMin, CRFMax, CRFFloat, ci->GetConstRate(0, nullptr), ci->crf_float, ci->min_crf, ci->max_crf);
    //UpdateValidators(CRFMin, CRFMax, CRFFloat->GetValue());
    UpdateValidators();
}

//---------------------------------------------------------------------------------------

void FFQCodecInfoEdit::UpdateValidator(wxTextCtrl *tc, bool is_float)
{
    if (!tc->GetValidator())
    {
        if (is_float) tc->SetValidator(*m_FloatVal);
        else tc->SetValidator(*m_IntVal);
    }
    else if (is_float)
    {
        //float v = tc->GetValidator() ? Str2Long(tc->GetValue(), 0) : Str2Float(tc->GetValue(), 0);
        float v = Str2Long(tc->GetValue(), 0);
        tc->SetValidator(*m_FloatVal);
        tc->SetValue(n2str(v, true));// ToStr(v));
    }
    else
    {
        //int v = tc->GetValidator() ? Str2Float(tc->GetValue(), 0) : Str2Long(tc->GetValue(), 0);
        int v = Str2Float(tc->GetValue(), 0);
        tc->SetValidator(*m_IntVal);
        tc->SetValue(n2str(v, false));// ToStr(v));
    }
}

//---------------------------------------------------------------------------------------

void FFQCodecInfoEdit::UpdateValidators()
{
    UpdateValidator(QScaleMin, QScaleFloat->GetValue());
    UpdateValidator(QScaleMax, QScaleFloat->GetValue());
    UpdateValidator(CRFMin, CRFFloat->GetValue());
    UpdateValidator(CRFMax, CRFFloat->GetValue());
}

//---------------------------------------------------------------------------------------

void FFQCodecInfoEdit::OnButtonClick(wxCommandEvent& event)
{
    if (event.GetId() == ID_RESETBTN) EndModal(wxID_CLEAR);
    else if (event.GetId() == ID_OKBTN) EndModal(wxID_OK);
    else if (event.GetId() == ID_CANCELBTN) EndModal(wxID_CANCEL);
    else if (event.GetId() == ID_QSFLOAT) UpdateValidators();//(QScaleMin, QScaleMax, QScaleFloat->GetValue());
    else if (event.GetId() == ID_CRFFLOAT) UpdateValidators();//(CRFMin, CRFMax, CRFFloat->GetValue());
}
