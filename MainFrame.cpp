#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <wx/wx.h>
#include <wx/spinctrl.h>

#include "constants.hpp"
#include "MainFrame.hpp"
#include "Simulation.hpp"

static std::map<std::string, wxSpinCtrl *> paramMap;
wxPanel *panel_ref;

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{
  wxPanel *panel = new wxPanel(this);
  panel_ref = panel;

  wxStaticText *prefillOptions = new wxStaticText(panel, wxID_ANY, "Prefill Options (and/or fill each field manually) - Counts represent # of cells.", wxPoint(150, 50));
  wxButton *prefill_cancer_btn = new wxButton(panel, wxID_ANY, "Cancer - WBC", wxPoint(150, 65), wxSize(100, 50));
  wxButton *prefill_hiv_btn = new wxButton(panel, wxID_ANY, "HIV - WBC", wxPoint(255, 65), wxSize(90, 50));
  wxButton *prefill_flu_btn = new wxButton(panel, wxID_ANY, "Flu - WBC", wxPoint(350, 65), wxSize(75, 50));
  wxButton *prefill_malaria_btn = new wxButton(panel, wxID_ANY, "Malaria - RBC", wxPoint(430, 65), wxSize(110, 50));
  wxButton *prefill_test_btn = new wxButton(panel, wxID_ANY, "Test Scenario", wxPoint(600, 65), wxSize(150, 50));

  prefill_cancer_btn->Bind(wxEVT_BUTTON, &MainFrame::PreFillParams_Cancer, this);
  prefill_hiv_btn->Bind(wxEVT_BUTTON, &MainFrame::PreFillParams_HIV, this);
  prefill_flu_btn->Bind(wxEVT_BUTTON, &MainFrame::PreFillParams_Flu, this);
  prefill_malaria_btn->Bind(wxEVT_BUTTON, &MainFrame::PreFillParams_Malaria, this);
  prefill_test_btn->Bind(wxEVT_BUTTON, &MainFrame::PreFillParams_Test, this);

  // If the cells are aggressive, they will attack. They become aggressive based on chance, otherwise
  // they will effectively serve as functionless zombies, waiting for their timespan to run out.

  // WBC and RBC aggression level
  // -- Intra aggression property
  wxStaticText *wbc_agg_pct_intra = new wxStaticText(panel, wxID_ANY, "Intra-Aggressive Mutation - White Blood Cells(WBC) %", wxPoint(150, 125));
  wxStaticText *rbc_agg_pct_intra = new wxStaticText(panel, wxID_ANY, "Intra-Aggressive Mutation - Red Blood Cells(RBC) %", wxPoint(150, 175));
  wxSpinCtrl *spinCtrl_wbc_agg_pct_intra = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 150));
  wxSpinCtrl *spinCtrl_rbc_agg_pct_intra = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 200));
  wxStaticText *wbc_agg_pct_intra_infected = new wxStaticText(panel, wxID_ANY, ", Agg % Post Infection", wxPoint(225, 150));
  wxStaticText *rbc_agg_pct_intra_infected = new wxStaticText(panel, wxID_ANY, ", Agg % Post Infection", wxPoint(225, 200));
  wxSpinCtrl *spinCtrl_wbc_agg_pct_intra_infected = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(375, 150));
  wxSpinCtrl *spinCtrl_rbc_agg_pct_intra_infected = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(375, 200));
  // -- Inter aggression property - attacks all entities: rbc, wbc and virus
  wxStaticText *wbc_agg_pct_inter = new wxStaticText(panel, wxID_ANY, "Inter-Aggressive Mutation - WBC %", wxPoint(500, 125));
  wxStaticText *rbc_agg_pct_inter = new wxStaticText(panel, wxID_ANY, "Inter-Aggressive Mutation - RBC %", wxPoint(500, 175));
  wxSpinCtrl *spinCtrl_wbc_agg_pct_inter = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(500, 150));
  wxSpinCtrl *spinCtrl_rbc_agg_pct_inter = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(500, 200));
  wxStaticText *wbc_agg_pct_inter_infected = new wxStaticText(panel, wxID_ANY, ", Agg % Post Infection", wxPoint(575, 150));
  wxStaticText *rbc_agg_pct_inter_infected = new wxStaticText(panel, wxID_ANY, ", Agg % Post Infection", wxPoint(575, 200));
  wxSpinCtrl *spinCtrl_wbc_agg_pct_inter_infected = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(725, 150));
  wxSpinCtrl *spinCtrl_rbc_agg_pct_inter_infected = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(725, 200));
  paramMap[WBC_AGG_PCT_INTRA] = spinCtrl_wbc_agg_pct_intra;
  paramMap[RBC_AGG_PCT_INTRA] = spinCtrl_rbc_agg_pct_intra;
  paramMap[WBC_AGG_PCT_INTER] = spinCtrl_wbc_agg_pct_inter;
  paramMap[RBC_AGG_PCT_INTER] = spinCtrl_rbc_agg_pct_inter;
  paramMap[WBC_AGG_PCT_INTRA_INFECTED] = spinCtrl_wbc_agg_pct_intra_infected;
  paramMap[RBC_AGG_PCT_INTRA_INFECTED] = spinCtrl_rbc_agg_pct_intra_infected;
  paramMap[WBC_AGG_PCT_INTER_INFECTED] = spinCtrl_wbc_agg_pct_inter_infected;
  paramMap[RBC_AGG_PCT_INTER_INFECTED] = spinCtrl_rbc_agg_pct_inter_infected;

  // Init Population
  wxStaticText *init_viral_pop_cnt = new wxStaticText(panel, wxID_ANY, "Initial Viral Population Count Range(R): [0,100]", wxPoint(150, 250));
  wxStaticText *init_wbc_pop_cnt = new wxStaticText(panel, wxID_ANY, "Initial WBC Population Count R: [0,100]", wxPoint(150, 300));
  wxStaticText *init_rbc_pop_cnt = new wxStaticText(panel, wxID_ANY, "Initial RBC Population Count R: [0,100]", wxPoint(500, 250));
  wxStaticText *init_bone_marrow_cell_cnt = new wxStaticText(panel, wxID_ANY, "Initial Bone Marrow(BM) Resource Count R: [0,1000]", wxPoint(500, 300));
  wxSpinCtrl *spinCtrl_init_viral_pop_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 275));
  wxSpinCtrl *spinCtrl_init_wbc_pop_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 325));
  wxSpinCtrl *spinCtrl_init_rbc_pop_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(500, 275));
  wxStaticText *init_descriptor_wbc_cnt = new wxStaticText(panel, wxID_ANY, "WBC-BM", wxPoint(500, 325));
  wxStaticText *init_descriptor_rbc_cnt = new wxStaticText(panel, wxID_ANY, "RBC-BM", wxPoint(700, 325));
  wxSpinCtrl *spinCtrl_init_bone_marrow_wbc_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(575, 325), wxSize(105, -1));
  wxSpinCtrl *spinCtrl_init_bone_marrow_rbc_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(775, 325), wxSize(105, -1));
  spinCtrl_init_viral_pop_cnt->SetRange(0, 100);
  spinCtrl_init_wbc_pop_cnt->SetRange(0, 100);
  spinCtrl_init_rbc_pop_cnt->SetRange(0, 100);
  spinCtrl_init_bone_marrow_wbc_cnt->SetRange(0, 1000);
  spinCtrl_init_bone_marrow_rbc_cnt->SetRange(0, 1000);
  paramMap[INIT_VIRAL_POP_CNT] = spinCtrl_init_viral_pop_cnt;
  paramMap[INIT_WBC_POP_CNT] = spinCtrl_init_wbc_pop_cnt;
  paramMap[INIT_RBC_POP_CNT] = spinCtrl_init_rbc_pop_cnt;
  paramMap[INIT_BONE_MARROW_WBC_COUNT] = spinCtrl_init_bone_marrow_wbc_cnt;
  paramMap[INIT_BONE_MARROW_RBC_COUNT] = spinCtrl_init_bone_marrow_rbc_cnt;

  // Cell Lifeforce
  wxStaticText *virus_lifeforce_cycles = new wxStaticText(panel, wxID_ANY, "Lifeforce of Virus R: [1,100]", wxPoint(150, 375));
  wxStaticText *wbc_lifeforce_cycles = new wxStaticText(panel, wxID_ANY, "Lifeforce of WBC R: [1,100]", wxPoint(400, 375));
  wxStaticText *rbc_lifeforce_cycles = new wxStaticText(panel, wxID_ANY, "Lifeforce of RBC R: [1,100]", wxPoint(650, 375));
  wxSpinCtrl *spinCtrl_virus_lifeforce_cycles = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 400));
  wxSpinCtrl *spinCtrl_wbc_lifeforce_cycles = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(400, 400));
  wxSpinCtrl *spinCtrl_rbc_lifeforce_cycles = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(650, 400));
  spinCtrl_virus_lifeforce_cycles->SetRange(1, 100);
  spinCtrl_wbc_lifeforce_cycles->SetRange(1, 100);
  spinCtrl_rbc_lifeforce_cycles->SetRange(1, 100);
  paramMap[VIRUS_LIFEFORCE_CYCLES] = spinCtrl_virus_lifeforce_cycles;
  paramMap[WBC_LIFEFORCE_CYCLES] = spinCtrl_wbc_lifeforce_cycles;
  paramMap[RBC_LIFEFORCE_CYCLES] = spinCtrl_rbc_lifeforce_cycles;

  // Virus parameters
  wxStaticText *viral_infection_rate = new wxStaticText(panel, wxID_ANY, "Viral Infection %", wxPoint(150, 425));
  wxStaticText *viral_reproduction_factor = new wxStaticText(panel, wxID_ANY, "Reproduction Factor R: [0,10]", wxPoint(300, 425));
  wxStaticText *viral_reproduction_factor_count = new wxStaticText(panel, wxID_ANY, "Lysogenic Feature", wxPoint(300, 435));
  wxStaticText *viral_mutation_rate = new wxStaticText(panel, wxID_ANY, "Viral Mutation %", wxPoint(500, 425));
  wxStaticText *viral_mutation_cap = new wxStaticText(panel, wxID_ANY, "Viral Mutation count Upper Limit R: [0,10]", wxPoint(650, 425));
  wxStaticText *viral_load_threshold_1 = new wxStaticText(panel, wxID_ANY, "Viral Count Threshold for Body to", wxPoint(150, 475));
  wxStaticText *viral_load_threshold_2 = new wxStaticText(panel, wxID_ANY, "Recognize as a threat R: [1, 100]", wxPoint(150, 490));
  wxStaticText *viral_aggression_WBC = new wxStaticText(panel, wxID_ANY, "Viral Aggression -> WBC R: {0,1}", wxPoint(400, 490));
  wxStaticText *viral_aggression_RBC = new wxStaticText(panel, wxID_ANY, "Viral Aggression -> RBC R: {0,1}", wxPoint(650, 490));

  wxSpinCtrl *spinCtrl_viral_infection_rate = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 450));
  wxSpinCtrl *spinCtrl_viral_reproduction_factor = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(300, 450));
  wxSpinCtrl *spinCtrl_viral_mutation_rate = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(500, 450));
  wxSpinCtrl *spinCtrl_viral_mutation_cap = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(650, 450));
  wxSpinCtrl *spinCtrl_viral_load_threshold = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 515), wxSize(150, -1));
  wxSpinCtrl *spinCtrl_viral_aggression_WBC = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(400, 515), wxSize(50, -1));
  wxSpinCtrl *spinCtrl_viral_aggression_RBC = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(650, 515), wxSize(50, -1));

  // When cells infected, the cells will die if reproduction rate is higher than 0.
  spinCtrl_viral_reproduction_factor->SetRange(0, 10);
  spinCtrl_viral_mutation_cap->SetRange(0, 10);
  spinCtrl_viral_load_threshold->SetRange(1, 100);
  spinCtrl_viral_aggression_WBC->SetRange(0, 1);
  spinCtrl_viral_aggression_RBC->SetRange(0, 1);
  paramMap[VIRAL_INFECTION_RATE] = spinCtrl_viral_infection_rate;
  paramMap[VIRAL_REPRODUCTION_FACTOR] = spinCtrl_viral_reproduction_factor;
  paramMap[VIRAL_MUTATION_RATE] = spinCtrl_viral_mutation_rate;
  paramMap[VIRAL_MUTATION_CAP] = spinCtrl_viral_mutation_cap;
  paramMap[VIRAL_LOAD_THRESHOLD] = spinCtrl_viral_load_threshold;
  paramMap[VIRAL_AGG_WBC] = spinCtrl_viral_aggression_WBC;
  paramMap[VIRAL_AGG_RBC] = spinCtrl_viral_aggression_RBC;

  // WBC, RBC, Bone Marrow replenish parameters
  wxStaticText *wbc_spawn_cnt = new wxStaticText(panel, wxID_ANY, "WBC Spawn Count R: [0,100]", wxPoint(150, 550));
  wxStaticText *rbc_spawn_cnt = new wxStaticText(panel, wxID_ANY, "RBC Spawn Count R: [0,100]", wxPoint(400, 550));
  wxStaticText *bone_marrow_replenish_wbc_cnt = new wxStaticText(panel, wxID_ANY, "BM Replenish WBC Count R: [0,100]", wxPoint(650, 550));
  wxStaticText *bone_marrow_replenish_rbc_cnt = new wxStaticText(panel, wxID_ANY, "BM Replenish RBC Count R: [0,100]", wxPoint(650, 600));
  wxStaticText *wbc_stable_cnt = new wxStaticText(panel, wxID_ANY, "WBC Stable Count R: [0,100]", wxPoint(150, 600));
  wxStaticText *rbc_stable_cnt = new wxStaticText(panel, wxID_ANY, "RBC Stable Count R: [0,100]", wxPoint(400, 600));
  wxSpinCtrl *spinCtrl_wbc_spawn_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 575));
  wxSpinCtrl *spinCtrl_rbc_spawn_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(400, 575));
  wxSpinCtrl *spinCtrl_bone_marrow_replenish_wbc_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(650, 575));
  wxSpinCtrl *spinCtrl_bone_marrow_replenish_rbc_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(650, 625));
  wxSpinCtrl *spinCtrl_wbc_stable_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(150, 625));
  wxSpinCtrl *spinCtrl_rbc_stable_cnt = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(400, 625));
  spinCtrl_wbc_spawn_cnt->SetRange(0, 100);
  spinCtrl_rbc_spawn_cnt->SetRange(0, 100);
  spinCtrl_bone_marrow_replenish_wbc_cnt->SetRange(0, 100);
  spinCtrl_bone_marrow_replenish_rbc_cnt->SetRange(0, 100);
  spinCtrl_wbc_stable_cnt->SetRange(0, 100);
  spinCtrl_rbc_stable_cnt->SetRange(0, 100);
  paramMap[WBC_SPAWN_CNT] = spinCtrl_wbc_spawn_cnt;
  paramMap[RBC_SPAWN_CNT] = spinCtrl_rbc_spawn_cnt;
  paramMap[BONE_MARROW_REPLENISH_WBC_CNT] = spinCtrl_bone_marrow_replenish_wbc_cnt;
  paramMap[BONE_MARROW_REPLENISH_RBC_CNT] = spinCtrl_bone_marrow_replenish_rbc_cnt;
  paramMap[WBC_STABLE_CNT] = spinCtrl_wbc_stable_cnt;
  paramMap[RBC_STABLE_CNT] = spinCtrl_rbc_stable_cnt;

  wxButton *submit_btn = new wxButton(panel, wxID_ANY, "Submit", wxPoint(150, 665));
  submit_btn->Bind(wxEVT_BUTTON, &MainFrame::Submit, this);
  wxButton *clear_form_btn = new wxButton(panel, wxID_ANY, "Clear Form", wxPoint(250, 665));
  clear_form_btn->Bind(wxEVT_BUTTON, &MainFrame::ClearForm, this);

  std::string note = "Note: lifeforce - damage durability; spawn, replenish every 100 iterations.";
  wxStaticText *note_to_user = new wxStaticText(panel, wxID_ANY, note, wxPoint(150, 350));
  CreateStatusBar();
}

void helper_prefill(int index)
{
  for (auto &pair : paramMap)
  {
    pair.second->SetValue(prefillMap.at(pair.first)[index]);
  }
}

void MainFrame::PreFillParams_Cancer(wxCommandEvent &evt)
{
  helper_prefill(0);
}
void MainFrame::PreFillParams_HIV(wxCommandEvent &evt)
{
  helper_prefill(1);
}
void MainFrame::PreFillParams_Flu(wxCommandEvent &evt)
{
  helper_prefill(2);
}
void MainFrame::PreFillParams_Malaria(wxCommandEvent &evt)
{
  helper_prefill(3);
}

void MainFrame::PreFillParams_Test(wxCommandEvent &evt)
{
  helper_prefill(4);
}

void MainFrame::ClearForm(wxCommandEvent &evt)
{
  helper_prefill(5);
}

void MainFrame::Submit(wxCommandEvent &evt)
{
  panel_ref->Enable(false);
  Simulation simulation(paramMap);
  simulation.Run();
}