#include <algorithm>
#include <map>
#include <iostream>
#include <utility>
#include <set>
#include <string>
#include <sstream>
#include <tuple>
#include <unordered_set>
#include <random>

#include <boost/functional/hash.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <wx/spinctrl.h>

#include "constants.hpp"
#include "Matter.hpp"
#include "Simulation.hpp"
#include "RNG.hpp"

// population and bone marrow parameters
RNG_Object rng_object;
std::list<LUCA> virus_pop;
std::list<LUCA> wbc_pop;
std::list<LUCA> rbc_pop;
std::list<LUCA *> infected_cells;
std::list<LUCA *> clear_cells;
int iteration_count = 0;
int wbc_bone_marrow_rsc = 0;
int rbc_bone_marrow_rsc = 0;
const std::pair default_pair(-1, -1);
struct IntDefaultedToZero
{
  int i = 0;
};
std::unordered_set<int> threat_registered;

// prepopulate positions
std::map<std::pair<int, int>, std::list<LUCA *>> virus_map;
std::map<std::pair<int, int>, std::list<LUCA *>> wbc_map;
std::map<std::pair<int, int>, std::list<LUCA *>> rbc_map;

// Yellow Ring - not Aggressive, Red Ring - Pre-infection Aggressive
sf::Color virus_ring = sf::Color::Yellow;
sf::Color wbc_ring = sf::Color::Yellow;
sf::Color rbc_ring = sf::Color::Yellow;

sf::Color green(0, 255, 0);
sf::Color white(255, 255, 255);
sf::Color red(255, 0, 0);

Simulation::Simulation(std::map<std::string, wxSpinCtrl *> initParams)
{
  this->params = initParams;
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  this->window_width = WINDOW_WIDTH;
  this->window_height = WINDOW_HEIGHT;
  this->window.create(sf::VideoMode(this->window_width, this->window_height, desktop.bitsPerPixel), "Cells Simulation", sf::Style::None);
  window.setFramerateLimit(60);

  sf::Font font;
  if (!font.loadFromFile("res/Arial.ttf"))
  {
    std::cout << "Issue loading font" << std::endl;
    system("pause");
  }
  this->font = font;
}

void Simulation::Run()
{
  wbc_bone_marrow_rsc = this->params[INIT_BONE_MARROW_WBC_COUNT]->GetValue();
  rbc_bone_marrow_rsc = this->params[INIT_BONE_MARROW_RBC_COUNT]->GetValue();
  virus_ring = Simulation::ring_color(this->params, VIRAL_AGG_WBC, VIRAL_AGG_RBC);
  wbc_ring = Simulation::ring_color(this->params, WBC_AGG_PCT_INTRA, WBC_AGG_PCT_INTER);
  rbc_ring = Simulation::ring_color(this->params, RBC_AGG_PCT_INTRA, RBC_AGG_PCT_INTER);
  // Populate viruses
  std::list<std::pair<int, int>> v_pos;
  for (int i = 0; i < this->params[INIT_VIRAL_POP_CNT]->GetValue(); i++)
  {
    v_pos.push_back(std::pair(rng_object.x_pos_rng(rng_object.rng), rng_object.y_pos_rng(rng_object.rng)));
  }
  Simulation::instantiate_virus(virus_pop, green, virus_ring, v_pos, std::pair("Virus", 0));
  v_pos.clear();

  // Populate WBC
  std::list<std::pair<int, int>> wbc_pos;
  for (int i = 0; i < this->params[INIT_WBC_POP_CNT]->GetValue(); i++)
  {
    wbc_pos.push_back(std::pair(rng_object.x_pos_rng(rng_object.rng), rng_object.y_pos_rng(rng_object.rng)));
  }
  Simulation::instantiate_cell(wbc_pop, white, wbc_ring, wbc_pos, "WBC",
                               this->params[WBC_LIFEFORCE_CYCLES]->GetValue(),
                               this->params[WBC_AGG_PCT_INTRA]->GetValue(),
                               this->params[WBC_AGG_PCT_INTER]->GetValue());
  wbc_pos.clear();

  // Populate RBC
  std::list<std::pair<int, int>> rbc_pos;
  for (int i = 0; i < this->params[INIT_RBC_POP_CNT]->GetValue(); i++)
  {
    rbc_pos.push_back(std::pair(rng_object.x_pos_rng(rng_object.rng), rng_object.y_pos_rng(rng_object.rng)));
  }
  Simulation::instantiate_cell(rbc_pop, red, rbc_ring, rbc_pos, "RBC",
                               this->params[RBC_LIFEFORCE_CYCLES]->GetValue(),
                               this->params[RBC_AGG_PCT_INTER]->GetValue(),
                               this->params[RBC_AGG_PCT_INTRA]->GetValue());
  rbc_pos.clear();

  Simulation::populate_map(virus_map, virus_pop);
  Simulation::populate_map(wbc_map, wbc_pop);
  Simulation::populate_map(rbc_map, rbc_pop);
  while (this->window.isOpen())
  {
    iteration_count += 1;
    Simulation::Render();
  }
}

