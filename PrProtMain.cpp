/***************************************************************
 * Name:      PrProtMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2012-12-21
 * Copyright:  ()
 * License:
 **************************************************************/

#include "PrProtMain.h"
//#include <wx/msgdlg.h>

//(*InternalHeaders(PrProtDialog)
//#include <wx/intl.h>
//#include <wx/string.h>
//#include <wx/file.h>
//*)

//(*IdInit(PrProtDialog)
const long PrProtDialog::ID_TEXTCTRL1 = wxNewId();
const long PrProtDialog::ID_STATICLINE1 = wxNewId();
const long PrProtDialog::ID_BUTTON2 = wxNewId();
const long PrProtDialog::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PrProtDialog,wxDialog)
    //(*EventTable(PrProtDialog)
    //*)
END_EVENT_TABLE()

PrProtDialog::PrProtDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PrProtDialog)
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("Text\n111\n22\n"), wxDefaultPosition, wxSize(585,533), wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    BoxSizer1->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    //StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    //BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Button22= new wxButton(this, ID_BUTTON2, _("Закрыть"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(Button22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Button11 = new wxButton(this, ID_BUTTON1, _("Очистить"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(Button11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, wxDLG_UNIT(this,wxSize(4,0)).GetWidth());
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PrProtDialog::OnOkButtonClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PrProtDialog::OnLbButtonClick);
    //*)
}

PrProtDialog::~PrProtDialog()
{
    //(*Destroy(PrProtDialog)
    //*)
}

void PrProtDialog::OnOkButtonClick(wxCommandEvent& event)
{
    if (IsModal())
        EndModal(wxID_OK);
}

void PrProtDialog::OnLbButtonClick(wxCommandEvent& event)
{
    wxFile f;
    f.Create(fn,1);     /*<wx/filefn.h> wxFileName::wxRemoveFile(fn)*/
    f.Flush();
    ldFile(fn);
}

void PrProtDialog::ldFile(wxString f)
{
    if (wxFile::Exists(f))
        TextCtrl1->LoadFile(f);
}

