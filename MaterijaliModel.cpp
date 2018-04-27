#include "MaterijaliModel.h"

MaterijaliCvor::MaterijaliCvor(MaterijaliCvor *roditelj, VrstaMaterijala vrsta,
    int id, const wxString naziv, const wxString vrijeme_od, const wxString vrijeme_do,
    int idDobavljaca, const wxString nazivDobavljaca, const wxString vrijeme_dobavljaca)
    {
        this->roditelj=roditelj;
        this->vrsta=vrsta;
        this->id=id;
        this->naziv=naziv;
        this->vrijeme_od=vrijeme_od;
        this->vrijeme_do=vrijeme_do;
        this->idDobavljaca=idDobavljaca;
        this->nazivDobavljaca=nazivDobavljaca;
        this->vrijeme_dobavljaca=vrijeme_dobavljaca;
        this->stanjeNaSkladistu=0.0;
        this->mjera="-";
        this->kontejner=false;
    }
MaterijaliCvor::MaterijaliCvor(MaterijaliCvor *roditelj, VrstaMaterijala vrsta)
{
    this->roditelj=roditelj;
    this->vrsta=vrsta;
    kontejner=true;
}

MaterijaliCvor::~MaterijaliCvor()
{
    for(int i=0; i<djeca.GetCount(); i++)
        delete djeca[i];
}

void MaterijaliCvor::DeleteChildren()
{
    for(int i=0; i<djeca.GetCount(); i++)
        delete djeca[i];
    djeca.Empty();
}


/****************/
/* model stabla */
/****************/


MaterijaliModel::MaterijaliModel(IPanel *panel)
{
    int i;
    MaterijaliCvor *grupa;
    this->panel = panel;
    korjen=new MaterijaliCvor(NULL,VrstaMaterijala::MATERIJALI);
    korjen->PostaviNaziv("Materijali");
    korjen->PostaviId(0);
    brojKolona=BROJ_KOLONA;
    for(i=1;i<=4;i++)
    {
        grupa=new MaterijaliCvor(korjen, (VrstaMaterijala)i);
        grupa->PostaviNaziv(MATERIJALI_VRSTE[i]);
        grupa->PostaviId(0);
        korjen->Append(grupa);
    }
}

MaterijaliModel::~MaterijaliModel()
{
    delete korjen;
}


