#pragma once

#include <iostream>
#include <random>

class RNG_Object
{
public:
  std::mt19937 rng;
  std::mt19937 gen;
  std::uniform_int_distribution<std::mt19937::result_type> x_pos_rng;
  std::uniform_int_distribution<std::mt19937::result_type> y_pos_rng;
  std::uniform_int_distribution<std::mt19937::result_type> d_pos;
  std::uniform_real_distribution<double> distribution;
  RNG_Object();
};