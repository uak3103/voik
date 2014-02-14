#ifndef NETWORKPANEL_H
#define NETWORKPANEL_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(NetworkPanel)
	#include <wx/listctrl.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/panel.h>
	#include <wx/choice.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(NetworkPanel)
#include <wx/spinctrl.h>
//*)
#include "PKVIDef.h"
extern u_int32_t g_nNetCommandNum;

class NetworkPanel: public wxPanel
{
	public:

		NetworkPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~NetworkPanel();
		void SetInfo(INFO_DATA* pInfo);
/*
		//(*Declarations(NetworkPanel)
		wxListCtrl* m_PropStatus;
		wxListCtrl* m_PropCommand;
		wxChoice* m_Camera_Num;
		wxChoice* m_Can_Interface;
		//*)
*/
		wxPropertyGridManager* m_PropStatus;
		wxPropertyGridManager* m_PropCommand;
		wxSpinCtrl* m_spinSec;

		INFO_DATA m_InfoData;

	protected:
/*
		//(*Identifiers(NetworkPanel)
		static const long ID_PROP_COMMAND;
		static const long ID_LISTCTRL2;
		static const long ID_SEND_COMMAND;
		static const long ID_STATICTEXT1;
		static const long ID_AUTO_STATUS;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_CAN_INTERFACE;
		static const long ID_STATICTEXT4;
		static const long ID_CAMERA_NUM;
		//*)
*/
		static const long ID_PROP_COMMAND;
		static const long ID_PROP_STATUS;
		static const long ID_SEND_COMMAND;
		static const long ID_STATICTEXT1;
		static const long ID_AUTO_STATUS;
		static const long ID_STATICTEXT2;

	private:
/*
		//(*Handlers(NetworkPanel)
		//*)
*/
//		void OnInit(wxInitDialogEvent& event);
//		void OnResize(wxSizeEvent& event);
		void OnSendCommandClick(wxCommandEvent& event);
		void OnAutoChange(wxSpinEvent& event);
		void OnTimerAuto(wxTimerEvent& event);


		wxTimer m_timerAuto;
		bool m_bFirst;
	protected:

		void FillInfoData();
		void Init();
		void SetCameraType(int nType);
		DECLARE_EVENT_TABLE()
};

#endif
