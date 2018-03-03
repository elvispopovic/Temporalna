#ifndef PANELSTRANICENJE_H_INCLUDED
#define PANELSTRANICENJE_H_INCLUDED
#define VELICINA_STRANICE 15

#include "IPanel.h"

class PanelStranicenje : public GUIPanelStranicenje
{
    public:
        PanelStranicenje(wxFrame *frame, IPanel *parentPanel);
        virtual ~PanelStranicenje();
        void PostaviStranice(int brojStranica, int trenutna=1);
    protected:
        IPanel *parentPanel;
        virtual void GumbPritisnut( wxCommandEvent& event );
    private:
        int brojStranica, trenutna;
        void aktivacija();
};


#endif // PANELSTRANICENJE_H_INCLUDED
