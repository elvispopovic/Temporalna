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
#include "PanelPocetni.h"
#include "PanelSkladista.h"
#include "PanelDobavljaci.h"
#include "PanelMaterijali.h"

class DijalogPrijava;

class TemporalnaBazaFrame: public GUIFrame
{
    public:
        TemporalnaBazaFrame(wxFrame *frame);
        ~TemporalnaBazaFrame();
        void PrimiRezultatDijalogaPrijave(short rezultat){rezultatDijalogaPrijave=rezultat;};
        short CreateConnString(const char korisnik[20], const char lozinka[20]);
    private:
        std::string connString;

        wxPanel *panel;
        short rezultatDijalogaPrijave;
        virtual void OnAbout(wxCommandEvent& event);
        virtual void AktivirajPanel( wxCommandEvent& event );
};

class DijalogPrijava: public dlgPrijava
{
    public:
        DijalogPrijava(TemporalnaBazaFrame* parent);
        virtual ~DijalogPrijava();
    protected:
        virtual void PrijavaDijalogZatvoren( wxCloseEvent& event );
    private:
        TemporalnaBazaFrame *tbFrame;
        int rezultatPrijave;
        virtual void GumbPritisnut( wxCommandEvent& event );
        virtual void OnEnter( wxCommandEvent& event );

};

#endif // TEMPORALNABAZAMAIN_H
