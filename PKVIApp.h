/***************************************************************
 * Name:      PKVIApp.h
 * Purpose:   Defines Application Class
 * Author:     ()
 * Created:   2010-03-22
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef PKVIAPP_H
#define PKVIAPP_H

#include <wx/app.h>

class PKVIApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

    void OnEndSession(wxCloseEvent& event);
   	DECLARE_EVENT_TABLE()

};

#endif // PKVIAPP_H
