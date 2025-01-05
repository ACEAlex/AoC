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
        // map[row][col] = '.';
        robot.p.x = col;
        robot.p.y = row;
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

bool test_move_box(const Map &m, XY pos, Dir dir) {
  const auto new_pos1 = pos + DirArray[dir];
  const auto new_pos2 = pos + DirArray[dir] + DirArray[Dir::Right];
  // Any of the new pos is a rock --> Cannot move
  if (map2c(m, new_pos1) == '#' || map2c(m, new_pos2) == '#')
    return false;

  if (dir == Dir::Up || dir == Dir::Down) {
    if (map2c(m, new_pos1) == '.' && map2c(m, new_pos2) == '.')
      return true;

    if (map2c(m, new_pos1) == '[') {
      return test_move_box(m, new_pos1, dir);
    }
    if (map2c(m, new_pos1) == ']') {
      if (!test_move_box(m, new_pos1 + DirArray[Dir::Left], dir)) {
        return false;
      }
    }
    if (map2c(m, new_pos2) == '[') {
      return test_move_box(m, new_pos2, dir);
    }
  }

  else if (dir == Dir::Left) {
    if (map2c(m, new_pos1) == '.')
      return true;
    auto cc = map2c(m, new_pos1);
    assert(map2c(m, new_pos1) == ']');

    return test_move_box(m, new_pos1 + DirArray[Dir::Left], dir);
  }

  else // if (dir == Dir::Right)
  {
    if (map2c(m, new_pos2) == '.')
      return true;
    assert(map2c(m, new_pos2) == '[');

    return test_move_box(m, new_pos1 + DirArray[Dir::Right], dir);
  }

  return true;
}

void move_box_from_tail_left(Map &map, XY pos) {
  const auto new_pos1 = pos + DirArray[Dir::Left];
  const auto new_pos2 = pos;
  const auto old_pos2 = pos + DirArray[Dir::Right];

  if (map2c(map, new_pos1) == ']')
    move_box_from_tail_left(map, new_pos1 + DirArray[Dir::Left]);

  // Erase old box
  map[pos.y][pos.x] = '.';
  map[old_pos2.y][old_pos2.x] = '.';

  // Write new box pos
  map[new_pos1.y][new_pos1.x] = '[';
  map[new_pos2.y][new_pos2.x] = ']';
}
void move_box_from_tail_right(Map &map, XY pos) {
  const auto right_pos = pos + DirArray[Dir::Right] + DirArray[Dir::Right];
  const auto old_pos2 = pos + DirArray[Dir::Right];

  if (map2c(map, right_pos) == '[')
    move_box_from_tail_right(map, right_pos);

  // Erase old box
  map[pos.y][pos.x] = '.';
  map[old_pos2.y][old_pos2.x] = '.';

  // Write new box pos
  map[old_pos2.y][old_pos2.x] = '[';
  map[right_pos.y][right_pos.x] = ']';
}
void move_box_from_tail_up_down(Map &map, XY pos, Dir dir) {
  const auto new_pos1 = pos + DirArray[dir];
  const auto new_pos2 = pos + DirArray[dir] + DirArray[Dir::Right];
  const auto old_pos2 = pos + DirArray[Dir::Right];

  if (map2c(map, new_pos1) == '[')
    move_box_from_tail_up_down(map, new_pos1, dir);

  if (map2c(map, new_pos1) == ']')
    move_box_from_tail_up_down(map, new_pos1 + DirArray[Dir::Left], dir);

  if (map2c(map, new_pos2) == '[')
    move_box_from_tail_up_down(map, new_pos2, dir);

  // Erase old box
  map[pos.y][pos.x] = '.';
  map[old_pos2.y][old_pos2.x] = '.';

  // Write new box pos
  map[new_pos1.y][new_pos1.x] = '[';
  map[new_pos2.y][new_pos2.x] = ']';
}

void move_robot_left(Map &map, Robot &robot) {
  auto to_move = robot.p + DirArray[Dir::Left];

  if (map2c(map, to_move) == ']' &&
      test_move_box(map, to_move + DirArray[Dir::Left], Dir::Left)) {
    move_box_from_tail_left(map, to_move + DirArray[Dir::Left]);
  }

  if (map2c(map, to_move) == '.') {
    robot.p = to_move;
  }
}
void move_robot_right(Map &map, Robot &robot) {
  auto to_move = robot.p + DirArray[Dir::Right];

  if (map2c(map, to_move) == '[' && test_move_box(map, to_move, Dir::Right)) {
    move_box_from_tail_right(map, to_move);
  }

  if (map2c(map, to_move) == '.') {
    robot.p = to_move;
  }
}
void move_robot_up_down(Map &map, Robot &robot, Dir dir) {
  auto to_move = robot.p + DirArray[dir];

  if (map2c(map, to_move) == '[' && test_move_box(map, to_move, dir)) {
    move_box_from_tail_up_down(map, to_move, dir);
  }

  if (map2c(map, to_move) == ']' &&
      test_move_box(map, to_move + DirArray[Dir::Left], dir)) {
    move_box_from_tail_up_down(map, to_move + DirArray[Dir::Left], dir);
  }
  if (map2c(map, to_move) == '.') {
    robot.p = to_move;
  }
}

void move_robot(Map &map, Robot &robot, char c) {
  Dir dir;
  XY dirdelta;
  map[robot.p.y][robot.p.x] = '.';
  switch (c) {
  case '>':
    move_robot_right(map, robot);
    break;
  case '<':
    move_robot_left(map, robot);
    break;
  case '^':
    move_robot_up_down(map, robot, Dir::Up);
    break;
  case 'v':
    move_robot_up_down(map, robot, Dir::Down);
    break;
  default:
    std::cout << "Wrong char: " << c << std::endl;
    assert(false);
  }
  map[robot.p.y][robot.p.x] = '@';
}

int64_t calc_score(const Map &map) {
  int64_t ret = 0;

  for (int64_t row = 0; row < map.size(); ++row) {
    for (int64_t col = 0; col < map[row].size(); ++col) {
      if (map[row][col] == '[')
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
    std::string line2;
    for (const auto &c : line) {
      if (c == '#')
        line2.append("##");
      if (c == 'O')
        line2.append("[]");
      if (c == '@')
        line2.append("@.");
      if (c == '.')
        line2.append("..");
    }
    map.push_back(line2);
  }

  auto robot = find_robot(map);

  // Parse movement
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    for (const auto &c : line) {
      // for (const auto& row : map)
      // {
      //   std::cout << row << std::endl;
      // }
      move_robot(map, robot, c);
    }
  }

  for (const auto &row : map) {
    std::cout << row << std::endl;
  }

  sum = calc_score(map);

  std::cout << "Score " << sum << std::endl;

  return 0;
}