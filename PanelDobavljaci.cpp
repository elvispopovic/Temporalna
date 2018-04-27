#include "PanelDobavljaci.h"

PanelDobavljaci::PanelDobavljaci(wxFrame *frame, std::string connString): GUIPanelDobavljaci(frame)
{
    pqxx::result naziv;
    wxVector<wxVariant> redak;
    poveznica=new pqxx::connection(connString);
    poveznica->set_client_encoding("UTF8");
    //ctor
    tablicaDobavljaci->DeleteAllItems();
    tablicaDobavljaci->ClearColumns();
    for(int i=0; i<6; i++)
        tablicaDobavljaci->AppendTextColumn(DOBAVLJACI_ZAGLAVLJA[i]);

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
            comboFilter->Append(wxString::FromUTF8(naziv[0][0].c_str()));
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
            tablicaDobavljaci->AppendTextColumn(DOBAVLJACI_ZAGLAVLJA[i]);
        if(stranica*VELICINA_STRANICE>comboFilter->GetCount())
            stranica=0;
        if(panelStranicenje)
            if(razlicitiId.size()>0)
                panelStranicenje->PostaviStranice((razlicitiId.size()-1)/VELICINA_STRANICE+1,stranica);
            else
                panelStranicenje->PostaviStranice(1,stranica);
        for(i=stranica*VELICINA_STRANICE; i<(stranica+1)*VELICINA_STRANICE; i++)
        {
            r = txn.exec("SELECT id,naziv,adresa,telefon,telefon2,\"e-mail\" FROM dobavljaci WHERE id="+txn.quote(razlicitiId[i][0].c_str())+
                         " AND vrijeme_do='infinity'::TIMESTAMP ORDER BY id");
            if(DEBUG_TEST)
                for(pqxx::result::const_iterator red = r.begin(); red !=r.end(); ++red)
                    std::cout << "Id: " << red[0].c_str() << ", naziv: " << wxString::FromUTF8(red[1].c_str()) << std::endl;
            upisiRetke(r);
        }
        dodajPrazniRedak();
    }
    else
    {
        wxStringTokenizer tokenizer(comboFilter->GetValue(), "|");
        for(i=0; i<8; i++)
            tablicaDobavljaci->AppendTextColumn(DOBAVLJACI_ZAGLAVLJA[i]);
        id = tokenizer.GetNextToken();
        r = txn.exec("SELECT COUNT(id) as broj FROM dobavljaci WHERE id="+txn.esc(id));
        if(stranica*VELICINA_STRANICE>r[0][0].as<int>())
            stranica=0;
        if(panelStranicenje)
            if(razlicitiId.size()>0)
                panelStranicenje->PostaviStranice((r[0][0].as<int>()-1)/VELICINA_STRANICE+1,stranica);
            else
                panelStranicenje->PostaviStranice(1,stranica);
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
            if(wxString(r[0]["max"].c_str())!="")
                redak.push_back(wxVariant(r[0]["max"].c_str()));
            else
                redak.push_back(wxVariant("1"));
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
                try
                {
                    r=txn.exec("DELETE from dobavljaci WHERE id="+txn.esc(tablicaDobavljaci->GetTextValue(tablicaDobavljaci->GetSelectedRow(),0).c_str()));
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
                TipPromjene tp = TipPromjene::DODAVANJE;
                pqxx::work txn(*poveznica);
                r=txn.exec("SELECT id,naziv,adresa,telefon,telefon2,\"e-mail\" FROM dobavljaci WHERE id="+
                           txn.esc(tablicaDobavljaci->GetTextValue(tablicaDobavljaci->GetSelectedRow(),0).c_str()) +
                           " AND vrijeme_do='infinity'::TIMESTAMP");
                if(r.size()==0) //neaktivni
                {
                    tp = TipPromjene::REAKTIVACIJA;
                    r=txn.exec("SELECT id,naziv,adresa,telefon,telefon2,\"e-mail\" FROM dobavljaci WHERE id="+
                           txn.esc(tablicaDobavljaci->GetTextValue(tablicaDobavljaci->GetSelectedRow(),0).c_str()) +
                           " ORDER BY vrijeme_do DESC LIMIT 1");
                }
                if(r.size()==0)
                    break;
                txn.commit();
                wxVector<wxVariant> redak;
                pqxx::result::const_iterator red=r.begin();
                redak.clear();

                for(pqxx::tuple::iterator celija=red.begin(); celija!=red.end(); ++celija)
                    redak.push_back(wxVariant(wxString::FromUTF8(celija->c_str())));
                DijalogUnosDobavljaca dlg(this,redak, tp);
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
        for(pqxx::tuple::iterator celija = red.begin(); celija!=red.end(); ++celija)
            redak.push_back(wxVariant(wxString::FromUTF8(celija.c_str())));
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
    if(redak[0].GetInteger()<=0||redak[1]==""||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    try
    {
    txn.exec("SET client_encoding = 'utf8'");
    r=txn.exec("UPDATE dobavljaci SET naziv='"+ txn.esc(redak[1].GetString().ToUTF8())+
             "',adresa='"+txn.esc(redak[2].GetString().ToUTF8())+
             "',telefon='"+txn.esc(redak[3])+
             "',telefon2='"+txn.esc(redak[4])+
             "',\"e-mail\"='"+txn.esc(redak[5].GetString().ToUTF8())+
             "' WHERE id="+txn.esc(redak[0]));
    txn.commit();
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
    }
    osvjeziCombo();
}
void PanelDobavljaci::DopuniBazu(wxVector<wxVariant> redak)
{
    if(redak[0].GetInteger()<=0||redak[1]==""||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    try
    {
    txn.exec("SET client_encoding = UTF8");
    r=txn.exec("INSERT INTO dobavljaci(id,naziv,adresa,telefon,telefon2,\"e-mail\") VALUES("+
               txn.esc(redak[0])+",'"+txn.esc(redak[1].GetString().ToUTF8())+"','"+
               txn.esc(redak[2].GetString().ToUTF8())+"','"+txn.esc(redak[3])+"','"+
               txn.esc(redak[4])+"','"+txn.esc(redak[5].GetString().ToUTF8())+"')");

    razlicitiId = txn.exec("SELECT DISTINCT id FROM dobavljaci ORDER BY id");
    txn.commit();
    osvjeziCombo();
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
    }
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
    int dlgRes;
    if(!redak.empty())
    {
        dlgDobavljaciId->SetValue(redak[0]);
        dlgDobavljaciNaziv->SetValue(redak[1]);
        dlgDobavljaciAdresa->SetValue(redak[2]);
        dlgDobavljaciTelefon1->SetValue(redak[3]);
        dlgDobavljaciTelefon2->SetValue(redak[4]);
        dlgDobavljaciEmail->SetValue(redak[5]);
    }
    if(tp==TipPromjene::REAKTIVACIJA)
    {
        wxMessageDialog dijalog(this,wxT("Dobavljač je neaktivan. Ovim će se postupkom reaktivirati."),
                                    wxT("Reaktivacija neaktivnog dobavljača"),  wxOK |  wxICON_INFORMATION);
        dijalog.SetOKLabel(wxT("&U redu"));
        dlgRes=dijalog.ShowModal();
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
        else if(tp==TipPromjene::DODAVANJE||tp==TipPromjene::REAKTIVACIJA)
            parent->DopuniBazu(redak);

    }
    Destroy();
}
