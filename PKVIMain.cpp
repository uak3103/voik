/***************************************************************
 * Name:      PKVIMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2010-03-22
 * Copyright:  ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "PKVIMain.h"
#include "SettingsDlg.h"
#include "DatChoiceMain.h"
#include "PrProtMain.h"
#include "NetworkPanel.h"
#include "CanPanel.h"
#include "can4linux.h"
#include <wx/datetime.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include <linux/ppdev.h>

#include "graylamp.xpm"
#include "greenlamp.xpm"
#include "redlamp.xpm"
#include "mylib1/base.h"

#define DiscrVoik 100
#define satosin(sa)     ((struct sockaddr_in *)(sa))
PKVIFrame* g_pFrame=NULL;
const u_int g_nSummCadrs=1000;
//helper functions

void handler_sigsegv(int signum)
{
    wxMessageBox(wxT("SIGSEGV Error"),wxT("POSIX Signal"),wxOK);
    signal(signum,SIG_DFL);
    exit(3);
}

int FindCameraInfo(CameraInfoArray& cameraInfoArr, wxString sCameraName, CAMERA_INFO* pCamInfo)
{
    CAMERA_INFO* pTempInfo;
    for(int i=0; i<cameraInfoArr.GetCount(); i++)
    {
        pTempInfo=cameraInfoArr[i];
        if(pTempInfo->sName==sCameraName)
        {
            if(pCamInfo)
                *pCamInfo=*pTempInfo;
            return i;
        }
    }
    return -1;
}

void* FindData(u_char* pData, int nDataSize, const char* pFind, int nFindSize)
{
    int i=0;
    for(int i=0; i<=nDataSize-nFindSize; i++)
    {
        if(memcmp(pData+i,pFind,nFindSize)==0)
            return pData+i;
    }
    return NULL;
}

void RemoveAllCameraInfo(CameraInfoArray& cameraInfoArr)
{
    for(int i=0; i<cameraInfoArr.GetCount(); i++)
    {
        CAMERA_INFO* pCamInfo=cameraInfoArr[i];
        delete pCamInfo;
    }
    cameraInfoArr.Clear();
}

void CopyCameraInfo(CameraInfoArray& src, CameraInfoArray& dest)
{
    RemoveAllCameraInfo(dest);
    for(int i=0; i<src.GetCount(); i++)
    {
        CAMERA_INFO* pCamInfoSrc=src[i];
        CAMERA_INFO* pCamInfoDest=new CAMERA_INFO;
        *pCamInfoDest=*pCamInfoSrc;
        dest.Add(pCamInfoDest);
    }
}

u_int32_t GetIP(wxString sIP)
{
    u_int32_t nIP=0;
    u_char pIP[4];
    wxString sTemp=sIP;
    wxString sPart;
    int n=0, nCount=0;
    while(nCount<4)
    {
        n=sTemp.Find(".");
        if(n==-1)
            n=sTemp.Len();
        sPart=sTemp.Left(n);
        pIP[3-nCount++]=(u_char)atoi(sPart);
        sTemp=sTemp.Right(sTemp.Len()-n-1);
    };
    if(nCount==4)
    {
        memcpy(&nIP,pIP,4);
        return nIP;
    }
    else
        return 0;
}

wxString GetIP(u_int32_t nIP)
{
    wxString s;
    s.Printf("%d.%d.%d.%d",HIBYTE(HIWORD(nIP)),LOBYTE(HIWORD(nIP)),HIBYTE(LOWORD(nIP)),LOBYTE(LOWORD(nIP)));
    return s;
}


int Compare(const void *arg1, const void *arg2)
{
    return memcmp(arg1,arg2,8);
}


enum wxbuildinfoformat
{
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if(format == long_f)
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}
DEFINE_EVENT_TYPE(wxEVT_DATA_READY)
DEFINE_EVENT_TYPE(wxEVT_INFO_READY)
DEFINE_EVENT_TYPE(wxEVT_CALC_READY)
DEFINE_EVENT_TYPE(wxEVT_CHANGE_CADR_SIZE)
DEFINE_EVENT_TYPE(wxEVT_CHANGE_CONNECT_STATUS)

WX_DEFINE_LIST(ListData);
WX_DEFINE_LIST(ListObj);

// -----------------------------------------------------------------------
// wxPointProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxPointProperty,wxPGProperty,
                               wxPoint,const wxPoint&,TextCtrl)

wxPointProperty::wxPointProperty(const wxString& label, const wxString& name,
                                 const wxPoint& value, bool read_only) : wxPGProperty(label,name)
{
    SetValueI(value);
    AddPrivateChild(new wxIntProperty(wxT("X"),wxPG_LABEL,value.x));
    AddPrivateChild(new wxIntProperty(wxT("Y"),wxPG_LABEL,value.y));
    if(!read_only)
    {
        Item(0)->SetEditor(wxPGEditor_SpinCtrl);
        Item(0)->SetAttribute(wxPG_ATTR_MIN, 0);   // Use constants instead of string
        Item(0)->SetAttribute(wxPG_ATTR_MAX, 4096);    // for reduced binary size.
        Item(0)->SetAttribute(wxT("Wrap"), true);
        Item(0)->SetAttribute(wxPG_ATTR_SPINCTRL_STEP, 2);

        Item(1)->SetEditor(wxPGEditor_SpinCtrl);
        Item(1)->SetAttribute(wxPG_ATTR_MIN, 0);   // Use constants instead of string
        Item(1)->SetAttribute(wxPG_ATTR_MAX, 4096);    // for reduced binary size.
        Item(1)->SetAttribute(wxT("Wrap"), true);
        Item(1)->SetAttribute(wxPG_ATTR_SPINCTRL_STEP, 2);
    }

}

wxPointProperty::~wxPointProperty() { }

void wxPointProperty::RefreshChildren()
{
    if(!GetChildCount())
        return;
    const wxPoint& point = wxPointRefFromVariant(m_value);
    Item(0)->SetValue((long)point.x);
    Item(1)->SetValue((long)point.y);
}

wxVariant wxPointProperty::ChildChanged(wxVariant& thisValue,
                                        int childIndex,
                                        wxVariant& childValue) const
{
    wxPoint& point = wxPointRefFromVariant(thisValue);
    int val = childValue.GetLong();
    switch(childIndex)
    {
    case 0:
        point.x = val;
        break;
    case 1:
        point.y = val;
        break;
    }
    wxVariant newVariant;
    newVariant << point;
    return newVariant;
}

void wxPointProperty::SetValue(wxPoint p)
{
    m_value << p;
    RefreshChildren();
}

// -----------------------------------------------------------------------
// wxBiningProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBiningProperty,wxPGProperty,
                               wxPoint,const wxPoint&,TextCtrl)

wxBiningProperty::wxBiningProperty(const wxString& label, const wxString& name,
                                   const wxPoint& value,bool read_only) : wxPGProperty(label,name)
{
    SetValueI(value);
    wxArrayString tchoices;
    tchoices.Add("1");
    tchoices.Add("2");
    tchoices.Add("4");
    tchoices.Add("8");

    wxPGProperty* p=new wxEnumProperty("X",wxPG_LABEL, tchoices);
    p->SetChoiceSelection(GetSelectFromVal(value.x));
    AddPrivateChild(p);
    p=new wxEnumProperty("Y",wxPG_LABEL, tchoices);
    p->SetChoiceSelection(GetSelectFromVal(value.y));
    AddPrivateChild(p);
    if(read_only)
        SetFlag(wxPG_PROP_READONLY);

}

wxBiningProperty::~wxBiningProperty() { }

int wxBiningProperty::GetSelectFromVal(int nVal)
{
    int nRet=0;
    switch(nVal)
    {
    case 1:
        nRet=0;
        break;
    case 2:
        nRet=1;
        break;
    case 4:
        nRet=2;
        break;
    case 8:
        nRet=3;
        break;
    }
    return nVal;
}

void wxBiningProperty::RefreshChildren()
{
    if(!GetChildCount())
        return;
    const wxPoint& point = wxPointRefFromVariant(m_value);
    Item(0)->SetChoiceSelection(GetSelectFromVal(point.x));
    Item(1)->SetChoiceSelection(GetSelectFromVal(point.y));
}

wxVariant wxBiningProperty::ChildChanged(wxVariant& thisValue,
        int childIndex,
        wxVariant& childValue) const
{
    wxPoint& point = wxPointRefFromVariant(thisValue);
    int val = childValue.GetLong();
    switch(childIndex)
    {
    case 0:
        point.x = val;
        break;
    case 1:
        point.y = val;
        break;
    }
    wxVariant newVariant;
    newVariant << point;
    return newVariant;
}

void wxBiningProperty::SetValue(wxPoint p)
{
    m_value << p;
    RefreshChildren();
}

bool IpToString(const u_char* pIP, wxString& psIP)
{
    if(pIP==NULL)
        return false;
    psIP.Printf(_("%d.%d.%d.%d"),pIP[0],pIP[1],pIP[2],pIP[3]);
    if(psIP.IsEmpty())
        return false;
    return true;
}

bool StringToIp(const wxString sIp, u_char* pIP)
{
    if(sIp.IsEmpty() || pIP==NULL)
        return false;
    else
    {
        ulong ulVal;
        wxStringTokenizer tkz(sIp, wxT("."));
        int i=0;
        while(tkz.HasMoreTokens())
        {
            if(i>3)
                return false;
            wxString token = tkz.GetNextToken();
            if(!token.ToULong(&ulVal))
                return false;
            else
                pIP[i]=BYTE(ulVal);
            i++;
        }
        if(i!=4)
            return false;
        return true;
    }
}

//(*IdInit(PKVIFrame)
const long PKVIFrame::ID_BIG_SCREEN = wxNewId();
const long PKVIFrame::ID_WORK_INFO = wxNewId();
const long PKVIFrame::ID_TEX_INFO = wxNewId();
const long PKVIFrame::ID_SMALL_SCREEN = wxNewId();
const long PKVIFrame::idMenuCP = wxNewId();                 //uak
const long PKVIFrame::idMenuPKVI = wxNewId();               //uak
const long PKVIFrame::idMenuSaveK = wxNewId();              //uak
const long PKVIFrame::idMenuVertFilter = wxNewId();              //uak
const long PKVIFrame::idMenuContrast = wxNewId();              //uak
const long PKVIFrame::idMenuQuit = wxNewId();
const long PKVIFrame::idMenuSettings = wxNewId();
const long PKVIFrame::idMenuStart = wxNewId();
const long PKVIFrame::idMenuStatistic = wxNewId();
const long PKVIFrame::ID_STATUSBAR1 = wxNewId();
const long PKVIFrame::idMenuSetting = wxNewId();
const long PKVIFrame::ID_TOOLBARITEM1 = wxNewId();
const long PKVIFrame::idMenuIntercept = wxNewId();
const long PKVIFrame::idMenuCommand = wxNewId();
const long PKVIFrame::idMenuSave = wxNewId();
const long PKVIFrame::idMenuSum = wxNewId();
const long PKVIFrame::idMenuBmp = wxNewId();
const long PKVIFrame::ID_TOOLBARITEM2 = wxNewId();
const long PKVIFrame::ID_TOOLBAR1 = wxNewId();
//*)
const long ID_MAINFRAME = wxNewId();
const long PKVIFrame::ID_SHOW_TOOLBAR = wxNewId();
const long PKVIFrame::ID_SHOW_STATUSBAR = wxNewId();
const long ID_COMMAND_DLG=wxNewId();
const long ID_STATUS_CAMERA=wxNewId();
const long ID_STATUS_CONNECT=wxNewId();
const long ID_STATUS_CONTRAST=wxNewId();
const long ID_STATUS_BRIGHT=wxNewId();
const long ID_STATUS_TRESH_GAMMA=wxNewId();
const long ID_STATUS_KOEFF_GAMMA=wxNewId();
const long ID_STATUS_DELAY=wxNewId();

const long PKVIFrame::ID_BIG_SCREEN_SASH=wxNewId();
const long PKVIFrame::ID_WORK_INFO_SASH=wxNewId();
const long PKVIFrame::ID_TEX_INFO_SASH=wxNewId();
const long PKVIFrame::ID_SMALL_SCREEN_SASH=wxNewId();



BEGIN_EVENT_TABLE(PKVIFrame,wxFrame)
    //(*EventTable(PKVIFrame)
    EVT_WINDOW_CREATE(PKVIFrame::OnCreate)
    EVT_SIZE(PKVIFrame::OnSize)
    EVT_MENU(idMenuSettings, PKVIFrame::OnSettings)
    EVT_MENU(idMenuStart, PKVIFrame::OnStart)
    //*)
    EVT_MENU(idMenuIntercept, PKVIFrame::OnIntercept)
    EVT_MENU(idMenuCommand, PKVIFrame::OnCommand)
    EVT_MENU(idMenuSave, PKVIFrame::OnSave)
    EVT_MENU(idMenuSum, PKVIFrame::OnSum)
    //    EVT_MENU(idMenuBmp, PKVIFrame::OnBmp)
    EVT_MENU(idMenuBmp, PKVIFrame::OnVOIK)
    EVT_UPDATE_UI(idMenuIntercept, PKVIFrame::OnUpdateIntercept)
    EVT_COMMAND(PKVIFrame::ID_BIG_SCREEN, wxEVT_CHANGE_ORIGIN, PKVIFrame::OnChangeOrigin)
    //	EVT_IDLE(PKVIFrame::OnIdle)
    EVT_TIMER(100,PKVIFrame::OnTimerCount)
    EVT_TIMER(101,PKVIFrame::OnTimerSum)
    EVT_TIMER(102,PKVIFrame::OnTimerUU)
    EVT_TIMER(103,PKVIFrame::OnTimerMinutes)
    EVT_TIMER(104,PKVIFrame::OnTimerConnect)
    EVT_TIMER(105,PKVIFrame::OnTimerVoik)
    EVT_TIMER(106,PKVIFrame::OnTimerVoik1)
    EVT_KEY_DOWN(PKVIFrame::OnKeyDown)                 //test
    EVT_KEY_UP(PKVIFrame::OnKeyUp)
    EVT_COMMAND(ID_MAINFRAME, wxEVT_DATA_READY, PKVIFrame::OnDataReady)
    EVT_COMMAND(ID_MAINFRAME, wxEVT_INFO_READY, PKVIFrame::OnInfoReady)
    EVT_COMMAND(ID_MAINFRAME, wxEVT_CALC_READY, PKVIFrame::OnCalcReady)
    EVT_COMMAND(ID_MAINFRAME, wxEVT_CHANGE_CONNECT_STATUS, PKVIFrame::OnChangeConnectStatus)
    EVT_COMBOBOX(ID_STATUS_CAMERA,PKVIFrame::OnStatusCameraSelect)
    EVT_COMBOBOX(ID_STATUS_CONNECT,PKVIFrame::OnStatusConnectSelect)
    EVT_SPINCTRLDOUBLE(ID_STATUS_BRIGHT,PKVIFrame::OnSpinBright)
    EVT_SPINCTRLDOUBLE(ID_STATUS_CONTRAST,PKVIFrame::OnSpinContrast)
    EVT_SPINCTRLDOUBLE(ID_STATUS_KOEFF_GAMMA,PKVIFrame::OnSpinKoeff)
    EVT_SPINCTRLDOUBLE(ID_STATUS_DELAY,PKVIFrame::OnSpinDelay)
    EVT_SPINCTRL(ID_STATUS_TRESH_GAMMA,PKVIFrame::OnSpinTresh)
    EVT_RIGHT_DOWN(PKVIFrame::OnRBtnDown)
    EVT_MENU(ID_SHOW_TOOLBAR, PKVIFrame::OnShowToolbar)
    EVT_MENU(ID_SHOW_STATUSBAR, PKVIFrame::OnShowStatusBar)
    EVT_UPDATE_UI(ID_SHOW_TOOLBAR, PKVIFrame::OnUpdateShowToolbar)
    EVT_UPDATE_UI(ID_SHOW_STATUSBAR, PKVIFrame::OnUpdateShowStatusbar)
    EVT_UPDATE_UI(idMenuStart, PKVIFrame::OnUpdateStart)
    EVT_UPDATE_UI(idMenuSave, PKVIFrame::OnUpdateSave)
    EVT_UPDATE_UI(idMenuSum, PKVIFrame::OnUpdateSum)
    EVT_SASH_DRAGGED_RANGE(ID_BIG_SCREEN_SASH, ID_SMALL_SCREEN_SASH, PKVIFrame::OnSashDrag)
END_EVENT_TABLE()

PKVIFrame::PKVIFrame(wxWindow* parent,wxWindowID id)
{
    /*
        //(*Initialize(PKVIFrame)
        wxMenuItem* MenuItem1;
        wxMenu* Menu1;
        wxBoxSizer* BoxSizer1;
        wxMenuBar* MenuBar1;
        wxBoxSizer* BoxSizer3;

        Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxNO_FULL_REPAINT_ON_RESIZE, _T("wxID_ANY"));
        SetClientSize(wxSize(298,198));
        Move(wxPoint(-1,-1));
        BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
        m_BigScreen = new wxPanel(this, ID_BIG_SCREEN, wxDefaultPosition, wxSize(300,300), wxSUNKEN_BORDER|wxFULL_REPAINT_ON_RESIZE, _T("ID_BIG_SCREEN"));
        m_BigScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT));
        m_BigScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
        BoxSizer1->Add(m_BigScreen, 3, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
        BoxSizer3 = new wxBoxSizer(wxVERTICAL);
        m_WorkInfo = new wxTextCtrl(this, ID_WORK_INFO, _("Text"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_LEFT|wxTE_WORDWRAP|wxVSCROLL, wxDefaultValidator, _T("ID_WORK_INFO"));
        BoxSizer3->Add(m_WorkInfo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
        m_TexInfo = new wxListBox(this, ID_TEX_INFO, wxDefaultPosition, wxSize(100,90), 0, 0, 0, wxDefaultValidator, _T("ID_TEX_INFO"));
        m_TexInfo->SetForegroundColour(wxColour(255,0,0));
        wxFont m_TexInfoFont(9,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
        m_TexInfo->SetFont(m_TexInfoFont);
        BoxSizer3->Add(m_TexInfo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
        m_SmallScreen = new wxPanel(this, ID_SMALL_SCREEN, wxDefaultPosition, wxSize(100,100), wxFULL_REPAINT_ON_RESIZE, _T("ID_SMALL_SCREEN"));
        m_SmallScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
        m_SmallScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
        BoxSizer3->Add(m_SmallScreen, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
        BoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
        SetSizer(BoxSizer1);
        MenuBar1 = new wxMenuBar();
        Menu1 = new wxMenu();
        MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Выход\tAlt-F4"), _("Выход из программы"), wxITEM_NORMAL);
        Menu1->Append(MenuItem1);
        MenuBar1->Append(Menu1, _("&Файл"));
        Menu2 = new wxMenu();
        MenuItem2 = new wxMenuItem(Menu2, idMenuSettings, _("Параметры..."), _("Установка параметров соединения"), wxITEM_NORMAL);
        Menu2->Append(MenuItem2);
        MenuItem3 = new wxMenuItem(Menu2, idMenuStart, _("Начать сеанс"), _("Начать сеанс передачи данных"), wxITEM_CHECK);
        Menu2->Append(MenuItem3);
        MenuItem4 = new wxMenuItem(Menu2, idMenuStatistic, _("Статистика..."), _("Отобразить статистику соединения"), wxITEM_NORMAL);
        Menu2->Append(MenuItem4);
        MenuBar1->Append(Menu2, _("Интерфейс"));
        SetMenuBar(MenuBar1);
        m_StatusBar = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
        int __wxStatusBarWidths_1[1] = { -10 };
        int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
        m_StatusBar->SetFieldsCount(1,__wxStatusBarWidths_1);
        m_StatusBar->SetStatusStyles(1,__wxStatusBarStyles_1);
        SetStatusBar(m_StatusBar);
        SingleInstanceChecker1.Create(wxTheApp->GetAppName() + _T("_") + wxGetUserId() + _T("_Guard"));
        m_ToolBar = new wxToolBar(this, ID_TOOLBAR1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER, _T("ID_TOOLBAR1"));
        ToolBarItem1 = m_ToolBar->AddTool(idMenuSetting, _("Настройка"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
        ToolBarItem2 = m_ToolBar->AddTool(ID_TOOLBARITEM1, _("Начать сеанс связи"), wxNullBitmap, wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString);
        ToolBarItem3 = m_ToolBar->AddTool(idMenuIntercept, _("Перехват трафика"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
        ToolBarItem4 = m_ToolBar->AddTool(idMenuCommand, _("Управление камерой"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
        ToolBarItem5 = m_ToolBar->AddTool(idMenuSave, _("Сохранение кадров"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
        ToolBarItem6 = m_ToolBar->AddTool(idMenuSum, _("Неравномерность чувствительности"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
        m_ToolBar->AddSeparator();
        ToolBarItem7 = m_ToolBar->AddTool(ID_TOOLBARITEM2, _("Выход из программы"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
        m_ToolBar->Realize();
        SetToolBar(m_ToolBar);
        BoxSizer1->SetSizeHints(this);

        m_BigScreen->Connect(wxEVT_PAINT,(wxObjectEventFunction)&PKVIFrame::Onm_BigScreenPaint,0,this);
        Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PKVIFrame::OnQuit);
        Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&PKVIFrame::OnClose);
        //*)
    */

    signal(SIGSEGV,handler_sigsegv);        //перехватываем segmentation fault

    g_pFrame=this;
    m_fXScale=1;
    m_fYScale=1;

    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem3;
    wxMenuItem* MenuItem4;
    wxMenuItem* MenuItem5;                  //UAK
    wxMenuItem* MenuItem6;                  //UAK
    wxMenuItem* MenuItem7;                  //UAK
    wxMenuItem* MenuItem8;                  //UAK
    wxMenuItem* MenuItem9;                  //UAK
    wxMenu* Menu1;
    wxMenu* Menu2;
