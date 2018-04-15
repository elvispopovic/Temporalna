#ifndef MATERIJALIMODEL_H
#define MATERIJALIMODEL_H
#include "IPanel.h"
#define BROJ_KOLONA 4

class MaterijaliCvor;
WX_DEFINE_ARRAY_PTR( MaterijaliCvor*, MaterijaliPoljeCvorova );


class MaterijaliCvor
{
    public:
        MaterijaliCvor(MaterijaliCvor *roditelj, VrstaMaterijala vrsta,
                       int id, const wxString naziv, const wxString vrijeme_od, const wxString vrijeme_do,
                       int idDobavljaca, const wxString nazivDobavljaca, const wxString vrijeme_dobavljaca);
        MaterijaliCvor(MaterijaliCvor *roditelj, VrstaMaterijala vrsta);
        ~MaterijaliCvor();

        VrstaMaterijala DajVrstu() {return vrsta;};
        void PostaviVrstu(VrstaMaterijala vrsta) {this->vrsta=vrsta;};
        void PostaviStanje(double stanje) {this->stanjeNaSkladistu=stanje;};
        void PostaviMjeru(const wxString &mjera){this->mjera=mjera;};

        int DajId(){return id;};
        void PostaviId(int id){this->id=id;};

        wxString DajNaziv() const{return naziv;};
        void PostaviNaziv(const wxString &naziv){this->naziv =naziv;};

        int DajIdDobavljaca(){return idDobavljaca;};
        void PostaviIdDobavljaca(int id){this->idDobavljaca=id;};

        wxString DajNazivDobavljaca() const{return nazivDobavljaca;};
        void PostaviNazivDobavljaca(const wxString &naziv){this->nazivDobavljaca=naziv;};

        wxString DajVrijeme_od() const{return vrijeme_od;};
        void PostaviVrijeme_od(const wxString &vrijeme_od){this->vrijeme_od=vrijeme_od;};

        wxString DajVrijeme_do() const{return vrijeme_do;};
        void PostaviVrijeme_do(const wxString &vrijeme_do){this->vrijeme_do=vrijeme_do;};

        wxString DajVrijemeDobavljaca() const{return vrijeme_dobavljaca;};
        void PostaviVrijemeDobavljaca(const wxString &vrijeme_dobavljaca){this->vrijeme_dobavljaca=vrijeme_dobavljaca;};

        double DajStanje(){return stanjeNaSkladistu;};
        wxString DajMjeru() const{return mjera;};

        MaterijaliCvor* GetParent() { return roditelj; }
        MaterijaliPoljeCvorova& GetChildren(){ return djeca; }
        MaterijaliCvor* GetNthChild( unsigned int n ){ return djeca.Item( n ); }
        unsigned int GetChildCount() const { return djeca.GetCount(); }
        void Insert( MaterijaliCvor* dijete, unsigned int n){ djeca.Insert( dijete, n); }
        void Append( MaterijaliCvor* dijete ){ djeca.Add( dijete ); }
        void DeleteChildren();
        bool JeLiKontejner(){return kontejner;};
    private:
        VrstaMaterijala vrsta;
        int id;
        wxString naziv;
        wxString vrijeme_od;
        wxString vrijeme_do;
        int idDobavljaca;
        wxString nazivDobavljaca;
        wxString vrijeme_dobavljaca;

        /* samo za potrebe stanja, odnosi se na odabrano skladiste */
        double stanjeNaSkladistu;
        wxString mjera;

        bool kontejner, aktivni;
        MaterijaliCvor *roditelj;
        MaterijaliPoljeCvorova djeca;

};

class MaterijaliModel : public wxDataViewModel
{
    public:
        MaterijaliModel(IPanel *panel);
        virtual ~MaterijaliModel();
        virtual void Postavi(bool neaktivni=false);
        virtual void PostaviZaSkladiste(int skladisteId, wxString vrijeme_skladista, bool neaktivni);
        virtual void Obrisi();
        virtual unsigned int GetColumnCount() const;
        virtual void SetColumnCount(unsigned int colNum);
        virtual wxString GetColumnType( unsigned int col ) const;
        virtual void GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const;
        virtual bool SetValue( const wxVariant &variant, const wxDataViewItem &item, unsigned int col );
        virtual wxDataViewItem GetParent( const wxDataViewItem &item ) const;
        virtual bool IsContainer( const wxDataViewItem &item ) const;
        virtual unsigned int GetChildren( const wxDataViewItem &parent,wxDataViewItemArray &array ) const;

    protected:
        IPanel *panel;
    private:
        int brojKolona;
        MaterijaliCvor *korjen;
};

#endif // MATERIJALIMODEL_H
