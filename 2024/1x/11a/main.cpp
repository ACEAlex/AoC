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
using Cache = std::map<int, int>;

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

  for (int i = 1; i <= 25; ++i) {
    blink(stuff, c, i);
    // print(stuff, i);
  }

  std::cout << "Score " << stuff.size() << std::endl;

  return 0;
}