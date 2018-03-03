#include "PanelSkladista.h"

wchar_t skladista_zaglavlja[][20]={L"Šifra (id)",L"Oznaka",L"Lokacija",L"Telefon",L"Fax",L"Vrijeme od",L"Vrijeme do"};

PanelSkladista::PanelSkladista(wxFrame *frame, std::string connString): GUIPanelSkladista(frame)
{
    pqxx::result naziv;
    wxVector<wxVariant> redak;
    poveznica=new pqxx::connection(connString);

    //ctor
    tablicaSkladista->DeleteAllItems();
    tablicaSkladista->ClearColumns();
    for(int i=0; i<5; i++)
        tablicaSkladista->AppendTextColumn(skladista_zaglavlja[i]);

    panelStranicenje=new PanelStranicenje(frame, this);
    stranica=0;
    osvjeziCombo();


    stranicenjeSizer->Add( panelStranicenje, 0, wxALL, 5 );
    this->Layout();


    btnAzuriraj->Disable();
    btnDodaj->SetLabel(L"Dodaj");
    btnDodaj->Disable();
}

PanelSkladista::~PanelSkladista()
{
    //dtor
    poveznica->disconnect();
    delete poveznica;
}

void PanelSkladista::osvjeziCombo()
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
            comboFilter->Append(naziv[0][0].c_str());
        }
    txn.commit();
    wxCommandEvent emptyEvent;
    OnCombo(emptyEvent);
}

void PanelSkladista::OnCombo( wxCommandEvent& event )
{
    int i;
    wxString id;
    tablicaSkladista->UnselectAll();
    tablicaSkladista->DeleteAllItems();
    tablicaSkladista->ClearColumns();
    pqxx::work txn(*poveznica);
    if(comboFilter->GetSelection()==0)
    {
        for(i=0; i<5; i++)
            tablicaSkladista->AppendTextColumn(skladista_zaglavlja[i]);
        if(stranica*VELICINA_STRANICE>comboFilter->GetCount())
            stranica=0;
        if(panelStranicenje)
            panelStranicenje->PostaviStranice((razlicitiId.size()-1)/VELICINA_STRANICE+1,stranica);
        for(i=stranica*VELICINA_STRANICE; i<(stranica+1)*VELICINA_STRANICE; i++)
        {
            r = txn.exec("SELECT id,oznaka,lokacija,telefon,faks FROM skladista WHERE id="+txn.quote(razlicitiId[i][0].c_str())+
                         " AND vrijeme_do='infinity'::TIMESTAMP ORDER BY id");
            upisiRetke(r);
        }
        dodajPrazniRedak();
    }
    else
    {
        wxStringTokenizer tokenizer(comboFilter->GetValue(), "|");
        for(i=0; i<7; i++)
            tablicaSkladista->AppendTextColumn(skladista_zaglavlja[i]);
        id = tokenizer.GetNextToken();
        r = txn.exec("SELECT COUNT(id) as broj FROM skladista WHERE id="+txn.esc(id));
        if(stranica*VELICINA_STRANICE>r[0][0].as<int>())
            stranica=0;
        if(panelStranicenje)
            panelStranicenje->PostaviStranice((r[0][0].as<int>()-1)/VELICINA_STRANICE+1,stranica);
        r = txn.exec("SELECT id,oznaka,lokacija,telefon,faks,vrijeme_od, vrijeme_do FROM skladista WHERE id="+
                     txn.esc(id)+" ORDER BY vrijeme_od ASC LIMIT "+txn.quote(VELICINA_STRANICE)+" OFFSET "+txn.quote(stranica*VELICINA_STRANICE));
        upisiRetke(r);
        dodajPrazniRedak();
    }
    txn.commit();
}

