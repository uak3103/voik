/***************************************************************
 * Name:      DatChoiceMain.h
 * Purpose:   Defines Application Frame
 * Author:    uak (uak103@mail.ru)
 * Created:   2012-11-01
 * Copyright: uak ()
 * License:
 **************************************************************/

#ifndef DATCHOICEMAIN_H
#define DATCHOICEMAIN_H

//(*Headers(DatChoiceDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class DatChoiceDialog: public wxDialog
{
    public:

        DatChoiceDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~DatChoiceDialog();
        wxString sCam;
        wxString sCadr;
        wxString sBright;

    private:

        //(*Handlers(DatChoiceDialog)
        void OnOkButtonClick(wxCommandEvent& event);
        void OnCancelButtonClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(DatChoiceDialog)
        static const long ID_STATICTEXT2;
        static const long ID_CHOICE1;
        static const long ID_STATICTEXT3;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT4;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        //*)

        //(*Declarations(DatChoiceDialog)
        wxStaticText* StaticText2;
        wxButton* OkButton;
        wxStaticText* StaticText3;
        wxTextCtrl* TextCtrl2;
        wxBoxSizer* BoxSizer1;
        wxTextCtrl* TextCtrl1;
        wxStaticText* StaticText4;
        wxButton* CancelButton;
        wxChoice* Choice1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // DATCHOICEMAIN_H
