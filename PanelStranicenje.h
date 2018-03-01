#ifndef PANELSTRANICENJE_H_INCLUDED
#define PANELSTRANICENJE_H_INCLUDED

#include "IPanel.h"

class PanelStranicenje : public GUIPanelStranicenje
{
    public:
        PanelStranicenje(wxFrame *frame, IPanel *parentPanel);
        virtual ~PanelStranicenje();
        void PostaviStranice(int najveca, int trenutna);
    protected:
        virtual void GumbPritisnut( wxCommandEvent& event );
    private:
        int najveca, trenutna;
};


#endif // PANELSTRANICENJE_H_INCLUDED
