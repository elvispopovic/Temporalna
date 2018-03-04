/***************************************************************
 * Name:      TestWx2Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    EPC (elvpopovi@foi.hr)
 * Created:   2018-02-21
 * Copyright: EPC (www.nema_web_site_niti_ce_ga_ikada_imati.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "TestWx2Main.h"

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
#endif // wxUSE_UNICODEYou are not alone
    }

    return wxbuild;
}


TestWx2Frame::TestWx2Frame(wxFrame *frame)
    : GUIFrame(frame)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("Hello Code::Blocks user!"), 0);
    statusBar->SetStatusText(wxbuildinfo(short_f), 1);
#endif


    modelStabla = new ModelStabla;

    dwKontrola->AssociateModel(modelStabla);

    txtIspis->Clear();

}

TestWx2Frame::~TestWx2Frame()
{
}

void TestWx2Frame::PromjenaSelekcije( wxDataViewEvent& event )
{
    wxString ispis;
    wxDataViewItem item = event.GetItem();
    ispis.Append(wxString::Format(wxT("Izabrano... Naziv: %s"),modelStabla->DajNaziv(item)));
    if(!(modelStabla->DajOznaku(item)).empty())
        ispis.Append(wxString::Format(wxT(", oznaka: %s"),modelStabla->DajOznaku(item)));
    if(modelStabla->DajVrijednost(item)!=-1)
        ispis.Append(wxString::Format(wxT(", vrijednost: %d"),modelStabla->DajVrijednost(item)));
    if(!(modelStabla->DajNapomenu(item)).empty())
        ispis.Append(wxString::Format(wxT(", napomena: %s"),modelStabla->DajNapomenu(item)));
    ispis.Append("\n");
    txtIspis->AppendText(ispis);
}

void TestWx2Frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void TestWx2Frame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void TestWx2Frame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
