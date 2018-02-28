#ifndef PANELPOCETNI_INCLUDED
#define PANELPOCETNI_INCLUDED


#include "IPanel.h"

class PanelPocetni: public GUIPanelPocetni, public IPanel
{
    public:
        PanelPocetni(wxFrame *frame);
        virtual ~PanelPocetni();
        virtual void Test();

    protected:

    private:
};


#endif // PANELPOCETNI_INCLUDED
