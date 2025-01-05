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

using Towels = std::vector<std::string>;

Towels parse_towels(const std::string &str) {
  std::stringstream ss(str);

  Towels ret;

  while (ss) {
    std::string current;
    std::getline(ss, current, ',');
    if (current.empty())
      break;
    auto ptr = current.c_str();
    if (*ptr == ' ')
      ptr++;
    ret.push_back(ptr);
  }

  return ret;
}

using VC = std::vector<bool>;

bool solution_1(const Towels &ts, const std::string &line, int i, VC &vc) {
  if (i >= line.size()) {
    std::cout << "It works! ";
    return true;
  }
  if (vc[i])
    return false;

  vc[i] = true;

  for (const auto &t : ts) {
    if (strncmp(t.c_str(), line.c_str() + i, t.size()) == 0) {
      if (solution_1(ts, line, i + t.size(), vc)) {
        return true;
      }
    }
  }

  return false;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    auto t = parse_towels("r, wr, b, g");
    assert(t.size() == 4);
    assert(t[0] == "r");
    assert(t[1] == "wr");
    assert(t[2] == "b");
    assert(t[3] == "g");
  }

  int64_t sum = 0;
  std::ifstream is("basic.input");
  Towels towels;

  {
    std::string line;
    std::getline(is, line);
    towels = parse_towels(line);
    std::getline(is, line);
  }

  // Parse quests
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    std::cout << "Testing " << line;
    VC visited(line.size(), false);
    if (solution_1(towels, line, 0, visited)) {
      sum++;
    }
    std::cout << std::endl;
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}