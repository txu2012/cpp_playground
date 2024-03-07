#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "base.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(MENU_New, MainFrame::NewFile)
EVT_MENU(MENU_Open, MainFrame::OpenFile)
EVT_MENU(MENU_Close, MainFrame::CloseFile)
EVT_MENU(MENU_Save, MainFrame::SaveFile)
EVT_MENU(MENU_SaveAs, MainFrame::SaveFileAs)
EVT_MENU(MENU_Quit, MainFrame::Quit)
END_EVENT_TABLE()


//IMPLEMENT_APP(MainApp) // Initializes the MainApp class and tells our program
// to run it
bool MainApp::OnInit()
{
    MainFrame* MainWin = new MainFrame(wxT("Hello World!"), wxPoint(1, 1),
        wxSize(300, 200)); // Create an instance of our frame, or window
    MainWin->Show(TRUE); // show the window
    SetTopWindow(MainWin);// and finally, set it as the main window

    return TRUE;
}

MainFrame::MainFrame(const wxString& title,
    const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
    CreateStatusBar(2);
    MainMenu = new wxMenuBar();
    wxMenu* FileMenu = new wxMenu();

    FileMenu->Append(MENU_New, wxT("&New"),
        wxT("Create a new file"));
    FileMenu->Append(MENU_Open, wxT("&Open"),
        wxT("Open an existing file"));
    FileMenu->Append(MENU_Close, wxT("&Close"),
        wxT("Close the current document"));
    FileMenu->Append(MENU_Save, wxT("&Save"),
        wxT("Save the current document"));
    FileMenu->Append(MENU_SaveAs, wxT("Save &As"),
        wxT("Save the current document under a new file name"));
    FileMenu->Append(MENU_Quit, wxT("&Quit"),
        wxT("Quit the editor"));

    MainMenu->Append(FileMenu, wxT("File"));
    SetMenuBar(MainMenu);

    MainEditBox = new wxTextCtrl(this, TEXT_Main,
        wxT("Hi!"), wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
    Maximize();
}

void MainFrame::NewFile(wxCommandEvent& WXUNUSED(event))
{
}

void MainFrame::OpenFile(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->LoadFile(wxT("base.h"));
}

void MainFrame::CloseFile(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->Clear();
}

void MainFrame::SaveFile(wxCommandEvent& WXUNUSED(event))
{
    MainEditBox->SaveFile(wxT("base.h"));
}

void MainFrame::SaveFileAs(wxCommandEvent& WXUNUSED(event))
{
}

void MainFrame::Quit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE); // Tells the OS to quit running this process
}