#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "RNG.hpp"

class LUCA : public sf::CircleShape
{
  // Last Universal Common Ancestor
private:
  int d_x;
  int d_y;

public:
  virtual ~LUCA() {}
  LUCA(int lifeforce, float wbc_aggressive, float rbc_aggressive, int d_x, int d_y, float infection_success_rate, std::string LUCA_name, int mutation_version, float mutation_success_rate, int mutation_upper_limit, int reproduction);
  float mutation_success_rate;
  float infection_success_rate;
  int mutation_upper_limit;
  int mutation_version;
  int reproduction;
  std::pair<int, int> pos_destination;
  float wbc_aggressive;
  float rbc_aggressive;
  int lifeforce;
  int danger_duration;
  bool under_attack;
  bool attacking;
  bool infected;
  std::string LUCA_name;
  bool move(int window_width, int window_height);
  bool move(std::pair<int, int> &pos_destination);
  void signal_attacked();
  int signal_attacking(sf::RenderWindow &window, LUCA &destination, const sf::Color &color_origin, RNG_Object &rng_object);
  int get_mutation_version();
  bool operator==(const LUCA &right) const;
};

class Cell : public LUCA
{
public:
  bool infected;
  Cell(int lifeforce, float wbc_aggressive, float rbc_aggressive, int d_x, int d_y, float infection_success_rate, std::string cell_type);
};

class Virus : public LUCA
{
private:
public:
  Virus(int lifeforce, float wbc_aggressive, float rbc_aggressive, int d_x, int d_y,
        int mutation_version, float infection_success_rate, float mutation_success_rate,
        int mutation_upper_limit, int reproduction, std::string LUCA_name);
};
