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
  std::vector<XY> history;
  // Dir dir;

  bool operator<(const QueueItem &other) const {
    // < = largest first
    // return cost > other.cost;
    return pos.distance(finish) > other.pos.distance(finish);
  }
  static XY start;
  static XY finish;
  static int64_t scorediff;
  static int64_t cheat_size;
};

XY QueueItem::start = XY{0, 0};
XY QueueItem::finish = XY{0, 0};
int64_t QueueItem::scorediff = 100;
int64_t QueueItem::cheat_size = 20;

// struct Robot {
//   XY p;
//   Dir dir;
// };

// struct Score {
//   int64_t
//   dir_score[4]{10000000000000000,10000000000000000,10000000000000000,10000000000000000};
// };

using Map = std::vector<std::string>;
// using VisitedMap = std::vector<std::vector<Score>>;
using VisitedMap = std::vector<std::vector<int64_t>>;
using Cheats = std::set<std::pair<XY, XY>>;

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

char map2c(const Map &m, XY pos) {
  if (is_inside(m, pos)) {
    return m[pos.y][pos.x];
  }
  return '#';
}

int64_t is_cheat(const Map &m, const VisitedMap &visitMap, XY start, XY end,
                 Cheats &cheats) {
  auto s1 = visitMap[start.y][start.x];
  if (is_walkable(m, end)) {
    auto s2 = visitMap[end.y][end.x];
    if (s2 == -1)
      return 0;
    auto save = s2 - s1 - start.distance(end);
    if (save >= (QueueItem::scorediff)) {
      // std::cout << "Cheat: " << start << end << " save " << save <<
      // std::endl;
      cheats.insert({start, end});
      return 1;
    }
  }
  return 0;
}

int64_t find_exit(const Map &m, Cheats &cheats) {
  int64_t best = std::numeric_limits<int64_t>::max();
  std::priority_queue<QueueItem> queue;
  VisitedMap visitMap(m.size(), std::vector<int64_t>(m[0].size(), -1));

  {
    QueueItem start;
    start.pos = QueueItem::start;
    start.cost = 0;
    queue.push(start);
  }

  std::vector<XY> best_history;

  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();

    // Already visited
    auto &old = visitMap[current.pos.y][current.pos.x];

    // Old was better
    if (old != -1 && old < current.cost)
      continue;

    old = current.cost;

    if (current.pos == QueueItem::finish) {
      if (best > current.cost) {
        // std::cout << "Found the end with new best " << current.cost <<
        // std::endl;
        best = current.cost;
        assert(current.cost == current.history.size());
        best_history = current.history;
        break;
      }
    }

    current.history.push_back(current.pos);

    // Try all the pos
    for (int i = 0; i < 4; ++i) {
      auto next = current;
      next.cost += 1;
      next.pos += DirArray[i];
      if (is_walkable(m, next.pos) &&
          (visitMap[next.pos.y][next.pos.x] == -1 ||
           visitMap[next.pos.y][next.pos.x] > next.cost))
        queue.push(next);
    }
  }

  int64_t sum = 0;

  std::cout << "Best: " << best << std::endl;

  for (const auto path : best_history) {
    XY ce; // cheat end
    for (ce.y = path.y - QueueItem::cheat_size;
         ce.y <= path.y + QueueItem::cheat_size; ce.y++) {
      for (ce.x = path.x - QueueItem::cheat_size;
           ce.x <= path.x + QueueItem::cheat_size; ce.x++) {
        if (!is_inside(m, ce))
          continue;
        if (path == ce || path.distance(ce) > QueueItem::cheat_size)
          continue;
        if (is_walkable(m, ce)) {
          is_cheat(m, visitMap, path, ce, cheats);
          // cheats.insert( {xy,ce});
        }
      }
    }
  }

  return sum;
}

void set_start(Map &m) {
  for (int r = 0; r < m.size(); ++r) {
    for (int c = 0; c < m[0].size(); ++c) {
      if (m[r][c] == 'S') {
        QueueItem::start = XY{c, r};
        return;
      }
    }
  }
  assert(false);
}

void set_finish(Map &m) {
  for (int r = 0; r < m.size(); ++r) {
    for (int c = 0; c < m[0].size(); ++c) {
      if (m[r][c] == 'E') {
        QueueItem::finish = XY{c, r};
        return;
      }
    }
  }
  assert(false);
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  std::ifstream is("basic.input");
  // std::ifstream is("abasic.input");

  Map map;

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    map.push_back(line);
  }

  set_start(map);
  set_finish(map);

  Cheats cheats;

  sum = find_exit(map, cheats);

  for (const auto &row : map) {
    for (const auto &c : row) {
      std::cout << c;
    }
    std::cout << std::endl;
  }

  // sum = calc_score(map);

  std::cout << "Score " << sum << std::endl;
  std::cout << "Score2 " << cheats.size() << std::endl;

  return 0;
}