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

struct Rules {
  int64_t width;
  int64_t height;
  int64_t seconds;
};

struct Robot {
  XY p;
  XY v;

  XY move(const Rules &r) const {
    XY ret;

    ret.x = (p.x + r.seconds * v.x) % r.width;
    ret.y = (p.y + r.seconds * v.y) % r.height;

    if (ret.x < 0)
      ret.x += r.width;

    if (ret.y < 0)
      ret.y += r.height;

    return ret;
  }
};

using Robots = std::vector<Robot>;

Robot parse_robot(const std::string &str) {
  Robot ret;

  auto r = sscanf(str.c_str(), "p=%ld,%ld v=%ld,%ld", &ret.p.x, &ret.p.y,
                  &ret.v.x, &ret.v.y);
  assert(r == 4);

  return ret;
}

int calc_quad(int64_t y, int64_t height) {
  if (y < (height / 2))
    return 0;
  if (y == (height / 2))
    return -1;
  return 1;
}

int64_t solution_1(const Rules &rules, const Robots &robots) {
  int64_t sum = 0;
  int64_t quad[2][2] = {0, 0, 0, 0};

  for (const auto r : robots) {
    auto pos = r.move(rules);
    auto row = calc_quad(pos.y, rules.height);
    auto col = calc_quad(pos.x, rules.width);
    if (row != -1 && col != -1)
      quad[row][col]++;
  }

  sum += quad[0][0] * quad[0][1] * quad[1][0] * quad[1][1];

  return sum;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    auto robot = parse_robot("p=0,4 v=3,-3");
    assert(robot.p.x == 0);
    assert(robot.p.y == 4);
    assert(robot.v.x == 3);
    assert(robot.v.y == -3);
  }
  {
    auto robot = parse_robot("p=10,3 v=-1,2");
    assert(robot.p.x == 10);
    assert(robot.p.y == 3);
    assert(robot.v.x == -1);
    assert(robot.v.y == 2);
  }
  {
    assert(calc_quad(0, 11) == 0);
    assert(calc_quad(1, 11) == 0);
    assert(calc_quad(2, 11) == 0);
    assert(calc_quad(3, 11) == 0);
    assert(calc_quad(4, 11) == 0);
    assert(calc_quad(5, 11) == -1);
    assert(calc_quad(6, 11) == 1);
    assert(calc_quad(7, 11) == 1);
    assert(calc_quad(8, 11) == 1);
    assert(calc_quad(9, 11) == 1);
    assert(calc_quad(10, 11) == 1);
  }
  {
    int64_t w = 11;
    int64_t h = 7;
    {
      auto robot = parse_robot("p=0,0 v=1,1");
      auto pos = robot.move({w, h, 10});
      pos.verify(10, 3);
    }
    {
      auto robot = parse_robot("p=0,0 v=-1,-1");
      auto pos = robot.move({w, h, 1});
      pos.verify(10, 6);
    }
  }

  // std::ifstream is("abasic.input");
  // Rules rules {11,7,100};
  std::ifstream is("basic.input");
  Rules rules{101, 103, 100};

  Robots robots;

  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    auto robot = parse_robot(line);
    robots.push_back(robot);
  }

  sum = solution_1(rules, robots);

  std::cout << "Score " << sum << std::endl;

  return 0;
}