#include <string>

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Matter.hpp"
#include "RNG.hpp"

// Last Universal Common Ancestor - LUCA constructor
LUCA::LUCA(int lifeforce, float wbc_aggressive, float rbc_aggressive, int d_x, int d_y, float infection_success_rate, std::string LUCA_name, int mutation_version, float mutation_success_rate, int mutation_upper_limit, int reproduction) : sf::CircleShape(CELL_SIZE, 30)
{
  this->mutation_version = mutation_version;
  this->mutation_success_rate = mutation_success_rate;
  this->mutation_upper_limit = mutation_upper_limit;
  this->reproduction = reproduction;
  this->lifeforce = lifeforce;
  this->wbc_aggressive = wbc_aggressive;
  this->rbc_aggressive = rbc_aggressive;
  this->d_x = d_x;
  this->d_y = d_y;
  this->danger_duration = DANGER_DURATION;
  this->under_attack = false;
  this->attacking = false;
  this->infected = false;
  this->infection_success_rate = infection_success_rate;
  this->LUCA_name = LUCA_name;
  this->pos_destination = std::pair(-1, -1);
  this->setOutlineThickness(10);
}

// Move method will not update the position when used in conjunction with attack
bool LUCA::move(int window_width, int window_height)
{
  // update cell under_attack status
  if (this->under_attack)
  {
    if (this->danger_duration > 0)
    {
      this->danger_duration -= 1;
    }
    else
    {
      this->danger_duration = DANGER_DURATION;
      this->under_attack = false;
    }
  }
  if (this->under_attack || this->attacking)
    return false;
  sf::Vector2f position = this->getPosition();
  if (position.x > window_width || position.x < 0)
    this->d_x = -d_x;
  if (position.y > window_height || position.y < 0)
    this->d_y = -d_y;
  this->setPosition(position.x + d_x, position.y + d_y);
  return true;
}

// move towards target destination
bool LUCA::move(std::pair<int, int> &pos_destination)
{
  sf::Vector2f position = this->getPosition();
  if ((int)position.x == pos_destination.first && (int)position.y == pos_destination.second)
  {
    this->pos_destination = std::pair(-1, -1);
    return false;
  }
  int d_x = 0, d_y = 0;
  if (position.x < pos_destination.first)
    d_x = 1;
  else if (position.x > pos_destination.first)
    d_x = -1;
  if (position.y < pos_destination.second)
    d_y = 1;
  else if (position.y > pos_destination.second)
    d_y = -1;
  if (d_x == 0 && d_y == 0)
    return false;
  this->setPosition(position.x + d_x, position.y + d_y);
  return true;
}

bool LUCA::operator==(const LUCA &right) const
{
  return (this == &right);
}

void LUCA::signal_attacked()
{
  sf::Color filter;
  filter = this->getFillColor();
  // Switch between opacity values.
  if (this->under_attack && this->danger_duration > 0)
  {
    if (filter.a == 255)
    {
      filter.a = 1;
    }
    else
    {
      filter.a = 255;
    }
    if (this->danger_duration > 0)
    {
      this->danger_duration -= 1;
    }
  }
  else
  {
    this->danger_duration = 5;
    this->under_attack = false;
    filter.a = 255;
  }
  this->setFillColor(filter);
}

// Makes calculations for results of attack and generates line between current LUCA to enemy
int LUCA::signal_attacking(sf::RenderWindow &window, LUCA &destination, const sf::Color &color_origin, RNG_Object &rng_object)
{
  if (this->infection_success_rate && this->infection_success_rate >= rng_object.distribution(rng_object.gen))
  {
    destination.infected = true;
    destination.setOutlineColor(sf::Color::Green);
  }
  this->attacking = true;
  destination.under_attack = true;
  destination.lifeforce -= 1;
  destination.danger_duration = DANGER_DURATION; // reset danger duration
  sf::VertexArray line(sf::LinesStrip, 2);
  sf::Vector2f this_position = this->getPosition();
  sf::Vector2f dest_position = destination.getPosition();
  if (destination.lifeforce > 0)
  {
    line[0].position = sf::Vector2f(this_position.x, this_position.y);
    line[0].color = color_origin;
    line[1].position = sf::Vector2f(dest_position.x, dest_position.y);
    line[1].color = color_origin;
    window.draw(line);
  }
  return destination.lifeforce;
}

// Cell constructor
Cell::Cell(int lifeforce, float wbc_aggressive, float rbc_aggressive, int d_x, int d_y, float infection_success_rate, std::string cell_type)
    : LUCA::LUCA(lifeforce, wbc_aggressive, rbc_aggressive, d_x, d_y, 0.0, cell_type, -1, 0, 0, 0)
{
  this->infected = false;
}

int LUCA::get_mutation_version()
{
  return this->mutation_version;
}

// Virus constructor
Virus::Virus(int lifeforce, float wbc_aggressive, float rbc_aggressive, int d_x, int d_y,
             int mutation_version, float infection_success_rate, float mutation_success_rate,
             int mutation_upper_limit, int reproduction, std::string LUCA_name) : LUCA::LUCA(lifeforce, wbc_aggressive, rbc_aggressive, d_x, d_y, infection_success_rate, LUCA_name, mutation_version, mutation_success_rate, mutation_upper_limit, reproduction)
{
}