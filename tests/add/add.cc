#include "../../include/encoder.hpp"

#include <vector>
#include <iostream>
#include <format>

#define ADD_OPCODE 0b110011
#define ADD_FUNCT3 0b0

int main() {
  unsigned res {};
  rv32i::encoder_type test{};
  std::vector<std::tuple<int, int, int>> list {
    {2, 3, 5},
    {5, 3, 4},
    {4, 6, 7},
    {5, 6, 9},
    {6, 7, 11},
    {3, 2, 12},
    {3, 2, 31}
  };
  for (auto &&[a,b,c] : list) {
    test = rv32i::r{ADD_OPCODE, a, ADD_FUNCT3, b, c};
    res = rv32i::encode(test);
    std::cout << std::format("{:x}\n", res);
  }
}
