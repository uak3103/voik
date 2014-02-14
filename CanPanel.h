#ifndef CanPanel_H
#define CanPanel_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(CanPanel)
	#include <wx/listctrl.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(CanPanel)
#include <wx/spinctrl.h>
//*)
#include "PKVIDef.h"
#include "ccan.h"

class CanPanel: public wxPanel
{
	public:

		CanPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CanPanel();
		void Init();

/*
		//(*Declarations(CanPanel)
		wxListCtrl* m_PropStatus;
		wxListCtrl* m_PropCommand;
		//*)
*/
		wxPropertyGridManager* m_PropStatus;
		wxPropertyGridManager* m_PropCommand;
		wxSpinCtrl* m_spinSec;
		wxChoice* m_Camera_Num;
		wxChoice* m_ChoiceCan;
		int m_nCanInterface;
		int m_nCanCameraNum;
		CCan* m_pCan;


	protected:
/*
		//(*Identifiers(CanPanel)
		static const long ID_PROP_COMMAND;
		static const long ID_LISTCTRL2;
		static const long ID_SEND_COMMAND;
		static const long ID_STATICTEXT1;
		static const long ID_SECONDS;
		static const long ID_STATICTEXT2;
		//*)
*/
		static const long ID_PROP_COMMAND;
		static const long ID_PROP_STATUS;
		static const long ID_SEND_COMMAND;
		static const long ID_STATICTEXT1;
		static const long ID_AUTO_STATUS;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_CAN_INTERFACE;
		static const long ID_STATICTEXT4;
		static const long ID_CAMERA_NUM;


	private:
/*
		//(*Handlers(CanPanel)
		void OnPaint(wxPaintEvent& event);
		//*)
*/
//		void OnInit(wxInitDialogEvent& event);
//		void OnResize(wxSizeEvent& event);
		void OnSendCommandClick(wxCommandEvent& event);
		void OnAutoChange(wxSpinEvent& event);
		void OnTimerAuto(wxTimerEvent& event);
		void OnTimerGetParameters(wxTimerEvent& event);
//		void OnCanInterface(wxCommandEvent& event);
//		void OnCameraNum(wxCommandEvent& event);

		wxTimer m_timerAuto;
		wxTimer m_timerGetParameters;
		bool m_bFirst;

	protected:

		bool SendQuery(int nNumParam);
		bool RecieveQuery();
		void SetCameraType(int nType);
		wxPGProperty* FindPropertyByData(wxPropertyGridPage* pg, u_int32_t nClientData);
		DECLARE_EVENT_TABLE()
};

#endif
