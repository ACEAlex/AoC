#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
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

using Map = std::vector<std::string>;

struct Pos {
  int row;
  int col;
  bool operator<(const Pos &other) const {
    return std::tie(row, col) < std::tie(other.row, other.col);
  }
};

bool is_inside(const Map &map, const Pos pos) {
  if (pos.row < 0 or map.size() <= pos.row)
    return false;
  if (pos.col < 0 or map[pos.row].size() <= pos.col)
    return false;

  return true;
}

bool is_char(const Map &map, const Pos pos, char c) {
  if (!is_inside(map, pos))
    return false;
  return map[pos.row][pos.col] == c;
}

bool set_char(Map &map, const Pos pos, char c) {
  if (!is_inside(map, pos))
    return false;
  return map[pos.row][pos.col] = c;
}

Pos find_start(const Map &map) {
  Pos p{0, 0};

  for (p.col = 0; p.col < map[0].size(); ++p.col) {
    if (is_char(map, p, 'S'))
      return p;
  }
  assert(false);
}

void print_map(const Map &map) {
  for (const auto &row : map) {
    std::cout << row << std::endl;
  }
}

using Cache = std::map<Pos, int64_t>;

int64_t solve(Map &map, Pos p, Cache &cache) {
  // Check if we reach bottom = 1p
  if (map.size() - 1 == p.row) {
    return 1;
  }

  if (auto it = cache.find(p); it != cache.end()) {
    return it->second;
  }

  assert(is_char(map, p, '|'));

  const Pos down{p.row + 1, p.col};
  const Pos down_left{p.row + 1, p.col - 1};
  const Pos down_right{p.row + 1, p.col + 1};

  int64_t ret = 0;

  if (is_char(map, down, '.')) {
    set_char(map, down, '|');
    ret += solve(map, down, cache);
    set_char(map, down, '.');
  } else if (is_char(map, down, '^')) { // For '^'
    if (is_char(map, down_left, '.')) {
      set_char(map, down_left, '|');
      ret += solve(map, down_left, cache);
      set_char(map, down_left, '.');
    }
    if (is_char(map, down_right, '.')) {
      set_char(map, down_right, '|');
      ret += solve(map, down_right, cache);
      set_char(map, down_right, '.');
    }
  }
  cache.insert({p, ret});
  return ret;
}

int64_t part_2(Map &map) {
  auto start = find_start(map);
  set_char(map, {start.row + 1, start.col}, '|');
  // ret++;

  Cache cache;
  auto ret = solve(map, {start.row + 1, start.col}, cache);

  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  uint64_t score = 0;

  Map map;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    map.push_back(input);
  }

  score = part_2(map);

  print_map(map);

  std::cout << "Score " << score << std::endl;

  return 0;
}