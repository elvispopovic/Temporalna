#include "PanelPocetni.h"

PanelPocetni::PanelPocetni(wxFrame *frame): GUIPanelPocetni(frame)
{
    //ctor
}

PanelPocetni::~PanelPocetni()
{
    //dtor
}

void PanelPocetni::PostaviStranicu(int stranica)
{


}

void PanelPocetni::AzurirajBazu(wxVector<wxVariant> redak)
{

}
void PanelPocetni::DopuniBazu(wxVector<wxVariant> redak)
{

}
pqxx::connection* PanelPocetni::DajPoveznicu()
{

}

void PanelPocetni::Test()
{
    std::cout << "Aktivan je pocetni panel." << std::endl;
}
