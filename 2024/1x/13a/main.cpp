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

  bool operator<(const XY &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

struct QueueItem {
  int64_t cost{0};
  int presses_a{0};
  int presses_b{0};
  XY pos;

  bool operator<(const QueueItem &other) const {
    // < = largest first
    return cost > other.cost;
  }
};

struct Quest {
  XY a;
  XY b;
  XY target;
};

XY parse_button(const std::string &str) {
  XY ret;

  auto r = sscanf(str.c_str(), "Button %*c: X+%ld, Y+%ld", &ret.x, &ret.y);
  assert(r == 2);

  return ret;
}

XY parse_target(const std::string &str) {
  XY ret;
  auto r = sscanf(str.c_str(), "Prize: X=%ld, Y=%ld", &ret.x, &ret.y);
  assert(r == 2);

  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    auto ba = parse_button("Button A: X+94, Y+34");
    assert(ba.x = 94);
    assert(ba.y = 34);
  }
  {
    auto ba = parse_button("Button B: X+22, Y+67");
    assert(ba.x = 22);
    assert(ba.y = 67);
  }
  {
    auto ba = parse_target("Prize: X=8400, Y=5400");
    assert(ba.x = 8400);
    assert(ba.y = 5400);
  }

  std::ifstream is("basic.input");

  XY target;

  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    Quest quest;
    quest.a = parse_button(line);
    std::getline(is, line);
    quest.b = parse_button(line);
    std::getline(is, line);
    quest.target = parse_target(line);

    // BFS
    std::priority_queue<QueueItem> queue;
    std::set<XY> visited;
    queue.push({0, 0, 0, {0, 0}});
    while (!queue.empty()) {
      auto current = queue.top();
      queue.pop();

      if (visited.find(current.pos) != visited.end()) {
        continue;
      }
      visited.insert(current.pos);

      // std::cout << "Testing: " << current.cost << " Pos " << current.pos.x <<
      // " " << current.pos.y << std::endl;
      if (current.pos == quest.target) {
        std::cout << "Found a solution: " << current.cost
                  << " apress=" << current.presses_a
                  << " bpress=" << current.presses_b << std::endl;
        sum += current.cost;
        break;
      }

      // Dont continue after 100
      if (current.presses_a >= 100 || current.presses_b >= 100)
        continue;

      // 3 token to push a
      {
        QueueItem next = current;
        next.presses_a++;
        next.cost += 3;
        next.pos.x += quest.a.x;
        next.pos.y += quest.a.y;
        queue.push(next);
      }
      // 1 token to push b
      {
        QueueItem next = current;
        next.presses_b++;
        next.cost += 1;
        next.pos.x += quest.b.x;
        next.pos.y += quest.b.y;
        queue.push(next);
      }
    }
    std::getline(is, line);
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}