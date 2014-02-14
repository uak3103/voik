/***************************************************************
 * Name:      PKVIMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2010-03-22
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef PKVIMAIN_H
#define PKVIMAIN_H

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif


#include "PKVIDef.h"
#include "ScreenWindow.h"
#include "ccan.h"

#define BASEPORT 0x378 /* lp1 */

WX_DECLARE_LIST_PTR(BYTE,ListData);
WX_DECLARE_LIST_PTR(wxPoint,ListObj);
//void PacketHandler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);

bool IpToString(const u_char* pIP, wxString& psIP);
bool StringToIp(const wxString sIP, u_char* pIP );

class wxDirTraverserSimple : public wxDirTraverser
{
public:
    wxDirTraverserSimple(wxArrayString& files) : m_files(files) { }
    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        m_files.Add(filename);
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname))
    {
        return wxDIR_CONTINUE;
    }
private:
    wxArrayString& m_files;
};

class wxPointProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxPointProperty)
public:

	wxPointProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
					const wxPoint& value = wxPoint(), bool read_only=false);
	virtual ~wxPointProperty();

	virtual wxVariant ChildChanged(wxVariant& thisValue,
								   int childIndex,
								   wxVariant& childValue) const;
	virtual void RefreshChildren();
	virtual void SetValue(wxPoint p);
protected:

	// I stands for internal
	void SetValueI(const wxPoint& value)
	{
		m_value = WXVARIANT(value);
	}
};

class wxBiningProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxBiningProperty)
public:

	wxBiningProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
					const wxPoint& value = wxPoint(),bool read_only=false);
	virtual ~wxBiningProperty();

	virtual wxVariant ChildChanged(wxVariant& thisValue,
								   int childIndex,
								   wxVariant& childValue) const;
	virtual void RefreshChildren();
	virtual void SetValue(wxPoint p);
protected:

	// I stands for internal
	void SetValueI(const wxPoint& value)
	{
		m_value = WXVARIANT(value);
	}

	int GetSelectFromVal(int nVal);
};

typedef enum
{
	connect_no=0,
	connect_tex=1,
	connect_multi=2,
	connect_auto=3,
}CONNECT_TYPE;

typedef enum
{
	status_no=0,
	status_try_tex=1,
	status_try_multi=2,
	status_connect_tex=3,
	status_connect_multi=4,
}CONNECT_STATUS;

typedef enum
{
    voik_Pusk=0,            //Пуск
    voik_Stop=1,            //Останов
    voik_Prev=2,            //Предыдущий
    voik_Next=3,            //Следующий
    voik_Up=4,              //Ускорить
    voik_Down=5,            //Замедлить
    voik_Get=6,             //Объект
    voik_SendKadr=7,        //Кадр
    voik_BZ=8,              //выполнить БЗ
    voik_Scan=9,           //сканировать папки на станциях
    voik_No=10,
}Voik_Com;

class PKVIFrame: public wxFrame, public wxThreadHelper
{
public:

	PKVIFrame(wxWindow* parent,wxWindowID id = -1);
	virtual ~PKVIFrame();
	u_int CalculateCRC(u_char *pData,int nLength);
	void SetExit();
	bool IsExit();
	bool IsInit();
	bool AddPaintData(u_char* pData, u_int32_t dwSize);
	void AddInfoData(u_char* pData);
	bool GetPaintData();
	void ChangeComPort(int nComPort);
	void SetSource(int nSource);
	int GetSource();
	void SetRecCadrCount(int nCount);
	int GetRecCadrCount();
	bool SetCadrSize(wxSize szCadrSize, bool bLIK);
	wxSize GetCadrSize();
	void ChangeMarkerParams(MARKER* pMarker);
	void ChangeViewParams(const wxPoint pOffset, const wxSize szDim, bool bFinish=false);
	void ChangeStatus(bool bUU, bool bSboy);
	virtual void* Entry();
	void BreakLink();
	void SendCommand(u_char* pCommandData);
	void SetSashWindows(bool bWideView);
	u_char* GetCommand();
	wxMutex *m_mutexPKVI;
	BOOL m_bExit;
	ListData m_listData;

    BYTE* pVoikData;
	BYTE* m_pData;
	BYTE* m_pBmpData;
	DWORD* m_pSummData;
	wxProgressDialog* m_pProgressDlg;
	DWORD m_dwSummCadrs;
	BITMAPFILEHEADER m_bfh;
	BITMAPINFOHEADER m_bih;
	BYTE* m_pSavedBitmap;
	int m_nWidthBitmap;
	int m_nSavedCadrsCount;

	TEX_INFO m_TexInfoStruct;
	INFO_DATA m_InfoData;
	CONVOLUTION* m_pConvolution;
	u_int16_t m_nConvolutionCount;
	ListData m_listCommand;
	bool m_bUU;

	bool m_bInit;
	bool m_bCapturePackets;
	bool m_bSave;
	bool m_bSum;
	bool m_bLIK;
	bool m_bVertFilter;
	int m_nVertOffset;
	CALC_INFO m_CalcInfo;

