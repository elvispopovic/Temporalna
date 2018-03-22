///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	
	mbar->Append( fileMenu, wxT("&File") ); 
	
	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );
	
	mbar->Append( helpMenu, wxT("&Help") ); 
	
	this->SetMenuBar( mbar );
	
	statusBar = this->CreateStatusBar( 3, wxST_SIZEGRIP, wxID_ANY );
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	radniSizer = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer1->Add( radniSizer, 1, wxEXPAND, 5 );
	
	kontrolniPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxRAISED_BORDER|wxTAB_TRAVERSAL );
	bSizer101 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( kontrolniPanel, wxID_ANY, wxT("Odjeljci") ), wxVERTICAL );
	
	sbSizer1->SetMinSize( wxSize( 200,-1 ) ); 
	
	sbSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	buttDobavljaci = new wxButton( sbSizer1->GetStaticBox(), buttPanelDobavljaci, wxT("Dobavljači"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( buttDobavljaci, 0, wxALL, 5 );
	
	buttSkladista = new wxButton( sbSizer1->GetStaticBox(), buttPanelSkladista, wxT("Skladišta"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( buttSkladista, 0, wxALL, 5 );
	
	buttMaterijali = new wxButton( sbSizer1->GetStaticBox(), buttPanelMaterijali, wxT("Materijali"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( buttMaterijali, 0, wxALL, 5 );
	
	
	bSizer101->Add( sbSizer1, 0, wxALL, 5 );
	
	
	kontrolniPanel->SetSizer( bSizer101 );
	kontrolniPanel->Layout();
	bSizer1->Add( kontrolniPanel, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	buttDobavljaci->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::AktivirajPanel ), NULL, this );
	buttSkladista->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::AktivirajPanel ), NULL, this );
	buttMaterijali->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::AktivirajPanel ), NULL, this );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( idMenuQuit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( idMenuAbout, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	buttDobavljaci->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::AktivirajPanel ), NULL, this );
	buttSkladista->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::AktivirajPanel ), NULL, this );
	buttMaterijali->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::AktivirajPanel ), NULL, this );
	
}

dlgPrijava::dlgPrijava( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_panel3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_panel3, wxID_ANY, wxT("Korisnik"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText1->Wrap( -1 );
	bSizer6->Add( m_staticText1, 0, wxALL|wxTOP, 5 );
	
	txtCtrlKorisnik = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !txtCtrlKorisnik->HasFlag( wxTE_MULTILINE ) )
	{
	txtCtrlKorisnik->SetMaxLength( 16 );
	}
	#else
	txtCtrlKorisnik->SetMaxLength( 16 );
	#endif
	txtCtrlKorisnik->SetForegroundColour( wxColour( 255, 145, 0 ) );
	txtCtrlKorisnik->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	txtCtrlKorisnik->SetToolTip( wxT("Unesi korisničko ime baze podataka") );
	
	bSizer6->Add( txtCtrlKorisnik, 1, 0, 5 );
	
	
	bSizer5->Add( bSizer6, 1, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( m_panel3, wxID_ANY, wxT("Lozinka"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText2->Wrap( -1 );
	bSizer7->Add( m_staticText2, 0, wxALL|wxTOP, 5 );
	
	txtCtrlLozinka = new wxTextCtrl( m_panel3, prijavaLozEnter, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD|wxTE_PROCESS_ENTER );
	#ifdef __WXGTK__
	if ( !txtCtrlLozinka->HasFlag( wxTE_MULTILINE ) )
	{
	txtCtrlLozinka->SetMaxLength( 16 );
	}
	#else
	txtCtrlLozinka->SetMaxLength( 16 );
	#endif
	txtCtrlLozinka->SetForegroundColour( wxColour( 255, 145, 0 ) );
	txtCtrlLozinka->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	txtCtrlLozinka->SetToolTip( wxT("Unesi lozinku korisnika baze podataka") );
	
	bSizer7->Add( txtCtrlLozinka, 1, 0, 5 );
	
	
	bSizer5->Add( bSizer7, 1, wxEXPAND|wxTOP, 5 );
	
	
	m_panel3->SetSizer( bSizer5 );
	m_panel3->Layout();
	bSizer5->Fit( m_panel3 );
	bSizer3->Add( m_panel3, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	btnCancel = new wxButton( m_panel4, prijavaOdustani, wxT("Odustani"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( btnCancel, 0, wxLEFT, 60 );
	
	btnYes = new wxButton( m_panel4, prijavaPrihvati, wxT("Prihvati"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( btnYes, 0, wxLEFT, 5 );
	
	
	m_panel4->SetSizer( bSizer4 );
	m_panel4->Layout();
	bSizer4->Fit( m_panel4 );
	bSizer3->Add( m_panel4, 0, wxALL, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dlgPrijava::PrijavaDijalogZatvoren ) );
	txtCtrlLozinka->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( dlgPrijava::OnEnter ), NULL, this );
	btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgPrijava::GumbPritisnut ), NULL, this );
	btnYes->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgPrijava::GumbPritisnut ), NULL, this );
}

dlgPrijava::~dlgPrijava()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dlgPrijava::PrijavaDijalogZatvoren ) );
	txtCtrlLozinka->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( dlgPrijava::OnEnter ), NULL, this );
	btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgPrijava::GumbPritisnut ), NULL, this );
	btnYes->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgPrijava::GumbPritisnut ), NULL, this );
	
}

