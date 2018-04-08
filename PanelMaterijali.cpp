#include "PanelMaterijali.h"

char NaziviTablica[][24]={"supravodici","shim_zavojnice","trake","stitovi"};

PanelMaterijali::PanelMaterijali(wxFrame *frame, std::string connString): GUIPanelMaterijali(frame)
{
    pqxx::result naziv;
    wxVector<wxVariant> redak;
    poveznica=new pqxx::connection(connString);


    modelMaterijala = new MaterijaliModel(this);
    dvcMaterijali->AssociateModel(modelMaterijala);
    modelMaterijala->Postavi(chkPrikaziNeaktivne->IsChecked());

    btnAzuriraj->Disable();
    btnDodaj->SetLabel(L"Dodaj");
    btnDodaj->Disable();
}

PanelMaterijali::~PanelMaterijali()
{
    poveznica->disconnect();
    delete poveznica;
    dvcMaterijali->AssociateModel(NULL);
    delete modelMaterijala;
}

bool PanelMaterijali::PrimiSQLZahtijev(wxString sqlString)
{
    pqxx::result r;
    wxCommandEvent emptyEvent;
    pqxx::work txn(*poveznica);
    try
    {
        r=txn.exec(sqlString.ToStdString());
        txn.commit();
    }
    catch (const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(this,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Ažuriranje baze podataka"),  wxOK |  wxICON_ERROR);
        dijalog.SetOKLabel(wxT("&U redu"));
        dijalog.ShowModal();
        return false;
    }
    NeaktivniChecked(emptyEvent);
    return true;
}

pqxx::connection* PanelMaterijali::DajPoveznicu()
{
    return poveznica;
}

void PanelMaterijali::PostaviStranicu(int stranica)
{

}
void PanelMaterijali::SelekcijaPromijenjena( wxDataViewEvent& event )
{
    long id;
    pqxx::result r;
    wxString naziv, vrijeme_od, vrijeme_do, vrijeme_dobavljaca;
    wxDataViewItem item = event.GetItem();
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    if (!cvor)
    {
        tablicaPovijesti->DeleteAllItems();
        return;
    }
    if(cvor->JeLiKontejner())
    {
        tablicaPovijesti->DeleteAllItems();
        txtMaterijalDetaljiL->Clear();
        txtMaterijalDetaljiD->Clear();
        if(cvor->DajVrstu()==VrstaMaterijala::MATERIJALI)
        {
            btnAzuriraj->Disable();
            btnDodaj->SetLabel(L"Dodaj");
            btnDodaj->Disable();
        }
        else
        {
            btnDodaj->SetLabel(L"Dodaj");
            btnDodaj->Enable();
            btnAzuriraj->Disable();
        }
        return;
    }
    btnAzuriraj->Enable();
    btnDodaj->SetLabel(L"Obriši");
    btnDodaj->Enable();
    id=cvor->DajId();
    naziv=cvor->DajNaziv();
    vrijeme_od=cvor->DajVrijeme_od();
    vrijeme_do=cvor->DajVrijeme_do();
    DohvatiPovijest(id);
    DohvatiPodvrstu(id,vrijeme_od);
}
void PanelMaterijali::PovijestPromijenjena( wxDataViewEvent& event )
{
    int i;
    long id;
    wxString vrijeme_od;
    wxVariant vrijednost;
    if((i=tablicaPovijesti->GetSelectedRow())==wxNOT_FOUND)
        return;

    wxDataViewItem item = dvcMaterijali->GetCurrentItem();
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    if (!cvor||cvor->JeLiKontejner())
    {
        tablicaPovijesti->DeleteAllItems();
        return;
    }
    id=cvor->DajId();
    tablicaPovijesti->GetValue(vrijednost,i,1);
    std::cout << "Selekcija promijenjena. vrijeme od = " << vrijednost.GetString() << std::endl;
    DohvatiPodvrstu(id,vrijednost.GetString());
}

void PanelMaterijali::NeaktivniChecked( wxCommandEvent& event )
{
    dvcMaterijali->AssociateModel(NULL);
    modelMaterijala->Obrisi();
    dvcMaterijali->AssociateModel(modelMaterijala);
    modelMaterijala->Postavi(chkPrikaziNeaktivne->IsChecked());
}

void PanelMaterijali::PoziviDijalogUnosa( wxCommandEvent& event )
{
    wxCommandEvent emptyEvent;
    wxVector<wxVariant> redak;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    wxDataViewItem item = dvcMaterijali->GetCurrentItem();
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    pqxx::result dobavljaci;
    pqxx::work txn(*poveznica);
    r = txn.exec("SELECT MAX(id)+1 as max FROM materijali");
    dobavljaci = txn.exec("SELECT id,naziv FROM dobavljaci WHERE vrijeme_do='infinity'::TIMESTAMP ORDER BY id");
    txn.commit();
    switch(id)
    {
        case IDmaterijaliDodaj:
            if(cvor->JeLiKontejner()) //dodavanje
            {
                if(cvor->DajVrstu()==VrstaMaterijala::MATERIJALI)
                    break;
                if(r.size()>0)
                    redak.push_back(wxVariant(r[0]["max"].c_str()));
                switch(cvor->DajVrstu())
                {
                case VrstaMaterijala::SUPRAVODICI:
                    {
                    DijalogUnosSupravodica dlg(this,redak,TipPromjene::DODAVANJE,dobavljaci);
                    dlg.ShowModal();
                    }
                    break;
                case VrstaMaterijala::SHIM_ZAVOJNICE:
                    {
                    DijalogUnosShim dlg(this,redak,TipPromjene::DODAVANJE,dobavljaci);
                    dlg.ShowModal();
                    }
                    break;
                case VrstaMaterijala::TRAKE:
                    {
                    DijalogUnosTraka dlg(this,redak,TipPromjene::DODAVANJE,dobavljaci);
                    dlg.ShowModal();
                    }
                    break;
                case VrstaMaterijala::STITOVI:
                    {
                    DijalogUnosStitova dlg(this,redak,TipPromjene::DODAVANJE,dobavljaci);
                    dlg.ShowModal();
                    }
                    break;
                }

            }
            else
            {//brisanje materijala
                if(cvor->DajVrijeme_do()!="infinity")
                {
                    wxMessageDialog dijalog(this,wxT("Materijal je neaktivan. Nije moguća manipulacija brisanja toj. deaktiviranja.\n"),
                                    wxT("Brisanje materijala"),  wxOK |  wxICON_ERROR);
                    dijalog.SetOKLabel(wxT("&U redu"));
                    dijalog.ShowModal();
                    break;
                }
                wxMessageDialog dijalog(this,wxT("Materijal će biti postavljen na neaktivan, tj. uvjetno obrisan.\nŽelite li nastaviti?"),
                                    wxT("Brisanje materijala"),  wxYES_NO |  wxICON_ERROR);
                dijalog.SetYesNoLabels(wxT("&Da"),wxT("&Ne"));
                if(dijalog.ShowModal()==wxID_NO)
                    break;
                pqxx::work txn(*poveznica);
                try
                {
                    r=txn.exec("DELETE from materijali WHERE id="+txn.quote(cvor->DajId()));
                    txn.commit();
                    NeaktivniChecked(emptyEvent);
                }
                catch (const pqxx::sql_error& e)
                {
                    txn.abort();
                    wxMessageDialog dijalog(this,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Brisanje materijala"),  wxOK |  wxICON_ERROR);
                    dijalog.SetOKLabel(wxT("&U redu"));
                    dijalog.ShowModal();
                }
            }
            break;
        case IDmaterijaliAzuriraj: //azuriranje materijala
            if(cvor->JeLiKontejner())
                break;
            redak=dohvatiPodatkeMaterijala(cvor);

            if(redak.size()>0)
            {
                switch(cvor->DajVrstu())
                {
                case VrstaMaterijala::SUPRAVODICI:
                    {
                        if(cvor->DajVrijeme_do()!="infinity")
                        {
                            DijalogUnosSupravodica dlg(this,redak,TipPromjene::REAKTIVACIJA,dobavljaci);
                            dlg.ShowModal();
                        }
                        else
                        {
                            DijalogUnosSupravodica dlg(this,redak,TipPromjene::AZURIRANJE,dobavljaci);
                            dlg.ShowModal();
                        }
                    }
                    break;
                case VrstaMaterijala::SHIM_ZAVOJNICE:
                    {
                        if(cvor->DajVrijeme_do()!="infinity")
                        {
                            DijalogUnosShim dlg(this,redak,TipPromjene::REAKTIVACIJA,dobavljaci);
                            dlg.ShowModal();
                        }
                        else
                        {
                            DijalogUnosShim dlg(this,redak,TipPromjene::AZURIRANJE,dobavljaci);
                            dlg.ShowModal();
                        }
                    }
                    break;
                case VrstaMaterijala::TRAKE:
                    {
                        if(cvor->DajVrijeme_do()!="infinity")
                        {
                            DijalogUnosTraka dlg(this,redak,TipPromjene::REAKTIVACIJA,dobavljaci);
                            dlg.ShowModal();
                        }
                        else
                        {
                            DijalogUnosTraka dlg(this,redak,TipPromjene::AZURIRANJE,dobavljaci);
                            dlg.ShowModal();
                        }
                    }
                    break;
                case VrstaMaterijala::STITOVI:
                    {
                        if(cvor->DajVrijeme_do()!="infinity")
                        {
                            DijalogUnosStitova dlg(this,redak,TipPromjene::REAKTIVACIJA,dobavljaci);
                            dlg.ShowModal();
                        }
                        else
                        {
                            DijalogUnosStitova dlg(this,redak,TipPromjene::AZURIRANJE,dobavljaci);
                            dlg.ShowModal();
                        }
                    }
                    break;
                }
            }
            break;
    }
}
wxString PanelMaterijali::DohvatiPodatkeODobavljacu(long id) const
{
    pqxx::result r;
    wxString povratna;
    pqxx::work txn(*poveznica);
    r = txn.exec("SELECT 'Naziv dobavljaca: '||dobavljaci.naziv||'\nAdresa: '||COALESCE(dobavljaci.adresa,'nepoznata')||'\ntelefoni: '\
||COALESCE(dobavljaci.telefon,'-')||' '||COALESCE(dobavljaci.telefon2,'-')||'\ne-mail: '||COALESCE(dobavljaci.\"e-mail\",'-') \
AS dobavljac_info FROM dobavljaci WHERE id="+txn.quote(id)+" ORDER BY vrijeme_od DESC LIMIT 1");
    txn.commit();
    if(r.size()>0)
        povratna = r.begin()["dobavljac_info"].c_str();
    return povratna;
}

wxVector<wxVariant> PanelMaterijali::dohvatiPodatkeMaterijala(MaterijaliCvor *cvor) const
{
    wxVector<wxVariant> redak;
    pqxx::result r;
    pqxx::work txn(*poveznica);
    if(cvor==nullptr || cvor->DajVrstu()==VrstaMaterijala::MATERIJALI)
        return redak;
    switch(cvor->DajVrstu())
    {
    case VrstaMaterijala::SUPRAVODICI:
    r = txn.exec("SELECT supravodici.id,supravodici.vrijeme_od,dobavljac,vrijeme_dobavljaca,\
supravodici.vrijeme_do,supravodici.naziv,tip,cisti_promjer,promjer_izolator,broj_niti,cu_sc,\
\"kriticna_struja3T\",\"kriticna_struja5T\",\"kriticna_struja7T\",\"kriticna_struja9T\",promjer_niti,\
dobavljaci.vrijeme_do AS dob_do, dobavljaci.naziv as dob_naziv \
FROM supravodici LEFT JOIN dobavljaci ON dobavljac=dobavljaci.id AND vrijeme_dobavljaca=dobavljaci.vrijeme_od \
WHERE supravodici.id="+txn.quote(cvor->DajId())+" AND supravodici.vrijeme_od='"+txn.esc(cvor->DajVrijeme_od())+"'");
        break;
    case VrstaMaterijala::SHIM_ZAVOJNICE:
    r = txn.exec("SELECT shim_zavojnice.id,shim_zavojnice.vrijeme_od,dobavljac,vrijeme_dobavljaca,\
shim_zavojnice.vrijeme_do,shim_zavojnice.naziv,tip,max_struja,sparivanje,promjer,jakost,\
dobavljaci.vrijeme_do AS dob_do, dobavljaci.naziv as dob_naziv \
FROM shim_zavojnice LEFT JOIN dobavljaci ON dobavljac=dobavljaci.id AND vrijeme_dobavljaca=dobavljaci.vrijeme_od \
WHERE shim_zavojnice.id="+txn.quote(cvor->DajId())+" AND shim_zavojnice.vrijeme_od='"+txn.esc(cvor->DajVrijeme_od())+"'");
        break;
    case VrstaMaterijala::TRAKE:
        r = txn.exec("SELECT trake.id,trake.vrijeme_od,dobavljac,vrijeme_dobavljaca,\
trake.vrijeme_do,trake.naziv,sirina,debljina,supstrat,stabilizator,krit_struja,\
dobavljaci.vrijeme_do AS dob_do, dobavljaci.naziv as dob_naziv \
FROM trake LEFT JOIN dobavljaci ON dobavljac=dobavljaci.id AND vrijeme_dobavljaca=dobavljaci.vrijeme_od \
WHERE trake.id="+txn.quote(cvor->DajId())+" AND trake.vrijeme_od='"+txn.esc(cvor->DajVrijeme_od())+"'");
        break;
    case VrstaMaterijala::STITOVI:
        r = txn.exec("SELECT stitovi.id,stitovi.vrijeme_od,dobavljac,vrijeme_dobavljaca,\
stitovi.vrijeme_do,stitovi.naziv,materijal,gustoca,debljina_zida,gustoca_mag_toka,faktor_zastite,efikasnost_zastite,\
krit_temp_zero,krit_temp_srednje,unut_promjer,duljina,\
dobavljaci.vrijeme_do AS dob_do, dobavljaci.naziv as dob_naziv \
FROM stitovi LEFT JOIN dobavljaci ON dobavljac=dobavljaci.id AND vrijeme_dobavljaca=dobavljaci.vrijeme_od \
WHERE stitovi.id="+txn.quote(cvor->DajId())+" AND stitovi.vrijeme_od='"+txn.esc(cvor->DajVrijeme_od())+"'");
        break;
    }
    txn.commit();
    if(r.size()>0)
    {
        pqxx::result::const_iterator red = r.begin();
        for(pqxx::tuple::iterator celija=red.begin(); celija!=red.end(); ++celija)
            redak.push_back(wxVariant(celija->c_str()));
    }
    return redak;
}

pqxx::result PanelMaterijali::DohvatiPovijest(long id) const
{
    wxVector<wxVariant> redak;
    pqxx::work txn(*poveznica);
    pqxx::result r;
    r = txn.exec("SELECT id,naziv,vrijeme_od,vrijeme_do FROM materijali WHERE id="+
                 txn.quote(id)+ " ORDER BY vrijeme_od");
    tablicaPovijesti->DeleteAllItems();
    for(pqxx::result::const_iterator red = r.begin(); red !=r.end(); ++red)
    {
        redak.clear();
        redak.push_back(wxVariant(red["naziv"].c_str()));
        redak.push_back(wxVariant(red["vrijeme_od"].c_str()));
        redak.push_back(wxVariant(red["vrijeme_do"].c_str()));
        tablicaPovijesti->AppendItem(redak);
    }
    txn.commit();
    return r;
}

pqxx::result PanelMaterijali::DohvatiPodvrstu(long id, const wxString& vrijeme_od) const
{
    int i;
    pqxx::work txn(*poveznica);
    wxString upisL, upisD;
    pqxx::result r1, r2, r3;
    for(i=0; i<4; i++)
    {
        r1 = txn.exec("SELECT * FROM "+txn.esc(NaziviTablica[i])+
                      " WHERE id="+txn.quote(id)+" AND vrijeme_od='"+txn.esc(vrijeme_od)+"'");
                    // " WHERE id="+txn.quote(id)+" AND vrijeme_do='infinity'::TIMESTAMP");
        if(!r1.empty())
        {
            r2 = txn.exec("SELECT * FROM dimenzije WHERE id="+txn.quote(i+1));
            break;
        }
    }
    if(i>3||r1.size()==0||r2.size()==0)
    {
        txn.commit();
        return r1;
    }
    pqxx::result::const_iterator red1=r1.begin();
    r3 = txn.exec("SELECT naziv,adresa FROM dobavljaci WHERE id="+txn.quote(red1["dobavljac"].c_str())+
                  " AND vrijeme_od="+txn.quote(red1["vrijeme_dobavljaca"].c_str()));
    txn.commit();
    pqxx::result::const_iterator red2=r2.begin();
    pqxx::result::const_iterator red3=r3.begin();
    switch(i)
    {
    case 0:
        upisL.append(wxString::Format(wxT(" Naziv materijala:\t %s\n"),red1["naziv"].c_str()));
        upisD.append(wxString::Format(wxT("Tip materijala:\t\t %s\n"),red1["tip"].c_str()));
        upisL.append(wxString::Format(wxT(" Promjer vodiča:\t\t %s %s\n"),red1["cisti_promjer"].c_str(),red2["2"].c_str()));
        upisL.append(wxString::Format(wxT(" Promjer izolatora:\t %s %s\n"),red1["promjer_izolator"].c_str(),red2["3"].c_str()));
        upisL.append(wxString::Format(wxT(" Broj supravod. niti:\t %s\n"),red1["broj_niti"].c_str()));
        upisL.append(wxString::Format(wxT(" Cu:SC omjer:\t\t %s\n"),red1["cu_sc"].c_str()));
        upisD.append(wxString::Format(wxT("Kritična struja (3 Tesla):\t %s %s\n"),red1[11].c_str(),red2["6"].c_str()));
        upisD.append(wxString::Format(wxT("Kritična struja (5 Tesla):\t %s %s\n"),red1[12].c_str(),red2["7"].c_str()));
        upisD.append(wxString::Format(wxT("Kritična struja (7 Tesla):\t %s %s\n"),red1[13].c_str(),red2["8"].c_str()));
        upisD.append(wxString::Format(wxT("Kritična struja (9 Tesla):\t %s %s\n"),red1[14].c_str(),red2["9"].c_str()));
        upisD.append(wxString::Format(wxT("Promjer sup. niti:\t %s %s\n"),red1["promjer_niti"].c_str(),red2["10"].c_str()));
        break;
    case 1:
        upisL.append(wxString::Format(wxT(" Naziv materijala:\t %s\n"),red1["naziv"].c_str()));
        upisD.append(wxString::Format(wxT("Tip materijala:\t\t %s\n"),red1["tip"].c_str()));
        upisL.append(wxString::Format(wxT(" Najveća struja:\t\t %s %s\n"),red1["max_struja"].c_str(),red2["2"].c_str()));
        upisD.append(wxString::Format(wxT("Sparivanje:\t\t\t %s\n"),red1["sparivanje"].c_str()));
        upisL.append(wxString::Format(wxT(" Promjer:\t\t\t %s %s\n"),red1["promjer"].c_str(),red2["4"].c_str()));
        upisL.append(wxString::Format(wxT(" Jakost mag. polja:\t %s %s\n"),red1["jakost"].c_str(),red2["5"].c_str()));
        break;

    case 2:
        upisL.append(wxString::Format(wxT(" Naziv materijala:\t %s\n"),red1["naziv"].c_str()));
        upisL.append(wxString::Format(wxT(" Širina:\t\t\t\t %s %s\n"),red1["sirina"].c_str(),red2["2"].c_str()));
        upisL.append(wxString::Format(wxT(" Debljina:\t\t\t %s %s\n"),red1["debljina"].c_str(),red2["3"].c_str()));
        upisD.append("\n");
        upisD.append(wxString::Format(wxT("Supstrat:\t\t\t %s\n"),red1["supstrat"].c_str()));
        upisD.append(wxString::Format(wxT("Katalizator:\t\t\t %s\n"),red1["stabilizator"].c_str()));
        upisD.append(wxString::Format(wxT("Kritična struja:\t\t %s %s\n"),red1["krit_struja"].c_str(),red2["6"].c_str()));
        break;
    case 3:
        upisL.append(wxString::Format(wxT(" Naziv materijala:\t %s\n"),red1["naziv"].c_str()));
        upisD.append(wxString::Format(wxT("Sastav materijala:\t\t %s\n"),red1["materijal"].c_str()));
        upisD.append(wxString::Format(wxT("Gustoća:\t\t\t\t %s %s\n"),red1["gustoca"].c_str(),red2["3"].c_str()));
        upisD.append(wxString::Format(wxT("Debljina zida:\t\t\t %s %s\n"),red1["debljina_zida"].c_str(),red2["4"].c_str()));
        upisL.append(wxString::Format(wxT(" Gustoća mag. toka:\t %s %s\n"),red1["gustoca_mag_toka"].c_str(),red2["5"].c_str()));
        upisL.append(wxString::Format(wxT(" Faktor zaštite:\t\t %s\n"),red1["faktor_zastite"].c_str()));
        upisL.append(wxString::Format(wxT(" Efikasnost zaštite:\t %s %s\n"),red1["efikasnost_zastite"].c_str(),red2["7"].c_str()));
        upisL.append(wxString::Format(wxT(" Krit. temp. (nula):\t %s %s\n"),red1["krit_temp_zero"].c_str(),red2["8"].c_str()));
        upisL.append(wxString::Format(wxT(" Krit. temp. (srednje):\t %s %s\n"),red1["krit_temp_srednje"].c_str(),red2["9"].c_str()));
        upisD.append(wxString::Format(wxT("Unut. promjer:\t\t\t %s %s\n"),red1["unut_promjer"].c_str(),red2["10"].c_str()));
        upisD.append(wxString::Format(wxT("Duljina:\t\t\t\t\t %s %s\n"),red1["duljina"].c_str(),red2["10"].c_str()));
        break;
    }
    if(r3.size()>0)
    {
        upisL.append(wxString::Format(wxT(" Naziv dobavljača:\t %s\n"),red3["naziv"].c_str()));
        upisL.append(wxString::Format(wxT(" Adresa dobavljača:\t %s\n"),red3["adresa"].c_str()));
    }
    txtMaterijalDetaljiL->Clear();
    txtMaterijalDetaljiD->Clear();
    txtMaterijalDetaljiL->AppendText(upisL);
    txtMaterijalDetaljiD->AppendText(upisD);
    return r;
}

/********************************************************************************/
/********************** Dijalog unos supravodica ********************************/
/********************************************************************************/
DijalogUnosSupravodica::DijalogUnosSupravodica(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci)
: dlgUnosSupravodica(NULL)
{
    wxVector<wxVariant>::iterator it;
    pqxx::result::iterator dobIt;
    this->parent=(PanelMaterijali *)parent;
    this->tp = tp;
    for(it=redak.begin(); it!=redak.end(); ++it)
        this->redak.push_back(*it);

    txtDobavljaciAdresaBazni=txtDobavljaciAdresa;
    if(redak.size()>=17)
    {
        if(redak[16].GetString()=="infinity")
        {
            for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
                comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));
            std::cout << "Dobavljac azuran: " << redak[16].GetString() << std::endl;
        }
        else
        {
            std::cout << "Stari dobavljac: " << redak[16].GetString() << std::endl;
        }
    }
    else
        for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
            comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));

}
// postoji samo ako je u wxFormBuilderu dijalogu dan event OnInit
void DijalogUnosSupravodica::OnInit( wxInitDialogEvent& event )
{
    long id;
    int dlgRes;
    wxString podaciODobavljacu;
    if(!redak.empty())
    {
        txtSupravodiciId->SetValue(redak[0]);
        if(redak.size()>=17)
        {
            txtSupravodiciNaziv->SetValue(redak[5]);
            txtSupravodiciTip->SetValue(redak[6]);
            txtSupravodiciCistiPromjer->SetValue(redak[7]);
            txtSupravodiciPromjerNiti->SetValue(redak[15]);
            txtSupravodiciPromjerIzolatora->SetValue(redak[8]);
            txtSupravodiciBrojNiti->SetValue(redak[9]);
            txtSupravodiciCuSC->SetValue(redak[10]);
            txtKritStruja3->SetValue(redak[11]);
            txtKritStruja5->SetValue(redak[12]);
            txtKritStruja7->SetValue(redak[13]);
            txtKritStruja9->SetValue(redak[14]);
            comboDobavljaci->SetValue(redak[2].GetString()+" | "+redak[17].GetString());
            redak[2].GetString().ToLong(&id);
            podaciODobavljacu=parent->DohvatiPodatkeODobavljacu(id);
            txtDobavljaciAdresa->SetValue(podaciODobavljacu);
        }
    }
    if(tp==TipPromjene::REAKTIVACIJA)
    {
        wxMessageDialog dijalog(this,wxT("Supravodič je neaktivan. Ovim će se postupkom reaktivirati."),
                                    wxT("Reaktivacija neaktivnog (uvjetno izbrisanog) supravodiča"),  wxOK |  wxICON_INFORMATION);
        dijalog.SetOKLabel(wxT("&U redu"));
        dlgRes=dijalog.ShowModal();
    }
}
void DijalogUnosSupravodica::OnCombo( wxCommandEvent& event )
{
    long int id;
    wxString token;
    wxString podaci;
    wxComboBox *combo = wxDynamicCast(event.GetEventObject(),wxComboBox);
    wxStringTokenizer tokenizer(combo->GetValue(), "|");
    token=tokenizer.GetNextToken().Trim();
    if(!token.ToLong(&id))
        return;
    podaci=parent->DohvatiPodatkeODobavljacu(id);
    if(txtDobavljaciAdresaBazni!=nullptr)
    {
        txtDobavljaciAdresaBazni->Clear();
        txtDobavljaciAdresaBazni->AppendText(podaci);
    }
}