void Simulation::Render()
{
  sf::Event event;
  bool wait = false;
  while (this->window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      this->window.close();
  }

  this->window.clear();
  // spawn and replenish
  // To show the path, uncomment the below. Note: will slow down application
  //  Simulation::highlight_path(virus_map, sf::Color::Green);
  //  Simulation::highlight_path(wbc_map, sf::Color::White);
  //  Simulation::highlight_path(rbc_map, sf::Color::Red);
  if (iteration_count % 100 == 0)
  {
    // Spawn until we reach stable levels
    std::uniform_int_distribution<std::mt19937::result_type> x_pos_rng(0, 2 * ATTACK_RADIUS);
    std::uniform_int_distribution<std::mt19937::result_type> y_pos_rng(this->window_height - 2 * ATTACK_RADIUS, this->window_height);
    std::list<std::pair<int, int>> pos_list;
    // Spawn WBC
    if (wbc_pop.size() < this->params[WBC_STABLE_CNT]->GetValue() && this->params[WBC_SPAWN_CNT]->GetValue() <= wbc_bone_marrow_rsc)
    {
      int new_wbc_cnt = this->params[WBC_SPAWN_CNT]->GetValue();
      for (int i = 0; i < new_wbc_cnt; i++)
      {
        pos_list.push_back(std::pair(x_pos_rng(rng_object.rng), y_pos_rng(rng_object.rng)));
      }
      Simulation::instantiate_cell(wbc_pop, white, wbc_ring, pos_list, "WBC",
                                   this->params[WBC_LIFEFORCE_CYCLES]->GetValue(),
                                   this->params[WBC_AGG_PCT_INTRA]->GetValue(),
                                   this->params[WBC_AGG_PCT_INTER]->GetValue());
      wbc_bone_marrow_rsc -= new_wbc_cnt;
      pos_list.clear();
    }
    // Spawn RBC
    if (rbc_pop.size() < this->params[RBC_STABLE_CNT]->GetValue() && this->params[RBC_SPAWN_CNT]->GetValue() <= rbc_bone_marrow_rsc)
    {
      int new_rbc_cnt = this->params[RBC_SPAWN_CNT]->GetValue();
      for (int i = 0; i < new_rbc_cnt; i++)
      {
        pos_list.push_back(std::pair(x_pos_rng(rng_object.rng), y_pos_rng(rng_object.rng)));
      }
      Simulation::instantiate_cell(rbc_pop, red, rbc_ring, pos_list, "RBC",
                                   this->params[RBC_LIFEFORCE_CYCLES]->GetValue(),
                                   this->params[RBC_AGG_PCT_INTRA]->GetValue(),
                                   this->params[RBC_AGG_PCT_INTER]->GetValue());
      rbc_bone_marrow_rsc -= new_rbc_cnt;
      pos_list.clear();
    }
    // Replenish
    wbc_bone_marrow_rsc += this->params[BONE_MARROW_REPLENISH_WBC_CNT]->GetValue();
    rbc_bone_marrow_rsc += this->params[BONE_MARROW_REPLENISH_RBC_CNT]->GetValue();
  }
  // Keep entity update loop separate as it has to update conditions in future, ie mutations, intra-aggression

  // check if viral mutations surpass threshold to be recognized by system
  std::pair<int, int> pos_destination(-1, -1);
  std::map<int, int> mutation_count;
  for (auto &v : virus_pop)
  {
    mutation_count[v.get_mutation_version()] += 1;
    std::pair<int, int> curr_pos_dest = Simulation::handle_virus(v);
    if (curr_pos_dest != default_pair && pos_destination == default_pair)
      pos_destination = curr_pos_dest;
  }
  for (auto const &[key, val] : mutation_count)
  {
    if (val >= this->params[VIRAL_LOAD_THRESHOLD]->GetValue())
      threat_registered.insert(key);
  }

  // wbc/rbc will attack viruses before it has a chance of attacking each other and rbc/wbc
  // dead cells are to be kept track of in a to-be-removed list and removed after the loop
  for (auto &wbc : wbc_pop)
  {
    // check for targeted destination position
    if (wbc.pos_destination == default_pair && pos_destination != default_pair && !wbc.infected) // and if threshold condition TODO
    {
      // shuffle position of destination location
      sf::Vector2f destination_location(pos_destination.first, pos_destination.second);
      std::tuple<int, int, int, int> dim = Simulation::get_range_coordinates(destination_location);
      std::uniform_int_distribution<std::mt19937::result_type> x_pos_rng(std::get<0>(dim), std::get<1>(dim));
      std::uniform_int_distribution<std::mt19937::result_type> y_pos_rng(std::get<2>(dim), std::get<3>(dim));

      wbc.pos_destination = std::pair(x_pos_rng(rng_object.rng), y_pos_rng(rng_object.rng));
    }
    Simulation::handle_cell(wbc, wbc_pop, wbc_map, true);
  }
  for (auto &wbc_addr : clear_cells)
  {
    infected_cells.remove(wbc_addr);
    Simulation::clear_LUCA(wbc_map, wbc_pop, (*wbc_addr), true);
  }
  clear_cells.clear();

  for (auto &rbc : rbc_pop)
  {
    Simulation::handle_cell(rbc, rbc_pop, rbc_map, false);
  }
  for (auto &rbc_addr : clear_cells)
  {
    infected_cells.remove(rbc_addr);
    Simulation::clear_LUCA(rbc_map, rbc_pop, (*rbc_addr), true);
  }
  clear_cells.clear();
  sf::Text object_count;
  object_count.setFont(this->font);
  std::stringstream ss;
  // Bone marrow resource and replenish rate information
  ss << "Bone Marrow Resource - WBC: " << wbc_bone_marrow_rsc << ", RBC: " << rbc_bone_marrow_rsc
     << "\nBone Marrow Replenish Rate - WBC: " << this->params[BONE_MARROW_REPLENISH_WBC_CNT]->GetValue()
     << ", RBC: " << this->params[BONE_MARROW_REPLENISH_RBC_CNT]->GetValue() << "\n";
  // LUCA counts and iteration information
  ss << "Viruses: " << virus_pop.size() << ", WBC: " << wbc_pop.size()
     << ", RBC: " << rbc_pop.size() << ", Infected: " << infected_cells.size()
     << "\nMutation version recognized: ";
  for (auto &mutation_version : threat_registered)
    ss << mutation_version << ", ";
  ss << "\nIterations : " << iteration_count << std::endl;

  // output bone marrow info, frame number also
  object_count.setString(ss.str());
  object_count.setCharacterSize(30);
  object_count.setFillColor(sf::Color::Cyan);
  this->window.draw(object_count);
  this->window.display();
}

