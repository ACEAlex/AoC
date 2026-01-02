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

bool is_paper(const Map &map, const Pos pos) {
  if (!is_inside(map, pos))
    return false;
  return map[pos.row][pos.col] == '@';
}

int adjecent(Map &map, const Pos pos) {
  int ret = 0;
  for (int i = -1; i <= 1; ++i) {
    if (is_paper(map, {pos.row - 1, pos.col + i}))
      ret++;
    if (is_paper(map, {pos.row + 1, pos.col + i}))
      ret++;
  }
  if (is_paper(map, {pos.row, pos.col - 1}))
    ret++;
  if (is_paper(map, {pos.row, pos.col + 1}))
    ret++;
  return ret;
}

int64_t part_1(Map &map) {
  int64_t ret = 0;

  bool cont = true;

  while (cont) {
    cont = false;

    for (int row = 0; row < map.size(); ++row) {
      for (int col = 0; col < map[row].size(); ++col) {
        if (is_paper(map, {row, col})) {
          auto adj = adjecent(map, {row, col});
          if (adj < 4) {
            ret++;
            map[row][col] = 'x';
            cont = true;
            // std::cout << 'x';
          }
          // else
          // std::cout << '@';
        }
        // else
        // std::cout << ' ';
      }
      // puts("");
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

  std::cout << "Score " << score << std::endl;

  return 0;
}