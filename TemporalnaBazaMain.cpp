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
    statusBar->SetStatusText(_("Projekt iz kolegija TBP"), 0);
    statusBar->SetStatusText("GUI: "+wxbuildinfo(short_f), 1);
    statusBar->SetStatusText(wxT("Autor: Elvis Popović"),2);
#endif

    panel = nullptr;

    dijalog = new DijalogPrijava(this);
    dijalog->Show();

    panel=new PanelPocetni(this);
    radniSizer->Add( panel, 0, wxEXPAND | wxALL, 5 );

    this->Layout();
}

TemporalnaBazaFrame::~TemporalnaBazaFrame()
{
    delete dijalog;
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
    wxString msg = wxString("Informacije o kompilaciji:\n") +
    wxT("C++ kompiler: GCC-5.1.0-TDM64 MinGW\n")
    wxT("Baza podataka: PostgreSQL\n")+
    wxT("Grafički sustav: ")+ wxbuildinfo(long_f)+ "\n" +
    wxT("Projektna aplikacija iz kolegija \"Teorija baza podataka\"\nAutor: Elvis Popović, 2018.");
    wxMessageBox(msg, _("Informacije"));
}

void TemporalnaBazaFrame::AktivirajPanel( wxCommandEvent& event )
{
    IPanel *p;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    if(panel != nullptr)
        panel -> Destroy();

    switch(id)
    {
        case buttPanelDobavljaci: p=dynamic_cast<PanelDobavljaci*>(panel=new PanelDobavljaci(this,connString));
        break;
        case buttPanelSkladista: p=dynamic_cast<PanelSkladista*>(panel=new PanelSkladista(this,connString));
        break;
    }


    radniSizer->Add( panel, 0, wxEXPAND | wxALL, 5 );
    this->Layout();

    std::cout << "Aktiviraj panel" << std::endl;


}

/*
prvi puta ulazak u postgres: sudo -u postgres psql postgres
postavljanje njegovog passworda: \password postgres


Postaviti u /etc/postgresql/9.5/main/pg_hba.conf
local   all  all  md5
sudo service postgresql restart
Za korisnika: alter role epc with password 'epc1'

Logiranje: psql -U postgres -W
Logiranje korisnika: psql -U korisnik -h 127.0.0.1 temporalna
*/

void TemporalnaBazaFrame::CreateConnString(const char korisnik[20], const char lozinka[20])
{
    pqxx::result r;

    connString.clear();
    connString.append("user = ");
    connString.append(korisnik);
    connString.append(" password = ");
    connString.append(lozinka);
    connString.append(" dbname = temporalna");
    connString.append(" host = 127.0.0.1 port = 5432");

    //connString = "dbname = temporalna user = korisnik password = kor1";
    std::cout << "Conn string: " << connString << std::endl;
;

    /* samo za testiranje */
    //pqxx::connection poveznica("dbname=temporalna");
    pqxx::connection poveznica(connString);


    if(poveznica.is_open())
    {
        std::cout << "Uspjesno povezana baza: " << poveznica.dbname() << std::endl;
    }
    else
    {
        std::cout << "Povezivanje sa bazom nije uspjelo." << std::endl;
        return;
    }

    pqxx::work txn(poveznica);
    r = txn.exec("SELECT current_user as korisnik");
    txn.commit();
    poveznica.disconnect();
    for (pqxx::result::const_iterator row = r.begin(); row != r.end(); ++row)
        std::cout << "Korisnik: " << row["korisnik"].c_str() << std::endl;
}



/* Dijalog prijava */
DijalogPrijava::DijalogPrijava(wxWindow* parent):dlgPrijava(parent)
{
    tbFrame = (TemporalnaBazaFrame*)parent;

}

DijalogPrijava::~DijalogPrijava()
{

}

void DijalogPrijava::PrijavaDijalogZatvoren( wxCloseEvent& event )
{
    tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    Destroy();
}

void DijalogPrijava::GumbPritisnut( wxCommandEvent& event )
{
    int id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    if(id==prijavaPrihvati) tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    Destroy();
}

void DijalogPrijava::OnEnter( wxCommandEvent& event )
{
    tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    Destroy();
}
