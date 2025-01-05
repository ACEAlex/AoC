#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
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

using Stuff = std::deque<int64_t>;

// Value, depth -> sum
using Cache = std::map<std::pair<int64_t, int>, int64_t>;

void parse_input(Stuff &stuff, const std::string &str) {
  std::stringstream ss(str);

  std::string s;

  while (getline(ss, s, ' ')) {
    auto i = atoi(s.c_str());
    stuff.push_back(i);
  }
}

void blink(Stuff &stuff, Cache &c, int lvl) {
  for (auto it = stuff.begin(); it != stuff.end(); ++it) {
    if (*it == 0) {
      *it = 1;
      continue;
    }

    auto str = std::to_string(*it);
    if (str.size() % 2 == 0) {
      auto part1 = str.substr(0, str.size() / 2);
      auto part2 = str.substr((str.size() / 2));
      auto i1 = atoll(part1.c_str());
      auto i2 = atoll(part2.c_str());
      it = stuff.insert(it, i1);
      ++it;
      *it = i2;
      continue;
    }

    *it = (*it) * 2024;
  }
}

int64_t down(Cache &c, int64_t value, int lvl) {
  if (lvl == 0)
    return 1;

  auto it = c.find({value, lvl});
  if (it != c.end()) {
    return it->second;
  }

  lvl--;

  if (value == 0) {
    auto r = down(c, 1, lvl);
    c[{1, lvl}] = r;
    return r;
  }

  auto str = std::to_string(value);
  if (str.size() % 2 == 0) {
    auto part1 = str.substr(0, str.size() / 2);
    auto part2 = str.substr((str.size() / 2));
    auto i1 = atoll(part1.c_str());
    auto i2 = atoll(part2.c_str());

    auto r1 = down(c, i1, lvl);
    c[{i1, lvl}] = r1;

    auto r2 = down(c, i2, lvl);
    c[{i2, lvl}] = r2;

    return r1 + r2;
  }

  return down(c, value * 2024, lvl);
}

void print(const Stuff &s, int lvl) {
  std::cout << "After " << lvl << " blink:" << std::endl;
  for (const auto &i : s) {
    std::cout << i << " ";
  }
  std::cout << std::endl << std::endl;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    Stuff s;
    parse_input(s, "125 17");
    assert(s.size() == 2);
    assert(s.front() == 125);
    assert(s.back() == 17);
  }
  {
    Stuff s;
    parse_input(s, "2 54 992917 5270417 2514 28561 0 990");
    assert(s.size() == 8);
    assert(s.front() == 2);
    assert(s.back() == 990);
  }

  int64_t sum = 0;
  Stuff stuff;

  std::string line("2 54 992917 5270417 2514 28561 0 990");

  parse_input(stuff, line);

  Cache c;

  print(stuff, 0);


  for (auto i : stuff) {
    sum += down(c, i, 75);
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}