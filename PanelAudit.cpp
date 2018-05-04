#include "PanelAudit.h"

PanelAudit::PanelAudit(wxFrame *frame, std::string connString) : GUIPanelAudit(frame)
{
    wxVector<wxVariant> redak;
    poveznica=new pqxx::connection(connString);
    poveznica->set_client_encoding("UTF8");

    tablicaRevizija->DeleteAllItems();
    panelStranicenje=new PanelStranicenje(frame, this);
    stranica=0;
    osvjezi();
    stranicenjeSizer->Add( panelStranicenje, 0, wxALL, 5 );
    this->Layout();
}

PanelAudit::~PanelAudit()
{
    poveznica->disconnect();
    delete panelStranicenje;
    delete poveznica;
}

void PanelAudit::osvjezi()
{
    pqxx::result r, audit;
    pqxx::work txn(*poveznica);
    tablicaRevizija->DeleteAllItems();
    try
    {
        r = txn.exec("SELECT COUNT(id) as broj FROM audit");
        if(stranica*VELICINA_STRANICE>r[0][0].as<int>())
            stranica=0;
        if(panelStranicenje)
            panelStranicenje->PostaviStranice((r[0][0].as<int>()-1)/VELICINA_STRANICE+1,stranica);
        audit = txn.exec("SELECT id,vrijeme,tablica,id_tablice,id_tablice2,dogadjaj,korisnik FROM audit\
 ORDER BY vrijeme ASC LIMIT "+txn.quote(VELICINA_STRANICE)+" OFFSET "+txn.quote(stranica*VELICINA_STRANICE));
        txn.commit();
        upisiRetke(audit);
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(NULL,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Dohvaćanje revizije..."),  wxOK |  wxICON_ERROR);
        dijalog.SetOKLabel(wxT("&U redu"));
        dijalog.ShowModal();
        return;
    }
}

void PanelAudit::AzurirajBazu(wxVector<wxVariant> redak)
{

}

void PanelAudit::DopuniBazu(wxVector<wxVariant> redak)
{

}

void PanelAudit::upisiRetke(pqxx::result r)
{
    wxVector<wxVariant> redak;
    for(pqxx::result::const_iterator red = r.begin(); red !=r.end(); ++red)
    {
        redak.clear();
        for(pqxx::tuple::iterator celija = red.begin(); celija!=red.end(); ++celija)
            redak.push_back(wxVariant(wxString::FromUTF8(celija.c_str())));
        tablicaRevizija->AppendItem(redak);
    }

}

void PanelAudit::PostaviStranicu(int stranica)
{
    this->stranica=stranica;
    osvjezi();
}

pqxx::connection* PanelAudit::DajPoveznicu()
{
    return poveznica;
}


