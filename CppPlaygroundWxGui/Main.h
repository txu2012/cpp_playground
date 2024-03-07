// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define ID_MENU_HELLO   1 // Menu Item ID

//////////////////////////////////////////////////////////////////////////////////////////////////
/////  DECLARE THE APPLICATION'S MAIN FRAME WINDOW
//////////////////////////////////////////////////////////////////////////////////////////////////

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/////   DECLARE THE APPLICATION CLASS
//////////////////////////////////////////////////////////////////////////////////////////////////

class HelloWorld : public wxApp
{
public:
	// Define the initialization method
	// Its called on application start up
	virtual bool OnInit();
};

//////////////////////////////////////////////////////////////////////////////////////////////
///// IMPLEMENT THE OnInit() WHICH INITIALIZES THE APPLICATION
/////////////////////////////////////////////////////////////////////////////////////////////
bool HelloWorld::OnInit()
{
	// Create main application window
	Frame* frame = new Frame("Hello World", wxPoint(50, 50), wxSize(300, 300));

	// Display the frame window
	frame->Show(true);

	// Start the event loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
///// START RUNNING THE APPLICATION
////////////////////////////////////////////////////////////////////////////////////////

wxIMPLEMENT_APP(HelloWorld);

////////////////////////////////////////////////////////////////////////////////////////
///// IMPLEMENT THE FRAME (APP MAIN WINDOW) CONSTRUCTOR
///////////////////////////////////////////////////////////////////////////////////////

Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
	:wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_MENU_HELLO, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

	// Register events for this frame
	Bind(wxEVT_MENU, &Frame::OnHello, this, ID_MENU_HELLO);
	Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &Frame::OnExit, this, wxID_EXIT);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////    DEFINE EVENT HANDLERS FOR THE APPLICATION FRAME
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Event Handler for clicking the About Menu item
// Navigate to Help > About in the GUI app
void Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

// Event Handler for clicking the Hello Menu item
// Naviaget to File > Hello in the GUI app
void Frame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

// Event handler on clicking the Exit Menu item
// Navigate to File > Exit in the GUI app
void Frame::OnExit(wxCommandEvent& event)
{
	Close(true);
}