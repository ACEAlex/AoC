#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

struct Pos {
  int64_t x{0};
  int64_t y{0};
  int64_t comp_x{0};
  int64_t comp_y{0};
  bool operator<(const Pos &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

using Positions = std::vector<std::unique_ptr<Pos>>;
using Lines = std::vector<std::pair<Pos *, Pos *>>;

// [n] represents int number of spaces in uncompressed world
using CompressedAxis = std::vector<int>;

using Map = std::vector<std::vector<char>>;

struct Rectangle {
  Rectangle(const Pos *a, const Pos *b) {
    m_a = a;
    m_b = b;
    area = (1 + std::abs(m_a->x - m_b->x)) * (1 + std::abs(m_a->y - m_b->y));
  }

  bool operator<(const Rectangle &other) const { return other.area < area; }

  const Pos *m_a;
  const Pos *m_b;
  int64_t area;
};

std::ostream &operator<<(std::ostream &os, const Pos &p) {
  os << p.x << "," << p.y;
  return os;
}

std::unique_ptr<Pos> parse_pos(const std::string &str) {
  std::vector<std::string> v;
  boost::split(v, str, boost::is_any_of(","), boost::token_compress_on);
  return std::make_unique<Pos>(atoi(v[0].c_str()), atoi(v[1].c_str()));
}

bool pos_inside(int64_t row, int64_t col, const Map &compressed_map) {
  return compressed_map[row][col] != '.';
}

bool all_inside(const Rectangle &r, const Positions &positions,
                const Map &compressed_map) {
  auto min_x = std::min(r.m_a->comp_x, r.m_b->comp_x);
  auto max_x = std::max(r.m_a->comp_x, r.m_b->comp_x);
  auto min_y = std::min(r.m_a->comp_y, r.m_b->comp_y);
  auto max_y = std::max(r.m_a->comp_y, r.m_b->comp_y);

  for (int64_t row = min_y; row <= max_y; ++row) {
    for (int64_t col = min_x; col <= max_x; ++col) {
      if (!pos_inside(row, col, compressed_map))
        return false;
    }
  }

  return true;
}

void flood(Map &compressed_map, int64_t row, int64_t col) {
  // Check inside
  if (row < 0 or row >= compressed_map.size() or col < 0 or
      col >= compressed_map[row].size())
    return;

  if (compressed_map[row][col] == '.') {
    compressed_map[row][col] = '0';
    flood(compressed_map, row + 1, col);
    flood(compressed_map, row - 1, col);
    flood(compressed_map, row, col + 1);
    flood(compressed_map, row, col - 1);
  }
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t score = 0;

  {
    auto p = parse_pos("123,456");
    assert(p->x == 123);
    assert(p->y == 456);
  }

  Positions positions; // Heap = memory stable
  std::multiset<Rectangle> rectangles;
  Lines lines;

  Pos *last_pos = nullptr;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    auto pos = parse_pos(input);

    // Make it a pair of every already read postions
    for (const auto &i : positions) {
      Rectangle r(i.get(), pos.get());
      rectangles.insert(r);
    }
    positions.push_back(std::move(pos));

    if (last_pos) {
      lines.emplace_back(last_pos, positions.back().get());
    }

    last_pos = positions.back().get();
  }

  lines.emplace_back(positions.back().get(), positions.front().get());

  // Compress coordinates so they fit in memory
  Map compressed_map;
  CompressedAxis comp_x, comp_y;

  // Start with x. Sort list accoring to x so we can build the compressed map
  // according to it
  std::sort(positions.begin(), positions.end(),
            [](const auto &a, const auto &b) { return a->x < b->x; });
  {
    int64_t last_x = -1;
    for (const auto &pos : positions) {
      if (pos->x == last_x) {
        pos->comp_x = comp_x.size() - 1;
        continue;
      }
      auto delta = (pos->x - last_x) - 1;
      if (delta != 0) {
        comp_x.push_back(delta);
      }
      pos->comp_x = comp_x.size();
      comp_x.push_back(1);

      last_x = pos->x;
    }
  }
  // Do the same with y
  std::sort(positions.begin(), positions.end(),
            [](const auto &a, const auto &b) { return a->y < b->y; });
  {
    int64_t last_y = -1;
    for (const auto &pos : positions) {
      if (pos->y == last_y) {
        pos->comp_y = comp_y.size() - 1;
        continue;
      }
      auto delta = (pos->y - last_y) - 1;
      if (delta != 0) {
        comp_y.push_back(delta);
      }
      pos->comp_y = comp_y.size();
      comp_y.push_back(1);

      last_y = pos->y;
    }
  }

  // Build the compressed map
  // Start by layout a matrix of '.'
  for (int i = 0; i < comp_y.size(); ++i) {
    compressed_map.emplace_back(comp_x.size(), '.');
  }
  // Add corners and draw lines
  {
    for (const auto &line : lines) {
      compressed_map[line.first->comp_y][line.first->comp_x] = '#';
      compressed_map[line.second->comp_y][line.second->comp_x] = '#';
      // Check horizontal
      if (line.first->comp_y == line.second->comp_y) {
        auto start = std::min(line.first->comp_x, line.second->comp_x) + 1;
        auto stop = std::max(line.first->comp_x, line.second->comp_x);
        while (start < stop) {
          compressed_map[line.first->comp_y][start] = '+';
          start++;
        }
      } else {
        auto start = std::min(line.first->comp_y, line.second->comp_y) + 1;
        auto stop = std::max(line.first->comp_y, line.second->comp_y);
        while (start < stop) {
          compressed_map[start][line.first->comp_x] = '+';
          start++;
        }
      }
    }
  }

  // Flood fil
  {
    // Start by finding suitable position
    // it is to find the first corner on row[] and then do x++ y++
    int row = 1;
    int col = 1;

    while (true) {
      if (compressed_map[row][col] == '#') {
        row++;
        col++;
        break;
      }
      col++;
    }
    std::cout << "Found a good start pos for filling: " << col << " " << row
              << std::endl;
    flood(compressed_map, row, col);
  }

  for (const auto &r : rectangles) {
    if (all_inside(r, positions, compressed_map)) {
      score = r.area;
      break;
    }
  }

  for (int row = 0; row < compressed_map.size(); row++) {
    for (int col = 0; col < compressed_map[row].size(); col++) {
      std::cout << compressed_map[row][col];
    }
    std::cout << "\n";
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}