GUIPanelDobavljaci::GUIPanelDobavljaci( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Odabir povijesti entiteta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer11->Add( m_staticText13, 0, wxALL, 5 );
	
	comboFilter = new wxComboBox( this, wxID_ANY, wxT("Aktivni"), wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, 0 ); 
	comboFilter->SetSelection( 0 );
	bSizer11->Add( comboFilter, 0, wxALL, 5 );
	
	
	bSizer13->Add( bSizer11, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer13->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Tablica dobavljača"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer13->Add( m_staticText14, 0, wxALL, 5 );
	
	tablicaDobavljaci = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxDV_MULTIPLE|wxDV_ROW_LINES );
	kolonaId = tablicaDobavljaci->AppendTextColumn( wxT("Šifra (id)") );
	kolonaNaziv = tablicaDobavljaci->AppendTextColumn( wxT("Naziv") );
	kolonaAdresa = tablicaDobavljaci->AppendTextColumn( wxT("Adresa") );
	kolonaTelefon = tablicaDobavljaci->AppendTextColumn( wxT("Telefon 1") );
	kolonaTelefon2 = tablicaDobavljaci->AppendTextColumn( wxT("Telefon 2") );
	kolonaEmail = tablicaDobavljaci->AppendTextColumn( wxT("E-pošta") );
	bSizer13->Add( tablicaDobavljaci, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	stranicenjeSizer = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer31->Add( stranicenjeSizer, 1, wxEXPAND, 5 );
	
	
	bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btnDodaj = new wxButton( this, IDdobavljaciDodaj, wxT("Dodaj"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( btnDodaj, 0, wxALL, 5 );
	
	btnAzuriraj = new wxButton( this, IDdobavljaciAzuriraj, wxT("Ažuriraj"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( btnAzuriraj, 0, wxALL, 5 );
	
	
	bSizer13->Add( bSizer31, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer13 );
	this->Layout();
	
	// Connect Events
	comboFilter->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GUIPanelDobavljaci::OnCombo ), NULL, this );
	tablicaDobavljaci->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelDobavljaci::SelectionChanged ), NULL, this );
	btnDodaj->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelDobavljaci::PoziviDijalogUnosa ), NULL, this );
	btnAzuriraj->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelDobavljaci::PoziviDijalogUnosa ), NULL, this );
}

GUIPanelDobavljaci::~GUIPanelDobavljaci()
{
	// Disconnect Events
	comboFilter->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GUIPanelDobavljaci::OnCombo ), NULL, this );
	tablicaDobavljaci->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelDobavljaci::SelectionChanged ), NULL, this );
	btnDodaj->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelDobavljaci::PoziviDijalogUnosa ), NULL, this );
	btnAzuriraj->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelDobavljaci::PoziviDijalogUnosa ), NULL, this );
	
}

