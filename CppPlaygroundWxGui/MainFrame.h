#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/calctrl.h"
#include "wx/splitter.h"
#include "wx/uilocale.h"
#include "wx/listctrl.h"

#include "CppPresenter.h"

namespace Gui {
	enum GuiId{
		// Menu
		MENU_HELLO = 1,

		// Texts
		TEXT_RESPONSE = 2,

		// Lists
		LIST_JOYSTICKS = 3,

		// Buttons
		BUTTON_CONNECT = 4,
		BUTTON_DISCONNECT = 5
	};

	class MainFrame : public wxFrame
	{
	public:
		MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	private:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnBtnConnect(wxCommandEvent& event);
		void OnBtnDisconnect(wxCommandEvent& event);

		wxTextCtrl* tbResponse;
		
		wxListCtrl* lstJoysticks;

		wxButton* btnConnect;
		wxButton* btnDisconnect;

		CppPresenter presenter_;

		void IntializeMenu();
		void InitializeListCtrl();
		void InitializeButtons();

		void BindEvents();
		void PopulateJoysticks();
		void UpdateResponse(std::string response);
		void SetPresenter();
	};

}