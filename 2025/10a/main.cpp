#include "boost/algorithm/string/split.hpp"
#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
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

using Lights = uint64_t;
using Schematic = uint64_t;
using Schematics = std::vector<Schematic>;

Lights parse_lights(const std::string &str) {
  Lights ret = 0;
  assert(str[0] == '[');
  for (int i = 0; str[i] != '\0'; ++i) {
    if (str[i] == '#')
      ret += 1 << (i - 1);
  }

  return ret;
}

Schematics parse_schematics(const std::string &str) {
  auto pos1 = str.find_first_of('(');
  auto pos2 = str.find_last_of(')');
  auto schematics_str = str.substr(pos1, pos2 - pos1 + 1);
  std::vector<std::string> v;
  boost::split(v, schematics_str, boost::is_any_of(" "),
               boost::token_compress_on);

  Schematics ret;
  for (const auto &i : v) {
    Schematic s{0};
    auto trim = i.substr(1, i.size() - 2);
    // std::cout << "Trim: " << trim << std::endl;
    std::vector<std::string> nums;
    boost::split(nums, trim, boost::is_any_of(","), boost::token_compress_on);
    for (const auto &n : nums) {
      auto value = std::atoi(n.c_str());
      s += 1 << value;
    }
    ret.push_back(s);
  }

  return ret;
}

struct QueueItem {
  int64_t cost{0};
  Lights lights{0};

  bool operator<(const QueueItem &other) const {
    // < = largest first
    return cost > other.cost;
  }
};

int64_t part1(const Lights &target, const Schematics &schemantics) {

  // BFS
  std::priority_queue<QueueItem> queue;
  std::set<Lights> visited;
  queue.push({0, 0});
  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();

    if (visited.find(current.lights) != visited.end()) {
      continue;
    }
    visited.insert(current.lights);

    if (current.lights == target) {
      std::cout << "Found a solution: " << current.cost << std::endl;
      return current.cost;
    }

    // Try all buttons from here
    for (const auto &s : schemantics) {
      queue.push({current.cost + 1, current.lights ^ s});
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t score = 0;

  {
    auto str = "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}";
    auto l = parse_lights(str);
    // std::cout << "l = " << l << std::endl;
    assert(l == 6);
    auto s = parse_schematics(str);
    // std::cout << "s[0] = " << s[0] << std::endl;
    assert(s[0] == 8);
    assert(s[1] == 10);
  }
  {
    auto str =
        "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}";
    auto l = parse_lights(str);
    // std::cout << "l = " << l << std::endl;
    assert(l == 46);
    auto s = parse_schematics(str);
    // std::cout << "s[0] = " << s[0] << std::endl;
    assert(s[0] == 31);
    assert(s[1] == 25);
  }

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    auto target = parse_lights(input);
    auto schematics = parse_schematics(input);

    score += part1(target, schematics);
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}