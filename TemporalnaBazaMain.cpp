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

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

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
    nazivBaze = "temporalna";
    DijalogPrijava dijalog(this);
    do
    {
        dijalog.ShowModal();

    }while(rezultatDijalogaPrijave==0);

    if(rezultatDijalogaPrijave==-1)
        Close();
    else
    {
        panel=new PanelPocetni(this);
        radniSizer->Add( panel, 0, wxEXPAND | wxALL, 5 );
        this->Layout();
    }

}

TemporalnaBazaFrame::~TemporalnaBazaFrame()
{
}

std::string TemporalnaBazaFrame::DohvatiNazivBaze() const
{
    return nazivBaze;
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
/*
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
        break;
        case buttPanelStanja: p=dynamic_cast<PanelStanja*>(panel=new PanelStanja(this,connString));
        break;
        case buttPanelAudit: p=dynamic_cast<PanelAudit*>(panel=new PanelAudit(this,connString));
    }


    radniSizer->Add( panel, 0, wxEXPAND | wxALL, 5 );
    //this->SetSizer( radniSizer );
    this->Layout();
}
*/

void TemporalnaBazaFrame::AktivirajPanel( wxCommandEvent& event )
{
    IPanel *p;
    wxWindowID id;
    int id2;

    if(panel != nullptr)
        panel -> Destroy();

    if(event.GetEventType()==wxEVT_BUTTON)
    {
        id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
        switch(id)
        {
            case buttPanelDobavljaci: p=dynamic_cast<PanelDobavljaci*>(panel=new PanelDobavljaci(this,connString));
            break;
            case buttPanelSkladista: p=dynamic_cast<PanelSkladista*>(panel=new PanelSkladista(this,connString));
            break;
            case buttPanelMaterijali: p=dynamic_cast<PanelMaterijali*>(panel=new PanelMaterijali(this,connString));
            break;
            case buttPanelStanja: p=dynamic_cast<PanelStanja*>(panel=new PanelStanja(this,connString));
            break;
            case buttPanelAudit: p=dynamic_cast<PanelAudit*>(panel=new PanelAudit(this,connString));
        }
    }
    else if(event.GetEventType()==wxEVT_MENU)
    {
        id2=event.GetId();
        switch(id2)
        {
            case mitPanelDobavljaci: p=dynamic_cast<PanelDobavljaci*>(panel=new PanelDobavljaci(this,connString));
            break;
            case mitPanelSkladista: p=dynamic_cast<PanelSkladista*>(panel=new PanelSkladista(this,connString));
            break;
            case mitPanelMaterijali: p=dynamic_cast<PanelMaterijali*>(panel=new PanelMaterijali(this,connString));
            break;
            case mitPanelStanja: p=dynamic_cast<PanelStanja*>(panel=new PanelStanja(this,connString));
            break;
            case mitPanelRevizija: p=dynamic_cast<PanelAudit*>(panel=new PanelAudit(this,connString));
        }
    }
    else
        return;

        radniSizer->Add( panel, 0, wxEXPAND | wxALL, 5 );
        this->Layout();

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

kreiranje baze iz skripte:
createdb temporalna -O epc -p 5432 && psql temporalna < temporalna.sql

dump baze u skriptu:
pq_dump temporalna > temporalna.sql

brisanje baze:
dropdb temporalna
*/

short TemporalnaBazaFrame::CreateConnString(const char korisnik[20], const char lozinka[20])
{
    std::unique_ptr<pqxx::connection_base> poveznica; //samo C++ 14, pametni pointer
    pqxx::result r;
    int v;
    connString.clear();
    connString.append("user = ");
    connString.append(korisnik);
    connString.append(" password = ");
    connString.append(lozinka);
    connString.append(" dbname = "+nazivBaze);
    connString.append(" host = 127.0.0.1 port = 5432");

    //connString = "dbname = temporalna user = korisnik password = kor1";
    if(DEBUG_TEST)
        std::cout << "Conn string: " << connString << std::endl;

    /* samo za testiranje bez korisnika */
    //pqxx::connection poveznica("dbname=temporalna");

    try
    {
        //samo za C++14
        poveznica = std::make_unique<pqxx::connection>(connString);
        poveznica->set_client_encoding("UTF8");
    }
    catch(...)
    {
        if(DEBUG_TEST)
            std::cout << "Povezivanje sa bazom nije uspjelo: " << std::endl;
        wxMessageDialog dijalog(this,wxT("Neuspješno spajanje na bazu podataka.\nŽelite li pokušati ponovno unijeti korisničko ime i loziku?."),
                                wxT("Spajanje na bazu podataka"),  wxYES_NO |  wxICON_QUESTION);
        dijalog.SetYesNoLabels(_("&Da"),_("&Ne"));
        v=dijalog.ShowModal();
        if(v==wxID_YES)
            return 0;
        else
            return -1;
    }
    if(poveznica->is_open())
    {

        if(DEBUG_TEST)
            std::cout << "Uspjesno povezana baza \"" << poveznica->dbname() << "\" od strane korisnika " << poveznica->username() << std::endl;
        pqxx::work txn(*poveznica);
        try
        {
            r = txn.exec("SELECT current_user as korisnik");
            txn.commit();
            if(DEBUG_TEST)
                for (pqxx::result::const_iterator row = r.begin(); row != r.end(); ++row)
                    std::cout << "Korisnik: " << row["korisnik"].c_str() << std::endl;
        }
        catch(const pqxx::sql_error& e)
        {
            txn.abort();
            if(DEBUG_TEST)
                std::cout << "Nije uspjelo dohvacanje korisnika: " << e.what() << std::endl;
            wxMessageDialog dijalog(this,wxT("Nije uspjelo dohvaćanje korisnika.\nAplikacija će se prekinuti."),
                                    wxT("Spajanje na bazu podataka"),  wxOK |  wxICON_ERROR);
            dijalog.SetOKLabel(_("&U redu"));
            v=dijalog.ShowModal();
            return -1;
        }
        return 1;
    }
    return -1;
}



/* Dijalog prijava */
DijalogPrijava::DijalogPrijava(TemporalnaBazaFrame* parent):dlgPrijava(NULL)
{
    tbFrame = parent;
    txtNazivBaze->SetValue(parent->DohvatiNazivBaze());
    rezultatPrijave=-1;
}

DijalogPrijava::~DijalogPrijava()
{

}
void DijalogPrijava::PrijavaDijalogZatvoren( wxCloseEvent& event )
{
    tbFrame->PrimiRezultatDijalogaPrijave(rezultatPrijave);
    EndModal(wxID_CANCEL);
}

void DijalogPrijava::GumbPritisnut( wxCommandEvent& event )
{
    int id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    if(id==prijavaPrihvati)
        rezultatPrijave=tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    Close();
}

void DijalogPrijava::OnEnter( wxCommandEvent& event )
{
    rezultatPrijave=tbFrame->CreateConnString(txtCtrlKorisnik->GetValue(), txtCtrlLozinka->GetValue());
    Close();
}
