#ifndef PANELMATERIJALI_H
#define PANELMATERIJALI_H
#include "MaterijaliModel.h"

class PanelMaterijali : public GUIPanelMaterijali, public IPanel
{
    public:
        PanelMaterijali(wxFrame *frame, std::string connString);
        virtual ~PanelMaterijali();
        virtual void AzurirajBazu(wxVector<wxVariant> redak){};
        virtual void DopuniBazu(wxVector<wxVariant> redak){};
        virtual bool PrimiSQLZahtijev(wxString sqlString);
        virtual pqxx::connection* DajPoveznicu();
        virtual void PostaviStranicu(int stranica);
        virtual pqxx::result DohvatiPovijest(long id) const;
        virtual pqxx::result DohvatiPodvrstu(long id, const wxString & vrijeme_od) const;
        virtual wxString DohvatiPodatkeODobavljacu(long id) const;
        virtual wxString DohvatiVrijemeDobavljaca(long id) const;

    protected:
        virtual void SelekcijaPromijenjena( wxDataViewEvent& event );
        virtual void PovijestPromijenjena( wxDataViewEvent& event );
        virtual void NeaktivniChecked( wxCommandEvent& event );
        virtual void PoziviDijalogUnosa( wxCommandEvent& event );
    private:
        MaterijaliModel *modelMaterijala;
        virtual wxVector<wxVariant> dohvatiPodatkeMaterijala(MaterijaliCvor *cvor) const;
};
/* genericka za unos */
class DijalogUnosMaterijala
{
    protected:
        PanelMaterijali *parent;
        TipPromjene tp;
        wxVector<wxVariant> redak;
        long dobavljacId;
        wxString vrijemeDobavljaca;
    public:
        void OnInit( wxInitDialogEvent& event ){};
};


/* dijalog za unos */
class DijalogUnosSupravodica: public DijalogUnosMaterijala, public dlgUnosSupravodica
{
    public:
        DijalogUnosSupravodica(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci);
        ~DijalogUnosSupravodica(){};
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void OnCombo( wxCommandEvent& event );
        void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};

class DijalogUnosShim: public DijalogUnosMaterijala, public dlgUnosShim
{
    public:
        DijalogUnosShim(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci);
        ~DijalogUnosShim(){};
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void OnCombo( wxCommandEvent& event );
        void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};

class DijalogUnosTraka: public DijalogUnosMaterijala, public dlgUnosTraka
{
    public:
        DijalogUnosTraka(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci);
        ~DijalogUnosTraka(){};
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void OnCombo( wxCommandEvent& event );
        void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};


class DijalogUnosStitova: public DijalogUnosMaterijala, public dlgUnosStitova
{
    public:
        DijalogUnosStitova(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, const pqxx::result& dobavljaci);
        ~DijalogUnosStitova(){};
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void OnCombo( wxCommandEvent& event );
        void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};


#endif // PANELMATERIJALI_H
