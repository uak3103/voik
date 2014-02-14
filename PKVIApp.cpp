/***************************************************************
 * Name:      PKVIApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2010-03-22
 * Copyright:  ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "PKVIApp.h"

//(*AppHeaders
#include "PKVIMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PKVIApp);

BEGIN_EVENT_TABLE(PKVIApp,wxApp)
	EVT_END_SESSION(PKVIApp::OnEndSession)
END_EVENT_TABLE()

bool PKVIApp::OnInit()
{
//    signal(SIGPIPE,SIG_IGN);
//	struct sigaction sa;
//	sa.sa_handler=SIG_IGN;
//    sigaction(SIGINT,&sa,0);
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    PKVIFrame* Frame = new PKVIFrame(0);
    Frame->Show();
    SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}

int PKVIApp::OnExit()
{
//	wxString s=wxStandardPaths::Get().GetUserConfigDir()+"/OnExit.bin";
//
//	wxFile f(s,wxFile::write);
//    GetTopWindow()->Close();
    return wxApp::OnExit();
}

void PKVIApp::OnEndSession(wxCloseEvent& event)
{
//   	wxString s=wxStandardPaths::Get().GetUserConfigDir()+"/OnEndSess.bin";
//
//	wxFile f(s,wxFile::write);
//
//    int a=0;
}

