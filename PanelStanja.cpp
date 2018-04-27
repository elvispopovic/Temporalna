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
    modelMaterijala->PostaviZaSkladiste(skladisteId.GetInteger(), skladiste_vrijeme.GetString(), chkPrikaziNeaktivne->IsChecked());
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
                    pqxx::work txn(*poveznica);
                    try
                    {
                        txn.exec("DELETE from stanje WHERE materijal="+txn.quote(cvor->DajId())+" AND skladiste="+txn.quote(skladisteId.GetLong()));
                        txn.commit();
                    }
                    catch (const pqxx::sql_error& e)
                    {
                        txn.abort();
                    }
                wxCommandEvent emptyEvent;
                OnCombo(emptyEvent);
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

void PanelStanja::NeaktivniChecked( wxCommandEvent& event )
{
    wxDataViewEvent emptyEvent;
    SkladisteSelPromijenjena(emptyEvent);
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
pqxx::result PanelStanja::DohvatiMjere()
{
    pqxx::result mjere;
    pqxx::work txn(*poveznica);
    try
    {
        mjere = txn.exec("SELECT id, mjera, skraceno FROM mjere_kol");
        txn.commit();
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(NULL,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Dohvaćanje mjera za combo..."),  wxOK |  wxICON_ERROR);
        dijalog.SetOKLabel(wxT("&U redu"));
        dijalog.ShowModal();
    }
    return mjere;
}

pqxx::result PanelStanja::ProvjeriPostojanje(long skladisteID, long materijalID)
{
    pqxx::result stanja;
    pqxx::work txn(*poveznica);
    try
    {
        stanja = txn.exec("SELECT kolicina, mjere_kol.mjera AS mjera, biljeska, stanje.vrijeme_od as vrijeme_od, stanje.vrijeme_do as vrijeme_do \
FROM stanje LEFT JOIN mjere_kol ON stanje.mjera=mjere_kol.id WHERE stanje.skladiste="+
                              txn.quote(skladisteID)+" AND stanje.materijal="+txn.quote(materijalID)+" ORDER BY stanje.vrijeme_od DESC LIMIT 1");
        txn.commit();
        return stanja;
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(NULL,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Usporedba sa postojećim stanjima..."),  wxOK |  wxICON_ERROR);
        dijalog.SetOKLabel(wxT("&U redu"));
        dijalog.ShowModal();
        return stanja;
    }
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
    long mjera;
    wxCommandEvent prazniEvent;
    pqxx::result rezultat;
    if(redak[0].GetInteger()<=0||redak[1].GetInteger()<=0||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    try
    {
        rezultat=txn.exec("SELECT id AS mId FROM mjere_kol WHERE mjera='"+txn.esc(redak[3].GetString().ToUTF8())+"'");
        mjera=wxVariant(rezultat[0]["mId"].c_str()).GetInteger();
        if(mjera>0)
            txn.exec("UPDATE stanje SET kolicina="+txn.quote(redak[2].GetDouble())+",mjera="+txn.quote(mjera)+
                ", biljeska='"+txn.esc(redak[4].GetString().ToUTF8())+"' WHERE materijal="+txn.quote(redak[0].GetInteger())+
                     " AND skladiste="+txn.quote(redak[1].GetInteger()));
        txn.commit();
        OnCombo(prazniEvent);
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(this,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Azuriranje stanja"),  wxOK |  wxICON_ERROR);
                    dijalog.SetOKLabel(wxT("&U redu"));
                    dijalog.ShowModal();
    }
}

void PanelStanja::DopuniBazu(wxVector<wxVariant> redak)
{
    long mjera;
    wxCommandEvent prazniEvent;
    pqxx::result rezultat;
    if(redak[0].GetInteger()<=0||redak[1].GetInteger()<=0||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    try
    {
        rezultat=txn.exec("SELECT id AS mId FROM mjere_kol WHERE mjera='"+txn.esc(redak[3].GetString().ToUTF8())+"'");
        mjera=wxVariant(rezultat[0]["mId"].c_str()).GetInteger();
        if(mjera>0)
            txn.exec("INSERT INTO stanje(materijal,skladiste,kolicina,mjera,biljeska) VALUES("+
                     txn.quote(redak[0].GetInteger())+","+
                     txn.quote(redak[1].GetInteger())+","+
                     txn.quote(redak[2].GetDouble())+","+
                     txn.quote(mjera)+",'"+
                     txn.esc(redak[4].GetString().Trim().ToUTF8())+
                     "')");
        txn.commit();
        OnCombo(prazniEvent);

    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(this,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Azuriranje stanja"),  wxOK |  wxICON_ERROR);
                    dijalog.SetOKLabel(wxT("&U redu"));
                    dijalog.ShowModal();
    }
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
    comboMjera->SetValue("kol.");
    skladisteId=materijalId=0;
}

DijalogUnosStanja :: ~DijalogUnosStanja()
{

}

void DijalogUnosStanja :: OnInit( wxInitDialogEvent& event )
{
    int dlgRes;
    wxString ispisMaterijala;
    pqxx::result materijali, mjere, stanje;
    if(!redak.empty())
    {
        lblImeSkladista->SetLabel(redak[3].GetString());
        skladisteId=redak[2].GetInteger();
        if(redak[0].GetString()=="0")
        {
            lblImeMaterijala->SetLabel(" - ");
            comboMaterijali->SetValue(wxT("Izbor materijala"));
        }

        else
        {
            lblImeMaterijala->SetLabel(redak[1].GetString());
            comboMaterijali->SetValue(redak[0].GetString()+" | "+redak[1].GetString());
        }

    }
    this->SetLabel(wxString(MATERIJALI_VRSTE[vrstaMaterijala])+wxT(" - stanje na skladištu"));

    if(tp==TipPromjene::DODAVANJE)
    {
        materijali = parent->DohvatiMaterijale(vrstaMaterijala);
        for(pqxx::result::const_iterator matIt = materijali.begin(); matIt !=materijali.end(); ++matIt)
            comboMaterijali->Append(wxString::Format("%4ld | %s",wxVariant(matIt["id"].c_str()).GetInteger(),wxString::FromUTF8(matIt["naziv"].c_str())));
    }
    else
        comboMaterijali->Disable();
    mjere = parent->DohvatiMjere();
    if(!mjere.empty())
        for(pqxx::result::const_iterator mjereIt = mjere.begin(); mjereIt != mjere.end(); ++mjereIt)
            comboMjera->Append(wxString::FromUTF8(mjereIt["mjera"].c_str()));

    stanje = parent->ProvjeriPostojanje(redak[2].GetInteger(), redak[0].GetInteger());
    if(!stanje.empty())
    {
        ispisMaterijala.Printf("%ld",wxVariant(stanje[0]["kolicina"].c_str()).GetInteger());
        txtStanjeKolicina->SetValue(ispisMaterijala);
        comboMjera->SetValue( wxString::FromUTF8(stanje[0]["mjera"].c_str()) );
        ispisMaterijala.Printf(wxT("Bilješka: %ls\n"),
                               (wxString::FromUTF8(stanje[0]["biljeska"].c_str())==wxT("")?wxT(" nema bilješki"):wxString::FromUTF8(stanje[0]["biljeska"].c_str())));
        txtStanjeNapomena->SetValue(ispisMaterijala);
    }
    else
    {
        comboMjera->SetValue("kol.");
        txtStanjeKolicina->Clear();
        txtStanjeNapomena->Clear();
    }
    if(tp==TipPromjene::REAKTIVACIJA)
    {
        wxMessageDialog dijalog(this,wxT("Stanje je neaktivno (materijal na tom skladištu je izbrisan). Ovim će se postupkom reaktivirati."),
                                    wxT("Reaktivacija neaktivnog (uvjetno izbrisanog) materijala na skladištu"),  wxOK |  wxICON_INFORMATION);
        dijalog.SetOKLabel(wxT("&U redu"));
        dlgRes=dijalog.ShowModal();
    }
}
void DijalogUnosStanja :: OnCombo( wxCommandEvent& event )
{
    wxString ispisMaterijala;
    pqxx::result stanje;
    wxComboBox *combo = wxDynamicCast(event.GetEventObject(),wxComboBox);
    wxStringTokenizer tokenizer(combo->GetValue(), "|");
    tokenizer.GetNextToken().ToLong(&materijalId);
    ispisMaterijala.Printf("id: %ld, ",materijalId);
    ispisMaterijala.Append(tokenizer.GetNextToken());
    lblImeMaterijala->SetLabel(ispisMaterijala);
    stanje = parent->ProvjeriPostojanje(redak[2].GetInteger(), materijalId);
    if(!stanje.empty())
    {
        ispisMaterijala.Printf("%ld",wxVariant(stanje[0]["kolicina"].c_str()).GetInteger());
        txtStanjeKolicina->SetValue(ispisMaterijala);
        comboMjera->SetValue( wxString::FromUTF8(stanje[0]["mjera"].c_str()) );
        ispisMaterijala.Printf(wxT("Materijal već postoji na tom skladištu.\nBilješka: %ls\n"),
                               (wxString::FromUTF8(stanje[0]["biljeska"].c_str())==wxT("")?wxT(" nema bilješki"):wxString::FromUTF8(stanje[0]["biljeska"].c_str())));
        txtStanjeNapomena->SetValue(ispisMaterijala);
    }
    else
    {
        comboMjera->SetValue("kol.");
        txtStanjeKolicina->Clear();
        txtStanjeNapomena->Clear();
    }

}

void DijalogUnosStanja :: Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}

void DijalogUnosStanja :: GumbPritisnut( wxCommandEvent& event )
{
    wxString token;
    double kolicina;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    wxStringTokenizer tokenizer(comboMaterijali->GetValue(), "|");
    if(id==dlgUnosStanjaPrihvati)
    {
        token = tokenizer.GetNextToken().Trim();
        if(!token.ToLong(&materijalId))
        {
            wxMessageDialog dijalog(this,wxString(wxT("Potrebno je odabrati materijal.\nOdabrano: "))+token,
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if(!txtStanjeKolicina->GetValue().ToDouble(&kolicina)||kolicina<0.0)
        {
            wxMessageDialog dijalog(this,wxT("Potrebno je unijeti ispravnu vrijednost za količinu."),
                                    wxT("Neispravan unos količine."),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        redak.clear();
        redak.push_back(materijalId);
        redak.push_back(skladisteId);
        redak.push_back(kolicina);
        redak.push_back(comboMjera->GetValue());
        redak.push_back(txtStanjeNapomena->GetValue().Trim());
        if(tp==TipPromjene::AZURIRANJE)
            parent->AzurirajBazu(redak);
        else if(tp==TipPromjene::DODAVANJE||tp==TipPromjene::REAKTIVACIJA)
            parent->DopuniBazu(redak);
    }
    Destroy();
}
