#pragma once

#include <string>
#include <map>
#include <vector>

const int WINDOW_HEIGHT = 1200;
const int WINDOW_WIDTH = 1400;

const int CELL_SIZE = 20;
const int ATTACK_RADIUS = 50;
const int DANGER_DURATION = 5;

const std::string WBC_AGG_PCT_INTRA = "WBC_Aggression_Percentage_Intra";
const std::string RBC_AGG_PCT_INTRA = "RBC_Aggression_Percentage_Intra";
const std::string WBC_AGG_PCT_INTER = "WBC_Aggression_Percentage_Inter";
const std::string RBC_AGG_PCT_INTER = "RBC_Aggression_Percentage_Inter";

const std::string WBC_AGG_PCT_INTRA_INFECTED = "WBC_Aggression_Percentage_Intra_Infected";
const std::string RBC_AGG_PCT_INTRA_INFECTED = "RBC_Aggression_Percentage_Intra_Infected";
const std::string WBC_AGG_PCT_INTER_INFECTED = "WBC_Aggression_Percentage_Inter_Infected";
const std::string RBC_AGG_PCT_INTER_INFECTED = "RBC_Aggression_Percentage_Inter_Infected";

const std::string INIT_VIRAL_POP_CNT = "INIT_Viral_Population_Count";
const std::string INIT_WBC_POP_CNT = "INIT_WBC_Population_Count";
const std::string INIT_RBC_POP_CNT = "INIT_RBC_Population_Count";
const std::string INIT_BONE_MARROW_WBC_COUNT = "INIT_Bone_Marrow_WBC_Population_Count";
const std::string INIT_BONE_MARROW_RBC_COUNT = "INIT_Bone_Marrow_RBC_Population_Count";

const std::string VIRUS_LIFEFORCE_CYCLES = "LIFEFORCE_Virus";
const std::string WBC_LIFEFORCE_CYCLES = "LIFEFORCE_WBC";
const std::string RBC_LIFEFORCE_CYCLES = "LIFEFORCE_RBC";

const std::string VIRAL_INFECTION_RATE = "VIRAL_Infection_Rate";
const std::string VIRAL_REPRODUCTION_FACTOR = "VIRAL_Reproduction_Factor";
const std::string VIRAL_MUTATION_RATE = "VIRAL_Mutation_Rate";
const std::string VIRAL_MUTATION_CAP = "VIRAL_Mutation_Cap";
const std::string VIRAL_LOAD_THRESHOLD = "VIRAL_Load_Threshold";

const std::string WBC_SPAWN_CNT = "WBC_Spawn_Rate";
const std::string RBC_SPAWN_CNT = "RBC_Spawn_Rate";
const std::string BONE_MARROW_REPLENISH_WBC_CNT = "BONE_MARROW_Replenish_WBC_Count";
const std::string BONE_MARROW_REPLENISH_RBC_CNT = "BONE_MARROW_Replenish_RBC_Count";
const std::string WBC_STABLE_CNT = "WBC_Stable_Count";
const std::string RBC_STABLE_CNT = "RBC_Stable_Count";

const std::string VIRAL_AGG_WBC = "VIRAL_AGG_WBC";
const std::string VIRAL_AGG_RBC = "VIRAL_AGG_RBC";

// default 4 virus types param
// cancer, hiv, flu, malaria, test

const std::map<std::string, std::vector<int>> prefillMap = {
    {WBC_AGG_PCT_INTRA, {0, 0, 0, 0, 20, 0}},
    {RBC_AGG_PCT_INTRA, {0, 0, 0, 0, 25, 0}},
    {WBC_AGG_PCT_INTER, {0, 0, 0, 0, 20, 0}},
    {RBC_AGG_PCT_INTER, {0, 0, 0, 0, 25, 0}},

    {WBC_AGG_PCT_INTRA_INFECTED, {25, 25, 25, 0, 40, 0}},
    {RBC_AGG_PCT_INTRA_INFECTED, {0, 0, 0, 25, 50, 0}},
    {WBC_AGG_PCT_INTER_INFECTED, {0, 0, 0, 0, 40, 0}},
    {RBC_AGG_PCT_INTER_INFECTED, {0, 0, 0, 0, 50, 0}},

    {INIT_VIRAL_POP_CNT, {5, 5, 5, 5, 5, 0}},
    {INIT_WBC_POP_CNT, {5, 5, 5, 5, 5, 0}},
    {INIT_RBC_POP_CNT, {5, 5, 5, 5, 5, 0}},
    {INIT_BONE_MARROW_WBC_COUNT, {100, 100, 100, 100, 100, 0}},
    {INIT_BONE_MARROW_RBC_COUNT, {100, 100, 100, 100, 100, 0}},

    {VIRUS_LIFEFORCE_CYCLES, {30, 30, 30, 30, 30, 1}},
    {WBC_LIFEFORCE_CYCLES, {50, 50, 50, 50, 50, 1}},
    {RBC_LIFEFORCE_CYCLES, {50, 50, 50, 50, 50, 1}},

    {VIRAL_INFECTION_RATE, {20, 20, 20, 20, 50, 0}},
    {VIRAL_MUTATION_RATE, {15, 60, 25, 35, 10, 0}},
    {VIRAL_MUTATION_CAP, {3, 10, 5, 3, 2, 0}},
    {VIRAL_LOAD_THRESHOLD, {10, 10, 10, 10, 5, 1}},
    {VIRAL_REPRODUCTION_FACTOR, {1, 3, 0, 1, 0, 0}},

    {WBC_SPAWN_CNT, {2, 2, 2, 2, 2, 0}},
    {RBC_SPAWN_CNT, {2, 2, 2, 2, 2, 0}},
    {BONE_MARROW_REPLENISH_WBC_CNT, {5, 5, 5, 5, 5, 0}},
    {BONE_MARROW_REPLENISH_RBC_CNT, {5, 5, 5, 5, 5, 0}},
    {WBC_STABLE_CNT, {10, 10, 10, 10, 10, 0}},
    {RBC_STABLE_CNT, {10, 10, 10, 10, 10, 0}},

    {VIRAL_AGG_WBC, {1, 1, 1, 0, 1, 0}},
    {VIRAL_AGG_RBC, {0, 0, 0, 1, 1, 0}}

};
