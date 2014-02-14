#include "wx_pch.h"
#include "PKVIMain.h"
#include "CanPanel.h"
#include "can4linux.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CanPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(CanPanel)
//*)
//u_int32_t g_nCommandNum=0;
/*
//(*IdInit(CanPanel)
const long CanPanel::ID_PROP_COMMAND = wxNewId();
const long CanPanel::ID_LISTCTRL2 = wxNewId();
const long CanPanel::ID_SEND_COMMAND = wxNewId();
const long CanPanel::ID_STATICTEXT1 = wxNewId();
const long CanPanel::ID_SECONDS = wxNewId();
const long CanPanel::ID_STATICTEXT2 = wxNewId();
//*)
*/
const long CanPanel::ID_PROP_COMMAND = wxNewId();
const long CanPanel::ID_PROP_STATUS = wxNewId();
const long CanPanel::ID_SEND_COMMAND = wxNewId();
const long CanPanel::ID_STATICTEXT1 = wxNewId();
const long CanPanel::ID_AUTO_STATUS = wxNewId();
const long CanPanel::ID_STATICTEXT2 = wxNewId();
const long CanPanel::ID_STATICTEXT3 = wxNewId();
const long CanPanel::ID_CAN_INTERFACE = wxNewId();
const long CanPanel::ID_STATICTEXT4 = wxNewId();
const long CanPanel::ID_CAMERA_NUM = wxNewId();
const long ID_TIMER_AUTO = wxNewId();
const long ID_TIMER_GET_PARAMETERS = wxNewId();

BEGIN_EVENT_TABLE(CanPanel,wxPanel)
	//(*EventTable(CanPanel)
	//*)
//	EVT_CHOICE(ID_CAN_INTERFACE, CanPanel::OnCanInterface)
//	EVT_CHOICE(ID_CAMERA_NUM, CanPanel::OnCameraNum)
	EVT_TIMER(ID_TIMER_AUTO,CanPanel::OnTimerAuto)
	EVT_TIMER(ID_TIMER_GET_PARAMETERS,CanPanel::OnTimerGetParameters)
//	EVT_INIT_DIALOG(CanPanel::OnInit)
END_EVENT_TABLE()