//	wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
//	wxBoxSizer* BoxSizer3;
    wxFrame::Create(parent, ID_MAINFRAME, "ПКВИ", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_MAINFRAME"));
    SetClientSize(wxSize(298,198));
    Move(wxPoint(-1,-1));
    m_BigScreenSash=NULL;
    m_WorkInfoSash=NULL;
    m_TexInfoSash=NULL;
    m_SmallScreenSash=NULL;
    m_szCadrSize.x=1024;
    m_szCadrSize.y=1;
    SetSashWindows(true);

//    m_BigScreen = new CScreenWindow(this, ID_BIG_SCREEN, wxDefaultPosition, wxSize(300,300), /*wxSUNKEN_BORDER|*/wxFULL_REPAINT_ON_RESIZE, _T("ID_BIG_SCREEN"));
//    m_BigScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT));
//    m_BigScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
//    m_WorkInfo = new wxTextCtrl(this, ID_WORK_INFO, _(""), wxDefaultPosition, wxDefaultSize,
//                                wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_LEFT|wxTE_WORDWRAP|wxVSCROLL, wxDefaultValidator, _T("ID_WORK_INFO"));
//    m_WorkInfo->SetForegroundColour(wxColour(0,0,255));
//    wxFont m_WorkInfoFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
//    m_WorkInfo->SetFont(m_WorkInfoFont);
//    m_pTexInfo = new wxPropertyGridManager(this, ID_TEX_INFO, wxDefaultPosition, wxSize(100, 90), wxPG_SPLITTER_AUTO_CENTER|wxPG_TOOLTIPS);
//    m_pTexInfo->GetGrid()->SetVerticalSpacing(2);
//    m_pTexInfo->AddPage(wxT("ЗТИ"));
//    m_SmallScreen = new CScreenWindow(this, ID_SMALL_SCREEN, wxDefaultPosition, wxSize(100,100), /*wxSUNKEN_BORDER|*/wxFULL_REPAINT_ON_RESIZE, _T("ID_SMALL_SCREEN"));
//    m_SmallScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
//    m_SmallScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));



    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Выход\tAlt-F4"), _("Выход из программы"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuItem5 = new wxMenuItem(Menu1, idMenuCP, _("СП\tF3"), _("СП"), wxITEM_NORMAL);
    Menu1->Append(MenuItem5);
    MenuItem6 = new wxMenuItem(Menu1, idMenuPKVI, _("ПКВИ\tF4"), _("ПКВИ"), wxITEM_NORMAL);
    Menu1->Append(MenuItem6);
    MenuItem7 = new wxMenuItem(Menu1, idMenuSaveK, _("Save\tF5"), _("Сохранить кадры"), wxITEM_NORMAL);
    Menu1->Append(MenuItem7);
    MenuItem8 = new wxMenuItem(Menu1, idMenuVertFilter, _("VertFilter\tF6"), _("Вкл/Выкл верт. фильтр"), wxITEM_NORMAL);
    Menu1->Append(MenuItem8);
    MenuItem9 = new wxMenuItem(Menu1, idMenuContrast, _("Contrast\tF7"), _("Вкл/Выкл авт. контр."), wxITEM_NORMAL);
    Menu1->Append(MenuItem9);

    MenuBar1->Append(Menu1, _("&Файл"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuSettings, _("Параметры..."), _("Установка параметров соединения"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuItem3 = new wxMenuItem(Menu2, idMenuStart, _("Начать сеанс"), _("Начать сеанс передачи данных"), wxITEM_CHECK);
    Menu2->Append(MenuItem3);
    MenuItem4 = new wxMenuItem(Menu2, idMenuStatistic, _("Статистика..."), _("Отобразить статистику соединения"), wxITEM_NORMAL);
    Menu2->Append(MenuItem4);
    MenuBar1->Append(Menu2, _("Интерфейс"));


    wxConfig config("PKVI");
    m_bToolBar=config.ReadLong("/Interface/ToolBar",1);
    m_bStatusBar=config.ReadLong("/Interface/StatusBar",1);
    m_bPult=config.ReadLong("/Interface/Pult",0);
    m_connectType=(CONNECT_TYPE)config.ReadLong("/Settings/ConnectType",0);

    m_ToolBar = new wxToolBar(this, ID_TOOLBAR1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER|wxTB_NOICONS | wxTB_TEXT, _T("ID_TOOLBAR1"));
    ToolBarItem1 = m_ToolBar->AddTool(idMenuSettings, _("Настройка"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem2 = m_ToolBar->AddTool(idMenuStart, _("Начать сеанс"), wxNullBitmap, wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString);
    ToolBarItem3 = m_ToolBar->AddTool(idMenuIntercept, _("Перехват трафика"), wxNullBitmap, wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString);
    ToolBarItem4 = m_ToolBar->AddTool(idMenuCommand, _("Управление камерой"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem5 = m_ToolBar->AddTool(idMenuSave, _("Сохранение кадров"), wxNullBitmap, wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString);
    ToolBarItem6 = m_ToolBar->AddTool(idMenuSum, _("Неравномерность чувствительности"), wxNullBitmap, wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString);
//    ToolBarItem7 = m_ToolBar->AddTool(idMenuBmp, _("Показать файл"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem7 = m_ToolBar->AddTool(idMenuBmp, _("ВОИК"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    m_ToolBar->AddSeparator();
    ToolBarItem8 = m_ToolBar->AddTool(idMenuQuit, _("Выход"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    m_ToolBar->Realize();
    SetToolBar(m_ToolBar);
    m_ToolBar->Show(m_bToolBar && m_bPult);

    SetMenuBar(MenuBar1);

//	wxImage::AddHandler(wxICOHandler);
    m_StatusBar = new wxStatusBar(this, ID_STATUSBAR1, wxFULL_REPAINT_ON_RESIZE, _T("ID_STATUSBAR1"));
    m_ucTresh=config.ReadLong("/Settings/Tresh",0);
    m_fKoeff=config.ReadDouble("/Settings/Koeff",1.0);
    docum=config.ReadLong("/Settings/docum",0);
    p_SaveBitmap=config.ReadLong("/Settings/SaveBitmap",0);
    BUILDROOT=config.ReadLong("/Settings/BUILDROOT",0);
    MaxSavedCadrs=config.ReadLong("/Settings/MaxSavedCadrs",10);        //ограничение на число сохраненных кадров
    FormatSavedCadrs=config.ReadLong("/Settings/FormatSavedCadrs",0);   //формат имени сохраненных кадров
    COeth=config.ReadLong("/Settings/COeth",0);                         //сетевая карта СО для ГО
    CCeth=config.ReadLong("/Settings/CCeth",0);                         //сетевая карта СC для ГО
    m_bVoik=config.ReadLong("/Settings/Voik",0);                         //работа в режиме ВОИК
    if (BUILDROOT || !m_bPult) m_bVoik=0;
    if (m_bVoik!=0)
    {
        pipe(voik_pipe);
        m_timerVoik.SetOwner(this,105);
        m_timerVoik1.SetOwner(this,106);
        m_pStatusDelay=new wxSpinCtrlDouble(m_StatusBar,ID_STATUS_DELAY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0.1,100000.0,0.9,0.1);
        m_bVoikPath=config.Read("/Settings/VoikPath","/opt/voik");
    }

    if(m_bPult)
    {
        int widths[] = { 26,400,180,180,180,150,-1,250 };
        int style[]= {wxSB_FLAT,wxSB_NORMAL,wxSB_FLAT,wxSB_FLAT,wxSB_FLAT,wxSB_NORMAL,wxSB_NORMAL,wxSB_NORMAL};
        m_StatusBar->SetFieldsCount(8,widths);
        m_StatusBar->SetStatusStyles(8,style);
        m_pStatusBmp=new wxStaticBitmap(m_StatusBar,wxID_ANY,wxIcon(graylamp));
        m_pStatusCamera= new wxComboBox(m_StatusBar,ID_STATUS_CAMERA,wxEmptyString,wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_DROPDOWN|wxCB_READONLY);
        m_pStatusConnect=NULL;
        m_pStatusContrast=new wxSpinCtrlDouble(m_StatusBar,ID_STATUS_CONTRAST,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,-100.0,100.0,1.0,0.01);
        m_pStatusBright=new wxSpinCtrlDouble(m_StatusBar,ID_STATUS_BRIGHT,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,-200,200.0,0.0,0.01);
        m_pStatusContrast->Disable();
        m_pStatusBright->Disable();
        m_connectType=connect_no;
        m_connectStatus=status_no;
        m_StatusBar->SetStatusText("Контраст:",3);
        m_StatusBar->SetStatusText("Яркость:",4);
        for(int i=0; i<256; i++)
        {
            if(i<=m_ucTresh)
                m_pfMapGamma[i]=i*m_fKoeff/256.0;
            else
                m_pfMapGamma[i]=i/256.0;
        }
    }
    else
    {
        int widths[]= {26,450,250,320,-1};
        int style[]= {wxSB_FLAT,wxSB_NORMAL,wxSB_FLAT,wxSB_FLAT,wxSB_NORMAL};
        m_StatusBar->SetFieldsCount(5,widths);
        m_StatusBar->SetStatusStyles(5,style);
        m_pStatusContrast=NULL;
        m_pStatusBright=NULL;
        m_pStatusTreshGamma=NULL;
        m_pStatusKoeffGamma=NULL;
        m_pStatusDelay=NULL;
        m_pStatusBmp=new wxStaticBitmap(m_StatusBar,wxID_ANY,wxIcon(graylamp));
        m_pStatusCamera= new wxComboBox(m_StatusBar,ID_STATUS_CAMERA,wxEmptyString,wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_DROPDOWN|wxCB_READONLY);
        m_pStatusConnect= new wxComboBox(m_StatusBar,ID_STATUS_CONNECT,wxEmptyString,wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_DROPDOWN|wxCB_READONLY);
        if (!BUILDROOT)
        {
            m_pStatusConnect->Append("Нет подключения",(long*)connect_no);
            m_pStatusConnect->Append("Технологическое подключение",(long*)connect_tex);
            m_pStatusConnect->Append("Автоматический режим подключения",(long*)connect_auto);
            m_pStatusConnect->Append("Перехват трафика",(long*)connect_multi);
            //m_connectType=(CONNECT_TYPE)3;
            m_connectStatus=status_no;   //status_try_multi;   //status_no;
            //m_bMulticast=true;
        }
        else
        {
            m_pStatusConnect->Append("Нет подключения",(long*)connect_no);
            //m_pStatusConnect->Append("Автоматический режим подключения",(long*)connect_auto);
            m_pStatusConnect->Append("Перехват трафика",(long*)connect_multi);
            //m_connectType=(CONNECT_TYPE)1;
            //m_connectStatus=status_no;
            m_connectType=connect_no;
            m_connectStatus=status_no;
        }
        m_pStatusConnect->SetSelection(m_connectType);
    }
    SetStatusBar(m_StatusBar);
    m_StatusBar->Show(m_bStatusBar);

//	BoxSizer1->SetSizeHints(this);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PKVIFrame::OnQuit);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&PKVIFrame::OnClose);
    Connect(idMenuCP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PKVIFrame::OnCP);         //UAK
    Connect(idMenuPKVI,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PKVIFrame::OnPKVI);
    Connect(idMenuSaveK,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PKVIFrame::OnSaveK);
    Connect(idMenuVertFilter,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PKVIFrame::OnVertFilter);
    Connect(idMenuContrast,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PKVIFrame::OnContrast);

    m_bExit=false;
    m_bInit=false;
    m_bReconnect=false;

    m_bCapturePackets=false;
    m_bSum=false;

    wxString sIP;
    if (!BUILDROOT)
    {
        wxString sMnFile=wxStandardPaths::Get().GetUserConfigDir()+"/PKVI.mn";
        if(wxFile::Exists(sMnFile))
        {
            if(m_fMinutes.Open(sMnFile,wxFile::read_write))
                m_fMinutes.Read(&m_ulMinutes,sizeof(u_long));
        }
        else
        {
            m_fMinutes.Create(sMnFile);
            m_ulMinutes=935;
            m_fMinutes.Write(&m_ulMinutes,sizeof(u_long));
            m_fMinutes.Flush();
        }
    }
    if(m_bStatusBar)
        m_StatusBar->SetStatusText(wxString::Format("Время наработки (ч:м): %02d:%02d",m_ulMinutes/60,m_ulMinutes%60),m_bPult?7:4);
    config.Read(_("/Settings/DeviceName"),&m_sDevice,"eth0");
//	m_bBOOTP=config.ReadLong("/Settings/BOOTP",0);
    m_nCanInterface=config.ReadLong("/Settings/CanInterface",0);
    m_nCanType=config.ReadLong("/Settings/CanType",0);
    m_nCanSpeed=config.ReadLong("/Settings/CanSpeed",1);
    m_nCanCameraNum=config.ReadLong("/Settings/CanCameraNum",0);
    m_nCommPort=config.ReadLong("/Settings/COM_Port",0);
    //m_bContrast=config.ReadLong("/Settings/Contrast",1);
    m_bContrast=1;              //!!!
    config.Write(_("/Settings/Contrast"),m_bContrast);
    //m_bVertFilter=config.ReadLong("/Settings/VertFilter",0);
    m_bVertFilter=0;            //!!!
    config.Write(_("/Settings/VertFilter"),m_bVertFilter);
    m_nVertOffset=config.ReadLong("/Settings/VertOffset",1);

    if(m_nCanType>0 && m_nCanInterface>0 && m_bPult)
    {
        if(!m_Can.Open(m_nCanType,m_nCanInterface,m_nCanSpeed))
        {
            m_nCanInterface=0;
            m_nCanType=0;
            m_StatusBar->SetStatusText("CAN не подключен",5);
        }
    }
    else if(m_bPult)
        m_StatusBar->SetStatusText("CAN не подключен",5);
    int i=0;
    wxString sName;
    wxString sCameraNum=wxString::Format("/CameraNames/Camera%d",i);
    while(config.Read(sCameraNum,&sName))
    {
        CAMERA_INFO* pCamInfo=new CAMERA_INFO;
        pCamInfo->sName=sName;
        pCamInfo->sGroupIP=config.Read("/"+sName+"/GroupIP","234.5.6.7");
        pCamInfo->sIP=config.Read("/"+sName+"/IP","192.168.1.1");
        pCamInfo->nPort=config.ReadLong("/"+sName+"/Port",11111);
        pCamInfo->nGroupPort=config.ReadLong("/"+sName+"/GroupPort",22222);
        m_CamInfoArr.Add(pCamInfo);
        sCameraNum.Printf("/CameraNames/Camera%d",++i);
    }
    m_nCamNum=config.ReadLong("/Settings/CameraNum",0);
    if(m_nCamNum<m_CamInfoArr.GetCount())
        m_pCamInfo=m_CamInfoArr[m_nCamNum];
    else
    {
        if (!BUILDROOT)
        {
            m_nCamNum=0xffff;
            config.Write("/Settings/CameraNum",m_nCamNum);
            m_pCamInfo=NULL;
        }
    }
    if(m_pStatusCamera)
    {
        if (m_bVoik!=0)
        {
            m_pStatusCamera->Append("Пуск",(long*)voik_Pusk);
            m_pStatusCamera->Append("Останов",(long*)voik_Stop);
            m_pStatusCamera->Append("Предыдущий",(long*)voik_Prev);
            m_pStatusCamera->Append("Следующий",(long*)voik_Next);
            m_pStatusCamera->Append("Ускорить",(long*)voik_Up);
            m_pStatusCamera->Append("Замедлить",(long*)voik_Down);
            m_pStatusCamera->Append("Объект",(long*)voik_Get);
            m_pStatusCamera->Append("Кадр",(long*)voik_SendKadr);
            m_pStatusCamera->Append("БЗ",(long*)voik_BZ);
            m_pStatusCamera->Append("Скан",(long*)voik_Scan);
            m_pStatusCamera->Append("--------",(long*)voik_No);
            m_pStatusCamera->SetSelection(voik_No);
        }
        else
        {
            for(int i=0; i<m_CamInfoArr.GetCount(); i++)
                m_pStatusCamera->Append(m_CamInfoArr[i]->sName);
            m_pStatusCamera->SetSelection(m_nCamNum);
        }
    }
    GetOurIP();
    m_ToolBar->EnableTool(idMenuStart, !m_sDevice.IsEmpty() && m_pCamInfo!=NULL);

    m_listData.DeleteContents(false);

//    FillTexInfo();
    memset(&m_InfoData,0,sizeof(INFO_DATA));

    m_pConvolution=NULL;
    m_nConvolutionCount=0;

    wxDisplay display(0);
    wxRect r=display.GetGeometry();
    SetSize(r.x, r.y,r.width,r.height);
    m_ucStatus=0x6f; //0x5f для ЦТК2

    //PARPORT
    nlpt="/dev/parport0";
    flpt=open(nlpt,O_RDWR);
    if (flpt<0)
    {
        wxString s;
        //s.Printf("%d:%s\n",nlpt,strerror(errno));
        //wxMessageBox(_T("Невозможно получить доступ к порту /dev/parport0 "+s));
    }
    else
    {
        int res;
        res=ioctl(flpt,PPCLAIM,0);     //claim access to the port
        if (res<0)
        {
            wxString s;
            s.Printf("%d:%s\n",res,strerror(errno));
            wxMessageBox(_T("parport0 ioctl: "+s));
        }

        wxString msg;
        unsigned char bbb=0,sss=0;
        bbb^=0x6f;                        //CTK1
        res=ioctl(flpt,PPWDATA,&bbb);
        msg.Printf("PPWDATA=%8x %8x:%s\n",bbb,res,strerror(errno));
        //wxMessageBox(_T(""+msg));
        m_bLPTEnable=true;

        res=ioctl(flpt,PPRSTATUS,&sss);
        msg.Printf("PPRSTATUS=%8x %8x:%s\n",sss,res,strerror(errno));
        //wxMessageBox(_T(msg));
        m_ucSwitch=sss;

//        m_WorkInfo->Connect(wxEVT_CHAR,wxKeyEventHandler(PKVIFrame::OnChar),NULL,this);  //???
    }
    /*
    	if(ioperm(BASEPORT, 3, 1))
    	{
    		wxMessageBox(_T("Невозможно получить доступ к LPT порту"));
    		m_bLPTEnable=false;

    	}
    	else
    	{
    		outb(m_ucStatus,BASEPORT);
    		m_bLPTEnable=true;
    		m_ucSwitch=inb(BASEPORT+1);
    	}
    */
    m_bUU=false;
    m_mutexPKVI=new wxMutex;
    m_bLIK=false;
    m_pBmpData=NULL;
    m_pSavedBitmap=NULL;
    m_pSummData=NULL;
    m_pProgressDlg=NULL;
    m_pData=NULL;
    m_dwSummCadrs=0;

    m_bSave=false;
//    m_BigScreen->m_bZoomed=true;
//    m_SmallScreen->m_bZoomed=false;
//    m_BigScreen->m_pSmallWnd=m_SmallScreen;
//    m_BigScreen->SetCadrSize(m_szCadrSize);
//    m_SmallScreen->SetCadrSize(m_szCadrSize);
    m_timerCount.SetOwner(this,100);
    m_timerSum.SetOwner(this,101);

    m_comm=0;
    ChangeComPort(m_nCommPort);
    m_timerUU.SetOwner(this,102);                   //FillTexInfo
    m_timerUU.Start(500,false);
    m_timerMinutes.SetOwner(this,103);
    m_timerMinutes.Start(60000,false);
    m_timerConnect.SetOwner(this,104);
    m_timerConnect.Start(500,false);
    m_nSource=1;
    m_nLevel=0;
    m_nDyn=1;
    m_bSboy=false;
    MARKER marker;
    marker.bVisible=false;
    marker.Coord=wxPoint(m_szCadrSize.x/2,m_szCadrSize.y/2);
    marker.nSize=config.ReadLong("/Settings/MarkerSize",20);
    marker.nType=cross;
    ChangeMarkerParams(&marker);
    ChangeViewParams(wxPoint(0,0),wxSize(m_szCadrSize.x,m_szCadrSize.y),true);
    ChangeStatus(m_bUU,m_bSboy);
    SetSource(m_nSource);
    long lStyle=wxFULLSCREEN_NOMENUBAR|wxFULLSCREEN_NOBORDER|wxFULLSCREEN_NOCAPTION;
    ShowFullScreen(true,lStyle);

    memset(&m_bfh,0,sizeof(m_bfh));
    m_bfh.bfType=MAKEWORD('B','M');
    m_bfh.bfSize=sizeof(m_bfh)+sizeof(m_bih)+1024*768*3;
    m_bfh.bfOffBits=sizeof(m_bfh)+sizeof(m_bih);

    memset(&m_bih,0,sizeof(m_bih));
    m_bih.biSize=sizeof(m_bih);
    m_bih.biWidth=1024;
    m_bih.biHeight=768;
    m_bih.biPlanes=1;
    m_bih.biBitCount=24;
    m_bih.biCompression=BI_RGB;

//	RequestUU();
    m_timerCount.Start(1000,false);
    wxThreadHelper::CreateThread();
    ChangeConnect();

}

PKVIFrame::~PKVIFrame()
{
    if (m_bVoik!=0)
    {
        ::close(voik_pipe[0]);
        ::close(voik_pipe[1]);
        m_VoiklistData.DeleteContents(true);
        m_VoiklistData.Clear();
    }
    if (flpt>0)  //PARPORT0
    {
        int res;
        res=ioctl(flpt,PPRELEASE,0);
        close(flpt);
    }

    SetExit();
    if(GetThread() && GetThread()->IsRunning())
        GetThread()->Wait();
    m_listData.DeleteContents(true);
    m_listData.Clear();
    m_listCommand.DeleteContents(true);
    m_listCommand.Clear();
    if(m_timerCount.IsRunning())
        m_timerCount.Stop();
    if(m_timerSum.IsRunning())
        m_timerSum.Stop();
    if(m_timerUU.IsRunning())
        m_timerUU.Stop();
    if(m_timerConnect.IsRunning())
        m_timerConnect.Stop();
    if(m_timerVoik.IsRunning())
        m_timerVoik.Stop();
    if(m_timerVoik1.IsRunning())
        m_timerVoik1.Stop();
    if(m_comm)
        close(m_comm);
    delete m_mutexPKVI;
    if(m_pSummData)
        delete [] m_pSummData;
    if(m_pSavedBitmap)
        delete [] m_pSavedBitmap;
    RemoveAllCameraInfo(m_CamInfoArr);
}

void PKVIFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void PKVIFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


void PKVIFrame::OnCreate(wxWindowCreateEvent& event)
{
//			rect=m_StatusBar->GetRect();
//			rect.y-=5;
//			rect.height+=5;
//			m_StatusBar->SetSize(rect);

}

void PKVIFrame::OnClose(wxCloseEvent& event)
{
    if(m_pProgressDlg)
    {
        m_pProgressDlg->Destroy();
        m_pProgressDlg=NULL;
    }

    Destroy();
}

void PKVIFrame::SetSashWindows(bool bWideView)
{
    m_bWideView=bWideView;
    if(m_BigScreenSash)
        m_BigScreenSash->Destroy();
    if(m_WorkInfoSash)
        m_WorkInfoSash->Destroy();
    if(m_TexInfoSash)
        m_TexInfoSash->Destroy();
    if(m_SmallScreenSash)
        m_SmallScreenSash->Destroy();
    wxSize szSize;
    szSize=GetClientSize();
    if(!m_bWideView)
    {
        m_BigScreenSash=new wxSashLayoutWindow(this, ID_BIG_SCREEN_SASH,
                                               wxDefaultPosition, wxSize(200, 30),
                                               wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_BigScreenSash->SetDefaultSize(wxSize(szSize.GetWidth()*4/5,100));
        m_BigScreenSash->SetOrientation(wxLAYOUT_VERTICAL);
        m_BigScreenSash->SetAlignment(wxLAYOUT_LEFT);
        m_BigScreenSash->SetBackgroundColour(wxColour(255, 0, 0));
        m_BigScreenSash->SetSashVisible(wxSASH_RIGHT, true);

        m_WorkInfoSash=new wxSashLayoutWindow(this, ID_WORK_INFO_SASH,
                                              wxDefaultPosition, wxSize(200, 30),
                                              wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_WorkInfoSash->SetDefaultSize(wxSize(100, szSize.GetHeight()/3));
        m_WorkInfoSash->SetOrientation(wxLAYOUT_HORIZONTAL);
        m_WorkInfoSash->SetAlignment(wxLAYOUT_TOP);
        m_WorkInfoSash->SetBackgroundColour(wxColour(0, 0, 255));
        m_WorkInfoSash->SetSashVisible(wxSASH_BOTTOM, true);

        m_TexInfoSash=new wxSashLayoutWindow(this, ID_TEX_INFO_SASH,
                                             wxDefaultPosition, wxSize(200, 30),
                                             wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_TexInfoSash->SetDefaultSize(wxSize(100, szSize.GetHeight()/3));
        m_TexInfoSash->SetOrientation(wxLAYOUT_HORIZONTAL);
        m_TexInfoSash->SetAlignment(wxLAYOUT_TOP);
        m_TexInfoSash->SetBackgroundColour(wxColour(0, 255, 0));
        m_TexInfoSash->SetSashVisible(wxSASH_BOTTOM, true);

        m_SmallScreenSash=new wxSashLayoutWindow(this, ID_SMALL_SCREEN_SASH,
                wxDefaultPosition, wxSize(200, 30),
                wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_SmallScreenSash->SetDefaultSize(wxSize(100, szSize.GetHeight()/3));
        m_SmallScreenSash->SetOrientation(wxLAYOUT_HORIZONTAL);
        m_SmallScreenSash->SetBackgroundColour(wxColour(0, 255, 255));
        m_SmallScreenSash->SetAlignment(wxLAYOUT_TOP);

        m_BigScreen = new CScreenWindow(m_BigScreenSash, ID_BIG_SCREEN, wxDefaultPosition, wxSize(300,300), /*wxSUNKEN_BORDER|*/wxFULL_REPAINT_ON_RESIZE, _T("ID_BIG_SCREEN"));
        m_BigScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT));
        m_BigScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));

        m_WorkInfo = new wxTextCtrl(m_WorkInfoSash, ID_WORK_INFO, _(""), wxDefaultPosition, wxDefaultSize,
                                    wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_LEFT|wxTE_WORDWRAP|wxVSCROLL, wxDefaultValidator, _T("ID_WORK_INFO"));
//        m_WorkInfo->SetForegroundColour(wxColour(0,0,255));
        wxFont m_WorkInfoFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
        m_WorkInfo->SetFont(m_WorkInfoFont);

        m_pTexInfo = new wxPropertyGridManager(m_TexInfoSash, ID_TEX_INFO, wxDefaultPosition, wxSize(100, 90), wxPG_SPLITTER_AUTO_CENTER|wxPG_TOOLTIPS);
        m_pTexInfo->GetGrid()->SetVerticalSpacing(2);
        m_pTexInfo->AddPage(wxT("ЗТИ"));

        m_SmallScreen = new CScreenWindow(m_SmallScreenSash, ID_SMALL_SCREEN, wxDefaultPosition, wxSize(100,100), /*wxSUNKEN_BORDER|*/wxFULL_REPAINT_ON_RESIZE, _T("ID_SMALL_SCREEN"));
        m_SmallScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
        m_SmallScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));

        m_BigScreen->m_bZoomed=true;
        m_SmallScreen->m_bZoomed=false;
        m_BigScreen->m_pSmallWnd=m_SmallScreen;
        m_BigScreen->SetCadrSize(m_szCadrSize);
        m_SmallScreen->SetCadrSize(m_szCadrSize);
        MARKER marker;
        marker.bVisible=false;
        marker.Coord=wxPoint(m_szCadrSize.x/2,m_szCadrSize.y/2);
        wxConfig config("PKVI");
        marker.nSize=config.ReadLong("/Settings/MarkerSize",20);
        marker.nType=cross;
        m_BigScreen->SetMarkerParams(&marker);
        FillTexInfo(true);
        FillWorkInfo();


    }
    else
    {
        m_BigScreenSash=new wxSashLayoutWindow(this, ID_BIG_SCREEN_SASH,
                                               wxDefaultPosition, wxSize(200, 30),
                                               wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_BigScreenSash->SetDefaultSize(wxSize(1000, szSize.GetHeight()*3/4));
        m_BigScreenSash->SetOrientation(wxLAYOUT_HORIZONTAL);
        m_BigScreenSash->SetAlignment(wxLAYOUT_TOP);
        m_BigScreenSash->SetBackgroundColour(wxColour(255, 0, 0));
        m_BigScreenSash->SetSashVisible(wxSASH_BOTTOM, true);

        m_WorkInfoSash=new wxSashLayoutWindow(this, ID_WORK_INFO_SASH,
                                              wxDefaultPosition, wxSize(200, 30),
                                              wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_WorkInfoSash->SetDefaultSize(wxSize(szSize.GetWidth()/3, 100));
        m_WorkInfoSash->SetOrientation(wxLAYOUT_VERTICAL);
        m_WorkInfoSash->SetAlignment(wxLAYOUT_LEFT);
        m_WorkInfoSash->SetBackgroundColour(wxColour(0, 0, 255));
        m_WorkInfoSash->SetSashVisible(wxSASH_RIGHT, true);

        m_TexInfoSash=new wxSashLayoutWindow(this, ID_TEX_INFO_SASH,
                                             wxDefaultPosition, wxSize(200, 30),
                                             wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_TexInfoSash->SetDefaultSize(wxSize(szSize.GetWidth()/3, 100));
        m_TexInfoSash->SetOrientation(wxLAYOUT_VERTICAL);
        m_TexInfoSash->SetAlignment(wxLAYOUT_LEFT);
        m_TexInfoSash->SetBackgroundColour(wxColour(0, 255, 0));
        m_TexInfoSash->SetSashVisible(wxSASH_RIGHT, true);

        m_SmallScreenSash=new wxSashLayoutWindow(this, ID_SMALL_SCREEN_SASH,
                wxDefaultPosition, wxSize(200, 30),
                wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        m_SmallScreenSash->SetDefaultSize(wxSize(szSize.GetWidth()/3, 100));
        m_SmallScreenSash->SetOrientation(wxLAYOUT_VERTICAL);
        m_SmallScreenSash->SetBackgroundColour(wxColour(0, 255, 255));
        m_SmallScreenSash->SetAlignment(wxLAYOUT_LEFT);


        m_BigScreen = new CScreenWindow(m_BigScreenSash, ID_BIG_SCREEN, wxDefaultPosition, wxSize(300,300), /*wxSUNKEN_BORDER|*/wxFULL_REPAINT_ON_RESIZE, _T("ID_BIG_SCREEN"));
        m_BigScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT));
        m_BigScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));

        m_WorkInfo = new wxTextCtrl(m_WorkInfoSash, ID_WORK_INFO, _(""), wxDefaultPosition, wxDefaultSize,
                                    wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_LEFT|wxTE_WORDWRAP|wxVSCROLL, wxDefaultValidator, _T("ID_WORK_INFO"));
//        m_WorkInfo->SetForegroundColour(wxColour(0,0,255));
        wxFont m_WorkInfoFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
        m_WorkInfo->SetFont(m_WorkInfoFont);

        m_pTexInfo = new wxPropertyGridManager(m_TexInfoSash, ID_TEX_INFO, wxDefaultPosition, wxSize(100, 90), wxPG_SPLITTER_AUTO_CENTER|wxPG_TOOLTIPS);
        m_pTexInfo->GetGrid()->SetVerticalSpacing(2);
        m_pTexInfo->AddPage(wxT("ЗТИ"));

        m_SmallScreen = new CScreenWindow(m_SmallScreenSash, ID_SMALL_SCREEN, wxDefaultPosition, wxSize(100,100), /*wxSUNKEN_BORDER|*/wxFULL_REPAINT_ON_RESIZE, _T("ID_SMALL_SCREEN"));
        m_SmallScreen->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
        m_SmallScreen->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));

        m_BigScreen->m_bZoomed=true;
        m_SmallScreen->m_bZoomed=false;
        m_BigScreen->m_pSmallWnd=m_SmallScreen;
        m_BigScreen->SetCadrSize(m_szCadrSize);
        m_SmallScreen->SetCadrSize(m_szCadrSize);
        MARKER marker;
        marker.bVisible=false;
        marker.Coord=wxPoint(m_szCadrSize.x/2,m_szCadrSize.y/2);
        wxConfig config("PKVI");
        marker.nSize=config.ReadLong("/Settings/MarkerSize",20);
        marker.nType=cross;
        m_BigScreen->SetMarkerParams(&marker);

        FillTexInfo(true);
        FillWorkInfo();

    }

    m_WorkInfo->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(PKVIFrame::OnKeyDown),NULL,this);
    //m_pTexInfo->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(PKVIFrame::OnKeyDown),NULL,this);
    //m_BigScreen->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(PKVIFrame::OnKeyDown),NULL,this);
    //m_SmallScreen->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(PKVIFrame::OnKeyDown),NULL,this);
    //m_BigScreenSash->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(PKVIFrame::OnKeyDown),NULL,this);
    //m_SmallScreenSash->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(PKVIFrame::OnKeyDown),NULL,this);
    //Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&PKVIFrame::OnKeyDown);
    m_WorkInfo->SetFocus();

    wxLayoutAlgorithm layout;
    layout.LayoutFrame(this);
}

void PKVIFrame::OnSize(wxSizeEvent& event)
{

    if(m_BigScreenSash)
    {
        if(m_bWideView)
        {
            m_BigScreenSash->SetDefaultSize(wxSize(100, event.GetSize().GetHeight()*3/4));
            m_WorkInfoSash->SetDefaultSize(wxSize(event.GetSize().GetWidth()/3,100));
            m_TexInfoSash->SetDefaultSize(wxSize(event.GetSize().GetWidth()/3,100));
            m_SmallScreenSash->SetDefaultSize(wxSize(event.GetSize().GetWidth()/3,100));
        }
        else
        {
            m_BigScreenSash->SetDefaultSize(wxSize(event.GetSize().GetWidth()*4/5,100));
            m_WorkInfoSash->SetDefaultSize(wxSize(100,event.GetSize().GetHeight()/3));
            m_TexInfoSash->SetDefaultSize(wxSize(100,event.GetSize().GetHeight()/3));
            m_SmallScreenSash->SetDefaultSize(wxSize(100,event.GetSize().GetHeight()/3));
        }
        wxLayoutAlgorithm layout;
        layout.LayoutFrame(this);

    }

    static bool bFirst=true;
    wxRect rect;
    if(m_StatusBar && !m_bPult)
    {
        if(bFirst)
        {
            rect=m_StatusBar->GetRect();
            rect.y-=8;
            rect.height+=8;
            m_StatusBar->SetSize(rect);
            bFirst=false;
        }
        m_StatusBar->GetFieldRect(0, rect);
        wxSize size = m_pStatusBmp->GetSize();

        m_pStatusBmp->Move(rect.x + (rect.width - size.x) / 2,
                           rect.y + (rect.height - size.y) / 2);

        m_StatusBar->GetFieldRect(2, rect);
        m_pStatusCamera->SetSize(rect);
        m_StatusBar->GetFieldRect(3, rect);
        m_pStatusConnect->SetSize(rect);

    }
    else if(m_StatusBar)
    {
        if(bFirst)
        {
            rect=m_StatusBar->GetRect();
            rect.y-=8;
            rect.height+=8;
            m_StatusBar->SetSize(rect);
            bFirst=false;
        }
        m_StatusBar->GetFieldRect(0, rect);
        wxSize size = m_pStatusBmp->GetSize();

        m_pStatusBmp->Move(rect.x + (rect.width - size.x) / 2,
                           rect.y + (rect.height - size.y) / 2);

        m_StatusBar->GetFieldRect(2, rect);
        m_pStatusCamera->SetSize(rect);

        m_StatusBar->GetFieldRect(3, rect);
        rect.width/=2;
        rect.x+=rect.width;
        m_pStatusContrast->SetSize(rect);
        m_StatusBar->GetFieldRect(4, rect);
        rect.width/=2;
        rect.x+=rect.width;
        m_pStatusBright->SetSize(rect);
        if (m_bVoik!=0)
        {
            m_StatusBar->SetStatusText("Задержка:",5);
            m_StatusBar->GetFieldRect(5, rect);
            rect.width/=2;
            rect.x+=rect.width;
            m_pStatusDelay->SetSize(rect);
        }
//		m_StatusBar->GetFieldRect(4, rect);
//		rect.width/=2;
//		rect.x+=rect.width;
//		m_pStatusKoeffGamma->SetSize(rect);
    }
    if(m_pTexInfo!=NULL && m_pTexInfo->IsShownOnScreen())
    {
        wxPropertyGridPage* pg = m_pTexInfo->GetPage(wxT("ЗТИ"));
        pg->FitColumns();
    }
    wxFrame::OnSize(event);
}

void PKVIFrame::OnChangeOrigin(wxCommandEvent& event)
{
//	wxRect* r=(wxRect*)event.GetClientData();
//	wxRect r1((-r->x),(-r->y),r->width,r->height);
//	m_SmallScreen->SetSelectRect(r1);
}

void PKVIFrame::OnSettings(wxCommandEvent& event)
{
    if(m_Can.IsOpen())
    {
        m_Can.Close();
    }
    SettingsDlg dlg(this);
    dlg.m_sDevice=m_sDevice;
    dlg.m_saInterfaces=m_saInterfaces;
    dlg.m_saIP=m_saIP;
//	dlg.m_bBOOTP=m_bBOOTP;
    dlg.m_nComPort=m_nCommPort;
    dlg.m_nCanCameraNum=m_nCanCameraNum;
    dlg.m_nCanInterface=m_nCanInterface;
    dlg.m_nCanType=m_nCanType;
    dlg.m_nCanSpeed=m_nCanSpeed;
    dlg.m_bContrast=m_bContrast;
    if(m_bContrast)
    {
        m_BigScreen->SetContrast(1);
        m_SmallScreen->SetContrast(1);
        m_BigScreen->SetBright(0);
        m_SmallScreen->SetBright(0);
    }
    else
    {
        m_BigScreen->SetContrast(m_pStatusContrast->GetValue());
        m_SmallScreen->SetContrast(m_pStatusContrast->GetValue());
        m_BigScreen->SetBright(m_pStatusBright->GetValue());
        m_SmallScreen->SetBright(m_pStatusBright->GetValue());
    }

    CopyCameraInfo(m_CamInfoArr,dlg.m_CamInfoArr);
    dlg.m_nCamNum=m_nCamNum;
    if(dlg.ShowModal()==wxID_OK)
    {
        m_sDevice=dlg.m_sDevice;
        GetOurIP();
//		m_bBOOTP=dlg.m_bBOOTP;
        m_nCanCameraNum=dlg.m_nCanCameraNum;
        m_nCanInterface=dlg.m_nCanInterface;
        m_nCanType=dlg.m_nCanType;
        m_nCanSpeed=dlg.m_nCanSpeed;
        m_bContrast=dlg.m_bContrast;
        if(m_nCommPort!=dlg.m_nComPort)
            ChangeComPort(dlg.m_nComPort);
        if(m_nCanType>0 && m_nCanInterface>0 && m_bPult)
        {
            if(!m_Can.Open(m_nCanType,m_nCanInterface,m_nCanSpeed))
            {
                m_nCanInterface=0;
                m_nCanType=0;
                if(m_bPult && m_bVoik==0)
                    m_StatusBar->SetStatusText("CAN не подключен",5);
            }
        }
        else if(m_bPult && m_bVoik==0)
            m_StatusBar->SetStatusText("CAN не подключен",5);

        CopyCameraInfo(dlg.m_CamInfoArr,m_CamInfoArr);
        m_nCamNum=dlg.m_nCamNum;
        if(m_nCamNum<m_CamInfoArr.GetCount())
            m_pCamInfo=m_CamInfoArr[m_nCamNum];
        else
        {
            m_nCamNum=0xffff;
            m_pCamInfo=NULL;
        }

        m_ToolBar->EnableTool(idMenuStart, !m_sDevice.IsEmpty() && m_pCamInfo!=NULL);

        if (!BUILDROOT)
        {
            wxConfig config(_("PKVI"));
            config.Write(_("/Settings/DeviceName"),dlg.m_sDevice);
            config.Write(_("/Settings/COM_Port"),dlg.m_nComPort);
//		    config.Write(_("/Settings/BOOTP"),dlg.m_bBOOTP);
            config.Write(_("/Settings/CanType"),dlg.m_nCanType);
            config.Write(_("/Settings/CanInterface"),dlg.m_nCanInterface);
            config.Write(_("/Settings/CanSpeed"),dlg.m_nCanSpeed);
            config.Write(_("/Settings/CanCameraNum"),dlg.m_nCanCameraNum);
            config.Write(_("/Settings/Contrast"),dlg.m_bContrast);

            int i=0;
            wxString sName;
            wxString sCameraNum=wxString::Format("/CameraNames/Camera%d",i);
            while(config.Read(sCameraNum,&sName))
            {
                config.DeleteGroup(sName);
                sCameraNum.Printf("/CameraNames/Camera%d",++i);
            }
            config.DeleteGroup("/CameraNames");

            for(i=0; i<m_CamInfoArr.GetCount(); i++)
            {
                CAMERA_INFO* pCamInfo=m_CamInfoArr[i];
                sCameraNum.Printf("/CameraNames/Camera%d",i);
                config.Write(sCameraNum,pCamInfo->sName);
                config.Write("/"+pCamInfo->sName+"/GroupIP",pCamInfo->sGroupIP);
                config.Write("/"+pCamInfo->sName+"/IP",pCamInfo->sIP);
                config.Write("/"+pCamInfo->sName+"/Port",pCamInfo->nPort);
                config.Write("/"+pCamInfo->sName+"/GroupPort",pCamInfo->nGroupPort);
            }
            config.Write("/Settings/CameraNum",m_nCamNum);
            config.Flush();
        }
    }

}
void PKVIFrame::OnStart(wxCommandEvent& event)
{

    if(m_bInit)
    {
        m_mutexPKVI->Lock();
        m_bInit=false;
        m_mutexPKVI->Unlock();
        m_connectType=connect_no;
        ChangeConnect();
        return;
    }

    if(!GetThread()->IsRunning())
    {
        GetThread()->SetPriority(80);
        GetThread()->Run();
    }

    m_bContrast=1;      //сначала убрать все фильтры
    m_bVertFilter=0;
    m_nVertOffset=1;

    m_nCadrCount=0;
    m_nRecCadrCount=0;
    FillTexInfo(false);
    FillWorkInfo();
    if(m_bMulticast)
        m_connectType=connect_multi;
    else
        m_connectType=connect_tex;
    ChangeConnect();
    m_TimeCadr=0;
    m_TimeCadr0=0;
    m_TimeCadrN=0;

    m_mutexPKVI->Lock();
    m_bInit=true;
    m_mutexPKVI->Unlock();
}


u_int PKVIFrame::CalculateCRC(u_char* pData,int nLength)
{
    u_int nCRC=0;

    for(int i=0; i<nLength; i+=2)
    {
        if(i+1<nLength)
//			nCRC+=MAKEWORD(pData[i+1],pData[i]);
            nCRC+=pData[i+1]|(((u_short)pData[i])<<8);
        else
//			nCRC+=MAKEWORD(0,pData[i]);
            nCRC+=((u_short)pData[i])<<8;
        if(nCRC&0xf0000)
            nCRC=(nCRC&0xffff)+((nCRC&0xf0000)>>16);
    }

    return nCRC;
}


void PKVIFrame::ShutdownSocket(int* pSocket, bool bMulticast, ip_mreq* pm_addr)
{
    if(*pSocket)
    {
        if(bMulticast)
            setsockopt(*pSocket,IPPROTO_IP,IP_DROP_MEMBERSHIP,(char*)pm_addr,sizeof(ip_mreq));
        shutdown(*pSocket,SHUT_RDWR);
        close(*pSocket);
        *pSocket=0;

    }
}

void* PKVIFrame::Entry()
{
    wxSize szCadrSize;
    szCadrSize.x=1024;
    szCadrSize.y=1;
    u_int32_t nDataSize=1024;
    u_char* pData=new u_char[nDataSize*2];
    u_char* pCommand=NULL;
    sockaddr_in serv_addr;
    ip_mreq m_addr;
    ssize_t nReaded=0;
    u_int32_t nCaptured=0;
    u_int32_t nMustRead=0;
    bool bStartCadr=false;
    bool bStartInfo=false;
    fd_set readfds,testfds;
    timeval timeout;
    int nfds=0;
    FD_ZERO(&readfds);
    int sockfd=0;
    CAMERA_INFO CamInfo;
    int yes=1;
    linger l;
    l.l_onoff=1;
    l.l_linger=1;
    bool bMulticast=false;
    char pSendCommand[1024];
    bool bFirstPacket=false;
    wxStopWatch watch;
    wxLog::AddTraceMask("frm");
    do
    {
        if(!IsInit())
        {
            ShutdownSocket(&sockfd,bMulticast,&m_addr);
            continue;
        }

        if (m_bVoik!=0)
        {
            VoikFilmProc();
            continue;
        }

        bool bMulticastNew;
        if(IsReconnect(&CamInfo,&bMulticastNew))
        {
            ShutdownSocket(&sockfd,bMulticast,&m_addr);
            bMulticast=bMulticastNew;
            bFirstPacket=false;
        }

//		if(GetCameraInfo(&CamInfo))
//			ShutdownSocket(&sockfd,bMulticast,&m_addr);
//		if(bMulticast!=IsMulticast())
//		{
//			ShutdownSocket(&sockfd,bMulticast,&m_addr);
//			bMulticast=!bMulticast;
//		}

        if(sockfd==0)
        {
            nfds=0;
            FD_ZERO(&readfds);              //очищает множество readfds  (неблокирующих сокетов )
            nCaptured=0;
///*UAK???
            if (nDataSize<1024)
            {
                szCadrSize.x=1024;
                szCadrSize.y=1;
                nDataSize=1024;
                if(pData)
                    delete [] pData;
                pData=new BYTE[nDataSize*2];
            }
//*/
            bStartCadr=false;
            bStartInfo=false;

            serv_addr.sin_family=AF_INET;
            if(!bMulticast)
            {
                if(!bFirstPacket)
                {
                    watch.Start();
                    bFirstPacket=true;
                }
                sockfd=socket(AF_INET,SOCK_STREAM,0);
                setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,1);                                  //Allow local address reuse.
                setsockopt(sockfd,SOL_SOCKET,SO_LINGER,&l,sizeof(linger));                          //Linger on close if data present
                inet_aton(CamInfo.sIP.c_str(),&serv_addr.sin_addr);
                serv_addr.sin_port=htons(CamInfo.nPort);
                int res=connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
                if(res<0 || sockfd<=0)
                {
                    ShutdownSocket(&sockfd,bMulticast,&m_addr);
                    if(watch.Time()>5000)
                    {
                        m_mutexPKVI->Lock();
                        if(m_connectType==connect_auto)
                        {
                            m_bMulticast=true;
                            wxCommandEvent* ev=new wxCommandEvent(wxEVT_CHANGE_CONNECT_STATUS,GetId());
                            ev->SetClientData((long*)(status_try_multi));
                            wxQueueEvent(this,ev);
                        }
                        m_mutexPKVI->Unlock();
                    }
                    continue;
                }
            }
            else
            {
                sockfd=socket(AF_INET,SOCK_DGRAM,0);
                setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,1);
                setsockopt(sockfd,SOL_SOCKET,SO_LINGER,&l,sizeof(linger));
                u_char ttl=1;
                int res=setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl));       //Указать TTL для групповых сообщений.Устанавливает TTL (Time To Live)
                inet_aton(CamInfo.sGroupIP.c_str(),&serv_addr.sin_addr);
                serv_addr.sin_port=htons(u_short(CamInfo.nGroupPort));
                res=bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
                inet_aton(m_sOurIP.c_str(),(in_addr*)&m_addr.imr_interface.s_addr);
                inet_aton(CamInfo.sGroupIP.c_str(),(in_addr*)&m_addr.imr_multiaddr.s_addr);
                res=setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&m_addr,sizeof(m_addr));      //Присоединиться к multicast-группе.
                if(res<0 || sockfd<=0)
                {
                    ShutdownSocket(&sockfd,bMulticast,&m_addr);
                    continue;
                }
                bFirstPacket=true;
            }
            FD_SET(sockfd,&readfds);                    //добавляет дескриптор fd в множество readfds
            nfds=max(sockfd,nfds);
        }
        if(sockfd==0)
            continue;
        if(!bMulticast)
        {
            pCommand=GetCommand();
            if(pCommand)
            {
                memcpy(pSendCommand,pCommand,1024);
                int nSend=send(sockfd,pSendCommand,1024,MSG_NOSIGNAL);
                delete[](pCommand);
            }
        }
        timeout.tv_sec=0;
        timeout.tv_usec=100000;
        testfds=readfds;
        // Ждём события в одном из сокетов
        int nResult=select(nfds+1,&testfds,0,0,&timeout);           //Если хотя бы один сокет готов к выполнению заданной операции, select возвращает ненулевое значение
        if(nResult==0)                                              //сработал таймаут
        {
            if(bMulticast && watch.Time()>5000 && !bFirstPacket)
            {
                wxCommandEvent* ev=new wxCommandEvent(wxEVT_CHANGE_CONNECT_STATUS,GetId());
                ev->SetClientData((long*)status_try_multi);
                wxQueueEvent(this,ev);
                bFirstPacket=true;
            }
            continue;
        }
        else if(nResult==-1)                                        //ошибкa -1
        {
            if(errno == EINTR)
                continue;
            else
            {
                SetExit();
                break;
            }
        }

        if(FD_ISSET(sockfd,&testfds))
        {
            if(bFirstPacket)
            {
                bFirstPacket=false;
                wxCommandEvent* ev=new wxCommandEvent(wxEVT_CHANGE_CONNECT_STATUS,GetId());
                ev->SetClientData((long*)(bMulticast?status_connect_multi:status_connect_tex));
                wxQueueEvent(this,ev);
            }
            if(bMulticast)
            {
                nMustRead=1024;
                nReaded=recv(sockfd,(char*)(pData+nCaptured),nMustRead,0);
            }
            else
            {
                nMustRead=!bStartCadr && !bStartInfo?1024:nDataSize-nCaptured;
                nReaded=read(sockfd,pData+nCaptured,nMustRead);
            }

            if(nReaded>0)
            {
                if(bMulticast)
                {
                    watch.Start();
                    if(memcmp(pData+nCaptured,"FRM_DATA",8)==0)
                    {
                        TEX_INFO* pInfo=(TEX_INFO*)(pData+nCaptured+16);
                        bool bLIK=pInfo->btCamType==2;
                        szCadrSize.y=bLIK?int((pInfo->wSpeedTransfer/256.0)*1000/(13*15)):pInfo->wSizeY;
                        if(szCadrSize.y==0)
                            szCadrSize.y=1;
                        szCadrSize.x=pInfo->wSizeX;
                        int nDataSizeNew=bLIK?(szCadrSize.x*2+1024)*szCadrSize.y:szCadrSize.y*szCadrSize.x*2+1024;
                        if(nDataSize!=nDataSizeNew || bLIK!=m_bLIK)
                        {
                            nDataSize=nDataSizeNew;
                            SetCadrSize(szCadrSize,bLIK);
                            delete [] pData;
                            pData=new BYTE[nDataSize*2];
                            nCaptured=0;
                        }
                        else if(nCaptured>=nDataSize)
                        {
                            if((m_bLIK || nCaptured==nDataSize) && memcmp(pData,"FRM_DATA",8)==0)
                                AddPaintData(pData,nDataSize);
                            memcpy(pData,pData+nCaptured,nReaded);
                            nCaptured=0;
                        }
                    }
                    else if(memcmp(pData+nCaptured,"INF_DATA",8)==0)
                    {
                        continue;
                    }
                    nCaptured+=nReaded;
                    if(nCaptured>=nDataSize*2)
                        nCaptured=0;
                    continue;
                }
                else if(nCaptured==0 && memcmp(pData,"FRM_DATA",8)==0)
                {
                    TEX_INFO* pInfo=(TEX_INFO*)(pData+16);
                    bool bLIK=pInfo->btCamType==2;
                    szCadrSize.y=bLIK?int((pInfo->wSpeedTransfer/256.0)*1000/(13*15)):pInfo->wSizeY;
                    if(szCadrSize.y==0)
                        szCadrSize.y=1;
                    szCadrSize.x=pInfo->wSizeX;
                    int nDataSizeNew=bLIK?(szCadrSize.x*2+1024)*szCadrSize.y:szCadrSize.y*szCadrSize.x*2+1024;
                    if(nDataSize!=nDataSizeNew || bLIK!=m_bLIK)
                    {
                        nDataSize=nDataSizeNew;
                        SetCadrSize(szCadrSize,bLIK);
                        BYTE* pTemp=new BYTE[nReaded];
                        memcpy(pTemp,pData,nReaded);
                        delete [] pData;
                        pData=new BYTE[nDataSize*2];
                        memcpy(pData,pTemp,nReaded);
                        delete [] pTemp;
                    }
                    bStartCadr=true;
                }
                else if(!bMulticast && nCaptured==0 && memcmp(pData,"INF_DATA",8)==0)
                {
                    bStartInfo=true;
                }
                if(bStartCadr)
                {
                    if(m_bLIK)
                    {
                        int nSize=nReaded/8;
                        if(lfind("INF_DATA",pData+nCaptured,(UINT*)&nSize,8,Compare))
                            nDataSize+=1024;
                    }
                    nCaptured+=nReaded;
                    if(nCaptured>=nDataSize)
                    {
                        if(nCaptured==nDataSize)
                            AddPaintData(pData,nDataSize);
                        nCaptured=0;
                        bStartCadr=false;
                    }

                }
                else if(bStartInfo && !bMulticast)
                {
                    nCaptured+=nReaded;
                    if(nCaptured>=1024)
                    {
                        AddInfoData(pData+16);
                        nCaptured=0;
                        bStartInfo=false;
                    }
                }
            }
            else
            {
                ShutdownSocket(&sockfd,bMulticast,&m_addr);
                wxCommandEvent* ev=new wxCommandEvent(wxEVT_CHANGE_CONNECT_STATUS,GetId());
                m_mutexPKVI->Lock();
                if(m_connectType==connect_auto)
                {
                    m_bMulticast=true;
                    ev->SetClientData((long*)(status_try_multi));
                }
                else
                    ev->SetClientData((long*)(status_try_tex));
                m_mutexPKVI->Unlock();
                wxQueueEvent(this,ev);
                continue;
            }
        }
    }
    while(!IsExit());
    if(pData)
        delete [] pData;
    ShutdownSocket(&sockfd,bMulticast,&m_addr);
    return NULL;
}