void MaterijaliModel::Postavi(bool neaktivni)
{
    int i;
    bool akt;
    MaterijaliCvor *grupa, *cvor;
    pqxx::result rezultat;
    wxVector<long> id_ovi;

    pqxx::work txn(*(panel->DajPoveznicu()));

    for(i=0; i<4; i++)
    {
        grupa=korjen->GetNthChild(i);
        if(neaktivni)
        {
            rezultat = txn.exec("SELECT DISTINCT id FROM "+txn.esc(MATERIJALI_TABLICE[i])+" ORDER BY id");
            id_ovi.clear();
            for(pqxx::result::const_iterator red = rezultat.begin(); red !=rezultat.end(); ++red)
                id_ovi.push_back(wxVariant(red["id"].c_str()).GetInteger());

            for(wxVector<long>::iterator it = id_ovi.begin(); it !=id_ovi.end(); ++it)
            {
                rezultat = txn.exec("SELECT COUNT(id) AS \"n\" FROM "+txn.esc(MATERIJALI_TABLICE[i])+
                                    " WHERE id="+txn.quote(*it)+" AND vrijeme_do='infinity'::TIMESTAMP");
                akt=(wxVariant(rezultat[0]["n"].c_str()).GetInteger()==0)?false:true;

                rezultat = txn.exec("SELECT s.id AS sId,s.naziv AS sNaziv,s.vrijeme_od AS sVrijemeOd,s.vrijeme_do AS sVrijemeDo,\
                    d.id as dId,d.naziv AS dNaziv,d.vrijeme_od AS dVrijemeOd FROM "+
                    txn.esc(MATERIJALI_TABLICE[i])+
                    " s LEFT JOIN dobavljaci d ON s.dobavljac=d.id AND s.vrijeme_dobavljaca=d.vrijeme_od \
                    WHERE s.id="+txn.quote(*it)+" ORDER BY s.vrijeme_od DESC LIMIT 1");
                cvor = new MaterijaliCvor(grupa,(VrstaMaterijala)(i+1),
                                          wxVariant(rezultat[0]["sId"].c_str()).GetInteger(),
                                          akt?wxString("(A) ")+wxString::FromUTF8(rezultat[0]["sNaziv"].c_str()):
                                            wxString("(N) ")+wxString::FromUTF8(rezultat[0]["sNaziv"].c_str()),
                                          wxVariant(rezultat[0]["sVrijemeOd"].c_str()),
                                          wxVariant(rezultat[0]["sVrijemeDo"].c_str()),
                                          wxVariant(rezultat[0]["dId"].c_str()).GetInteger(),
                                          wxString::FromUTF8(rezultat[0]["dNaziv"].c_str()),
                                          wxVariant(rezultat[0]["dVrijemeOd"].c_str())
                                          );
                grupa->Append(cvor);
                wxDataViewItem roditelj((void*) grupa);
                wxDataViewItem dijete((void*) cvor);
                ItemAdded(roditelj,dijete);
            }
        }
        else
        {
            rezultat = txn.exec("SELECT s.id AS sId,s.naziv AS sNaziv,s.vrijeme_od AS sVrijemeOd,s.vrijeme_do AS sVrijemeDo,\
                d.id as dId,d.naziv AS dNaziv,d.vrijeme_od AS dVrijemeOd FROM "+
                txn.esc(MATERIJALI_TABLICE[i])+
                " s LEFT JOIN dobavljaci d ON s.dobavljac=d.id AND s.vrijeme_dobavljaca=d.vrijeme_od \
                WHERE s.vrijeme_do='infinity'::TIMESTAMP ORDER BY s.id");

            grupa=korjen->GetNthChild(i);
            for(pqxx::result::const_iterator red = rezultat.begin(); red !=rezultat.end(); ++red)
            {
                cvor = new MaterijaliCvor(grupa,(VrstaMaterijala)(i+1),
                                          wxVariant(red["sId"].c_str()).GetInteger(),
                                          wxString::FromUTF8(red["sNaziv"].c_str()),
                                          wxVariant(red["sVrijemeOd"].c_str()),
                                          wxVariant(red["sVrijemeDo"].c_str()),
                                          wxVariant(red["dId"].c_str()).GetInteger(),
                                          wxString::FromUTF8(red["dNaziv"].c_str()),
                                          wxVariant(red["dVrijemeOd"].c_str())
                                          );
                grupa->Append(cvor);
                wxDataViewItem roditelj((void*) grupa);
                wxDataViewItem dijete((void*) cvor);
                ItemAdded(roditelj,dijete);
            }
        }

    }
    txn.commit();
}

