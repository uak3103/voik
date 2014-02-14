#include "wx_pch.h"
#include "PKVIMain.h"
#include "NetworkPanel.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(NetworkPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(NetworkPanel)
//*)
u_int32_t g_nNetCommandNum=0;

/*
//(*IdInit(NetworkPanel)
const long NetworkPanel::ID_PROP_COMMAND = wxNewId();
const long NetworkPanel::ID_LISTCTRL2 = wxNewId();
const long NetworkPanel::ID_SEND_COMMAND = wxNewId();
const long NetworkPanel::ID_STATICTEXT1 = wxNewId();
const long NetworkPanel::ID_AUTO_STATUS = wxNewId();
const long NetworkPanel::ID_STATICTEXT2 = wxNewId();
const long NetworkPanel::ID_STATICTEXT3 = wxNewId();
const long NetworkPanel::ID_CAN_INTERFACE = wxNewId();
const long NetworkPanel::ID_STATICTEXT4 = wxNewId();
const long NetworkPanel::ID_CAMERA_NUM = wxNewId();
//*)
*/
const long NetworkPanel::ID_PROP_COMMAND = wxNewId();
const long NetworkPanel::ID_PROP_STATUS = wxNewId();
const long NetworkPanel::ID_SEND_COMMAND = wxNewId();
const long NetworkPanel::ID_STATICTEXT1 = wxNewId();
const long NetworkPanel::ID_AUTO_STATUS = wxNewId();
const long NetworkPanel::ID_STATICTEXT2 = wxNewId();

BEGIN_EVENT_TABLE(NetworkPanel,wxPanel)
	//(*EventTable(NetworkPanel)
	//*)
	EVT_TIMER(999,NetworkPanel::OnTimerAuto)

END_EVENT_TABLE()

NetworkPanel::NetworkPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/*
	//(*Initialize(NetworkPanel)
	wxSpinCtrl* m_spinSec;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxStaticText* StaticText2;
	wxButton* Button1;
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxStaticText* StaticText3;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	wxStaticText* StaticText4;

	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(768,539), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetMaxSize(wxSize(-1,-1));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_PropCommand = new wxListCtrl(this, ID_PROP_COMMAND, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER, wxDefaultValidator, _T("ID_PROP_COMMAND"));
	BoxSizer2->Add(m_PropCommand, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_PropStatus = new wxListCtrl(this, ID_LISTCTRL2, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER, wxDefaultValidator, _T("ID_LISTCTRL2"));
	BoxSizer2->Add(m_PropStatus, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 9, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_SEND_COMMAND, _("Отправить команду"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SEND_COMMAND"));
	BoxSizer5->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Получение состояния каждые"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer6->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_spinSec = new wxSpinCtrl(this, ID_AUTO_STATUS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_AUTO_STATUS"));
	m_spinSec->SetValue(_T("0"));
	BoxSizer6->Add(m_spinSec, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("сек."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer6->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(BoxSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer5, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("CAN интерфейс:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_Can_Interface = new wxChoice(this, ID_CAN_INTERFACE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CAN_INTERFACE"));
	m_Can_Interface->Append(_("CAN1"));
	m_Can_Interface->Append(_("CAN2"));
	BoxSizer3->Add(m_Can_Interface, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Номер камеры:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer3->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_Camera_Num = new wxChoice(this, ID_CAMERA_NUM, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CAMERA_NUM"));
	m_Camera_Num->SetSelection( m_Camera_Num->Append(_("Камера 1")) );
	m_Camera_Num->Append(_("Камера 2"));
	m_Camera_Num->Append(_("Камера 3"));
	m_Camera_Num->Append(_("Камера 4"));
	BoxSizer3->Add(m_Camera_Num, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
	//*)
*/
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxStaticText* StaticText2;
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
//	wxBoxSizer* BoxSizer3;
	wxButton* ButtonSend;

	Create(parent, id, wxDefaultPosition, wxSize(800,600), wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
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

//	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&NetworkPanel::OnInit);
//	Connect(wxEVT_SIZE,(wxObjectEventFunction)&NetworkPanel::OnResize);
	Connect(ID_SEND_COMMAND,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NetworkPanel::OnSendCommandClick);
	Connect(ID_AUTO_STATUS,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&NetworkPanel::OnAutoChange);

	memset(&m_InfoData,0,sizeof(INFO_DATA));
	SetSizerAndFit(BoxSizer1);
	m_timerAuto.SetOwner(this,999);
	Init();
}

