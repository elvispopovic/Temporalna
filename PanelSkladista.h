#ifndef PANELSKLADISTA_H
#define PANELSKLADISTA_H

#include "IPanel.h"
#include "PanelStranicenje.h"

class PanelSkladista : public GUIPanelSkladista, public IPanel
{
    public:
        PanelSkladista(wxFrame *frame, std::string connString);
        virtual ~PanelSkladista();
        virtual void AzurirajBazu(wxVector<wxVariant> redak);
        virtual void DopuniBazu(wxVector<wxVariant> redak);
        virtual bool PrimiSQLZahtijev(wxString sqlString){};
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
class DijalogUnosSkladista : public dlgUnosSkladista
{
    private:
        wxVector<wxVariant> redak;
        TipPromjene tp;
    protected:
        PanelSkladista *parent;
    public:
        DijalogUnosSkladista(IPanel* parent, wxVector<wxVariant> redak, TipPromjene tp);
        virtual ~DijalogUnosSkladista();
        virtual void OnInit( wxInitDialogEvent& event );
        virtual void Reset( wxCommandEvent& event );
        virtual void GumbPritisnut( wxCommandEvent& event );
};

#endif // PANELSKLADISTA_H
