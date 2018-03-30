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
#include <wx/statbmp.h>
#include <wx/scrolwin.h>
#include <wx/checkbox.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001
#define buttPanelDobavljaci 1002
#define buttPanelSkladista 1003
#define buttPanelMaterijali 1004
#define prijavaLozEnter 1005
#define prijavaOdustani 1006
#define prijavaPrihvati 1007
#define IDdobavljaciDodaj 1008
#define IDdobavljaciAzuriraj 1009
#define IDskladistaDodaj 1010
#define IDskladistaAzuriraj 1011
#define dlgUnosDobavljacaOdustani 1012
#define dlgUnosDobavljacaPrihvati 1013
#define dlgUnosSkladistaOdustani 1014
#define dlgUnosSkladistaPrihvati 1015
#define IDStranicenjeSmanjii 1016
#define IDStranicenjePovecaj 1017
#define IDmaterijaliDodaj 1018
#define IDmaterijaliAzuriraj 1019

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
		wxButton* buttMaterijali;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void AktivirajPanel( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxBoxSizer* bSizer1;
		wxBoxSizer* radniSizer;
		wxBoxSizer* bSizer101;
		
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Temporalna baza podataka"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1100,768 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
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
		
		GUIPanelDobavljaci( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelDobavljaci();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelSkladista
///////////////////////////////////////////////////////////////////////////////
class GUIPanelSkladista : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText13;
		wxComboBox* comboFilter;
		wxStaticLine* m_staticline4;
		wxStaticText* m_staticText14;
		wxDataViewListCtrl* tablicaSkladista;
		wxDataViewColumn* kolonaId;
		wxDataViewColumn* kolonaOznaka;
		wxDataViewColumn* kolonaLokacija;
		wxDataViewColumn* kolonaTelefon;
		wxDataViewColumn* kolonaTelefax;
		wxButton* btnDodaj;
		wxButton* btnAzuriraj;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCombo( wxCommandEvent& event ) { event.Skip(); }
		virtual void SelectionChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void PoziviDijalogUnosa( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxBoxSizer* stranicenjeSizer;
		
		GUIPanelSkladista( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelSkladista();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelPocetni
///////////////////////////////////////////////////////////////////////////////
class GUIPanelPocetni : public wxPanel 
{
	private:
	
	protected:
		wxScrolledWindow* m_scrolledWindow1;
		wxStaticBitmap* m_bitmap1;
	
	public:
		
		GUIPanelPocetni( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,600 ), long style = wxTAB_TRAVERSAL ); 
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
/// Class dlgUnosSkladista
///////////////////////////////////////////////////////////////////////////////
class dlgUnosSkladista : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText3;
		wxTextCtrl* dlgSkladisteId;
		wxButton* btnDlgSkladisteReset;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText4;
		wxTextCtrl* dlgSkladisteOznaka;
		wxStaticText* m_staticText41;
		wxTextCtrl* dlgSkladisteLokacija;
		wxStaticText* m_staticText42;
		wxTextCtrl* dlgSkladisteTelefon;
		wxStaticText* dlgSkladisteFaks;
		wxTextCtrl* dlgSkladisteTelefaks;
		wxStaticLine* m_staticline3;
		wxButton* btnOdustani;
		wxButton* btnPrihvati;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void Reset( wxCommandEvent& event ) { event.Skip(); }
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgUnosSkladista( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Unos dobavljača"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 480,310 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dlgUnosSkladista();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelStranicenje
///////////////////////////////////////////////////////////////////////////////
class GUIPanelStranicenje : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* sttStranicenje;
		wxButton* btnSmanji;
		wxTextCtrl* tbStranica;
		wxButton* btnPovecaj;
		
		// Virtual event handlers, overide them in your derived class
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIPanelStranicenje( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 150,50 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelStranicenje();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIPanelMaterijali
///////////////////////////////////////////////////////////////////////////////
class GUIPanelMaterijali : public wxPanel 
{
	private:
	
	protected:
		wxDataViewCtrl* dvcMaterijali;
		wxDataViewColumn* m_dataViewColumn1;
		wxDataViewColumn* m_dataViewColumn2;
		wxDataViewColumn* m_dataViewColumn3;
		wxDataViewColumn* m_dataViewColumn4;
		wxCheckBox* chkPrikaziNeaktivne;
		wxButton* btnDodaj;
		wxButton* btnAzuriraj;
		wxDataViewListCtrl* tablicaPovijesti;
		wxDataViewColumn* m_dataViewListColumn12;
		wxDataViewColumn* m_dataViewListColumn13;
		wxDataViewColumn* m_dataViewListColumn14;
		wxTextCtrl* txtMaterijalDetaljiL;
		wxTextCtrl* txtMaterijalDetaljiD;
		
		// Virtual event handlers, overide them in your derived class
		virtual void SelekcijaPromijenjena( wxDataViewEvent& event ) { event.Skip(); }
		virtual void NeaktivniChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void PoziviDijalogUnosa( wxCommandEvent& event ) { event.Skip(); }
		virtual void PovijestPromijenjena( wxDataViewEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIPanelMaterijali( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxTAB_TRAVERSAL ); 
		~GUIPanelMaterijali();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgUnosSupravodica
///////////////////////////////////////////////////////////////////////////////
class dlgUnosSupravodica : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText3;
		wxTextCtrl* dlgSupravodiciId;
		wxButton* btnDlgSupravodiciReset;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText4;
		wxTextCtrl* txtSupravodiciNaziv;
		wxStaticText* m_staticText43;
		wxTextCtrl* txtSupravodiciTip;
		wxStaticText* m_staticText41;
		wxTextCtrl* txtDobavljaciAdresa;
		wxComboBox* comboDobavljaci;
		wxStaticLine* m_staticline13;
		wxStaticText* m_staticText42;
		wxTextCtrl* txtSupravodiciCistiPromjer;
		wxStaticText* lblCistiPromjerDim;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText37;
		wxTextCtrl* txtSupravodiciPromjerNiti;
		wxStaticText* lblPromjerNitiDim;
		wxStaticText* m_staticText421;
		wxTextCtrl* txtSupravodiciPromjerIzolatora;
		wxStaticText* lblCistiPromjerIzolatoraDim;
		wxStaticLine* m_staticline121;
		wxStaticText* m_staticText371;
		wxTextCtrl* txtSupravodiciBrojNiti;
		wxStaticLine* m_staticline16;
		wxStaticText* m_staticText49;
		wxTextCtrl* txtSupravodiciCuSC;
		wxStaticText* m_staticText50;
		wxTextCtrl* txtKritStruja3;
		wxStaticText* lblKritStruja3Dim;
		wxStaticLine* m_staticline17;
		wxStaticText* m_staticText501;
		wxTextCtrl* txtKritStruja5;
		wxStaticText* lblKritStruja5Dim;
		wxStaticText* m_staticText502;
		wxTextCtrl* txtKritStruja7;
		wxStaticText* lblKritStruja7Dim;
		wxStaticLine* m_staticline171;
		wxStaticText* m_staticText5011;
		wxTextCtrl* txtKritStruja9;
		wxStaticText* lblKritStruja9Dim;
		wxStaticLine* m_staticline3;
		wxButton* btnOdustani;
		wxButton* btnPrihvati;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void Reset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCombo( wxCommandEvent& event ) { event.Skip(); }
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgUnosSupravodica( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Unos parametara za supravodiče"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,480 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dlgUnosSupravodica();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgUnosShim
///////////////////////////////////////////////////////////////////////////////
class dlgUnosShim : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText3;
		wxTextCtrl* dlgShimId;
		wxButton* btnDlgShimReset;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText4;
		wxTextCtrl* txtShimNaziv;
		wxStaticText* m_staticText43;
		wxTextCtrl* txtShimTip;
		wxStaticText* m_staticText41;
		wxTextCtrl* txtDobavljaciAdresa;
		wxComboBox* comboDobavljaci;
		wxStaticLine* m_staticline13;
		wxStaticText* m_staticText42;
		wxTextCtrl* txtShimMStruja;
		wxStaticText* lblMaxStrujaDim;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText37;
		wxTextCtrl* txtShimSparivanje;
		wxStaticText* m_staticText421;
		wxTextCtrl* txtShimPromjer;
		wxStaticText* lblPromjerDim;
		wxStaticLine* m_staticline121;
		wxStaticText* m_staticText371;
		wxTextCtrl* txtShimJakost;
		wxStaticText* lblJakostDim;
		wxStaticLine* m_staticline3;
		wxButton* btnOdustani;
		wxButton* btnPrihvati;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void Reset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCombo( wxCommandEvent& event ) { event.Skip(); }
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgUnosShim( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Unos parametara za shimming zavojnice"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,400 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dlgUnosShim();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgUnosTraka
///////////////////////////////////////////////////////////////////////////////
class dlgUnosTraka : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText3;
		wxTextCtrl* dlgTrakeId;
		wxButton* btnDlgTrakeReset;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText4;
		wxTextCtrl* txtTrakeNaziv;
		wxStaticText* m_staticText41;
		wxTextCtrl* txtDobavljaciAdresa;
		wxComboBox* comboDobavljaci;
		wxStaticLine* m_staticline13;
		wxStaticText* m_staticText42;
		wxTextCtrl* txtTrakeSirina;
		wxStaticText* lblSirinaDim;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText37;
		wxTextCtrl* txtTrakeDebljina;
		wxStaticText* lblDebljinaDim;
		wxStaticText* m_staticText421;
		wxTextCtrl* txtTrakeSupstrat;
		wxStaticLine* m_staticline121;
		wxStaticText* m_staticText371;
		wxTextCtrl* txtTrakeStabilizator;
		wxStaticText* m_staticText4211;
		wxTextCtrl* txtTrakeKritStruja;
		wxStaticText* lblKritStrujaDim;
		wxStaticLine* m_staticline3;
		wxButton* btnOdustani;
		wxButton* btnPrihvati;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void Reset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCombo( wxCommandEvent& event ) { event.Skip(); }
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgUnosTraka( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Unos parametara za trake"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 577,380 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dlgUnosTraka();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgUnosStitova
///////////////////////////////////////////////////////////////////////////////
class dlgUnosStitova : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText3;
		wxTextCtrl* dlgStitoviId;
		wxButton* btnDlgStitoviReset;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText4;
		wxTextCtrl* txtStitoviNaziv;
		wxStaticText* m_staticText43;
		wxTextCtrl* txtStitoviMaterijal;
		wxStaticText* m_staticText41;
		wxTextCtrl* txtDobavljaciAdresa;
		wxComboBox* comboDobavljaci;
		wxStaticLine* m_staticline13;
		wxStaticText* m_staticText42;
		wxTextCtrl* txtStitoviGustoca;
		wxStaticText* lblGustocaDim;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText37;
		wxTextCtrl* txtStitoviDebljinaZida;
		wxStaticText* lblDebljinaZidaDim;
		wxStaticText* m_staticText422;
		wxTextCtrl* txtStitoviUnutPromjer;
		wxStaticText* lblUnutPromjerDim;
		wxStaticLine* m_staticline121;
		wxStaticText* m_staticText372;
		wxTextCtrl* txtStitoviDuljina;
		wxStaticText* lblDuljinaDim;
		wxStaticText* m_staticText421;
		wxTextCtrl* txtStitoviGustocaMagToka;
		wxStaticText* lblCistiGustocaMagTokaDim;
		wxStaticText* m_staticText4211;
		wxTextCtrl* txtStitoviFaktorZastite;
		wxStaticLine* m_staticline1211;
		wxStaticText* m_staticText3711;
		wxTextCtrl* txtStitoviEfikasnostZastite;
		wxStaticText* lblEfikasnostZastiteDim;
		wxStaticText* m_staticText50;
		wxTextCtrl* txtKritTempZero;
		wxStaticText* lblKritTempZeroDim;
		wxStaticLine* m_staticline17;
		wxStaticText* m_staticText501;
		wxTextCtrl* txtKritTempSrednje;
		wxStaticText* lblKritTempSrDim;
		wxStaticLine* m_staticline3;
		wxButton* btnOdustani;
		wxButton* btnPrihvati;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void Reset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCombo( wxCommandEvent& event ) { event.Skip(); }
		virtual void GumbPritisnut( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgUnosStitova( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Unos parametara za štit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 720,580 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dlgUnosStitova();
	
};

#endif //__GUIFRAME_H__
