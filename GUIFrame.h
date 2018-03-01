///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIFRAME_H__
#define __GUIFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/dataview.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001
#define buttPanelDobavljaci 1002
#define buttPanelSkladista 1003
#define prijavaLozEnter 1004
#define prijavaOdustani 1005
#define prijavaPrihvati 1006
#define IDdobavljaciDodaj 1007
#define IDdobavljaciAzuriraj 1008
#define dlgUnosDobavljacaOdustani 1009
#define dlgUnosDobavljacaPrihvati 1010
#define IDStranicenjeSmanjii 1011
#define IDStranicenjePovecaj 1012

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		wxPanel* kontrolniPanel;
		wxButton* buttDobavljaci;
		wxButton* buttSkladista;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void AktivirajPanel( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxBoxSizer* bSizer1;
		wxBoxSizer* radniSizer;
		wxBoxSizer* bSizer101;
		
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Temporalna baza podataka"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,768 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~GUIFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgPrijava
///////////////////////////////////////////////////////////////////////////////
class dlgPrijava : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel3;
		wxStaticText* m_staticText1;
		wxTextCtrl* txtCtrlKorisnik;
		wxStaticText* m_staticText2;
		wxTextCtrl* txtCtrlLozinka;
		wxStaticLine* m_staticline1;
		wxPanel* m_panel4;
		wxButton* btnCancel;
		wxButton* btnYes;
		
		// Virtual event handlers, overide them in your derived class
		virtual void PrijavaDijalogZatvoren( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgPrijava( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Prijavljivanje"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 250,160 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dlgPrijava();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelDobavljaci
///////////////////////////////////////////////////////////////////////////////
class GUIPanelDobavljaci : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText13;
		wxComboBox* comboFilter;
		wxStaticLine* m_staticline4;
		wxStaticText* m_staticText14;
		wxDataViewListCtrl* tablicaDobavljaci;
		wxDataViewColumn* kolonaId;
		wxDataViewColumn* kolonaNaziv;
		wxDataViewColumn* kolonaAdresa;
		wxDataViewColumn* kolonaTelefon;
		wxDataViewColumn* kolonaTelefon2;
		wxDataViewColumn* kolonaEmail;
		wxButton* btnDodaj;
		wxButton* btnAzuriraj;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCombo( wxCommandEvent& event ) { event.Skip(); }
		virtual void SelectionChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void PoziviDijalogUnosa( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxBoxSizer* stranicenjeSizer;
		
		GUIPanelDobavljaci( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,600 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelDobavljaci();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelSkladista
///////////////////////////////////////////////////////////////////////////////
class GUIPanelSkladista : public wxPanel 
{
	private:
	
	protected:
		wxDataViewListCtrl* m_dataViewListCtrl1;
		wxDataViewColumn* m_dataViewListColumn1;
		wxDataViewColumn* m_dataViewListColumn2;
		wxDataViewColumn* m_dataViewListColumn3;
	
	public:
		
		GUIPanelSkladista( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelSkladista();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelPocetni
///////////////////////////////////////////////////////////////////////////////
class GUIPanelPocetni : public wxPanel 
{
	private:
	
	protected:
	
	public:
		
		GUIPanelPocetni( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,300 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelPocetni();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgUnosDobavljaca
///////////////////////////////////////////////////////////////////////////////
class dlgUnosDobavljaca : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText3;
		wxTextCtrl* dlgDobavljaciId;
		wxButton* btnDlgDobavljaciReset;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText4;
		wxTextCtrl* dlgDobavljaciNaziv;
		wxStaticText* m_staticText41;
		wxTextCtrl* dlgDobavljaciAdresa;
		wxStaticText* m_staticText42;
		wxTextCtrl* dlgDobavljaciTelefon1;
		wxStaticText* m_staticText421;
		wxTextCtrl* dlgDobavljaciTelefon2;
		wxStaticText* m_staticText4211;
		wxTextCtrl* dlgDobavljaciEmail;
		wxStaticLine* m_staticline3;
		wxButton* btnOdustani;
		wxButton* btnPrihvati;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void Reset( wxCommandEvent& event ) { event.Skip(); }
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgUnosDobavljaca( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Unos dobavljača"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 480,350 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dlgUnosDobavljaca();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelStranicenje
///////////////////////////////////////////////////////////////////////////////
class GUIPanelStranicenje : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText11;
		wxButton* btnSmanji;
		wxTextCtrl* tbStranica;
		wxButton* btnPovecaj;
		
		// Virtual event handlers, overide them in your derived class
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIPanelStranicenje( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 120,50 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelStranicenje();
	
};

#endif //__GUIFRAME_H__
