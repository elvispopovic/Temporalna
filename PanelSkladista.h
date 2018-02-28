#ifndef PANELSKLADISTA_H
#define PANELSKLADISTA_H

#include "IPanel.h"

class PanelSkladista: public GUIPanelSkladista, public IPanel
{
    public:
        PanelSkladista(wxFrame *frame, std::string connString);
        virtual ~PanelSkladista();
        virtual void Test();

    protected:

    private:
};

#endif // PANELSKLADISTA_H
