#ifndef PANELSKLADISTA_H
#define PANELSKLADISTA_H

#include "IPanel.h"

class PanelSkladista: public GUIPanelSkladista, public IPanel
{
    public:
        PanelSkladista(wxFrame *frame, std::string connString);
        virtual ~PanelSkladista();
        virtual void PostaviStranicu(int stranica);
        virtual void AzurirajBazu(wxVector<wxVariant> redak);
        virtual void DopuniBazu(wxVector<wxVariant> redak);
        virtual void Test();

    protected:

    private:
};

#endif // PANELSKLADISTA_H
