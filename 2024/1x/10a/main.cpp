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

using Map = std::vector<std::string>;
using Coord = std::pair<int, int>;

bool is_inside(const Map &map, Coord c) {
  auto x = c.first;
  auto y = c.second;
  if (x < 0 || y < 0)
    return false;
  if (y >= map.size())
    return false;
  if (x >= map[y].size())
    return false;
  return true;
}

char get_char(const Map &m, Coord c) {
  if (!is_inside(m, c)) {
    return 'o';
  }

  return m[c.second][c.first];
}

int64_t brute(Map &m, Coord c, int lvl) {
  // We scoored!
  if (lvl >= 9) {
    m[c.second][c.first] = '.';
    return 1;
  }

  int64_t ret = 0;
  lvl++;

  auto up = Coord(c.first, c.second - 1);
  auto down = Coord(c.first, c.second + 1);
  auto left = Coord(c.first - 1, c.second);
  auto right = Coord(c.first + 1, c.second);

  if (get_char(m, up) == '0' + lvl)
    ret += brute(m, up, lvl);
  if (get_char(m, down) == '0' + lvl)
    ret += brute(m, down, lvl);
  if (get_char(m, left) == '0' + lvl)
    ret += brute(m, left, lvl);
  if (get_char(m, right) == '0' + lvl)
    ret += brute(m, right, lvl);

  return ret;
}

int64_t brute_start(const Map &m) {
  int64_t res = 0;

  // Test left/right
  auto right_edge_col = m[0].size() - 1;
  auto bottom_edge_row = m.size() - 1;
  for (int r = 0; r <= bottom_edge_row; ++r) {
    for (int c = 0; c <= right_edge_col; ++c) {
      if (m[r][c] == '0') {
        auto m2 = m;
        res += brute(m2, {c, r}, 0);
      }
    }
  }
  return res;
}

void evolve(Map &m, int evolution) {}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  Map map;

  while (std::cin) {
    std::string line;
    std::getline(std::cin, line);
    if (line.empty())
      break;

    map.push_back(line);
  }

  sum = brute_start(map);

  std::cout << "Score " << sum << std::endl;

  return 0;
}