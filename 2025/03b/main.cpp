#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
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

int64_t power(int64_t value, int64_t power) {
  int64_t ret = 1;
  while (power) {
    ret *= value;
    power--;
  }

  return ret;
}

using Key = std::pair<int, int>;
using Cache = std::map<Key, int64_t>;

int64_t solve(const std::vector<int> v, std::pair<int, int> key,
              Cache &cache) // key = index, depth
{
  if (key.first >= v.size() or key.second <= 0)
    return -1;
  if (auto it = cache.find(key); it != cache.end())
    return it->second;

  int64_t ret = 0;
  int64_t highest_battery = 0;
  int64_t highest_value = -1;
  auto pow = power(10, key.second - 1);
  for (int i = key.first; i < v.size(); ++i) {
    if (v[i] < highest_battery)
      continue;
    highest_battery = v[i];

    if (key.second == 1)
      continue;

    auto next = solve(v, {i + 1, key.second - 1}, cache);
    if (next == -1)
      break;

    auto val = next + pow * v[i];
    if (val > highest_value)
      highest_value = val;
  }
  if (key.second == 1)
    return highest_battery;
  cache.insert({key, highest_value});
  return highest_value;
}

int64_t part_2(const std::string &str) {
  std::vector<int> bank;
  for (auto i : str) {
    bank.push_back(i - '0');
  }

  Cache cache;
  auto ret = solve(bank, {0, 12}, cache);

  std::cout << "Line: " << ret << std::endl;

  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  uint64_t score = 0;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    score += part_2(input);
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}