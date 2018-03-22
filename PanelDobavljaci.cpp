#include "PanelDobavljaci.h"

wchar_t dobavljaci_zaglavlja[][20]={L"Šifra (id)",L"Naziv",L"Adresa",L"Telefon 1",L"Telefon 2",L"E-pošta",L"Vrijeme od",L"Vrijeme do"};

PanelDobavljaci::PanelDobavljaci(wxFrame *frame, std::string connString): GUIPanelDobavljaci(frame)
{
    pqxx::result naziv;
    wxVector<wxVariant> redak;
    poveznica=new pqxx::connection(connString);

    //ctor
    tablicaDobavljaci->DeleteAllItems();
    tablicaDobavljaci->ClearColumns();
    for(int i=0; i<6; i++)
        tablicaDobavljaci->AppendTextColumn(dobavljaci_zaglavlja[i]);

    panelStranicenje=new PanelStranicenje(frame, this);
    stranica=0;
    osvjeziCombo();


    stranicenjeSizer->Add( panelStranicenje, 0, wxALL, 5 );
    this->Layout();


    btnAzuriraj->Disable();
    btnDodaj->SetLabel(L"Dodaj");
    btnDodaj->Disable();
}

PanelDobavljaci::~PanelDobavljaci()
{
    //dtor
    poveznica->disconnect();
    delete poveznica;
}

void PanelDobavljaci::osvjeziCombo()
{
    pqxx::result naziv;
    comboFilter->Clear();
    comboFilter->Append("Aktivni");
    comboFilter->SetSelection(0);
    pqxx::work txn(*poveznica);
    razlicitiId = txn.exec("SELECT DISTINCT id FROM dobavljaci ORDER BY id");
    if(!razlicitiId.empty())
        for(pqxx::result::const_iterator redak = razlicitiId.begin(); redak !=razlicitiId.end(); ++redak)
        {
            naziv = txn.exec("SELECT id || ' | ' || naziv FROM dobavljaci WHERE id="+txn.esc(redak[0].c_str()) +" ORDER BY vrijeme_od DESC LIMIT 1");
            comboFilter->Append(naziv[0][0].c_str());
        }

    txn.commit();
    wxCommandEvent emptyEvent;
    OnCombo(emptyEvent);
}

void PanelDobavljaci::OnCombo( wxCommandEvent& event )
{
    int i;
    wxString id;
    tablicaDobavljaci->UnselectAll();
    tablicaDobavljaci->DeleteAllItems();
    tablicaDobavljaci->ClearColumns();
    pqxx::work txn(*poveznica);
    if(comboFilter->GetSelection()==0)
    {
        for(i=0; i<6; i++)
            tablicaDobavljaci->AppendTextColumn(dobavljaci_zaglavlja[i]);
        if(stranica*VELICINA_STRANICE>comboFilter->GetCount())
            stranica=0;
        if(panelStranicenje)
            panelStranicenje->PostaviStranice((razlicitiId.size()-1)/VELICINA_STRANICE+1,stranica);
        for(i=stranica*VELICINA_STRANICE; i<(stranica+1)*VELICINA_STRANICE; i++)
        {
            r = txn.exec("SELECT id,naziv,adresa,telefon,telefon2,\"e-mail\" FROM dobavljaci WHERE id="+txn.quote(razlicitiId[i][0].c_str())+
                         " AND vrijeme_do='infinity'::TIMESTAMP ORDER BY id");
            upisiRetke(r);
        }
        dodajPrazniRedak();
    }
    else
    {
        wxStringTokenizer tokenizer(comboFilter->GetValue(), "|");
        for(i=0; i<8; i++)
            tablicaDobavljaci->AppendTextColumn(dobavljaci_zaglavlja[i]);
        id = tokenizer.GetNextToken();
        r = txn.exec("SELECT COUNT(id) as broj FROM dobavljaci WHERE id="+txn.esc(id));
        if(stranica*VELICINA_STRANICE>r[0][0].as<int>())
            stranica=0;
        if(panelStranicenje)
            panelStranicenje->PostaviStranice((r[0][0].as<int>()-1)/VELICINA_STRANICE+1,stranica);
        r = txn.exec("SELECT id,naziv,adresa,telefon,telefon2,\"e-mail\",vrijeme_od, vrijeme_do FROM dobavljaci WHERE id="+
                     txn.esc(id)+" ORDER BY vrijeme_od ASC LIMIT "+txn.quote(VELICINA_STRANICE)+" OFFSET "+txn.quote(stranica*VELICINA_STRANICE));
        upisiRetke(r);
        dodajPrazniRedak();
    }
    txn.commit();
}
void PanelDobavljaci::SelectionChanged( wxDataViewEvent& event )
{
    int i;
    wxVariant vrijednost, vrijeme_do;
    if((i=tablicaDobavljaci->GetSelectedRow())==wxNOT_FOUND)
    {
        btnDodaj->SetLabel(L"Dodaj");
        btnDodaj->Disable();
        btnAzuriraj->Disable();
    }
    else
    {
        tablicaDobavljaci->GetValue(vrijednost,i,0);
        if(comboFilter->GetSelection()>0)
        {
            tablicaDobavljaci->GetValue(vrijeme_do,i,7);
            if(vrijednost.GetString()=="*")
            {
                btnDodaj->SetLabel(L"Dodaj");
                btnDodaj->Enable();
                btnAzuriraj->Disable();
            }
            else if(vrijeme_do.GetString()=="infinity")
            {
                btnDodaj->SetLabel(L"Obriši");
                btnDodaj->Enable();
                btnAzuriraj->Enable();
            }
            else
            {
                btnDodaj->SetLabel(L"Obriši");
                btnDodaj->Disable();
                btnAzuriraj->Enable();
            }
        }
        else
        {
            if(vrijednost.GetString()=="*")
            {
                btnDodaj->SetLabel(L"Dodaj");
                btnDodaj->Enable();
                btnAzuriraj->Disable();
            }
            else
            {
                btnDodaj->SetLabel(L"Obriši");
                btnDodaj->Enable();
                btnAzuriraj->Enable();
            }
        }
    }
}


