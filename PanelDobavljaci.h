#ifndef PANELDOBAVLJACI_H
#define PANELDOBAVLJACI_H

#include "IPanel.h"
#include "PanelStranicenje.h"

class PanelDobavljaci: public GUIPanelDobavljaci, public IPanel
{
    public:
        PanelDobavljaci(wxFrame *frame, std::string connString);
        virtual ~PanelDobavljaci();
        virtual void AzurirajBazu(wxVector<wxVariant> redak);
        virtual void DopuniBazu(wxVector<wxVariant> redak);
        virtual bool PosaljiSQLZahtijev(wxString sqlString){};
        virtual pqxx::connection* DajPoveznicu();
        virtual void PostaviStranicu(int stranica);

    protected:
        pqxx::result razlicitiId;
        wxDataViewColumn *kolonaVrijeme_od, *kolonaVrijeme_do;
        virtual void OnCombo( wxCommandEvent& event );
        virtual void SelectionChanged( wxDataViewEvent& event );
        virtual void PoziviDijalogUnosa( wxCommandEvent& event );

    private:
        PanelStranicenje *panelStranicenje;
        virtual void osvjeziCombo();
        virtual void upisiRetke(pqxx::result r);
        virtual void dodajPrazniRedak();
};


/* dijalog za unos */
class DijalogUnosDobavljaca: public dlgUnosDobavljaca
{
    private:
        wxVector<wxVariant> redak;
        TipPromjene tp;
    protected:
        PanelDobavljaci *parent;
    public:
        DijalogUnosDobavljaca(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp);
        virtual ~DijalogUnosDobavljaca();
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};

#endif // PANELDOBAVLJACI_H