int Simulation::getDirection(int index)
{
  if (index == 0)
  {
    return -1;
  }
  return 1;
}

// This function determines whether the LUCA is aggressive or not and assign the appropriate colors.
// Dark Red being that it is aggressive, and Yellow, being that it is not.
sf::Color Simulation::ring_color(std::map<std::string, wxSpinCtrl *> &params, const std::string &agg_param_1, const std::string &agg_param_2)
{
  if (params[agg_param_1]->GetValue() > 0 || params[agg_param_2]->GetValue() > 0)
  {
    // Dark Red color
    return sf::Color(139, 0, 0);
  }
  return sf::Color::Yellow;
}

// Populate map with LUCA address that falls in range of attack (x, y)
void Simulation::populate_map(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, std::list<LUCA> &population)
{
  for (auto &unit : population)
  {
    sf::Vector2f position = unit.getPosition();
    auto region = Simulation::region(position);
    for (auto &pair : region)
    {
      occupied_map[pair].push_back(&unit);
    }
  }
}

// Find enemy to attack
LUCA *Simulation::fetch_first_enemy(LUCA &self, const std::list<LUCA *> &candidates)
{
  std::list<LUCA *> first = candidates;
  first.sort();
  std::list<LUCA *> second;
  // excluded infected cells from selection
  if (self.LUCA_name != "WBC" && self.LUCA_name != "RBC" && !self.reproduction)
  {
    second = infected_cells;
  }
  // reject virus from candidates that don't have mutation identified
  for (auto const &c : candidates)
  {
    int m_version = (*c).get_mutation_version();
    if (m_version != -1 && threat_registered.find(m_version) == threat_registered.end())
      second.push_back(c);
  }

  second.push_back(&self);
  second.sort();
  std::list<LUCA *> results;
  auto it = std::set_difference(first.begin(), first.end(), second.begin(), second.end(), results.begin());
  if (results.size() == 0)
  {
    return nullptr;
  }
  return results.front();
}

