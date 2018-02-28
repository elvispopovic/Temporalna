#include "PanelPocetni.h"

PanelPocetni::PanelPocetni(wxFrame *frame): GUIPanelPocetni(frame)
{
    //ctor
}

PanelPocetni::~PanelPocetni()
{
    //dtor
}

void PanelPocetni::Test()
{
    std::cout << "Aktivan je pocetni panel." << std::endl;
}
