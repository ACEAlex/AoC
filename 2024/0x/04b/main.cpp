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

bool match_c(const Puzzle &p, char c, int x, int y) {
  if (!is_inside(p, x, y))
    return false;
  return p[y][x] == c;
}

bool match_dir(const Puzzle &p, int x, int y, int dx, int dy) {
  for (int i = 0; i < 4; ++i) {
    if (!match_xmas(p, i, x + dx * i, y + dy * i))
      return false;
  }
  return true;
}

int64_t match_mas1(const Puzzle &p, int x, int y) {
  // M.S
  // .A.
  // M.S
  if (!match_c(p, 'A', x, y))
    return 0;

  if (!match_c(p, 'M', x - 1, y - 1))
    return 0;
  if (!match_c(p, 'M', x - 1, y + 1))
    return 0;

  if (!match_c(p, 'S', x + 1, y - 1))
    return 0;
  if (!match_c(p, 'S', x + 1, y + 1))
    return 0;

  return 1;
}

int64_t match_mas2(const Puzzle &p, int x, int y) {
  // S.M
  // .A.
  // S.M
  if (!match_c(p, 'A', x, y))
    return 0;

  if (!match_c(p, 'S', x - 1, y - 1))
    return 0;
  if (!match_c(p, 'S', x - 1, y + 1))
    return 0;

  if (!match_c(p, 'M', x + 1, y - 1))
    return 0;
  if (!match_c(p, 'M', x + 1, y + 1))
    return 0;

  return 1;
}
int64_t match_mas3(const Puzzle &p, int x, int y) {
  // M.M
  // .A.
  // S.S

  if (!match_c(p, 'A', x, y))
    return 0;

  if (!match_c(p, 'M', x - 1, y - 1))
    return 0;
  if (!match_c(p, 'S', x - 1, y + 1))
    return 0;

  if (!match_c(p, 'M', x + 1, y - 1))
    return 0;
  if (!match_c(p, 'S', x + 1, y + 1))
    return 0;

  return 1;
}
int64_t match_mas4(const Puzzle &p, int x, int y) {
  // S.S
  // .A.
  // M.M
  if (!match_c(p, 'A', x, y))
    return 0;

  if (!match_c(p, 'S', x - 1, y - 1))
    return 0;
  if (!match_c(p, 'M', x - 1, y + 1))
    return 0;

  if (!match_c(p, 'S', x + 1, y - 1))
    return 0;
  if (!match_c(p, 'M', x + 1, y + 1))
    return 0;

  return 1;
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
      sum += match_mas1(p, x, y);
      sum += match_mas2(p, x, y);
      sum += match_mas3(p, x, y);
      sum += match_mas4(p, x, y);
    }
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}