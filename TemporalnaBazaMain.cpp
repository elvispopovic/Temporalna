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
    do
    {
        dijalog = new DijalogPrijava(this);
        dijalog->ShowModal();
    }while(rezultatDijalogaPrijave==0);
    if(rezultatDijalogaPrijave==-1)
        Destroy();
    else
    {
        panel=new PanelPocetni(this);
        radniSizer->Add( panel, 0, wxEXPAND | wxALL, 5 );
        this->Layout();
    }
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
        case buttPanelMaterijali: p=dynamic_cast<PanelMaterijali*>(panel=new PanelMaterijali(this,connString));
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

short TemporalnaBazaFrame::CreateConnString(const char korisnik[20], const char lozinka[20])
{
    pqxx::result r;
    int v;
    connString.clear();
    connString.append("user = ");
    connString.append(korisnik);
    connString.append(" password = ");
    connString.append(lozinka);
    connString.append(" dbname = temporalna");
    connString.append(" host = 127.0.0.1 port = 5432");

    //connString = "dbname = temporalna user = korisnik password = kor1";
    std::cout << "Conn string: " << connString << std::endl;

    /* samo za testiranje */
    //pqxx::connection poveznica("dbname=temporalna");
    try
    {
        pqxx::connection poveznica(connString);
        if(poveznica.is_open())
        {
            std::cout << "Uspjesno povezana baza: " << poveznica.dbname() << std::endl;
                pqxx::work txn(poveznica);
            r = txn.exec("SELECT current_user as korisnik");
            txn.commit();
            poveznica.disconnect();
            for (pqxx::result::const_iterator row = r.begin(); row != r.end(); ++row)
                std::cout << "Korisnik: " << row["korisnik"].c_str() << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Povezivanje sa bazom nije uspjelo." << std::endl;
            wxMessageDialog dijalog(this,wxT("Greška prilikom unosa korisničkog imena i lozinke."),
                                    wxT("Spajanje na bazu podataka"),  wxYES_NO |  wxICON_EXCLAMATION);
            dijalog.SetYesNoLabels(_("&Da"),_("&Ne"));
            v=dijalog.ShowModal();
        if(v==wxID_YES)
            return 0;
        else
            return -1;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Povezivanje sa bazom nije uspjelo." << std::endl;
        wxMessageDialog dijalog(this,wxT("Neuspješno spajanje na bazu podataka.\nŽelite li pokušati ponovno unijeti korisničko ime i loziku?."),
                                    wxT("Spajanje na bazu podataka"),  wxYES_NO |  wxICON_QUESTION);
        dijalog.SetYesNoLabels(_("&Da"),_("&Ne"));
        v=dijalog.ShowModal();
        if(v==wxID_YES)
        {
            std::cout << "dijalog 0" << std::endl;
            return 0;

        }
        else
        {
            std::cout << "dijalog -1" << std::endl;
            return -1;
        }
    }
}



/* Dijalog prijava */
DijalogPrijava::DijalogPrijava(TemporalnaBazaFrame* parent):dlgPrijava(parent)
{
    tbFrame = parent;
}

DijalogPrijava::~DijalogPrijava()
{

}

void DijalogPrijava::PrijavaDijalogZatvoren( wxCloseEvent& event )
{
    //tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    tbFrame->PosaljiRezultatDijalogaPrijave(-1);
    Destroy();
}

void DijalogPrijava::GumbPritisnut( wxCommandEvent& event )
{
    short rezultat=-1;
    int id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    if(id==prijavaPrihvati)
        rezultat=tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    tbFrame->PosaljiRezultatDijalogaPrijave(rezultat);
    Destroy();
}

void DijalogPrijava::OnEnter( wxCommandEvent& event )
{
    short rezultat=-1;
    rezultat=tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    tbFrame->PosaljiRezultatDijalogaPrijave(rezultat);
    Destroy();
}