void MaterijaliModel::PostaviZaSkladiste(int skladisteId, wxString vrijeme_skladista, bool neaktivni=false)
{
    int i;
    bool akt;
    MaterijaliCvor *grupa, *podgrupa, *cvor;
    pqxx::result rezultat_m, rezultat, rezultat2;
    wxVector<long> id_ovi;
    pqxx::work txn(*(panel->DajPoveznicu()));
    try
    {
        for(i=0; i<4; i++) //4 grupe
        {
            grupa=korjen->GetNthChild(i);
/* prvo se ucitavaju samo id-ovi i vremena kao dio kljuca materijala za odredjenu grupu, materijal i skladiste moraju biti aktivni */
            rezultat_m = txn.exec("SELECT mat.id AS mId FROM stanje LEFT JOIN "+txn.esc(MATERIJALI_TABLICE[i])+
                                " mat ON stanje.materijal=mat.id AND stanje.vrijeme_materijala=mat.vrijeme_od \
 LEFT JOIN skladista skl ON stanje.skladiste=skl.id AND stanje.vrijeme_skladista=skl.vrijeme_od \
 WHERE stanje.skladiste="+txn.quote(skladisteId)+" AND mat.id IS NOT NULL GROUP BY mId");

            std::cout << "Velicina rezultat_m: " << rezultat_m.size() << std::endl;

            if(!rezultat_m.empty())
            for(pqxx::result::const_iterator mIt=rezultat_m.begin(); mIt!=rezultat_m.end(); ++mIt)
            {

            std::cout << "id: " << mIt["mId"].c_str() <<  std::endl;
/* ucitavaju se svi materijali za odredjenu grupu i izabrano skladiste, ali samo zadnji vremenski gledano */
            rezultat = txn.exec("SELECT mat.id AS mId, mat.naziv AS mNaziv, mat.vrijeme_od AS mVrijeme, stanje.kolicina AS stKol, mk.mjera AS mjera,\
 stanje.vrijeme_od AS stVrijemeOd, stanje.vrijeme_do AS stVrijeme_do, coalesce(stanje.kolicina,0.0) AS kol, d.id AS dId, d.naziv AS dNaziv\
 FROM stanje LEFT JOIN "+txn.esc(MATERIJALI_TABLICE[i])+" mat ON stanje.materijal=mat.id AND stanje.vrijeme_materijala=mat.vrijeme_od\
 LEFT JOIN dobavljaci d ON mat.dobavljac=d.id AND mat.vrijeme_dobavljaca=d.vrijeme_od LEFT JOIN mjere_kol mk ON stanje.mjera=mk.id \
 WHERE stanje.skladiste="+txn.quote(skladisteId)+
 " AND stanje.materijal="+txn.esc(mIt["mId"].c_str())+" AND mat.id IS NOT NULL ORDER BY stVrijemeOd DESC LIMIT 1");

                if(wxString(rezultat[0]["stVrijeme_do"].c_str())=="infinity")
                    akt=true;
                else
                    akt=false;
                if(neaktivni||akt)
                {

                    podgrupa=new MaterijaliCvor(grupa, (VrstaMaterijala)(i+1));
                    podgrupa->PostaviNaziv(wxString(wxT("id: "))+rezultat[0]["mId"].c_str()+
                                           (akt?"(A)":"(N)")+
                                           ", "+rezultat[0]["mNaziv"].c_str()+"\n"+
                                           wxString(wxT("Dobavljač: "))+rezultat[0]["dId"].c_str()+
                                           ", "+rezultat[0]["dNaziv"].c_str()+"\n"+
                                           wxString(wxT("Količina: "))+rezultat[0]["stKol"].c_str()+" "+rezultat[0]["mjera"].c_str());
                    podgrupa->PostaviId(wxVariant(rezultat[0]["mId"].c_str()).GetInteger());
                    podgrupa->PostaviVrijeme_do(rezultat[0]["stVrijeme_do"].c_str());

                    grupa->Append(podgrupa);
                    wxDataViewItem roditelj((void*) grupa);
                    wxDataViewItem dijete((void*) podgrupa);
                    ItemAdded(roditelj,dijete);
                /* ucitava se povijest tog materijala */
                rezultat2 = txn.exec("SELECT m.naziv AS mNaziv,d.id AS dId, d.naziv AS dNaziv,d.vrijeme_od AS dVrijemeOd,\
st.vrijeme_od AS stVrijemeOd, st.vrijeme_do AS stVrijemeDo,COALESCE(st.kolicina,0.0) AS stKol, mk.skraceno AS skr FROM stanje st LEFT JOIN "+
                                     txn.esc(MATERIJALI_TABLICE[i])+" m ON st.materijal=m.id AND st.vrijeme_materijala=m.vrijeme_od \
LEFT JOIN dobavljaci d ON m.dobavljac=d.id AND m.vrijeme_dobavljaca=d.vrijeme_od LEFT JOIN mjere_kol mk ON st.mjera=mk.id \
WHERE st.skladiste="+txn.quote(skladisteId)+" AND st.materijal = "+ txn.esc(rezultat[0]["mId"].c_str())+" AND m.id IS NOT NULL ORDER BY stVrijemeOd");


                    for(pqxx::result::const_iterator red2 = rezultat2.begin(); red2 !=rezultat2.end(); ++red2)
                    {
                        cvor=new MaterijaliCvor(podgrupa,(VrstaMaterijala)(i+1),
                                            wxVariant(rezultat[0]["mId"].c_str()).GetInteger(),
                                            wxString::FromUTF8(red2["mNaziv"].c_str())+"\n"+wxString::FromUTF8(red2["dNaziv"].c_str()),
                                            wxVariant(red2["stVrijemeOd"].c_str()),
                                            wxVariant(red2["stVrijemeDo"].c_str()),
                                            wxVariant(red2["dId"].c_str()).GetInteger(),
                                            wxString::FromUTF8(red2["dNaziv"].c_str()),
                                            wxVariant(red2["dVrijemeOd"].c_str())
                                            );
                        cvor->PostaviStanje(wxVariant(red2["stKol"].c_str()).GetDouble());
                        cvor->PostaviMjeru(red2["skr"].c_str());
                        podgrupa->Append(cvor);
                        wxDataViewItem roditelj2((void*) podgrupa);
                        wxDataViewItem dijete2((void*) cvor);
                        ItemAdded(roditelj2,dijete2);
                        std::cout << "Naziv: " << wxString::FromUTF8(red2["mNaziv"].c_str()) << ", dobavljac: " << wxString::FromUTF8(red2["dNaziv"].c_str()) <<
                        "skladiste: " << skladisteId << std::endl;

                    }
                }

/*
            if(rezultat.empty())
                std::cout << "Grupa " << MATERIJALI_TABLICE[i] << ", id: -, n: 0" << std::endl;
            else
                for(pqxx::result::const_iterator red = rezultat.begin(); red !=rezultat.end(); ++red)
                    std::cout << "Grupa " << MATERIJALI_TABLICE[i] <<", id: " << red["mId"] << ", n: " << rezultat.size() << std::endl;
            //std::cout << "Grupa " << MATERIJALI_TABLICE[i] << " - id: " << rezultat[0]["id"].c_str() << ", n: " << rezultat[0]["n"].c_str() << std::endl;
*/
            }//kraj petlje materijala
        }
        txn.commit();
    }
    catch(const pqxx::sql_error& e)
    {
        txn.abort();
        wxMessageDialog dijalog(NULL,wxString(wxT("Transakcija nije uspjela. Razlog:\n"))+e.what(),
                                    wxT("Postavljanje materijala u drill-down tablicu"),  wxOK |  wxICON_ERROR);
        dijalog.SetOKLabel(wxT("&U redu"));
        dijalog.ShowModal();
    }
}

