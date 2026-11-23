#pragma once
#include <format>
#include <tuple>
#include <variant>

namespace rv32i {
namespace details {

template<typename... ts> struct helper : ts... { using ts::operator()...; };

constexpr auto forward_tuple(auto &&...xs) {
  return std::tuple<decltype(xs)...>( (decltype(xs))xs... );
}

constexpr auto shift_and_or(auto &&off, auto &&val) {
  auto &&[...p1] = val;
  auto &&[...p2] = off;
  static_assert(sizeof...(p1) == sizeof...(p2));

  return 0u + ((p1 << p2) | ...);
}
  namespace format {
  struct r {
    unsigned opcode : 7;
    unsigned rd : 5;
    unsigned funct3 : 3;
    unsigned rs1 : 5;
    unsigned rs2 : 5;
    unsigned funct7 : 7;
  };
  struct i {
    unsigned opcode : 7;
    unsigned rd : 5;
    unsigned funct3 : 3;
    unsigned rs1 : 5;
    unsigned imm : 12;
    i(auto o, auto r, auto f, auto rr, auto i) : opcode(o), rd(r), funct3(f), rs1(rr), imm(i) {}
  };
  struct s {
    unsigned opcode : 7;
    unsigned funct3 : 3;
    unsigned rs1 : 5;
    unsigned rs2 : 5;
    unsigned imm : 12;
    s(auto o, auto f, auto r, auto rr, auto i) : opcode(o), funct3(f), rs1(r), rs2(rr), imm(i) {}
  };
  struct b : s {
    b(auto o, auto f, auto r, auto rr, auto i) : s(o,f,r,rr,i>>1) {}
  }; // both B and S share the same layout, only differences in encoding behavior
  struct u {
    unsigned opcode : 7;
    unsigned rd : 5;
    unsigned imm : 20;
    u(auto o, auto r, auto i) : opcode(o), rd(r), imm(i) {}
  };
  struct j : u {
    j(auto o, auto r, auto i) : u(o, r, i>>1) {}
  }; // same situation as with S/B formats
  }
template<typename X, typename Y>
concept same = std::is_same_v<std::remove_cvref_t<X>, std::remove_cvref_t<Y>>;

constexpr auto _ = helper {
  [](same<format::r> auto &&x) {
    return shift_and_or(forward_tuple(0, 7, 25, 15, 20, 12), x);
  },
  [](same<format::i> auto &&x) {
    return shift_and_or(forward_tuple(0, 7, 12, 15, 20), x);
  },
  [](same<format::s> auto &&x){
    auto imm_4_0 = x.imm & 0x1F;
    auto imm_11_5 = (x.imm & 0xFE0) >> 5;
    return shift_and_or(forward_tuple(0, 7, 12, 15, 20, 25),
      forward_tuple(auto(x.opcode), imm_4_0, auto(x.funct3), auto(x.rs1), auto(x.rs2), imm_11_5));
  },
  [](same<format::b> auto &&x) {
    if (x.imm & 0x1) [[unlikely]]
     throw "instruction address misaligned";

    auto imm = static_cast<signed>(x.imm << 20) >> 20;
    auto sign = imm >> 31 & 1;
    auto imm_10_5 = imm >> 4 & 0x3F;
    auto imm_4_1 = imm & 0xF;
    auto imm_11 = imm >> 10 & 0x1;
    return shift_and_or(forward_tuple(0, 7, 8, 12, 15, 20, 25, 31),
      forward_tuple(auto(x.opcode), imm_11, imm_4_1, auto(x.funct3), auto(x.rs1), auto(x.rs2), imm_10_5, sign));
  },
  [](same<format::u> auto &&x) {
    return shift_and_or(forward_tuple(0, 7, 12),
      forward_tuple(auto(x.opcode), auto(x.rd), auto(x.imm)));
  },
  [](same<format::j> auto &&x) {
    auto imm = static_cast<signed>(x.imm << 12) >> 12;
    auto sign = imm >> 31 & 1;
    auto imm_10_1 = imm & 0x3FF;
    auto imm_11 = (imm & 1 << 10) >> 10;
    auto imm_19_12 = imm >> 11 & 0xFF;
    return shift_and_or(forward_tuple(0, 7, 12, 20, 21, 31),
      forward_tuple(auto(x.opcode), auto(x.rd), imm_19_12, imm_11, imm_10_1, sign));
  },
};
}

constexpr auto encode(auto &&fwd) {
  return std::visit(details::_, (decltype(fwd))fwd);
}

using r = details::format::r;
using i = details::format::i;
using s = details::format::s;
using b = details::format::b;
using u = details::format::u;
using j = details::format::j;
using encoder_type = std::variant<r, i,
                                  s, b,
                                  u, j>;
}
