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

using Towels = std::vector<std::pair<std::string, int>>;

Towels parse_towels(const std::string &str) {
  std::stringstream ss(str);

  Towels ret;

  while (ss) {
    std::string current;
    std::getline(ss, current, ',');
    if (current.empty())
      break;
    auto ptr = current.c_str();
    if (*ptr == ' ')
      ptr++;
    std::string foo(ptr);
    ret.push_back({foo, foo.size()});
  }

  return ret;
}

using Cache = std::map<const char *, int64_t>;
int64_t solution_1(const Towels &ts, const std::string &line, int i,
                   Cache &cache) {
  auto it = cache.find(line.c_str() + i);
  if (it != cache.end())
    return it->second;

  if (i >= line.size())
    return 1;

  int64_t res = 0;
  for (const auto &t : ts) {
    if (strncmp(t.first.c_str(), line.c_str() + i, t.second) == 0) {
      res += solution_1(ts, line, i + t.second, cache);
    }
  }
  cache[line.c_str() + i] = res;

  return res;
}

Towels reduce_towels(const Towels &ts, const std::string &line) {
  Towels ret;

  for (const auto &t : ts) {
    if (line.contains(t.first))
      ret.push_back(t);
  }

  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    auto t = parse_towels("r, wr, b, g");
    assert(t.size() == 4);
    assert(t[0].first == "r");
    assert(t[1].first == "wr");
    assert(t[2].first == "b");
    assert(t[3].first == "g");
  }

  int64_t sum = 0;
  std::ifstream is("basic.input");
  Towels towels;

  {
    std::string line;
    std::getline(is, line);
    towels = parse_towels(line);
    std::getline(is, line);
  }

  // Parse quests
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    std::cout << "Testing " << line;
    Cache cache;
    auto reduced_towels = reduce_towels(towels, line);
    sum += solution_1(reduced_towels, line, 0, cache);
    std::cout << std::endl;
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}