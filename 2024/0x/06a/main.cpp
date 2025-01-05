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

int find_start(const std::string &row) {
  for (int i = 0; i < row.size(); ++i) {
    if (row[i] == '^')
      return i;
  }
  return -1;
}

Coord find_start(const Map &m) {
  Coord c;

  for (c.second = 0; c.second < m.size(); ++c.second) {
    auto x = find_start(m[c.second]);
    if (x != -1) {
      c.first = x;
      break;
    }
  }

  return c;
}

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

void rotate_right(Coord &dir) {
  if (dir.first == 1 && dir.second == 0)
    dir = Coord(0, 1);
  else if (dir.first == 0 && dir.second == 1)
    dir = Coord(-1, 0);
  else if (dir.first == -1 && dir.second == 0)
    dir = Coord(0, -1);
  else if (dir.first == 0 && dir.second == -1)
    dir = Coord(1, 0);
}

void mark_used(Map &m, Coord c) { m[c.second][c.first] = 'x'; }

bool is_walkable(Map &m, Coord c) { return m[c.second][c.first] != '#'; }

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  Map map;

  {
    auto x1 = find_start(".....#.....");
    assert(x1 == -1);
    auto x2 = find_start(".....^.....");
    assert(x2 == 5);
  }

  {
    Map m;
    m.push_back("...");
    assert(is_inside(m, {0, 0}));
    assert(!is_inside(m, {-1, 0}));
    assert(!is_inside(m, {0, -1}));
    assert(is_inside(m, {2, 0}));
    assert(!is_inside(m, {3, 0}));
    assert(!is_inside(m, {0, 1}));
    m.push_back("^#.");
    auto start = find_start(m);
    assert(start.first == 0);
    assert(start.second == 1);
  }

  {
    Coord dir(0, -1);
    rotate_right(dir);
    assert(dir.first == 1);
    assert(dir.second == 0);
    rotate_right(dir);
    assert(dir.first == 0);
    assert(dir.second == 1);
    rotate_right(dir);
    assert(dir.first == -1);
    assert(dir.second == 0);
    rotate_right(dir);
    assert(dir.first == 0);
    assert(dir.second == -1);
  }

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;
    map.push_back(input);
  }

  Coord dir{0, -1};
  auto pos = find_start(map);
  mark_used(map, pos);
  while (true) {
    Coord next_pos{pos.first + dir.first, pos.second + dir.second};

    if (!is_inside(map, next_pos))
      break; // We have escaped

    if (is_walkable(map, next_pos)) {
      pos = next_pos;
      mark_used(map, pos);
    } else {
      rotate_right(dir);
    }
  }

  for (const auto &row : map) {
    sum += std::count_if(row.begin(), row.end(),
                         [](const auto &i) { return i == 'x'; });
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}