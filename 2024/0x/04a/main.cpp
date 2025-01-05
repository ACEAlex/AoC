#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
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

using Puzzle = std::vector<std::string>;

struct Pos {
  int x;
  int y;
};

bool is_inside(const Puzzle &p, int x, int y) {
  if (x < 0 || y < 0)
    return false;

  if (y >= p.size() || x >= p[y].size())
    return false;

  return true;
}

bool match_xmas(const Puzzle &p, int n, int x, int y) {
  const char *m = "XMAS";
  if (!is_inside(p, x, y))
    return false;
  return p[y][x] == m[n];
}

bool match_right(const Puzzle &p, int x, int y) {
  for (int i = 0; i < 4; ++i) {
    if (!match_xmas(p, i, x + i, y))
      return false;
  }
  return true;
}

bool match_left(const Puzzle &p, int x, int y) {
  for (int i = 0; i < 4; ++i) {
    if (!match_xmas(p, i, x - i, y))
      return false;
  }
  return true;
}

bool match_up(const Puzzle &p, int x, int y) {
  for (int i = 0; i < 4; ++i) {
    if (!match_xmas(p, i, x, y - i))
      return false;
  }
  return true;
}

bool match_down(const Puzzle &p, int x, int y) {
  for (int i = 0; i < 4; ++i) {
    if (!match_xmas(p, i, x, y + i))
      return false;
  }
  return true;
}

bool match_dir(const Puzzle &p, int x, int y, int dx, int dy) {
  for (int i = 0; i < 4; ++i) {
    if (!match_xmas(p, i, x + dx * i, y + dy * i))
      return false;
  }
  return true;
}

int64_t match_all(const Puzzle &p, int x, int y) {
  int64_t sum = 0;

  if (match_dir(p, x, y, 0, 1))
    sum++;
  if (match_dir(p, x, y, 1, 1))
    sum++;
  if (match_dir(p, x, y, 1, 0))
    sum++;

  if (match_dir(p, x, y, 0, -1))
    sum++;
  if (match_dir(p, x, y, -1, -1))
    sum++;
  if (match_dir(p, x, y, -1, 0))
    sum++;

  if (match_dir(p, x, y, -1, 1))
    sum++;
  if (match_dir(p, x, y, 1, -1))
    sum++;

  return sum;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    Puzzle p;
    p.push_back("Hej");
    p.push_back("då");
    assert(is_inside(p, 0, 0));
    assert(!is_inside(p, -1, -1));
    assert(!is_inside(p, -1, 0));
    assert(!is_inside(p, 0, -1));
    assert(!is_inside(p, 0, 2));
    assert(!match_left(p, 0, 0));
  }
  {
    Puzzle p;
    p.push_back("aXMASb");
    assert(!match_all(p, 0, 0));
    assert(match_all(p, 1, 0));
  }

  {
    Puzzle p;
    p.push_back("aSAMXb");
    assert(!match_all(p, 0, 0));
    assert(match_all(p, 4, 0));
  }
  {
    Puzzle p;
    p.push_back("a");
    p.push_back("X");
    p.push_back("M");
    p.push_back("A");
    p.push_back("S");
    p.push_back("b");
    assert(!match_all(p, 0, 0));
    assert(match_all(p, 0, 1));
  }

  int64_t sum = 0;

  Puzzle p;

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    p.push_back(input);
  }

  for (int y = 0; y < p.size(); ++y) {
    for (int x = 0; x < p[y].size(); ++x) {
      sum += match_all(p, x, y);
    }
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}