void PanelSkladista::SelectionChanged( wxDataViewEvent& event )
{
    int i;
    wxVariant vrijednost, vrijeme_do;
    if((i=tablicaSkladista->GetSelectedRow())==wxNOT_FOUND)
    {
        btnDodaj->SetLabel(L"Dodaj");
        btnDodaj->Disable();
        btnAzuriraj->Disable();
    }
    else
    {
        tablicaSkladista->GetValue(vrijednost,i,0);
        if(comboFilter->GetSelection()>0)
        {
            tablicaSkladista->GetValue(vrijeme_do,i,6);
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

void PanelSkladista::PoziviDijalogUnosa( wxCommandEvent& event )
{
    int i, v;
    wxVariant vrijednost;
    wxVector<wxVariant> redak;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    v=tablicaSkladista->GetSelectedRow();
    tablicaSkladista->GetValue(vrijednost,v,0);
    switch(id)
    {
    case IDskladistaDodaj:
        if(vrijednost.GetString()=="*") //dodavanje
        {
            pqxx::work txn(*poveznica);
            r = txn.exec("SELECT MAX(id)+1 as max FROM skladista");
            txn.commit();
            redak.push_back(wxVariant(r[0]["max"].c_str()));
            for(i=1; i<tablicaSkladista->GetColumnCount(); i++)
                redak.push_back("");
            DijalogUnosSkladista dlg(this,redak,TipPromjene::DODAVANJE);
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
                r=txn.exec("DELETE from skladista WHERE id="+txn.esc(tablicaSkladista->GetTextValue(tablicaSkladista->GetSelectedRow(),0).c_str()));
                txn.commit();
                wxCommandEvent emptyEvent;
                OnCombo(emptyEvent);
            }
        }
        break;
    case IDskladistaAzuriraj:
        if(vrijednost.GetString()!="*") //azuriranje
        {
            if(tablicaSkladista->GetColumnCount()==5) //mod aktivnih - uvijek su sa vrijeme_do=infinity
            {
                for(i=0; i<tablicaSkladista->GetColumnCount(); i++)
                redak.push_back(tablicaSkladista->GetTextValue(v,i));
                DijalogUnosSkladista dlg(this,redak, TipPromjene::AZURIRANJE);
                dlg.ShowModal();
            }
            else //mod entiteta - ne mora imati vrijeme_do=infinity
            {
                pqxx::work txn(*poveznica);
                r=txn.exec("SELECT id,oznaka,lokacija,telefon,faks FROM skladista WHERE id="+
                           txn.esc(tablicaSkladista->GetTextValue(tablicaSkladista->GetSelectedRow(),0).c_str()) +
                           " AND vrijeme_do='infinity'::TIMESTAMP");
                if(r.size()==0) //neaktivni
                {
                    r=txn.exec("SELECT id,oznaka,lokacija,telefon,faks FROM skladista WHERE id="+
                           txn.esc(tablicaSkladista->GetTextValue(tablicaSkladista->GetSelectedRow(),0).c_str()) +
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
                redak.push_back(wxVariant(red["oznaka"].c_str()));
                redak.push_back(wxVariant(red["lokacija"].c_str()));
                redak.push_back(wxVariant(red["telefon"].c_str()));
                redak.push_back(wxVariant(red["faks"].c_str()));
                DijalogUnosSkladista dlg(this,redak, TipPromjene::DODAVANJE);
                dlg.ShowModal();

            }
        }
        break;
    }
}

void PanelSkladista::upisiRetke(pqxx::result r)
{
    wxVector<wxVariant> redak;
    for(pqxx::result::const_iterator red = r.begin(); red !=r.end(); ++red)
    {
        redak.clear();
        redak.push_back(wxVariant(red["id"].c_str()));
        redak.push_back(wxVariant(red["oznaka"].c_str()));
        redak.push_back(wxVariant(red["lokacija"].c_str()));
        redak.push_back(wxVariant(red["telefon"].c_str()));
        redak.push_back(wxVariant(red["faks"].c_str()));

        if(tablicaSkladista->GetColumnCount()==7)
        {
            redak.push_back(wxVariant(red["vrijeme_od"].c_str()));
            redak.push_back(wxVariant(red["vrijeme_do"].c_str()));
        }
        tablicaSkladista->AppendItem(redak);
    }

}

void PanelSkladista::dodajPrazniRedak()
{
    int i;
    wxVector<wxVariant> redak;
    redak.push_back("*");
    for(i=1; i<tablicaSkladista->GetColumnCount(); i++)
        redak.push_back("");
    tablicaSkladista->AppendItem(redak);
}

void PanelSkladista::AzurirajBazu(wxVector<wxVariant> redak)
{
    if(redak[0].GetInteger()<=0||redak[1].GetString()==""||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    r=txn.exec("UPDATE skladista SET oznaka='"+ txn.esc(redak[1].GetString())+
             "',lokacija='"+txn.esc(redak[2].GetString())+
             "',telefon='"+txn.esc(redak[3].GetString())+
             "',faks='"+txn.esc(redak[4].GetString())+
             "' WHERE id="+txn.esc(redak[0].GetString()));
    txn.commit();
    osvjeziCombo();

}
void PanelSkladista::DopuniBazu(wxVector<wxVariant> redak)
{
    if(redak[0].GetInteger()<=0||redak[1].GetString()==""||poveznica==nullptr)
        return;
    pqxx::work txn(*poveznica);
    r=txn.exec("INSERT INTO skladista(id,oznaka,lokacija,telefon,faks) VALUES("+
               txn.esc(redak[0].GetString())+",'"+txn.esc(redak[1].GetString())+"','"+
               txn.esc(redak[2].GetString())+"','"+txn.esc(redak[3].GetString())+"','"+
               txn.esc(redak[4].GetString())+"')");

    razlicitiId = txn.exec("SELECT DISTINCT id FROM skladista ORDER BY id");
    txn.commit();
    osvjeziCombo();
}

void PanelSkladista::PostaviStranicu(int stranica)
{
    this->stranica=stranica;
    wxCommandEvent emptyEvent;
    OnCombo(emptyEvent);
}

pqxx::connection* PanelSkladista::DajPoveznicu()
{
    return poveznica;
}

/********************************************************************************/
/********************** Dijalog unos ********************************************/
/********************************************************************************/
DijalogUnosSkladista :: DijalogUnosSkladista(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp) : dlgUnosSkladista(NULL)
{
    wxVector<wxVariant>::iterator it;
    this->parent=(PanelSkladista *)parent;
    this->tp = tp;
    for(it=redak.begin(); it!=redak.end(); ++it)
        this->redak.push_back(*it);
}

DijalogUnosSkladista :: ~DijalogUnosSkladista()
{

}
void DijalogUnosSkladista :: OnInit( wxInitDialogEvent& event )
{
    if(!redak.empty())
    {
        dlgSkladisteId->SetValue(redak[0]);
        dlgSkladisteOznaka->SetValue(redak[1]);
        dlgSkladisteLokacija->SetValue(redak[2]);
        dlgSkladisteTelefon->SetValue(redak[3]);
        dlgSkladisteTelefaks->SetValue(redak[4]);
    }
}
void DijalogUnosSkladista :: Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}

void DijalogUnosSkladista :: GumbPritisnut( wxCommandEvent& event )
{
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    if(id==dlgUnosSkladistaPrihvati)
    {
        if(dlgSkladisteOznaka->GetValue()=="")
        {
            wxMessageDialog dijalog(this,wxT("Potrebno je unijeti barem oznaku skladišta."),
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        redak.clear();
        redak.push_back(dlgSkladisteId->GetValue());
        redak.push_back(dlgSkladisteOznaka->GetValue());
        redak.push_back(dlgSkladisteLokacija->GetValue());
        redak.push_back(dlgSkladisteTelefon->GetValue());
        redak.push_back(dlgSkladisteTelefaks->GetValue());
        if(tp==TipPromjene::AZURIRANJE)
            parent->AzurirajBazu(redak);
        else if(tp==TipPromjene::DODAVANJE)
            parent->DopuniBazu(redak);

    }
    Destroy();
}


