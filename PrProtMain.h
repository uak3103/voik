/***************************************************************
 * Name:      PrProtMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2012-12-21
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef PRPROTMAIN_H
#define PRPROTMAIN_H

//(*Headers(PrProtDialog)
//#include <wx/sizer.h>
//#include <wx/textctrl.h>
//#include <wx/statline.h>
//#include <wx/button.h>
//#include <wx/dialog.h>
//*)

class PrProtDialog: public wxDialog
{
    public:

        PrProtDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~PrProtDialog();
        void ldFile(wxString f);
        wxString fn;

    private:

        //(*Handlers(PrProtDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOkButtonClick(wxCommandEvent& event);
        void OnLbButtonClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(PrProtDialog)
        static const long ID_TEXTCTRL1;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON1;
        //*)

        //(*Declarations(PrProtDialog)
        wxBoxSizer* BoxSizer2;
        wxButton* Button11;
        wxButton* Button22;
        //wxStaticLine* StaticLine1;
        wxBoxSizer* BoxSizer1;
        wxTextCtrl* TextCtrl1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PRPROTMAIN_H
