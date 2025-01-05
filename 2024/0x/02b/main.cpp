#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
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

using Level = std::vector<int32_t>;

Level parse_level(const std::string &input) {
  Level level;
  std::cout << "Parsing level: ";
  std::istringstream is(input);
  while (is.good()) {
    int32_t i;
    is >> i;
    std::cout << i << " ";
    level.push_back(i);
  }
  std::cout << ".done" << std::endl;
  return level;
}

enum class Dir {
  // UNKNOWN,
  Inc,
  Dec
};

bool check(const Level &l, std::size_t pos, int last, Dir dir, int reactors) {
  if (pos >= l.size()) {
    std::cout << "Safe" << std::endl;
    return true;
  }

  auto i = l[pos];
  constexpr auto delta = 3 + 1;

  if (last < i && i < last + delta && dir == Dir::Inc) {
    if (check(l, pos + 1, i, dir, reactors))
      return true;
  }
  if (last > i && i > last - delta && dir == Dir::Dec) {
    if (check(l, pos + 1, i, dir, reactors))
      return true;
  }
  if (reactors > 0) {
    reactors--;
    if (check(l, pos + 1, last, dir, reactors)) {
      return true;
    }
  }

  return false;
}

bool check_level(const Level &level) {
  if (check(level, 1, level[0], Dir::Inc, 1))
    return true;
  if (check(level, 1, level[0], Dir::Dec, 1))
    return true;
  if (check(level, 2, level[1], Dir::Inc, 0))
    return true;
  if (check(level, 2, level[1], Dir::Dec, 0))
    return true;
  std::cout << "Unsafe" << std::endl;
  return false;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    std::string foo("1 2 4 6");
    auto level = parse_level(foo);
    assert(level.size() == 4);
    assert(level[0] == 1);
    assert(level[1] == 2);
    assert(level[2] == 4);
    assert(level[3] == 6);
  }

  {
    Level l1{1, 2, 3, 4};
    assert(check_level(l1));
    Level l2{1, 3, 6, 7, 9};
    assert(check_level(l2));
    Level l3{9, 7, 6, 3, 1};
    assert(check_level(l3));
    Level l4{7, 6, 4, 2, 1};
    assert(check_level(l4));
    Level l5{99, 1, 2, 3, 4};
    assert(check_level(l5));
    Level l6{99, 1, 99, 2, 3, 4};
    assert(!check_level(l6));
    Level l7{1, 2, 3, 4, 99};
    assert(check_level(l7));
    Level l8{10, 11, 9, 8, 7};
    assert(check_level(l8));
  }

  int64_t sum = 0;

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    auto current = parse_level(input);
    if (check_level(current))
      sum++;
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}