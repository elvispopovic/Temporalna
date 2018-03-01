#ifndef IPANEL_INCLUDED
#define IPANEL_INCLUDED

#include <pqxx/pqxx>
#include <pqxx/except>
#include <wx/tokenzr.h>
#include <wx/msgdlg.h>
#include "GUIFrame.h"


class IPanel
{
public:
    virtual void Test()=0;

protected:
    pqxx::connection *poveznica;
    pqxx::result r;
};



#endif // IPANEL_INCLUDED