// Clear enemy from map and its presence in range of pointers
void Simulation::clear_LUCA(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, std::list<LUCA> &population, LUCA &enemy, bool destroy_LUCA)
{
  std::list<LUCA>::iterator position = std::find(population.begin(), population.end(), enemy);
  if (position != population.end())
  {
    sf::Vector2f coordinates = enemy.getPosition();
    auto region = Simulation::region(coordinates);
    Simulation::clear_path(occupied_map, enemy, region);
    if (destroy_LUCA)
      population.erase(position);
  }
}

// update LUCA positions
void Simulation::update_occupy_map(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map,
                                   LUCA &luca, sf::Vector2f &old_pos, sf::Vector2f &new_pos)
{
  // clear old entries first, individually
  // **vertical/horizontal travel can only happens when the cell has a target it's moving towards
  std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> occupy_new_positions;
  std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> positions_to_remove;
  // clear operations
  int y_begin_old = (int)old_pos.y - ATTACK_RADIUS;
  int y_end_old = (int)old_pos.y + ATTACK_RADIUS;
  int d_x = (int)new_pos.x - old_pos.x;
  bool update_x_remove = false, update_x_add = false;
  int clear_x = 0, add_x = 0;
  if (d_x == 1)
  {
    // clear left side column
    clear_x = (int)old_pos.x - ATTACK_RADIUS;
    if (clear_x >= 0)
      update_x_remove = true;
    // add right side column
    add_x = (int)new_pos.x + ATTACK_RADIUS;
    if (add_x <= this->window_width)
      update_x_add = true;
  }
  if (d_x == -1)
  {
    // clear right side column
    clear_x = (int)old_pos.x + ATTACK_RADIUS;
    if (clear_x <= this->window_width)
      update_x_remove = true;
    // add left side column
    add_x = (int)new_pos.x - ATTACK_RADIUS;
    if (add_x >= 0)
      update_x_add = true;
  }
  if (update_x_remove)
  {
    for (int j = y_begin_old; j <= y_end_old; j++)
      positions_to_remove.insert(std::pair(clear_x, j));
  }

  // row operations
  int x_begin_old = (int)old_pos.x - ATTACK_RADIUS;
  int x_end_old = (int)old_pos.x + ATTACK_RADIUS;
  int d_y = (int)new_pos.y - old_pos.y;
  bool update_y_remove = false, update_y_add = false;
  int clear_y = 0, add_y = 0;
  if (d_y == 1)
  {
    // clear top row
    clear_y = (int)old_pos.y - ATTACK_RADIUS;
    if (clear_y >= 0)
      update_y_remove = true;
    // add bottom row
    add_y = (int)new_pos.y + ATTACK_RADIUS;
    if (add_y <= this->window_height)
      update_y_add = true;
  }
  if (d_y == -1)
  {
    // clear bottom row
    clear_y = (int)old_pos.y + ATTACK_RADIUS;
    if (clear_y <= this->window_width)
      update_y_remove = true;
    // add top row
    add_y = (int)new_pos.y - ATTACK_RADIUS;
    if (add_y >= 0)
      update_y_add = true;
  }
  if (update_y_remove)
  {
    for (int i = x_begin_old; i <= x_end_old; i++)
      positions_to_remove.insert(std::pair(i, clear_y));
  }

  // Add path - x, shifts coordinates if cell is moving in both axis
  if (update_x_add)
  {
    int y_begin_new = (int)new_pos.y - ATTACK_RADIUS;
    int y_end_new = (int)new_pos.y + ATTACK_RADIUS;
    for (int j = y_begin_new; j <= y_end_new; j++)
      occupy_new_positions.insert(std::pair(add_x, j));
  }
  // Add path - y
  if (update_y_add)
  {
    int x_begin_new = (int)new_pos.x - ATTACK_RADIUS;
    int x_end_new = (int)new_pos.x + ATTACK_RADIUS;
    for (int i = x_begin_new; i <= x_end_new; i++)
      occupy_new_positions.insert(std::pair(i, add_y));
  }
  // update new positions, jointly, and check if new position is within borders
  Simulation::clear_path(occupied_map, luca, positions_to_remove);
  Simulation::add_path(occupied_map, luca, occupy_new_positions);
}

