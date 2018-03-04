#include "ModelStabla.h"

CvorStabla::CvorStabla(CvorStabla *roditelj, const wxString naziv, const wxString oznaka, int vrijednost, const wxString napomena)
{
    this->roditelj=roditelj;
    this->naziv=naziv;
    this->oznaka=oznaka;
    this->vrijednost=vrijednost;
    this->napomena=napomena;
    jeLiKontejner=false;
}

CvorStabla::CvorStabla(CvorStabla *roditelj, const wxString nazivGrane)
{
    this->roditelj=roditelj;
    this->naziv=nazivGrane;
    this->oznaka.Clear();
    this->vrijednost=-1;
    this->napomena.Clear();
    jeLiKontejner=true;
}

CvorStabla::~CvorStabla()
{
    int i;
    for (i = 0; i < djeca.GetCount(); i++)
    {
        CvorStabla *dijete = djeca[i];
        delete dijete;
    }
}

bool CvorStabla::JeLiKontejner()
{
    return jeLiKontejner;
}


ModelStabla::ModelStabla()
{
    //ctor
    korjen = new CvorStabla( NULL, "Stablo" );
    grupa1 = new CvorStabla(korjen, "Grupa 1");
        grupa1->Append(new CvorStabla(grupa1,wxT("Sadržaj1"),"O_G1_1",11,wxT("Napomena uz sadržaj 1")));
        grupa1->Append(new CvorStabla(grupa1,wxT("Sadržaj2"),"O_G1_2",12,wxT("Napomena uz sadržaj 2")));
        grupa1->Append(new CvorStabla(grupa1,wxT("Sadržaj3"),"O_G1_3",13,wxT("Napomena uz sadržaj 3")));
    korjen->Append(grupa1);
    grupa2 = new CvorStabla(korjen, "Grupa 2");
        grupa2->Append(new CvorStabla(grupa2,wxT("Sadržaj1"),"O_G2_1",21,wxT("Napomena uz sadržaj 1")));
        grupa2->Append(new CvorStabla(grupa2,wxT("Sadržaj2"),"O_G2_2",22,wxT("Napomena uz sadržaj 2")));
        grupa2->Append(new CvorStabla(grupa2,wxT("Sadržaj3"),"O_G2_3",23,wxT("Napomena uz sadržaj 3")));
        grupa2->Append(new CvorStabla(grupa2,wxT("Sadržaj4"),"O_G2_4",24,wxT("Napomena uz sadržaj 4")));
    korjen->Append(grupa2);
    grupa3 = new CvorStabla(korjen, "Grupa 3");
        grupa3a = new CvorStabla(grupa3, "Grupa 3a");
            grupa3a->Append(new CvorStabla(grupa3a,wxT("Sadržaj1"),"O_G3a_1",311,wxT("Napomena uz sadržaj 1")));
            grupa3a->Append(new CvorStabla(grupa3a,wxT("Sadržaj2"),"O_G3a_2",312,wxT("Napomena uz sadržaj 2")));
            grupa3a->Append(new CvorStabla(grupa3a,wxT("Sadržaj3"),"O_G3a_1",313,wxT("Napomena uz sadržaj 3")));
        grupa3->Append(grupa3a);
        grupa3b = new CvorStabla(grupa3, "Grupa 3b");
            grupa3b->Append(new CvorStabla(grupa3b,wxT("Sadržaj1"),"O_G3b_1",321,wxT("Napomena uz sadržaj 1")));
            grupa3b->Append(new CvorStabla(grupa3b,wxT("Sadržaj2"),"O_G3b_2",322,wxT("Napomena uz sadržaj 2")));
            grupa3b->Append(new CvorStabla(grupa3b,wxT("Sadržaj3"),"O_G3b_1",323,wxT("Napomena uz sadržaj 3")));
        grupa3->Append(grupa3b);
    korjen->Append(grupa3);

}

ModelStabla::~ModelStabla()
{
    //dtor
    delete korjen;
}

wxString ModelStabla::DajNaziv( const wxDataViewItem &item ) const
{
    CvorStabla *cvor = (CvorStabla*) item.GetID();
    if (!cvor)
        return wxEmptyString;
    return cvor->naziv;
}
wxString ModelStabla::DajOznaku( const wxDataViewItem &item ) const
{
    CvorStabla *cvor = (CvorStabla*) item.GetID();
    if (!cvor)
        return wxEmptyString;
    return cvor->oznaka;
}
int ModelStabla::DajVrijednost( const wxDataViewItem &item ) const
{
    CvorStabla *cvor = (CvorStabla*) item.GetID();
    if (!cvor)
        return 0;
    return cvor->vrijednost;
}
wxString ModelStabla::DajNapomenu( const wxDataViewItem &item ) const
{
    CvorStabla *cvor = (CvorStabla*) item.GetID();
    if (!cvor)
        return wxEmptyString;
    return cvor->napomena;
}

/* obavezno iz sucelja */
unsigned int ModelStabla::GetColumnCount() const
{
    return 4;
}
wxString ModelStabla::GetColumnType( unsigned int col ) const
{
    if(col==2)
        return wxT("long");
    else
        return wxT("string");
}

void ModelStabla::GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const
{
    wxASSERT(item.IsOk());
    CvorStabla *cvor = (CvorStabla*) item.GetID();
    switch(col)
    {
        case 0: variant=cvor->naziv; break;
        case 1: variant=cvor->oznaka; break;
        //case 2: variant=(long)cvor->vrijednost; break;
        case 2: variant=wxString::Format(wxT("%d"),cvor->vrijednost); break;
        case 3: variant=cvor->napomena; break;
        default: wxLogError("Greska u pozivu kolone %d.",col);
    }
}

bool ModelStabla::SetValue( const wxVariant &variant, const wxDataViewItem &item, unsigned int col )
{
    wxASSERT(item.IsOk());
    CvorStabla *cvor = (CvorStabla*) item.GetID();
    switch(col)
    {
        case 0: cvor->naziv = variant.GetString(); return true;
        case 1: cvor->oznaka = variant.GetString(); return true;
        case 2: cvor->vrijednost = variant.GetLong(); return true;
        case 3: cvor->napomena = variant.GetString(); return true;
        default: wxLogError("Greska u pozivu kolone %d.",col);
    }
    return false;
}
wxDataViewItem ModelStabla::GetParent( const wxDataViewItem &item ) const
{
    if (!item.IsOk())
        return wxDataViewItem(0);
    CvorStabla *cvor = (CvorStabla*) item.GetID();
    if (cvor == korjen)
        return wxDataViewItem(0);

    return wxDataViewItem( (void*) cvor->GetParent() );
}

bool ModelStabla::IsContainer( const wxDataViewItem &item ) const
{
    if (!item.IsOk()) return true;

    CvorStabla *cvor = (CvorStabla*) item.GetID();
    return cvor->JeLiKontejner();
}
unsigned int ModelStabla::GetChildren( const wxDataViewItem &parent,wxDataViewItemArray &array ) const
{
    CvorStabla *cvor = (CvorStabla*) parent.GetID();
    if (!cvor)
    {
        array.Add( wxDataViewItem( (void*) korjen ) );
        return 1;
    }
    if (cvor->GetChildCount() == 0)
    {
        return 0;
    }
    unsigned int brojDjece = cvor->GetChildren().GetCount();
    for (unsigned int pos = 0; pos < brojDjece; pos++)
    {
        CvorStabla *dijete = cvor->GetChildren().Item( pos );
        array.Add( wxDataViewItem( (void*) dijete ) );
    }

    return brojDjece;
}
