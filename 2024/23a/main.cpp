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
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

using Arcs = std::set<std::string>;
using Computers = std::map<std::string, Arcs>;

std::pair<std::string, std::string> parse_pair(const std::string &str) {
  std::stringstream ss(str);
  std::pair<std::string, std::string> ret;
  std::getline(ss, ret.first, '-');
  std::getline(ss, ret.second, '-');
  return ret;
}

using Loop = std::set<std::string>;
using Loops = std::set<Loop>;

void has_loop(const std::string &start_c, const std::string &current, int depth,
              const Computers &computers, Loop current_loop, Loops &loops) {
  if (depth == 0) {
    if (current_loop.size() == 3)
      loops.insert(current_loop);
    return;
  }

  // Only allow to finish on target
  if (start_c == current) {
    return;
  }

  auto it = computers.find(current);
  assert(it != computers.end());

  current_loop.insert(current);

  for (const auto &arc : it->second) {
    has_loop(start_c, arc, depth - 1, computers, current_loop, loops);
  }
}

int64_t solution_1(const Computers &computers) {
  int64_t sum = 0;
  Loops loops;

  for (const auto &c : computers) {
    if (c.first.starts_with("t")) {
      std::cout << "Testing: " << c.first << std::endl;

      for (const auto &a1 : c.second) {
        auto it1 = computers.find(a1);
        for (const auto &a2 : it1->second) {
          auto it2 = computers.find(a2);
          for (const auto &a3 : it2->second) {
            if (a3 == c.first) {
              Loop loop;
              loop.insert(c.first);
              loop.insert(a1);
              loop.insert(a2);
              loops.insert(loop);
            }
          }
        }
      }
    }
  }
  return loops.size();
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    auto r = parse_pair("kh-tc");
    assert(r.first == "kh");
    assert(r.second == "tc");
  }

  // std::ifstream is("basic.input");
  std::ifstream is("basic.input");

  Computers computers;

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    auto pair = parse_pair(line);

    computers[pair.first].insert(pair.second);
    computers[pair.second].insert(pair.first);
  }

  sum = solution_1(computers);

  std::cout << "Total score " << sum << std::endl;

  return 0;
}