NetworkPanel::~NetworkPanel()
{
	if(m_timerAuto.IsRunning())
		m_timerAuto.Stop();
}

void NetworkPanel::Init()
{
	wxPropertyGridPage* pg=m_PropCommand->GetPage(0);
	wxArrayString tchoices;

	wxPGProperty* p=new wxFloatProperty("Скорость переноса (мм/с)", wxPG_LABEL, 5.0);
	pg->Append(p);
	p->SetClientData((void*)0x1010);
//	p->SetEditor(wxPGEditor_SpinCtrl);
//	p->SetAttribute(wxPG_ATTR_MIN, 1);
//	p->SetAttribute(wxPG_ATTR_MAX, 100);
//	p->SetAttribute("Wrap", false);
	p->SetHelpString("Установить скорость вертикального переноса в мм/сек (поддерживаются камеры ЛИК)");

	tchoices.Add("4");
	tchoices.Add("16");
	tchoices.Add("32");
	tchoices.Add("64");
	tchoices.Add("96");
	p=new wxEnumProperty("Количество ступеней интеграции",wxPG_LABEL, tchoices);
	pg->Append(p);
	p->SetClientData((void*)0x1011);
	p->SetHelpString("Установить количество ступеней интеграции (поддерживаются камеры ЛИК)");
	p->SetValue("");


	p=new wxUIntProperty("Время экспозиции", wxPG_LABEL, 1);
	pg->Append(p);
	p->SetClientData((void*)0x0010);
	p->SetEditor(wxPGEditor_SpinCtrl);
	p->SetAttribute(wxPG_ATTR_MIN, 1);
	p->SetAttribute(wxPG_ATTR_MAX, 65535);
	p->SetAttribute("Wrap", false);
	p->SetHelpString("Установить время экспозиции в миллисекундах (поддерживаются все типы камер)");

	p=new wxFloatProperty("Аналоговое усиление", wxPG_LABEL, 0.0);
	pg->Append(p);
	p->SetClientData((void*)0x2010);
	p->SetHelpString("Установить аналоговое усиление в АЦП (дБ)  (поддерживаются все типы камер)");

	p=new wxUIntProperty("Уровень черного", wxPG_LABEL, 1);
	pg->Append(p);
	p->SetClientData((void*)0x2110);
	p->SetEditor(wxPGEditor_SpinCtrl);
	p->SetAttribute(wxPG_ATTR_MIN, 0);
	p->SetAttribute(wxPG_ATTR_MAX, 65535);
	p->SetAttribute("Wrap", false);
	p->SetHelpString("Установить смещение уровня черного в АЦП (поддерживаются все типы камер)");

	tchoices.Clear();
	tchoices.Add("c ЕМ-усилением");
	tchoices.Add("без ЕМ-усиления");
	p=new wxEnumProperty("Выход матрицы",wxPG_LABEL, tchoices);
	pg->Append(p);
	p->SetClientData((void*)0x8011);
	p->SetHelpString("Выбрать выход матрицы (поддерживаются камеры БИК)");
	p->SetValue("");
//	p->SetChoiceSelection(0);

	p=new wxFloatProperty("ЕМ-усиление", wxPG_LABEL, 1.0);
	pg->Append(p);
	p->SetClientData((void*)0x8240);
	p->SetHelpString("Установить ЕМ-усиление (дБ) (поддерживаются камеры БИК)");

	tchoices.Clear();
	tchoices.Add("0");
	tchoices.Add("1");
	tchoices.Add("2");
	p=new wxEnumProperty("Темп считывания", wxPG_LABEL, tchoices);
	pg->Append(p);
	p->SetClientData((void*)0x4012);
	p->SetHelpString("Задать код темпа считывания (поддерживаются камеры БИК, ШПК, БПК)");
	p->SetValue("");

	p=new wxPointProperty("Начало фрагмента", wxPG_LABEL, wxPoint(0,0));
	pg->Append(p);
	pg->SetPropertyReadOnly("Начало фрагмента",true,0);
	p->SetClientData((void*)0x6010);
	p->SetHelpString("Задать координаты начала фрагмента (поддерживаются все камеры)");

	p=new wxPointProperty("Размер фрагмента", wxPG_LABEL, wxPoint(0,0));
	pg->Append(p);
	pg->SetPropertyReadOnly("Размер фрагмента",true,0);
	p->SetClientData((void*)0x6110);
	p->SetHelpString("Задать размер фрагмента (поддерживаются все камеры)");

	p=new wxBiningProperty("Биннинг", wxPG_LABEL, wxPoint(0,0));
	pg->Append(p);
	pg->SetPropertyReadOnly("Биннинг",true,0);
	p->SetClientData((void*)0x9212);
	p->SetHelpString("Установить величину биннинга (поддерживаются все камеры)");

//	tchoices.Clear();
//	tchoices.Add("остановить");
//	tchoices.Add("запустиь");
	p=new wxUIntProperty("Считывание кадров",wxPG_LABEL, 0);
	pg->Append(p);
	p->SetClientData((void*)0x8124);
	p->SetEditor(wxPGEditor_SpinCtrl);
//	p->SetAttribute(wxPG_ATTR_MIN, 0);
//	p->SetAttribute(wxPG_ATTR_MAX, 4294967295);
//	p->SetAttribute("Wrap", false);
	p->SetHelpString("Запустить считывание заданного количества кадров; 0 - непрерывное считывание (поддерживаются все камеры)");

	p=new wxBoolProperty("Остановить считывание кадров", wxPG_LABEL,false);
	pg->Append(p);
	p->SetClientData((void*)0x8024);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Остановить считывание кадров (поддерживаются все камеры)");

	tchoices.Clear();
	tchoices.Add("выключить");
	tchoices.Add("включить");
	p=new wxEnumProperty("Камера",wxPG_LABEL, tchoices);
	pg->Append(p);
	p->SetClientData((void*)0x8022);
	p->SetHelpString("Включить или выключить камеру (поддерживаются все камеры)");
	p->SetValue("");

	p=new wxEnumProperty("Охлаждение",wxPG_LABEL, tchoices);
	pg->Append(p);
	p->SetClientData((void*)0x8016);
	p->SetHelpString("Включить или выключить охлаждение (поддерживаются камеры БИК)");
	p->SetValue("");

	p=new wxBoolProperty("Сообщить состояние", wxPG_LABEL,false);
	pg->Append(p);
	p->SetClientData((void*)0x0080);
	p->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	p->SetHelpString("Запрос параметров состояния камеры (поддерживаются все камеры)");

	p=new wxPropertyCategory(wxT("Установить IP-адрес и порты кмеры"),wxPG_LABEL);
	pg->Append(p);

	p=new wxStringProperty("IP-адрес камеры", wxPG_LABEL, "192.168.0.1");
	pg->Append(p);
	p->SetClientData((void*)0x8080);
	p->SetHelpString("Установить новый IP-адрес камеры (поддерживаются все камеры)");

	p=new wxUIntProperty("Порт камеры (в. п.)", wxPG_LABEL, 22222);
	pg->Append(p);
	p->SetClientData((void*)0x8080);
	p->SetHelpString("Установить высокоприоритетный порт камеры (поддерживаются все типы камер)");
//
//	p=new wxStringProperty("IP-адрес СУ-С", wxPG_LABEL, "192.168.0.2");
//	pg->Append(p);
//	p->SetClientData((void*)0x8080);
//	p->SetHelpString("Установить новый IP-адрес СУ-С (поддерживаются все камеры)");

	p=new wxUIntProperty("Порт камеры (н. п.)", wxPG_LABEL, 11111);
	pg->Append(p);
	p->SetClientData((void*)0x8080);
	p->SetHelpString("Установить низкоприоритетный порт камеры (поддерживаются все типы камер)");

//	p=new wxStringProperty("IP-адрес техн. ПКВИ", wxPG_LABEL, "192.168.0.3");
//	pg->Append(p);
//	p->SetClientData((void*)0x8080);
//	p->SetHelpString("Установить новый IP-адрес технологического ПКВИ (поддерживаются все камеры)");
//
//	p=new wxUIntProperty("Порт техн. ПКВИ", wxPG_LABEL, 11111);
//	pg->Append(p);
//	p->SetClientData((void*)0x8080);
//	p->SetHelpString("Установить новый порт технологического ПКВИ (поддерживаются все типы камер)");

//	состояние
	pg=m_PropStatus->GetPage(0);
	pg->Append(new wxUIntProperty("Номер камеры",wxPG_LABEL));
	pg->SetPropertyReadOnly("Номер камеры");
	pg->Append(new wxStringProperty("Тип камеры",wxPG_LABEL));
	pg->SetPropertyReadOnly("Тип камеры");
	pg->Append(new wxStringProperty("MAC-адрес камеры",wxPG_LABEL));
	pg->SetPropertyReadOnly("MAC-адрес камеры");
	pg->Append(new wxStringProperty("Счетчик времени",wxPG_LABEL));
	pg->SetPropertyReadOnly("Счетчик времени");
	pg->Append(new wxUIntProperty("Время наработки (мин.)",wxPG_LABEL));
	pg->SetPropertyReadOnly("Время наработки (мин.)");

	pg->Append(new wxPropertyCategory("IP-адрес и порты камеры",wxPG_LABEL));
	pg->Append(new wxStringProperty(" IP-адрес камеры",wxPG_LABEL));
	pg->Append(new wxUIntProperty(" Высокоприоритетный порт камеры",wxPG_LABEL));
//	pg->Append(new wxStringProperty(" IP-адрес СУ-С",wxPG_LABEL));
	pg->Append(new wxUIntProperty(" Низкоприоритетный порт камеры",wxPG_LABEL));
//	pg->Append(new wxStringProperty(" IP-адрес техн. ПКВИ",wxPG_LABEL));
//	pg->Append(new wxUIntProperty(" Порт техн. ПКВИ",wxPG_LABEL));
	pg->SetPropertyReadOnly("IP-адрес и порты камеры");

	pg->Append(new wxPropertyCategory("Готовность к работе",wxPG_LABEL));
	pg->Append(new wxStringProperty(" Охлаждение",wxPG_LABEL));
	pg->Append(new wxStringProperty(" Выход матрицы",wxPG_LABEL));
	pg->Append(new wxStringProperty(" Камера",wxPG_LABEL));
	pg->Append(new wxStringProperty(" Питание камеры",wxPG_LABEL));
	pg->Append(new wxUIntProperty(" Код ошибки",wxPG_LABEL));
	pg->SetPropertyReadOnly("Готовность к работе");

	pg->Append(new wxPropertyCategory("Параметры формирования кадра",wxPG_LABEL));
	pg->Append(new wxUIntProperty(" Время экспозиции (мс)",wxPG_LABEL));
	pg->Append(new wxPointProperty(" Координаты начала фрагмента", wxPG_LABEL, wxPoint(0,0),true));
	pg->Append(new wxPointProperty(" Размер фрагмента", wxPG_LABEL, wxPoint(0,0),true));
	pg->Append(new wxBiningProperty(" Бинирование", wxPG_LABEL, wxPoint(0,0),true));

	pg->Append(new wxUIntProperty(" Величина порога",wxPG_LABEL));
	pg->Append(new wxFloatProperty(" Текущая температура",wxPG_LABEL));
	pg->Append(new wxFloatProperty(" Заданная температура",wxPG_LABEL));
	pg->Append(new wxFloatProperty(" Аналоговое усиление",wxPG_LABEL));

	pg->Append(new wxFloatProperty(" ЕМ-усиление",wxPG_LABEL));
	pg->Append(new wxFloatProperty(" Темп считывания",wxPG_LABEL));
	pg->Append(new wxUIntProperty(" Смещение уровня черного",wxPG_LABEL));
	pg->Append(new wxFloatProperty(" Скорость переноса (мм/с)",wxPG_LABEL));
	pg->Append(new wxUIntProperty(" Количество ступеней интеграции",wxPG_LABEL));
	pg->AppendIn("Параметры формирования кадра",new wxPropertyCategory(" Параметры обработки",wxPG_LABEL));
	pg->Append(new wxBoolProperty("Стабилизация уровня черного",wxPG_LABEL));
	pg->Append(new wxBoolProperty("Иск. темнового тока",wxPG_LABEL));
	pg->Append(new wxBoolProperty("Корр. неравномерности чувств.",wxPG_LABEL));
	pg->AppendIn("Параметры формирования кадра",new wxPropertyCategory(" Алгоритм расчета порога",wxPG_LABEL));
	pg->Append(new wxBoolProperty("Авт. задание порога",wxPG_LABEL));
	pg->Append(new wxBoolProperty("Поиск сверток",wxPG_LABEL));

	pg->SetPropertyReadOnly("Параметры формирования кадра");

	pg->Append(new wxPropertyCategory("Показания датчиков",wxPG_LABEL));
	pg->Append(new wxStringProperty(" оси A",wxPG_LABEL));
	pg->Append(new wxStringProperty(" оси L",wxPG_LABEL));
	pg->Append(new wxStringProperty(" оси R",wxPG_LABEL));
	pg->SetPropertyReadOnly("Показания датчиков");

	m_bFirst=true;

	FillInfoData();
}

