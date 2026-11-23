#include "../../include/encoder.hpp"

#include <vector>
#include <iostream>
#include <format>

#define JAL_OPCODE 0b1101111

int main() {
  unsigned res {};
  rv32i::encoder_type test{};
  std::vector<std::tuple<int, int>> list {
    {0, 0},
    {0, 2},
    {2, 0},
    {0, 1048574},
    {0, -1048576},
    {0, 2048},
    {0, -2048},
    {1, -1}
  };
  for (auto &&[a,b] : list) {
    test = rv32i::j{JAL_OPCODE, a, b};
    res = rv32i::encode(test);
    std::cout << std::format("{:x}\n", res);
  }
}