GUIPanelSkladista::GUIPanelSkladista( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Odabir povijesti entiteta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer11->Add( m_staticText13, 0, wxALL, 5 );
	
	comboFilter = new wxComboBox( this, wxID_ANY, wxT("Aktivni"), wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, 0 ); 
	comboFilter->SetSelection( 0 );
	bSizer11->Add( comboFilter, 0, wxALL, 5 );
	
	
	bSizer14->Add( bSizer11, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer14->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Tablica skladišta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer14->Add( m_staticText14, 0, wxALL, 5 );
	
	tablicaSkladista = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 400,-1 ), wxDV_MULTIPLE|wxDV_ROW_LINES );
	kolonaId = tablicaSkladista->AppendTextColumn( wxT("Šifra (id)") );
	kolonaOznaka = tablicaSkladista->AppendTextColumn( wxT("Oznaka") );
	kolonaLokacija = tablicaSkladista->AppendTextColumn( wxT("Lokacija") );
	kolonaTelefon = tablicaSkladista->AppendTextColumn( wxT("Telefon") );
	kolonaTelefax = tablicaSkladista->AppendTextColumn( wxT("Fax") );
	bSizer14->Add( tablicaSkladista, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	stranicenjeSizer = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer31->Add( stranicenjeSizer, 1, wxEXPAND, 5 );
	
	
	bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btnDodaj = new wxButton( this, IDskladistaDodaj, wxT("Dodaj"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( btnDodaj, 0, wxALL, 5 );
	
	btnAzuriraj = new wxButton( this, IDskladistaAzuriraj, wxT("Ažuriraj"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( btnAzuriraj, 0, wxALL, 5 );
	
	
	bSizer14->Add( bSizer31, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer14 );
	this->Layout();
	
	// Connect Events
	comboFilter->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GUIPanelSkladista::OnCombo ), NULL, this );
	tablicaSkladista->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelSkladista::SelectionChanged ), NULL, this );
	btnDodaj->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelSkladista::PoziviDijalogUnosa ), NULL, this );
	btnAzuriraj->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelSkladista::PoziviDijalogUnosa ), NULL, this );
}

GUIPanelSkladista::~GUIPanelSkladista()
{
	// Disconnect Events
	comboFilter->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GUIPanelSkladista::OnCombo ), NULL, this );
	tablicaSkladista->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelSkladista::SelectionChanged ), NULL, this );
	btnDodaj->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelSkladista::PoziviDijalogUnosa ), NULL, this );
	btnAzuriraj->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelSkladista::PoziviDijalogUnosa ), NULL, this );
	
}

