#ifndef IPANEL_INCLUDED
#define IPANEL_INCLUDED



#include <pqxx/pqxx>
#include <pqxx/except>
#include <wx/tokenzr.h>
#include <wx/msgdlg.h>
#include "GUIFrame.h"

enum TipPromjene:unsigned char{DODAVANJE=1, AZURIRANJE=2, REAKTIVACIJA=3};
enum VrstaMaterijala:unsigned char{MATERIJALI=0, SUPRAVODICI=1, SHIM_ZAVOJNICE=2, TRAKE=3, STITOVI=4};

class IPanel
{
public:
    virtual void PostaviStranicu(int stranica)=0;
    virtual void AzurirajBazu(wxVector<wxVariant> redak)=0;
    virtual void DopuniBazu(wxVector<wxVariant> redak)=0;
    virtual bool PrimiSQLZahtijev(wxString sqlString)=0;
    virtual pqxx::connection* DajPoveznicu()=0;
protected:
    pqxx::connection *poveznica;
    pqxx::result r;
    int stranica;

private:

};

#endif // IPANEL_INCLUDED
