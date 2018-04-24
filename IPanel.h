#ifndef IPANEL_INCLUDED
#define IPANEL_INCLUDED


#include <pqxx/pqxx>
#include <pqxx/except>
#include <wx/tokenzr.h>
#include <wx/msgdlg.h>
#include "GUIFrame.h"
#include "wx/base64.h"

enum TipPromjene:unsigned char{DODAVANJE=1, AZURIRANJE=2, REAKTIVACIJA=3};
enum VrstaMaterijala:unsigned char{MATERIJALI=0, SUPRAVODICI=1, SHIM_ZAVOJNICE=2, TRAKE=3, STITOVI=4};

char    const MATERIJALI_TABLICE[][24]={"supravodici","shim_zavojnice","trake","stitovi"};
wchar_t const SKLADISTA_ZAGLAVLJA[][20]={L"Šifra (id)",L"Oznaka",L"Lokacija",L"Telefon",L"Fax",L"Vrijeme od",L"Vrijeme do"};
wchar_t const SKLADISTA_ZAGLAVLJA2[][20]={L"Šifra (id)",L"Oznaka",L"Vrijeme od"};
wchar_t const DOBAVLJACI_ZAGLAVLJA[][20]={L"Šifra (id)",L"Naziv",L"Adresa",L"Telefon 1",L"Telefon 2",L"E-pošta",L"Vrijeme od",L"Vrijeme do"};
wchar_t const MATERIJALI_VRSTE[][24]={wxT("Tipovi materijala"),wxT("Supravodiči"),wxT("Shim zavojnice"),wxT("Trake"),wxT("Štitovi")};
wchar_t const MATERIJALI_VRSTE2[][24]={wxT("Tip"),wxT("Supravodič"),wxT("Shim"),wxT("Traka"),wxT("Štit")};


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
