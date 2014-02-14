#ifndef FILTRDIALOG_H
#define FILTRDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(FiltrDialog)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/checkbox.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(FiltrDialog)
#include <wx/spinctrl.h>
//*)

class FiltrDialog: public wxDialog
{
	public:

		FiltrDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FiltrDialog();
		bool m_bVertFiltr;
		bool m_bHorzFiltr;
		int m_nVertOffset;
		int m_nHorzOffset;
		//(*Declarations(FiltrDialog)
		wxSpinCtrl* m_wndHorzOffset;
		wxSpinCtrl* m_wndVertOffset;
		wxCheckBox* m_wndHorzFiltr;
		wxCheckBox* m_wndVertFiltr;
		//*)

	protected:

		//(*Identifiers(FiltrDialog)
		static const long ID_VERT_FILTR;
		static const long ID_STATICTEXT1;
		static const long ID_VERT_OFFSET;
		static const long ID_HORZ_FILTR;
		static const long ID_STATICTEXT2;
		static const long ID_HORZ_OFFSET;
		//*)

	private:

		//(*Handlers(FiltrDialog)
		void OnVertFiltrClick(wxCommandEvent& event);
		void OnHorzFiltrClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		//*)
//		void OnOK(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
