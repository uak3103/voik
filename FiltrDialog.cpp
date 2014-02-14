#include "wx_pch.h"
#include "FiltrDialog.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FiltrDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FiltrDialog)
#include <wx/button.h>
//*)

//(*IdInit(FiltrDialog)
const long FiltrDialog::ID_VERT_FILTR = wxNewId();
const long FiltrDialog::ID_STATICTEXT1 = wxNewId();
const long FiltrDialog::ID_VERT_OFFSET = wxNewId();
const long FiltrDialog::ID_HORZ_FILTR = wxNewId();
const long FiltrDialog::ID_STATICTEXT2 = wxNewId();
const long FiltrDialog::ID_HORZ_OFFSET = wxNewId();
//*)

BEGIN_EVENT_TABLE(FiltrDialog,wxDialog)
	//(*EventTable(FiltrDialog)
	//*)
END_EVENT_TABLE()

FiltrDialog::FiltrDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FiltrDialog)
	wxStaticText* StaticText2;
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, _("Фильтрация"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(570,141));
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_wndVertFiltr = new wxCheckBox(this, ID_VERT_FILTR, _("Вертикальное вычитание"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_VERT_FILTR"));
	m_wndVertFiltr->SetValue(false);
	BoxSizer2->Add(m_wndVertFiltr, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Смещение (строк):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	m_wndVertOffset = new wxSpinCtrl(this, ID_VERT_OFFSET, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_VERT_OFFSET"));
	m_wndVertOffset->SetValue(_T("0"));
	BoxSizer2->Add(m_wndVertOffset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	m_wndHorzFiltr = new wxCheckBox(this, ID_HORZ_FILTR, _("Горизонтальное вычитание"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_HORZ_FILTR"));
	m_wndHorzFiltr->SetValue(false);
	BoxSizer3->Add(m_wndHorzFiltr, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Смещение (пикселей):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer3->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_wndHorzOffset = new wxSpinCtrl(this, ID_HORZ_OFFSET, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_HORZ_OFFSET"));
	m_wndHorzOffset->SetValue(_T("0"));
	BoxSizer3->Add(m_wndHorzOffset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_VERT_FILTR,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FiltrDialog::OnVertFiltrClick);
	Connect(ID_HORZ_FILTR,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FiltrDialog::OnHorzFiltrClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&FiltrDialog::OnInit);
	//*)
	m_wndHorzFiltr->SetValidator(wxGenericValidator(&m_bHorzFiltr));
	m_wndVertFiltr->SetValidator(wxGenericValidator(&m_bVertFiltr));
	m_wndHorzOffset->SetValidator(wxGenericValidator(&m_nHorzOffset));
	m_wndVertOffset->SetValidator(wxGenericValidator(&m_nVertOffset));

}

FiltrDialog::~FiltrDialog()
{
	//(*Destroy(FiltrDialog)
	//*)
}


void FiltrDialog::OnVertFiltrClick(wxCommandEvent& event)
{
	m_wndVertOffset->Enable(event.GetInt()==1);
}

void FiltrDialog::OnHorzFiltrClick(wxCommandEvent& event)
{
	m_wndHorzOffset->Enable(event.GetInt()==1);
}

void FiltrDialog::OnInit(wxInitDialogEvent& event)
{
	TransferDataToWindow();

	m_wndVertOffset->Enable(m_bVertFiltr);
	m_wndHorzOffset->Enable(m_bHorzFiltr);
}
