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

  bool anyInputIsX(int nbr) const {
    char tmp[12];
    sprintf(tmp, "x%02d", nbr);
    return (rin1 == tmp || rin2 == tmp);
  }
};

struct Computer {
  bool stable{false};
  std::map<RegName, RegValue> registers;

  void zero_input() {
    for (auto &i : registers) {
      if (i.first.starts_with("x") || i.first.starts_with("y")) {
        i.second = false;
      }
    }
  }

  void set_registers(int64_t x, int64_t y) {
    zero_input();
    std::bitset<45> xb(x);
    std::bitset<45> yb(y);

    for (int i = 0; i < 45; ++i) {
      char x_str[10];
      char y_str[10];
      sprintf(x_str, "x%02d", i);
      sprintf(y_str, "y%02d", i);

      registers[x_str] = xb[i];
      registers[y_str] = yb[i];
    }
  }
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

int64_t get_output(const Computer &comp) {
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

const char *Op_to_str(Op op) {
  switch (op) {
  case Op::AND:
    return "AND";
  case Op::OR:
    return "OR";
  case Op::XOR:
    return "XOR";
  }
  assert(false);
  return nullptr;
}

std::string indent(int depth) {
  std::string ret;
  while (depth--) {
    ret.append("  ");
  }
  return ret;
}

const Instr *find_out(const std::string &str, const Program &program) {
  for (const auto &instr : program.m_instructions) {
    if (instr.rout == str) {
      return &instr;
    }
  }
  return nullptr;
}

void rev_trace(const std::string &str, const Program &program, int depth = 0) {
  if (str.starts_with("x") || str.starts_with("y"))
    return;

  auto instr = find_out(str, program);

  std::cout << str << " depth=" << depth << std::endl;
  std::cout << indent(depth + 1) << instr->rin1 << std::endl;
  std::cout << indent(depth + 1) << Op_to_str(instr->op) << std::endl;
  std::cout << indent(depth + 1) << instr->rin2 << std::endl;
  rev_trace(instr->rin1, program, depth + 1);
  rev_trace(instr->rin2, program, depth + 1);
}

void analyze2(const Instr *or_instr, const Program &program, int depth) {
  if (depth == 2)
    return;
  auto a1 = find_out(or_instr->rin1, program);
  auto a2 = find_out(or_instr->rin2, program);

  const Instr *and_instr = nullptr;
  const Instr *xyand_instr = nullptr;

  if (a1->op == Op::AND && !a1->anyInputIsX(depth - 1)) {
    and_instr = a1;
    xyand_instr = a2;
  } else if (a2->op == Op::AND && !a2->anyInputIsX(depth - 1)) {
    xyand_instr = a1;
    and_instr = a2;
  } else
    assert(false);

  assert(xyand_instr->anyInputIsX(depth - 1));

  const Instr *next_or = nullptr;
  const Instr *xor_instr = nullptr;
  auto a3 = find_out(and_instr->rin1, program);
  auto a4 = find_out(and_instr->rin2, program);
  if (a3->op == Op::OR) {
    next_or = a3;
    xor_instr = a4;
  } else if (a4->op == Op::OR) {
    xor_instr = a3;
    next_or = a4;
  } else
    assert(false);

  if (xor_instr->op != Op::XOR)
    assert(false);

  assert(xor_instr->anyInputIsX(depth - 1));

  analyze2(next_or, program, depth - 1);
}

void analyze(const std::string &str, const Program &program, int depth) {
  auto instr = find_out(str, program);
  auto a1 = find_out(instr->rin1, program);
  auto a2 = find_out(instr->rin2, program);

  if (a1->op == Op::OR)
    analyze2(a1, program, depth);
  else if (a2->op == Op::OR)
    analyze2(a2, program, depth);
  else
    assert(false);
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

    // Patch z05
    if (line == "qjq XOR kbj -> frn")
      line = "qjq XOR kbj -> z05";
    if (line == "srp OR jcf -> z05")
      line = "srp OR jcf -> frn";

    // Patch z16
    if (line == "x16 AND y16 -> wnf")
      line = "x16 AND y16 -> vtj";
    if (line == "y16 XOR x16 -> vtj")
      line = "y16 XOR x16 -> wnf";

    // Patch z21
    if (line == "x21 AND y21 -> z21")
      line = "x21 AND y21 -> gmq";
    if (line == "mjj XOR fqr -> gmq")
      line = "mjj XOR fqr -> z21";

    // Patch z39
    if (line == "bkq AND jhv -> z39")
      line = "bkq AND jhv -> wtt";
    if (line == "jhv XOR bkq -> wtt")
      line = "jhv XOR bkq -> z39";

    // first instruction is "bkq AND jhv -> z39" // should be xor somethign
    // child of first should be "y39 XOR x39 -> bkq"
    // Then first instruction should be "jhv XOR bkq -> wtt"

    auto instr = parse_instr(line);
    program.m_instructions.push_back(instr);
  }

  // program.patch();

  for (int64_t i = 0; i < 45; i++) {
    int64_t x = 1;
    int64_t y = 0;
    x = x << i;
    y = y << i;
    program.complete = false;
    computer.set_registers(x, y);
    propagate_program(computer, program);
    sum = get_output(computer);
    if (sum != (x + y)) {
      std::cout << "Found an error sumbits " << std::bitset<45>(sum)
                << " x=" << x << " y=" << y << " sum=" << sum << " bit " << i
                << std::endl;
    }
  }

  // z00 + z01 =  ok
  const auto to_test = "z05";
  rev_trace(to_test, program);
  analyze(to_test, program, 5);

  // Ok
  // z05 start with an or instead of XOR --> Search for wrong X0R that has the
  // standard pattern of a working bit. There must be something simular to this.
  // Ok, so z05 should start with A XOR B where wither A or B is x05 XOR y05.
  // Just search for it and replace the OR ouput with its output

  // z16 standard is: first instruction is A xor B, where A: x16 XOR y16 B: ...
  // OR .... This instruction is wrong x16 AND y16 -> wnf And it may be this one
  // y16 XOR x16 -> vtj

  // z21 is just and of X21 and Y21
  // This instruction is wrong "x21 AND y21 -> z21"
  // --> Look for a instruction containing x21 XOR y21 it will lead you to the
  // parent Found "x21 XOR y21 -> fqr"
  // "mjj XOR fqr -> gmq" is a good parent since one dep is OR

  // z39 is wrong
  // first instruction is "bkq AND jhv -> z39" // should be xor somethign
  // child of first should be "y39 XOR x39 -> bkq"
  // Then first instruction should be "jhv XOR bkq -> wtt"

  // Right answer should be  frn,gmq,vtj,wnf,wtt,z05,z21,z39

  std::cout << "Total score " << sum << std::endl;

  return 0;
}