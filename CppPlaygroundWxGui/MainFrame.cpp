#include "MainFrame.h"
//#define ID_MENU_HELLO   1 // Menu Item ID

namespace Gui {
	////////////////////////////////////////////////////////////////////////////////////////
	///// IMPLEMENT THE FRAME (APP MAIN WINDOW) CONSTRUCTOR
	///////////////////////////////////////////////////////////////////////////////////////
	MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
		:wxFrame(NULL, wxID_ANY, title, pos, size)
	{
		SetPresenter();
		wxWindow::SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX);
				
		wxFrameBase::CreateStatusBar();
		wxFrameBase::SetStatusText("Welcome to wxWidgets!");

		IntializeMenu();

		// Create Response textbox
		tbResponse = new wxTextCtrl(this, TEXT_RESPONSE, "", wxPoint(0, 0), wxSize(785, 547),
			wxTE_READONLY | wxTE_MULTILINE);

		InitializeListCtrl();
		InitializeButtons();		

		BindEvents();

		lstJoysticks->SetFocus();
		tbResponse->HideNativeCaret();
	}

	void MainFrame::SetPresenter() {
		presenter_ = Gui::CppPresenter();
	}

	void MainFrame::IntializeMenu() {
		wxMenu* menuFile = new wxMenu;
		menuFile->Append(MENU_HELLO, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
		menuFile->AppendSeparator();
		menuFile->Append(wxStandardID::wxID_EXIT);

		wxMenu* menuHelp = new wxMenu;
		menuHelp->Append(wxStandardID::wxID_ABOUT);

		wxMenuBar* menuBar = new wxMenuBar;
		menuBar->Append(menuFile, "&File");
		menuBar->Append(menuHelp, "&Help");

		wxFrameBase::SetMenuBar(menuBar);
	}

	void MainFrame::InitializeListCtrl() {
		// Create Joystick list
		lstJoysticks = new wxListCtrl(this, LIST_JOYSTICKS, wxPoint(0, 547), wxSize(707, 70), wxTE_MULTILINE);
		wxListItem col0;
		col0.SetId(0);
		col0.SetText(_("Index"));
		col0.SetWidth(45);
		lstJoysticks->InsertColumn(0, col0);

		// Add second column 
		wxListItem col1;
		col1.SetId(1);
		col1.SetText(_("Name"));
		col1.SetWidth(200);
		lstJoysticks->InsertColumn(1, col1);

		// Add third column      
		wxListItem col2;
		col2.SetId(2);
		col2.SetText(_("Instance Guid"));
		col2.SetWidth(230);
		lstJoysticks->InsertColumn(2, col2);

		// Add fourth column      
		wxListItem col3;
		col3.SetId(3);
		col3.SetText(_("Product Guid"));
		col3.SetWidth(230);
		lstJoysticks->InsertColumn(3, col3);

		lstJoysticks->SetItemCount(5);

		PopulateJoysticks();
	}

	void MainFrame::PopulateJoysticks() {
		auto joysticks = presenter_.GetJoystickList();

		for (int i = 0; i < joysticks.size(); i++) {
			char prod_guid[37];
			char instance_guid[37];

			long index = lstJoysticks->InsertItem(i, i);
			lstJoysticks->SetItem(index, 0, std::to_string(i));
			lstJoysticks->SetItem(index, 1, joysticks[i].name);
			lstJoysticks->SetItem(index, 2, Playground::Joystick::guid_to_str(&joysticks[i].instance_guid, instance_guid));
			lstJoysticks->SetItem(index, 3, Playground::Joystick::guid_to_str(&joysticks[i].product_guid, prod_guid));
		}
	}

	void MainFrame::InitializeButtons() {
		btnConnect = new wxButton(this, BUTTON_CONNECT, _T("Connect"), wxPoint(707, 548), wxSize(75, 34));
		btnDisconnect = new wxButton(this, BUTTON_DISCONNECT, _T("Disconnect"), wxPoint(707, 582), wxSize(75, 34));
	}

	void MainFrame::UpdateResponse(std::string response) {
		tbResponse->AppendText(response + "\n");
		tbResponse->HideNativeCaret();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////    DEFINE EVENT HANDLERS FOR THE APPLICATION FRAME
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void MainFrame::BindEvents() {
		// Register events for this frame
		wxEvtHandler::Bind(wxEVT_MENU, &MainFrame::OnHello, this, MENU_HELLO);
		wxEvtHandler::Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxStandardID::wxID_ABOUT);
		wxEvtHandler::Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxStandardID::wxID_EXIT);
		wxEvtHandler::Bind(wxEVT_BUTTON, &MainFrame::OnBtnConnect, this, BUTTON_CONNECT);
		wxEvtHandler::Bind(wxEVT_BUTTON, &MainFrame::OnBtnDisconnect, this, BUTTON_DISCONNECT);
	}
	
	// Event Handler for clicking the About Menu item
	// Navigate to Help > About in the GUI app
	void MainFrame::OnAbout(wxCommandEvent& event)
	{
		wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
	}

	// Event Handler for clicking the Hello Menu item
	// Naviaget to File > Hello in the GUI app
	void MainFrame::OnHello(wxCommandEvent& event)
	{
		wxLogMessage("Hello world from wxWidgets!");
	}

	// Event handler on clicking the Exit Menu item
	// Navigate to File > Exit in the GUI app
	void MainFrame::OnExit(wxCommandEvent& event)
	{
		wxWindowBase::Close(true);
	}

	void MainFrame::OnBtnConnect(wxCommandEvent& event) {
		long itemIndex = -1;
		wxString indexString;
		while ((itemIndex = lstJoysticks->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
			// Got the selected item index
			indexString = lstJoysticks->GetItemText(itemIndex);
		}
		
		int jsIndex = wxAtoi(indexString);

		UpdateResponse("Connecting to Joystick: " + std::to_string(jsIndex));
		presenter_.ConnectJoystick(jsIndex);
	}
	void MainFrame::OnBtnDisconnect(wxCommandEvent& event) {
	}

}