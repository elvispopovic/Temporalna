/***************************************************************
 * Name:      TemporalnaBazaMain.h
 * Purpose:   Defines Application Frame
 * Author:    EPC (elvpopovi@foi.hr)
 * Created:   2018-02-20
 * Copyright: EPC (www.nema_web_site_niti_ce_ga_ikada_imati.org)
 * License:
 **************************************************************/

#ifndef TEMPORALNABAZAMAIN_H
#define TEMPORALNABAZAMAIN_H



#include "TemporalnaBazaApp.h"


#include "GUIFrame.h"

class TemporalnaBazaFrame: public GUIFrame
{
    public:
        TemporalnaBazaFrame(wxFrame *frame);
        ~TemporalnaBazaFrame();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
};

#endif // TEMPORALNABAZAMAIN_H
