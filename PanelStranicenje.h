#ifndef PANELSTRANICENJE_H_INCLUDED
#define PANELSTRANICENJE_H_INCLUDED

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
};


#endif // PANELSTRANICENJE_H_INCLUDED
