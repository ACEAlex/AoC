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

using VisitCache = std::set<XY>;

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

  ret.x += 10000000000000;
  ret.y += 10000000000000;

  return ret;
}

bool is_visited(const VisitCache &vc, const XY &xy) {
  return vc.find(xy) != vc.end();
}

int64_t solution_1(const Quest &quest) {
  int64_t sum = 0;
  // BFS
  std::priority_queue<QueueItem> queue;
  std::set<XY> visited;
  queue.push({0, 0, 0, {0, 0}});
  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();

    if (current.pos == quest.target) {
      std::cout << "Found a solution: " << current.cost
                << " apress=" << current.presses_a
                << " bpress=" << current.presses_b << std::endl;
      std::cout << "For: " << quest.target.x << " " << quest.target.y
                << std::endl;
      sum += current.cost;
      break;
    }

    // Dont continue after 100
    // if (current.presses_a >= 100 || current.presses_b >= 100)
    //   continue;

    // Dont continue if we are past x or y on target
    if (current.pos.x > quest.target.x || current.pos.y > quest.target.y)
      continue;

    // 3 token to push a
    {
      QueueItem next = current;
      do {
        next.presses_a++;
        next.cost += 3;
        next.pos.x += quest.a.x;
        next.pos.y += quest.a.y;
        if ((quest.target.x - next.pos.x) % quest.b.x == 0 &&
            (quest.target.y - next.pos.y) % quest.b.y == 0 &&
            !is_visited(visited, next.pos)) {
          visited.insert(next.pos);
          queue.push(next);
        }
      } while (next.pos.x <= quest.target.x && next.pos.y <= quest.target.y);
    }
    // 1 token to push b
    {
      QueueItem next = current;
      do {
        next.presses_b++;
        next.cost += 1;
        next.pos.x += quest.b.x;
        next.pos.y += quest.b.y;
        if ((quest.target.x - next.pos.x) % quest.a.x == 0 &&
            (quest.target.y - next.pos.y) % quest.a.y == 0 &&
            !is_visited(visited, next.pos)) {
          visited.insert(next.pos);
          queue.push(next);
        }
      } while (next.pos.x <= quest.target.x && next.pos.y <= quest.target.y);
    }
  }
  return sum;
}

int64_t solution_2(const Quest &quest) {
  int64_t sum = 0;
  int found = false;
  int64_t best = std::numeric_limits<int64_t>::max();
  int64_t best_a = 0;
  int64_t best_b = 0;

  XY pos{0, 0};
  int64_t cost_a{0};
  int64_t presses_a{0};

  while (pos.x <= quest.target.x && pos.y <= quest.target.y) {
    XY delta{quest.target.x - pos.x, quest.target.y - pos.y};
    if (delta.x % quest.b.x == 0 && delta.y % quest.b.y == 0) {
      auto presses_b = delta.x / quest.b.x;
      auto presses_by = delta.y / quest.b.y;

      auto cost_total = cost_a + presses_b;

      if (presses_b == presses_by && cost_total < best) {
        best = cost_total;
        best_a = presses_a;
        best_b = presses_b;
        found = true;
      }
    }
    presses_a++;
    cost_a += 3;
    pos.x += quest.a.x;
    pos.y += quest.a.y;
  }

  if (found) {
    std::cout << "Found a solution: " << best << " apress=" << best_a
              << " bpress=" << best_b << std::endl;
    std::cout << "For: " << quest.target.x << " " << quest.target.y
              << std::endl;
    return best;
  }
  return 0;
}

int64_t solution_3(const Quest &q) {
  const auto &t = q.target;
  const auto &a = q.a;
  const auto &b = q.b;

  int64_t top = t.y * a.x - a.y * t.x;
  int64_t bot = b.y * a.x - a.y * b.x;
  assert(bot != 0);

  bool solution = (top % bot == 0);
  if (solution) {
    int64_t d = top / bot;
    int64_t top2 = (t.x - d * b.x);
    bool solution2 = (top2 % a.x == 0);
    if (solution2) {
      int64_t c = top2 / a.x;

      auto best = 3 * c + 1 * d;
      std::cout << "Found a solution: " << best << " apress=" << c
                << " bpress=" << d << std::endl;
      std::cout << "For: " << q.target.x << " " << q.target.y << std::endl;
      return best;
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    auto ba = parse_button("Button A: X+94, Y+34");
    assert(ba.x == 94);
    assert(ba.y == 34);
  }
  {
    auto ba = parse_button("Button B: X+22, Y+67");
    assert(ba.x == 22);
    assert(ba.y == 67);
  }
  {
    auto ba = parse_target("Prize: X=8400, Y=5400");
    // assert(ba.x == 8400);
    // assert(ba.y == 5400);
  }

  std::ifstream is("basic.input");

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

    sum += solution_3(quest);

    std::getline(is, line);
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}