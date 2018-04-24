#include "PanelStanja.h"

PanelStanja::PanelStanja(wxFrame *frame, std::string connString) : GUIPanelStanja(frame)
{
    poveznica=new pqxx::connection(connString);
    poveznica->set_client_encoding("UTF8");
    modelMaterijala = new MaterijaliModel(this);
    dvcMaterijali->AssociateModel(modelMaterijala);

    tablicaSkladista->DeleteAllItems();
    tablicaSkladista->ClearColumns();
    for(int i=0; i<3; i++)
        tablicaSkladista->AppendTextColumn(SKLADISTA_ZAGLAVLJA2[i]);
    panelStranicenje=new PanelStranicenje(frame, this);
    stranica=0;
    osvjeziCombo();

    stranicenjeSizer->Add( panelStranicenje, 0, wxALL, 5 );
    this->Layout();
    btnAzuriraj->Disable();
    btnDodaj->SetLabel(L"Dodaj");
    btnDodaj->Disable();
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
            tablicaSkladista->AppendTextColumn(SKLADISTA_ZAGLAVLJA2[i]);
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
            tablicaSkladista->AppendTextColumn(SKLADISTA_ZAGLAVLJA2[i]);
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
    dvcMaterijali->AssociateModel(NULL);
    modelMaterijala->Obrisi();
    dvcMaterijali->AssociateModel(modelMaterijala);
    modelMaterijala->Postavi(true);
}
void PanelStanja::SkladisteSelPromijenjena( wxDataViewEvent& event )
{
    int i;
    wxVariant skladisteId, skladiste_vrijeme;

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

void PanelStanja::MaterijalSelPromijenjena( wxDataViewEvent& event )
{
    int i;
    pqxx::result stanja;
    wxDataViewItem item = event.GetItem();
    wxVariant skladisteId;
    if((i=tablicaSkladista->GetSelectedRow())==wxNOT_FOUND)
        return;
    tablicaSkladista->GetValue(skladisteId,i,0);
    if(!item.IsOk()||skladisteId.GetLong()==0)
        return;
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    if(cvor->DajVrstu()==VrstaMaterijala::MATERIJALI)
    {
        btnAzuriraj->Disable();
        btnDodaj->Disable();
        return;
    }
    btnAzuriraj->SetLabel(L"Ažuriraj");
    btnAzuriraj->Enable();
    btnDodaj->SetLabel(L"Obriši");
    if(cvor->DajId()==0)
    {
        pqxx::work txn(*poveznica);
        try
        {
            r = txn.exec("SELECT COUNT(*) AS broj FROM stanje WHERE skladiste="+txn.quote(skladisteId.GetLong())+" AND materijal="+txn.quote(cvor->DajId()));
            txn.commit();
            if(!r.empty())
                btnAzuriraj->SetLabel(L"Dodaj");
            else
                btnAzuriraj->SetLabel(L"Ažuriraj");
        }
        catch (const pqxx::sql_error& e)
        {
            txn.abort();
            wxMessageDialog dijalog(NULL,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Dohvaćanje broja zapisa iz tablice stanja..."),  wxOK |  wxICON_ERROR);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
        }
        btnDodaj->Disable();
    }
    else
        btnDodaj->Enable();
}

void PanelStanja::PoziviDijalogUnosa( wxCommandEvent& event )
{
    int i;
    wxVariant vrijednost;
    wxVector<wxVariant> redak;
    wxVariant skladisteId, skladisteNaziv;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    wxDataViewItem item = dvcMaterijali->GetCurrentItem();
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    if (!cvor)
        return;
    if((i=tablicaSkladista->GetSelectedRow())==wxNOT_FOUND)
        return;
    tablicaSkladista->GetValue(skladisteId,i,0);
    tablicaSkladista->GetValue(skladisteNaziv,i,1);
    if(skladisteId.GetLong()==0)
        return;

    redak.push_back(cvor->DajId());
    redak.push_back(cvor->DajNaziv());
    redak.push_back(skladisteId);
    redak.push_back(skladisteNaziv);
    switch(id)
    {
        case IDstanjaDodaj: //brisanje
            if(cvor->DajVrijeme_do()!="infinity")
            {
                wxMessageDialog dijalog(this,wxT("Materijal je neaktivan. Nije moguća manipulacija brisanja toj. deaktiviranja.\n"),
                                wxT("Brisanje materijala"),  wxOK |  wxICON_ERROR);
                dijalog.SetOKLabel(wxT("&U redu"));
                dijalog.ShowModal();
            }
            else
            {
                wxMessageDialog dijalog(this,wxT("Materijal će biti postavljen na neaktivan, tj. uvjetno obrisan.\nŽelite li nastaviti?"),
                                wxT("Brisanje materijala"),  wxYES_NO |  wxICON_ERROR);            dijalog.SetYesNoLabels(wxT("&Da"),wxT("&Ne"));
                if(dijalog.ShowModal()!=wxID_NO)
                {
                    ;
                }
            }

            break;
        case IDstanjaAzuriraj:
            if(cvor->DajId()==0) //dodavanje stanja
            {
                DijalogUnosStanja dlg(this,redak,TipPromjene::DODAVANJE, cvor->DajVrstu());
                dlg.ShowModal();
            }
            else //azuriranje
            {
                if(cvor->DajVrijeme_do()!="infinity")
                {
                    DijalogUnosStanja dlg(this,redak,TipPromjene::REAKTIVACIJA, cvor->DajVrstu());
                    dlg.ShowModal();
                }
                else
                {
                    DijalogUnosStanja dlg(this,redak,TipPromjene::AZURIRANJE, cvor->DajVrstu());
                    dlg.ShowModal();
                }
            }
            break;
    }


}

pqxx::result PanelStanja::DohvatiMaterijale(char vrsta)
{
    pqxx::result materijali;
    if(vrsta<1||vrsta>4)
        return materijali;
    pqxx::work txn(*poveznica);
    try
    {
        materijali = txn.exec("SELECT id,naziv FROM "+txn.esc(MATERIJALI_TABLICE[vrsta-1])+" WHERE vrijeme_do='infinity'::TIMESTAMP ORDER BY id");
        txn.commit();
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(NULL,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Dohvaćanje materijala za combo..."),  wxOK |  wxICON_ERROR);
        dijalog.SetOKLabel(wxT("&U redu"));
        dijalog.ShowModal();
    }
    return materijali;
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

/********************************************************************************/
/********************** Dijalog unos ********************************************/
/********************************************************************************/

DijalogUnosStanja :: DijalogUnosStanja(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, VrstaMaterijala vrstaMaterijala) : dlgUnosStanja(NULL)
{
    wxVector<wxVariant>::iterator it;
    this->parent=(PanelStanja *)parent;
    this->tp = tp;
    for(it=redak.begin(); it!=redak.end(); ++it)
        this->redak.push_back(*it);
    this->vrstaMaterijala = vrstaMaterijala;
}

DijalogUnosStanja :: ~DijalogUnosStanja()
{

}

void DijalogUnosStanja :: OnInit( wxInitDialogEvent& event )
{
    int dlgRes;
    pqxx::result materijali;
    if(!redak.empty())
    {
        lblImeSkladista->SetLabel(redak[3].GetString());
        if(redak[0].GetString()=="0")
            lblImeMaterijala->SetLabel(" - ");
        else
            lblImeMaterijala->SetLabel(redak[1].GetString());
    }
    this->SetLabel(wxString(MATERIJALI_VRSTE[vrstaMaterijala])+wxT(" - stanje na skladištu"));
    comboMaterijali->SetValue(wxT("Izbor materijala"));
    if(tp==TipPromjene::DODAVANJE)
    {
        materijali = parent->DohvatiMaterijale(vrstaMaterijala);
        for(pqxx::result::const_iterator matIt = materijali.begin(); matIt !=materijali.end(); ++matIt)
            comboMaterijali->Append(wxString::Format("%4ld | %s",wxVariant(matIt["id"].c_str()).GetInteger(),wxString::FromUTF8(matIt["naziv"].c_str())));
    }
    else
        comboMaterijali->Disable();
}
void DijalogUnosStanja :: OnCombo( wxCommandEvent& event )
{
    wxString ispisMaterijala;
    wxComboBox *combo = wxDynamicCast(event.GetEventObject(),wxComboBox);
    wxStringTokenizer tokenizer(combo->GetValue(), "|");
    tokenizer.GetNextToken().ToLong(&materijalId);
    ispisMaterijala.Printf("id: %ld, ",materijalId);
    ispisMaterijala.Append(tokenizer.GetNextToken());
    lblImeMaterijala->SetLabel(ispisMaterijala);
}

void DijalogUnosStanja :: Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}

void DijalogUnosStanja :: GumbPritisnut( wxCommandEvent& event )
{
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    Destroy();
}