void MaterijaliModel::Obrisi()
{
    int i;
    MaterijaliPoljeCvorova grupe = korjen->GetChildren();
    for(int j=0; j<grupe.GetCount(); j++)
        grupe.Item(j)->DeleteChildren();
}

/* obavezne metode wxDataViewModel - interface */
unsigned int MaterijaliModel::GetColumnCount() const
{
    return brojKolona;
}

void MaterijaliModel::SetColumnCount(unsigned int colNum)
{
    brojKolona=colNum;
}

wxString MaterijaliModel::GetColumnType( unsigned int col ) const
{
    if(col<2)
        return wxT("long");
    else
        return wxT("string");
}

void MaterijaliModel::GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const
{
    wxASSERT(item.IsOk());
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    switch(col)
    {
        case 0:
            if(cvor->JeLiKontejner())
                if(cvor->DajNaziv()=="")
                    variant=MATERIJALI_VRSTE[cvor->DajVrstu()];
                else
                    variant=cvor->DajNaziv();
            else
                variant=cvor->DajNaziv();
            break;
        case 1:
            variant=wxString::Format(wxT("%d"),cvor->DajId()); break;
        case 2:
            variant=cvor->DajNazivDobavljaca();  break;
        case 3:
            variant=wxString::Format(wxT("%d"),cvor->DajIdDobavljaca()); break;
        case 4:
            variant=cvor->DajVrijeme_od(); break;
        case 5:
            variant=cvor->DajVrijeme_do(); break;
        case 6:
            variant=wxString::Format(wxT("%.2lf"),cvor->DajStanje()); break;
        case 7:
            variant=cvor->DajMjeru(); break;
        default: wxLogError("Greska u pozivu kolone za GetValue %d.",col);
    }
}

bool MaterijaliModel::SetValue( const wxVariant &variant, const wxDataViewItem &item, unsigned int col )
{
    return false;
}

wxDataViewItem MaterijaliModel::GetParent( const wxDataViewItem &item ) const
{
    if (!item.IsOk())
        return wxDataViewItem(0);
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    if (cvor == korjen)
        return wxDataViewItem(0);

    return wxDataViewItem( (void*) cvor->GetParent() );

}

bool MaterijaliModel::IsContainer( const wxDataViewItem &item ) const
{
    if (!item.IsOk()) return true;
    MaterijaliCvor *cvor = (MaterijaliCvor*) item.GetID();
    return cvor->JeLiKontejner();
}

unsigned int MaterijaliModel::GetChildren( const wxDataViewItem &parent,wxDataViewItemArray &array ) const
{
    MaterijaliCvor *cvor = (MaterijaliCvor*) parent.GetID();
    if (!cvor)
    {
        array.Add( wxDataViewItem( (void*) korjen ) );
        return 1;
    }
    if (cvor->GetChildCount() == 0)
        return 0;
    unsigned int brojDjece = cvor->GetChildren().GetCount();
    for (unsigned int pos = 0; pos < brojDjece; pos++)
    {
        MaterijaliCvor *dijete = cvor->GetChildren().Item( pos );
        array.Add( wxDataViewItem( (void*) dijete ) );
    }

    return brojDjece;
}