void NetworkPanel::SetCameraType(int nType)
{
	wxPropertyGridPage* pg=m_PropCommand->GetPage(0);
	pg->HideProperty("Скорость переноса (мм/с)",nType!=2);
	pg->HideProperty("Количество ступеней интеграции",nType!=2);
	pg->HideProperty("Время экспозиции",nType==2);
	pg->HideProperty("Уровень черного",nType!=2);
	pg->HideProperty("Выход матрицы",nType!=0);
	pg->HideProperty("ЕМ-усиление",nType!=0);
	pg->HideProperty("Темп считывания",nType==2);
	pg->HideProperty("Начало фрагмента",nType==2);
	pg->HideProperty("Размер фрагмента",nType==2);
	pg->HideProperty("Биннинг",nType==2);
//	pg->HideProperty("Охлаждение",nType!=0);

	pg=m_PropStatus->GetPage(0);
//	pg->HideProperty(" Охлаждение",nType!=0);
	pg->HideProperty(" Выход матрицы",nType!=0);
	pg->HideProperty(" Время экспозиции (мс)",nType==2);
	pg->HideProperty(" Координаты начала фрагмента",nType==2);
	pg->HideProperty(" Размер фрагмента",nType==2);
	pg->HideProperty(" Бинирование",nType==2);
	pg->HideProperty(" Скорость переноса (мм/с)",nType!=2);
	pg->HideProperty(" Количество ступеней интеграции",nType!=2);
	pg->HideProperty(" ЕМ-усиление",nType!=0);

}