CanPanel::CanPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/*
	//(*Initialize(CanPanel)
	wxBoxSizer* BoxSizer4;
	wxSpinCtrl* m_spinSec;
	wxBoxSizer* BoxSizer5;
	wxStaticText* StaticText2;
	wxButton* Button1;
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;

	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(728,490), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetMaxSize(wxSize(-1,-1));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_PropCommand = new wxListCtrl(this, ID_PROP_COMMAND, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER, wxDefaultValidator, _T("ID_PROP_COMMAND"));
	BoxSizer2->Add(m_PropCommand, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_PropStatus = new wxListCtrl(this, ID_LISTCTRL2, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER, wxDefaultValidator, _T("ID_LISTCTRL2"));
	BoxSizer2->Add(m_PropStatus, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 8, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_SEND_COMMAND, _("Отправить команду"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEND_COMMAND"));
	BoxSizer5->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer5, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Получение состояния каждые"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer4->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_spinSec = new wxSpinCtrl(this, ID_SECONDS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SECONDS"));
	m_spinSec->SetValue(_T("0"));
	BoxSizer4->Add(m_spinSec, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("сек."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer4->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer4, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);

	Connect(wxEVT_PAINT,(wxObjectEventFunction)&CanPanel::OnPaint);
	//*)
*/
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer5;
	wxStaticText* StaticText2;
	wxStaticText* StaticText1;
	wxStaticText* StaticText3;
	wxStaticText* StaticText4;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxButton* ButtonSend;

	Create(parent, id, wxDefaultPosition, wxSize(800,600), wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("CAN интерфейс:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_ChoiceCan = new wxChoice(this, ID_CAN_INTERFACE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CAN_INTERFACE"));
	m_ChoiceCan->Append(_("Нет"));
	m_ChoiceCan->Append(_("CAN1"));
	m_ChoiceCan->Append(_("CAN2"));
	wxConfig config("PKVI");
	m_nCanInterface=config.ReadLong("CAN",0);
	m_ChoiceCan->SetSelection(m_nCanInterface);
	m_ChoiceCan->Disable();

	BoxSizer3->Add(m_ChoiceCan, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Номер камеры:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer3->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_Camera_Num = new wxChoice(this, ID_CAMERA_NUM, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CAMERA_NUM"));
	m_Camera_Num->Append(_("Камера 1"));
	m_Camera_Num->Append(_("Камера 2"));
	m_Camera_Num->Append(_("Камера 3"));
	m_Camera_Num->Append(_("Камера 4"));
	m_nCanCameraNum=config.ReadLong("CanCameraNum",0);
	m_Camera_Num->SetSelection(m_nCanCameraNum);
	m_Camera_Num->Disable();

	BoxSizer3->Add(m_Camera_Num, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_PropCommand = new wxPropertyGridManager(this, ID_PROP_COMMAND, wxDefaultPosition, wxDefaultSize,
		wxPG_TOOLBAR|wxPG_BOLD_MODIFIED |wxPG_TOOLTIPS|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE);
	m_PropCommand->GetGrid()->SetVerticalSpacing(2);
	m_PropCommand->AddPage("Команды");
	m_PropCommand->GetToolBar()->SetWindowStyleFlag(wxTB_TEXT|wxTB_NOICONS);
	m_PropCommand->SetDescBoxHeight(60);

	BoxSizer2->Add(m_PropCommand, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);

	m_PropStatus = new wxPropertyGridManager(this, ID_PROP_STATUS, wxDefaultPosition, wxDefaultSize,
		wxPG_TOOLBAR|wxPG_BOLD_MODIFIED |wxPG_TOOLTIPS|wxPG_DESCRIPTION|wxPGMAN_DEFAULT_STYLE);
	m_PropStatus->GetGrid()->SetVerticalSpacing(2);
	m_PropStatus->AddPage("Статус");
	m_PropStatus->GetToolBar()->SetWindowStyleFlag(wxTB_TEXT|wxTB_NOICONS);
	m_PropStatus->SetDescBoxHeight(60);

	BoxSizer2->Add(m_PropStatus, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);

	BoxSizer1->Add(BoxSizer2, 9, wxGROW|wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	ButtonSend = new wxButton(this, ID_SEND_COMMAND, _("Отправить команду"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEND_COMMAND"));
	BoxSizer5->Add(ButtonSend, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Получение состояния каждые"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer6->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_spinSec = new wxSpinCtrl(this, ID_AUTO_STATUS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_AUTO_STATUS"));
	m_spinSec->SetValue(_T("0"));
//	m_spinSec->Enable(false);
	BoxSizer6->Add(m_spinSec, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("сек."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer6->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(BoxSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer5, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);

//	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&CanPanel::OnInit);
//	Connect(wxEVT_SIZE,(wxObjectEventFunction)&CanPanel::OnResize);
	Connect(ID_SEND_COMMAND,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CanPanel::OnSendCommandClick);
	Connect(ID_AUTO_STATUS,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&CanPanel::OnAutoChange);

//	m_Camera_Num->SetValidator(wxGenericValidator(&m_nCanCameraNum));
//	m_ChoiceCan->SetValidator(wxGenericValidator(&m_nCanInterface));

//	memset(&m_InfoData,0,sizeof(INFO_DATA));
	SetSizerAndFit(BoxSizer1);
	m_timerAuto.SetOwner(this,ID_TIMER_AUTO);
	m_timerGetParameters.SetOwner(this,ID_TIMER_GET_PARAMETERS);

//	Init();
}


CanPanel::~CanPanel()
{
	if(m_timerAuto.IsRunning())
		m_timerAuto.Stop();
	if(m_timerGetParameters.IsRunning())
		m_timerGetParameters.Stop();
}

void CanPanel::Init()
{

	m_Camera_Num->SetSelection(m_nCanCameraNum);
	m_ChoiceCan->SetSelection(m_nCanInterface);

	wxPropertyGridPage* pg=m_PropCommand->GetPage(0);
	wxArrayString tchoices;

	wxPGProperty* p=new wxUIntProperty("Номер камеры", wxPG_LABEL, 0);
	pg->Append(p);
	p->SetClientData((void*)0x070000);
	p->SetEditor(wxPGEditor_SpinCtrl);
	p->SetAttribute(wxPG_ATTR_MIN, 1);
	p->SetAttribute(wxPG_ATTR_MAX, 4);
	p->SetAttribute("Wrap", true);
	p->SetHelpString("Присвоить номер камере (поддерживаются все типы камер)");

	p=new wxUIntProperty("Старт", wxPG_LABEL, 0);
	pg->Append(p);
	p->SetClientData((void*)0x00);
	p->SetEditor(wxPGEditor_SpinCtrl);
	p->SetHelpString("Запустить съемку заданного числа кадров (0 - непрерывная съемка) (поддерживаются все типы камер)");

	p=new wxBoolProperty("Стоп", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x010000);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Остановить съемку (поддерживаются все типы камер)");

	p=new wxBoolProperty("Перезапуск", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x020000);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Сброс камеры в исходное положение (поддерживаются все типы камер)");

	p=new wxBoolProperty("Включить камеру", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068122);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Включение камеры (поддерживаются все типы камер)");

	p=new wxBoolProperty("Выключить камеру", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068022);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Выключение камеры (снятие питания и остановка синхрогенератора CCD) (поддерживаются все типы камер)");

	p=new wxBoolProperty("Включить охлаждение", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068116);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Включение системы охлаждения CCD (поддерживаются камеры БИК)");

	p=new wxBoolProperty("Выключить охлаждение", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068016);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Выключение системы охлаждения CCD (поддерживаются камеры БИК)");

	p=new wxBoolProperty("Включить EM-усиление", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068011);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Выбор выхода матрицы с EM-усилением (поддерживаются камеры БИК)");

	p=new wxBoolProperty("Выключить EM-усиление", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068111);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Выбор выхода матрицы без EM-усиления (поддерживаются камеры БИК)");

	p=new wxBoolProperty("Тестовое изображение (макс. ск.)", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068130);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Запуск передачи камерой тестового изображения на максимальной скорости до получения команды \"Стоп\" (поддерживаются все типы камер)");

	p=new wxBoolProperty("Тестовое изображение (1 кадр/с.)", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x068131);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Запуск передачи камерой тестового изображения со скоростью 1 кадр/сек. до получения команды \"Стоп\" (поддерживаются все типы камер)");

//	p=new wxUIntProperty("Количество кадров", wxPG_LABEL, 0);
//	pg->Append(p);
//	p->SetClientData((void*)0x068124);
//	p->SetEditor(wxPGEditor_SpinCtrl);
//	p->SetAttribute(wxPG_ATTR_MIN, 0);
//	p->SetAttribute(wxPG_ATTR_MAX, 65535);
//	p->SetAttribute("Wrap", false);
//	p->SetHelpString("Запустить съемку заданного количества кадров (если 0, то съемка идет непрерывно) (поддерживаются все типы камер)");

	p=new wxBoolProperty("Разрешить соединение по Ethernet", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x06c110);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Разрешить соединение камеры по Ethernet (поддерживаются все типы камер)");

//	p=new wxBoolProperty("Установить соединение с ПКВИ", wxPG_LABEL, false);
//	pg->Append(p);
//	p->SetClientData((void*)0x06c111);
//	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
//	p->SetHelpString("Установить соединение с технологическим ПКВИ по Ethernet (поддерживаются все типы камер)");

	p=new wxBoolProperty("Разорвать текущее соединение", wxPG_LABEL, false);
	pg->Append(p);
	p->SetClientData((void*)0x06c010);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Разорвать текущее соединение по Ethernet (поддерживаются все типы камер)");

	p=new wxPropertyCategory(wxT("Параметры камеры"),wxPG_LABEL);
	pg->Append(p);

	p=new wxDateProperty("Дата", wxPG_LABEL, wxDateTime::Now());
	pg->Append(p);
//	p->SetAttribute(wxPG_DATE_FORMAT, "%d.%m.%y");
	p->SetClientData((void*)(0x050000|MAKEWORD('D','T')));
	p->SetHelpString("Установить дату (поддерживаются все типы камер)");

	p=new wxStringProperty("Время", wxPG_LABEL, wxDateTime::Now().Format("%H:%M:%S"));
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('D','T')));
	p->SetHelpString("Установить время (поддерживаются все типы камер)");

//	p=new wxUIntProperty("Количество кадров съемки", wxPG_LABEL, 1);
//	pg->Append(p);
//	p->SetClientData((void*)(0x050000|MAKEWORD('Q','Q')));
//	p->SetEditor(wxPGEditor_SpinCtrl);
//	p->SetAttribute(wxPG_ATTR_MIN, 0);
//	p->SetAttribute(wxPG_ATTR_MAX, 65535);
//	p->SetAttribute("Wrap", true);
//	p->SetHelpString("Установить количество кадров съемки (поддерживаются все типы камер)");

	p=new wxFloatProperty("Коэффициент усиления", wxPG_LABEL, 0.0);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('K','A')));
	p->SetHelpString("Установить коэффициент усиления (дБ) (поддерживаются все типы камер)");

	p=new wxFloatProperty("Скорость переноса зарядов", wxPG_LABEL, 0.0);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('L','V')));
	p->SetHelpString("Установить скорость переноса зарядов (мм/сек.)(поддерживаются камеры ЛИК)");

	tchoices.Add("4");
	tchoices.Add("16");
	tchoices.Add("32");
	tchoices.Add("64");
	tchoices.Add("96");
	p=new wxEnumProperty("Количество ступеней интеграции",wxPG_LABEL, tchoices);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('L','I')));
	p->SetHelpString("Установить количество ступеней интеграции (поддерживаются камеры ЛИК)");
	p->SetChoiceSelection(0);

	p=new wxUIntProperty("Время экспозиции", wxPG_LABEL, 1);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('E','T')));
	p->SetEditor(wxPGEditor_SpinCtrl);
	p->SetAttribute(wxPG_ATTR_MIN, 0);
	p->SetAttribute(wxPG_ATTR_MAX, 65535);
	p->SetAttribute("Wrap", true);
	p->SetHelpString("Установить время экспозиции (мс) (поддерживаются камеры БИК, ШПК, БПК)");

	p=new wxFloatProperty("ЕМ-усиление", wxPG_LABEL, 1.0);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('E','M')));
	p->SetHelpString("Установить ЕМ-усиление (дБ) (поддерживаются камеры БИК)");

	p=new wxUIntProperty("Код темпа считывания", wxPG_LABEL, 1);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('T','R')));
	p->SetHelpString("Задать код темпа считывания пикселей (поддерживаются камеры БИК, ШПК, БПК)");

	p=new wxPointProperty("Начало фрагмента", wxPG_LABEL, wxPoint(0,0));
	pg->Append(p);
	pg->SetPropertyReadOnly("Начало фрагмента",true,0);
	p->SetClientData((void*)(0x050000|MAKEWORD('X','0')));
	p->SetHelpString("Задать координаты начала фрагмента (поддерживаются камеры БИК, ШПК, БПК)");

	p=new wxPointProperty("Размер фрагмента", wxPG_LABEL, wxPoint(0,0));
	pg->Append(p);
	pg->SetPropertyReadOnly("Размер фрагмента",true,0);
	p->SetClientData((void*)(0x050000|MAKEWORD('D','X')));
	p->SetHelpString("Задать размер фрагмента (поддерживаются камеры БИК, ШПК, БПК)");

	p=new wxBiningProperty("Биннинг", wxPG_LABEL, wxPoint(0,0));
	pg->Append(p);
	pg->SetPropertyReadOnly("Биннинг",true,0);
	p->SetClientData((void*)(0x050000|MAKEWORD('B','X')));
	p->SetHelpString("Установить величину биннинга (поддерживаются все камеры)");

	p=new wxUIntProperty("Уровень черного", wxPG_LABEL, 1);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('B','S')));
	p->SetEditor(wxPGEditor_SpinCtrl);
	p->SetAttribute(wxPG_ATTR_MIN, 0);
	p->SetAttribute(wxPG_ATTR_MAX, 65535);
	p->SetAttribute("Wrap", false);
	p->SetHelpString("Установить смещение уровня черного в АЦП (поддерживаются все типы камер)");

	p=new wxPropertyCategory(wxT("Установить IP-адрес и порты"),wxPG_LABEL);
	pg->Append(p);

	p=new wxStringProperty("IP-адрес камеры", wxPG_LABEL, "192.168.0.1");
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('A','0')));
	p->SetHelpString("Установить новый IP-адрес камеры (поддерживаются все камеры)");

	p=new wxUIntProperty("Порт камеры (в. п.)", wxPG_LABEL, 11111);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('P','0')));
	p->SetHelpString("Установить новый высокоприоритетный порт камеры (поддерживаются все типы камер)");

