#ifndef SCREENWINDOW_H
#define SCREENWINDOW_H

#include "wx_pch.h"
#include <wx/glcanvas.h>


BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_CHANGE_ORIGIN, wxEVT_USER_FIRST)
DECLARE_EVENT_TYPE(wxEVT_CHANGE_MASHTAB, wxEVT_USER_FIRST+1)
DECLARE_EVENT_TYPE(wxEVT_DATA_READY, wxEVT_USER_FIRST+2)
DECLARE_EVENT_TYPE(wxEVT_INFO_READY, wxEVT_USER_FIRST+3)
DECLARE_EVENT_TYPE(wxEVT_CALC_READY, wxEVT_USER_FIRST+4)
DECLARE_EVENT_TYPE(wxEVT_CHANGE_CADR_SIZE, wxEVT_USER_FIRST+5)
DECLARE_EVENT_TYPE(wxEVT_CHANGE_CONNECT_STATUS, wxEVT_USER_FIRST+6)
END_DECLARE_EVENT_TYPES()


extern const long ID_MAINFRAME;
class CScreenWindow;

//wxDECLARE_EVENT(wxEVT_CHANGE_ORIGIN,wxCommandEvent);
class CScreenWindow : public wxGLCanvas
{
public:
    CScreenWindow(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                 long style = wxTAB_TRAVERSAL, const wxString& name = _("panel"));
    virtual ~CScreenWindow();
    void AddPaintData(u_char* pData, DWORD dwSize);
    bool m_bZoomed;

    void SetOrigin(float fOriginX, float fOriginY);
	void SetMashtab(const float fMashtab);
	void SetDimention(float fDimX, float fDimY);
	void SetCadrSize(wxSize szCadrSize);
	void SetContrast(float fContrast);
	void SetBright(float fBright);
	void SetGamma(float* pfMapGamma);

	void SetMarkerParams(MARKER* pMarker);
	MARKER m_marker;
	float m_fMashtab;
	float m_fOldMashtab;
	float m_fSmallMashtab;
	float m_fOriginX;
	float m_fOriginY;
	float m_fOffsetX;
	float m_fOffsetY;
	float m_fSmallOriginX;
	float m_fSmallOriginY;
	float m_fDimX;
	float m_fDimY;
    CScreenWindow* m_pSmallWnd;
    u_char* m_pBmpData;
    wxSize m_szCadrSize;
    int m_nRealCadrCY;
protected:
    bool m_bCaptured;
    bool m_bMarkerMode;
    wxPoint m_pStartCapture;
	bool m_bFirstData;
	bool m_bLIK;
	wxGLContext* m_glRC;
	void Change();
	wxStopWatch sw;

private:
    void OnClose(wxCloseEvent& event);
    void OnPaint(wxPaintEvent& event);
   	void OnSize(wxSizeEvent& event);

    void OnLBtnDown(wxMouseEvent& event);
    void OnLBtnUp(wxMouseEvent& event);
    void OnMotion(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnRBtnDown(wxMouseEvent& event);
//	void OnLBtnlick(wxMouseEvent& event);
  	void OnEnterWindow(wxMouseEvent& event);
  	void OnLeaveWindow(wxMouseEvent& event);
  	void OnMBtnDown(wxMouseEvent& event);
  	void OnChangeCadrSize(wxCommandEvent& event);
 DECLARE_EVENT_TABLE()
};

#endif // SCREENWINDOW_H