void PanelDobavljaci::PoziviDijalogUnosa( wxCommandEvent& event )
{
    int i, v;
    wxVariant vrijednost;
    wxVector<wxVariant> redak;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    v=tablicaDobavljaci->GetSelectedRow();
    tablicaDobavljaci->GetValue(vrijednost,v,0);
    switch(id)
    {
    case IDdobavljaciDodaj:
        if(vrijednost.GetString()=="*") //dodavanje
        {
            pqxx::work txn(*poveznica);
            r = txn.exec("SELECT MAX(id)+1 as max FROM dobavljaci");
            txn.commit();
            redak.push_back(wxVariant(r[0]["max"].c_str()));
            for(i=1; i<tablicaDobavljaci->GetColumnCount(); i++)
                redak.push_back("");
            DijalogUnosDobavljaca dlg(this,redak,TipPromjene::DODAVANJE);
            dlg.ShowModal();
        }
        else //brisanje
        {
            wxMessageDialog dijalog(this,wxT("Ovom radnjom će odabrani zapis postati neaktivan.\nŽelite li nastaviti?"),
                                wxT("Brisanje zapisa"),  wxYES_NO |  wxICON_QUESTION);
            dijalog.SetYesNoLabels(_("&Da"), _("&Ne"));
            v=dijalog.ShowModal();
            if(v==wxID_NO)
                break;
            else
            {
                pqxx::work txn(*poveznica);
                r=txn.exec("DELETE from dobavljaci WHERE id="+txn.esc(tablicaDobavljaci->GetTextValue(tablicaDobavljaci->GetSelectedRow(),0).c_str()));
                txn.commit();
                wxCommandEvent emptyEvent;
                OnCombo(emptyEvent);
            }
        }
        break;
    case IDdobavljaciAzuriraj:
        if(vrijednost.GetString()!="*") //azuriranje
        {
            if(tablicaDobavljaci->GetColumnCount()==6) //mod aktivnih - uvijek su sa vrijeme_do=infinity
            {
                for(i=0; i<tablicaDobavljaci->GetColumnCount(); i++)
                redak.push_back(tablicaDobavljaci->GetTextValue(v,i));
                DijalogUnosDobavljaca dlg(this,redak, TipPromjene::AZURIRANJE);
                dlg.ShowModal();
            }
            else //mod entiteta - ne mora imati vrijeme_do=infinity
            {
                pqxx::work txn(*poveznica);
                r=txn.exec("SELECT id,naziv,adresa,telefon,telefon2,\"e-mail\" FROM dobavljaci WHERE id="+
                           txn.esc(tablicaDobavljaci->GetTextValue(tablicaDobavljaci->GetSelectedRow(),0).c_str()) +
                           " AND vrijeme_do='infinity'::TIMESTAMP");
                if(r.size()==0) //neaktivni
                {
                    r=txn.exec("SELECT id,naziv,adresa,telefon,telefon2,\"e-mail\" FROM dobavljaci WHERE id="+
                           txn.esc(tablicaDobavljaci->GetTextValue(tablicaDobavljaci->GetSelectedRow(),0).c_str()) +
                           " ORDER BY vrijeme_do DESC LIMIT 1");
                    std::cout << "Neaktivni..." << std::endl;
                }
                if(r.size()==0)
                    break;
                txn.commit();
                wxVector<wxVariant> redak;
                pqxx::result::const_iterator red=r.begin();
                redak.clear();

                redak.push_back(wxVariant(red["id"].c_str()));
                redak.push_back(wxVariant(red["naziv"].c_str()));
                redak.push_back(wxVariant(red["adresa"].c_str()));
                redak.push_back(wxVariant(red["telefon"].c_str()));
                redak.push_back(wxVariant(red["telefon2"].c_str()));
                redak.push_back(wxVariant(red["e-mail"].c_str()));
                DijalogUnosDobavljaca dlg(this,redak, TipPromjene::DODAVANJE);
                dlg.ShowModal();

            }
        }
        break;
    }
}

