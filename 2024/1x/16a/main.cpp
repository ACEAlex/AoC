#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <limits>
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

struct XY {
  int64_t x{0};
  int64_t y{0};

  bool operator==(const XY &other) const {
    return x == other.x && y == other.y;
  }

  XY &operator+=(const XY &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  XY operator+(const XY &other) const {
    XY ret = *this;
    ret.x += other.x;
    ret.y += other.y;
    return ret;
  }

  // XY operator-(const XY& other) const
  // {
  //   XY ret = *this;
  //   ret.x -= other.x;
  //   ret.y -= other.y;
  //   return ret;
  // }

  void verify(int64_t _x, int64_t _y) const {
    if (x != _x || y != _y) {
      std::cout << "XY verification failed:" << std::endl;
      std::cout << "  Is : " << x << " " << y << std::endl;
      std::cout << "  Cmp: " << _x << " " << _y << std::endl;
      assert(false);
    }
  }

  bool operator<(const XY &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

std::ostream &operator<<(std::ostream &os, const XY &c) {
  os << "(" << c.x << ',' << c.y << ")";
  return os;
}

enum Dir { Up, Right, Down, Left };

static const XY DirArray[4]{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct QueueItem {
  int64_t cost{0};
  XY pos;
  Dir dir;

  bool operator<(const QueueItem &other) const {
    // < = largest first
    return cost > other.cost;
  }
};

struct Robot {
  XY p;
  Dir dir;
};

struct Score {
  int64_t dir_score[4]{10000000000000000, 10000000000000000, 10000000000000000,
                       10000000000000000};
};

using Map = std::vector<std::string>;
using VisitedMap = std::vector<std::vector<Score>>;

bool is_inside(const Map &map, XY c) {
  if (c.x < 0 || c.y < 0)
    return false;
  if (c.y >= map.size())
    return false;
  if (c.x >= map[c.y].size())
    return false;
  return true;
}
bool is_walkable(const Map &map, XY c) {
  if (c.x < 0 || c.y < 0)
    return false;
  if (c.y >= map.size())
    return false;
  if (c.x >= map[c.y].size())
    return false;
  return map[c.y][c.x] != '#';
}

Robot find_robot(Map &map) {
  Robot robot;
  for (int64_t row = 0; row < map.size(); ++row) {
    for (int64_t col = 0; col < map[row].size(); ++col) {
      if (map[row][col] == 'S') {
        map[row][col] = '.';
        robot.p.x = col;
        robot.p.y = row;
        robot.dir = Dir::Right;
        return robot;
      }
    }
  }
  assert(false);
  return robot;
}

char map2c(const Map &m, XY pos) {
  if (is_inside(m, pos)) {
    return m[pos.y][pos.x];
  }
  return '#';
}

void solution_1(const Map &m, const Robot &r, int64_t &sum) {
  int64_t best = std::numeric_limits<int64_t>::max();
  std::priority_queue<QueueItem> queue;
  VisitedMap visitMap(m.size(), std::vector<Score>(m[0].size()));

  {
    QueueItem start;
    start.pos = r.p;
    start.dir = r.dir;
    start.cost = 0;
    // visitMap[r.p.y][r.p.x].dir_score[r.dir] = 0;
    queue.push(start);
  }

  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();

    // Already visited
    auto &old = visitMap[current.pos.y][current.pos.x];

    // Old was better
    if (old.dir_score[current.dir] < current.cost)
      continue;

    old.dir_score[current.dir] = current.cost;

    if (map2c(m, current.pos) == 'E') {
      if (best > current.cost) {
        std::cout << "Found the end with new best " << current.cost
                  << std::endl;
        best = current.cost;
      }
    }

    // Try all the pos
    for (int i = 0; i < 4; ++i) {
      if (i == current.dir) // Move forward
      {
        auto next = current;
        next.pos += DirArray[current.dir];
        next.cost += 1;
        if (is_walkable(m, next.pos)) {
          queue.push(next);
        }
      } else {
        auto next = current;
        next.dir = (Dir)i;
        next.cost += 1000;
        queue.push(next);
      }
    }
  }

  // Report best
  sum = best;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  std::ifstream is("basic.input");

  Map map;

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    map.push_back(line);
  }

  auto robot = find_robot(map);

  solution_1(map, robot, sum);

  // for (const auto &row : map) {
  //   std::cout << row << std::endl;
  // }

  // sum = calc_score(map);

  std::cout << "Score " << sum << std::endl;

  return 0;
}