/***************************************************************
 * Name:      PrintProtMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2012-12-21
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef PRINTPROTMAIN_H
#define PRINTPROTMAIN_H

//(*Headers(PrintProtFrame)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/file.h>
//*)

class PrintProtFrame: public wxFrame
{
    public:

        PrintProtFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PrintProtFrame();
        wxString fn;

    private:

        //(*Handlers(PrintProtFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTextCtrl1Text(wxCommandEvent& event);
        //*)

        //(*Identifiers(PrintProtFrame)
        static const long ID_TEXTCTRL1;
        static const long ID_PANEL1;
        //*)

        //(*Declarations(PrintProtFrame)
        wxPanel* Panel1;
        wxTextCtrl* TextCtrl1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PRINTPROTMAIN_H
