#include "PanelSkladista.h"

PanelSkladista::PanelSkladista(wxFrame *frame, std::string connString): GUIPanelSkladista(frame)
{
    poveznica=new pqxx::connection(connString);

    //ctor
}

PanelSkladista::~PanelSkladista()
{
    //dtor
    poveznica->disconnect();
    delete poveznica;
}

void PanelSkladista::Test()
{
    std::cout << "Aktivan je Panel skladista." << std::endl;
}
