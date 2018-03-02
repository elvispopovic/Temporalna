#ifndef PANELPOCETNI_INCLUDED
#define PANELPOCETNI_INCLUDED


#include "IPanel.h"

class PanelPocetni: public GUIPanelPocetni, public IPanel
{
    public:
        PanelPocetni(wxFrame *frame);
        virtual ~PanelPocetni();
        virtual void PostaviStranicu(int stranica);
        virtual void AzurirajBazu(wxVector<wxVariant> redak);
        virtual void DopuniBazu(wxVector<wxVariant> redak);
        virtual void Test();

    protected:

    private:
};


#endif // PANELPOCETNI_INCLUDED