// highlight path for each cell - used for debugging to show moving path, probably best to be used on one cell.
int Simulation::highlight_path(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, sf::Color color)
{
  int pixels_in_use = 0;
  for (auto const &[key, val] : occupied_map)
  {
    if (!val.empty())
    {
      pixels_in_use += 1;
      sf::VertexArray point(sf::Points, 1);
      point[0].position = sf::Vector2f(key.first, key.second);
      point[0].color = color;
      this->window.draw(point);
    }
  }
  return pixels_in_use;
}

// remove from occupied map
void Simulation::clear_path(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, LUCA &luca_ptr, std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> &pos_to_clear)
{
  for (auto &pair : pos_to_clear)
  {
    std::list<LUCA *> &curr_vector = occupied_map[pair];
    std::list<LUCA *>::iterator position = std::find(curr_vector.begin(), curr_vector.end(), &luca_ptr);
    if (position != curr_vector.end())
      curr_vector.erase(position);
  }
}

// add to occupied map
void Simulation::add_path(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, LUCA &luca_ptr,
                          std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> &new_positions)
{
  for (auto &pair : new_positions)
  {
    if (0 <= pair.first && pair.first <= this->window_width && 0 <= pair.second && pair.second <= this->window_height)
      occupied_map[pair].push_back(&luca_ptr);
  }
}

// fetch attack region of LUCA
std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>>
Simulation::region(sf::Vector2f position)
{
  std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> region;
  auto ranges = Simulation::get_range_coordinates(position);
  int left = std::get<0>(ranges);
  int right = std::get<1>(ranges);
  int top = std::get<2>(ranges);
  int bot = std::get<3>(ranges);
  for (int i = left; i <= right; i++)
  {
    for (int j = top; j <= bot; j++)
    {
      region.insert(std::pair(i, j));
    }
  }
  return region;
}

// get x, y ranges : left, right, top, bottom
std::tuple<int, int, int, int> Simulation::get_range_coordinates(sf::Vector2f &position)
{
  std::tuple<int, int, int, int> ranges;
  std::get<0>(ranges) = std::max((int)position.x - ATTACK_RADIUS, 0);
  std::get<1>(ranges) = std::min((int)position.x + ATTACK_RADIUS, this->window_width);
  std::get<2>(ranges) = std::max((int)position.y - ATTACK_RADIUS, 0);
  std::get<3>(ranges) = std::min((int)position.y + ATTACK_RADIUS, this->window_height);
  return ranges;
}

