#include "../../include/encoder.hpp"

#include <vector>
#include <iostream>
#include <format>

#define ADDI_OPCODE 0b10011
#define ADDI_FUNCT3 0b0

int main() {
  unsigned res {};
  rv32i::encoder_type test{};
  std::vector<std::tuple<int, int, int>> list {
    {2, 3, 5},
    {5, 3, 2},
    {4, 6, 7},
    {5, 6, -224},
    {6, 7, -62},
    {3, 2, -1337},
    {3, 2, -2041},
    {1, 0, 2047},
    {1, 0, -2048},
    {0, 0, 0}
  };
  for (auto &&[a,b,c] : list) {
    test = rv32i::i{ADDI_OPCODE, a, ADDI_FUNCT3, b, c};
    res = rv32i::encode(test);
    std::cout << std::format("{:x}\n", res);
  }
}