void PanelDobavljaci::upisiRetke(pqxx::result r)
{
    wxVector<wxVariant> redak;
    for(pqxx::result::const_iterator red = r.begin(); red !=r.end(); ++red)
    {
        redak.clear();
        redak.push_back(wxVariant(red["id"].c_str()));
        redak.push_back(wxVariant(red["naziv"].c_str()));
        redak.push_back(wxVariant(red["adresa"].c_str()));
        redak.push_back(wxVariant(red["telefon"].c_str()));
        redak.push_back(wxVariant(red["telefon2"].c_str()));
        redak.push_back(wxVariant(red["e-mail"].c_str()));

        if(tablicaDobavljaci->GetColumnCount()==8)
        {
            redak.push_back(wxVariant(red["vrijeme_od"].c_str()));
            redak.push_back(wxVariant(red["vrijeme_do"].c_str()));
        }
        tablicaDobavljaci->AppendItem(redak);
    }

}
void PanelDobavljaci::dodajPrazniRedak()
{
    int i;
    wxVector<wxVariant> redak;
    redak.push_back("*");
    for(i=1; i<tablicaDobavljaci->GetColumnCount(); i++)
        redak.push_back("");
    tablicaDobavljaci->AppendItem(redak);
}

void PanelDobavljaci::AzurirajBazu(wxVector<wxVariant> redak)
{
    if(redak[0].GetInteger()<=0||redak[1].GetString()==""||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    r=txn.exec("UPDATE dobavljaci SET naziv='"+ txn.esc(redak[1].GetString())+
             "',adresa='"+txn.esc(redak[2].GetString())+
             "',telefon='"+txn.esc(redak[3].GetString())+
             "',telefon2='"+txn.esc(redak[4].GetString())+
             "',\"e-mail\"='"+txn.esc(redak[5].GetString())+
             "' WHERE id="+txn.esc(redak[0].GetString()));
    txn.commit();
    osvjeziCombo();
}
void PanelDobavljaci::DopuniBazu(wxVector<wxVariant> redak)
{
    if(redak[0].GetInteger()<=0||redak[1].GetString()==""||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    r=txn.exec("INSERT INTO dobavljaci(id,naziv,adresa,telefon,telefon2,\"e-mail\") VALUES("+
               txn.esc(redak[0].GetString())+",'"+txn.esc(redak[1].GetString())+"','"+
               txn.esc(redak[2].GetString())+"','"+txn.esc(redak[3].GetString())+"','"+
               txn.esc(redak[4].GetString())+"','"+txn.esc(redak[5].GetString())+"')");

    razlicitiId = txn.exec("SELECT DISTINCT id FROM dobavljaci ORDER BY id");
    txn.commit();
    osvjeziCombo();
}

void PanelDobavljaci::PostaviStranicu(int stranica)
{
    this->stranica=stranica;
    wxCommandEvent emptyEvent;
    OnCombo(emptyEvent);
}

pqxx::connection* PanelDobavljaci::DajPoveznicu()
{
    return poveznica;
}

/********************************************************************************/
/********************** Dijalog unos ********************************************/
/********************************************************************************/
DijalogUnosDobavljaca :: DijalogUnosDobavljaca(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp) : dlgUnosDobavljaca(NULL)
{
    wxVector<wxVariant>::iterator it;
    this->parent=(PanelDobavljaci *)parent;
    this->tp = tp;
    for(it=redak.begin(); it!=redak.end(); ++it)
        this->redak.push_back(*it);
}

DijalogUnosDobavljaca :: ~DijalogUnosDobavljaca()
{

}
void DijalogUnosDobavljaca :: OnInit( wxInitDialogEvent& event )
{
    if(!redak.empty())
    {
        dlgDobavljaciId->SetValue(redak[0]);
        dlgDobavljaciNaziv->SetValue(redak[1]);
        dlgDobavljaciAdresa->SetValue(redak[2]);
        dlgDobavljaciTelefon1->SetValue(redak[3]);
        dlgDobavljaciTelefon2->SetValue(redak[4]);
        dlgDobavljaciEmail->SetValue(redak[5]);
    }
}
void DijalogUnosDobavljaca :: Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}

void DijalogUnosDobavljaca :: GumbPritisnut( wxCommandEvent& event )
{
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    if(id==dlgUnosDobavljacaPrihvati)
    {
        if(dlgDobavljaciNaziv->GetValue()=="")
        {
            wxMessageDialog dijalog(this,wxT("Potrebno je unijeti barem naziv dobavljača."),
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        redak.clear();
        redak.push_back(dlgDobavljaciId->GetValue());
        redak.push_back(dlgDobavljaciNaziv->GetValue());
        redak.push_back(dlgDobavljaciAdresa->GetValue());
        redak.push_back(dlgDobavljaciTelefon1->GetValue());
        redak.push_back(dlgDobavljaciTelefon2->GetValue());
        redak.push_back(dlgDobavljaciEmail->GetValue());
        if(tp==TipPromjene::AZURIRANJE)
            parent->AzurirajBazu(redak);
        else if(tp==TipPromjene::DODAVANJE)
            parent->DopuniBazu(redak);

    }
    Destroy();
}