void DijalogUnosSupravodica::Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}

void DijalogUnosSupravodica::GumbPritisnut( wxCommandEvent& event )
{
    wxString sqlString, valuesString, token;
    long iv;
    long int idDobavljac;
    double dv;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    wxStringTokenizer tokenizer(comboDobavljaci->GetValue(), "|");
    if(id==ID_UnosMaterijalaPrihvati)
    {
        token = tokenizer.GetNextToken().Trim();
        if(!token.ToLong(&idDobavljac))
        {
            wxMessageDialog dijalog(this,wxString(wxT("Potrebno je odabrati dobavljača.\nOdabrano: "))+token,
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if((txtSupravodiciNaziv->GetValue()=="")||(txtSupravodiciTip->GetValue()==""))
        {
            wxMessageDialog dijalog(this,wxT("Potrebno je unijeti barem naziv i tip supravodiča."),
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if(tp==TipPromjene::AZURIRANJE)
        {
            sqlString.Append("UPDATE supravodici SET ");
            sqlString.Append(wxString::Format("naziv='%s'",txtSupravodiciNaziv->GetValue().c_str()));
            sqlString.Append(wxString::Format(", tip='%s'",txtSupravodiciTip->GetValue().c_str()));
            if(txtSupravodiciCistiPromjer->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", cisti_promjer=%lf",dv));
            if(txtSupravodiciPromjerIzolatora->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", promjer_izolator=%lf",dv));
            if(txtSupravodiciBrojNiti->GetValue().ToLong(&iv))
                sqlString.Append(wxString::Format(", broj_niti=%ld",iv));
            if(txtSupravodiciCuSC->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", cu_sc=%lf",dv));
            if(txtKritStruja3->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", \"kriticna_struja3T\"=%lf",dv));
            if(txtKritStruja5->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", \"kriticna_struja5T\"=%lf",dv));
            if(txtKritStruja7->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", \"kriticna_struja7T\"=%lf",dv));
            if(txtKritStruja9->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", \"kriticna_struja9T\"=%lf",dv));
            if(txtSupravodiciPromjerNiti->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", promjer_niti=%lf ",dv));
            sqlString.Append(wxString::Format(" WHERE id=%s",txtSupravodiciId->GetValue()));
        }
        else if(tp==TipPromjene::DODAVANJE||tp==TipPromjene::REAKTIVACIJA)
        {
            sqlString.Append("INSERT INTO supravodici(id,dobavljac,naziv,tip");

            valuesString.Append(wxString::Format(" VALUES(%s,%ld,'%s','%s'",txtSupravodiciId->GetValue(),
                        idDobavljac,txtSupravodiciNaziv->GetValue(),txtSupravodiciTip->GetValue()));
            if(txtSupravodiciCistiPromjer->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",cisti_promjer");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtSupravodiciPromjerIzolatora->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",promjer_izolator");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtSupravodiciBrojNiti->GetValue().ToLong(&iv))
            {
                sqlString.Append(",broj_niti");
                valuesString.Append(wxString::Format(", %ld",iv));
            }
            if(txtSupravodiciCuSC->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",cu_sc");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtKritStruja3->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",\"kriticna_struja3T\"");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtKritStruja5->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",\"kriticna_struja5T\"");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtKritStruja7->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",\"kriticna_struja7T\"");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtKritStruja9->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",\"kriticna_struja9T\"");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtSupravodiciPromjerNiti->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",promjer_niti");
                valuesString.Append(wxString::Format(", %lf",dv));
            }

            sqlString.Append(")");
            valuesString.Append(")");
            sqlString.Append(valuesString);
            std::cout << "SQL string: " << sqlString << std::endl;
        }

        if(parent->PrimiSQLZahtijev(sqlString)==false)
        {
            return;
        }
    };
    Destroy();
}