int PKVIFrame::GetSvrAddress(const char* pIface, u_char* pMac)
{
//	int nSD; // Socket descriptor
//	struct ifreq sIfReq; // Interface request
//	struct if_nameindex* pIfList; // Ptr to interface name index
//	struct if_nameindex* pListSave; // Ptr to interface name index
//
////
//// Initialize this function
////
//	pIfList = (struct if_nameindex *)NULL;
//	pListSave = (struct if_nameindex *)NULL;
//#ifndef SIOCGIFADDR
//// The kernel does not support the required ioctls
//	return(0);
//#endif
//
////
//// Create a socket that we can use for all of our ioctls
////
//	nSD = socket(PF_INET, SOCK_STREAM, 0);
//	if(nSD < 0)
//	{
//// Socket creation failed, this is a fatal error
//		wxString sErr;
//		sErr.Format(wxT("File %s: line %d: Socket failed\n"), __FILE__, __LINE__);
//		wxMessageBox(sErr,wxT("Ошибка"),wxOK|wxICON_ERROR);
//		return(0);
//	}
//
////
//// Obtain a list of dynamically allocated structures
////
////	pIfList = pListSave = if_nameindex();
//
////
//// Walk thru the array returned and query for each interface's
//// address
////
//	for(pIfList=pListSave = if_nameindex(); *(char *)pIfList != 0; pIfList++)
//	{
////
//// Determine if we are processing the interface that we
//// are interested in
////
//		if(strcmp(pIfList->if_name, pIface))
//// Nope, check the next one in the list
//			continue;
//		strncpy(sIfReq.ifr_name, pIfList->if_name, IF_NAMESIZE);
//
////
//// Get the MAC address for this interface
////
//		if(ioctl(nSD, SIOCGIFHWADDR, &sIfReq) != 0)
//		{
//// We failed to get the MAC address for the interface
//			printf("File %s: line %d: Ioctl failed\n", __FILE__, __LINE__);
//			return(0);
//		}
////		memmove((void *)&m_pOurMacAddress1[0], (void *)&sIfReq.ifr_ifru.ifru_hwaddr.sa_data[0], 6);
//		memmove((void *)pMac, (void *)&sIfReq.ifr_ifru.ifru_hwaddr.sa_data[0], 6);
//		break;
//	}
//
////
//// Clean up things and return
////
//	if_freenameindex(pListSave);
//	close(nSD);
    return(1);
}

