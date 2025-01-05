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

using Values = std::vector<int64_t>;

bool solve1_rec(int64_t target, const Values &v, int i, int64_t acc) {
  // We are at the end
  if (i >= v.size())
    return acc == target;

  if (target < acc)
    return false;

  if (solve1_rec(target, v, i + 1, acc + v[i]))
    return true;
  if (solve1_rec(target, v, i + 1, acc * v[i]))
    return true;

  auto acc2 = v[i];
  auto accstr = std::to_string(acc);
  auto acc2str = std::to_string(acc2);
  accstr.append(acc2str);
  auto sum = std::atoll(accstr.c_str());
  return solve1_rec(target, v, i + 1, sum);
}

int64_t solve1(int64_t target, const Values &v) {
  if (solve1_rec(target, v, 1, v[0])) {
    // return std::accumulate(v.begin(), v.end(), 0);
    return target;
  }
  return 0;
}

int64_t parse_line(const std::string &line, Values &v_out) {
  std::stringstream ss(line);
  std::string str;
  std::vector<int> update;
  getline(ss, str, ':');
  assert(!str.empty());
  auto target = std::atoll(str.c_str());
  getline(ss, str);
  std::stringstream ss2(str.c_str() + 1);
  std::string str2;
  while (ss2) {
    int64_t i = 0;
    ss2 >> i;
    if (i != 0)
      v_out.push_back(i);
  }
  return target;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    Values v;
    auto target = parse_line("190: 10 19", v);
    assert(target == 190);
    assert(v.size() == 2);
    assert(v[0] == 10);
    assert(v[1] == 19);
  }
  {
    Values v;
    auto target = parse_line("156: 15 6", v);
    auto ret = solve1(target, v);
    assert(ret == target);
  }
  {
    Values v;
    auto target = parse_line("7290: 6 8 6 15", v);
    auto ret = solve1(target, v);
    assert(ret == target);
  }
  {
    Values v;
    auto target = parse_line("192: 17 8 14", v);
    auto ret = solve1(target, v);
    assert(ret == target);
  }
  {
    Values v;
    auto target = parse_line("161011: 16 10 13", v);
    auto ret = solve1(target, v);
    assert(ret != target);
  }

  while (std::cin) {
    std::string line;
    std::getline(std::cin, line);
    if (line.empty())
      break;

    Values v;
    auto target = parse_line(line, v);

    sum += solve1(target, v);
  }

  std::cout << "Score " << sum << std::endl;

  return 0;
}