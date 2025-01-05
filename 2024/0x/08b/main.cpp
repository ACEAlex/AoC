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

using Row = std::string;
using Map = std::vector<Row>;

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

using Antennas = std::map<char, std::vector<Coord>>;

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

  // Scan for antennas
  Antennas antennas;
  for (int r = 0; r < map.size(); r++) {
    for (int c = 0; c < map.size(); c++) {
      auto current = map[r][c];
      if (current == '.')
        continue;

      // std::cout << "Found " << current << " at (" << c << "," << r << ")\n";
      antennas[current].push_back({c, r});
    }
  }

  std::set<Coord> antinodes;
  for (const auto &current : antennas) {
    for (auto it1 = current.second.begin(); it1 != current.second.end();
         ++it1) {
      for (auto it2 = current.second.begin(); it2 != current.second.end();
           ++it2) {
        if (it1 == it2)
          continue;

        // antinodes.insert({*it1, current.first});
        // antinodes.insert({*it2, current.first});
        antinodes.insert({*it1});
        antinodes.insert({*it2});

        Coord delta = {it1->first - it2->first, it1->second - it2->second};

        // Coord pos1 = {it1->first - delta.first, it1->second - delta.second};
        // Coord pos2 = {it2->first + delta.first, it2->second + delta.second};

        // Coord pos3 = {it1->first + delta.first, it1->second + delta.second};
        Coord pos4 = {it2->first - delta.first, it2->second - delta.second};

        // if (is_inside(map, pos1))
        //   antinodes.insert(pos1);

        // if (is_inside(map, pos2))
        //   antinodes.insert(pos2);
        while (is_inside(map, pos4)) {
          antinodes.insert({pos4});
          pos4 = {pos4.first - delta.first, pos4.second - delta.second};
        }

        // if (is_inside(map, pos3))
        //   antinodes.insert(pos3);

        // if (is_inside(map, pos4))
        //   antinodes.insert(pos4);
      }
    }
  }

  for (const auto &a : antinodes) {
    // std::cout << "(" << a.first << "," << a.second << ")";
    map[a.second][a.first] = '#';
  }

  for (const auto &row : map) {
    std::cout << row << std::endl;
  }

  // std::cout << "Score " << sum << std::endl;
  std::cout << "Score " << antinodes.size() << std::endl;

  return 0;
}