#ifndef PANELDOBAVLJACI_H
#define PANELDOBAVLJACI_H

#include "IPanel.h"
#include "PanelStranicenje.h"

enum TipPromjene:unsigned char{DODAVANJE=1, AZURIRANJE=2};

class PanelDobavljaci: public GUIPanelDobavljaci, public IPanel
{
    public:
        PanelDobavljaci(wxFrame *frame, std::string connString);
        virtual ~PanelDobavljaci();
        virtual void AzurirajBazu(wxVector<wxVariant> redak);
        virtual void DopuniBazu(wxVector<wxVariant> redak);
        virtual void Test();
        virtual pqxx::connection* DajPoveznicu();

    protected:
        pqxx::result razlicitiId;
        wxDataViewColumn *kolonaVrijeme_od, *kolonaVrijeme_do;
        virtual void OnCombo( wxCommandEvent& event );
        virtual void SelectionChanged( wxDataViewEvent& event );
        virtual void PoziviDijalogUnosa( wxCommandEvent& event );

    private:
        std::string connString;
        PanelStranicenje *panelStranicenje;
        virtual void osvjeziCombo();
        virtual void upisiRetke(pqxx::result r);
        virtual void dodajPrazniRedak();
};


/* dijalog za unos */
class DijalogUnos: public dlgUnosDobavljaca
{
    private:
        wxVector<wxVariant> redak;
        TipPromjene tp;
    protected:
        PanelDobavljaci *parent;
    public:
        DijalogUnos(wxWindow* parent, wxVector<wxVariant> redak, TipPromjene tp);
        virtual ~DijalogUnos();
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};

#endif // PANELDOBAVLJACI_H
