/***************************************************************
 * Name:      TemporalnaBazaMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    EPC (elvpopovi@foi.hr)
 * Created:   2018-02-20
 * Copyright: EPC (www.nema_web_site_niti_ce_ga_ikada_imati.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "TemporalnaBazaMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
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


TemporalnaBazaFrame::TemporalnaBazaFrame(wxFrame *frame)
    : GUIFrame(frame)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("Hello Code::Blocks user!"), 0);
    statusBar->SetStatusText(wxbuildinfo(short_f), 1);
#endif
}

TemporalnaBazaFrame::~TemporalnaBazaFrame()
{
}

void TemporalnaBazaFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void TemporalnaBazaFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void TemporalnaBazaFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
