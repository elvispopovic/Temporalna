#ifndef PANELAUDIT_H
#define PANELAUDIT_H

#include "IPanel.h"
#include "PanelStranicenje.h"

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

class PanelAudit : public GUIPanelAudit, public IPanel
{
    public:
        PanelAudit(wxFrame *frame, std::string connString);
        virtual ~PanelAudit();
        virtual void AzurirajBazu(wxVector<wxVariant> redak);
        virtual void DopuniBazu(wxVector<wxVariant> redak);
        virtual bool PrimiSQLZahtijev(wxString sqlString){};
        virtual pqxx::connection* DajPoveznicu();
        virtual void PostaviStranicu(int stranica);
    protected:

    private:
        PanelStranicenje *panelStranicenje;
        virtual void osvjezi();
        virtual void upisiRetke(pqxx::result r);
};

#endif // PANELAUDIT_H
