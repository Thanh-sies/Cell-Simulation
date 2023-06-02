#pragma once

#include <iostream>
#include <map>
#include <utility>
#include <list>
#include <string>
#include <tuple>
#include <unordered_set>
#include <random>

#include <boost/functional/hash.hpp>
#include <wx/spinctrl.h>

#include "Matter.hpp"

class Simulation
{
private:
  std::map<std::string, wxSpinCtrl *> params;
  int window_width;
  int window_height;
  sf::RenderWindow window;
  sf::Font font;
  void Render();

public:
  Simulation(std::map<std::string, wxSpinCtrl *>);
  int getDirection(int index);
  int highlight_path(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, sf::Color color);
  void instantiate_virus(std::list<LUCA> &virus_pop, sf::Color &fill, sf::Color &ring, std::list<std::pair<int, int>> &pos_list, std::pair<std::string, int> name_pair);
  void instantiate_cell(std::list<LUCA> &cell_pop, sf::Color &fill, sf::Color &ring,std::list<std::pair<int, int>> &pos_list, std::string name, int lifeforce, int wbc_agg, int rbc_agg);
  void populate_map(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, std::list<LUCA> &population);
  void update_occupy_map(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, LUCA &luca, sf::Vector2f &old_pos, sf::Vector2f &new_pos);
  void clear_path(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, LUCA &luca, std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> &pos_to_clear);
  void add_path(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, LUCA &luca_ptr, std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> &new_positions);
  void clear_LUCA(std::map<std::pair<int, int>, std::list<LUCA *>> &occupied_map, std::list<LUCA> &population, LUCA &enemy, bool destroy_LUCA);
  std::pair<int ,int> handle_virus(LUCA &virus);
  void handle_cell(LUCA &cell, std::list<LUCA> &cell_pop, std::map<std::pair<int, int>, std::list<LUCA *>> &cell_map, bool target_virus);
  void idle(LUCA &luca, std::map<std::pair<int, int>, std::list<LUCA *>> &luca_map, sf::Vector2f &old_pos, std::pair<int, int> pos_destination);
  std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> region(sf::Vector2f position);
  std::tuple<int, int, int, int> get_range_coordinates(sf::Vector2f &position);
  sf::Color ring_color(std::map<std::string, wxSpinCtrl *> &params, const std::string &agg_param_1, const std::string &agg_param_2);
  LUCA *fetch_first_enemy(LUCA &self, const std::list<LUCA *> &candidates);
  void Run();
};