#pragma once
#include <string>
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
  MainFrame(const wxString &title);

private:
  void PreFillParams_Cancer(wxCommandEvent &evt);
  void PreFillParams_HIV(wxCommandEvent &evt);
  void PreFillParams_Flu(wxCommandEvent &evt);
  void PreFillParams_Malaria(wxCommandEvent &evt);
  void PreFillParams_Test(wxCommandEvent &evt);
  void Submit(wxCommandEvent &evt);
  void ClearForm(wxCommandEvent &evt);
};

void helper_prefill(int index);