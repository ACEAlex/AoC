#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cmath>
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
  int64_t x;
  int64_t y;
  bool operator<(const Pos &other) const {
    return std::tie(x, y) < std::tie(x, y);
  }
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

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t score = 0;

  {
    auto p = parse_pos("123,456");
    assert(p->x == 123);
    assert(p->y == 456);
  }

  std::vector<std::unique_ptr<Pos>> positions; // Heap = memory stable

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    auto pos = parse_pos(input);

    // Make it a pair of every already read postions
    for (const auto &i : positions) {
      auto rect = (1 + std::abs(i->x - pos->x)) * (1 + std::abs(i->y - pos->y));
      score = std::max(score, rect);
    }
    positions.push_back(std::move(pos));
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}