void PKVIFrame::SetExit()
{
    wxMutexLocker lock(*m_mutexPKVI);
    m_bExit=true;
    m_bInit=false;
}

bool PKVIFrame::IsExit()
{
    wxMutexLocker lock(*m_mutexPKVI);
    return m_bExit;
}

bool PKVIFrame::IsInit()
{
    wxMutexLocker lock(*m_mutexPKVI);
    return m_bInit;
}

bool PKVIFrame::AddPaintData(u_char* pData, u_int32_t dwSize)
{
    wxMutexLocker lock(*m_mutexPKVI);
    if(pData==NULL || dwSize==0)
    {
        ListData::Node* node=NULL;
        do
        {
            node=m_listData.GetFirst();
            if(node)
            {
                BYTE* pTemp=(node->GetData());
                m_listData.DeleteNode(node);
                if(pTemp)
                    delete [] pTemp;
            }
        }
        while(node);
        return false;
    }
    if(m_listData.GetCount()<10)
    {
        BYTE* pAddedData=new BYTE[dwSize];
        memcpy(pAddedData,pData,dwSize);
        m_listData.Append(pAddedData);
    }
    else
    {
        int a=m_listData.GetCount();
        int b=0;
    }
    wxCommandEvent* ev=new wxCommandEvent(wxEVT_DATA_READY,GetId());
    ev->SetExtraLong(dwSize);
    wxQueueEvent(this,ev);

    printProtCam(m_nCamNum,pData);

    if(!m_bLIK)
        m_nRecCadrCount++;
    else
        m_nRecCadrCount+=dwSize/(m_szCadrSize.x*2+1024);
    return true;
}

bool PKVIFrame::GetPaintData()
{
    wxMutexLocker lock(*m_mutexPKVI);
    ListData::Node* node=m_listData.GetFirst();
    if(node)
    {
        m_pData=(node->GetData());
        m_listData.DeleteNode(node);
        return true;
    }
    return false;
}

void PKVIFrame::OnIdle(wxIdleEvent& event)
{
    event.Skip();
}

void PKVIFrame::OnTimerCount(wxTimerEvent& event)
{
    wxString s;
    int nRecCadrs=GetRecCadrCount();
    s.Printf(_("Принимается: %d к/с; отображается: %d к/с."),nRecCadrs,m_nCadrCount);
    if(m_bStatusBar && m_bPult)       //UAK
    {
        m_StatusBar->SetStatusText(s,6);
//        if (m_bInit && !m_pStatusBmp)
//        {
//            m_StatusBar->SetStatusText(wxString::Format("%s",m_bMulticast?"Multicast":"Tex Connect"),4);
//        }
    };
    m_nCadrCount=0;
    SetRecCadrCount(0);
    if(m_bPult && m_bStatusBar && m_Can.IsOpen())
    {
        MY_CAN_MSG msg;
//КДПО
//		memset(&msg,0,sizeof(msg));
//		msg.id=((0x3<<7)|0x4);
//		msg.length=8;
//		write(m_hCan,&msg,1);
//
        memset(&msg,0,sizeof(msg));
        msg.id=((4+m_nCanCameraNum)<<7);
        msg.id|=0x3;
        msg.length=0;
        msg.flags=MSG_RTR;
        bool bRet=false;
        while(!bRet)
            bRet=m_Can.SendMsg(&msg);

        wxStopWatch sw;
        memset(&msg,0,sizeof(msg));
//		int nReaded=0, nNeed=sizeof(msg);
        static int nCountReopen=0;
        bRet=false;
        do
        {
            bRet=m_Can.RcvMsg(&msg);
        }
        while(sw.Time()<=20 && !bRet);
        if(!bRet)
        {
            m_StatusBar->SetStatusText("CAN не подключен",5);
            if(nCountReopen>3)
            {
                m_Can.Close();
                if(!m_Can.Open(m_nCanType,m_nCanInterface,m_nCanSpeed))
                {
                    m_nCanInterface=0;
                    m_nCanType=0;
                    m_StatusBar->SetStatusText("CAN не подключен",5);
                }
            }
            nCountReopen++;
            if(nCountReopen>4)
                nCountReopen=0;
        }
        else
        {
            nCountReopen=0;
            int nAddress=(msg.id>>7)&0xf;
            int nCod=msg.id&0x3f;
            int nR=(msg.id>>6)&0x01;
            if(nAddress==m_nCanCameraNum+4 && nR!=0 && nCod==0x3 && msg.length==4) //Статус
            {
                bool bGood=(msg.data[0]&0xf2)==0xf2 && (msg.data[1]&0x0f)==0;
                bool bReady=(msg.data[0]&0xf3)==0xf3 && (msg.data[1]&0x3f)==0x30 && (msg.data[2]&0x0a)==0x2;
                m_StatusBar->SetStatusText(wxString::Format("%s %s",bGood?"Исправность":"Неисправность",bReady?"Готовность":"Неготовность"),5);
            }
        }

    }
}

void PKVIFrame::OnTimerSum(wxTimerEvent& event)
{
//Debug
//	BYTE* pAddedData=new BYTE[m_szCadrSize.x*m_szCadrSize.y*2];
//	memset(pAddedData,0xf,m_szCadrSize.x*m_szCadrSize.y*2);
//	m_listData.Append(pAddedData);
//	wxQueueEvent(this,new wxCommandEvent(wxEVT_DATA_READY,GetId()));
//End Debug

    if(m_bSum && m_pProgressDlg && m_pProgressDlg->WasCancelled())
    {
        m_pProgressDlg->Destroy();
        m_pProgressDlg=NULL;
        m_bSum=false;
        if(m_pSummData)
        {
            delete [] m_pSummData;
            m_pSummData=NULL;
        }
        m_timerSum.Stop();
    }

}

void PKVIFrame::OnTimerMinutes(wxTimerEvent& event)
{
    m_ulMinutes++;
    if (!BUILDROOT)
    {
        m_fMinutes.Seek(0);
        m_fMinutes.Write(&m_ulMinutes,sizeof(u_long));
        m_fMinutes.Flush();
    }
    if(m_bStatusBar)
    {
        wxString s;
        s=m_bSave?"SAVE! ":"";
        m_StatusBar->SetStatusText(wxString::Format("%sВремя наработки (ч:м): %02d:%02d",s,m_ulMinutes/60,m_ulMinutes%60),m_bPult?7:4);
    }
}

void PKVIFrame::ChangeMarkerParams(MARKER* pMarker)
{
    if(m_comm)
    {
        u_char pData[10];
        pData[0]=0xf0;
        pData[1]=LOBYTE(7);
        pData[2]=HIBYTE(7);

        pData[3]=LOBYTE(u_short(pMarker->Coord.x));
        pData[4]=HIBYTE(u_short(pMarker->Coord.x));
        pData[5]=LOBYTE(u_short(pMarker->Coord.y));
        pData[6]=HIBYTE(u_short(pMarker->Coord.y));

        pData[7]=pMarker->bVisible?1:0;

        pData[8]=pMarker->nType;

        pData[9]=pMarker->nSize;

//		int nTry=0;
//		do
//		{
        write(m_comm,pData,10);
//			u_short usACK=0;
//			wxMilliSleep(20);
//			int nReaded=read(m_comm,&usACK,2);
//			if(nReaded==2 && usACK==0xFFFF)
//				break;
//		}
//		while(++nTry<3);
    }
    if(m_bLPTEnable)
    {
        if(pMarker->bVisible && (m_ucStatus&0x2)==0x2)
        {
            m_ucStatus&=0xfd;
            //outb(m_ucStatus,BASEPORT);       //UAK
            ioctl(flpt,PPWDATA,&m_ucStatus);

        }
        else if(!pMarker->bVisible && (m_ucStatus&0x2)==0x0)
        {
            m_ucStatus|=0x2;
            //outb(m_ucStatus,BASEPORT);
            ioctl(flpt,PPWDATA,&m_ucStatus);
        }
    }
    if (!BUILDROOT)
    {
        wxConfig config("PKVI");
        config.Write("/Settings/MarkerSize",pMarker->nSize);
    }
//    FillWorkInfo();
}

void PKVIFrame::OnTimerUU(wxTimerEvent& event)
{
    if(m_bInit)
    {
        FillTexInfo(false);
    }
    FillWorkInfo();
    if(m_comm)
    {
        u_char pData[3];
        u_char* pDataEx=NULL;
        int nReaded=0;
        nReaded=read(m_comm,pData,3);
        if(nReaded==3)
        {
            short nNeedRead=MAKEWORD(pData[1],pData[2]);
            if(nNeedRead>0)
                pDataEx=new u_char[nNeedRead];
            nReaded=0;
            wxStopWatch sw;
            do
            {
                nReaded+=read(m_comm,pDataEx+nReaded,nNeedRead);

            }
            while(nReaded<nNeedRead && sw.Time()<=100);
            if(nReaded>=nNeedRead)
            {
                switch(pData[0])
                {
                case 0x1:	//Source
                    if(m_bUU)
                    {
                        if(pDataEx[0]==0)
                        {
                            m_ucStatus&=0xf7;
                            m_ucStatus|=0x10;
                            m_ucStatus|=0x20;
                        }
                        else if(pDataEx[0]==1)
                        {
                            m_ucStatus&=0xef;
                            m_ucStatus|=0x20;
                            m_ucStatus|=0x8;
                        }
                        else
                        {
                            m_ucStatus&=0xdf;
                            m_ucStatus|=0x8;
                            m_ucStatus|=0x10;
                        }
                        SetSource(pDataEx[0]);
                        //outb(m_ucStatus,BASEPORT);            //UAK
                        ioctl(flpt,PPWDATA,&m_ucStatus);
                    }
                    break;
                case 0x2:	//Mashtab
                    if(m_bUU && m_BigScreen)
                    {
                        m_BigScreen->SetMashtab(pDataEx[0]);
                    }
                    break;
                case 0x3:	//Offset
                    if(m_bUU)
                    {
                        wxPoint p(MAKEWORD(pDataEx[0],pDataEx[1]),MAKEWORD(pDataEx[2],pDataEx[3]));
                        m_BigScreen->SetOrigin(p.x,p.y);
                    }
                    break;
                case 0x4:	//Marker On/Off
                    if(m_bUU)
                    {
                        if(m_bLPTEnable)
                        {
                            if(pDataEx[0]==1)
                                m_ucStatus&=0xfd;
                            else
                                m_ucStatus|=0x2;

                            //outb(m_ucStatus,BASEPORT);            //UAk
                            ioctl(flpt,PPWDATA,&m_ucStatus);
                        }
                        if(m_BigScreen)
                        {
                            m_BigScreen->m_marker.bVisible=pDataEx[0];
                            m_BigScreen->Refresh();
                        }
                        FillWorkInfo();
                    }
                    break;
                case 0x5:	//Marker type
                    if(m_bUU && m_BigScreen)
                    {
                        m_BigScreen->m_marker.nType=(marker_type)pDataEx[0];
                        m_BigScreen->Refresh();
                        FillWorkInfo();
                    }
                    break;
                case 0xd:	//Marker coordinate
                    if(m_bUU && m_BigScreen)
                    {
                        m_BigScreen->m_marker.Coord=wxPoint(MAKEWORD(pDataEx[0],pDataEx[1]),MAKEWORD(pDataEx[2],pDataEx[3]));
                        m_BigScreen->Refresh();
                        FillWorkInfo();
                    }
                    break;
                case 0x6:	//Marker size
                    if(m_bUU && m_BigScreen)
                    {
                        m_BigScreen->m_marker.nSize=(int)pDataEx[0];
                        m_BigScreen->Refresh();
                        FillWorkInfo();
                    }

                    break;
                case 0x7:
                    break;
                case 0x8:	//Служебная информация
                    if(m_bUU)
                    {
                        m_sWorkText=(char*)(pDataEx);
                        FillWorkInfo();
                    }
                    break;
                case 0x9:	//Порог
                    if(m_bUU)
                    {
                        m_nLevel=MAKEWORD(pDataEx[0],pDataEx[1]);
//						m_TexInfo->SetItem(13,1,wxString::Format("%d",m_nLevel));
                    }

                    break;
                case 0xa:	//Динамический диапазон
                    if(m_bUU)
                    {
                        m_nDyn=MAKEWORD(pDataEx[0],pDataEx[1]);
//						m_TexInfo->SetItem(18,1,wxString::Format("%d",m_nDyn));
                    }
                    break;
                case 0xb:	//UU ore RU
                    m_bUU=pDataEx[0]==1;
                    if(m_bLPTEnable)
                    {
                        if(m_bUU)
                        {
                            m_ucStatus|=0x80;
                            m_ucStatus&=0xbf;
                        }
                        else
                        {
                            m_ucStatus&=0x7F;
                            m_ucStatus|=0x40;
                        }
                        //outb(m_ucStatus,BASEPORT);            //UAk
                        ioctl(flpt,PPWDATA,&m_ucStatus);
                    }
                    break;
                case 0xc:	//Размер кадра
//					if(m_bUU)
                {
                    wxSize sz(MAKEWORD(pDataEx[0],pDataEx[1]),MAKEWORD(pDataEx[2],pDataEx[3]));
//						SetCadrSize(sz);
                }
                break;
                case 0xe:	//Сбой
                    if(m_bLPTEnable)
                    {
                        if(pDataEx[0]==1)
                            m_ucStatus&=0xfe;
                        else
                            m_ucStatus|=0x1;

                        //outb(m_ucStatus,BASEPORT);            //UAK
                        ioctl(flpt,PPWDATA,&m_ucStatus);
                    }
                    m_bSboy=pDataEx[0]==1;
                    break;
                case 0xf:	//Отображение технологической информации
                    break;
                case 0x10:	//Включение/выключение ждущего режима
                {
                    if(pDataEx[0]==0)
                    {
                        wxString s=wxStandardPaths::Get().GetUserConfigDir()+"/Sleep.sh";
                        int nRet=system(s.c_str());
                    }
                    else
                    {
                        wxString s=wxStandardPaths::Get().GetUserConfigDir()+"/Unsleep.sh";
                        int nRet=system(s.c_str());
                    }
                }
                break;

                }
//				tcflush(m_comm,TCIOFLUSH);
//				u_short usACK=0xFFFF;
//				write(m_comm,&usACK,2);
            }

        }
//		tcflush(m_comm,TCIOFLUSH);
        if(pDataEx)
            delete pDataEx;
    }
//	if(!m_bUU)
    {
        static bool bPressed=false;
        if(m_bLPTEnable)
        {
            u_char ucSwitch;            //=inb(BASEPORT+1);         //UAK
            ioctl(flpt,PPRSTATUS,&ucSwitch);
            if(ucSwitch!=m_ucSwitch && !bPressed)
            {
                if((ucSwitch&0x20)!=(m_ucSwitch&0x20))	//UU-RU
                {
                    if((m_ucStatus&0x80)==0x0)	//RU
                    {
                        m_ucStatus|=0x80;
                        m_ucStatus&=0xbf;
                        m_bUU=true;
                    }
                    else						//UU
                    {
                        m_ucStatus&=0x7F;
                        m_ucStatus|=0x40;
                        m_bUU=false;
                    }
                    //outb(m_ucStatus,BASEPORT);            //UAK
                    ioctl(flpt,PPWDATA,&m_ucStatus);
                    ChangeStatus(m_bUU, m_bSboy);
                }
                if((ucSwitch&0x80)!=(m_ucSwitch&0x80))	//VGA-CTK1-CTK2
                {
                    if((m_ucStatus&0x20)==0x0)	//CTK2
                    {
                        m_ucStatus&=0xf7;
                        m_ucStatus|=0x10;
                        m_ucStatus|=0x20;
                        SetSource(0);
                    }
                    else if((m_ucStatus&0x10)==0x0)	//CTK1
                    {
                        m_ucStatus&=0xdf;
                        m_ucStatus|=0x8;
                        m_ucStatus|=0x10;
                        SetSource(2);
                    }
                    else							//VGA
                    {
                        m_ucStatus&=0xef;
                        m_ucStatus|=0x20;
                        m_ucStatus|=0x8;
                        SetSource(1);
                    }
                    //outb(m_ucStatus,BASEPORT);            //UAK
                    ioctl(flpt,PPWDATA,&m_ucStatus);
                }
                bPressed=true;
            }
            else if(bPressed && ucSwitch==m_ucSwitch)
                bPressed=false;
        }
    }
}

void PKVIFrame::OnInfoReady(wxCommandEvent& event)
{
    wxMutexLocker lock(*m_mutexPKVI);
    wxPropertySheetDialog* dlg=(wxPropertySheetDialog*)wxWindow::FindWindowById(ID_COMMAND_DLG,NULL);
    if(dlg)
    {
        NetworkPanel* pPanel=(NetworkPanel*)dlg->GetBookCtrl()->GetPage(0);
        pPanel->SetInfo(&m_InfoData);
    }
}

void PKVIFrame::OnCalcReady(wxCommandEvent& event)
{
    wxString msg=wxString::Format("Минимальное значение Lmin: %d\nМаксимальное значение Lmax: %d\nДевиация амплитуды Dl: %d\nНеравномерность чувствительности D: %f%%\n\nСреднее значение: %d",
                                  m_CalcInfo.wMin,m_CalcInfo.wMax,(u_int)m_CalcInfo.ullDev,m_CalcInfo.fD,(u_int)m_CalcInfo.ullMid);

    wxMessageDialog dlg(this,msg,"Результаты вычисления",wxOK|wxCENTRE|wxICON_INFORMATION);
    dlg.ShowModal();

}

void PKVIFrame::OnDataReady(wxCommandEvent& event)
{
    wxMutexLocker lock(*m_mutexPKVI);
//Debug
    if(m_bInit)
//End Debug
    {
        while(GetPaintData())
        {
            if(m_pData==NULL || memcmp(m_pData,"FRM_DATA",8)<0)
            {
                if(m_pData)
                {
                    delete [] m_pData;
                    m_pData=NULL;
                }
                continue;
            }
            m_swCadr.Start();
            memcpy(&m_TexInfoStruct,m_pData+16,sizeof(TEX_INFO));
////Debug
            /*
                        m_TexInfoStruct.wConvolutionsCount=5;
                        static int nXConv=0;
                        static int nYConv=0;
            */
////
            if(m_TexInfoStruct.wConvolutionsCount!=m_nConvolutionCount)
            {
                if(m_pConvolution)
                {
                    delete [] m_pConvolution;
                    m_pConvolution=NULL;
                }
                m_nConvolutionCount=m_TexInfoStruct.wConvolutionsCount;
                if(m_nConvolutionCount>0)
                {
                    m_pConvolution=new CONVOLUTION[m_nConvolutionCount];
                }
            }
            if(m_nConvolutionCount>0 && m_pConvolution!=NULL)
            {
                memcpy(m_pConvolution,m_pData+256,m_nConvolutionCount*sizeof(CONVOLUTION));        //256=240+16
                //memcpy(m_pConvolution,&m_TexInfoStruct+240,m_nConvolutionCount*sizeof(CONVOLUTION));        //256=240+16
            }
////Debug
            /*
                        for(int i=0; i<5; i++)
                        {
                            m_pConvolution[i].nX=nXConv+i*10;
                            if(m_pConvolution[i].nX>=m_szCadrSize.x)
                                nXConv=0;
                            m_pConvolution[i].nY=nYConv+i*5;
                            if(m_pConvolution[i].nY>=m_szCadrSize.y)
                                nYConv=0;
                        }
                        nXConv++;
                        nYConv++;
            */
////

            if(m_bWideView && (m_TexInfoStruct.btCamType==0 || m_TexInfoStruct.btCamType==3))
                SetSashWindows(false);
            else if(!m_bWideView && (m_TexInfoStruct.btCamType==1 || m_TexInfoStruct.btCamType==2))
                SetSashWindows(true);

            int nSize=event.GetExtraLong();
            if(m_bLIK==1)
            {

                int nCadrCount=min(nSize/(m_szCadrSize.x*2+1024),m_szCadrSize.y);
                if( nCadrCount<=0 || nCadrCount>m_szCadrSize.y)
                {
                    if(m_pData)
                    {
                        delete [] m_pData;
                        m_pData=NULL;
                    }
                    continue;
                }
                BYTE* pTemp=new BYTE[nCadrCount*m_szCadrSize.x*2];
                memset(pTemp,0,nCadrCount*m_szCadrSize.x*2);
                DWORD nOffset=0;
                DWORD dwCadrNum=0xffffffff;
                DWORD dwCurrCadrRec=0;
                do
                {
                    if(nOffset>nSize-m_szCadrSize.x*2-1024)
                        break;
                    bool bBreak=false;
                    while(memcmp(m_pData+nOffset,"FRM_DATA",8)!=0)
                    {
                        if(memcmp(m_pData+nOffset,"INF_DATA",8)==0)
                        {
                            memcpy(&m_InfoData,m_pData+nOffset+16,sizeof(m_InfoData));
                            wxPropertySheetDialog* dlg=(wxPropertySheetDialog*)wxWindow::FindWindowById(ID_COMMAND_DLG,NULL);
                            if(dlg)
                            {
                                NetworkPanel* pPanel=(NetworkPanel*)dlg->GetBookCtrl()->GetPage(0);
                                pPanel->SetInfo(&m_InfoData);
                            }
                        }
                        nOffset+=1024;
                        if(nOffset>nSize-m_szCadrSize.x*2-1024)
                        {
                            bBreak=true;
                            break;
                        }
                    }
                    if(bBreak)
                        break;
                    DWORD dwCurrCadrNum=*((DWORD*)(m_pData+nOffset+16+16));
                    int nResCadrNum=dwCurrCadrNum-dwCadrNum;
                    if(nResCadrNum>1 && dwCurrCadrRec>0)
                        dwCurrCadrRec--;
                    dwCadrNum=dwCurrCadrNum;

                    memcpy(pTemp+((nCadrCount-dwCurrCadrRec-1)*m_szCadrSize.x*2),m_pData+nOffset+m_TexInfoStruct.nVideoOffset,m_szCadrSize.x*2);

                    static u_int32_t nTimeString=0;
                    u_int32_t nMks=*((u_int32_t*)(m_pData+nOffset+16+28));
                    u_char btSec=*(m_pData+nOffset+16+27);
                    u_char btMin=*(m_pData+nOffset+16+26);
                    u_int32_t nCurrMksString=nMks+btSec*1000000+btMin*60000000;
                    m_nDeltaStringMks=nCurrMksString-nTimeString;
                    nTimeString=nCurrMksString;

                    dwCurrCadrRec++;
                    nOffset+=(m_szCadrSize.x*2)+1024;

                }
                while(nOffset<=nSize-m_szCadrSize.x*2-1024);

                if(dwCurrCadrRec>=0)
                {
                    CopyMono8ToRGB(pTemp+((nCadrCount-dwCurrCadrRec)*m_szCadrSize.x*2),m_szCadrSize.x,dwCurrCadrRec,m_pBmpData);
                    m_SmallScreen->AddPaintData(m_pBmpData,m_szCadrSize.x*dwCurrCadrRec*3);
                    m_BigScreen->AddPaintData(m_pBmpData,m_szCadrSize.x*dwCurrCadrRec*3);
                }
                delete [] pTemp;
                m_nCadrCount+=dwCurrCadrRec;
            }
            else if(nSize==m_szCadrSize.x*m_szCadrSize.y*2+m_TexInfoStruct.nVideoOffset)       //1024
            {
                CopyMono8ToRGB(m_pData+m_TexInfoStruct.nVideoOffset,m_szCadrSize.x,m_szCadrSize.y,m_pBmpData);
                m_SmallScreen->AddPaintData(m_pBmpData,m_szCadrSize.x*m_szCadrSize.y*3);
                m_BigScreen->AddPaintData(m_pBmpData,m_szCadrSize.x*m_szCadrSize.y*3);
                m_nCadrCount++;
            }
            if(m_pData)
            {
                delete [] m_pData;
                m_pData=NULL;
            }
        }
    }
}

