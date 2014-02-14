#include "wx_pch.h"
#include "PKVIDef.h"
#include "SettingsDlg.h"
#include "ccan.h"


//(*IdInit(SettingsDlg)
const long SettingsDlg::ID_STATICTEXT4 = wxNewId();
const long SettingsDlg::ID_COMPORT = wxNewId();
const long SettingsDlg::ID_STATICTEXT1 = wxNewId();
const long SettingsDlg::ID_CAN_TYPE = wxNewId();
const long SettingsDlg::ID_STATICTEXT11 = wxNewId();
const long SettingsDlg::ID_CAN_SPEED = wxNewId();
const long SettingsDlg::ID_STATICTEXT8 = wxNewId();
const long SettingsDlg::ID_CAN_NUM = wxNewId();
const long SettingsDlg::ID_STATICTEXT9 = wxNewId();
const long SettingsDlg::ID_CAMERA_NUM = wxNewId();
const long SettingsDlg::ID_CHECKBOX2 = wxNewId();
const long SettingsDlg::ID_STATICTEXT5 = wxNewId();
const long SettingsDlg::ID_DEVICE = wxNewId();
const long SettingsDlg::ID_STATICTEXT2 = wxNewId();
const long SettingsDlg::ID_CAMERA_NAME = wxNewId();
const long SettingsDlg::ID_STATICTEXT3 = wxNewId();
const long SettingsDlg::ID_IP = wxNewId();
const long SettingsDlg::ID_STATICTEXT7 = wxNewId();
const long SettingsDlg::ID_PORT = wxNewId();
const long SettingsDlg::ID_STATICTEXT10 = wxNewId();
const long SettingsDlg::ID_GROUP_IP = wxNewId();
const long SettingsDlg::ID_STATICTEXT6 = wxNewId();
const long SettingsDlg::ID_GROUP_PORT = wxNewId();
const long SettingsDlg::ID_ADD_CAMERA = wxNewId();
const long SettingsDlg::ID_DEL_CAMERA = wxNewId();
//*)

BEGIN_EVENT_TABLE(SettingsDlg,wxDialog)
	//(*EventTable(SettingsDlg)
	//*)
	EVT_BUTTON( wxID_OK, SettingsDlg::OnOK)
END_EVENT_TABLE()

