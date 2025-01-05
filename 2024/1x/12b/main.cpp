#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
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

struct Score {
  int area;
  int fences;
};

using Map = std::vector<std::string>;
using RegionMap = std::vector<std::vector<int>>;
using RegionScore = std::map<int, Score>;

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

bool is_inside(const RegionMap &map, Coord c) {
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

void area_fill(const Map &m, RegionMap &rm, RegionScore &rs, char c_id, int id,
               Coord c) {
  if (!is_inside(m, c) || rm[c.second][c.first] != -1)
    return;

  if (m[c.second][c.first] != c_id)
    return;

  rm[c.second][c.first] = id;
  rs[id].area++;
  area_fill(m, rm, rs, c_id, id, {c.first + 1, c.second});
  area_fill(m, rm, rs, c_id, id, {c.first - 1, c.second});
  area_fill(m, rm, rs, c_id, id, {c.first, c.second + 1});
  area_fill(m, rm, rs, c_id, id, {c.first, c.second - 1});
}

void populate_region_map(const Map &m, RegionMap &rm, RegionScore &rs) {
  int next_id = 0;
  for (int r = 0; r < m.size(); ++r) {
    for (int c = 0; c < m[r].size(); ++c) {
      if (rm[r][c] == -1) {
        next_id++;
        area_fill(m, rm, rs, m[r][c], next_id, {c, r});
      }
    }
  }
}

int get_region(const RegionMap &rm, Coord c) {
  if (is_inside(rm, c))
    return rm[c.second][c.first];

  return -1;
}

void calc_fences(const RegionMap &rm, RegionScore &rs) {
  for (int r = 0; r < rm.size(); ++r) {
    bool fence_top{false};
    bool fence_botom{false};
    int last_region = -1;
    for (int c = 0; c < rm[r].size(); ++c) {
      auto current_region = get_region(rm, {c, r});
      if (last_region != current_region) {
        last_region = current_region;
        fence_top = false;
        fence_botom = false;
      }
      if (get_region(rm, {c, r + 1}) != current_region) {
        if (!fence_botom)
          rs[rm[r][c]].fences++;
        fence_botom = true;
      } else {
        fence_botom = false;
      }
      if (get_region(rm, {c, r - 1}) != current_region) {
        if (!fence_top)
          rs[rm[r][c]].fences++;
        fence_top = true;
      } else {
        fence_top = false;
      }
    }
  }

  for (int c = 0; c < rm[0].size(); ++c) {
    bool fence_left{false};
    bool fence_right{false};
    int last_region = -1;
    for (int r = 0; r < rm.size(); ++r) {
      auto current_region = get_region(rm, {c, r});
      if (last_region != current_region) {
        last_region = current_region;
        fence_left = false;
        fence_right = false;
      }
      if (get_region(rm, {c - 1, r}) != current_region) {
        if (!fence_left)
          rs[rm[r][c]].fences++;
        fence_left = true;
      } else {
        fence_left = false;
      }
      if (get_region(rm, {c + 1, r}) != current_region) {
        if (!fence_right)
          rs[rm[r][c]].fences++;
        fence_right = true;
      } else {
        fence_right = false;
      }
    }
  }
}

int64_t calc_score(const RegionScore &rs) {
  int64_t ret = 0;
  for (const auto &s : rs) {
    ret += s.second.area * s.second.fences;
  }
  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  std::ifstream is("basic.input");

  Map map;
  RegionMap rmap;
  RegionScore rscore;

  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    map.push_back(line);
    rmap.push_back(std::vector<int>(line.size(), -1));
  }

  populate_region_map(map, rmap, rscore);

  calc_fences(rmap, rscore);

  sum = calc_score(rscore);

  std::cout << "Score " << sum << std::endl;

  return 0;
}