void PKVIFrame::ChangeComPort(int nComPort)
{
    wxString sCom;
    if(m_comm)
        close(m_comm);
    m_comm=0;
    if(nComPort>0)
    {
        sCom.Printf("/dev/ttyS%d",nComPort-1);
        m_comm=open(sCom.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        if(m_comm==-1)
        {
            wxMessageBox("Не удается открыть COM порт");
            m_comm=0;
            m_nCommPort=0;
        }
        else
        {
            fcntl(m_comm, F_SETFL, FNDELAY);

            struct termios options;
            tcgetattr(m_comm, &options);
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
//			cfmakeraw(&options);
            options.c_cflag |= (CLOCAL | CREAD);
            options.c_cflag &= ~CSIZE; /* Маскирование битов размера символов */
            options.c_cflag |= CS8;    /* Установка 8 битов данных */
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_cflag &= ~CSTOPB;
//			options.c_oflag &= ~OPOST;
//			options.c_iflag |= (INPCK | ISTRIP);
//			options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//			options.c_iflag|=IUTF8;
            options.c_iflag&=~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|ICRNL|IXON);
            options.c_oflag&=~OPOST;
            options.c_lflag&=~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
            tcsetattr(m_comm, TCSANOW, &options);
            m_nCommPort=nComPort;
        }
    }

}


void PKVIFrame::SetSource(int nSource)
{
    wxMutexLocker lock(*m_mutexPKVI);
    m_nSource=nSource;
    FillWorkInfo();
    if(m_comm)
    {
        u_char pData[4];
        pData[0]=0xf2;
        pData[1]=LOBYTE(1);
        pData[2]=HIBYTE(1);

        pData[3]=nSource;
//		int nTry=0;
//		do
//		{
        write(m_comm,pData,4);
//			u_short usACK=0;
//			wxMilliSleep(20);
//			int nReaded=read(m_comm,&usACK,2);
//			if(nReaded==2 && usACK==0xFFFF)
//				break;
//		}
//		while(++nTry<3);
    }

}

int PKVIFrame::GetSource()
{
    wxMutexLocker lock(*m_mutexPKVI);
    return m_nSource;

}

bool PKVIFrame::SetCadrSize(wxSize szCadrSize, bool bLIK)
{
    wxMutexLocker lock(*m_mutexPKVI);
    if(m_szCadrSize!=szCadrSize || m_bLIK!=bLIK)
    {
        m_bLIK=bLIK;
        m_szCadrSize=szCadrSize;
        if(m_pBmpData)
            delete [] m_pBmpData;
        m_pBmpData=new BYTE[m_szCadrSize.x*3*m_szCadrSize.y];
        m_nWidthBitmap=((m_szCadrSize.x*3+3)/4)*4;
        if(m_pSavedBitmap)
            delete [] m_pSavedBitmap;
        m_pSavedBitmap=new BYTE[m_nWidthBitmap*m_szCadrSize.y];
//		if(m_pData)
//			delete m_pData;
//		m_pData=new BYTE[m_szCadrSize.x*2*m_szCadrSize.y];

        AddPaintData(NULL,0);
        wxCommandEvent* ev=new wxCommandEvent(wxEVT_CHANGE_CADR_SIZE,0);
        ev->SetClientData(&m_szCadrSize);
        wxQueueEvent(m_BigScreen,ev);
        ev=new wxCommandEvent(wxEVT_CHANGE_CADR_SIZE,0);
        ev->SetClientData(&m_szCadrSize);
        wxQueueEvent(m_SmallScreen,ev);

//		m_BigScreen->SetCadrSize(m_szCadrSize);
//		m_SmallScreen->SetCadrSize(m_szCadrSize);
        m_bfh.bfSize=sizeof(m_bfh)+sizeof(m_bih)+m_nWidthBitmap*m_szCadrSize.y;
        m_bih.biWidth=m_szCadrSize.x;
        m_bih.biHeight=m_szCadrSize.y;  //-
        m_bih.biSizeImage=m_bfh.bfSize-m_bfh.bfOffBits;
        return true;
    }
    return false;
}

wxSize PKVIFrame::GetCadrSize()
{
    wxMutexLocker lock(*m_mutexPKVI);
    return m_szCadrSize;
}

void PKVIFrame::SetRecCadrCount(int nCount)
{
    wxMutexLocker lock(*m_mutexPKVI);
    m_nRecCadrCount=nCount;
}

int PKVIFrame::GetRecCadrCount()
{
    wxMutexLocker lock(*m_mutexPKVI);
    return m_nRecCadrCount;
}

