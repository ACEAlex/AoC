#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

// using Instr = std::pair<int,int>;
using Program = std::vector<int64_t>;

struct Computer {
  int64_t A{0};
  int64_t B{0};
  int64_t C{0};

  int64_t get_combo(int64_t combo) const {
    switch (combo) {
    case 0:
    case 1:
    case 2:
    case 3:
      return combo;
    case 4:
      return A;
    case 5:
      return B;
    case 6:
      return C;
    }
    assert(false);
  }

  bool execute(const Program &program, int64_t foo) {
    int i = 0;

    for (int64_t pc = 0; pc < program.size();) {

      const auto &opcode = program[pc];
      const auto &lit = program[pc + 1];

      if (opcode == 0) { // adv
        int64_t num = A;
        int64_t dom = std::pow(2, get_combo(lit));
        A = num / dom;
      } else if (opcode == 1) { // bxl
        B = B ^ lit;
      } else if (opcode == 2) { // bst
        B = get_combo(lit) % 8;
      } else if (opcode == 3) { // jnz
        if (A != 0) {
          auto old_pc = pc;
          pc = lit;
          if (old_pc != pc)
            continue;
        }
      } else if (opcode == 4) { // bxc
        B = B ^ C;
      } else if (opcode == 5) { // out
        auto v = (get_combo(lit) % 8);
        if (program[i] != v)
          return false;

        // if (i == 12)
        //   std::cout << "i=" << i << " for A=" << foo << std::endl;
        i++;

      } else if (opcode == 6) { // bdv
        int64_t num = A;
        int64_t dom = std::pow(2, get_combo(lit));
        B = num / dom;
      } else if (opcode == 7) { // cdv
        int64_t num = A;
        int64_t dom = std::pow(2, get_combo(lit));
        C = num / dom;
      } else {
        assert(false);
      }
      pc = pc + 2;
    }
    return i == program.size();
  }
};

int64_t parse_register(const std::string &str) {
  int64_t ret = 0;

  auto r = sscanf(str.c_str(), "Register %*c: %ld", &ret);
  assert(r == 1);
  return ret;
}

Program parse_program(const std::string &str) {
  Program program;
  std::stringstream ss(str.c_str() + 9);
  while (ss) {
    std::string i_str;
    int instr;
    std::getline(ss, i_str, ',');
    if (i_str.empty())
      break;
    instr = atoi(i_str.c_str());
    program.push_back(instr);
  }

  return program;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;
#if 0
  {
    auto r = parse_register("Register A: 729");
    assert(r == 729);
    r = parse_register("Register B: 0");
    assert(r == 0);
  }
  {
    auto p = parse_program("Program: 0,1,5,4,3,0");
    assert(p.size() == 6);
    assert(p[0] == 0 && p[1] == 1);
    assert(p[2] == 5 && p[3] == 4);
    assert(p[4] == 3 && p[5] == 0);
  }
  {
    Computer c {0,0,9};
    Program p {2,6};
    c.execute(p);
    assert(c.B == 1);
  }
  {
    Computer c {10,0,0};
    Program p {5,0,5,1,5,4};
    auto out = c.execute(p);
    assert(out == "0,1,2");
  }
  {
    Computer c {2024,0,0};
    Program p {0,1,5,4,3,0};
    auto out = c.execute(p);
    assert(c.A == 0);
    assert(out == "4,2,5,6,7,7,7,7,3,1,0");
  }
  {
    Computer c {0,29,0};
    Program p {1,7};
    auto out = c.execute(p);
    assert(c.B == 26);
  }
  {
    Computer c {0,2024,43690};
    Program p {4,0};
    auto out = c.execute(p);
    assert(c.B == 44354);
  }
#endif
  int64_t sum = 0;

  std::ifstream is("basic.input");

  Computer computer;
  Program program;

  // Parse map
  // while (std::cin)
  {
    std::string line;
    std::getline(is, line);
    computer.A = parse_register(line);
    std::getline(is, line);
    computer.B = parse_register(line);
    std::getline(is, line);
    computer.C = parse_register(line);

    // Blank
    std::getline(is, line);
    // Program
    std::getline(is, line);

    program = parse_program(line);
  }

  int64_t loop_start6 = 106914495;

  std::vector<int64_t> loop_incr6{
      33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 28441518,
      82,       2097070,  82,       2097070,  82,       130990,   82,
      787456,   1178542,  82,       2097070,  82,       2097070,  82,
      2097070,  82,       130990,   82,       1965998,  82,       2097070,
      82,       2097070,  82,       2097070,  82,       130990,   82,
      1965998,  82,       2097070,  82,       2097070,  82,       2097070,
      82,       65454,    82,       65454,    82,       1965998,  82,
      65454,    82,       7144448};

  int64_t loop_start12 = 2343249731181;
  std::vector<int64_t> loop_incr12{82, 6291374, 82, 1099505336238

  };

  computer.A = loop_start12;

  while (true) {
    for (int i = 0; i < loop_incr12.size(); ++i) {
      computer.A += loop_incr12[i];
      auto c = computer;
      // c.A = i;
      if (c.execute(program, computer.A)) {
        std::cout << "Found a solution: " << computer.A << std::endl;
        exit(0);
      }
    }
  }

  return 0;
}