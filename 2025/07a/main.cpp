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

int64_t part_1(Map &map) {
  int64_t ret = 0;

  bool cont = true;

  auto start = find_start(map);
  set_char(map, {start.row + 1, start.col}, '|');
  // ret++;

  for (int row = 1; row < map.size() - 1; ++row) {
    for (int col = 0; col < map[row].size(); ++col) {
      if (is_char(map, {row, col}, '|')) { // For '|'
        if (is_char(map, {row + 1, col}, '.')) {
          set_char(map, {row + 1, col}, '|');
        } else if (is_char(map, {row + 1, col}, '^')) { // For '^'
          ret++;
          if (is_char(map, {row + 1, col - 1}, '.')) {
            set_char(map, {row + 1, col - 1}, '|');
          }
          if (is_char(map, {row + 1, col + 1}, '.')) {
            set_char(map, {row + 1, col + 1}, '|');
          }
        }
      }
    }
  }
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

  score = part_1(map);

  print_map(map);

  std::cout << "Score " << score << std::endl;

  return 0;
}