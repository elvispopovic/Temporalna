/***************************************************************
 * Name:      TestWx2App.cpp
 * Purpose:   Code for Application Class
 * Author:    EPC (elvpopovi@foi.hr)
 * Created:   2018-02-21
 * Copyright: EPC (www.nema_web_site_niti_ce_ga_ikada_imati.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "TestWx2App.h"
#include "TestWx2Main.h"

IMPLEMENT_APP(TestWx2App);

bool TestWx2App::OnInit()
{
    TestWx2Frame* frame = new TestWx2Frame(0L);
    
    frame->Show();
    
    return true;
}
