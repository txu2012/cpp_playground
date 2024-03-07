#include "Main.h"
#include "MainFrame.h"

namespace Gui {
	//////////////////////////////////////////////////////////////////////////////////////////////
	///// IMPLEMENT THE OnInit() WHICH INITIALIZES THE APPLICATION
	/////////////////////////////////////////////////////////////////////////////////////////////
	bool Main::OnInit()
	{
		// Create main application window
		Gui::MainFrame* frame = new Gui::MainFrame("Hello World", wxPoint(50, 50), wxSize(800, 700));
		// Display the frame window
		frame->Show(true);

		// Start the event loop
		return true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
///// START RUNNING THE APPLICATION
////////////////////////////////////////////////////////////////////////////////////////

wxIMPLEMENT_APP(Gui::Main);
