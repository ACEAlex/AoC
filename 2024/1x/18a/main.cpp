#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
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

  int distance(const XY &other) const {
    return std::abs(other.x - x) + std::abs(other.y - y);
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
  // Dir dir;

  bool operator<(const QueueItem &other) const {
    // < = largest first
    // return cost > other.cost;
    return pos.distance(finish) > other.pos.distance(finish);
  }
  static XY finish;
};

XY QueueItem::finish = XY{0, 0};

// struct Robot {
//   XY p;
//   Dir dir;
// };

// struct Score {
//   int64_t
//   dir_score[4]{10000000000000000,10000000000000000,10000000000000000,10000000000000000};
// };

using Map = std::vector<std::vector<char>>;
// using VisitedMap = std::vector<std::vector<Score>>;
using VisitedMap = std::vector<std::vector<int64_t>>;

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

// Robot find_robot(Map& map)
// {
//   Robot robot;
//   for (int64_t row = 0; row < map.size(); ++row)
//   {
//     for (int64_t col = 0; col < map[row].size(); ++col)
//     {
//       if (map[row][col] == 'S')
//       {
//         map[row][col] = '.';
//         robot.p.x = col;
//         robot.p.y = row;
//         robot.dir = Dir::Right;
//         return robot;
//       }
//     }
//   }
//   assert(false);
//   return robot;
// }

char map2c(const Map &m, XY pos) {
  if (is_inside(m, pos)) {
    return m[pos.y][pos.x];
  }
  return '#';
}

int64_t solution_1(const Map &m) {
  int64_t best = std::numeric_limits<int64_t>::max();
  std::priority_queue<QueueItem> queue;
  VisitedMap visitMap(m.size(), std::vector<int64_t>(m[0].size(), best));

  const XY exit_pos{(int)m[0].size() - 1, (int)m.size() - 1};
  QueueItem::finish = exit_pos;
  {
    QueueItem start;
    start.pos = XY{0, 0};
    start.cost = 0;
    queue.push(start);
  }

  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();

    // Already visited
    auto &old = visitMap[current.pos.y][current.pos.x];

    // Old was better
    if (old < current.cost)
      continue;

    old = current.cost;

    if (current.pos == exit_pos) {
      if (best > current.cost) {
        std::cout << "Found the end with new best " << current.cost
                  << std::endl;
        best = current.cost;
        // for (int row = 0; row < m.size(); ++row)
        // {
        //   for (int col = 0; col < m[row].size(); ++col)
        //   {
        //     if (visitMap[row][col] <= best)
        //       m[row][col] = 'O';
        //   }
        // }
        // return best;
      }
    }

    // Try all the pos
    for (int i = 0; i < 4; ++i) {
      auto next = current;
      next.cost += 1;
      next.pos += DirArray[i];
      if (is_inside(m, next.pos) && is_walkable(m, next.pos) &&
          visitMap[next.pos.y][next.pos.x] > next.cost)
        queue.push(next);
    }
  }

  // Report best
  return best;
}

XY parse_xy(const std::string &str) {
  std::stringstream ss(str);

  std::string current;
  XY ret;

  std::getline(ss, current, ',');
  ret.x = atoi(current.c_str());
  std::getline(ss, current, ',');
  ret.y = atoi(current.c_str());
  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    auto xy = parse_xy("1,4");
    assert(xy.x == 1);
    assert(xy.y == 4);
  }

  int64_t sum = 0;

  std::ifstream is("basic.input");
  constexpr auto map_size = 71;
  constexpr auto sim_time = 1024;
  // std::ifstream is("abasic.input");
  // constexpr auto map_size = 7;
  // constexpr auto sim_time = 12;

  Map map(map_size, std::vector<char>(map_size, '.'));
  std::vector<XY> byte_vec;

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    auto xy = parse_xy(line);
    byte_vec.push_back(xy);
  }

  for (int i = 0; i < sim_time; i++) {
    const auto &xy = byte_vec[i];
    map[xy.y][xy.x] = '#';
  }

  sum = solution_1(map);

  for (const auto &row : map) {
    for (const auto &c : row) {
      std::cout << c;
    }
    std::cout << std::endl;
  }

  // sum = calc_score(map);

  std::cout << "Score " << sum << std::endl;

  return 0;
}