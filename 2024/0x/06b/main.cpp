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

using Coord = std::pair<int, int>;

struct Dirrer {
  char c{'.'};
  bool up{false};
  bool down{false};
  bool right{false};
  bool left{false};

  bool looking_for(Coord dir) {
    if (dir.first == 1 && right)
      return true;
    else if (dir.first == -1 && left)
      return true;
    else if (dir.second == 1 && down)
      return true;
    else if (dir.second == -1 && up)
      return true;
    return false;
  }

  void set_used(Coord dir) {
    if (dir.first == 1)
      right = true;
    else if (dir.first == -1)
      left = true;
    else if (dir.second == 1)
      down = true;
    else if (dir.second == -1)
      up = true;
  }
};

using Row = std::vector<Dirrer>;
using Map = std::vector<Row>;

int find_start(const Row &row) {
  for (int i = 0; i < row.size(); ++i) {
    if (row[i].c == '^')
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

char dir_to_char(Coord dir) {
  if (dir.first == 1)
    return 'r';
  else if (dir.second == 1)
    return 'd';
  else if (dir.first == -1)
    return 'l';
  else if (dir.second == -1)
    return 'u';
  assert(false);
  return 'q';
}

void mark_used(Map &m, Coord c, Coord dir) {
  m[c.second][c.first].set_used(dir);
}

void rotate_right(Coord &dir) {
  if (dir.first == 1) {
    dir = Coord(0, 1);
  } else if (dir.second == 1) {
    dir = Coord(-1, 0);
  } else if (dir.first == -1) {
    dir = Coord(0, -1);
  } else if (dir.second == -1) {
    dir = Coord(1, 0);
  }
}

bool is_walkable(Map &m, Coord c) { return m[c.second][c.first].c != '#'; }

bool is_old_path(Map &m, Coord c, Coord dir) {
  const auto &i = m[c.second][c.first];
  // if left or right
  if (dir.first == 1 && i.right) {
    return true;
  }
  if (dir.first == -1 && i.left) {
    return true;
  } else if (dir.second == 1 && i.down) {
    return true;
  } else if (dir.second == -1 && i.up) {
    return true;
  }
  return false;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  Map orig_map;

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    Row r;
    for (const auto &c : input) {
      Dirrer d;
      d.c = c;
      r.push_back(d);
    }
    orig_map.push_back(r);
  }
  const auto start_pos = find_start(orig_map);

  for (int r = 0; r < orig_map.size(); ++r) {
    for (int c = 0; c < orig_map[r].size(); ++c) {
      // Loop whole map for every empty space
      if (orig_map[r][c].c == '#' || orig_map[r][c].c == '^')
        continue;
      // Create a copy of map and inject obsticle
      auto map = orig_map;
      map[r][c].c = '#';

      Coord dir{0, -1};
      auto pos = start_pos;
      mark_used(map, pos, dir);
      while (true) {
        Coord next_pos{pos.first + dir.first, pos.second + dir.second};

        if (!is_inside(map, next_pos))
          break; // We have escaped

        if (is_walkable(map, next_pos)) {
          if (is_old_path(map, next_pos, dir)) {
            sum++;
            break;
          }
          pos = next_pos;
          mark_used(map, pos, dir);
        } else {
          rotate_right(dir);
          mark_used(map, pos, dir);
        }
      }
    }
  }

  std::cout << "Score " << sum << std::endl;
  return 0;
}