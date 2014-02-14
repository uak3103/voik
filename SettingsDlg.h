#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H


class SettingsDlg: public wxDialog
{
	public:

		SettingsDlg(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~SettingsDlg();

		//(*Declarations(SettingsDlg)
		wxSpinCtrl* m_spinPort;
		wxChoice* m_choiceDevice;
		wxTextCtrl* m_editIP;
		wxChoice* m_choiceCanSpeed;
		wxStaticText* StaticText6;
		wxTextCtrl* m_editGroupIP;
		wxChoice* m_choiceCanNum;
		wxButton* m_btnDelCamera;
		wxCheckBox* m_checkContrast;
		wxChoice* m_choiceComPort;
		wxSpinCtrl* m_spinGroupPort;
		wxChoice* m_choiceCameraNum;
		wxComboBox* m_comboCameraName;
		wxChoice* m_choiceCanType;
		//*)

		wxString m_sDevice;
		int m_nComPort;
		int m_nCanInterface;
		int m_nCanType;
		int m_nCanSpeed;
		int m_nCanCameraNum;
		bool m_bContrast;
		int m_nCadrFormat;
		wxArrayString m_saIP;
		wxArrayString m_saInterfaces;
		CameraInfoArray m_CamInfoArr;
		int m_nCamNum;
		CAMERA_INFO* m_pCamInfo;
	protected:
		wxString m_sIP;
		int m_nPort;
		wxString m_sGroupIP;
		int m_nGroupPort;
		int m_nDevice;
		bool m_bInit;
		//(*Identifiers(SettingsDlg)
		static const long ID_STATICTEXT4;
		static const long ID_COMPORT;
		static const long ID_STATICTEXT1;
		static const long ID_CAN_TYPE;
		static const long ID_STATICTEXT11;
		static const long ID_CAN_SPEED;
		static const long ID_STATICTEXT8;
		static const long ID_CAN_NUM;
		static const long ID_STATICTEXT9;
		static const long ID_CAMERA_NUM;
		static const long ID_CHECKBOX2;
		static const long ID_STATICTEXT5;
		static const long ID_DEVICE;
		static const long ID_STATICTEXT2;
		static const long ID_CAMERA_NAME;
		static const long ID_STATICTEXT3;
		static const long ID_IP;
		static const long ID_STATICTEXT7;
		static const long ID_PORT;
		static const long ID_STATICTEXT10;
		static const long ID_GROUP_IP;
		static const long ID_STATICTEXT6;
		static const long ID_GROUP_PORT;
		static const long ID_ADD_CAMERA;
		static const long ID_DEL_CAMERA;
		//*)

	private:

		//(*Handlers(SettingsDlg)
		void OnInit(wxInitDialogEvent& event);
		void OnCanNumSelect(wxCommandEvent& event);
		void OnCanTypeSelect(wxCommandEvent& event);
		void OnChoice1Select(wxCommandEvent& event);
		void OnCameraNameSelect(wxCommandEvent& event);
		void OnAddCamera(wxCommandEvent& event);
		void OnDelCamera(wxCommandEvent& event);
		//*)
		void OnCameraNameDropdown(wxCommandEvent& event);
		void OnOK(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
