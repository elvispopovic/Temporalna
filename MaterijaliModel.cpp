#include "MaterijaliModel.h"

wchar_t MATERIJALI_VRSTE[][24]={wxT("Tipovi materijala"),wxT("Supravodiči"),wxT("Shim zavojnice"),wxT("Trake"),wxT("Štitovi")};
wchar_t MATERIJALI_VRSTE2[][24]={wxT("Tip"),wxT("Supravodič"),wxT("Shim"),wxT("Traka"),wxT("Štit")};
char MATERIJALI_TABLICE[][24]={"supravodici","shim_zavojnice","trake","stitovi"};

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
    brojKolona=BROJ_KOLONA;
    for(i=1;i<=4;i++)
    {
        grupa=new MaterijaliCvor(korjen, (VrstaMaterijala)i);
        grupa->PostaviNaziv(MATERIJALI_TABLICE[i-1]);
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
                                          akt?wxVariant("(A) "+wxString(rezultat[0]["sNaziv"].c_str())):
                                            wxVariant("(N) "+wxString(rezultat[0]["sNaziv"].c_str())),
                                          wxVariant(rezultat[0]["sVrijemeOd"].c_str()),
                                          wxVariant(rezultat[0]["sVrijemeDo"].c_str()),
                                          wxVariant(rezultat[0]["dId"].c_str()).GetInteger(),
                                          wxVariant(rezultat[0]["dNaziv"].c_str()),
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
                                          wxVariant(red["sNaziv"].c_str()),
                                          wxVariant(red["sVrijemeOd"].c_str()),
                                          wxVariant(red["sVrijemeDo"].c_str()),
                                          wxVariant(red["dId"].c_str()).GetInteger(),
                                          wxVariant(red["dNaziv"].c_str()),
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
                variant=MATERIJALI_VRSTE[cvor->DajVrstu()];
            else
                variant=cvor->DajNaziv();
            break;
        case 1:
            variant=wxString::Format(wxT("%d"),cvor->DajId()); break;
        case 2:
            variant=cvor->DajNazivDobavljaca();  break;
        case 3:
            variant=wxString::Format(wxT("%d"),cvor->DajIdDobavljaca()); break;
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


