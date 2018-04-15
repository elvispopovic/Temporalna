#include "PanelStanja.h"

wchar_t skladista_zaglavlja2[][20]={L"Šifra (id)",L"Oznaka",L"Vrijeme od"};

PanelStanja::PanelStanja(wxFrame *frame, std::string connString) : GUIPanelStanja(frame)
{
    poveznica=new pqxx::connection(connString);
    poveznica->set_client_encoding("UTF8");
    modelMaterijala = new MaterijaliModel(this);
    dvcMaterijali->AssociateModel(modelMaterijala);

    tablicaSkladista->DeleteAllItems();
    tablicaSkladista->ClearColumns();
    for(int i=0; i<3; i++)
        tablicaSkladista->AppendTextColumn(skladista_zaglavlja2[i]);
    panelStranicenje=new PanelStranicenje(frame, this);
    stranica=0;
    osvjeziCombo();

    stranicenjeSizer->Add( panelStranicenje, 0, wxALL, 5 );
    this->Layout();
}

PanelStanja::~PanelStanja()
{
    poveznica->disconnect();
    delete poveznica;
    dvcMaterijali->AssociateModel(NULL);
    delete modelMaterijala;
}

void PanelStanja::osvjeziCombo()
{
    pqxx::result naziv;
    comboFilter->Clear();
    comboFilter->Append("Aktivni");
    comboFilter->SetSelection(0);
    pqxx::work txn(*poveznica);
    razlicitiId = txn.exec("SELECT DISTINCT id FROM skladista ORDER BY id");
    if(!razlicitiId.empty())
        for(pqxx::result::const_iterator redak = razlicitiId.begin(); redak !=razlicitiId.end(); ++redak)
        {
            naziv = txn.exec("SELECT id || ' | ' || oznaka FROM skladista WHERE id="+txn.esc(redak[0].c_str()) +" ORDER BY vrijeme_od DESC LIMIT 1");
            comboFilter->Append(wxString::FromUTF8(naziv[0][0].c_str()));
        }
    txn.commit();
    wxCommandEvent emptyEvent;
    OnCombo(emptyEvent);
}

void PanelStanja::OnCombo( wxCommandEvent& event )
{
    int i;
    wxString id;
    pqxx::work txn(*poveznica);
    tablicaSkladista->UnselectAll();
    tablicaSkladista->DeleteAllItems();
    tablicaSkladista->ClearColumns();
    if(comboFilter->GetSelection()==0) //aktivni
    {
        for(i=0; i<2; i++)
            tablicaSkladista->AppendTextColumn(skladista_zaglavlja2[i]);
        if(stranica*VELICINA_STRANICE>comboFilter->GetCount())
            stranica=0;
        if(panelStranicenje)
            if(razlicitiId.size()>0)
                panelStranicenje->PostaviStranice((razlicitiId.size()-1)/VELICINA_STRANICE+1,stranica);
            else
                panelStranicenje->PostaviStranice(1,stranica);
        for(i=stranica*VELICINA_STRANICE; i<(stranica+1)*VELICINA_STRANICE; i++)
        {
            r = txn.exec("SELECT id,oznaka FROM skladista WHERE id="+txn.quote(razlicitiId[i][0].c_str())+
                         " AND vrijeme_do='infinity'::TIMESTAMP ORDER BY id");
            upisiRetke(r);
        }
    }
    else //neaktivni
    {
        wxStringTokenizer tokenizer(comboFilter->GetValue(), "|");
        id = tokenizer.GetNextToken();
        r = txn.exec("SELECT COUNT(id) as broj FROM skladista WHERE id="+txn.esc(id));
        for(i=0; i<3; i++)
            tablicaSkladista->AppendTextColumn(skladista_zaglavlja2[i]);
        if(stranica*VELICINA_STRANICE>r[0][0].as<int>())
            stranica=0;
        if(panelStranicenje)
            if(razlicitiId.size()>0)
                panelStranicenje->PostaviStranice((r[0][0].as<int>()-1)/VELICINA_STRANICE+1,stranica);
            else
                panelStranicenje->PostaviStranice(1,stranica);
        r = txn.exec("SELECT id,oznaka,vrijeme_od FROM skladista WHERE id="+
                     txn.esc(id)+" ORDER BY vrijeme_od ASC LIMIT "+txn.quote(VELICINA_STRANICE)+" OFFSET "+txn.quote(stranica*VELICINA_STRANICE));
        upisiRetke(r);
    }

    txn.commit();
}
void PanelStanja::SkladisteSelPromijenjena( wxDataViewEvent& event )
{
    int i;
    wxVariant skladisteId, skladiste_vrijeme;
    wxDataViewItem redak;
    if((i=tablicaSkladista->GetSelectedRow())==wxNOT_FOUND)
        return;
    tablicaSkladista->GetValue(skladisteId,i,0);
    pqxx::work txn(*poveznica);
    if(tablicaSkladista->GetColumnCount()<3)
    {
        r = txn.exec("SELECT vrijeme_od FROM skladista WHERE id="+txn.esc(skladisteId.GetString())+
                     " AND vrijeme_do='infinity'::TIMESTAMP");
        if(!r.empty())
            skladiste_vrijeme = r[0][0].c_str();
    }
    else
    {
        tablicaSkladista->GetValue(skladiste_vrijeme,i,2);
    }
    txn.commit();
    dvcMaterijali->AssociateModel(NULL);
    modelMaterijala->Obrisi();
    dvcMaterijali->AssociateModel(modelMaterijala);
    modelMaterijala->PostaviZaSkladiste(skladisteId.GetInteger(), skladiste_vrijeme.GetString(), true);
}


void PanelStanja::upisiRetke(pqxx::result r)
{
    wxVector<wxVariant> redak;
    for(pqxx::result::const_iterator red = r.begin(); red !=r.end(); ++red)
    {
        redak.clear();
        for(pqxx::tuple::iterator celija = red.begin(); celija!=red.end(); ++celija)
            redak.push_back(wxVariant(wxString::FromUTF8(celija.c_str())));
        tablicaSkladista->AppendItem(redak);
    }
}

void PanelStanja::AzurirajBazu(wxVector<wxVariant> redak)
{

}

void PanelStanja::DopuniBazu(wxVector<wxVariant> redak)
{

}

bool PanelStanja::PrimiSQLZahtijev(wxString sqlString)
{

}

void PanelStanja::PostaviStranicu(int stranica)
{
    this->stranica=stranica;
    wxCommandEvent emptyEvent;
    OnCombo(emptyEvent);
}

pqxx::connection* PanelStanja::DajPoveznicu()
{
    return poveznica;
}
