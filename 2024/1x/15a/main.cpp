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

struct Robot {
  XY p;
};

using Map = std::vector<std::string>;

bool is_inside(const Map &map, XY c) {
  if (c.x < 0 || c.y < 0)
    return false;
  if (c.y >= map.size())
    return false;
  if (c.x >= map[c.y].size())
    return false;
  return true;
}

Robot find_robot(Map &map) {
  Robot robot;
  for (int64_t row = 0; row < map.size(); ++row) {
    for (int64_t col = 0; col < map[row].size(); ++col) {
      if (map[row][col] == '@') {
        map[row][col] = '.';
        robot.p.x = col;
        robot.p.y = row;
        return robot;
      }
    }
  }
  assert(false);
  return robot;
}

bool try_move(Map &map, XY start, Dir dir) {
  auto pos_c = map[start.y][start.x];

  if (pos_c == '.')
    return true;

  if (pos_c == '#')
    return false;

  auto cur_pos = start + DirArray[dir];

  while (is_inside(map, cur_pos) && map[cur_pos.y][cur_pos.x] == 'O') {
    cur_pos += DirArray[dir];
  }

  if (map[cur_pos.y][cur_pos.x] == '.') {
    map[cur_pos.y][cur_pos.x] = 'O';
    map[start.y][start.x] = '.';
    return true;
  }

  return false;
}

void move_robot(Map &map, Robot &robot, char c) {
  Dir dir;
  XY dirdelta;
  switch (c) {
  case '>':
    dir = Dir::Right;
    break;
  case '<':
    dir = Dir::Left;
    break;
  case '^':
    dir = Dir::Up;
    break;
  case 'v':
    dir = Dir::Down;
    break;
  default:
    std::cout << "Wrong char: " << c << std::endl;
    assert(false);
  }

  auto to_move = robot.p + DirArray[dir];

  if (try_move(map, to_move, dir)) {
    robot.p += DirArray[dir];
  }
}

int64_t calc_score(const Map &map) {
  int64_t ret = 0;

  for (int64_t row = 0; row < map.size(); ++row) {
    for (int64_t col = 0; col < map[row].size(); ++col) {
      if (map[row][col] == 'O')
        ret += (row * 100 + col);
    }
  }

  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    XY a{1, 2};
    XY b{10, 11};
    auto c = a + b;
    assert(c.x == 11);
    assert(c.y == 13);
    a += b;
    assert(a.x == 11);
    assert(a.y == 13);
  }

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

  // Parse movement
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    for (const auto &c : line) {
      move_robot(map, robot, c);
    }
  }

  sum = calc_score(map);

  std::cout << "Score " << sum << std::endl;

  return 0;
}