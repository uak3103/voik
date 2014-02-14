/***************************************************************
 * Name:      PrintProtMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2012-12-21
 * Copyright:  ()
 * License:
 **************************************************************/

#include "PrintProtMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(PrintProtFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(PrintProtFrame)
const long PrintProtFrame::ID_TEXTCTRL1 = wxNewId();
const long PrintProtFrame::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PrintProtFrame,wxFrame)
    //(*EventTable(PrintProtFrame)
    //*)
END_EVENT_TABLE()

PrintProtFrame::PrintProtFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PrintProtFrame)
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer1;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(682,577));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(594,508), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    TextCtrl1 = new wxTextCtrl(Panel1, ID_TEXTCTRL1, _("Text1\ntext2\n"), wxDefaultPosition, wxSize(662,563), wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    BoxSizer2->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer2);
    BoxSizer2->SetSizeHints(Panel1);
    BoxSizer1->Add(Panel1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&PrintProtFrame::OnTextCtrl1Text);
    fn="/home/user/Bitmaps/PKVI1.prt";
    if (wxFile::Exists(fn))
        TextCtrl1->LoadFile(fn);
    //*)
}

PrintProtFrame::~PrintProtFrame()
{
    //(*Destroy(PrintProtFrame)
    //*)
}

void PrintProtFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void PrintProtFrame::OnAbout(wxCommandEvent& event)
{
}

void PrintProtFrame::OnTextCtrl1Text(wxCommandEvent& event)
{
}