// instantiate virus
void Simulation::instantiate_virus(std::list<LUCA> &virus_pop, sf::Color &fill, sf::Color &ring,
                                   std::list<std::pair<int, int>> &pos_list, std::pair<std::string, int> name_version)
{
  for (auto &pos : pos_list)
  {
    int x = this->getDirection(rng_object.d_pos(rng_object.rng)), y = this->getDirection(rng_object.d_pos(rng_object.rng));
    Virus virus(this->params[VIRUS_LIFEFORCE_CYCLES]->GetValue(), (float)this->params[VIRAL_AGG_WBC]->GetValue() / 100,
                (float)this->params[VIRAL_AGG_RBC]->GetValue() / 100, x, y, name_version.second, (float)this->params[VIRAL_INFECTION_RATE]->GetValue() / 100,
                (float)this->params[VIRAL_MUTATION_RATE]->GetValue() / 100, this->params[VIRAL_MUTATION_CAP]->GetValue(), this->params[VIRAL_REPRODUCTION_FACTOR]->GetValue(), name_version.first);
    virus.setFillColor(fill);
    virus.setOutlineColor(ring);
    virus.setPosition(pos.first, pos.second);
    virus.setOrigin(virus.getRadius(), virus.getRadius());
    virus_pop.push_back(virus);
  }
}

// instantiate cells
void Simulation::instantiate_cell(std::list<LUCA> &cell_pop, sf::Color &fill, sf::Color &ring,
                                  std::list<std::pair<int, int>> &pos_list, std::string name,
                                  int lifeforce, int wbc_agg, int rbc_agg)
{
  for (auto &pos : pos_list)
  {
    int x = this->getDirection(rng_object.d_pos(rng_object.rng)), y = this->getDirection(rng_object.d_pos(rng_object.rng));
    Cell cell(lifeforce, (float)wbc_agg / 100, (float)rbc_agg / 100, x, y, 0.0, name);
    cell.setFillColor(fill);
    cell.setOutlineColor(ring);
    cell.setPosition(pos.first, pos.second);
    cell.setOrigin(cell.getRadius(), cell.getRadius());
    cell_pop.push_back(cell);
  }
}

// handle virus actions, returns position of dead cell
std::pair<int, int> Simulation::handle_virus(LUCA &virus)
{
  std::pair<int, int> pos_destination(-1, -1);
  sf::Vector2f old_pos = virus.getPosition();
  LUCA *enemy = nullptr;
  auto population = &wbc_pop;
  auto range_map = &wbc_map;
  if (this->params[VIRAL_AGG_WBC]->GetValue())
    enemy = Simulation::fetch_first_enemy(virus, wbc_map[std::pair(old_pos.x, old_pos.y)]);
  if (enemy == nullptr && this->params[VIRAL_AGG_RBC]->GetValue())
  {
    enemy = Simulation::fetch_first_enemy(virus, rbc_map[std::pair(old_pos.x, old_pos.y)]);
    population = &rbc_pop;
    range_map = &rbc_map;
  }

  if (enemy != nullptr)
  {
    bool old_infection_state = (*enemy).infected;
    int enemy_health = virus.signal_attacking(this->window, (*enemy), virus.getFillColor(), rng_object);
    if (enemy_health <= 0)
    {
      sf::Vector2f enemy_position = (*enemy).getPosition();
      pos_destination = std::pair((int)enemy_position.x, (int)enemy_position.y);
      infected_cells.remove(enemy);
      if ((*enemy).infected && virus.reproduction)
      {
        std::list<std::pair<int, int>> pos_list;
        sf::Vector2f enemy_position = (*enemy).getPosition();
        std::tuple<int, int, int, int> dim = Simulation::get_range_coordinates(enemy_position);
        std::uniform_int_distribution<std::mt19937::result_type> x_pos_rng(std::get<0>(dim), std::get<1>(dim));
        std::uniform_int_distribution<std::mt19937::result_type> y_pos_rng(std::get<2>(dim), std::get<3>(dim));
        for (int i = 0; i < virus.reproduction; i++)
        {
          pos_list.push_back(std::pair(x_pos_rng(rng_object.rng), y_pos_rng(rng_object.rng)));
        }
        int new_mutation_version = virus.mutation_version;
        if (virus.mutation_success_rate &&
            virus.mutation_success_rate >= rng_object.distribution(rng_object.gen) &&
            virus.mutation_version < this->params[VIRAL_MUTATION_CAP]->GetValue())
        {
          new_mutation_version += 1;
        }
        sf::Color ring_color(90, 90, 90);
        if (new_mutation_version <= 0)
        {
          ring_color = virus_ring;
        }
        Simulation::instantiate_virus(virus_pop, green, ring_color, pos_list, std::pair("Virus", new_mutation_version));
      }
      Simulation::clear_LUCA((*range_map), (*population), (*enemy), true);
    }
    else if ((*enemy).infected)
    {
      if (old_infection_state != (*enemy).infected)
      {
        infected_cells.push_back(enemy);
        if (population == &wbc_pop)
        {
          (*enemy).wbc_aggressive = (float)this->params[WBC_AGG_PCT_INTRA_INFECTED]->GetValue() / 100;
          (*enemy).rbc_aggressive = (float)this->params[WBC_AGG_PCT_INTER_INFECTED]->GetValue() / 100;
        }
        else
        {
          (*enemy).wbc_aggressive = (float)this->params[RBC_AGG_PCT_INTER_INFECTED]->GetValue() / 100;
          (*enemy).rbc_aggressive = (float)this->params[RBC_AGG_PCT_INTRA_INFECTED]->GetValue() / 100;
        }
      }
      if (!virus.reproduction)
        Simulation::clear_LUCA((*range_map), (*population), (*enemy), false);
    }
  }
  else
  {
    Simulation::idle(virus, virus_map, old_pos, default_pair);
  }
  virus.signal_attacked();
  this->window.draw(virus);
  return pos_destination;
}