SettingsDlg::SettingsDlg(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(SettingsDlg)
	wxStaticText* StaticText10;
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticText* StaticText9;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer7;
	wxStaticText* StaticText2;
	wxStaticText* StaticText8;
	wxStaticText* StaticText11;
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxStaticText* StaticText3;
	wxBoxSizer* BoxSizer11;
	wxButton* Button2;
	wxStaticText* StaticText5;
	wxStaticText* StaticText7;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;
	wxStaticText* StaticText4;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;
	
	Create(parent, wxID_ANY, _("Параметры соединения"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU|wxRESIZE_BORDER, _T("wxID_ANY"));
	SetClientSize(wxSize(720,147));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("COM порт для связи с УУ:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer9->Add(StaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_choiceComPort = new wxChoice(this, ID_COMPORT, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_COMPORT"));
	m_choiceComPort->SetSelection( m_choiceComPort->Append(_("Нет")) );
	m_choiceComPort->Append(_("COM1"));
	m_choiceComPort->Append(_("COM2"));
	m_choiceComPort->Append(_("COM3"));
	m_choiceComPort->Append(_("COM4"));
	BoxSizer9->Add(m_choiceComPort, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer9, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("CAN"));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Тип CAN:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_choiceCanType = new wxChoice(this, ID_CAN_TYPE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CAN_TYPE"));
	m_choiceCanType->SetSelection( m_choiceCanType->Append(_("Нет")) );
	m_choiceCanType->Append(_("ADVANTAGE PCI-1680U"));
	m_choiceCanType->Append(_("ADLINK PCI-7841"));
	m_choiceCanType->Append(_("USB"));
	BoxSizer2->Add(m_choiceCanType, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Скорость CAN (kBps):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	BoxSizer2->Add(StaticText11, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_choiceCanSpeed = new wxChoice(this, ID_CAN_SPEED, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CAN_SPEED"));
	m_choiceCanSpeed->Append(_("125"));
	m_choiceCanSpeed->Append(_("250"));
	m_choiceCanSpeed->Append(_("500"));
	BoxSizer2->Add(m_choiceCanSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("CAN интерфейс:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	BoxSizer10->Add(StaticText8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_choiceCanNum = new wxChoice(this, ID_CAN_NUM, wxDefaultPosition, wxSize(96,29), 0, 0, 0, wxDefaultValidator, _T("ID_CAN_NUM"));
	m_choiceCanNum->SetSelection( m_choiceCanNum->Append(_("Нет")) );
	m_choiceCanNum->Append(_("CAN1"));
	m_choiceCanNum->Append(_("CAN2"));
	BoxSizer10->Add(m_choiceCanNum, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Номер камеры:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	BoxSizer10->Add(StaticText9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_choiceCameraNum = new wxChoice(this, ID_CAMERA_NUM, wxDefaultPosition, wxSize(173,29), 0, 0, 0, wxDefaultValidator, _T("ID_CAMERA_NUM"));
	m_choiceCameraNum->SetSelection( m_choiceCameraNum->Append(_("Камера 1")) );
	m_choiceCameraNum->Append(_("Камера 2"));
	m_choiceCameraNum->Append(_("Камера 3"));
	m_choiceCameraNum->Append(_("Камера 4"));
	BoxSizer10->Add(m_choiceCameraNum, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer10, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer4->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	m_checkContrast = new wxCheckBox(this, ID_CHECKBOX2, _("Использовать автоматическое контрастирование"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	m_checkContrast->SetValue(true);
	BoxSizer4->Add(m_checkContrast, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Источники видеосигнала"));
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Сетевой интерфейс:"), wxDefaultPosition, wxSize(158,17), 0, _T("ID_STATICTEXT5"));
	BoxSizer5->Add(StaticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_choiceDevice = new wxChoice(this, ID_DEVICE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_DEVICE"));
	BoxSizer5->Add(m_choiceDevice, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Имя камеры:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer5->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_comboCameraName = new wxComboBox(this, ID_CAMERA_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CAMERA_NAME"));
	BoxSizer5->Add(m_comboCameraName, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("IP адрес:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer7->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_editIP = new wxTextCtrl(this, ID_IP, _("Text"), wxDefaultPosition, wxSize(179,27), 0, wxDefaultValidator, _T("ID_IP"));
	BoxSizer7->Add(m_editIP, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7->Add(-1,-1,0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Порт:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	BoxSizer7->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_spinPort = new wxSpinCtrl(this, ID_PORT, _T("11111"), wxDefaultPosition, wxDefaultSize, 0, 11111, 99999, 11111, _T("ID_PORT"));
	m_spinPort->SetValue(_T("11111"));
	BoxSizer7->Add(m_spinPort, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer7, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Групповой IP адрес:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	BoxSizer11->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_editGroupIP = new wxTextCtrl(this, ID_GROUP_IP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GROUP_IP"));
	BoxSizer11->Add(m_editGroupIP, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Групповой порт:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer11->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_spinGroupPort = new wxSpinCtrl(this, ID_GROUP_PORT, _T("11111"), wxDefaultPosition, wxDefaultSize, 0, 11111, 99999, 11111, _T("ID_GROUP_PORT"));
	m_spinGroupPort->SetValue(_T("11111"));
	BoxSizer11->Add(m_spinGroupPort, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer11, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	Button2 = new wxButton(this, ID_ADD_CAMERA, _("Добавить камеру"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ADD_CAMERA"));
	BoxSizer3->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_btnDelCamera = new wxButton(this, ID_DEL_CAMERA, _("Удалить камеру"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DEL_CAMERA"));
	BoxSizer3->Add(m_btnDelCamera, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_CAN_TYPE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&SettingsDlg::OnCanTypeSelect);
	Connect(ID_CAN_NUM,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&SettingsDlg::OnCanNumSelect);
	Connect(ID_CAMERA_NAME,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&SettingsDlg::OnCameraNameSelect);
	Connect(ID_ADD_CAMERA,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDlg::OnAddCamera);
	Connect(ID_DEL_CAMERA,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDlg::OnDelCamera);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&SettingsDlg::OnInit);
	//*)
	Connect(ID_CAMERA_NAME,wxEVT_COMMAND_COMBOBOX_DROPDOWN,(wxObjectEventFunction)&SettingsDlg::OnCameraNameDropdown);
//	wxTextValidator val1(wxFILTER_NUMERIC, & m_sIP);
//	wxTextValidator val3(wxFILTER_NUMERIC, & m_sGroupIP);
	m_editIP->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & m_sIP));
	m_editGroupIP->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & m_sGroupIP));
	m_spinPort->SetValidator(wxGenericValidator(& m_nPort));
	m_spinGroupPort->SetValidator(wxGenericValidator(& m_nGroupPort));
	m_choiceDevice->SetValidator(wxGenericValidator(&m_nDevice));
	m_choiceDevice->SetValidator(wxGenericValidator(&m_nCadrFormat));
//	m_comboCameraName->SetValidator(wxGenericValidator(&m_nCamNum));
	m_choiceComPort->SetValidator(wxGenericValidator(&m_nComPort));
	m_choiceCanNum->SetValidator(wxGenericValidator(&m_nCanInterface));
	m_choiceCanType->SetValidator(wxGenericValidator(&m_nCanType));
	m_choiceCanSpeed->SetValidator(wxGenericValidator(&m_nCanSpeed));
	m_choiceCameraNum->SetValidator(wxGenericValidator(&m_nCanCameraNum));
	m_checkContrast->SetValidator(wxGenericValidator(&m_bContrast));

	m_pCamInfo=NULL;
	m_bInit=false;
}

SettingsDlg::~SettingsDlg()
{
	//(*Destroy(SettingsDlg)
	//*)
	RemoveAllCameraInfo(m_CamInfoArr);
}



void SettingsDlg::OnInit(wxInitDialogEvent& event)
{
	for(u_int i=0; i<m_saInterfaces.GetCount(); i++)
	{
		m_choiceDevice->Append(wxString::Format("%s (%s)",m_saInterfaces[i], m_saIP[i]));
		if(m_saInterfaces[i]==m_sDevice)
			m_nDevice=i;
	}
	for(int i=0; i<m_CamInfoArr.GetCount(); i++)
	{
		CAMERA_INFO* pInfo=m_CamInfoArr[i];
		m_comboCameraName->Append(pInfo->sName);
	}
	if(m_nCamNum<m_CamInfoArr.GetCount())
		m_pCamInfo=m_CamInfoArr[m_nCamNum];
	else
		m_pCamInfo=NULL;
	if(m_pCamInfo)
	{
		m_comboCameraName->SetSelection(m_nCamNum);
		m_sIP=m_pCamInfo->sIP;
		m_sGroupIP=m_pCamInfo->sGroupIP;
		m_nPort=m_pCamInfo->nPort;
		m_nGroupPort=m_pCamInfo->nGroupPort;
	}
	m_comboCameraName->Enable(m_pCamInfo);
	m_btnDelCamera->Enable(m_pCamInfo!=NULL);
	FindWindow(ID_IP)->Enable(m_pCamInfo);
	FindWindow(ID_PORT)->Enable(m_pCamInfo);
	FindWindow(ID_GROUP_IP)->Enable(m_pCamInfo);
	FindWindow(ID_GROUP_PORT)->Enable(m_pCamInfo);

	TransferDataToWindow();
	m_bInit=true;
}


void SettingsDlg::OnOK(wxCommandEvent& event)
{
    if ( Validate() && TransferDataFromWindow() )
    {
    	m_sDevice=m_saInterfaces[m_nDevice];
		if(m_pCamInfo)
		{
			m_pCamInfo->sName=m_comboCameraName->GetValue();
			m_pCamInfo->sIP=m_sIP;
			m_pCamInfo->sGroupIP=m_sGroupIP;
			m_pCamInfo->nPort=m_nPort;
			m_pCamInfo->nGroupPort=m_nGroupPort;
		}
		if ( IsModal() )
			EndModal(wxID_OK);
    }
}

void SettingsDlg::OnCanNumSelect(wxCommandEvent& event)
{
	if ( Validate() && TransferDataFromWindow() )
	{
		if(m_nCanInterface>0 && m_nCanType>0)
		{
			CCan can;
			if(!can.Open(m_nCanType,m_nCanInterface,m_nCanSpeed))
			{
				m_nCanInterface=0;
				m_nCanType=0;
				m_choiceCanNum->SetSelection(m_nCanInterface);
				m_choiceCanType->SetSelection(m_nCanType);
			}
			can.Close();
		}
	}
}

void SettingsDlg::OnCanTypeSelect(wxCommandEvent& event)
{
	if ( Validate() && TransferDataFromWindow() )
	{
		if(m_nCanInterface>0 && m_nCanType>0)
		{
			CCan can;
			if(!can.Open(m_nCanType,m_nCanInterface,m_nCanSpeed))
			{
				m_nCanInterface=0;
				m_nCanType=0;
				m_choiceCanNum->SetSelection(m_nCanInterface);
				m_choiceCanType->SetSelection(m_nCanType);
			}
			can.Close();
		}
	}
}

void SettingsDlg::OnCameraNameSelect(wxCommandEvent& event)
{
    if (m_bInit && Validate() && TransferDataFromWindow() )
    {
		if(m_pCamInfo)
		{
			m_pCamInfo->sIP=m_sIP;
			m_pCamInfo->sGroupIP=m_sGroupIP;
			m_pCamInfo->nPort=m_nPort;
			m_pCamInfo->nGroupPort=m_nGroupPort;
		}

		m_nCamNum=event.GetInt();
		m_pCamInfo=m_CamInfoArr[m_nCamNum];
		if(m_pCamInfo)
		{
			m_sIP=m_pCamInfo->sIP;
			m_sGroupIP=m_pCamInfo->sGroupIP;
			m_nPort=m_pCamInfo->nPort;
			m_nGroupPort=m_pCamInfo->nGroupPort;
			TransferDataToWindow();
		}
    }
}

void SettingsDlg::OnAddCamera(wxCommandEvent& event)
{
	if(m_pCamInfo && Validate() && TransferDataFromWindow())
	{
		m_pCamInfo->sName=m_comboCameraName->GetValue();
		m_pCamInfo->sIP=m_sIP;
		m_pCamInfo->sGroupIP=m_sGroupIP;
		m_pCamInfo->nPort=m_nPort;
		m_pCamInfo->nGroupPort=m_nGroupPort;
	}
	m_pCamInfo=new CAMERA_INFO;
	int i=0;
	do
	{
		m_pCamInfo->sName.Printf("Камера %d",i++);

	}while(FindCameraInfo(m_CamInfoArr,m_pCamInfo->sName)!=-1);
	m_pCamInfo->sIP="192.168.1.1";
	m_pCamInfo->sGroupIP="234.5.6.7";
	m_pCamInfo->nPort=11111;
	m_pCamInfo->nGroupPort=22222;
	m_nCamNum=m_CamInfoArr.GetCount();
	m_CamInfoArr.Add(m_pCamInfo);
	m_comboCameraName->Append(m_pCamInfo->sName);
	m_comboCameraName->SetSelection(m_nCamNum);
	m_comboCameraName->Enable(m_pCamInfo);
	m_btnDelCamera->Enable(m_pCamInfo);
	FindWindow(ID_IP)->Enable(m_pCamInfo);
	FindWindow(ID_PORT)->Enable(m_pCamInfo);
	FindWindow(ID_GROUP_IP)->Enable(m_pCamInfo);
	FindWindow(ID_GROUP_PORT)->Enable(m_pCamInfo);

}

void SettingsDlg::OnDelCamera(wxCommandEvent& event)
{
	if(m_pCamInfo)
	{
		wxMessageDialog dlg(NULL,wxString::Format("Вы деййствительно хотите удалить камеру %s из списка камер?",m_pCamInfo->sName),"Удаление камеры",
			wxYES_NO|wxCENTER|wxNO_DEFAULT|wxICON_QUESTION);
		if(dlg.ShowModal()==wxID_YES)
		{
			m_CamInfoArr.RemoveAt(m_nCamNum);
			delete m_pCamInfo;
			m_comboCameraName->Delete(m_nCamNum);
			if(m_CamInfoArr.GetCount()>0)
			{
				m_pCamInfo=m_CamInfoArr[0];
				m_nCamNum=0;
				m_comboCameraName->SetSelection(0);
			}
			else
			{
				m_nCamNum=-1;
				m_pCamInfo=NULL;
				m_comboCameraName->SetSelection(wxNOT_FOUND);
				m_comboCameraName->Enable(m_pCamInfo);
				m_btnDelCamera->Enable(m_pCamInfo);
				FindWindow(ID_IP)->Enable(m_pCamInfo);
				FindWindow(ID_PORT)->Enable(m_pCamInfo);
				FindWindow(ID_GROUP_IP)->Enable(m_pCamInfo);
				FindWindow(ID_GROUP_PORT)->Enable(m_pCamInfo);

			}
		}
	}
}

void SettingsDlg::OnCameraNameDropdown(wxCommandEvent& event)
{
	if(m_pCamInfo && m_bInit)
	{
		m_pCamInfo->sName=m_comboCameraName->GetValue();
		m_comboCameraName->SetString(m_nCamNum,m_pCamInfo->sName);
	}

}

