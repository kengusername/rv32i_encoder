#include "../../include/encoder.hpp"

#include <vector>
#include <iostream>
#include <format>

#define LB_OPCODE 0b11
#define LB_FUNCT3 0b0

int main() {
  unsigned res {};
  rv32i::encoder_type test{};
  std::vector<std::tuple<int, int, int>> list {
    {2, 204, 3},
    {2, -204, 3},
    {0, -2048, 2},
    {0, 2047, 1},
    {1, 2047, 30},
    {0, 0, 0}
  };
  for (auto &&[a,b,c] : list) {
    test = rv32i::i{LB_OPCODE, a, LB_FUNCT3, c, b};
    res = rv32i::encode(test);
    std::cout << std::format("{:x}\n", res);
  }
}