void NetworkPanel::SetInfo(INFO_DATA* pInfo)
{
	memcpy(&m_InfoData,pInfo,sizeof(INFO_DATA));
	FillInfoData();
//	static bool bFirst=true;
	if(m_bFirst)
	{
		SetCameraType(m_InfoData.btType);
		m_bFirst=false;
	}
}

void NetworkPanel::FillInfoData()
{
	wxPropertyGridPage* pg=m_PropStatus->GetPage(0);

	pg->SetPropertyValue("Номер камеры",m_InfoData.btCamNum);
	wxString s;
	switch(m_InfoData.btType)
	{
		case 0:
			s="БИК";
			break;
		case 1:
			s="ШПК";
			break;
		case 2:
			s="ЛИК";
			break;
		case 3:
			s="БПК";
			break;
	}
	pg->SetPropertyValue("Тип камеры",s);
	s.Printf("%02x:%02x:%02x:%02x:%02x:%02x",m_InfoData.pbtMAC[0],m_InfoData.pbtMAC[1],m_InfoData.pbtMAC[2],
			m_InfoData.pbtMAC[3],m_InfoData.pbtMAC[4],m_InfoData.pbtMAC[5]);
	pg->SetPropertyValue("MAC-адрес камеры",s);
	s.Printf("%02d:%02d:%02d %02d.%02d.%02d",m_InfoData.btHour,m_InfoData.btMinute,m_InfoData.btSecond,m_InfoData.btDay,
			m_InfoData.btMonth,m_InfoData.btYear);
	pg->SetPropertyValue("Счетчик времени",s);
	pg->SetPropertyValue("Время наработки (мин.)",(int)m_InfoData.nCamWork);

	s.Printf("%d.%d.%d.%d",m_InfoData.pbtIPCam[3],m_InfoData.pbtIPCam[2],m_InfoData.pbtIPCam[1],m_InfoData.pbtIPCam[0]);
	pg->SetPropertyValue(" IP-адрес камеры",s);
	pg->SetPropertyValue(" Высокоприоритетный порт камеры",m_InfoData.wPortHP);
//	s.Printf("%d.%d.%d.%d",m_InfoData.pbtIPSUS[0],m_InfoData.pbtIPSUS[1],m_InfoData.pbtIPSUS[2],m_InfoData.pbtIPSUS[3]);
//	pg->SetPropertyValue(" IP-адрес СУ-С",s);
//	pg->SetPropertyValue(" Порт СУ-С",m_InfoData.wPortSUS);
//	s.Printf("%d.%d.%d.%d",m_InfoData.pbtIPPKVI[0],m_InfoData.pbtIPPKVI[1],m_InfoData.pbtIPPKVI[2],m_InfoData.pbtIPPKVI[3]);
//	pg->SetPropertyValue(" IP-адрес техн. ПКВИ",s);
	pg->SetPropertyValue(" Низкоприоритетный порт камеры",m_InfoData.wPortLP);

	s=m_InfoData.Ready.btCooler?"включено":"выключено";
	pg->SetPropertyValue(" Охлаждение",s);
	s=m_InfoData.Ready.btMatrixOutput?"без ЕМ-усиления":"с ЕМ-усилением";
	pg->SetPropertyValue(" Выход матрицы",s);
	s=m_InfoData.Ready.btCamOn?"включена":"выключена";
	pg->SetPropertyValue(" Камера",s);
	s=m_InfoData.Ready.btPower?"в норме":"ошибка питания";
	pg->SetPropertyValue(" Питание камеры",s);
	pg->SetPropertyValue(" Код ошибки",m_InfoData.Ready.btError);

	pg->SetPropertyValue(" Время экспозиции (мс)",(int)(m_InfoData.nExposition/1000));
	wxPointProperty* pProp=(wxPointProperty*)pg->GetProperty(" Координаты начала фрагмента");

	pProp->SetValue(wxPoint(m_InfoData.wOffsetX,m_InfoData.wOffsetY));
	pProp=(wxPointProperty*)pg->GetProperty(" Размер фрагмента");
	pProp->SetValue(wxPoint(m_InfoData.wSizeX,m_InfoData.wSizeY));
	pProp=(wxPointProperty*)pg->GetProperty(" Бинирование");
	pProp->SetValue(wxPoint(m_InfoData.btBinX,m_InfoData.btBinY));

	pg->SetPropertyValue(" Величина порога",m_InfoData.nAmpLevel);
	pg->SetPropertyValue(" Текущая температура",m_InfoData.nTempCurrent/256.0);
	pg->SetPropertyValue(" Заданная температура",m_InfoData.nTempSet/256.0);
	pg->SetPropertyValue(" Аналоговое усиление",m_InfoData.wAnalogAmpl/256.0);
	pg->SetPropertyValue(" ЕМ-усиление",m_InfoData.wEmAmpl/256.0);
	pg->SetPropertyValue(" Темп считывания",m_InfoData.wReadFreq/256.0);
	pg->SetPropertyValue(" Смещение уровня черного",m_InfoData.wBlackLevel);
	pg->SetPropertyValue(" Скорость переноса (мм/с)",m_InfoData.wSpeedTransfer/256.0);
	pg->SetPropertyValue(" Количество ступеней интеграции",m_InfoData.btStepCount);

	s=m_InfoData.CadrProcessing.btStabBlackLevel?"включена":"выключена";
	pg->SetPropertyValue("Стабилизация уровня черного",s);
	s=m_InfoData.CadrProcessing.btExcludeDarkCurrent?"включено":"выключено";
	pg->SetPropertyValue("Иск. темнового тока",s);
	s=m_InfoData.CadrProcessing.btCorrSens?"включена":"выключена";
	pg->SetPropertyValue("Корр. неравномерности чувств.",s);

	s=m_InfoData.AmpLevel.btAutoLevel?"включено":"выключено";
	pg->SetPropertyValue("Авт. задание порога",s);
	s=m_InfoData.AmpLevel.btFindConvolution?"включен":"выключен";
	pg->SetPropertyValue("Поиск сверток",s);

	s.Printf("%03d\xb0 %02d\' %02d\"",m_InfoData.wDatchAGrad,m_InfoData.btDatchAMinutes,m_InfoData.btDatchASeconds);
	pg->SetPropertyValue(" оси A",s);
	s.Printf("%03d\xb0 %02d\' %02d\"",m_InfoData.wDatchLGrad,m_InfoData.btDatchLMinutes,m_InfoData.btDatchLSeconds);
	pg->SetPropertyValue(" оси L",s);
	s.Printf("%03d\xb0 %02d\' %02d\"",m_InfoData.wDatchRGrad,m_InfoData.btDatchRMinutes,m_InfoData.btDatchRSeconds);
	pg->SetPropertyValue(" оси R",s);

}

