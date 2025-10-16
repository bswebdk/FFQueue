#ifndef FFQCODECINFOEDIT_H
#define FFQCODECINFOEDIT_H

#include <wx/valnum.h>
#include "utils/FFQCodecInfo.h"

#ifndef WX_PRECOMP
    //(*HeadersPCH(FFQCodecInfoEdit)
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/dialog.h>
    #include <wx/sizer.h>
    #include <wx/stattext.h>
    #include <wx/textctrl.h>
    //*)
#endif
//(*Headers(FFQCodecInfoEdit)
//*)


class FFQCodecInfoEdit: public wxDialog
{
    public:

        FFQCodecInfoEdit(wxWindow* parent, int id = wxID_ANY);
        virtual ~FFQCodecInfoEdit();

        bool Execute(LPCODEC_INFO ci, wxString codec, bool video = true);

        //(*Declarations(FFQCodecInfoEdit)
        wxButton* CancelBtn;
        wxButton* OkBtn;
        wxButton* ResetBtn;
        wxCheckBox* CRFFloat;
        wxCheckBox* QScaleFloat;
        wxStaticText* ST4;
        wxTextCtrl* CRFMax;
        wxTextCtrl* CRFMin;
        wxTextCtrl* QScaleMax;
        wxTextCtrl* QScaleMin;
        //*)

    protected:

        //(*Identifiers(FFQCodecInfoEdit)
        static const long ID_QSMIN;
        static const long ID_QSMAX;
        static const long ID_QSFLOAT;
        static const long ID_CRFMIN;
        static const long ID_CRFMAX;
        static const long ID_CRFFLOAT;
        static const long ID_RESETBTN;
        static const long ID_OKBTN;
        static const long ID_CANCELBTN;
        //*)

    private:

        //int m_Integer;
        //float m_Float;
        //wxArrayString m_IntChars, m_FloatChars;
        //wxTextValidator *m_IntVal, *m_FloatVal;
        wxIntegerValidator<int> *m_IntVal;
        wxFloatingPointValidator<float> *m_FloatVal;
        LPCODEC_INFO m_CodecInfo;
        wxString m_Codec;

        void SetControlValues(LPCODEC_INFO ci = nullptr);
        void UpdateValidator(wxTextCtrl *tc, bool is_float);
        void UpdateValidators();

        //(*Handlers(FFQCodecInfoEdit)
        void OnButtonClick(wxCommandEvent& event);
        //*)

        DECLARE_EVENT_TABLE()
};

#endif
