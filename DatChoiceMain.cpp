/***************************************************************
 * Name:      DatChoiceMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    uak (uak103@mail.ru)
 * Created:   2012-11-01
 * Copyright: uak ()
 * License:
 **************************************************************/

#include "DatChoiceMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(DatChoiceDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(DatChoiceDialog)
const long DatChoiceDialog::ID_STATICTEXT2 = wxNewId();
const long DatChoiceDialog::ID_CHOICE1 = wxNewId();
const long DatChoiceDialog::ID_STATICTEXT3 = wxNewId();
const long DatChoiceDialog::ID_TEXTCTRL1 = wxNewId();
const long DatChoiceDialog::ID_STATICTEXT4 = wxNewId();
const long DatChoiceDialog::ID_TEXTCTRL2 = wxNewId();
const long DatChoiceDialog::ID_BUTTON3 = wxNewId();
const long DatChoiceDialog::ID_BUTTON4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(DatChoiceDialog,wxDialog)
    //(*EventTable(DatChoiceDialog)
    //*)
END_EVENT_TABLE()

DatChoiceDialog::DatChoiceDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(DatChoiceDialog)
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer3;

    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Камера\n"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer4->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4->Add(44,20,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    Choice1 = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice1->SetSelection( Choice1->Append(_("БИК")) );
    Choice1->Append(_("ШПК"));
    Choice1->Append(_("ЛИК"));
    Choice1->Append(_("БПК"));
    BoxSizer4->Add(Choice1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(BoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Кадр\n"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    BoxSizer5->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("2048-1024"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    BoxSizer5->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(BoxSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Яркость"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    BoxSizer6->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl2 = new wxTextCtrl(this, ID_TEXTCTRL2, _("128-1920"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    BoxSizer6->Add(TextCtrl2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(BoxSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    OkButton = new wxButton(this, ID_BUTTON3, _("Ввод\n"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer7->Add(OkButton, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    CancelButton = new wxButton(this, ID_BUTTON4, _("Отмена"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer7->Add(CancelButton, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(BoxSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DatChoiceDialog::OnOkButtonClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DatChoiceDialog::OnCancelButtonClick);
    //*)
    wxString wxs1=TextCtrl1->GetValue();
    wxString wxs2=TextCtrl2->GetValue();
}

DatChoiceDialog::~DatChoiceDialog()
{
    //(*Destroy(DatChoiceDialog)
    //*)
}

void DatChoiceDialog::OnOkButtonClick(wxCommandEvent& event)
{
    TransferDataFromWindow();
    sCam=Choice1->GetString(Choice1->GetSelection());
    sCadr=TextCtrl1->GetValue();
    sBright=TextCtrl2->GetValue();
    if (IsModal())
        EndModal(wxID_OK);
}

void DatChoiceDialog::OnCancelButtonClick(wxCommandEvent& event)
{
    if (IsModal())
        EndModal(wxID_CANCEL);
}
