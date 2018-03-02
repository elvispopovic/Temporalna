#include "PanelStranicenje.h"

PanelStranicenje::PanelStranicenje(wxFrame *frame, IPanel *parentPanel) : GUIPanelStranicenje(frame)
{
    trenutna=0;
    brojStranica=1;
    this->parentPanel=parentPanel;
    wxString vrijednost = wxString::Format(wxT("%i/%i"),trenutna+1,brojStranica);
    tbStranica->SetValue(vrijednost);
}

PanelStranicenje::~PanelStranicenje()
{

}

void PanelStranicenje::PostaviStranice(int brojStranica, int trenutna)
{
    wxString vrijednost = wxString::Format(wxT("%i/%i"),trenutna+1,brojStranica);
    this->brojStranica=brojStranica;
    this->trenutna=trenutna;
    tbStranica->SetValue(vrijednost);
    std::cout << "Broj stranica: " << brojStranica << std::endl;
}

void PanelStranicenje::GumbPritisnut( wxCommandEvent& event )
{
    wxString vrijednost;
    wxWindowID id=wxDynamicCast(event.GetEventObject(),wxButton)->GetId();
    switch(id)
    {
    case IDStranicenjeSmanjii:
        if(trenutna>0)
            trenutna--;
        break;
    case IDStranicenjePovecaj:
        if(trenutna<brojStranica-1)
            trenutna++;
        break;
    }
    vrijednost = wxString::Format(wxT("%i/%i"),trenutna+1,brojStranica);
    parentPanel->PostaviStranicu(trenutna);
    tbStranica->SetValue(vrijednost);
}
