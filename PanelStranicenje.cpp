#include "PanelStranicenje.h"

PanelStranicenje::PanelStranicenje(wxFrame *frame, IPanel *parentPanel) : GUIPanelStranicenje(frame)
{
    trenutna=1;
    wxString vrijednost = wxString::Format(wxT("%i"),trenutna);
    tbStranica->SetValue(vrijednost);
}

PanelStranicenje::~PanelStranicenje()
{

}

void PanelStranicenje::PostaviStranice(int najveca, int trenutna=1)
{
    wxString vrijednost = wxString::Format(wxT("%i"),trenutna);
    this->najveca=najveca;
    this->trenutna=trenutna;
    tbStranica->SetValue(vrijednost);
}

void PanelStranicenje::GumbPritisnut( wxCommandEvent& event )
{
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    switch(id)
    {
    case IDStranicenjeSmanjii:
        std::cout << "Smanjivanje stranice" << std::endl;
        break;
    case IDStranicenjePovecaj:
        std::cout << "Povecavanje stranice" << std::endl;
        break;
    }
}
