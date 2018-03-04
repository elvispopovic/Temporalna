#ifndef MODELSTABLA_H
#define MODELSTABLA_H
#include "GUIFrame.h"

class CvorStabla;
WX_DEFINE_ARRAY_PTR( CvorStabla*, CvorStablaPoljePokazivaca );

class CvorStabla
{
    public:
        wxString    naziv;
        wxString    oznaka;
        int         vrijednost;
        wxString    napomena;
        CvorStabla(CvorStabla *roditelj, const wxString naziv, const wxString oznaka, int vrijednost, const wxString napomena);
        CvorStabla(CvorStabla *roditelj, const wxString nazivGrane);
        ~CvorStabla();
        bool JeLiKontejner();
        CvorStabla* GetParent() { return roditelj; }
        CvorStablaPoljePokazivaca& GetChildren(){ return djeca; }
        CvorStabla* GetNthChild( unsigned int n ){ return djeca.Item( n ); }
        void Insert( CvorStabla* dijete, unsigned int n){ djeca.Insert( dijete, n); }
        void Append( CvorStabla* dijete ){ djeca.Add( dijete ); }
        unsigned int GetChildCount() const { return djeca.GetCount(); }


    protected:

    private:
        bool jeLiKontejner;
        CvorStabla *roditelj;
        CvorStablaPoljePokazivaca djeca;

};

class ModelStabla : public wxDataViewModel
{
    public:
        ModelStabla();
        virtual ~ModelStabla();
        wxString DajNaziv( const wxDataViewItem &item ) const;
        wxString DajOznaku( const wxDataViewItem &item ) const;
        int DajVrijednost( const wxDataViewItem &item ) const;
        wxString DajNapomenu( const wxDataViewItem &item ) const;


        virtual unsigned int GetColumnCount() const;
        virtual wxString GetColumnType( unsigned int col ) const;
        virtual void GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const;
        virtual bool SetValue( const wxVariant &variant, const wxDataViewItem &item, unsigned int col );
        virtual wxDataViewItem GetParent( const wxDataViewItem &item ) const;
        virtual bool IsContainer( const wxDataViewItem &item ) const;
        virtual unsigned int GetChildren( const wxDataViewItem &parent,wxDataViewItemArray &array ) const;

    protected:

    private:
        CvorStabla* korjen;
        CvorStabla *grupa1, *grupa2, *grupa3, *grupa3a, *grupa3b;
};

#endif // MODELSTABLA_H