void PKVIFrame::CopyMono8ToRGB(BYTE* pSrc, int nWidth, int nHeight, BYTE* pDest)
{
//	wxMutexLocker lock(*m_mutexPKVI);
    if(pDest==NULL)
        return;
    WORD* pSrcWord=(WORD*)pSrc;
    for(int i=0; i<nHeight; i++)
    {
        for(int j=0; j<nWidth; j++)
        {
            if(m_bSum)
            {
                if(m_bLIK==1)
                    m_pSummData[j]+=pSrcWord[i*nWidth+j];
                else
                    m_pSummData[i*nWidth+j]+=pSrcWord[i*nWidth+j];
            }
            if(m_bVertFilter && ((i+m_nVertOffset)<nHeight) && !m_bLIK)
            {
                int nDiff=pSrcWord[i*nWidth+j]-pSrcWord[(i+m_nVertOffset)*nWidth+j];
                if(nDiff<0)
                    nDiff=0;
                pSrcWord[i*nWidth+j]=(WORD)nDiff;
            }
            float fData;
            if(!m_bLIK && m_TexInfoStruct.wMaxBright>m_TexInfoStruct.wMinBright && m_bContrast)
            {
                fData=pSrcWord[i*nWidth+j]-m_TexInfoStruct.wMinBright;
                if(fData<0)
                    fData=0;
                fData*=255.0/(m_TexInfoStruct.wMaxBright-m_TexInfoStruct.wMinBright);
                if(fData>255)
                    fData=255;
                pDest[i*nWidth*3+j*3] = pDest[i*nWidth*3+j*3+1] = pDest[i*nWidth*3+j*3+2] = (BYTE)fData;
            }
            else
                pDest[i*nWidth*3+j*3] = pDest[i*nWidth*3+j*3+1] = pDest[i*nWidth*3+j*3+2]=HIBYTE(pSrcWord[i*nWidth+j]);
        }
        if(m_bSave)
            memcpy(m_pSavedBitmap+i*m_nWidthBitmap,pDest+i*nWidth*3,nWidth*3);
        if(m_bSum && m_bLIK==1)
        {
            m_dwSummCadrs++;
            if(m_pProgressDlg)
            {
                m_pProgressDlg->Update(m_dwSummCadrs,wxString::Format("Накоплено %d кадров",m_dwSummCadrs));
                if(m_dwSummCadrs>=g_nSummCadrs)
                {
                    m_bSum=false;
                    CalculateSumm();
                }
            }
        }
    }
    if(m_bSave)
    {
        wxString sFile,s;
//        wxDateTime now=wxDateTime::UNow();
//        wxString sFn=wxString::Format("%s",now.FormatISOCombined());
        s.Printf("%d.%d.%d %d:%d:%d:%06d",m_TexInfoStruct.btExpYear,m_TexInfoStruct.btExpMonth,m_TexInfoStruct.btExpDay,m_TexInfoStruct.btExpHour,m_TexInfoStruct.btExpMinutes,m_TexInfoStruct.btExpSeconds,
                 m_TexInfoStruct.nExpMicroseconds);

        int i=0;
        wxString sBitmapsDir=wxFileName::GetHomeDir()+"/Bitmaps/"+sdir;
        wxFileName::Mkdir(sBitmapsDir,wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
        if (p_SaveBitmap)
        {
            if (FormatSavedCadrs)
            {
                do
                {
                    sFile.Printf(sBitmapsDir+"/Bitmap%05d.bmp",i++);
                }
                while(wxFile::Exists(sFile));
            }
            else
                sFile=sBitmapsDir+"/Bitmap"+s+".bmp";
            wxFile f(sFile,wxFile::write);
            if(f.IsOpened())
            {
                f.Write(&m_bfh,sizeof(m_bfh));
                f.Write(&m_bih,sizeof(m_bih));
                f.Write(m_pSavedBitmap,m_nWidthBitmap*m_szCadrSize.y);
                f.Close();
                f.Flush();
                //Save as JPEG
                wxBitmap m_bmp=wxBitmap(sFile,wxBITMAP_TYPE_BMP);
                wxImage m_img=m_bmp.ConvertToImage();
                //wxRemoveFile(sFile);
                sFile.Replace(".bmp",".jpg");
                m_img.SaveFile(sFile,wxBITMAP_TYPE_JPEG);
            }
//Test

            if (FormatSavedCadrs)
            {
                i--;
                do
                {
                    sFile.Printf(sBitmapsDir+"/Image%05d.dat",i++);
                }
                while(wxFile::Exists(sFile));
            }
            else
                sFile=sBitmapsDir+"/Image"+s+".dat";
            wxFile fImage(sFile,wxFile::write);
            if(fImage.IsOpened())
            {
                fImage.Write(pSrc,nWidth*nHeight*2);
            }
        }
//Kadr

        if (FormatSavedCadrs)
        {
            i=0;
            do
            {
                sFile.Printf(sBitmapsDir+"/Kadr%05d.dat",i++);
            }
            while(wxFile::Exists(sFile));
        }
        else
            sFile=sBitmapsDir+"/Kadr"+s+".dat";
        wxFile fKadr(sFile,wxFile::write);
        if(fKadr.IsOpened())
        {
            fKadr.Write(m_pData,m_szCadrSize.x*m_szCadrSize.y*2+m_TexInfoStruct.nVideoOffset);     //1024???
        }
        m_nSavedCadrsCount++;
        if(m_nSavedCadrsCount>=MaxSavedCadrs)
            m_bSave=false;

    }

    if(m_bSum && !m_bLIK && m_dwSummCadrs<g_nSummCadrs)
    {
        m_dwSummCadrs++;
        if(m_pProgressDlg)
        {
            m_pProgressDlg->Update(m_dwSummCadrs,wxString::Format("Накоплено %d кадров",m_dwSummCadrs));
            if(m_dwSummCadrs>=g_nSummCadrs)
            {
                m_bSum=false;
                CalculateSumm();
            }
        }
    }
}

void PKVIFrame::CalculateSumm()
{
    memset(&m_CalcInfo,0,sizeof(CALC_INFO));
    m_CalcInfo.wMax=0;
    m_CalcInfo.wMin=0xffff;
    int nCadrHeight=m_bLIK==1?1:m_szCadrSize.y;
    //Calculate min and max; make 2-bytes array
    WORD* pRawData=new WORD[m_szCadrSize.x*nCadrHeight];
    for(int i=0; i<m_szCadrSize.x*nCadrHeight; i++)
    {
        WORD wValue=WORD(m_pSummData[i]/g_nSummCadrs);
        m_CalcInfo.wMin=min(wValue,m_CalcInfo.wMin);
        m_CalcInfo.wMax=max(wValue,m_CalcInfo.wMax);
        m_CalcInfo.ullMid+=wValue;
        pRawData[i]=wValue;
    }
    m_CalcInfo.ullMid/=(m_szCadrSize.x*nCadrHeight);

    //Свертка окном 5*5
    WORD* pMaskData=new WORD[m_szCadrSize.x*nCadrHeight];
    memset(pMaskData,0,m_szCadrSize.x*nCadrHeight*2);
    CALC_INFO calcMask;
    memset(&calcMask,0,sizeof(CALC_INFO));
    calcMask.wMax=0;
    calcMask.wMin=0xffff;
    if(m_bLIK==1)
    {
        for(int j=2; j<m_szCadrSize.x-2; j++)
        {
            DWORD dwSum=0;
            for(int l=j-2; l<=j+2; l++)
                dwSum+=pRawData[l];
            WORD wValue=pMaskData[j]=dwSum/5;
            calcMask.wMin=min(wValue,calcMask.wMin);
            calcMask.wMax=max(wValue,calcMask.wMax);
            calcMask.ullMid+=wValue;
        }
        calcMask.ullMid/=(m_szCadrSize.x-4);

    }
    else
    {
        for(int i=2; i<nCadrHeight-2; i++)
        {
            for(int j=2; j<m_szCadrSize.x-2; j++)
            {
                DWORD dwSum=0;
                for(int k=i-2; k<=i+2; k++)
                {
                    for(int l=j-2; l<=j+2; l++)
                        dwSum+=pRawData[k*m_szCadrSize.x+l];
                }
                WORD wValue=pMaskData[i*m_szCadrSize.x+j]=dwSum/25;
                calcMask.wMin=min(wValue,calcMask.wMin);
                calcMask.wMax=max(wValue,calcMask.wMax);
                calcMask.ullMid+=wValue;
            }
        }
        calcMask.ullMid/=((m_szCadrSize.x-4)*(nCadrHeight-4));

    }


    //Вычитаем MaskData из RawData
    WORD* pResData=new WORD[m_szCadrSize.x*nCadrHeight];
    memset(pResData,0,m_szCadrSize.x*nCadrHeight*2);
    CALC_INFO calcRes;
    memset(&calcRes,0,sizeof(CALC_INFO));
    calcRes.wMax=0;
    calcRes.wMin=0xffff;

    if(m_bLIK==1)
    {
        for(int j=2; j<m_szCadrSize.x-2; j++)
        {
            WORD wValue=pResData[j]=abs(pRawData[j]-pMaskData[j]);
            calcRes.wMin=min(wValue,calcRes.wMin);
            calcRes.wMax=max(wValue,calcRes.wMax);
            calcRes.ullMid+=wValue;
        }
        calcRes.ullMid/=(m_szCadrSize.x-4);

    }
    else
    {
        for(int i=2; i<nCadrHeight-2; i++)
        {
            for(int j=2; j<m_szCadrSize.x-2; j++)
            {
                WORD wValue=pResData[i*m_szCadrSize.x+j]=abs(pRawData[i*m_szCadrSize.x+j]-pMaskData[i*m_szCadrSize.x+j]);
                calcRes.wMin=min(wValue,calcRes.wMin);
                calcRes.wMax=max(wValue,calcRes.wMax);
                calcRes.ullMid+=wValue;
            }
        }
        calcRes.ullMid/=((m_szCadrSize.x-4)*(nCadrHeight-4));

    }


//	//Save raw data in file
//	int i=0;
    wxString sFile;
//	do
//	{
//		sFile.Printf("/home/user/Bitmaps/SumCadr%03d.bin",i++);
//	}while(wxFile::Exists(sFile));
//	wxFile f(sFile,wxFile::write);
//	f.Write(pRawData,m_szCadrSize.x*nCadrHeight*2);
//	f.Close();

    //Make RGB array
    u_char* pDataRaw=new u_char[m_szCadrSize.x*nCadrHeight*3];
    u_char* pDataMask=new u_char[m_szCadrSize.x*nCadrHeight*3];
    u_char* pDataRes=new u_char[m_szCadrSize.x*nCadrHeight*3];

    for(int i=0; i<m_szCadrSize.x*nCadrHeight; i++)
    {
        float fData=pRawData[i]-m_CalcInfo.wMin;
        if(fData<0)
            fData=0;
        fData*=255.0/(m_CalcInfo.wMax-m_CalcInfo.wMin);
        if(fData>255)
            fData=255;
        pDataRaw[i*3] = pDataRaw[i*3+1] = pDataRaw[i*3+2] = m_bLIK==1?HIBYTE(pRawData[i]):(BYTE)fData;

        fData=pMaskData[i]-calcMask.wMin;
        if(fData<0)
            fData=0;
        fData*=255.0/(calcMask.wMax-calcMask.wMin);
        if(fData>255)
            fData=255;
        pDataMask[i*3] = pDataMask[i*3+1] = pDataMask[i*3+2] = m_bLIK==1?HIBYTE(pMaskData[i]):(BYTE)fData;

        fData=pResData[i]-calcRes.wMin;
        if(fData<0)
            fData=0;
        fData*=255.0/(calcRes.wMax-calcRes.wMin);
        if(fData>255)
            fData=255;
        pDataRes[i*3] = pDataRes[i*3+1] = pDataRes[i*3+2] = m_bLIK==1?HIBYTE(pResData[i]):(BYTE)fData;
    }

    //Save BMP file
    int i=0;
    wxString sBitmapsDir=wxFileName::GetHomeDir()+"/Bitmaps";
    wxFileName::Mkdir(sBitmapsDir,wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
    do
    {
        sFile.Printf(sBitmapsDir+"/SumCadr%03d.bmp",i++);
    }
    while(wxFile::Exists(sFile));
    wxFile f(sFile,wxFile::write);
    m_bfh.bfSize=sizeof(m_bfh)+sizeof(m_bih)+m_nWidthBitmap*nCadrHeight;
    m_bih.biHeight=nCadrHeight;
    f.Write(&m_bfh,sizeof(m_bfh));
    f.Write(&m_bih,sizeof(m_bih));
    f.Write(pDataRaw,m_szCadrSize.x*nCadrHeight*3);
    f.Close();

    i=0;
    do
    {
        sFile.Printf(sBitmapsDir+"/SumCadrMask%03d.bmp",i++);
    }
    while(wxFile::Exists(sFile));
    f.Create(sFile);
    f.Write(&m_bfh,sizeof(m_bfh));
    f.Write(&m_bih,sizeof(m_bih));
    f.Write(pDataMask,m_szCadrSize.x*nCadrHeight*3);
    f.Close();

    i=0;
    do
    {
        sFile.Printf(sBitmapsDir+"/SumCadrRes%03d.bmp",i++);
    }
    while(wxFile::Exists(sFile));
    f.Create(sFile);
    f.Write(&m_bfh,sizeof(m_bfh));
    f.Write(&m_bih,sizeof(m_bih));
    f.Write(pDataRes,m_szCadrSize.x*nCadrHeight*3);
    f.Close();

    m_bfh.bfSize=sizeof(m_bfh)+sizeof(m_bih)+m_nWidthBitmap*nCadrHeight;
    m_bih.biHeight=nCadrHeight;

    m_CalcInfo.ullDev=calcRes.ullMid;
    m_CalcInfo.wMax=m_bLIK==1?m_CalcInfo.wMax:calcMask.wMax;
    m_CalcInfo.wMin=m_bLIK==1?m_CalcInfo.wMin:m_TexInfoStruct.wMinBright;

//	m_CalcInfo.fD=m_CalcInfo.ullMid!=0?(100.0*(m_CalcInfo.wMax-m_CalcInfo.wMin)/m_CalcInfo.ullMid):0;
    if(!m_bLIK)
        m_CalcInfo.fD=100.0*m_CalcInfo.ullDev/(m_CalcInfo.wMax-m_CalcInfo.wMin);
    else
        m_CalcInfo.fD=100.0*m_CalcInfo.ullDev/m_CalcInfo.ullMid;
    wxQueueEvent(this,new wxCommandEvent(wxEVT_CALC_READY,GetId()));
    delete pRawData;
    delete pDataRaw;
    delete pMaskData;
    delete pDataMask;
}


void PKVIFrame::FillTexInfo(bool bFirst)
{
    wxPropertyGridPage* pg = m_pTexInfo->GetPage(wxT("ЗТИ"));
    if(bFirst)
    {
        pg->Clear();
        pg->SetBoolChoices("есть","нет");
        pg->Append(new wxUIntProperty("Номер камеры",wxPG_LABEL));
        pg->SetPropertyReadOnly("Номер камеры");
        pg->Append(new wxStringProperty("Тип камеры",wxPG_LABEL));
        pg->SetPropertyReadOnly("Тип камеры");
        pg->Append(new wxStringProperty("MAC-адрес камеры",wxPG_LABEL));
        pg->SetPropertyReadOnly("MAC-адрес камеры");
        pg->Append(new wxUIntProperty("Номер кадра",wxPG_LABEL));
        pg->SetPropertyReadOnly("Номер кадра");

        pg->Append(new wxPropertyCategory("Параметры формирования кадра",wxPG_LABEL));
        pg->Append(new wxStringProperty(" Время начала экспозиции",wxPG_LABEL));
        pg->Append(new wxStringProperty(" Период следования строк (мкс)",wxPG_LABEL));
        pg->Append(new wxUIntProperty(" Экспозиция (мкс)",wxPG_LABEL));
        pg->Append(new wxPointProperty(" Координаты начала фрагмента (пикс.)", wxPG_LABEL, wxPoint(0,0),true));
        pg->Append(new wxPointProperty(" Размер фрагмента (пикс.)", wxPG_LABEL, wxPoint(0,0),true));
        pg->Append(new wxBiningProperty(" Бинирование", wxPG_LABEL, wxPoint(0,0),true));
        pg->Append(new wxUIntProperty(" Минимальная яркость",wxPG_LABEL));
        pg->Append(new wxUIntProperty(" Максимальная яркость",wxPG_LABEL));

        pg->Append(new wxUIntProperty(" Величина порога",wxPG_LABEL));
        pg->Append(new wxFloatProperty(" Текущая температура",wxPG_LABEL));
        pg->Append(new wxFloatProperty(" Заданная температура",wxPG_LABEL));
        pg->Append(new wxFloatProperty(" Аналоговое усиление (дБ)",wxPG_LABEL));
        pg->Append(new wxFloatProperty(" ЕМ-усиление (дБ)",wxPG_LABEL));
        pg->Append(new wxFloatProperty(" Темп считывания (МГц)",wxPG_LABEL));
        pg->Append(new wxUIntProperty(" Смещение уровня черного",wxPG_LABEL));
        pg->Append(new wxStringProperty(" Выход матрицы",wxPG_LABEL));
        pg->Append(new wxFloatProperty(" Скорость переноса (мм/сек)",wxPG_LABEL));
        pg->Append(new wxStringProperty(" Количество ступеней интеграции",wxPG_LABEL));

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

        pg->Append(new wxPropertyCategory("Свертки",wxPG_LABEL));
        pg->AppendIn("Свертки",new wxUIntProperty(" Количество сверток",wxPG_LABEL));

        for(int i=0; i<64; i++)
        {
//			wxString s=wxString::Format(" Свертка %d",i+1);
            pg->AppendIn("Свертки",new wxPropertyCategory(wxString::Format(" Свертка %d",i+1),wxPG_LABEL));
            pg->Append(new wxPointProperty(wxString::Format(" Координаты свертки %d (пикс.)",i+1), wxPG_LABEL, wxPoint(0,0)));
            pg->Append(new wxUIntProperty(wxString::Format(" Объем над порогом %d (усл. ед.)",i+1),wxPG_LABEL));
            pg->Append(new wxUIntProperty(wxString::Format(" Площадь порога %d (усл. ед.)",i+1),wxPG_LABEL));
            pg->Append(new wxUIntProperty(wxString::Format(" Момент X*X %d",i+1),wxPG_LABEL));
            pg->Append(new wxUIntProperty(wxString::Format(" Момент X*Y %d",i+1),wxPG_LABEL));
            pg->Append(new wxUIntProperty(wxString::Format(" Момент Y*Y %d",i+1),wxPG_LABEL));
            pg->HideProperty(wxString::Format(" Свертка %d",i+1));
        }
        pg->SetPropertyReadOnly("Свертки");
        pg->CollapseAll();
    }
    else
    {
        pg->SetPropertyValue("Номер камеры",m_TexInfoStruct.btCamNum);
        wxString s;
        switch(m_TexInfoStruct.btCamType)
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
        s.Printf("%02x:%02x:%02x:%02x:%02x:%02x",m_TexInfoStruct.pbtMAC[0],m_TexInfoStruct.pbtMAC[1],
                 m_TexInfoStruct.pbtMAC[2],m_TexInfoStruct.pbtMAC[3],m_TexInfoStruct.pbtMAC[4],m_TexInfoStruct.pbtMAC[5]);
        pg->SetPropertyValue("MAC-адрес камеры",s);
        pg->SetPropertyValue("Номер кадра",long(m_TexInfoStruct.nCadrNum));

        s.Printf("%d:%d:%d:%06d %d.%d.%d",m_TexInfoStruct.btExpHour,m_TexInfoStruct.btExpMinutes,m_TexInfoStruct.btExpSeconds,
                 m_TexInfoStruct.nExpMicroseconds,m_TexInfoStruct.btExpDay,m_TexInfoStruct.btExpMonth,m_TexInfoStruct.btExpYear);
        pg->SetPropertyValue(" Время начала экспозиции",s);
        s.Printf("%d",m_nDeltaStringMks);
        pg->SetPropertyValue(" Период следования строк (мкс)",s);

        pg->SetPropertyValue(" Экспозиция (мкс)",long(m_TexInfoStruct.nExposition));
        wxPointProperty* pProp=(wxPointProperty*)pg->GetProperty(" Координаты начала фрагмента (пикс.)");
        pProp->SetValue(wxPoint(m_TexInfoStruct.wOffsetX,m_TexInfoStruct.wOffsetY));
        pProp=(wxPointProperty*)pg->GetProperty(" Размер фрагмента (пикс.)");
        pProp->SetValue(wxPoint(m_TexInfoStruct.wSizeX,m_TexInfoStruct.wSizeY));
        pProp=(wxPointProperty*)pg->GetProperty(" Бинирование");
        pProp->SetValue(wxPoint(m_TexInfoStruct.btBinX,m_TexInfoStruct.btBinY));
        pg->SetPropertyValue(" Минимальная яркость",m_TexInfoStruct.wMinBright);
        pg->SetPropertyValue(" Максимальная яркость",m_TexInfoStruct.wMaxBright);
//		pg->SetPropertyValue(" Парамтры обработки",wxPG_LABEL);

//		pg->SetPropertyValue(" Алгоритм расчета порога",wxPG_LABEL);
        pg->SetPropertyValue(" Величина порога",m_TexInfoStruct.wThresh);
        pg->SetPropertyValue(" Текущая температура",long(m_TexInfoStruct.nTempCurrent/256.0));
        pg->SetPropertyValue(" Заданная температура",long(m_TexInfoStruct.nTempSet/256.0));
        pg->SetPropertyValue(" Аналоговое усиление (дБ)",m_TexInfoStruct.wAnalogAmpl/256.0);
        pg->SetPropertyValue(" ЕМ-усиление (дБ)",m_TexInfoStruct.wEmAmpl/256.0);
        pg->SetPropertyValue(" Темп считывания (МГц)",m_TexInfoStruct.wReadFreq/256.0);
        pg->SetPropertyValue(" Смещение уровня черного",m_TexInfoStruct.wBlackLevel);
        pg->SetPropertyValue(" Выход матрицы",m_TexInfoStruct.btMatrixOutputNum?"без EM-усиления":"с EM-усилением");
        pg->SetPropertyValue(" Скорость переноса (мм/сек)",m_TexInfoStruct.wSpeedTransfer/256.0);
        pg->SetPropertyValue(" Количество ступеней интеграции",m_TexInfoStruct.btStepIntegration);

        pg->SetPropertyValue("Стабилизация уровня черного",m_TexInfoStruct.ProcessType.btStabBlackLevel);
        pg->SetPropertyValue("Иск. темнового тока",m_TexInfoStruct.ProcessType.btExcludeDarkCurrent);
        pg->SetPropertyValue("Корр. неравномерности чувств.",m_TexInfoStruct.ProcessType.btCorrSens);
        pg->SetPropertyValue("Авт. задание порога",m_TexInfoStruct.ThreshType.btAutoLevel);
        pg->SetPropertyValue("Поиск сверток",m_TexInfoStruct.ThreshType.btFindConvolution);

        float i=m_TexInfoStruct.DatchA%524288*360/524288.0;
        if (i>200.0)
            i-=360;
        s.Printf("%f",i);
//		s.Printf("%03d\xb0 %02d\' %02d\"",m_TexInfoStruct.wDatchAGrad,m_TexInfoStruct.btDatchAMinutes,m_TexInfoStruct.btDatchASeconds);
        pg->SetPropertyValue(" оси A",s);
        i=m_TexInfoStruct.DatchL%524288*360/524288.0;
        if (i>180)
            i-=180;
        s.Printf("%f",i);
//		s.Printf("%03d\xb0 %02d\' %02d\"",m_TexInfoStruct.wDatchLGrad,m_TexInfoStruct.btDatchLMinutes,m_TexInfoStruct.btDatchLSeconds);
        pg->SetPropertyValue(" оси L",s);
        i=m_TexInfoStruct.DatchR%524288*360/524288.0;
        if (i>180)
            i-=180;
        s.Printf("%f",i);
//		s.Printf("%03d\xb0 %02d\' %02d\"",m_TexInfoStruct.wDatchRGrad,m_TexInfoStruct.btDatchRMinutes,m_TexInfoStruct.btDatchRSeconds);
        pg->SetPropertyValue(" оси R",s);

        pg->SetPropertyValue(" Количество сверток",m_TexInfoStruct.wConvolutionsCount);

        for(int i=0; i<64; i++)
        {
            wxString sProp=wxString::Format(" Свертка %d",i+1);
            if(i>=m_nConvolutionCount)
            {
                pg->HideProperty(wxString::Format(" Свертка %d",i+1));
            }
            else if(m_pConvolution)
            {
                pg->HideProperty(wxString::Format(" Свертка %d",i+1),false);
                wxPointProperty* p=(wxPointProperty*)pg->GetProperty(wxString::Format(" Координаты свертки %d (пикс.)",i+1));
                p->SetValue(wxPoint(m_pConvolution[i].nX,m_pConvolution[i].nY));
                pg->SetPropertyValue(wxString::Format(" Объем над порогом %d (усл. ед.)",i+1),(int)m_pConvolution[i].nV);
                pg->SetPropertyValue(wxString::Format(" Площадь порога %d (усл. ед.)",i+1),(int)m_pConvolution[i].nS);
                pg->SetPropertyValue(wxString::Format(" Момент X*X %d",i+1),(int)m_pConvolution[i].nS_XX);
                pg->SetPropertyValue(wxString::Format(" Момент X*Y %d",i+1),(int)m_pConvolution[i].nS_XY);
                pg->SetPropertyValue(wxString::Format(" Момент Y*Y %d",i+1),(int)m_pConvolution[i].nS_YY);
            }
        }
        if (m_bVoik!=0) m_WorkInfo->SetFocus();
    }
}

void PKVIFrame::FillWorkInfo()
{
    wxString s;
    float fA=m_TexInfoStruct.DatchA%524288*360/524288.0;
    if (fA>200.0)
        fA-=360;
    float fL=m_TexInfoStruct.DatchL%524288*360/524288.0;
    if (fL>180)
        fL-=180;
    float fR=m_TexInfoStruct.DatchR%524288*360/524288.0;
    if (fR>180)
        fR-=180;

//    if (!BUILDROOT)
//    {
//        s=wxDateTime::Now().Format(wxT("%d.%m.%y %H:%M:%S"));     //это не работает в buildroote
//    }
//    else
//    {
    time_t rawtime;
    time (&rawtime);
    s.Printf ("%s", ctime (&rawtime));
//    }

    s.Printf("Источник сигнала: %s\nПоказания осей:\n\tA: %f\n\tL: %f\n\tR: %f\nДата и время: %s\n", m_pCamInfo==NULL?"НЕТ":m_pCamInfo->sName,fA,fL,fR,s);
    m_WorkInfo->Clear();
    m_WorkInfo->SetDefaultStyle(wxTextAttr(*wxBLACK));
    m_WorkInfo->AppendText(s);
    if(m_connectStatus==status_connect_tex || m_connectStatus==status_connect_multi)
    {
        if(m_swCadr.Time()<=5000)
        {
            m_WorkInfo->SetDefaultStyle(wxTextAttr(*wxGREEN));
            m_WorkInfo->AppendText("СОЕДИНЕНИЕ УСТАНОВЛЕНО");
        }
        else
        {
            m_WorkInfo->SetDefaultStyle(wxTextAttr(*wxRED));
            m_WorkInfo->AppendText("ДАННЫЕ НЕ ПОСТУПАЮТ");
        }
    }
    else
    {
        m_WorkInfo->SetDefaultStyle(wxTextAttr(*wxBLUE));
        m_WorkInfo->AppendText("СОЕДИНЕНИЕ ОТСУСТВУЕТ");
    }
    if (m_bVoik!=0)
    {
        wxString sm;
        sm.Printf("\n\nКоординаты: х=%d у=%d D=%d\n", m_BigScreen->m_marker.Coord.x,m_BigScreen->m_marker.Coord.y,m_BigScreen->m_marker.nSize);
        m_WorkInfo->AppendText(sm);
    }
}

void PKVIFrame::ChangeViewParams(const wxPoint pOffset, const wxSize szDim,bool bFinish)
{
//	m_TexInfo->SetItem(8,1,	wxString::Format("%d : %d",pOffset.x, pOffset.y));
//	m_TexInfo->SetItem(9,1,wxString::Format("%d : %d",szDim.x, szDim.y));
    FillWorkInfo();
    if(m_comm && bFinish)
    {
        u_char pData[12];
        pData[0]=0xf1;
        pData[1]=LOBYTE(9);
        pData[2]=HIBYTE(9);

        pData[3]=LOBYTE(u_short(pOffset.x));
        pData[4]=HIBYTE(u_short(pOffset.x));
        pData[5]=LOBYTE(u_short(pOffset.y));
        pData[6]=HIBYTE(u_short(pOffset.y));

        pData[7]=LOBYTE(u_short(m_szCadrSize.x));
        pData[8]=HIBYTE(u_short(m_szCadrSize.x));
        pData[9]=LOBYTE(u_short(m_szCadrSize.y));
        pData[10]=HIBYTE(u_short(m_szCadrSize.y));

        pData[11]=m_BigScreen->m_fMashtab;

//		int nTry=0;
//		do
//		{
        write(m_comm,pData,12);
//			u_short usACK=0;
//			wxMilliSleep(20);
//			int nReaded=read(m_comm,&usACK,2);
//			if(nReaded==2 && usACK==0xFFFF)
//				break;
//		}
//		while(++nTry<3);
    }

}

void PKVIFrame::ChangeStatus(bool bUU, bool bSboy)
{
    m_bUU=bUU;
    m_bSboy=bSboy;
    if(m_comm)
    {
        u_char pData[4];
        pData[0]=0xf3;
        pData[1]=LOBYTE(1);
        pData[2]=HIBYTE(1);

        pData[3]=(m_bUU==false?0:1)|((m_bSboy==0?0:1)<<1);
        write(m_comm,pData,4);
    }
}

void PKVIFrame::RequestUU()
{
    if(m_comm)
    {
        u_char pData[3];
        pData[0]=0xf4;
        pData[1]=0;
        pData[2]=0;
        write(m_comm,pData,3);
    }
}

void PKVIFrame::OnRBtnDown(wxMouseEvent& event)
{
    if(m_bPult)
    {
        wxMenu* menu = new wxMenu;
        menu->AppendCheckItem(ID_SHOW_STATUSBAR, wxT("&Статусная строка"),"Показать или спрятать статусную строку");
        menu->AppendCheckItem(ID_SHOW_TOOLBAR, wxT("&Панель инструментов"),"Показать или спрятать панель инструментов");
        PopupMenu(menu, event.GetPosition());
    }
}

void PKVIFrame::OnShowStatusBar(wxCommandEvent& event)
{
    m_bStatusBar=!m_bStatusBar;
    m_StatusBar->Show(m_bStatusBar);
    if (!BUILDROOT)
    {
        wxConfig config("PKVI");
        config.Write("/Interface/StatusBar",(long)m_bStatusBar);
    }
}

void PKVIFrame::OnShowToolbar(wxCommandEvent& event)
{
    m_bToolBar=!m_bToolBar;
    m_ToolBar->Show(m_bToolBar);
    if (!BUILDROOT)
    {
        wxConfig config("PKVI");
        config.Write("/Interface/ToolBar",(long)m_bToolBar);
    }
}

void PKVIFrame::OnUpdateShowToolbar(wxUpdateUIEvent& event)
{
    event.Check(m_bToolBar);
}

void PKVIFrame::OnUpdateShowStatusbar(wxUpdateUIEvent& event)
{
    event.Check(m_bStatusBar);
}

void PKVIFrame::OnIntercept(wxCommandEvent& event)
{
    wxMutexLocker lock(*m_mutexPKVI);
    m_bMulticast=!m_bMulticast;
}

void PKVIFrame::OnSave(wxCommandEvent& event)
{
//    SetSashWindows(!m_bWideView);
//    return;
    m_nSavedCadrsCount=0;
    m_bSave=!m_bSave;
    if (m_bSave)
    {
        wxDateTime now=wxDateTime::UNow();
        sdir=wxString::Format("%s",now.FormatISOCombined());
    }
}

void PKVIFrame::OnSum(wxCommandEvent& event)
{
    wxMutexLocker lock(*m_mutexPKVI);
//Debug
//	SetCadrSize(wxSize(512,512));
//End Debug
    m_bSum=!m_bSum;
    if(m_pSummData)
    {
        delete [] m_pSummData;
        m_pSummData=NULL;

    }

    if(m_bSum)
    {
        int nCadrHeight=m_bLIK?1:m_szCadrSize.y;
        m_pSummData=new DWORD[m_szCadrSize.x*nCadrHeight];
        memset(m_pSummData,0,m_szCadrSize.x*nCadrHeight*sizeof(DWORD));
        m_pProgressDlg=new wxProgressDialog("Накопление кадров",
                                            "Накоплено 0 кадров",g_nSummCadrs,this,wxPD_APP_MODAL|wxPD_SMOOTH|wxPD_CAN_ABORT|wxPD_ELAPSED_TIME|wxPD_AUTO_HIDE);
        m_dwSummCadrs=0;
        m_timerSum.Start(500,false);
    }
}

void PKVIFrame::OnUpdateIntercept(wxUpdateUIEvent& event)
{
    event.Check(m_bMulticast);
}

void PKVIFrame::OnUpdateStart(wxUpdateUIEvent& event)
{
    event.Check(m_bInit);
}

void PKVIFrame::OnUpdateSave(wxUpdateUIEvent& event)
{
    event.Check(m_bSave);
}

void PKVIFrame::OnUpdateSum(wxUpdateUIEvent& event)
{
//	event.Enable(m_bInit);
    event.Check(m_bSum);
}

//bool PKVIFrame::IsMulticast()
//{
//	wxMutexLocker lock(*m_mutexPKVI);
//	return m_bMulticast;
//}

void PKVIFrame::AddInfoData(u_char* pData)
{
    wxMutexLocker lock(*m_mutexPKVI);
//	if(dwSize>=sizeof(INFO_DATA)+16 && memcmp(pData,"INF_DATA",8)==0)
    {
        memcpy(&m_InfoData,pData,sizeof(INFO_DATA));
        wxQueueEvent(this,new wxCommandEvent(wxEVT_INFO_READY,GetId()));
    }
}

void PKVIFrame::OnCommand(wxCommandEvent& event)
{
    wxPropertySheetDialog dlg(this,ID_COMMAND_DLG,"Управление камерой",wxDefaultPosition,wxDefaultSize,
                              wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxPROPSHEET_SHRINKTOFIT);
    dlg.CreateButtons(wxOK);
    NetworkPanel* pNetPanel=new NetworkPanel(dlg.GetBookCtrl());
    memcpy(&(pNetPanel->m_InfoData),&m_InfoData,sizeof(INFO_DATA));

    dlg.GetBookCtrl()->AddPage(pNetPanel,"Управление по ethernet",true);
    CanPanel* pCanPanel=new CanPanel(dlg.GetBookCtrl());
    pCanPanel->m_nCanCameraNum=m_nCanCameraNum;
    pCanPanel->m_nCanInterface=m_nCanInterface;
    pCanPanel->m_pCan=&m_Can;
    pCanPanel->Init();
    dlg.GetBookCtrl()->AddPage(pCanPanel,"Управление по CAN",false);
    dlg.LayoutDialog();
    dlg.ShowModal();

}

void PKVIFrame::SendCommand(u_char* pCommandData)
{
    wxMutexLocker lock(*m_mutexPKVI);
    if(!IsInit())
        return;
    u_char* pCommand=new u_char[1024];
    memcpy(pCommand,pCommandData,1024);
    m_listCommand.Append(pCommand);
}

u_char* PKVIFrame::GetCommand()
{
    wxMutexLocker lock(*m_mutexPKVI);
    ListData::Node* node=m_listCommand.GetFirst();
    u_char* pCommand=NULL;
    if(node)
    {
        pCommand=(node->GetData());
        m_listCommand.DeleteNode(node);
        return pCommand;
    }
    return NULL;
}

void PKVIFrame::BreakLink()
{
    wxMutexLocker lock(*m_mutexPKVI);
    m_InfoData.btReady=0;
}

void PKVIFrame::GetOurIP()
{
    ifaddrs* ifaddr=NULL, *ifa=NULL;
    int family;
    char host[NI_MAXHOST];
    m_saInterfaces.Clear();
    m_saIP.Clear();
    if(getifaddrs(&ifaddr) == -1)
    {
        wxMessageBox("Не удается получить IP адреса.",wxT("Ошибка"),wxOK|wxICON_HAND);
        return;
    }
    /* Walk through linked list, maintaining head pointer so we
       can free list later */
    for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr)
        {
            family = ifa->ifa_addr->sa_family;
            /* Display interface name and family (including symbolic
               form of the latter for the common families) */
            /* For an AF_INET* interface address, display the address */
            if(family == AF_INET && strncmp(ifa->ifa_name,"eth",3)==0)
            {
                getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

                m_saIP.Add(host);
                m_saInterfaces.Add(ifa->ifa_name);

                if(strcmp(ifa->ifa_name,m_sDevice.c_str())==0)
                {
                    m_sOurIP=host;
                }
            }

        }
    }
    freeifaddrs(ifaddr);
    if(m_saInterfaces.IsEmpty())
        m_sDevice.Empty();
}

//bool PKVIFrame::GetCameraInfo(CAMERA_INFO* pCamInfo)
//{
//	wxMutexLocker lock(*m_mutexPKVI);
//	if(pCamInfo->sName!=m_pCamInfo->sName)
//	{
//		*pCamInfo=*m_pCamInfo;
//		return true;
//	}
//	return false;
//
//}

bool PKVIFrame::IsReconnect(CAMERA_INFO* pCamInfo, bool* pMulticast)
{
    wxMutexLocker lock(*m_mutexPKVI);
    bool bRet=false;

    if ((!m_bPult) && (BUILDROOT))          //uak 27.05.2013
    {
        int i=m_pStatusCamera->GetCurrentSelection();
        //wxString sCamName=m_pStatusCamera->GetValue();
        //wxMessageBox(sCamName);
        if (m_nCamNum!=i)
        {
            m_nCamNum=i;
            m_pCamInfo=m_CamInfoArr[m_nCamNum];
        }
    }

    if(pCamInfo->sName!=m_pCamInfo->sName || *pMulticast!=m_bMulticast || m_bReconnect)
    {
        //wxString sCamName=m_pStatusCamera->GetValue();
        //wxMessageBox(sCamName);
        *pCamInfo=*m_pCamInfo;
        *pMulticast=m_bMulticast;
        m_bReconnect=false;
        bRet=true;
    }
    return bRet;
}

void PKVIFrame::OnChangeConnectStatus(wxCommandEvent& event)
{
    CONNECT_STATUS stNew=(CONNECT_STATUS)(long)event.GetClientData();
    if(m_connectStatus!=stNew)
    {
        switch(stNew)
        {
        case status_no:
            m_pStatusBmp->SetIcon(graylamp);
            m_StatusBar->SetStatusText("Нет подключения",1);
            break;
        case status_connect_multi:
            m_pStatusBmp->SetIcon(greenlamp);
            m_StatusBar->SetStatusText(wxString::Format("Перехват трафика камеры %s (%s)",m_pCamInfo->sName,m_pCamInfo->sGroupIP),1);
            m_swCadr.Start();
            break;
        case status_connect_tex:
            m_pStatusBmp->SetIcon(redlamp);
            m_StatusBar->SetStatusText(wxString::Format("Подключение к камере %s (%s)",m_pCamInfo->sName,m_pCamInfo->sIP),1);
            m_swCadr.Start();
            break;
        case status_try_tex:
            m_StatusBar->SetStatusText(wxString::Format("Попытка подключения к камере %s (%s)",m_pCamInfo->sName,m_pCamInfo->sIP),1);
            break;
        case status_try_multi:
            m_StatusBar->SetStatusText(wxString::Format("Попытка перехвата трафика камеры %s (%s)",m_pCamInfo->sName,m_pCamInfo->sGroupIP),1);
            break;
        }
        m_connectStatus=stNew;
    }

}

void PKVIFrame::OnSpinContrast(wxSpinDoubleEvent& event)
{
    if(!m_bContrast)
    {
        m_BigScreen->SetContrast(event.GetValue());
        m_SmallScreen->SetContrast(event.GetValue());
        if (m_bVoik!=0) m_WorkInfo->SetFocus();

    }
}

void PKVIFrame::OnSpinBright(wxSpinDoubleEvent& event)
{
    if(!m_bContrast)
    {
        m_BigScreen->SetBright(event.GetValue());
        m_SmallScreen->SetBright(event.GetValue());
        if (m_bVoik!=0) m_WorkInfo->SetFocus();
    }
}

void PKVIFrame::OnSpinTresh(wxSpinEvent& event)
{
    m_ucTresh=event.GetValue();
    wxConfig config(_("PKVI"));
    if (!BUILDROOT)
    {
        config.Write("/Settings/Tresh",long(m_ucTresh));
    }
    m_fKoeff=config.ReadDouble("/Settings/Koeff",1.0);

    for(int i=0; i<256; i++)
    {
        if(i<=m_ucTresh)
            m_pfMapGamma[i]=i*m_fKoeff/256.0;
        else
            m_pfMapGamma[i]=i/256.0;
    }
    m_BigScreen->SetGamma(m_pfMapGamma);
    m_SmallScreen->SetGamma(m_pfMapGamma);

}

void PKVIFrame::OnSpinDelay(wxSpinDoubleEvent& event)
{
    float m_u=event.GetValue();
    m_timerVoik.Stop();
    m_bVoikTimer=m_u*1000;
    m_timerVoik.Start(m_bVoikTimer,false);
    m_WorkInfo->SetFocus();
}

void PKVIFrame::OnSpinKoeff(wxSpinDoubleEvent& event)
{
    m_fKoeff=event.GetValue();
    if (!BUILDROOT)
    {
        wxConfig config(_("PKVI"));
        config.Write("/Settings/Koeff",m_fKoeff);
    }
    for(int i=0; i<256; i++)
    {
        if(i<=m_ucTresh)
            m_pfMapGamma[i]=i*m_fKoeff/256.0;
        else
            m_pfMapGamma[i]=i/256.0;
    }
    m_BigScreen->SetGamma(m_pfMapGamma);
    m_SmallScreen->SetGamma(m_pfMapGamma);
}

