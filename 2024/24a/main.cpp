#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

using RegName = std::string;
using RegValue = bool;
using RegPair = std::pair<RegName, RegValue>;

enum class Op { AND, XOR, OR };

struct Instr {
  RegName rin1;
  RegName rin2;
  Op op;
  RegName rout;
};

struct Computer {
  bool stable{false};
  std::map<RegName, RegValue> registers;
};

struct Program {
  bool complete{false};
  std::vector<Instr> m_instructions;
};

RegPair parse_initial_register(const std::string &input) {
  RegPair ret;

  ret.first = input.substr(0, 3);
  if (input[5] == '1')
    ret.second = true;
  else
    ret.second = false;

  return ret;
}

Instr parse_instr(const std::string &input) {
  Instr ret;

  ret.rin1 = input.substr(0, 3);

  int offset = 0;
  auto inst = input.substr(4, 3);

  if (inst == "AND") {
    ret.op = Op::AND;
  } else if (inst == "XOR") {
    ret.op = Op::XOR;
  } else if (inst == "OR ") {
    ret.op = Op::OR;
    offset = -1;
  } else {
    assert(false);
  }

  ret.rin2 = input.substr(8 + offset, 3);
  ret.rout = input.substr(15 + offset, 3);

  return ret;
}

void propagate_program(Computer &computer, Program &program) {

  while (!program.complete) {
    program.complete = true;
    for (const auto &i : program.m_instructions) {
      auto &rin1 = computer.registers[i.rin1];
      auto &rin2 = computer.registers[i.rin2];
      auto &rout = computer.registers[i.rout];
      auto rout_old = rout;
      switch (i.op) {
      case Op::AND:
        rout = rin1 & rin2;
        break;
      case Op::OR:
        rout = rin1 | rin2;
        break;
      case Op::XOR:
        rout = rin1 ^ rin2;
        break;
      default:
        assert(false);
      }
      if (rout_old != rout) {
        program.complete = false;
      }
    }
  }
}

int64_t solution_1(const Computer &comp) {
  std::stringstream ss;

  for (auto it = comp.registers.rbegin(); it != comp.registers.rend(); ++it) {
    if (it->first.starts_with("z")) {
      if (it->second)
        ss << "1";
      else
        ss << "0";
    }
  }
  auto str = ss.str();
  auto i = stol(str, nullptr, 2);
  return i;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    auto reg = parse_initial_register("x02: 1");
    assert(reg.first == "x02");
    assert(reg.second == true);
  }

  {
    auto reg = parse_initial_register("x04: 0");
    assert(reg.first == "x04");
    assert(reg.second == false);
  }
  {
    auto inst = parse_instr("x00 AND y00 -> z00");
    assert(inst.rin1 == "x00");
    assert(inst.op == Op::AND);
    assert(inst.rin2 == "y00");
    assert(inst.rout == "z00");
  }
  {
    auto inst = parse_instr("x01 XOR y01 -> z01");
    assert(inst.rin1 == "x01");
    assert(inst.op == Op::XOR);
    assert(inst.rin2 == "y01");
    assert(inst.rout == "z01");
  }
  {
    auto inst = parse_instr("x02 OR y02 -> z02");
    assert(inst.rin1 == "x02");
    assert(inst.op == Op::OR);
    assert(inst.rin2 == "y02");
    assert(inst.rout == "z02");
  }

  // std::ifstream is("basic.input");
  std::ifstream is("basic.input");

  Computer computer;
  Program program;

  // Parse parse init registers
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;
    auto reg = parse_initial_register(line);
    computer.registers.insert({reg.first, reg.second});
  }

  // Parse logic connections
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;
    auto instr = parse_instr(line);
    program.m_instructions.push_back(instr);
  }

  propagate_program(computer, program);

  sum = solution_1(computer);

  std::cout << "Total score " << sum << std::endl;

  return 0;
}