GUIPanelPocetni::GUIPanelPocetni( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );
	
	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap1 = new wxStaticBitmap( m_scrolledWindow1, wxID_ANY, wxBitmap( wxT("ERA model.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( m_bitmap1, 0, wxALL|wxEXPAND, 5 );
	
	
	m_scrolledWindow1->SetSizer( bSizer39 );
	m_scrolledWindow1->Layout();
	bSizer39->Fit( m_scrolledWindow1 );
	bSizer38->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer38 );
	this->Layout();
}

GUIPanelPocetni::~GUIPanelPocetni()
{
}

dlgUnosDobavljaca::dlgUnosDobavljaca( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( m_panel4, wxID_ANY, wxT("Šifra"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer17->Add( m_staticText3, 0, wxALL, 5 );
	
	dlgDobavljaciId = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	dlgDobavljaciId->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgDobavljaciId->SetForegroundColour( wxColour( 255, 122, 0 ) );
	dlgDobavljaciId->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	bSizer17->Add( dlgDobavljaciId, 0, wxALL, 5 );
	
	
	bSizer17->Add( 100, 0, 1, wxEXPAND, 5 );
	
	btnDlgDobavljaciReset = new wxButton( m_panel4, wxID_ANY, wxT("Vrati početno"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( btnDlgDobavljaciReset, 0, wxALL, 5 );
	
	
	bSizer16->Add( bSizer17, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer16->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( m_panel4, wxID_ANY, wxT("Naziv"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText4->Wrap( -1 );
	bSizer18->Add( m_staticText4, 0, wxALL, 5 );
	
	dlgDobavljaciNaziv = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dlgDobavljaciNaziv->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgDobavljaciNaziv->SetToolTip( wxT("Unesi naziv dobavljača") );
	
	bSizer18->Add( dlgDobavljaciNaziv, 1, wxALL, 5 );
	
	
	bSizer16->Add( bSizer18, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText41 = new wxStaticText( m_panel4, wxID_ANY, wxT("Adresa"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText41->Wrap( -1 );
	bSizer181->Add( m_staticText41, 0, wxALL, 5 );
	
	dlgDobavljaciAdresa = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,50 ), wxTE_MULTILINE );
	dlgDobavljaciAdresa->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgDobavljaciAdresa->SetToolTip( wxT("Unesi adresu dobavljača") );
	
	bSizer181->Add( dlgDobavljaciAdresa, 1, wxALL, 5 );
	
	
	bSizer16->Add( bSizer181, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer182;
	bSizer182 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText42 = new wxStaticText( m_panel4, wxID_ANY, wxT("Telefon 1"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText42->Wrap( -1 );
	bSizer182->Add( m_staticText42, 0, wxALL, 5 );
	
	dlgDobavljaciTelefon1 = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	dlgDobavljaciTelefon1->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgDobavljaciTelefon1->SetToolTip( wxT("Unesi telefon dobavljača") );
	
	bSizer182->Add( dlgDobavljaciTelefon1, 2, wxALL, 5 );
	
	
	bSizer182->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer16->Add( bSizer182, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1821;
	bSizer1821 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText421 = new wxStaticText( m_panel4, wxID_ANY, wxT("Telefon 2"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText421->Wrap( -1 );
	bSizer1821->Add( m_staticText421, 0, wxALL, 5 );
	
	dlgDobavljaciTelefon2 = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	dlgDobavljaciTelefon2->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgDobavljaciTelefon2->SetToolTip( wxT("Po potrebi unesi drugi telefon ili fax") );
	
	bSizer1821->Add( dlgDobavljaciTelefon2, 2, wxALL, 5 );
	
	
	bSizer1821->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer16->Add( bSizer1821, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18211;
	bSizer18211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4211 = new wxStaticText( m_panel4, wxID_ANY, wxT("E-pošta"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText4211->Wrap( -1 );
	bSizer18211->Add( m_staticText4211, 0, wxALL, 5 );
	
	dlgDobavljaciEmail = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	dlgDobavljaciEmail->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgDobavljaciEmail->SetToolTip( wxT("Unesi e-poštu (e-mail)") );
	
	bSizer18211->Add( dlgDobavljaciEmail, 2, wxALL, 5 );
	
	
	bSizer18211->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer16->Add( bSizer18211, 1, wxEXPAND, 5 );
	
	
	m_panel4->SetSizer( bSizer16 );
	m_panel4->Layout();
	bSizer16->Fit( m_panel4 );
	bSizer12->Add( m_panel4, 1, wxEXPAND | wxALL, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer14->Add( 0, 0, 2, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	btnOdustani = new wxButton( this, dlgUnosDobavljacaOdustani, wxT("Odustani"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( btnOdustani, 0, wxALL, 5 );
	
	btnPrihvati = new wxButton( this, dlgUnosDobavljacaPrihvati, wxT("Prihvati"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( btnPrihvati, 0, wxALL, 5 );
	
	
	bSizer14->Add( bSizer15, 1, wxEXPAND, 5 );
	
	
	bSizer12->Add( bSizer14, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer12 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( dlgUnosDobavljaca::OnInit ) );
	btnDlgDobavljaciReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosDobavljaca::Reset ), NULL, this );
	btnOdustani->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosDobavljaca::GumbPritisnut ), NULL, this );
	btnPrihvati->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosDobavljaca::GumbPritisnut ), NULL, this );
}

dlgUnosDobavljaca::~dlgUnosDobavljaca()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( dlgUnosDobavljaca::OnInit ) );
	btnDlgDobavljaciReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosDobavljaca::Reset ), NULL, this );
	btnOdustani->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosDobavljaca::GumbPritisnut ), NULL, this );
	btnPrihvati->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosDobavljaca::GumbPritisnut ), NULL, this );
	
}

dlgUnosSkladista::dlgUnosSkladista( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( m_panel4, wxID_ANY, wxT("Šifra"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer17->Add( m_staticText3, 0, wxALL, 5 );
	
	dlgSkladisteId = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	dlgSkladisteId->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgSkladisteId->SetForegroundColour( wxColour( 255, 122, 0 ) );
	dlgSkladisteId->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	bSizer17->Add( dlgSkladisteId, 0, wxALL, 5 );
	
	
	bSizer17->Add( 100, 0, 1, wxEXPAND, 5 );
	
	btnDlgSkladisteReset = new wxButton( m_panel4, wxID_ANY, wxT("Vrati početno"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( btnDlgSkladisteReset, 0, wxALL, 5 );
	
	
	bSizer16->Add( bSizer17, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer16->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( m_panel4, wxID_ANY, wxT("Oznaka"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText4->Wrap( -1 );
	bSizer18->Add( m_staticText4, 0, wxALL, 5 );
	
	dlgSkladisteOznaka = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dlgSkladisteOznaka->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgSkladisteOznaka->SetToolTip( wxT("Unesi naziv dobavljača") );
	
	bSizer18->Add( dlgSkladisteOznaka, 1, wxALL, 5 );
	
	
	bSizer16->Add( bSizer18, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText41 = new wxStaticText( m_panel4, wxID_ANY, wxT("Lokacija"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText41->Wrap( -1 );
	bSizer181->Add( m_staticText41, 0, wxALL, 5 );
	
	dlgSkladisteLokacija = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,50 ), wxTE_MULTILINE );
	dlgSkladisteLokacija->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgSkladisteLokacija->SetToolTip( wxT("Unesi adresu dobavljača") );
	
	bSizer181->Add( dlgSkladisteLokacija, 1, wxALL, 5 );
	
	
	bSizer16->Add( bSizer181, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer182;
	bSizer182 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText42 = new wxStaticText( m_panel4, wxID_ANY, wxT("Telefon"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText42->Wrap( -1 );
	bSizer182->Add( m_staticText42, 0, wxALL, 5 );
	
	dlgSkladisteTelefon = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	dlgSkladisteTelefon->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgSkladisteTelefon->SetToolTip( wxT("Unesi telefon dobavljača") );
	
	bSizer182->Add( dlgSkladisteTelefon, 2, wxALL, 5 );
	
	
	bSizer182->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer16->Add( bSizer182, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1821;
	bSizer1821 = new wxBoxSizer( wxHORIZONTAL );
	
	dlgSkladisteFaks = new wxStaticText( m_panel4, wxID_ANY, wxT("Telefaks"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	dlgSkladisteFaks->Wrap( -1 );
	bSizer1821->Add( dlgSkladisteFaks, 0, wxALL, 5 );
	
	dlgSkladisteTelefaks = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	dlgSkladisteTelefaks->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	dlgSkladisteTelefaks->SetToolTip( wxT("Po potrebi unesi drugi telefon ili fax") );
	
	bSizer1821->Add( dlgSkladisteTelefaks, 2, wxALL, 5 );
	
	
	bSizer1821->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer16->Add( bSizer1821, 0, wxEXPAND, 5 );
	
	
	m_panel4->SetSizer( bSizer16 );
	m_panel4->Layout();
	bSizer16->Fit( m_panel4 );
	bSizer12->Add( m_panel4, 1, wxEXPAND | wxALL, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer14->Add( 0, 0, 2, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	btnOdustani = new wxButton( this, dlgUnosSkladistaOdustani, wxT("Odustani"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( btnOdustani, 0, wxALL, 5 );
	
	btnPrihvati = new wxButton( this, dlgUnosSkladistaPrihvati, wxT("Prihvati"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( btnPrihvati, 0, wxALL, 5 );
	
	
	bSizer14->Add( bSizer15, 1, wxEXPAND, 5 );
	
	
	bSizer12->Add( bSizer14, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer12 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( dlgUnosSkladista::OnInit ) );
	btnDlgSkladisteReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosSkladista::Reset ), NULL, this );
	btnOdustani->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosSkladista::GumbPritisnut ), NULL, this );
	btnPrihvati->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosSkladista::GumbPritisnut ), NULL, this );
}

dlgUnosSkladista::~dlgUnosSkladista()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( dlgUnosSkladista::OnInit ) );
	btnDlgSkladisteReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosSkladista::Reset ), NULL, this );
	btnOdustani->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosSkladista::GumbPritisnut ), NULL, this );
	btnPrihvati->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgUnosSkladista::GumbPritisnut ), NULL, this );
	
}

GUIPanelStranicenje::GUIPanelStranicenje( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	sttStranicenje = new wxStaticText( this, wxID_ANY, wxT("Straničenje"), wxDefaultPosition, wxDefaultSize, 0 );
	sttStranicenje->Wrap( -1 );
	sttStranicenje->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ) );
	
	bSizer26->Add( sttStranicenje, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	btnSmanji = new wxButton( this, IDStranicenjeSmanjii, wxT("<"), wxDefaultPosition, wxSize( 30,31 ), 0 );
	bSizer23->Add( btnSmanji, 0, wxRIGHT, 3 );
	
	tbStranica = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 80,-1 ), wxTE_CENTRE|wxTE_READONLY );
	tbStranica->SetFont( wxFont( 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Latin Modern Math") ) );
	
	bSizer23->Add( tbStranica, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	btnPovecaj = new wxButton( this, IDStranicenjePovecaj, wxT(">"), wxDefaultPosition, wxSize( 30,31 ), 0 );
	bSizer23->Add( btnPovecaj, 0, wxLEFT, 3 );
	
	
	bSizer26->Add( bSizer23, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer26 );
	this->Layout();
	
	// Connect Events
	btnSmanji->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelStranicenje::GumbPritisnut ), NULL, this );
	btnPovecaj->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelStranicenje::GumbPritisnut ), NULL, this );
}

GUIPanelStranicenje::~GUIPanelStranicenje()
{
	// Disconnect Events
	btnSmanji->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelStranicenje::GumbPritisnut ), NULL, this );
	btnPovecaj->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIPanelStranicenje::GumbPritisnut ), NULL, this );
	
}

GUIPanelMaterijali::GUIPanelMaterijali( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Materijali") ), wxVERTICAL );
	
	dvcMaterijali = new wxDataViewCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_dataViewColumn1 = dvcMaterijali->AppendTextColumn( wxT("Vrsta / Naziv"), 0 );
	m_dataViewColumn2 = dvcMaterijali->AppendTextColumn( wxT("Šifra (id)"), 1 );
	m_dataViewColumn3 = dvcMaterijali->AppendTextColumn( wxT("Dobavljač"), 2 );
	m_dataViewColumn4 = dvcMaterijali->AppendTextColumn( wxT("Šifra dob."), 3 );
	sbSizer2->Add( dvcMaterijali, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	wxBoxSizer* bSizer421;
	bSizer421 = new wxBoxSizer( wxHORIZONTAL );
	
	chkPrikaziNeaktivne = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, wxT("prikaži neaktivne"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer421->Add( chkPrikaziNeaktivne, 0, wxLEFT, 5 );
	
	
	sbSizer2->Add( bSizer421, 0, 0, 5 );
	
	
	bSizer42->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Povijest promjena") ), wxVERTICAL );
	
	tablicaPovijesti = new wxDataViewListCtrl( sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0|wxVSCROLL );
	m_dataViewListColumn12 = tablicaPovijesti->AppendTextColumn( wxT("Naziv") );
	m_dataViewListColumn13 = tablicaPovijesti->AppendTextColumn( wxT("Vrijeme od") );
	m_dataViewListColumn14 = tablicaPovijesti->AppendTextColumn( wxT("Vrijeme do") );
	sbSizer3->Add( tablicaPovijesti, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer42->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	
	bSizer40->Add( bSizer42, 4, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Detaljni podaci o materijalu") ), wxHORIZONTAL );
	
	txtMaterijalDetaljiL = new wxTextCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER );
	sbSizer4->Add( txtMaterijalDetaljiL, 1, wxBOTTOM|wxEXPAND|wxLEFT, 5 );
	
	txtMaterijalDetaljiD = new wxTextCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER );
	sbSizer4->Add( txtMaterijalDetaljiD, 1, wxBOTTOM|wxEXPAND|wxRIGHT, 5 );
	
	
	bSizer40->Add( sbSizer4, 2, wxALIGN_TOP|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer40 );
	this->Layout();
	
	// Connect Events
	dvcMaterijali->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelMaterijali::SelekcijaPromijenjena ), NULL, this );
	chkPrikaziNeaktivne->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIPanelMaterijali::NeaktivniChecked ), NULL, this );
	tablicaPovijesti->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelMaterijali::PovijestPromijenjena ), NULL, this );
}

GUIPanelMaterijali::~GUIPanelMaterijali()
{
	// Disconnect Events
	dvcMaterijali->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelMaterijali::SelekcijaPromijenjena ), NULL, this );
	chkPrikaziNeaktivne->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIPanelMaterijali::NeaktivniChecked ), NULL, this );
	tablicaPovijesti->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIPanelMaterijali::PovijestPromijenjena ), NULL, this );
	
}

MyPanel6::MyPanel6( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
}

MyPanel6::~MyPanel6()
{
}