	wxString m_sDevice;
	wxArrayString m_saIP;
	wxArrayString m_saInterfaces;
	wxString m_sOurIP;
	float m_pfMapGamma[256];

private:
	wxStopWatch m_swCadr;

	int GetSvrAddress(const char* pIface, u_char* pMac);
	void CopyMono8ToRGB(BYTE* pSrc, int nWidth, int nHeight, BYTE* pDest);
	void CalculateSumm();
	void FillTexInfo(bool bFirst=true);
	void FillWorkInfo();
	void RequestUU();
	void GetOurIP();
//	bool GetCameraInfo(CAMERA_INFO* pCamInfo);
//	bool IsMulticast();
	void ShutdownSocket(int* pSocket, bool bMulticast, ip_mreq* pm_addr/*, pfring** ring*/);
	void ChangeConnect();
	bool IsReconnect(CAMERA_INFO* pCamInfo, bool* bMulticast);
	CONNECT_STATUS GetConnectStatus();
	CONNECT_TYPE GetConnectType();
	//(*Handlers(PKVIFrame)
	void OnCP(wxCommandEvent& event);         //UAK
	void OnPKVI(wxCommandEvent& event);
	void OnSaveK(wxCommandEvent& event);
	void OnVertFilter(wxCommandEvent& event);
	void OnContrast(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnCreate(wxWindowCreateEvent& event);
	void OnSettings(wxCommandEvent& event);
	void OnStart(wxCommandEvent& event);
	//*)
	void OnSize(wxSizeEvent& event);
	void OnChangeOrigin(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnTimerCount(wxTimerEvent& event);
	void OnTimerUU(wxTimerEvent& event);
	void OnTimerMinutes(wxTimerEvent& event);
	void OnTimerSum(wxTimerEvent& event);
	void OnTimerConnect(wxTimerEvent& event);
	void OnTimerVoik(wxTimerEvent& event);
	void OnTimerVoik1(wxTimerEvent& event);
    void OnDataReady(wxCommandEvent& event);
    void OnChangeConnectStatus(wxCommandEvent& event);
    void OnInfoReady(wxCommandEvent& event);
    void OnCalcReady(wxCommandEvent& event);
    void OnStatusCameraSelect(wxCommandEvent& event);
    void OnSpinBright(wxSpinDoubleEvent& event);
    void OnSpinContrast(wxSpinDoubleEvent& event);
    void OnSpinTresh(wxSpinEvent& event);
    void OnSpinDelay(wxSpinDoubleEvent& event);
    void OnSpinKoeff(wxSpinDoubleEvent& event);
    void OnStatusConnectSelect(wxCommandEvent& event);
    void OnRBtnDown(wxMouseEvent& event);
	void OnShowStatusBar(wxCommandEvent& event);
	void OnShowToolbar(wxCommandEvent& event);
	void OnCommand(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSum(wxCommandEvent& event);
	void OnBmp(wxCommandEvent& event);
	void OnVOIK(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);          //test
	void OnKeyUp(wxKeyEvent& event);
	void VoikDlgProc();
	void VoikFilmProc();
	void VoikShowKadr(int n);
	void VoikGetObject();
	void VoikSendKadr();
	void VoikBZ();
	void ShowVoikRes();
	void CopyDirSt(wxString from,wxString to);
	void OnChar(wxKeyEvent& event) {LogEvent(event);};
	void LogEvent(wxKeyEvent& event);
	void printProtCam(int num, u_char* pData);
	void OnUpdateShowToolbar(wxUpdateUIEvent& event);
	void OnUpdateShowStatusbar(wxUpdateUIEvent& event);
	void OnIntercept(wxCommandEvent& event);
	void OnUpdateIntercept(wxUpdateUIEvent& event);
	void OnUpdateStart(wxUpdateUIEvent& event);
	void OnUpdateSave(wxUpdateUIEvent& event);
	void OnUpdateSum(wxUpdateUIEvent& event);
	void OnSashDrag(wxSashEvent& event);
	//(*Identifiers(PKVIFrame)
	static const long ID_BIG_SCREEN;
	static const long ID_WORK_INFO;
	static const long ID_TEX_INFO;
	static const long ID_SMALL_SCREEN;
	static const long ID_BIG_SCREEN_SASH;
	static const long ID_WORK_INFO_SASH;
	static const long ID_TEX_INFO_SASH;
	static const long ID_SMALL_SCREEN_SASH;
	static const long idMenuCP;             //UAK
	static const long idMenuPKVI;           //UAK
	static const long idMenuSaveK;          //UAK
	static const long idMenuVertFilter;     //UAK
	static const long idMenuContrast;     //UAK

	static const long idMenuQuit;
	static const long idMenuSettings;
	static const long idMenuStart;
	static const long idMenuStatistic;
	static const long ID_STATUSBAR1;
	static const long idMenuSetting;
	static const long ID_TOOLBARITEM1;
	static const long idMenuIntercept;
	static const long idMenuCommand;
	static const long idMenuSave;
	static const long idMenuSum;
	static const long idMenuBmp;
	static const long ID_TOOLBARITEM2;
	static const long ID_TOOLBAR1;
	//*)
	static const long ID_SHOW_TOOLBAR;
	static const long ID_SHOW_STATUSBAR;
	/*
		//(*Declarations(PKVIFrame)
		wxPanel* m_SmallScreen;
		wxToolBarToolBase* ToolBarItem4;
		wxToolBarToolBase* ToolBarItem3;
		wxMenuItem* MenuItem2;
		wxMenuItem* MenuItem4;
		wxListBox* m_TexInfo;
		wxToolBarToolBase* ToolBarItem6;
		wxSingleInstanceChecker SingleInstanceChecker1;
		wxToolBarToolBase* ToolBarItem1;
		wxMenuItem* MenuItem3;
		wxTextCtrl* m_WorkInfo;
		wxStatusBar* m_StatusBar;
		wxToolBarToolBase* ToolBarItem5;
		wxToolBar* m_ToolBar;
		wxMenu* Menu2;
		wxToolBarToolBase* ToolBarItem2;
		wxPanel* m_BigScreen;
		wxToolBarToolBase* ToolBarItem7;
		//*)
	*/
	wxToolBarToolBase* ToolBarItem1;
	wxToolBarToolBase* ToolBarItem2;
	wxToolBarToolBase* ToolBarItem3;
	wxToolBarToolBase* ToolBarItem4;
	wxToolBarToolBase* ToolBarItem5;
	wxToolBarToolBase* ToolBarItem6;
	wxToolBarToolBase* ToolBarItem7;
	wxToolBarToolBase* ToolBarItem8;
	wxToolBar* m_ToolBar;
	CScreenWindow* m_SmallScreen;
	CScreenWindow* m_BigScreen;
	wxPropertyGridManager*  m_pTexInfo;

	wxTextCtrl* m_WorkInfo;
	wxStatusBar* m_StatusBar;

    wxSashLayoutWindow* m_BigScreenSash;
    wxSashLayoutWindow* m_WorkInfoSash;
    wxSashLayoutWindow* m_TexInfoSash;
    wxSashLayoutWindow* m_SmallScreenSash;
    bool m_bWideView;
//	wxSingleInstanceChecker SingleInstanceChecker1;

	int m_nCommPort;
	int m_comm;
	u_char m_ucStatus;
	bool m_bLPTEnable;
	u_char m_ucSwitch;
//	bool m_bBOOTP;

	bool m_bToolBar;
	bool m_bStatusBar;
	bool m_bPult;
	wxStaticBitmap* m_pStatusBmp;
	wxComboBox* m_pStatusCamera;
	wxComboBox* m_pStatusConnect;
	wxSpinCtrlDouble* m_pStatusContrast;
	wxSpinCtrlDouble* m_pStatusBright;
	wxSpinCtrl* m_pStatusTreshGamma;
	wxSpinCtrlDouble* m_pStatusKoeffGamma;
	u_char docum;
	u_char p_SaveBitmap;
	u_char BUILDROOT;
	u_char FormatSavedCadrs;
	u_char COeth;
	u_char CCeth;
	u_char m_bVoik;
    double* m_tob;
	int    nVoikKadr;
	wxString m_bVoikPath;
	wxTimer m_timerVoik;
	wxTimer m_timerVoik1;
	int m_bVoikTimer;
	ListObj m_VoiklistData;
	u_char nFile,nKadr;
	int voik_pipe[2];
	wxSize szVoikCadrSize;
	wxString v_stan;
	wxArrayString v_paths,v_filenames;
	wxSpinCtrlDouble* m_pStatusDelay;
	char *nlpt;
	wxString sdir;
	int MaxSavedCadrs;
	int flpt;
	u_char m_ucTresh;
	float m_fKoeff;

	bool m_bMulticast;
	CONNECT_TYPE m_connectType;
	CONNECT_STATUS m_connectStatus;
	bool m_bReconnect;

	int m_nSource;
	float m_fXScale;
	float m_fYScale;
	wxSize m_szCadrSize;
//	bool m_bCadrSizeChanged;
	int m_nLevel;
	int m_nDyn;
	bool m_bSboy;
	wxString m_sWorkText;

	int m_nCanType;
	int m_nCanInterface;
	int m_nCanCameraNum;
	int m_nCanSpeed;
	bool m_bContrast;
//	int m_hCan;
	CCan m_Can;
	u_int32_t m_nDeltaStringMks;


	wxTimer m_timerCount;
	wxTimer m_timerUU;
	wxTimer m_timerMinutes;
	wxTimer m_timerSum;
	wxTimer m_timerConnect;

	u_int m_nCadrCount;
	u_int m_nRecCadrCount;
	u_long m_ulMinutes;
	wxFile m_fMinutes;
	wxFile m_fProtCam;
	long m_TimeCadr0;               /*время начала считывания кадров*/
	long m_TimeCadr;                /*время начала кадра*/
	long m_TimeCadrN;               /*число считанных кадров*/
	CameraInfoArray m_CamInfoArr;
	CAMERA_INFO* m_pCamInfo;
	int m_nCamNum;
	DECLARE_EVENT_TABLE()
};

extern PKVIFrame* g_pFrame;
#endif // PKVIMAIN_H
