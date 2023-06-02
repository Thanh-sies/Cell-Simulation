#include <iostream>
#include <random>

#include "RNG.hpp"
#include "constants.hpp"

RNG_Object::RNG_Object()
{
  std::mt19937 rng(static_cast<long unsigned int>(time(0)));
  std::mt19937 gen(0);
  std::uniform_int_distribution<std::mt19937::result_type> dist_window_width(0, WINDOW_WIDTH);
  std::uniform_int_distribution<std::mt19937::result_type> dist_window_height(0, WINDOW_HEIGHT);
  std::uniform_int_distribution<std::mt19937::result_type> dist_direction(0, 1);
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  this->rng = rng;
  this->x_pos_rng = dist_window_width;
  this->y_pos_rng = dist_window_height;
  this->d_pos = dist_direction;
  this->distribution = distribution;
}