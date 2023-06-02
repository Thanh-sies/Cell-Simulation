#include <wx/wx.h>
#include "App.hpp"
#include "MainFrame.hpp"


wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  MainFrame *mainFrame = new MainFrame("Cells Parameter Input");
  mainFrame->SetClientSize(1000, 700);
  mainFrame->Center();
  mainFrame->Show(true);
  return true;
};