void PKVIFrame::OnStatusCameraSelect(wxCommandEvent& event)
{
//    wxString COString=wxT("56;58;60;");
//    wxString CCString=wxT("57;59;");
    wxString COString=wxT("61;62;81;82;10;");
    wxString CCString=wxT("71;72;91;92;");
    wxString ST0,ST1;
    wxString str_eth;
    MARKER marker;
    int i0,i1;

    if(m_mutexPKVI==NULL)
        return;
    wxMutexLocker lock(*m_mutexPKVI);
    int nCamera=event.GetSelection();

    if (m_bVoik!=0)
    {
        wxString s;
        int wmas,res,n;
        switch (nCamera)
        {
        case voik_Stop:
            WarpPointer(1*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginX*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetX,          //убрать мигание
                        1*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginY*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetY);
            marker.nSize=20;
            marker.nType=rect;
            marker.bVisible=true;
            m_BigScreen->SetMarkerParams(&marker);                  //делаем маркер видимым
            m_timerVoik.Stop();
            break;
        case voik_Pusk:
            m_timerVoik.Start(m_bVoikTimer,false);
            break;
        case voik_Up:
            m_timerVoik.Stop();
            if (m_bVoikTimer-DiscrVoik>=1)
                m_bVoikTimer-=DiscrVoik;
            m_timerVoik.Start(m_bVoikTimer,false);
            break;
        case voik_Down:
            m_timerVoik.Stop();
            m_bVoikTimer+=DiscrVoik;
            m_timerVoik.Start(m_bVoikTimer,false);
            break;
        case voik_Next:
            m_timerVoik.Stop();
            res=write(voik_pipe[1],&wmas,sizeof(wmas));
            break;
        case voik_Prev:
            n=v_paths.Count()-1;
            m_timerVoik.Stop();
            switch (n)
            {
            case 0:
            case 1:
                break;
            default:
                if (nFile==0)
                    nFile=n-1;
                else
                    nFile-=2;
                break;
            }
            res=write(voik_pipe[1],&wmas,sizeof(wmas));
            break;
        case voik_Get:
            VoikGetObject();
            break;
        case voik_SendKadr:
            VoikSendKadr();
            break;
        case voik_BZ:
            if (IsInit())
            {
                wxCommandEvent ev;
                OnStart(ev);
            }
            res=write(voik_pipe[1],&wmas,sizeof(wmas));
            VoikBZ();
            ShowVoikRes();
            break;
        case voik_Scan:
            CopyDirSt(m_bVoikPath+"/voik_mnt/561",m_bVoikPath+"/voik_wrk/561");           //561
            CopyDirSt(m_bVoikPath+"/voik_mnt/562",m_bVoikPath+"/voik_wrk/562");           //562
            CopyDirSt(m_bVoikPath+"/voik_mnt/571",m_bVoikPath+"/voik_wrk/571");           //571
            CopyDirSt(m_bVoikPath+"/voik_mnt/572",m_bVoikPath+"/voik_wrk/572");           //572
            CopyDirSt(m_bVoikPath+"/voik_mnt/581",m_bVoikPath+"/voik_wrk/581");           //581
            CopyDirSt(m_bVoikPath+"/voik_mnt/582",m_bVoikPath+"/voik_wrk/582");           //582
            CopyDirSt(m_bVoikPath+"/voik_mnt/591",m_bVoikPath+"/voik_wrk/591");           //591
            CopyDirSt(m_bVoikPath+"/voik_mnt/592",m_bVoikPath+"/voik_wrk/592");           //592
            CopyDirSt(m_bVoikPath+"/voik_mnt/601",m_bVoikPath+"/voik_wrk/601");           //601
            CopyDirSt(m_bVoikPath+"/voik_mnt/602",m_bVoikPath+"/voik_wrk/602");           //602
            break;
        }
        m_pStatusCamera->SetSelection(voik_No);
        m_WorkInfo->SetFocus();
        return;
    }

    if(m_nCamNum!=nCamera)
    {
        /*
                i0=m_CamInfoArr[m_nCamNum]->sName.Index(wxT("5"));
                if (i0<0)
                    i0=m_CamInfoArr[m_nCamNum]->sName.Index(wxT("6"));
                i1=m_CamInfoArr[nCamera]->sName.Index(wxT("5"));
                if (i1<0)
                    i1=m_CamInfoArr[nCamera]->sName.Index(wxT("6"));
                ST0=m_CamInfoArr[m_nCamNum]->sName.substr(i0,2)+wxT(";");
                ST1=m_CamInfoArr[nCamera]->sName.substr(i1,2)+wxT(";");
        */
        ST0=m_CamInfoArr[m_nCamNum]->sGroupIP.Mid(6,2)+wxT(";");    //224.0.61.104
        ST1=m_CamInfoArr[nCamera]->sGroupIP.Mid(6,2)+wxT(";");      //224.0.101.104
        //wxMessageBox(ST0+" -> "+ST1);
        //сменить сетевую карту: ETH2 - CO  ETH1 - CC
        if (COString.Contains(ST0) && CCString.Contains(ST1))        //eth2->eth1
        {
            str_eth.Printf("%s%d","eth",CCeth);
            m_sDevice=str_eth;   //wxT("eth1");
            GetOurIP();
        }
        else if (COString.Contains(ST1) && CCString.Contains(ST0))   //eth1->eth2
        {
            str_eth.Printf("%s%d","eth",COeth);
            m_sDevice=str_eth;   //wxT("eth2");
            GetOurIP();
        }

        //wxMessageBox(str_eth);
        m_nCamNum=nCamera;
        m_pCamInfo=m_CamInfoArr[m_nCamNum];
        ChangeConnect();
        if (!BUILDROOT)
        {
            wxConfig config("PKVI");
            config.Write("/Settings/CameraNum",m_nCamNum);
        }
//        m_StatusBar->SetStatusText(wxString::Format("%s (%s)",m_pCamInfo->sName,m_pCamInfo->sIP),m_bPult?6:4);
    }
}

void PKVIFrame::OnStatusConnectSelect(wxCommandEvent& event)
{
    if(m_mutexPKVI==NULL)
        return;
    wxMutexLocker lock(*m_mutexPKVI);
    CONNECT_TYPE typeNew=(CONNECT_TYPE)(long)event.GetClientData();
    if(m_connectType==typeNew)
        return;
    m_connectType=typeNew;
    ChangeConnect();
    if (!BUILDROOT)
    {
        wxConfig config("PKVI");
        config.Write("/Settings/ConnectType",(long)m_connectType);
    }
}

void PKVIFrame::ChangeConnect()
{
    wxMutexLocker lock(*m_mutexPKVI);
//    if(!m_bPult)
    {
        AddPaintData(NULL,0);
        switch(m_connectType)
        {
        case connect_no:
            m_bInit=false;
            m_pStatusBmp->SetIcon(graylamp);
            m_connectStatus=status_no;
            m_StatusBar->SetStatusText("Нет подключения",1);
            break;
        case connect_auto:
        case connect_tex:
            m_bInit=true;
            m_bMulticast=false;
            if(!GetThread()->IsRunning())
            {
                GetThread()->SetPriority(80);
                GetThread()->Run();
            }
            m_connectStatus=status_try_tex;
            m_StatusBar->SetStatusText(wxString::Format("Попытка подключения к камере %s (%s)",m_pCamInfo->sName,m_pCamInfo->sIP),1);
            break;
        case connect_multi:
            m_bInit=true;
            m_bMulticast=true;
            if(!GetThread()->IsRunning())
            {
                GetThread()->SetPriority(80);
                GetThread()->Run();
            }
            m_connectStatus=status_try_multi;
            m_StatusBar->SetStatusText(wxString::Format("Попытка перехвата трафика %s (%s)",m_pCamInfo->sName,m_pCamInfo->sGroupIP),1);
            break;
        }
        m_bReconnect=true;
    }
}

CONNECT_STATUS PKVIFrame::GetConnectStatus()
{
    wxMutexLocker lock(*m_mutexPKVI);
    return m_connectStatus;
}

CONNECT_TYPE PKVIFrame::GetConnectType()
{
    wxMutexLocker lock(*m_mutexPKVI);
    return m_connectType;
}

void PKVIFrame::OnTimerConnect(wxTimerEvent& event)
{
    wxMutexLocker lock(*m_mutexPKVI);
    if(m_pStatusBmp && (m_connectStatus==status_try_multi || m_connectStatus==status_try_tex))
    {
        static bool bSwitch=true;
        if(bSwitch)
            m_pStatusBmp->SetIcon(m_connectStatus==status_try_multi?greenlamp:redlamp);
        else
            m_pStatusBmp->SetIcon(graylamp);
        bSwitch=!bSwitch;
    }
}

void PKVIFrame::OnCP(wxCommandEvent& event)
{
    int res;
    unsigned char bbb=0;
    if (flpt>0)
    {
        bbb^=0x77;
        res=ioctl(flpt,PPWDATA,&bbb);
    }
}

void PKVIFrame::OnPKVI(wxCommandEvent& event)
{
    int res;
    unsigned char bbb=0;
    if (flpt>0)
    {
        bbb^=0x6f;
        res=ioctl(flpt,PPWDATA,&bbb);
    }
}

void PKVIFrame::OnSaveK(wxCommandEvent& event)
{
    if(!m_bPult && !BUILDROOT)
    {
        m_nSavedCadrsCount=0;
        m_bSave=!m_bSave;
        if (m_bSave)
        {
            wxDateTime now=wxDateTime::UNow();
            sdir=wxString::Format("%s",now.FormatISOCombined());
            wxMessageBox(_T("Сохранение кадров"));
        }
        else
        {
            wxMessageBox(_T("Выполнено сохранение кадров"));
        }
    }
}

void PKVIFrame::OnVertFilter(wxCommandEvent& event)
{
    m_bVertFilter=!m_bVertFilter;
    if (!BUILDROOT)
    {
        wxConfig config(_("PKVI"));
        config.Write(_("/Settings/VertFilter"),m_bVertFilter);
    }
}

void PKVIFrame::OnContrast(wxCommandEvent& event)
{
    m_bContrast=!m_bContrast;
    if (!BUILDROOT)
    {
        wxConfig config(_("PKVI"));
        config.Write(_("/Settings/Contrast"),m_bContrast);
    }
    if(m_bContrast)
    {
        m_BigScreen->SetContrast(1);
        m_SmallScreen->SetContrast(1);
        m_BigScreen->SetBright(0);
        m_SmallScreen->SetBright(0);
        m_pStatusContrast->Disable();
        m_pStatusBright->Disable();
    }
    else
    {
        if (m_bPult)
        {
            m_BigScreen->SetContrast(m_pStatusContrast->GetValue());
            m_SmallScreen->SetContrast(m_pStatusContrast->GetValue());
            m_BigScreen->SetBright(m_pStatusBright->GetValue());
            m_SmallScreen->SetBright(m_pStatusBright->GetValue());
            m_pStatusContrast->Enable();
            m_pStatusBright->Enable();
        }
    }

}
void PKVIFrame::OnKeyDown(wxKeyEvent& event)
{
    if (!BUILDROOT)
    {
        wxString key;
        MARKER marker;
        int wmas,res,n;
        long keycode=event.GetKeyCode();
        switch (keycode)
        {
        case WXK_F11:
            //key=wxT("F11");
            //wxMessageBox(key);
            if (!m_bSave)
            {
                wxDateTime now=wxDateTime::UNow();
                sdir=wxString::Format("%s",now.FormatISOCombined());
            }
            m_nSavedCadrsCount=0;
            m_bSave=true;
            break;
        case WXK_HOME:                                              //STOP
            //case 'S':                                              //STOP
            if (m_bVoik!=0)   //((m_bVoik!=0) && (event.AltDown()))
            {
                WarpPointer(1*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginX*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetX,
                            1*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginY*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetY);
                marker.nSize=20;
                marker.nType=rect;
                marker.bVisible=true;
                m_BigScreen->SetMarkerParams(&marker);                  //делаем маркер видимым
                m_timerVoik.Stop();
                break;
            }
            break;
        case WXK_PAGEUP:                                            //PREV
            if (m_bVoik!=0)
            {
                n=v_paths.Count()-1;
                m_timerVoik.Stop();
                switch (n)
                {
                case 0:
                case 1:
                    break;
                default:
                    if (nFile==0)
                        nFile=n-1;
                    else
                        nFile-=2;
                    break;
                }
                res=write(voik_pipe[1],&wmas,sizeof(wmas));
            }
            break;
        case WXK_END:                                               //PUSK
            if (m_bVoik!=0)
            {
                m_timerVoik.Start(m_bVoikTimer,false);
            }
            break;
        case WXK_PAGEDOWN:                                          //NEXT
            if (m_bVoik!=0)
            {
                m_timerVoik.Stop();
                res=write(voik_pipe[1],&wmas,sizeof(wmas));
            }
            break;
        case 'O':                                                   //object Alt-O
            if (m_bVoik!=0) //&& (event.AltDown()))
            {
                VoikGetObject();
            }
            break;
        case 'K':                                                   //Cadr Alt-K
            if (m_bVoik!=0) //&& (event.AltDown()))
            {
                VoikSendKadr();
            }
            break;
        case 'B':                                                   //BZ Alt-B
            if (m_bVoik!=0) //&& (event.AltDown()))
            {
                if (IsInit())
                {
                    wxCommandEvent ev;
                    OnStart(ev);
                }
                res=write(voik_pipe[1],&wmas,sizeof(wmas));
                VoikBZ();
                ShowVoikRes();
            }
            break;
        case 'S':                                                   //Scan Alt-S
            if (m_bVoik!=0) //&& (event.AltDown()))
            {
                CopyDirSt(m_bVoikPath+"/voik_mnt/561",m_bVoikPath+"/voik_wrk/561");           //561
                CopyDirSt(m_bVoikPath+"/voik_mnt/562",m_bVoikPath+"/voik_wrk/562");           //562
                CopyDirSt(m_bVoikPath+"/voik_mnt/571",m_bVoikPath+"/voik_wrk/571");           //571
                CopyDirSt(m_bVoikPath+"/voik_mnt/572",m_bVoikPath+"/voik_wrk/572");           //572
                CopyDirSt(m_bVoikPath+"/voik_mnt/581",m_bVoikPath+"/voik_wrk/581");           //581
                CopyDirSt(m_bVoikPath+"/voik_mnt/582",m_bVoikPath+"/voik_wrk/582");           //582
                CopyDirSt(m_bVoikPath+"/voik_mnt/591",m_bVoikPath+"/voik_wrk/591");           //591
                CopyDirSt(m_bVoikPath+"/voik_mnt/592",m_bVoikPath+"/voik_wrk/592");           //592
                CopyDirSt(m_bVoikPath+"/voik_mnt/601",m_bVoikPath+"/voik_wrk/601");           //601
                CopyDirSt(m_bVoikPath+"/voik_mnt/602",m_bVoikPath+"/voik_wrk/602");           //602
            }
            break;
        case WXK_LEFT:
            if (m_bVoik!=0)
            {
                m_BigScreen->m_marker.Coord.x--;
                m_BigScreen->SetBright(0);                  //это Refresh()
            }
            break;
        case WXK_RIGHT:
            if (m_bVoik!=0)
            {
                m_BigScreen->m_marker.Coord.x++;
                m_BigScreen->SetBright(0);
            }
            break;
        case WXK_UP:
            if (m_bVoik!=0)
            {
                m_BigScreen->m_marker.Coord.y--;
                m_BigScreen->SetBright(0);
            }
            break;
        case WXK_DOWN:
            if (m_bVoik!=0)
            {
                m_BigScreen->m_marker.Coord.y++;
                m_BigScreen->SetBright(0);
            }
            break;
        default:
            event.Skip();

            return;
        }
    }
    event.Skip();
}

void PKVIFrame::OnKeyUp(wxKeyEvent& event)
{
    if (!BUILDROOT)
    {
        long keycode=event.GetKeyCode();
        switch (keycode)
        {
        case WXK_F11:
            m_bSave=false;
            break;
        }
    }
    event.Skip();
}

void PKVIFrame::LogEvent(wxKeyEvent& event)
{
    if (flpt>0)
    {
        wxString msg;
        int res;
        unsigned char bbb=0,sss=0;
        long keycode=event.GetKeyCode();
        switch (keycode)
        {
        case '1':
        case '4':
        case '7':
            bbb^=0x6f;
            res=ioctl(flpt,PPRSTATUS,&sss);
            msg.Printf("PPRSTATUS=%8x %8x:%s\n",sss,res,strerror(errno));
            //wxMessageBox(_T(msg));
            res=ioctl(flpt,PPWDATA,&bbb);
            msg.Printf("PPWDATA=%8x %8x:%s\n",bbb,res,strerror(errno));
            //wxMessageBox(_T(""+msg));
            break;
        case '2':
        case '5':
        case '8':
            bbb^=0x5f;
            res=ioctl(flpt,PPRSTATUS,&sss);
            msg.Printf("PPRSTATUS=%8x %8x:%s\n",sss,res,strerror(errno));
            //wxMessageBox(_T(msg));
            res=ioctl(flpt,PPWDATA,&bbb);
            msg.Printf("PPWDATA=%8x %8x:%s\n",bbb,res,strerror(errno));
            //wxMessageBox(_T(""+msg));
            break;
        case '3':
        case '6':
        case '9':
            bbb^=0x77;
            res=ioctl(flpt,PPRSTATUS,&sss);
            msg.Printf("PPRSTATUS=%8x %8x:%s\n",sss,res,strerror(errno));
            //wxMessageBox(_T(msg));
            res=ioctl(flpt,PPWDATA,&bbb);
            msg.Printf("PPWDATA=%8x %8x:%s\n",bbb,res,strerror(errno));
            //wxMessageBox(_T(""+msg));
            break;
        case 's':                       //сохранение кадров на диск
            if(!m_bPult && !BUILDROOT)
            {
                m_bSave=!m_bSave;
                if (m_bSave)
                {
                    wxMessageBox(_T("Сохранение кадров"));
                }
                else
                {
                    wxMessageBox(_T("Выполнено сохранение кадров"));
                }
            }
            break;
        default:
            event.Skip();
            break;
        }
    }
    else
    {
        event.Skip();
    }
}

