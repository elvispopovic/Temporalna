#ifndef PANELSTANJA_H
#define PANELSTANJA_H

#include "IPanel.h"
#include "MaterijaliModel.h"
#include "PanelStranicenje.h"


class PanelStanja : public GUIPanelStanja, public IPanel
{
    public:
        PanelStanja(wxFrame *frame, std::string connString);
        virtual ~PanelStanja();
        virtual void AzurirajBazu(wxVector<wxVariant> redak);
        virtual void DopuniBazu(wxVector<wxVariant> redak);
        virtual bool PrimiSQLZahtijev(wxString sqlString);
        virtual pqxx::connection* DajPoveznicu();
        virtual void PostaviStranicu(int stranica);
        pqxx::result DohvatiMaterijale(char vrsta);
        pqxx::result DohvatiMjere();
        pqxx::result ProvjeriPostojanje(long skladisteID, long materijalID);

    protected:
        pqxx::result razlicitiId;
        virtual void OnCombo( wxCommandEvent& event );
        virtual void SkladisteSelPromijenjena( wxDataViewEvent& event );
        virtual void MaterijalSelPromijenjena( wxDataViewEvent& event );
        virtual void NeaktivniChecked( wxCommandEvent& event );
        virtual void PoziviDijalogUnosa( wxCommandEvent& event );

    private:
        PanelStranicenje *panelStranicenje;
        MaterijaliModel *modelMaterijala;
        virtual void osvjeziCombo();
        virtual void upisiRetke(pqxx::result r);
};

/* dijalog za unos */
class DijalogUnosStanja: public dlgUnosStanja
{
    private:
        wxVector<wxVariant> redak;
        VrstaMaterijala vrstaMaterijala;
        TipPromjene tp;

    protected:
        PanelStanja *parent;
        long materijalId;
        virtual void OnCombo( wxCommandEvent& event );
    public:
        DijalogUnosStanja(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp, VrstaMaterijala vrstaMaterijala);
        virtual ~DijalogUnosStanja();
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};


#endif // PANELSTANJA_H
