/***************************************************************
 * Name:      TemporalnaBazaApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EPC (elvpopovi@foi.hr)
 * Created:   2018-02-20
 * Copyright: EPC (www.nema_web_site_niti_ce_ga_ikada_imati.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "TemporalnaBazaApp.h"
#include "TemporalnaBazaMain.h"

IMPLEMENT_APP(TemporalnaBazaApp);

bool TemporalnaBazaApp::OnInit()
{
    TemporalnaBazaFrame* frame = new TemporalnaBazaFrame(0L);
    
    frame->Show();
    
    return true;
}