void PKVIFrame::OnBmp(wxCommandEvent& event)
{
    if (m_bInit)
        return;

    bool bBmp=1;
    int bWidth,bHeight,bOffBits;
    wxFileOffset Pos1;
    size_t bSize=0,nReaded;
    wxString sFile;
    BYTE* p;

    wxString caption=wxT("Выберите файл");
    wxString wildcard=wxT("DAT files (*.dat)|*.dat|BMP files (*.bmp)|*.bmp|PRT files (*.prt)|*.prt");
    wxString DefaultFileName=wxEmptyString;
    wxString sBitmapsDir=wxFileName::GetHomeDir()+"/Bitmaps";
    wxFileDialog* FileDialog1;
    FileDialog1 = new wxFileDialog(this, caption, sBitmapsDir, wxEmptyString, wildcard, wxFD_DEFAULT_STYLE|wxFD_OPEN, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

    if (!m_bVoik)
    {
        //    wxFileDialog fdialog(this,caption,sBitmapsDir,DefaultFileName,wildcard,wxOpen);
        if (FileDialog1->ShowModal()==wxID_OK)  //(FileDialog1.ShowModal()==wxID_OK)
        {
            sFile=FileDialog1->GetPath();
        }
        else
        {
            return;
        }
    }
    else
        sFile="/tmp/resKPST.dat";

    /*это очистка экранов CScreenWindow*/
    m_szCadrSize.x=0;
    m_szCadrSize.y=0;
    m_BigScreen->SetCadrSize(m_szCadrSize);
    m_SmallScreen->SetCadrSize(m_szCadrSize);
    if (m_pSavedBitmap)
        delete [] m_pSavedBitmap;
    m_pSavedBitmap=new BYTE(1);
    m_BigScreen->AddPaintData(m_pSavedBitmap,3);
    m_SmallScreen->AddPaintData(m_pSavedBitmap,3);
    /*    m_BigScreen->SetMashtab(1.9);   изменяет масштаб big_screena  */

    bBmp=sFile.Contains(".bmp");
    m_sWorkText=sFile;
    FillWorkInfo();
    if (bBmp)
    {
        //sFile.Printf(sBitmapsDir+"/Bitmap042.bmp");
        wxFile f(sFile,wxFile::read);
        if(f.IsOpened())
        {
            f.Read(&m_bfh,sizeof(m_bfh));
            f.Read(&m_bih,sizeof(m_bih));
            bWidth=m_bih.biWidth;
            bHeight=abs(m_bih.biHeight);
            m_szCadrSize.x=bWidth;
            m_szCadrSize.y=bHeight;
            m_BigScreen->SetCadrSize(m_szCadrSize);
            m_SmallScreen->SetCadrSize(m_szCadrSize);
            /*
             		wxCommandEvent* ev=new wxCommandEvent(wxEVT_CHANGE_CADR_SIZE,0);
            		ev->SetClientData(&m_szCadrSize);
            		wxQueueEvent(m_BigScreen,ev);
            */
            bOffBits=m_bfh.bfOffBits;    /*начало изображения*/
            if (m_bih.biSizeImage==0)
            {
                bSize = bWidth * 3 + bWidth % 4;
                bSize = bSize * bHeight;
            }
            else
            {
                bSize=m_bih.biSizeImage;
            }

            if (m_pSavedBitmap)
                delete [] m_pSavedBitmap;
            m_pSavedBitmap=new BYTE[bSize];
            Pos1=f.Seek(bOffBits,wxFromStart);
            if (Pos1==wxInvalidOffset)
            {
                wxMessageBox(wxT("Sorry, could not seek in file."));
            }

            nReaded=f.Read(m_pSavedBitmap,bSize);
            if (nReaded<bSize)
            {
                wxMessageBox(wxT("Error read file."));
            }
            p=m_pSavedBitmap;

            m_BigScreen->AddPaintData(m_pSavedBitmap,m_szCadrSize.x*m_szCadrSize.y*3);
            m_SmallScreen->AddPaintData(m_pSavedBitmap,m_szCadrSize.x*m_szCadrSize.y*3);

        }
    }
    else
    {
        if (sFile.Contains(".prt"))     //show protokol
        {
            PrProtDialog dlg(this);
            dlg.fn=sFile;
            dlg.ldFile(sFile);
            dlg.ShowModal();

            /*
                        wxFileInputStream f(sFile);
                        wxTextInputStream fs(f);
                        wxString msg;
                        fs >> msg;
                        wxMessageDialog dlg(this,msg,"Протокол работы",wxOK|wxCENTRE|wxICON_INFORMATION);
                        dlg.ShowModal();
            */
        }
        else if (!sFile.Contains("Image"))    //Kadr042.dat  kadr+texinfo
        {
            wxFile f(sFile,wxFile::read);
            if(f.IsOpened())
            {
                bSize=(long)f.Length();//-1024;
                if (m_pSavedBitmap)
                    delete [] m_pSavedBitmap;
                m_pSavedBitmap=new BYTE[bSize];
                nReaded=f.Read(m_pSavedBitmap,bSize);    //1024 texinfo
                f.Close();
                memcpy(&m_TexInfoStruct,m_pSavedBitmap+16,sizeof(TEX_INFO));

                TEX_INFO TexInfoStruct;
                memcpy(&TexInfoStruct,m_pSavedBitmap+16,sizeof(TEX_INFO));

                m_bLIK=m_TexInfoStruct.btCamType==2;
                m_szCadrSize.y=m_bLIK?int((m_TexInfoStruct.wSpeedTransfer/256.0)*1000/(13*15)):m_TexInfoStruct.wSizeY;
                m_szCadrSize.x=m_bLIK?1:m_TexInfoStruct.wSizeX;

                //nReaded=f.Read(m_pSavedBitmap,bSize);
                if (nReaded<bSize)
                {
                    wxMessageBox(wxT("Error read file."));
                }
                FillTexInfo(false);

                if(m_TexInfoStruct.wConvolutionsCount!=m_nConvolutionCount)
                {
                    if(m_pConvolution)
                    {
                        delete [] m_pConvolution;
                        m_pConvolution=NULL;
                    }
                    m_nConvolutionCount=m_TexInfoStruct.wConvolutionsCount;
                    if(m_nConvolutionCount>0)
                    {
                        m_pConvolution=new CONVOLUTION[m_nConvolutionCount];
                    }
                }
                if(m_nConvolutionCount>0 && m_pConvolution!=NULL)
                {
                    memcpy(m_pConvolution,m_pSavedBitmap+256,m_nConvolutionCount*sizeof(CONVOLUTION));        //256=240+16
                    //memcpy(m_pConvolution,&m_TexInfoStruct+240,m_nConvolutionCount*sizeof(CONVOLUTION));        //256=240+16
                }
                p=m_pSavedBitmap+m_TexInfoStruct.nVideoOffset;
                if (m_bVoik!=0)
                    if (nVoikKadr==-1)
                        p=m_pSavedBitmap+m_TexInfoStruct.nVideoOffset;
                    else
                    {
                        wxFile fv(v_paths[nVoikKadr],wxFile::read);
                        if(fv.IsOpened())
                        {
                            long bSize1=(long)fv.Length();
                            BYTE* pData=new BYTE[bSize];
                            size_t nReaded1=fv.Read(pData,bSize1);
                            fv.Close();
                            TEX_INFO pTex;
                            memcpy(&pTex,pData+16,sizeof(TEX_INFO));
                            memcpy(p,pData+pTex.nVideoOffset,m_szCadrSize.y*m_szCadrSize.x*2);
                            delete [] pData;
                        }
                    }
            }
        }
        else
        {
            //sFile.Printf(sBitmapsDir+"/Image042.dat");        //kadr
            wxFile f(sFile,wxFile::read);
            if(f.IsOpened())
            {
                bSize=(long)f.Length();
                if (m_pSavedBitmap)
                    delete [] m_pSavedBitmap;
                m_pSavedBitmap=new BYTE[bSize];
                nReaded=f.Read(m_pSavedBitmap,bSize);
                if (nReaded<bSize)
                {
                    wxMessageBox(wxT("Error read file."));
                }

                m_szCadrSize.x=2048;
                m_szCadrSize.y=1024;
                m_TexInfoStruct.wMaxBright=1920;
                m_TexInfoStruct.wMinBright=120;
                DatChoiceDialog dlg(this);
                dlg.sCam=_("БПК");
                dlg.sCadr=_("2048-1024");
                dlg.sBright=_("128-1920");
                if(dlg.ShowModal()==wxID_OK)
                {
                    wxString st1;
                    st1=dlg.sCam;
                    if (st1.Contains("ЛИК"))
                    {
                        m_bLIK=true;
                    }
                    else
                    {
                        if (st1.Contains("БПК") || st1.Contains("ШПК") || st1.Contains("БИК"))
                        {
                            m_bLIK=false;
                        }
                        else
                        {
                            m_bLIK=false;
                        }
                    }
                    int i=0,n1=0,n2=0;
                    st1=dlg.sCadr;
                    i=st1.Index(_("-"));
                    if (i!=0)
                    {
                        n1=atoi(st1.SubString(0,i-1));
                        n2=atoi(st1.SubString(i+1,st1.length()-1));
                        if (n1*n2*2!=bSize)
                        {
                            wxMessageBox(_("Неверно заданы параметры кадра"),"");
                            return;
                        }
                        m_szCadrSize.x=n1;
                        m_szCadrSize.y=n2;
                    }
                    st1=dlg.sBright;
                    i=st1.Index(_("-"));
                    if (i!=0)
                    {
                        n1=atoi(st1.SubString(0,i-1));
                        n2=atoi(st1.SubString(i+1,st1.length()-1));
                        if (n1>=n2)
                        {
                            wxMessageBox(_("Неверно заданa яркость"),"");
                            return;
                        }
                        m_TexInfoStruct.wMaxBright=n2;
                        m_TexInfoStruct.wMinBright=n1;
                        FillTexInfo(false);
                    }
                }

            }
        }
        if (m_pBmpData)
            delete [] m_pBmpData;
        m_pBmpData=new BYTE[m_szCadrSize.x*m_szCadrSize.y*3];
        m_BigScreen->SetCadrSize(m_szCadrSize);
        m_SmallScreen->SetCadrSize(m_szCadrSize);

//        CopyMono8ToRGB(m_pSavedBitmap,m_szCadrSize.x,m_szCadrSize.y,m_pBmpData);
        CopyMono8ToRGB(p,m_szCadrSize.x,m_szCadrSize.y,m_pBmpData);

        m_BigScreen->AddPaintData(m_pBmpData,m_szCadrSize.x*m_szCadrSize.y*3);
        m_SmallScreen->AddPaintData(m_pBmpData,m_szCadrSize.x*m_szCadrSize.y*3);
    }
}
void PKVIFrame::printProtCam(int num, u_char* pData)
{
    if (!docum) return;
    long tt;
    long deltat=0;
    wxString sBitmapsDir=wxFileName::GetHomeDir()+"/Bitmaps";
    wxFileName::Mkdir(sBitmapsDir,wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);           //create if not exist
    wxString sFile;
    sFile.Printf("%s%d%s",wxStandardPaths::Get().GetUserConfigDir()+"/Bitmaps/PKVI",num,".prt");
    TEX_INFO TexInfoStruct;
    memcpy(&TexInfoStruct,pData+16,sizeof(TEX_INFO));
    tt=TexInfoStruct.nExpMicroseconds+1000000*TexInfoStruct.btExpSeconds+1000000*60*TexInfoStruct.btExpMinutes+1000000*60*60*TexInfoStruct.btExpHour;
    if (m_TimeCadr)
        deltat=tt-m_TimeCadr;
    //deltat=800001;
    float t1;
    t1=deltat/1000000.0;
    wxString s;
    s.Printf("%d:%d:%d:%06d %d.%d.%d ДТ=%f",TexInfoStruct.btExpHour,TexInfoStruct.btExpMinutes,TexInfoStruct.btExpSeconds,
             TexInfoStruct.nExpMicroseconds,TexInfoStruct.btExpDay,TexInfoStruct.btExpMonth,TexInfoStruct.btExpYear,t1);
    float i=TexInfoStruct.wReadFreq/256.0;
    wxDateTime now = wxDateTime::UNow();
    wxString s1;
    long dt=TexInfoStruct.nExpMicroseconds+1000000*TexInfoStruct.btExpSeconds+1000000*60*TexInfoStruct.btExpMinutes+1000000*60*60*TexInfoStruct.btExpHour-m_TimeCadr0;
    if ((!m_TimeCadr0) || (dt<0.0000001))
    {
        s1="";
    }
    else
    {
        s1.Printf("%f",1000000.0*(m_TimeCadrN+1)/dt);
    }
    int xb=0,yb=0;
    xb=1 << TexInfoStruct.btBinX;
    yb=1 << TexInfoStruct.btBinY;
    /*
        switch (TexInfoStruct.btBinX)
        {
            case 0:
                xb=1;
                break;
            case 1:
                xb=2;
                break;
            case 2:
                xb=4;
                break;
            case 3:
                xb=8;
                break;
        }
        switch (TexInfoStruct.btBinY)
        {
            case 0:
                yb=1;
                break;
            case 1:
                yb=2;
                break;
            case 2:
                yb=4;
                break;
            case 3:
                yb=8;
                break;
        }
    */
    wxString sb=wxString::Format("Бинирование(X,Y) %d,%d",xb,yb);
    wxString msg=wxString::Format("%s: Номер кадра: %d\nВремя начала экспозиции: %s\nЭкспозиция(мкс): %d\nТемп считывания(МГц): %f\n%s\nЧастота кадров(к/c) %s\n\n",
                                  now.FormatISOCombined(),TexInfoStruct.nCadrNum,s,TexInfoStruct.nExposition,i,sb,s1);
//								  now.Format("%c", wxDateTime::CET).c_str(),TexInfoStruct.nCadrNum,s,TexInfoStruct.nExposition,i);
    if(wxFile::Exists(sFile))
    {
        if ((m_TimeCadr) && (deltat))
        {
            if(m_fProtCam.Open(sFile,wxFile::write_append))
                m_fProtCam.Write(msg);
        }
    }
    else
    {
        m_fProtCam.Create(sFile);
        m_fProtCam.Write(msg);
        m_fProtCam.Flush();
    }
    m_TimeCadr=TexInfoStruct.nExpMicroseconds+1000000*TexInfoStruct.btExpSeconds+1000000*60*TexInfoStruct.btExpMinutes+1000000*60*60*TexInfoStruct.btExpHour;
    if (!m_TimeCadr0)
        m_TimeCadr0=m_TimeCadr;
    m_TimeCadrN++;
}

void PKVIFrame::OnSashDrag(wxSashEvent& event)
{
    if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE)
        return;
}
void PKVIFrame::ShowVoikRes()
{
    /*
        return;
        v_paths.Empty();
        v_filenames.Empty();
        v_paths.Add("/tmp/resKPST.dat");
        v_filenames.Add("resKPST.dat");
    */
    if (!IsInit())
    {
        nVoikKadr=-1;
        FILE *fp;
        double dt1,dt2;

        if ((fp = fopen("/tmp/testVOIK.obj", "rb"))==NULL)
        {
            std::cout<<"He удается открыть файл tesVOIK.obj"<<std::endl;
            return;
        }
        nObj* nob=(nObj*)malloc(sizeof(nObj));
        fread(nob, sizeof(nObj),1, fp);
        double t=nob[0].T;

        if (m_tob!=NULL)
            delete [] m_tob;
        m_tob=new double[v_paths.Count()];
        BYTE* pData=new BYTE[1024];

        int i;
        if (m_tob!=NULL)
        {
            for(i=v_paths.Count()-1; i>=0; i--)
            {
                wxFile f(v_paths[i],wxFile::read);
                if(f.IsOpened())
                {
                    f.Read(pData,1024);    //читаем texinfo
                    TEX_INFO* pInfo=(TEX_INFO*)(pData+16);
                    f.Close();
                    m_tob[i]=pInfo->btExpHour*3600+pInfo->btExpMinutes*60+pInfo->btExpSeconds+pInfo->nExpMicroseconds/1000000.0+pInfo->nExposition/2000000.0;
                    if (m_tob[i]<=t)
                    {
                        int k1;
                        dt1=t-m_tob[i];
                        if (i+1<v_paths.Count())
                        {
                            dt2=m_tob[i+1]-t;
                        }
                        else
                        {
                            dt2=3600;
                        }
                        if (dt2<dt1)
                        {
                            k1=i+1;
                        }
                        else
                        {
                            k1=i;
                        }
                        nVoikKadr=k1;
                        std::cout<<v_paths[k1]<<std::endl;
                        break;
                    }
                }
            }
        }

        if (m_tob!=NULL)
            delete [] m_tob;
        delete [] pData;

        fclose(fp);
        free(nob);

        wxCommandEvent ev;
        OnBmp(ev);   //
    }
}
void PKVIFrame::VoikDlgProc()
{
    if (m_bVoik==0)
        return;
//    CopyDirSt("/opt/temp/fts_test","/home/user/uak/tmp");
//    if (!v_paths.IsEmpty())
//    {
    v_paths.Empty();
//    }
//    if (!v_filenames.IsEmpty())
//    {
    v_filenames.Empty();
//    }

    wxString caption=wxT("Выберите файлы");
    wxString wildcard=wxT("DAT files (*.dat)|*.dat");
    wxString DefaultFileName=wxEmptyString;
    wxString sBitmapsDir=m_bVoikPath+"/voik_wrk";        //wxFileName::GetHomeDir()+"/Bitmaps";
    wxFileDialog* FileDialog1;
    FileDialog1 = new wxFileDialog(this, caption, sBitmapsDir, wxEmptyString, wildcard, wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_MULTIPLE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

//    wxFileDialog fdialog(this,caption,sBitmapsDir,DefaultFileName,wildcard,wxOpen);
    if (FileDialog1->ShowModal()==wxID_OK)  //(FileDialog1.ShowModal()==wxID_OK)
    {
        FileDialog1->GetPaths(v_paths);
        FileDialog1->GetFilenames(v_filenames);

        wxString msg, s;
        size_t count = v_paths.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            if (n>30) break;
            s.Printf(wxT("%d: %s\n"),
                     (int)n, v_paths[n].c_str());

            msg += s;
        }
        s.Printf(wxT("Filter index: %d"), FileDialog1->GetFilterIndex());
        msg += s;

        wxMessageDialog dialog2(this, msg, wxT("Selected files"));
        dialog2.ShowModal();
        nFile=0;
        nKadr=0;
        MARKER marker;
        marker.nSize=20;
        marker.nType=rect;
        marker.bVisible=true;
        m_BigScreen->SetMarkerParams(&marker);
        m_VoiklistData.DeleteContents(false);
        //clear voik files
        wxString sFile;
        for (int k=0;; k++)
        {
            sFile.Printf(wxT("%s%d%s"),"/tmp/ForApoi",k,".dat");
            if (wxFileExists(sFile))
                wxRemoveFile(sFile);
            else
                break;
        }
        sFile="/tmp/resKPST.dat";
        if (wxFileExists(sFile))
            wxRemoveFile(sFile);
        sFile="/tmp/testVOIK.dat";
        if (wxFileExists(sFile))
            wxRemoveFile(sFile);
        sFile="/tmp/testVOIK.obj";
        if (wxFileExists(sFile))
            wxRemoveFile(sFile);

        //номер станции
        if (v_paths[0].Contains("561"))
            v_stan="561";
        else if (v_paths[0].Contains("562"))
            v_stan="562";
        else if (v_paths[0].Contains("571"))
            v_stan="571";
        else if (v_paths[0].Contains("572"))
            v_stan="572";
        else if (v_paths[0].Contains("581"))
            v_stan="581";
        else if (v_paths[0].Contains("582"))
            v_stan="582";
        else if (v_paths[0].Contains("591"))
            v_stan="591";
        else if (v_paths[0].Contains("592"))
            v_stan="592";
        else if (v_paths[0].Contains("601"))
            v_stan="601";
        else if (v_paths[0].Contains("602"))
            v_stan="602";
        else
            v_stan="";

    }
    else
    {
        return;
    }
}

void PKVIFrame::VoikShowKadr(int n)
{
    wxSize szCadrSize;
    szCadrSize.x=szVoikCadrSize.x;
    szCadrSize.y=szVoikCadrSize.y;
    u_int32_t nDataSize=szCadrSize.x*szCadrSize.y;
    /*
            szCadrSize.x=1024;
            szCadrSize.y=1;
            u_int32_t nDataSize=1024;
            u_char* pData=new u_char[nDataSize*2];
    */
    size_t bSize,nReaded;
    int rmas,res;
    wxString sFile;

    sFile.Printf(wxT("%s"),v_paths[n].c_str());
    //wxMessageBox(sFile);
    wxFile f(sFile,wxFile::read);
    if(f.IsOpened())
    {
        bSize=(long)f.Length()-1024;
        nReaded=f.Read(pVoikData,1024);    //читаем texinfo
        TEX_INFO* pInfo=(TEX_INFO*)(pVoikData+16);
        bool bLIK=pInfo->btCamType==2;
        szCadrSize.y=bLIK?int((pInfo->wSpeedTransfer/256.0)*1000/(13*15)):pInfo->wSizeY;
        if(szCadrSize.y==0)
            szCadrSize.y=1;
        szCadrSize.x=pInfo->wSizeX;
        int nDataSizeNew=bLIK?(szCadrSize.x*2+1024)*szCadrSize.y:szCadrSize.y*szCadrSize.x*2+pInfo->nVideoOffset;
        if(nDataSize!=nDataSizeNew || bLIK!=m_bLIK)
        {
            nDataSize=nDataSizeNew;
            SetCadrSize(szCadrSize,bLIK);
            szVoikCadrSize.x=szCadrSize.x;
            szVoikCadrSize.y=szCadrSize.y;
            BYTE* pTemp=new BYTE[nReaded];
            memcpy(pTemp,pVoikData,nReaded);
            delete [] pVoikData;
            pVoikData=new BYTE[nDataSize*2];
            memcpy(pVoikData,pTemp,nReaded);
            delete [] pTemp;
        }
        nReaded=f.Read(pVoikData+1024,bSize);    //читаем kadr

        if (nReaded<bSize)
        {
            wxMessageBox(wxT("Error read file."));
        }
        AddPaintData(pVoikData,nDataSize);
        f.Close();
        f.Flush();
    }
    res=read(voik_pipe[0],&rmas,sizeof(rmas));
    //wxMilliSleep(900);
}

void PKVIFrame::VoikFilmProc()
{
    if (m_bVoik!=1)
        return;
    size_t count = v_paths.GetCount();
    wxSize szVoikCadrSize;
    szVoikCadrSize.x=1024;
    szVoikCadrSize.y=1;
    pVoikData=new u_char[szVoikCadrSize.x*szVoikCadrSize.y*2];
    if (count==0)           //ничего не выбрано
        return;

//    for ( size_t n = 0; n < count; n++ )
    for ( ;; )
    {
        if (!m_bInit) break;
//        nFile=n;
        VoikShowKadr(nFile);
        if (!m_BigScreen->m_marker.bVisible)                //маркер не виден после setsashwindow
        {
            m_BigScreen->m_marker.bVisible=true;
            m_BigScreen->m_marker.nSize=20;
            m_BigScreen->m_marker.nType=rect;
        }
        nFile++;
        if (nFile>=count)
            nFile=0;
    }           //for
    delete [] pVoikData;
}

void PKVIFrame::OnTimerVoik(wxTimerEvent& event)
{
    int wmas,res;

    if (m_bVoik!=1)
        return;
    float f=m_pStatusDelay->GetValue();
    if (m_bPult && fabsf(f*1000-m_bVoikTimer)>1)
    {
        wxString s;
        m_pStatusDelay->SetValue(m_bVoikTimer/1000.);
        //s.Printf("%d мсек",m_bVoikTimer);
        //m_StatusBar->SetStatusText(s,5);
    }
    res=write(voik_pipe[1],&wmas,sizeof(wmas));
}

void PKVIFrame::OnTimerVoik1(wxTimerEvent& event)
{
    if (m_bVoik!=1)
        return;
    ::wxSetCursor(wxCURSOR_MAGNIFIER);
}

void PKVIFrame::VoikGetObject()
{
    wxPoint obj=m_BigScreen->m_marker.Coord,pt;
    float Mashtab;
    int x,y;

    int Count = m_VoiklistData.GetCount();
    if (Count>0)                                //очистим список
    {
        ListObj::Node* node=NULL;
        do
        {
            node=m_VoiklistData.GetFirst();
            if(node)
            {
                wxPoint* pTemp=(node->GetData());
                m_VoiklistData.DeleteNode(node);
                if(pTemp)
                    delete [] pTemp;
            }
        }
        while(node);
        //m_VoiklistData.DeleteContents(false);               //.true, instructs the list to call delete on objects stored in the list whenever they are removed
        Count = m_VoiklistData.GetCount();
        //m_VoiklistData.Clear();
    }

    x=m_BigScreen->m_marker.Coord.x;
    y=m_BigScreen->m_marker.Coord.y;

    if(m_VoiklistData.GetCount()<=10)        //number of objects
    {
        wxPoint* pAddedData=new wxPoint;
        pAddedData->x=x;
        pAddedData->y=y;
        m_VoiklistData.Append(pAddedData);
    }
    WarpPointer(x*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginX*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetX,
                y*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginY*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetY);
    ::wxSetCursor(wxCURSOR_BULLSEYE);
    m_timerVoik1.Start(600,wxTIMER_ONE_SHOT);
    /*
        int Count = m_VoiklistData.GetCount();
        ListObj::Node* node = m_VoiklistData.GetFirst();
        wxPoint* Cust = node->GetData();
        x=Cust->x;
        y=Cust->y;
    */
    Mashtab=m_BigScreen->m_fMashtab;
}

void PKVIFrame::VoikSendKadr()
{
    wxPoint* obj;
    int x,y;
    int nc=m_VoiklistData.GetCount();
//    if (nc<1) return;
    CONVOLUTION* VoikConvolution;
    int i=0;
    if (nc>0)
    {
        VoikConvolution=new CONVOLUTION[nc];
        memset(VoikConvolution,0,sizeof(VoikConvolution));
    }

    wxString sFile;
    u_char* pData;
    size_t nReaded;
    sFile.Printf(wxT("%s"),v_paths[nFile].c_str());
    wxFile f1(sFile,wxFile::read);
    if(!f1.IsOpened()) return;

    int nDataSizeNew=m_szCadrSize.y*m_szCadrSize.x*2+1024;
    pData=new BYTE[nDataSizeNew];
    nReaded=f1.Read(pData,nDataSizeNew);    //
    f1.Close();
    f1.Flush();

    TEX_INFO TexInfoStruct;
    memcpy(&TexInfoStruct,pData+16,sizeof(TEX_INFO));
    u_int16_t ConvolutionCount=TexInfoStruct.wConvolutionsCount;

    ListObj::Node* node=NULL;
    do
    {
        node=m_VoiklistData.GetFirst();
        if(node)
        {
            wxPoint* pTemp=node->GetData();
            //x=pTemp->x;
            //y=pTemp->y;
            VoikConvolution[i].nID='VOIK';
            VoikConvolution[i].wNum=i;
            VoikConvolution[i].nX=pTemp->x;
            VoikConvolution[i].nY=pTemp->y;
            VoikConvolution[i].nS=m_BigScreen->m_marker.nSize*m_BigScreen->m_marker.nSize/m_BigScreen->m_fMashtab/m_BigScreen->m_fMashtab;
            m_VoiklistData.DeleteNode(node);
            if (pTemp) delete [] pTemp;
            //node = node->GetNext();
            i++;
        }
    }
    while(node);
    TexInfoStruct.wConvolutionsCount+=nc;
    for (int sc=0; sc<v_stan.Len(); sc++)       //номер станции
        TexInfoStruct.pbtReserv2[sc]=v_stan[sc];
    TexInfoStruct.pbtReserv2[v_stan.Len()]=0;
    memcpy(pData+16,&TexInfoStruct,sizeof(TEX_INFO));
    if(nc>0 && VoikConvolution!=NULL)
    {
        memcpy(pData+256+ConvolutionCount*sizeof(CONVOLUTION),VoikConvolution,nc*sizeof(CONVOLUTION));        //
    }
    sFile.Printf(wxT("%s%d%s"),"/tmp/ForApoi",nKadr,".dat");
    wxFile f(sFile,wxFile::write);
    if(f.IsOpened())
    {
        f.Write(pData,m_szCadrSize.x*m_szCadrSize.y*2+m_TexInfoStruct.nVideoOffset);     //
        f.Close();
        f.Flush();
        nKadr++;
    }
    delete [] pData;
    if (nc>0)
    {
        WarpPointer(VoikConvolution[0].nX*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginX*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetX,
                    VoikConvolution[0].nY*m_BigScreen->m_fMashtab-m_BigScreen->m_fOriginY*m_BigScreen->m_fMashtab+m_BigScreen->m_fOffsetY);
        delete [] VoikConvolution;
    }
    ::wxSetCursor(wxCURSOR_WAIT);
    m_timerVoik1.Start(600,wxTIMER_ONE_SHOT);
}

void PKVIFrame::VoikBZ()
{
    wxString cmd;
    Base base;
    base.fun_obrb(0);                       //APOI
    extern int kpstmain();
    kpstmain();                             //KPST
    //int code=wxExecute(cmd,wxEXEC_SYNC);
}

void PKVIFrame::OnVOIK(wxCommandEvent& event)
{
//    m_bVoik=!m_bVoik;
    if (m_bVoik!=0 && !m_bInit)
    {
        if (m_timerVoik.IsRunning())
            m_timerVoik.Stop();
        VoikDlgProc();
        m_bVoikTimer=900;
        m_timerVoik.Start(m_bVoikTimer,false);
    }
    /*
        else
        {
            m_timerVoik.Stop();
            ::close(voik_pipe[0]);
            ::close(voik_pipe[1]);
        }
    */
}

void PKVIFrame::CopyDirSt(wxString from,wxString to)
{
    if (m_bVoik==0) return;
    if (!wxDir::Exists(from)) return;
    // get the names of all files in the array
    wxArrayString files;
    wxDirTraverserSimple traverser(files);
    wxDir dir(from);
    dir.Traverse(traverser);
    wxString str,ss,sd,sr;
    int kk;

    size_t count = files.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        str.Empty();
        if (wxDir::Exists(files[n]))        //directory
        {
            str.Append(to);
            str.Append("/");
            str.Append(from);
        }
        else if (wxFile::Exists(files[n]))  //file
        {
            sr=files[n];
            sr.Replace(from,"",false);
            str.Append(to);
            str.Append(sr);
            ss=sr;/*files[n]*/;
            sd=to;         //create directories
            do
            {
                kk=ss.Find('/');
                if (kk==wxNOT_FOUND) break;
                sd+=ss.SubString(0,kk);
                ss=ss.Mid(kk+1,ss.Len()-kk-1);
                if (!wxDir::Exists(sd))
                    wxDir::Make(sd);
            }
            while(true);
            if ((wxFile::Exists(str)) && (wxFile::Exists(files[n])))
            {
                wxFile f1(files[n],wxFile::read);
                wxFile f2(str,wxFile::read);
                size_t b1Size=(long)f1.Length();
                size_t b2Size=(long)f2.Length();
                f1.Close();
                f2.Close();
                if (b1Size!=b2Size)
                    if (wxCopyFile(files[n],str,true))
                    {
                        chmod(str.ToAscii(),0666);
                    }
            }
            else if (wxFile::Exists(files[n]))
                if (wxCopyFile(files[n],str,true))
                {
                    chmod(str.ToAscii(),0666);
                }
            /*
                            wxRemoveFile(files[n]);           //

                        ss=files[n];        //удаляем папки из from
                        ss=ss.BeforeLast('/');
                        if (ss.Len()>from.Len())        //внутренняя папка
                        {
                            if (wxDir::Exists(ss))
                            {
                                wxDir dd(ss);
                                if (!dd.HasFiles("*.*"))
                                    wxDir::Remove(ss);

                            }
                        }
            */
        }
    }
}
