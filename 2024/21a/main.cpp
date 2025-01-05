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
  XY operator-(const XY &other) const {
    XY ret = *this;
    ret.x -= other.x;
    ret.y -= other.y;
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

enum Dir { Up, Right, Down, Left };

static const XY DirArray[4]{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

using Seq = std::string;
using Seqs = std::vector<Seq>;

struct Keypad {
  std::vector<std::string> layout;
  XY start_hand;

  std::map<std::pair<char, char>, Seqs> from_to_cache;

  const Seqs &from_to(char c1, char c2) const {
    auto it = from_to_cache.find({c1, c2});
    assert(it != from_to_cache.end());
    return it->second;
  }

  void prep_cache() {
    XY xy1;
    for (xy1.y = 0; xy1.y < layout.size(); xy1.y++) {
      for (xy1.x = 0; xy1.x < layout[xy1.y].size(); xy1.x++) {
        XY xy2;
        for (xy2.y = 0; xy2.y < layout.size(); xy2.y++) {
          for (xy2.x = 0; xy2.x < layout[xy2.y].size(); xy2.x++) {
            std::pair<char, char> key = {get_char(xy1), get_char(xy2)};
            prep_cache("", xy1, xy2, key);
          }
        }
      }
    }
  }

  void prep_cache(Seq seq, XY current, XY target,
                  const std::pair<char, char> &key) {
    if (get_char(current) == '-' || get_char(target) == '-')
      return;
    // if (key.first == key.second)
    //   from_to_cache[key].push_back("AA");
    if (current == target) {
      seq.append("A");
      auto &m = from_to_cache[key];
      m.push_back(seq);
      return;
    }

    auto delta = target - current;

    if (delta.y > 0) {
      auto next_seq = seq;
      auto next_pos = current;
      next_pos.y++;
      next_seq.append("v");
      prep_cache(next_seq, next_pos, target, key);
    }
    if (delta.y < 0) {
      auto next_seq = seq;
      auto next_pos = current;
      next_pos.y--;
      next_seq.append("^");
      prep_cache(next_seq, next_pos, target, key);
    }
    if (delta.x > 0) {
      auto next_seq = seq;
      auto next_pos = current;
      next_pos.x++;
      next_seq.append(">");
      prep_cache(next_seq, next_pos, target, key);
    }
    if (delta.x < 0) {
      auto next_seq = seq;
      auto next_pos = current;
      next_pos.x--;
      next_seq.append("<");
      prep_cache(next_seq, next_pos, target, key);
    }
  }

  XY find(char c) const {
    XY xy;
    for (xy.y = 0; xy.y < layout.size(); ++xy.y) {
      for (xy.x = 0; xy.x < layout[xy.y].size(); ++xy.x) {
        if (layout[xy.y][xy.x] == c)
          return xy;
      }
    }
    assert(false);
    return xy;
  }

  char get_char(XY xy) const { return layout[xy.y][xy.x]; }
};

struct NumericKeypad : public Keypad {
  NumericKeypad() {
    layout = {"789", "456", "123", "-0A"};
    start_hand = {2, 3};
    prep_cache();
  }
};

struct DirectionalKeypad : public Keypad {
  DirectionalKeypad() {
    layout = {"-^A", "<v>"};
    start_hand = {2, 0};
    prep_cache();
  }
};

using CacheKey = std::tuple<char, char, int>;
using Cache = std::map<CacheKey, int64_t>;

int64_t solve_string(std::string code, const Keypad &currentkp,
                     const DirectionalKeypad &dc, Cache &cache, int depth);

int64_t solve_pair(char from, char to, const Keypad &currentkp,
                   const DirectionalKeypad &dc, Cache &cache, int depth) {
  CacheKey key{from, to, depth};
  auto it = cache.find(key);
  if (it != cache.end())
    return it->second;

  auto seqs = currentkp.from_to(from, to);
  int64_t best = std::numeric_limits<int64_t>::max();
  assert(!seqs.empty());
  for (const auto &s : seqs) {
    int64_t tmp = 1;
    if (s != "A")
      tmp = solve_string(s, dc, dc, cache, depth - 1);
    best = std::min(best, tmp);
  }
  // Update cache with calculated value
  cache[key] = best;

  return best;
}

int64_t solve_string(std::string code, const Keypad &currentkp,
                     const DirectionalKeypad &dc, Cache &cache, int depth) {
  std::string foo("A");
  foo.append(code);
  code = foo;
  // std::cout << "solve: " << code << " depth=" << depth << std::endl;

  // This is the one. No need to resolve deper
  if (depth == 0) {
    return code.size() - 1;
    // const auto& seq = currentkp.from_to(from, to);
    // return seq.front().size();
  }

  int64_t sum = 0;
  for (int i = 0; i < code.size() - 1; ++i) {
    auto from = code[i];
    auto to = code[i + 1];
    sum += solve_pair(from, to, currentkp, dc, cache, depth);
    // std::cout << " score: " << code << " depth=" << depth << " sum=" << sum
    // << std::endl;
  }

  return sum;
}

int64_t solution_1(const std::string &code, int depth) {
  Cache cache;
  NumericKeypad nc;
  DirectionalKeypad dc;

  // int64_t ret = 0;
  // for (int i = 0; i < code.size()-1; ++i)
  // {
  // ret += solve(code.c_str() + i, nc, dc, cache, depth);
  // }

  auto ret = solve_string(code, nc, dc, cache, depth);

  return ret;
}

int64_t calc_score(const std::string &code, int64_t nbr) {
  auto short_code = code.substr(0, code.size() - 1);
  auto tmp = atol(short_code.c_str());
  int64_t ret = tmp * nbr;
  std::cout << "Score: " << ret << " (" << nbr << " * " << tmp << ")"
            << std::endl;
  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;
  {
    DirectionalKeypad keypad;
    auto r = keypad.from_to('A', 'v');
    for (const auto &i : r) {
      // std::cout << "Foo: " << i << std::endl;
    }
    int i = 0;
  }

  std::ifstream is("basic.input");

  std::vector<std::string> map;

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    map.push_back(line);
  }

  for (const auto &i : map) {
    std::cout << "Testing: " << i << std::endl;
    // Add A as it is the start
    // std::string astring("A");
    // astring.append(i);

    auto solution = solution_1(i, 3);
    sum += calc_score(i, solution);
    // break;
  }

  std::cout << "Total score " << sum << std::endl;

  return 0;
}