#include "../../include/encoder.hpp"

#include <vector>
#include <iostream>
#include <format>

#define BEQ_OPCODE 0b1100011
#define BEQ_FUNCT3 0b0

int main() {
  unsigned res {};
  rv32i::encoder_type test{};
  std::vector<std::tuple<int, int, int>> list {
    {2, 3, 4},
  };
  for (auto &&[a,b,c] : list) {
    test = rv32i::b{BEQ_OPCODE, BEQ_FUNCT3, a,b,c};
    res = rv32i::encode(test);
    std::cout << std::format("{:x}\n", res);
  }
}