void NetworkPanel::OnSendCommandClick(wxCommandEvent& event)
{
	wxPropertyGridPage* pg=m_PropCommand->GetPage(0);

	wxPropertyGridIterator it;
	u_char* pCommandData=new u_char[1024];
	memset(pCommandData,0,1024);
	memcpy(pCommandData,"CMD_DATA",8);
	u_int32_t nCommandSize=1024;
	memcpy(pCommandData+8,&nCommandSize,4);
	bool bIPSet=false;
	bool bSendCommand=false;
    for ( it = pg->GetIterator(); !it.AtEnd(); it++ )
    {
        wxPGProperty* p = *it;
        if(!p->HasFlag(wxPG_PROP_MODIFIED))
			continue;
        memcpy(pCommandData+12,&g_nNetCommandNum,4);                        //Номер командного блока (Относительно момента инициализации камеры)
        g_nNetCommandNum++;
        u_int16_t nData=(u_int16_t)(u_int32_t)p->GetClientData();
        wxAny val=p->GetValue();
        switch(nData)
        {
			case 0x8024:                    //Остановить считывание кадров
				{
					if(val.As<bool>())
					{
						memcpy(pCommandData+16,&nData,2);
						p->SetValue(false);
						bSendCommand=true;
					}
					else
						bSendCommand=false;
				}
				break;
        	//2 байта целое
        	case 0x0010:                    //Установить время экспозиции
        	case 0x2110:                    //Установить смещение уровня черного в АЦП
				{
					memcpy(pCommandData+16,&nData,2);
					u_int16_t wVal=val.As<int>();
					memcpy(pCommandData+18,&wVal,2);
					bSendCommand=true;
				}
				break;
        	case 0x1011:                    //Установить количество строк интегрирования
				{
					memcpy(pCommandData+16,&nData,2);
					u_char btVal=(u_char)val.As<int>();
					if(nData==0x1011)
					{
						switch(btVal)
						{
						case 0:
							btVal=4;
							break;
						case 1:
							btVal=16;
							break;
						case 2:
							btVal=32;
							break;
						case 3:
							btVal=64;
							break;
						case 4:
							btVal=96;
							break;
						}
						p->SetValue("");
					}
					memcpy(pCommandData+18,&btVal,1);
					bSendCommand=true;
				}
				break;
        	case 0x8124:                    //Запустить считывание кадров
				{
					memcpy(pCommandData+16,&nData,2);
					u_int32_t dwVal=val.As<int>();
					memcpy(pCommandData+20,&dwVal,4);
					bSendCommand=true;
				}
				break;
			//2 байта 8.8
			case 0x2010:                    //Установить аналоговое усиление в АЦП
			case 0x8240:                    //Установить EM-усиление
        	case 0x1010:                    //Установить скорость вертикального переноса
				{
					memcpy(pCommandData+16,&nData,2);
					float fVal=val.As<float>();
					u_int16_t wVal=(u_int16_t)(fVal*256);
					memcpy(pCommandData+18,&wVal,2);
					bSendCommand=true;
				}
				break;
			case 0x4012:                    //Задать темп считывания
				{
					memcpy(pCommandData+16,&nData,2);
					u_char btData=p->GetChoiceSelection();
					memcpy(pCommandData+18,&btData,1);
					p->SetValue("");
					bSendCommand=true;
				}
			break;
			case 0x8011:                    //Выбрать выход матрицы с EM-усилением
			case 0x8022:                    //Выключить камеру
			case 0x8016:                    //Включить охлаждение
				{
					u_int16_t wData=p->GetChoiceSelection()==0?nData:nData+0x100;
					memcpy(pCommandData+16,&wData,2);
					p->SetValue("");
					bSendCommand=true;
				}
				break;
			case 0x6010:                     //Задать координаты начала фрагмента
			case 0x6110:                     //Задать размер фрагмента
				{
					memcpy(pCommandData+16,&nData,2);
					wxPoint pointVal=val.As<wxPoint>();
					u_int16_t wVal=(u_int16_t)pointVal.x;
					memcpy(pCommandData+18,&wVal,2);
					wVal=(u_int16_t)pointVal.y;
					memcpy(pCommandData+20,&wVal,2);
					bSendCommand=true;
				}
				break;
			case 0x9212:                    //Установить величину биннинга
				{
					memcpy(pCommandData+16,&nData,2);
					wxPoint pointVal=val.As<wxPoint>();
					u_char cVal=(u_char)pointVal.x;
					memcpy(pCommandData+18,&cVal,1);
					cVal=(u_char)pointVal.y;
					memcpy(pCommandData+19,&cVal,1);
					bSendCommand=true;
				}
				break;

			case 0x8080:                    //Установить новые IP-адреса
				{
					if(!bIPSet)
					{
						memcpy(pCommandData+16,&nData,2);
						wxPGProperty* pIPProp=pg->GetProperty("IP-адрес камеры");
						wxString sVal=pIPProp->GetDisplayedString();
						u_char pIP[4],pIPr[4];

						if(StringToIp(sVal,pIPr))
						{
							pIP[3]=pIPr[0];
							pIP[2]=pIPr[1];
							pIP[1]=pIPr[2];
							pIP[0]=pIPr[3];
						}

						memcpy(pCommandData+18+2,pIP,4);
						u_int16_t wVal=(u_int16_t)pg->GetPropertyValueAsInt("Порт камеры (в. п.)");
						memcpy(pCommandData+18+14,&wVal,2);
//						pIPProp=pg->GetProperty("IP-адрес СУ-С");
//						sVal=pIPProp->GetDisplayedString();
//						StringToIp(sVal,pIP);
//						memcpy(pCommandData+28,pIP,4);
//						wVal=(u_int16_t)pg->GetPropertyValueAsInt("Порт СУ-С");
//						memcpy(pCommandData+32,&wVal,2);
//						pIPProp=pg->GetProperty("IP-адрес техн. ПКВИ");
//						sVal=pIPProp->GetDisplayedString();
//						StringToIp(sVal,pIP);
//						memcpy(pCommandData+36,pIP,4);
						wVal=(u_int16_t)pg->GetPropertyValueAsInt("Порт камеры (н. п.)");
						memcpy(pCommandData+18+22,&wVal,2);
						bIPSet=true;
						bSendCommand=true;
					}
					else
						bSendCommand=false;

				}
				break;
			case 0x0080:                    //Сообщить состояние
				if(val.As<bool>())
				{
					memcpy(pCommandData+16,&nData,2);
					p->SetValue(false);
					bSendCommand=true;
				}
				else
					bSendCommand=false;
				break;
        }
        if(bSendCommand)
			g_pFrame->SendCommand(pCommandData);
        p->SetModifiedStatus(false);
        pg->RefreshProperty(p);
    }
	delete pCommandData;
}

void NetworkPanel::OnAutoChange(wxSpinEvent& event)
{
	int nMilliSeconds=m_spinSec->GetValue()*1000;
	if(nMilliSeconds<=0)
		m_timerAuto.Stop();
	else
		m_timerAuto.Start(nMilliSeconds,false);
}

void NetworkPanel::OnTimerAuto(wxTimerEvent& event)
{
	u_char* pCommandData=new u_char[1024];
	memcpy(pCommandData,"CMD_DATA",8);
	int nSize=1024;
	memcpy(pCommandData+8,&nSize,4);
	memcpy(pCommandData+12,&g_nNetCommandNum,4);
	g_nNetCommandNum++;
	int16_t nData=0x0080;
	memcpy(pCommandData+16,&nData,2);
	g_pFrame->SendCommand(pCommandData);
	delete pCommandData;
}


