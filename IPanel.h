#ifndef IPANEL_INCLUDED
#define IPANEL_INCLUDED

#include <pqxx/pqxx>
#include <pqxx/except>
#include <wx/tokenzr.h>
#include <wx/msgdlg.h>
#include "GUIFrame.h"

enum TipPromjene:unsigned char{DODAVANJE=1, AZURIRANJE=2};

class IPanel
{
public:
    virtual void PostaviStranicu(int stranica)=0;
    virtual void AzurirajBazu(wxVector<wxVariant> redak)=0;
    virtual void DopuniBazu(wxVector<wxVariant> redak)=0;

protected:
    pqxx::connection *poveznica;
    pqxx::result r;
    int stranica;

private:

};



#endif // IPANEL_INCLUDED