/********************************************************************************/
/********************** Dijalog unos shim ********************************/
/********************************************************************************/
DijalogUnosShim::DijalogUnosShim(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci)
: dlgUnosShim(NULL)
{
    wxVector<wxVariant>::iterator it;
    pqxx::result::iterator dobIt;
    this->parent=(PanelMaterijali *)parent;
    this->tp = tp;
    for(it=redak.begin(); it!=redak.end(); ++it)
        this->redak.push_back(*it);
    txtDobavljaciAdresaBazni=txtDobavljaciAdresa;
    if(redak.size()>=12)
    {
        if(redak[11].GetString()=="infinity")
        {
            for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
                comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));
            std::cout << "Dobavljac azuran: " << redak[11].GetString() << std::endl;
        }
        else
        {
            std::cout << "Stari dobavljac: " << redak[11].GetString() << std::endl;
        }
    }
    else
        for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
            comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));

}

// staviti On init event
void DijalogUnosShim::OnInit( wxInitDialogEvent& event )
{
    long id;
    int dlgRes;
    wxString podaciODobavljacu;
    if(!redak.empty())
    {
        txtShimId->SetValue(redak[0]);
        if(redak.size()>12)
        {
            txtShimNaziv->SetValue(redak[5]);
            txtShimTip->SetValue(redak[6]);
            txtShimMStruja->SetValue(redak[7]);
            txtShimSparivanje->SetValue(redak[8]);
            txtShimPromjer->SetValue(redak[9]);
            txtShimJakost->SetValue(redak[10]);
            comboDobavljaci->SetValue(redak[2].GetString()+" | "+redak[12].GetString());
            redak[2].GetString().ToLong(&id);
            podaciODobavljacu=parent->DohvatiPodatkeODobavljacu(id);
            txtDobavljaciAdresaBazni->SetValue(podaciODobavljacu);
        }
    }
    if(tp==TipPromjene::REAKTIVACIJA)
    {
        wxMessageDialog dijalog(this,wxT("Shim zavojnica je neaktivna. Ovim će se postupkom reaktivirati."),
                                    wxT("Reaktivacija neaktivne (uvjetno izbrisane) shim zavojnice"),  wxOK |  wxICON_INFORMATION);
        dijalog.SetOKLabel(wxT("&U redu"));
        dlgRes=dijalog.ShowModal();
    }
}
void DijalogUnosShim::OnCombo( wxCommandEvent& event )
{
    long int id;
    wxString podaci;
    wxComboBox *combo = wxDynamicCast(event.GetEventObject(),wxComboBox);
    wxStringTokenizer tokenizer(combo->GetValue(), "|");
    tokenizer.GetNextToken().ToLong(&id);
    podaci=parent->DohvatiPodatkeODobavljacu(id);
    if(txtDobavljaciAdresaBazni!=nullptr)
    {
        txtDobavljaciAdresaBazni->Clear();
        txtDobavljaciAdresaBazni->AppendText(podaci);
    }
}
void DijalogUnosShim::Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}
void DijalogUnosShim::GumbPritisnut( wxCommandEvent& event )
{
    wxString sqlString, valuesString, token;
    long iv;
    long int idDobavljac;
    double dv;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    wxStringTokenizer tokenizer(comboDobavljaci->GetValue(), "|");
    if(id==ID_UnosMaterijalaPrihvati)
    {
        token = tokenizer.GetNextToken().Trim();
        if(!token.ToLong(&idDobavljac))
        {
            wxMessageDialog dijalog(this,wxString(wxT("Potrebno je odabrati dobavljača.\nOdabrano: "))+token,
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if((txtShimNaziv->GetValue()=="")||(txtShimTip->GetValue()==""))
        {
            wxMessageDialog dijalog(this,wxT("Potrebno je unijeti barem naziv i tip shimming materijala."),
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if(tp==TipPromjene::AZURIRANJE)
        {
            sqlString.Append("UPDATE shim_zavojnice SET ");
            sqlString.Append(wxString::Format("naziv='%s'",txtShimNaziv->GetValue().c_str()));
            sqlString.Append(wxString::Format(", tip='%s'",txtShimTip->GetValue().c_str()));
            if(txtShimMStruja->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", max_struja=%lf",dv));
            if(txtShimSparivanje->GetValue()!="")
                sqlString.Append(wxString::Format(", sparivanje='%s'",txtShimSparivanje->GetValue()));
            if(txtShimPromjer->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", promjer=%lf",dv));
            if(txtShimJakost->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", jakost=%lf",dv));
            sqlString.Append(wxString::Format(" WHERE id=%s",txtShimId->GetValue()));
        }
        else if(tp==TipPromjene::DODAVANJE||tp==TipPromjene::REAKTIVACIJA)
        {
            sqlString.Append("INSERT INTO shim_zavojnice(id,dobavljac,naziv,tip");

            valuesString.Append(wxString::Format(" VALUES(%s,%ld,'%s','%s'",txtShimId->GetValue(),
                        idDobavljac,txtShimNaziv->GetValue(),txtShimTip->GetValue()));
            if(txtShimMStruja->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",max_struja");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtShimSparivanje->GetValue()!="")
            {
                sqlString.Append(",sparivanje");
                valuesString.Append(wxString::Format(", '%s'",txtShimSparivanje->GetValue()));
            }
            if(txtShimPromjer->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",promjer");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtShimJakost->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",jakost");
                valuesString.Append(wxString::Format(", %lf",dv));
            }

            sqlString.Append(")");
            valuesString.Append(")");
            sqlString.Append(valuesString);
            std::cout << "SQL string: " << sqlString << std::endl;
        }
        if(parent->PrimiSQLZahtijev(sqlString)==false)
        {
            return;
        }
    }
    Destroy();
}

