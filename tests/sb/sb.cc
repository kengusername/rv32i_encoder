#include "../../include/encoder.hpp"

#include <vector>
#include <iostream>
#include <format>

#define SB_OPCODE 0b100011
#define SB_FUNCT3 0b0

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
    test = rv32i::s{SB_OPCODE, SB_FUNCT3, c, a, b};
    res = rv32i::encode(test);
    std::cout << std::format("{:x}\n", res);
  }
}
