/***************************************************************
 * Name:      TestWx2Main.h
 * Purpose:   Defines Application Frame
 * Author:    EPC (elvpopovi@foi.hr)
 * Created:   2018-02-21
 * Copyright: EPC (www.nema_web_site_niti_ce_ga_ikada_imati.org)
 * License:
 **************************************************************/

#ifndef TESTWX2MAIN_H
#define TESTWX2MAIN_H



#include "TestWx2App.h"
#include "GUIFrame.h"
#include "ModelStabla.h"

class TestWx2Frame: public GUIFrame
{
    public:
        TestWx2Frame(wxFrame *frame);
        ~TestWx2Frame();
    private:
        ModelStabla *modelStabla;
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
    protected:
        virtual void PromjenaSelekcije( wxDataViewEvent& event );
};

#endif // TESTWX2MAIN_H