/********************************************************************************/
/********************** Dijalog unos traka ********************************/
/********************************************************************************/
DijalogUnosTraka::DijalogUnosTraka(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci)
: dlgUnosTraka(NULL)
{
    wxVector<wxVariant>::iterator it;
    pqxx::result::iterator dobIt;
    this->parent=(PanelMaterijali *)parent;
    this->tp = tp;
    for(it=redak.begin(); it!=redak.end(); ++it)
        this->redak.push_back(*it);
    txtDobavljaciAdresaBazni=txtDobavljaciAdresa;
    if(redak.size()>=12)
    {
        if(redak[11].GetString()=="infinity")
        {
            for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
                comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));
            std::cout << "Dobavljac azuran: " << redak[12].GetString() << std::endl;
        }
        else
        {
            std::cout << "Stari dobavljac: " << redak[12].GetString() << std::endl;
        }
    }
    else
        for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
            comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));

}

// staviti On init event
void DijalogUnosTraka::OnInit( wxInitDialogEvent& event )
{
    long id;
    int dlgRes;
    wxString podaciODobavljacu;
    if(!redak.empty())
    {
        txtTrakeId->SetValue(redak[0]);
        if(redak.size()>=12)
        {
            txtTrakeNaziv->SetValue(redak[5]);
            txtTrakeSirina->SetValue(redak[6]);
            txtTrakeDebljina->SetValue(redak[7]);
            txtTrakeSupstrat->SetValue(redak[8]);
            txtTrakeStabilizator->SetValue(redak[9]);
            txtTrakeKritStruja->SetValue(redak[10]);

            comboDobavljaci->SetValue(redak[2].GetString()+" | "+redak[12].GetString());
            redak[2].GetString().ToLong(&id);
            podaciODobavljacu=parent->DohvatiPodatkeODobavljacu(id);
            txtDobavljaciAdresa->SetValue(podaciODobavljacu);
        }
    }
    if(tp==TipPromjene::REAKTIVACIJA)
    {
        wxMessageDialog dijalog(this,wxT("Traka je neaktivna. Ovim će se postupkom reaktivirati."),
                                    wxT("Reaktivacija neaktivne (uvjetno izbrisane) trake"),  wxOK |  wxICON_INFORMATION);
        dijalog.SetOKLabel(wxT("&U redu"));
        dlgRes=dijalog.ShowModal();
    }
}
void DijalogUnosTraka::OnCombo( wxCommandEvent& event )
{
    long int id;
    wxString podaci;
    wxComboBox *combo = wxDynamicCast(event.GetEventObject(),wxComboBox);
    wxStringTokenizer tokenizer(combo->GetValue(), "|");
    tokenizer.GetNextToken().ToLong(&id);
    podaci=parent->DohvatiPodatkeODobavljacu(id);
    if(txtDobavljaciAdresaBazni!=nullptr)
    {
        txtDobavljaciAdresaBazni->Clear();
        txtDobavljaciAdresaBazni->AppendText(podaci);
    }
}
void DijalogUnosTraka::Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}
void DijalogUnosTraka::GumbPritisnut( wxCommandEvent& event )
{
    wxString sqlString, valuesString, token;
    long iv;
    long int idDobavljac;
    double dv;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    wxStringTokenizer tokenizer(comboDobavljaci->GetValue(), "|");
    if(id==ID_UnosMaterijalaPrihvati)
    {
        token = tokenizer.GetNextToken().Trim();
        if(!token.ToLong(&idDobavljac))
        {
            wxMessageDialog dijalog(this,wxString(wxT("Potrebno je odabrati dobavljača.\nOdabrano: "))+token,
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if(txtTrakeNaziv->GetValue()=="")
        {
            wxMessageDialog dijalog(this,wxT("Potrebno je unijeti barem naziv shimming materijala."),
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if(tp==TipPromjene::AZURIRANJE)
        {
            sqlString.Append("UPDATE trake SET ");
            sqlString.Append(wxString::Format("naziv='%s'",txtTrakeNaziv->GetValue().c_str()));
            if(txtTrakeSirina->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", sirina=%lf",dv));
            if(txtTrakeDebljina->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", debljina=%lf",dv));
            if(txtTrakeSupstrat->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", supstrat=%lf",dv));
            if(txtTrakeStabilizator->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", stabilizator=%lf",dv));
            if(txtTrakeKritStruja->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", krit_struja=%lf",dv));
            sqlString.Append(wxString::Format(" WHERE id=%s",txtTrakeId->GetValue()));
        }
        else if(tp==TipPromjene::DODAVANJE||tp==TipPromjene::REAKTIVACIJA)
        {
            sqlString.Append("INSERT INTO trake(id,dobavljac,naziv");

            valuesString.Append(wxString::Format(" VALUES(%s,%ld,'%s'",txtTrakeId->GetValue(),
                        idDobavljac,txtTrakeNaziv->GetValue()));
            if(txtTrakeSirina->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",sirina");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtTrakeDebljina->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",debljina");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtTrakeSupstrat->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",supstrat");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtTrakeStabilizator->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",stabilizator");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtTrakeKritStruja->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",krit_struja");
                valuesString.Append(wxString::Format(", %lf",dv));
            }

            sqlString.Append(")");
            valuesString.Append(")");
            sqlString.Append(valuesString);
            std::cout << "SQL string: " << sqlString << std::endl;
        }
        if(parent->PrimiSQLZahtijev(sqlString)==false)
        {
            return;
        }
    }
    Destroy();
}


