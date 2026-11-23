#include "../../include/encoder.hpp"

#include <vector>
#include <iostream>
#include <format>

#define AUIPC_OPCODE 0b10111

int main() {
  unsigned res {};
  rv32i::encoder_type test{};
  std::vector<std::tuple<int, int>> list {
    {2, 62},
    {4, 132},
    {5, 61},
    {14, 15224},
    {3, 15524},
    {2, 524288},
    {27, 1}
  };
  for (auto &&[a,b] : list) {
    test = rv32i::u{AUIPC_OPCODE, a, b};
    res = rv32i::encode(test);
    std::cout << std::format("{:x}\n", res);
  }
}
