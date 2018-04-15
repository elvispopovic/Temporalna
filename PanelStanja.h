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

    protected:
        pqxx::result razlicitiId;
        virtual void OnCombo( wxCommandEvent& event );
        virtual void SkladisteSelPromijenjena( wxDataViewEvent& event );

    private:
        PanelStranicenje *panelStranicenje;
        MaterijaliModel *modelMaterijala;
        virtual void osvjeziCombo();
        virtual void upisiRetke(pqxx::result r);
};

#endif // PANELSTANJA_H
