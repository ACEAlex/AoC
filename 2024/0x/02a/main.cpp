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

enum class Dir { UNKNOWN, Inc, Dec };

bool check_level(const Level &level) {
  auto it = level.begin();
  auto last = *it;

  Dir dir = Dir::UNKNOWN;

  while (++it != level.end()) {
    auto i = *it;
    constexpr auto delta = 3 + 1;
    if (last < i && i < last + delta && dir != Dir::Dec) {
      last = i;
      dir = Dir::Inc;
      continue;
    }
    if (last > i && i > last - delta && dir != Dir::Inc) {
      last = i;
      dir = Dir::Dec;
      continue;
    }
    std::cout << "Unsafe" << std::endl;
    return false;
  }

  std::cout << "Safe" << std::endl;
  return true;
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
  }

  int64_t sum = 0;
  std::vector<Level> levels;

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    auto current = parse_level(input);
    levels.push_back(current);
  }

  for (const auto &l : levels) {
    if (check_level(l))
      sum++;
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}