// handle cell actions
void Simulation::handle_cell(LUCA &cell, std::list<LUCA> &cell_pop, std::map<std::pair<int, int>, std::list<LUCA *>> &cell_map, bool target_virus)
{
  sf::Vector2f old_pos = cell.getPosition();
  if (cell.pos_destination != std::pair(-1, -1) && !cell.infected)
  {
    Simulation::idle(cell, cell_map, old_pos, cell.pos_destination);
  }
  else
  {
    LUCA *enemy = nullptr;
    auto population = &virus_pop;
    auto range_map = &virus_map;
    if (target_virus && !cell.infected)
    {
      enemy = Simulation::fetch_first_enemy(cell, virus_map[std::pair(old_pos.x, old_pos.y)]);
    }
    LUCA *target = nullptr;
    if (enemy == nullptr && cell.wbc_aggressive > 0)
    {
      target = Simulation::fetch_first_enemy(cell, wbc_map[std::pair(old_pos.x, old_pos.y)]);
      if (target != nullptr && cell.wbc_aggressive >= rng_object.distribution(rng_object.gen))
      {
        population = &wbc_pop;
        range_map = &wbc_map;
        enemy = target;
      }
    }
    if (target == nullptr && enemy == nullptr && cell.rbc_aggressive > 0)
    {
      if (cell.rbc_aggressive >= rng_object.distribution(rng_object.gen))
      {
        population = &rbc_pop;
        range_map = &rbc_map;
        enemy = Simulation::fetch_first_enemy(cell, rbc_map[std::pair(old_pos.x, old_pos.y)]);
      }
    }

    if (enemy != nullptr)
    {
      int enemy_health = cell.signal_attacking(this->window, (*enemy), cell.getFillColor(), rng_object);
      if (enemy_health <= 0)
      {
        if (cell.LUCA_name == (*enemy).LUCA_name)
        {
          clear_cells.push_back(enemy);
        }
        else
        {
          Simulation::clear_LUCA((*range_map), (*population), (*enemy), true);
          infected_cells.remove(enemy);
        }
      }
    }
    else
    {
      Simulation::idle(cell, cell_map, old_pos, default_pair);
    }
  }
  cell.signal_attacked();
  this->window.draw(cell);
}

// LUCA continues traveling on path without any engangements
void Simulation::idle(LUCA &luca, std::map<std::pair<int, int>, std::list<LUCA *>> &luca_map, sf::Vector2f &old_pos, std::pair<int, int> pos_destination)
{
  luca.attacking = false;
  if (pos_destination != default_pair)
    luca.move(pos_destination);
  else
    luca.move(this->window_width, this->window_height);
  sf::Vector2f new_pos = luca.getPosition();
  if (new_pos != old_pos)
    Simulation::update_occupy_map(luca_map, luca, old_pos, new_pos);
}