//	p=new wxStringProperty("IP-адрес СУ-С", wxPG_LABEL, "192.168.0.2");
//	pg->Append(p);
//	p->SetClientData((void*)(0x050000|MAKEWORD('A','1')));
//	p->SetHelpString("Установить новый IP-адрес СУ-С (поддерживаются все камеры)");
//
//	p=new wxUIntProperty("Порт СУ-С", wxPG_LABEL, 11111);
//	pg->Append(p);
//	p->SetClientData((void*)(0x050000|MAKEWORD('A','1')));
//	p->SetHelpString("Установить новый порт СУ-С (поддерживаются все типы камер)");
//
//	p=new wxStringProperty("IP-адрес техн. ПКВИ", wxPG_LABEL, "192.168.0.3");
//	pg->Append(p);
//	p->SetClientData((void*)(0x050000|MAKEWORD('A','2')));
//	p->SetHelpString("Установить новый IP-адрес технологического ПКВИ (поддерживаются все камеры)");

	p=new wxUIntProperty("Порт камеры (н. п.)", wxPG_LABEL, 11111);
	pg->Append(p);
	p->SetClientData((void*)(0x050000|MAKEWORD('P','1')));
	p->SetHelpString("Установить новый низкоприоритетный порт камеры (поддерживаются все типы камер)");


	//состояние
	pg=m_PropStatus->GetPage(0);

	p=new wxStringProperty("Тип камеры",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('C','T')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("Версия ПО",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('#','V')));
	p->SetHelpString("Версия программного обеспечения и дата создания прошивки");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("Дата создания ПО",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('#','D')));
	p->SetHelpString("Дата создания прошивки");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxUIntProperty("Время наработки (мин.)",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('W','W')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("MAC-адрес камеры",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('M','M')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxFloatProperty("Температура кристалла",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('T','1')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxIntProperty("Температура поддона",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('T','2')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxIntProperty("Температура Пельтье",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('T','3')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxIntProperty("Температура радиатора",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('T','4')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxUIntProperty("Число принятых CAN кадров",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('R','X')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxUIntProperty("Число отправленных CAN кадров",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('T','X')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxUIntProperty("Число ошибочных кадров CAN",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('E','R')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("Последнее неправильное сообщение",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('$','M')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("Последняя неправильная расш. команда",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('$','C')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("Последний неправильный параметр",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('$','P')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("Величина неправильного параметра",wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('$','R')));
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxStringProperty("Дата и время", wxPG_LABEL, "");
	p->SetClientData((void*)(0x440000|MAKEWORD('D','T')));
	p->SetHelpString("Установленная дата и время (поддерживаются все типы камер)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

//	p=new wxUIntProperty("Количество кадров съемки", wxPG_LABEL, 0);
//	p->SetClientData((void*)(0x440000|MAKEWORD('Q','Q')));
//	p->SetHelpString("Текущее количество кадров съемки (поддерживаются все типы камер)");
//	p->SetFlag(wxPG_PROP_READONLY);
//	pg->Append(p);
//
	p=new wxFloatProperty("Коэффициент усиления", wxPG_LABEL, 0.0);
	p->SetClientData((void*)(0x440000|MAKEWORD('K','A')));
	p->SetHelpString("Текущий коэффициент усиления (дБ) (поддерживаются все типы камер)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxFloatProperty("Скорость переноса зарядов", wxPG_LABEL, 0);
	p->SetClientData((void*)(0x440000|MAKEWORD('L','V')));
	p->SetHelpString("Текущая скорость переноса зарядов (мм/сек.)(поддерживаются камеры ЛИК)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxUIntProperty("Количество ступеней интеграции",wxPG_LABEL, 0);
	p->SetClientData((void*)(0x440000|MAKEWORD('L','I')));
	p->SetHelpString("Текущее количество ступеней интеграции (поддерживаются камеры ЛИК)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxUIntProperty("Время экспозиции", wxPG_LABEL, 0);
	p->SetClientData((void*)(0x440000|MAKEWORD('E','T')));
	p->SetHelpString("Установленное время экспозиции (мс) (поддерживаются камеры БИК, ШПК, БПК)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxUIntProperty("Точное время экспозиции", wxPG_LABEL, 0);
	p->SetClientData((void*)(0x440000|MAKEWORD('X','T')));
	p->SetHelpString("Текущее точное время экспозиции (мкс) (поддерживаются камеры БИК, ШПК, БПК)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxFloatProperty("ЕМ-усиление", wxPG_LABEL, 0.0);
	p->SetClientData((void*)(0x440000|MAKEWORD('E','M')));
	p->SetHelpString("Текущее ЕМ-усиление (дБ) (поддерживаются камеры БИК)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxFloatProperty("Код темпа считывания", wxPG_LABEL, 0.0);
	p->SetClientData((void*)(0x440000|MAKEWORD('T','R')));
	p->SetHelpString("Заданный код темпа считывания пикселей (поддерживаются камеры БИК, ШПК, БПК)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxPointProperty("Начало фрагмента", wxPG_LABEL, wxPoint(0,0),true);
	p->SetClientData((void*)(0x440000|MAKEWORD('X','0')));
	p->SetHelpString("Заданные координаты начала фрагмента (поддерживаются камеры БИК, ШПК, БПК)");
	pg->Append(p);
	pg->SetPropertyReadOnly("Начало фрагмента");

	p=new wxPointProperty("Размер фрагмента", wxPG_LABEL, wxPoint(0,0),true);
	p->SetClientData((void*)(0x440000|MAKEWORD('D','X')));
	p->SetHelpString("Заданный размер фрагмента (поддерживаются камеры БИК, ШПК, БПК)");
	pg->Append(p);
	pg->SetPropertyReadOnly("Размер фрагмента");

	p=new wxBiningProperty("Биннинг", wxPG_LABEL, wxPoint(0,0),true);
	p->SetClientData((void*)(0x440000|MAKEWORD('B','X')));
	p->SetHelpString("Заданная величина биннинга (поддерживаются все камеры)");
	pg->Append(p);
	pg->SetPropertyReadOnly("Биннинг");


	p=new wxUIntProperty("Уровень черного", wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('B','S')));
	p->SetHelpString("Заданное смещение уровня черного в АЦП (поддерживаются все типы камер)");
	p->SetFlag(wxPG_PROP_READONLY);
	pg->Append(p);

	p=new wxPropertyCategory(wxT("Установленные IP-адрес и порты камеры"),wxPG_LABEL);
	pg->Append(p);

	p=new wxStringProperty("IP-адрес камеры", wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('A','0')));
	p->SetHelpString("Установленный IP-адрес камеры (поддерживаются все камеры)");
	pg->Append(p);

	p=new wxUIntProperty("Порт камеры (в. п.)", wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('P','0')));
	pg->Append(p);
	p->SetHelpString("Установленный высокоприоритетный порт камеры (поддерживаются все типы камер)");

//	p=new wxStringProperty("IP-адрес СУ-С", wxPG_LABEL);
//	p->SetClientData((void*)(0x440000|MAKEWORD('A','1')));
//	pg->Append(p);
//	p->SetHelpString("Установленный IP-адрес СУ-С (поддерживаются все камеры)");
//
	p=new wxUIntProperty("Порт камеры (н. п.)", wxPG_LABEL);
	p->SetClientData((void*)(0x440000|MAKEWORD('P','1')));
	pg->Append(p);
	p->SetHelpString("Установленный низкоприоритетный порт камеры (поддерживаются все типы камер)");

//	p=new wxStringProperty("IP-адрес техн. ПКВИ", wxPG_LABEL);
//	p->SetClientData((void*)(0x440000|MAKEWORD('A','2')));
//	pg->Append(p);
//	p->SetHelpString("Установленный IP-адрес технологического ПКВИ (поддерживаются все камеры)");
//
//	p=new wxUIntProperty("Порт техн. ПКВИ", wxPG_LABEL);
//	p->SetClientData((void*)(0x440000|MAKEWORD('P','2')));
//	pg->Append(p);
//	p->SetHelpString("Установленный порт технологического ПКВИ (поддерживаются все типы камер)");

	pg->SetPropertyReadOnly("Установленные IP-адрес и порты камеры");

	pg->Append(new wxPropertyCategory("Статус камеры",wxPG_LABEL));

	p=new wxStringProperty("Термодатчик радиатора", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','7')));
	pg->Append(p);
	p->SetHelpString("Исправность термодатчика радиатора");

	p=new wxStringProperty("Термодатчик пельтье", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','6')));
	pg->Append(p);
	p->SetHelpString("Исправность термодатчика на пельтье матрицы");

	p=new wxStringProperty("Термодатчик радиатора матрицы", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','5')));
	pg->Append(p);
	p->SetHelpString("Исправность термодатчика на радиаторе матрицы");

	p=new wxStringProperty("Термодатчик кристалла матрицы", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','4')));
	pg->Append(p);
	p->SetHelpString("Исправность термодатчика на кристалле матрицы (для ЛИК всегда исправна)");

	p=new wxStringProperty("Заданная температура", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','3')));
	pg->Append(p);
	p->SetHelpString("Состояние термостата матрицы (достигнута ли заданная температура)");

	p=new wxStringProperty("Перегрев камеры", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','2')));
	pg->Append(p);

	p=new wxStringProperty("Исправность термостата", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','1')));
	pg->Append(p);

	p=new wxStringProperty("Охлаждение", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('0','0')));
	pg->Append(p);
	p->SetHelpString("Состояние охлаждения (только для БИК, в остальных ЦТК всегда включено)");

	p=new wxStringProperty("Показания КДПО", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('1','7')));
	pg->Append(p);

	p=new wxStringProperty("Синхроимпульсы", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('1','6')));
	pg->Append(p);

	p=new wxStringProperty("Связь по ethernet", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('1','5')));
	pg->Append(p);

	p=new wxStringProperty("Питание камеры", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('1','4')));
	pg->Append(p);

	p=new wxUIntProperty("Код ошибки питания", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('1','3')));
	pg->Append(p);

	p=new wxStringProperty("Тестовое изображение", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('2','3')));
	pg->Append(p);

	p=new wxStringProperty("Выход матрицы", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('2','2')));
	p->SetHelpString("Используемый выход матрицы (только для БИК, в остальных ЦТК всегда без ЕМ-усиления)");
	pg->Append(p);

	p=new wxStringProperty("Камера", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('2','1')));
	p->SetHelpString("Состояние камеры");
	pg->Append(p);

	p=new wxStringProperty("Съемка", wxPG_LABEL);
	p->SetClientData((void*)(0x430000|MAKEWORD('2','0')));
	pg->Append(p);

	pg->SetPropertyReadOnly("Статус камеры");

	m_bFirst=true;

//	pg->Append(new wxPropertyCategory("Показания датчиков",wxPG_LABEL));
//	pg->Append(new wxStringProperty(" оси A",wxPG_LABEL));
//	pg->Append(new wxStringProperty(" оси L",wxPG_LABEL));
//	pg->Append(new wxStringProperty(" оси R",wxPG_LABEL));
//	pg->SetPropertyReadOnly("Показания датчиков");

}


void CanPanel::OnSendCommandClick(wxCommandEvent& event)
{
	wxPropertyGridPage* pg=m_PropCommand->GetPage(0);

	wxPropertyGridIterator it;
	MY_CAN_MSG msg;
	memset(&msg,0,sizeof(msg));
	msg.id=((4+m_nCanCameraNum)<<7);

    for ( it = pg->GetIterator(); !it.AtEnd(); it++ )
    {
		if(!m_pCan->IsOpen())
			break;

		bool bSendCommand=false;
        wxPGProperty* p = *it;
        if(!p->HasFlag(wxPG_PROP_MODIFIED))
			continue;
        u_int32_t nData=(u_int32_t)p->GetClientData();
		wxAny val=p->GetValue();
		msg.id|=HIWORD(nData);
        switch(nData)
        {
        	case 0x0:			//Старт
        	{
        		msg.length=4;
				DWORD dwVal=(DWORD)(val.As<long>());
				memcpy(msg.data,&dwVal,4);
				bSendCommand=true;
        	}
        	break;
        	case 0x010000:		//Стоп
        	case 0x020000:		//Перезапуск
				{
					if(val.As<bool>())
					{
						p->SetValue(false);
						bSendCommand=true;
					}
				}
				break;
        	case 0x068122:		//Включить камеру
        	case 0x068022:		//Выключить камеру
        	case 0x068116:		//Включить охлаждение
        	case 0x068016:		//Выключить охлаждение
        	case 0x068011:		//Включить EM-усиление
        	case 0x068111:		//Выключить EM-усиление
        	case 0x068130:		//Тестовое изображение (макс. скорость)
        	case 0x068131:		//Тестовое изображение (1 кадр/с)
        	case 0x06c110:		//Соединение по Ethernet
        	case 0x06c010:		//Разорвать соединение
				{
					if(val.As<bool>())
					{
//						msg.flags=MSG_EXT;
						msg.length=2;
						msg.data[0]=LOBYTE(LOWORD(nData));
						msg.data[1]=HIBYTE(LOWORD(nData));
						p->SetValue(false);
						bSendCommand=true;
					}
				}
				break;
        	case 0x050000|MAKEWORD('D','T'):	//Дата и время
				{
					msg.length=8;
					msg.data[0]=LOBYTE(LOWORD(nData));
					msg.data[1]=HIBYTE(LOWORD(nData));
					wxPGProperty* pTemp=pg->GetPropertyByName("Дата");
					if(pTemp)
					{
						val=pTemp->GetValue();
						wxDateTime dt=val.As<wxDateTime>();
						int nYear=dt.GetYear();
						msg.data[2]=BYTE(nYear>=100?nYear-2000:nYear);
						msg.data[3]=BYTE(dt.GetMonth()+1);
						msg.data[4]=BYTE(dt.GetDay());
						pTemp->SetModifiedStatus(false);
					}
					else
						continue;
					pTemp=pg->GetPropertyByName("Время");
					if(pTemp)
					{
						val=pTemp->GetValue();
						wxString s=val.As<wxString>();
						wxDateTime dt;
						wxString::const_iterator end;
						dt.ParseTime(s,&end);
						if(dt.IsValid())
						{
							msg.data[5]=BYTE(dt.GetHour());
							msg.data[6]=BYTE(dt.GetMinute());
							msg.data[7]=BYTE(dt.GetSecond());
						}
						else
							continue;
						pTemp->SetModifiedStatus(false);
					}
					else
						continue;
					bSendCommand=true;
				}
				break;
			case 0x050000|MAKEWORD('K','A'):	//Коэффициент усиления (8.8)
			case 0x050000|MAKEWORD('E','M'):	//ЕМ-усиление (8.8)
			case 0x050000|MAKEWORD('L','V'):	//Скорость переноса (8.8)
				{
					msg.length=6;
					msg.data[0]=LOBYTE(LOWORD(nData));
					msg.data[1]=HIBYTE(LOWORD(nData));
					float fVal=val.As<float>();
					DWORD dwVal=(DWORD)(fVal*256);
					memcpy(msg.data+2,&dwVal,4);
					bSendCommand=true;
				}
				break;
			case 0x050000|MAKEWORD('P','0'):	//Port (h. p.)
			case 0x050000|MAKEWORD('P','1'):	//Port (l. p.)
			case 0x050000|MAKEWORD('E','T'):	//Время экспозиции
			case 0x050000|MAKEWORD('T','R'):	//Код темпа считывания
			case 0x050000|MAKEWORD('B','S'):	//Уровень черного
				{
					msg.length=6;
					msg.data[0]=LOBYTE(LOWORD(nData));
					msg.data[1]=HIBYTE(LOWORD(nData));
					DWORD dwVal=(DWORD)(val.As<long>());
					memcpy(msg.data+2,&dwVal,4);
					bSendCommand=true;

				}
				break;
			case 0x050000|MAKEWORD('X','0'):	//Начало фрагмента
				{
					msg.length=6;
					msg.data[0]=LOBYTE(LOWORD(nData));
					msg.data[1]=HIBYTE(LOWORD(nData));
					wxPoint ptVal=val.As<wxPoint>();
					DWORD dwVal=(DWORD)ptVal.x;
					memcpy(msg.data+2,&dwVal,4);
					m_pCan->SendMsg(&msg);
					msg.data[0]='Y';
					dwVal=(DWORD)ptVal.y;
					memcpy(msg.data+2,&dwVal,4);
					m_pCan->SendMsg(&msg);
					bSendCommand=true;
				}
				break;
			case 0x050000|MAKEWORD('D','X'):	//Размер фрагмента
			case 0x050000|MAKEWORD('B','X'):	//Бининг
				{
					msg.length=6;
					msg.data[0]=LOBYTE(LOWORD(nData));
					msg.data[1]=HIBYTE(LOWORD(nData));
					wxPoint ptVal=val.As<wxPoint>();
					DWORD dwVal=(DWORD)ptVal.x;
					memcpy(msg.data+2,&dwVal,4);
					m_pCan->SendMsg(&msg);
					msg.data[1]='Y';
					dwVal=(DWORD)ptVal.y;
					memcpy(msg.data+2,&dwVal,4);
					m_pCan->SendMsg(&msg);
					bSendCommand=true;
				}
				break;
			case 0x050000|MAKEWORD('L','I'):	//Количество ступеней интеграции
				{
					msg.length=6;
					msg.data[0]=LOBYTE(LOWORD(nData));
					msg.data[1]=HIBYTE(LOWORD(nData));
					DWORD dwVal=(DWORD)val.As<long>();
					switch(dwVal)
					{
					case 0:
						dwVal=4;
						break;
					case 1:
						dwVal=16;
						break;
					case 2:
						dwVal=32;
						break;
					case 3:
						dwVal=64;
						break;
					case 4:
						dwVal=96;
						break;
					}
					memcpy(msg.data+2,&dwVal,4);
					bSendCommand=true;
				}
				break;
        	case 0x050000|MAKEWORD('A','0'):	//IP-адрес камеры
				{
					msg.length=6;
					msg.data[0]=LOBYTE(LOWORD(nData));
					msg.data[1]=HIBYTE(LOWORD(nData));
					BYTE pIP[4];
					wxString s=val.As<wxString>();
					if(StringToIp(s,pIP))
					{
						msg.data[2]=pIP[3];
						msg.data[3]=pIP[2];
						msg.data[4]=pIP[1];
						msg.data[5]=pIP[0];
					}
					bSendCommand=true;
				}
				break;
        }
        if(bSendCommand)
			m_pCan->SendMsg(&msg);
    }
	pg->ClearModifiedStatus();
	m_PropCommand->RefreshGrid();
}

void CanPanel::OnAutoChange(wxSpinEvent& event)
{
	int nMilliSeconds=m_spinSec->GetValue()*1000;
	if(nMilliSeconds<=0)
		m_timerAuto.Stop();
	else
		m_timerAuto.Start(nMilliSeconds,false);
}

void CanPanel::OnTimerAuto(wxTimerEvent& event)
{
	if(!m_pCan->IsOpen())
		return;
	m_timerGetParameters.Start(20,false);
}

void CanPanel::OnTimerGetParameters(wxTimerEvent& event)
{
	static int nNumParam=0;
	if(!m_pCan->IsOpen())
	{
		m_timerGetParameters.Stop();
		nNumParam=0;
		return;
	}
	if(SendQuery(nNumParam))
	{
		RecieveQuery();
	}
	nNumParam++;
	if(nNumParam>=36)
	{
		nNumParam=0;
		m_timerGetParameters.Stop();
	}

}

bool CanPanel::SendQuery(int nNumParam)
{
	MY_CAN_MSG msg;
	memset(&msg,0,sizeof(msg));
	msg.id=((4+m_nCanCameraNum)<<7);
	if(nNumParam==0)
	{
		msg.id|=0x3;
		msg.length=0;
		msg.flags=MSG_RTR;
	}
	else
	{
		msg.id|=0x4;
		msg.length=2;
		switch(nNumParam)
		{
		case 1:
			msg.data[0]='D';
			msg.data[1]='T';
			break;
//		case 2:
//			msg.data[0]='Q';
//			msg.data[1]='Q';
//			break;
		case 2:
			msg.data[0]='K';
			msg.data[1]='A';
			break;
		case 3:
			msg.data[0]='L';
			msg.data[1]='V';
			break;
		case 4:
			msg.data[0]='L';
			msg.data[1]='I';
			break;
		case 5:
			msg.data[0]='E';
			msg.data[1]='T';
			break;
		case 6:
			msg.data[0]='X';
			msg.data[1]='T';
			break;
		case 7:
			msg.data[0]='E';
			msg.data[1]='M';
			break;
		case 8:
			msg.data[0]='T';
			msg.data[1]='R';
			break;
		case 9:
			msg.data[0]='X';
			msg.data[1]='0';
			break;
		case 10:
			msg.data[0]='Y';
			msg.data[1]='0';
			break;
		case 11:
			msg.data[0]='D';
			msg.data[1]='X';
			break;
		case 12:
			msg.data[0]='D';
			msg.data[1]='Y';
			break;
		case 13:
			msg.data[0]='B';
			msg.data[1]='X';
			break;
		case 14:
			msg.data[0]='B';
			msg.data[1]='Y';
			break;
		case 15:
			msg.data[0]='C';
			msg.data[1]='R';
			break;
		case 16:
			msg.data[0]='B';
			msg.data[1]='S';
			break;
		case 17:
			msg.data[0]='A';
			msg.data[1]='0';
			break;
		case 18:
			msg.data[0]='P';
			msg.data[1]='0';
			break;
		case 19:
			msg.data[0]='P';
			msg.data[1]='1';
			break;
		case 20:
			msg.data[0]='W';
			msg.data[1]='W';
			break;
		case 21:
			msg.data[0]='M';
			msg.data[1]='M';
			break;
		case 22:
			msg.data[0]='T';
			msg.data[1]='1';
			break;
		case 23:
			msg.data[0]='T';
			msg.data[1]='2';
			break;
		case 24:
			msg.data[0]='T';
			msg.data[1]='3';
			break;
		case 25:
			msg.data[0]='T';
			msg.data[1]='4';
			break;
		case 26:
			msg.data[0]='C';
			msg.data[1]='T';
			break;
		case 27:
			msg.data[0]='#';
			msg.data[1]='V';
			break;
		case 28:
			msg.data[0]='#';
			msg.data[1]='D';
			break;
		case 29:
			msg.data[0]='R';
			msg.data[1]='X';
			break;
		case 30:
			msg.data[0]='T';
			msg.data[1]='X';
			break;
		case 31:
			msg.data[0]='E';
			msg.data[1]='R';
			break;
		case 32:
			msg.data[0]='$';
			msg.data[1]='M';
			break;
		case 33:
			msg.data[0]='$';
			msg.data[1]='C';
			break;
		case 34:
			msg.data[0]='$';
			msg.data[1]='P';
			break;
		case 35:
			msg.data[0]='$';
			msg.data[1]='R';
			break;
		}
//		do
//		{
//		}while(send==0);
	}
	m_pCan->SendMsg(&msg);
	return true;
}

void CanPanel::SetCameraType(int nType)
{
	wxPropertyGridPage* pg=m_PropCommand->GetPage(0);
//	pg->HideProperty("Включить охлаждение",nType!=0);
//	pg->HideProperty("Выключить охлаждение",nType!=0);
	pg->HideProperty("Включить EM-усиление",nType!=0);
	pg->HideProperty("Выключить EM-усиление",nType!=0);
	pg->HideProperty("Скорость переноса зарядов",nType!=2);
	pg->HideProperty("Количество ступеней интеграции",nType!=2);
	pg->HideProperty("Время экспозиции",nType==2);
	pg->HideProperty("ЕМ-усиление",nType!=0);
	pg->HideProperty("Код темпа считывания",nType==2);
	pg->HideProperty("Начало фрагмента",nType==2);
	pg->HideProperty("Размер фрагмента",nType==2);
	pg->HideProperty("Биннинг",nType==2);
	pg->HideProperty("Уровень черного",nType!=0);

	pg=m_PropStatus->GetPage(0);
	pg->HideProperty("Скорость переноса зарядов",nType!=2);
	pg->HideProperty("Количество ступеней интеграции",nType!=2);
	pg->HideProperty("Время экспозиции",nType==2);
	pg->HideProperty("Точное время экспозиции",nType==2);
	pg->HideProperty("ЕМ-усиление",nType!=0);
	pg->HideProperty("Код темпа считывания",nType==2);
	pg->HideProperty("Начало фрагмента",nType==2);
	pg->HideProperty("Размер фрагмента",nType==2);
	pg->HideProperty("Биннинг",nType==2);
	pg->HideProperty("Уровень черного",nType!=0);
}

bool CanPanel::RecieveQuery()
{
	wxStopWatch sw;
	MY_CAN_MSG msg;
	memset(&msg,0,sizeof(msg));
	int nReaded=0, nNeed=sizeof(msg);
	bool bGot=false;
//	do
//	{
//		nGot=read(m_hCan,&msg+nReaded,nNeed);
//		nReaded+=nGot;
//		nNeed-=nGot;
//	}while(sw.Time()<=1000 && nReaded<sizeof(msg));
//	if(nReaded<sizeof(msg))
//		return false;
	do
	{
		bGot=m_pCan->RcvMsg(&msg);
	}while(sw.Time()<=20 && !bGot);
	if(!bGot)
		return false;

	int nAddress=(msg.id>>7)&0xf;
	int nCod=msg.id&0x3f;
	int nR=(msg.id>>6)&0x01;
	if(nAddress!=m_nCanCameraNum+4 || nR==0)
		return false;
	wxPropertyGridPage* pg=m_PropStatus->GetPage(0);
	wxPGProperty* p;
	wxString s;
	if(nCod==0x3 && msg.length==4) //Статус
	{
		CAN_INFO can_info;
		memcpy(&can_info,&msg.data,sizeof(CAN_INFO));
		s=can_info.sTermostat.btTermRadiator?"исправен":"неисправен";
		pg->SetPropertyValue("Термодатчик радиатора",s);
		s=can_info.sTermostat.btTermMatrixPeltie?"исправен":"неисправен";
		pg->SetPropertyValue("Термодатчик пельтье",s);
		s=can_info.sTermostat.btTermMatrixRadiator?"исправен":"неисправен";
		pg->SetPropertyValue("Термодатчик радиатора матрицы",s);
		s=can_info.sTermostat.btTermMatrixCrist?"исправен":"неисправен";
		pg->SetPropertyValue("Термодатчик кристалла матрицы",s);
		s=can_info.sTermostat.btTermostatStatus?"достигнута":"не достигнута";
		pg->SetPropertyValue("Заданная температура",s);
		s=can_info.sTermostat.btUpHot?"перегрев":"норма";
		pg->SetPropertyValue("Перегрев камеры",s);
		s=can_info.sTermostat.btTermostatReady?"исправен":"неисправен";
		pg->SetPropertyValue("Исправность термостата",s);
		s=can_info.sTermostat.btCool?"включено":"выключено";
		pg->SetPropertyValue("Охлаждение",s);

		s=can_info.sErrors.btKPDO?"отсутствуют":"принимаются";
		pg->SetPropertyValue("Показания КДПО",s);
		s=can_info.sErrors.btErrorSyncro?"отсутствуют":"в норме";
		pg->SetPropertyValue("Синхроимпульсы",s);
		s=can_info.sErrors.btEthernetLink?"установлена":"нет связи";
		pg->SetPropertyValue("Связь по ethernet",s);
		s=can_info.sErrors.btPower?"в норме":"нет нормы";
		pg->SetPropertyValue("Питание камеры",s);
		pg->SetPropertyValue("Код ошибки питания",can_info.sErrors.btErrorCode);

		s=can_info.sStatus.btTestImage==0?"выключено":can_info.sStatus.btTestImage==1?"включено (макс. скорость)":"включено (1 кадр/с)";
		pg->SetPropertyValue("Тестовое изображение",s);
		s=can_info.sStatus.btMatrixOutput?"без ЕМ-усиления":"с ЕМ-усилением";
		pg->SetPropertyValue("Выход матрицы",s);
		s=can_info.sStatus.btCamera?"включена":"выключена";
		pg->SetPropertyValue("Камера",s);
		s=can_info.sStatus.btSurvay?"включена":"выключена";
		pg->SetPropertyValue("Съемка",s);
	}
	else if(nCod==0x04 && msg.length>=2)	//параметр
	{
		u_int32_t nData=MAKEWORD(msg.data[0],msg.data[1]);
		switch(nData)
		{
		case MAKEWORD('C','T'):
			{
				if(msg.data[2]==0)
					s="БИК";
				else if(msg.data[2]==1)
					s="ШПК";
				else if(msg.data[2]==2)
					s="ЛИК";
				else if(msg.data[2]==3)
					s="БПК";
				pg->SetPropertyValue("Тип камеры",s);
				if(m_bFirst)
				{
					SetCameraType(msg.data[2]);
					m_bFirst=false;
				}

			}
			break;
		case MAKEWORD('#','V'):
			{
				s.Printf("%02d.%02d",(msg.data[2]>>4)*10+(msg.data[2]&0xf),(msg.data[3]>>4)*10+(msg.data[3]&0xf));
				pg->SetPropertyValue("Версия ПО",s);
			}
			break;
		case MAKEWORD('#','D'):
			{
				s.Printf("%02d.%02d.%02d",(msg.data[4]>>4)*10+(msg.data[4]&0xf),(msg.data[3]>>4)*10+(msg.data[3]&0xf),
							(msg.data[2]>>4)*10+(msg.data[2]&0xf));
				pg->SetPropertyValue("Дата создания ПО",s);
			}
			break;
		case MAKEWORD('W','W'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Время наработки (мин.)",(long)nVal);
			}
			break;
		case MAKEWORD('M','M'):
			{
				s.Printf("%02x:%02x:%02x:%02x:%02x:%02x",msg.data[2],msg.data[3],msg.data[4],msg.data[5],msg.data[6],msg.data[7]);
				pg->SetPropertyValue("MAC-адрес камеры",s);
			}
			break;
		case MAKEWORD('T','1'):
			{
				int32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				float fVal=nVal/256.0;
				pg->SetPropertyValue("Температура кристалла",fVal);
			}
			break;
		case MAKEWORD('T','2'):
			{
				int32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Температура поддона",(long)nVal);
			}
			break;
		case MAKEWORD('T','3'):
			{
				int32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Температура Пельтье",(long)nVal);
			}
			break;
		case MAKEWORD('T','4'):
			{
				int32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Температура радиатора",(long)nVal);
			}
			break;
		case MAKEWORD('R','X'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Число принятых CAN кадров",(long)nVal);
			}
			break;
		case MAKEWORD('T','X'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Число отправленных CAN кадров",(long)nVal);
			}
			break;
		case MAKEWORD('E','R'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Число ошибочных кадров CAN",(long)nVal);
			}
			break;
		case MAKEWORD('$','M'):
			{
				s.Printf("код: 0x%x; кол. байт: %d",msg.data[2]&0x1f,msg.data[3]);
				pg->SetPropertyValue("Последнее неправильное сообщение",s);
			}
			break;
		case MAKEWORD('$','C'):
			{
				s.Printf("код: 0x%x; кол. байт: %d",MAKEWORD(msg.data[2],msg.data[3]),msg.data[4]);
				pg->SetPropertyValue("Последняя неправильная расш. команда",s);
			}
			break;
		case MAKEWORD('$','P'):
			{
				s.Printf("ID: 0x%x; кол. байт: %d",MAKEWORD(msg.data[2],msg.data[3]),msg.data[4]);
				pg->SetPropertyValue("Последний неправильный параметр",s);
			}
			break;
		case MAKEWORD('$','R'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				s.Printf("ID: 0x%x; знач.: 0x%x",MAKEWORD(msg.data[2],msg.data[3]),nVal);
				pg->SetPropertyValue("Величина неправильного параметра",s);
			}
			break;
		case MAKEWORD('D','T'):
			{
				s.Printf("%02d.%02d.%02d %02d:%02d:%02d",msg.data[4],msg.data[3],msg.data[2],msg.data[5],msg.data[6],msg.data[7]);
				pg->SetPropertyValue("Дата и время",s);
			}
			break;
//		case MAKEWORD('Q','Q'):
//			{
//				int nVal;
//				memcpy(&nVal,msg.data+2,4);
//				pg->SetPropertyValue("Количество кадров съемки",nVal);
//			}
//			break;
		case MAKEWORD('K','A'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				float fVal=nVal/256.0;
				pg->SetPropertyValue("Коэффициент усиления",fVal);
			}
			break;
		case MAKEWORD('L','V'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				float fVal=nVal/256.0;
				pg->SetPropertyValue("Скорость переноса зарядов",fVal);
			}
			break;
		case MAKEWORD('L','I'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Количество ступеней интеграции",(long)nVal);
			}
			break;
		case MAKEWORD('E','T'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Время экспозиции",(long)nVal);
			}
			break;
		case MAKEWORD('X','T'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Точное время экспозиции",(long)nVal);
			}
			break;
		case MAKEWORD('E','M'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				float fVal=nVal/256.0;
				pg->SetPropertyValue("ЕМ-усиление",fVal);
			}
			break;
		case MAKEWORD('T','R'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Код темпа считывания",(long)nVal);
			}
			break;
		case MAKEWORD('X','0'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				wxAny val=pg->GetPropertyValue("Начало фрагмента");
				wxPoint point=val.As<wxPoint>();
				point.x=nVal;
				val=point;
				pg->SetPropertyValue("Начало фрагмента",val);
			}
			break;
		case MAKEWORD('Y','0'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				wxAny val=pg->GetPropertyValue("Начало фрагмента");
				wxPoint point=val.As<wxPoint>();
				point.y=nVal;
				val=point;
				pg->SetPropertyValue("Начало фрагмента",val);
			}
			break;
		case MAKEWORD('D','X'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				wxAny val=pg->GetPropertyValue("Размер фрагмента");
				wxPoint point=val.As<wxPoint>();
				point.x=nVal;
				val=point;
				pg->SetPropertyValue("Размер фрагмента",val);
			}
			break;
		case MAKEWORD('D','Y'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				wxAny val=pg->GetPropertyValue("Размер фрагмента");
				wxPoint point=val.As<wxPoint>();
				point.y=nVal;
				val=point;
				pg->SetPropertyValue("Размер фрагмента",val);
			}
			break;
		case MAKEWORD('B','X'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				wxAny val=pg->GetPropertyValue("Биннинг");
				wxPoint point=val.As<wxPoint>();
				point.x=nVal;
				val=point;
				pg->SetPropertyValue("Биннинг",val);
			}
			break;
		case MAKEWORD('B','Y'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				wxAny val=pg->GetPropertyValue("Биннинг");
				wxPoint point=val.As<wxPoint>();
				point.y=nVal;
				val=point;
				pg->SetPropertyValue("Биннинг",val);
			}
			break;
		case MAKEWORD('B','S'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Уровень черного",(long)nVal);
			}
			break;
		case MAKEWORD('A','0'):
			{
				s.Printf("%d.%d.%d.%d",msg.data[5],msg.data[4],msg.data[3],msg.data[2]);
				pg->SetPropertyValue("IP-адрес камеры",s);
			}
			break;
		case MAKEWORD('P','0'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Порт камеры (в. п.)",(long)nVal);
			}
			break;
		case MAKEWORD('P','1'):
			{
				uint32_t nVal;
				memcpy(&nVal,msg.data+2,4);
				pg->SetPropertyValue("Порт камеры (н. п.)",(long)nVal);
			}
			break;
		}
	}
}

wxPGProperty* CanPanel::FindPropertyByData(wxPropertyGridPage* pg, u_int32_t nClientData)
{
	wxPropertyGridIterator it;
    for ( it = pg->GetIterator(); !it.AtEnd(); it++ )
	{
		wxPGProperty* p = *it;
		if((u_int32_t)p->GetClientData()==nClientData)
			return p;
	}
	return NULL;
}

//void CanPanel::OnCanInterface(wxCommandEvent& event)
//{
//	if(m_nCanInterface!=m_ChoiceCan->GetSelection())
//	{
//		m_nCanInterface=m_ChoiceCan->GetSelection();
//		if(m_hCan>=0)
//		{
//			close(m_hCan);
//			m_hCan=-1;
//		}
//		if(m_nCanInterface>0)
//		{
//			char device[40];
//			sprintf(device, "/dev/can%d", m_nCanInterface-1);
//			m_hCan=open(device,O_RDWR|O_NONBLOCK);
//			if(m_hCan<0)
//			{
//				m_nCanInterface=0;
//				m_ChoiceCan->SetSelection(m_nCanInterface);
//			}
//			else
//				set_bitrate(m_hCan,500);
//		}
//	}
//}

//void CanPanel::OnCameraNum(wxCommandEvent& event)
//{
//	m_nCanCameraNum=m_Camera_Num->GetSelection();
//}