/********************************************************************************/
/********************** Dijalog unos stitova ********************************/
/********************************************************************************/
DijalogUnosStitova::DijalogUnosStitova(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci)
: dlgUnosStitova(NULL)
{
    wxVector<wxVariant>::iterator it;
    pqxx::result::iterator dobIt;
    this->parent=(PanelMaterijali *)parent;
    this->tp = tp;
    for(it=redak.begin(); it!=redak.end(); ++it)
        this->redak.push_back(*it);
    txtDobavljaciAdresaBazni=txtDobavljaciAdresa;
    if(redak.size()>=16)
    {
        if(redak[11].GetString()=="infinity")
        {
            for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
                comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));
            std::cout << "Dobavljac azuran: " << redak[15].GetString() << std::endl;
        }
        else
        {
            std::cout << "Stari dobavljac: " << redak[15].GetString() << std::endl;
        }
    }
    else
        for(dobIt=dobavljaci.begin(); dobIt!=dobavljaci.end(); ++dobIt)
            comboDobavljaci->Append(wxString::Format("%4ld | %s",wxVariant(dobIt["id"].c_str()).GetInteger(),dobIt["naziv"].c_str()));

}

// staviti On init event
void DijalogUnosStitova::OnInit( wxInitDialogEvent& event )
{
    long id;
    int dlgRes;
    wxString podaciODobavljacu;
    if(!redak.empty())
    {
        txtStitoviId->SetValue(redak[0]);
        if(redak.size()>=17)
        {
            txtStitoviNaziv->SetValue(redak[5]);
            txtStitoviMaterijal->SetValue(redak[6]);
            txtStitoviGustoca->SetValue(redak[7]);
            txtStitoviDebljinaZida->SetValue(redak[8]);
            txtStitoviGustocaMagToka->SetValue(redak[9]);
            txtStitoviFaktorZastite->SetValue(redak[10]);
            txtStitoviEfikasnostZastite->SetValue(redak[11]);
            txtKritTempZero->SetValue(redak[12]);
            txtKritTempSrednje->SetValue(redak[13]);
            txtStitoviUnutPromjer->SetValue(redak[14]);
            txtStitoviDuljina->SetValue(redak[15]);
            comboDobavljaci->SetValue(redak[2].GetString()+" | "+redak[17].GetString());
            redak[2].GetString().ToLong(&id);
            podaciODobavljacu=parent->DohvatiPodatkeODobavljacu(id);
            txtDobavljaciAdresa->SetValue(podaciODobavljacu);
        }
    }
    if(tp==TipPromjene::REAKTIVACIJA)
    {
        wxMessageDialog dijalog(this,wxT("Magnetski štit je neaktivan. Ovim će se postupkom reaktivirati."),
                                    wxT("Reaktivacija neaktivnog (uvjetno izbrisanog) magnetskog štita"),  wxOK |  wxICON_INFORMATION);
        dijalog.SetOKLabel(wxT("&U redu"));
        dlgRes=dijalog.ShowModal();
    }
}
void DijalogUnosStitova::OnCombo( wxCommandEvent& event )
{
    long int id;
    wxString podaci;
    wxComboBox *combo = wxDynamicCast(event.GetEventObject(),wxComboBox);
    wxStringTokenizer tokenizer(combo->GetValue(), "|");
    tokenizer.GetNextToken().ToLong(&id);
    podaci=parent->DohvatiPodatkeODobavljacu(id);
    if(txtDobavljaciAdresaBazni!=nullptr)
    {
        txtDobavljaciAdresaBazni->Clear();
        txtDobavljaciAdresaBazni->AppendText(podaci);
    }
}
void DijalogUnosStitova::Reset( wxCommandEvent& event )
{
    wxInitDialogEvent eventEmpty;
    OnInit(eventEmpty);
}
void DijalogUnosStitova::GumbPritisnut( wxCommandEvent& event )
{
    wxString sqlString, valuesString, token;
    long iv;
    long int idDobavljac;
    double dv;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    wxStringTokenizer tokenizer(comboDobavljaci->GetValue(), "|");
    if(id==ID_UnosMaterijalaPrihvati)
    {
        token = tokenizer.GetNextToken().Trim();
        if(!token.ToLong(&idDobavljac))
        {
            wxMessageDialog dijalog(this,wxString(wxT("Potrebno je odabrati dobavljača.\nOdabrano: "))+token,
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if((txtStitoviNaziv->GetValue()=="")||(txtStitoviMaterijal->GetValue()==""))
        {
            wxMessageDialog dijalog(this,wxT("Potrebno je unijeti barem naziv i vrstu shimming materijala."),
                                    wxT("Neispravan unos"),  wxOK |  wxICON_EXCLAMATION);
            dijalog.SetOKLabel(wxT("&U redu"));
            dijalog.ShowModal();
            return;
        }
        if(tp==TipPromjene::AZURIRANJE)
        {
            sqlString.Append("UPDATE stitovi SET ");
            sqlString.Append(wxString::Format("naziv='%s'",txtStitoviNaziv->GetValue().c_str()));
            sqlString.Append(wxString::Format(", materijal='%s'",txtStitoviMaterijal->GetValue().c_str()));
            if(txtStitoviGustoca->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", gustoca=%lf",dv));
            if(txtStitoviDebljinaZida->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", debljina_zida=%lf",dv));
            if(txtStitoviUnutPromjer->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", unut_promjer=%lf",dv));
            if(txtStitoviDuljina->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", duljina=%lf",dv));
            if(txtStitoviGustocaMagToka->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", gustoca_mag_toka=%lf",dv));
            if(txtStitoviFaktorZastite->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", faktor_zastite=%lf",dv));
            if(txtStitoviEfikasnostZastite->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", efikasnost_zastite=%lf",dv));
            if(txtKritTempZero->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", krit_temp_zero=%lf",dv));
            if(txtKritTempSrednje->GetValue().ToDouble(&dv))
                sqlString.Append(wxString::Format(", krit_temp_srednje=%lf",dv));
            sqlString.Append(wxString::Format(" WHERE id=%s",txtStitoviId->GetValue()));
        }
        else if(tp==TipPromjene::DODAVANJE||tp==TipPromjene::REAKTIVACIJA)
        {
            sqlString.Append("INSERT INTO stitovi(id,dobavljac,naziv,materijal");

            valuesString.Append(wxString::Format(" VALUES(%s,%ld,'%s','%s'",txtStitoviId->GetValue(),
                        idDobavljac,txtStitoviNaziv->GetValue(),txtStitoviMaterijal->GetValue()));
            if(txtStitoviGustoca->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",gustoca");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtStitoviDebljinaZida->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",debljina_zida");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtStitoviUnutPromjer->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",unut_promjer");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtStitoviDuljina->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",duljina");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtStitoviGustocaMagToka->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",gustoca_mag_toka");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtStitoviFaktorZastite->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",faktor_zastite");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtStitoviEfikasnostZastite->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",efikasnost_zastite");
                valuesString.Append(wxString::Format(", %lf",dv));
            }

            if(txtKritTempZero->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",krit_temp_zero");
                valuesString.Append(wxString::Format(", %lf",dv));
            }
            if(txtKritTempSrednje->GetValue().ToDouble(&dv))
            {
                sqlString.Append(",krit_temp_srednje");
                valuesString.Append(wxString::Format(", %lf",dv));
            }

            sqlString.Append(")");
            valuesString.Append(")");
            sqlString.Append(valuesString);
            std::cout << "SQL string: " << sqlString << std::endl;
        }
        if(parent->PrimiSQLZahtijev(sqlString)==false)
        {
            return;
        }
    }
    Destroy();
}
