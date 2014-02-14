/***************************************************************
 * Name:      wx_pch.h
 * Purpose:   Header to create Pre-Compiled Header (PCH)
 * Author:     ()
 * Created:   2010-03-22
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED

// basic wxWidgets headers
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef WX_PRECOMP
   #include  <wx/display.h>
#endif // WX_PRECOMP
#include <wx/sizer.h>
#include <wx/app.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/snglinst.h>
#include <wx/utils.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/listctrl.h>
#include <wx/config.h>
#include <wx/msgdlg.h>
#include <wx/listimpl.cpp>
#include <wx/arrimpl.cpp>
#include <wx/tokenzr.h>
#include <wx/mstream.h>
//#include <wx/dynarray.h>
//#include <wx/arrimpl.cpp>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/valgen.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/arrstr.h>
#include <wx/stdpaths.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/progdlg.h>
#include <wx/bookctrl.h>
#include <wx/propdlg.h>
#include <wx/datectrl.h>
#include <wx/filedlg.h>
#include <wx/laywin.h>
#include <wx/image.h>
#include <wx/dir.h>
#include <wx/filefn.h>

#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <sys/io.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <search.h>
//#include <signal.h>


//#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <asm/socket.h>
#include <net/if_arp.h>
#include <GL/glu.h>

//#include <pfring.h>
//#include <pcap.h>

//(*InternalHeaders(PKVIFrame)
//#include <wx/settings.h>
//#include <wx/font.h>
//#include <wx/intl.h>
//#include <wx/string.h>
//*)

#endif // WX_PCH_H_INCLUDED
