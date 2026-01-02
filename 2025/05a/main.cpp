#include "boost/algorithm/string/split.hpp"
#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
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

using Range = std::pair<int64_t, int64_t>;
using Ranges = std::vector<Range>;

bool fresh(const Ranges &ranges, int64_t value) {
  std::cout << "Testing: " << value << std::endl;
  for (const auto &r : ranges) {
    if (value >= r.first and value <= r.second)
      return true;
  }
  return false;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  uint64_t score = 0;

  Ranges ranges;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    std::vector<std::string> v;
    boost::split(v, input, boost::is_any_of("-"));
    ranges.push_back({strtoll(v[0].c_str(), nullptr, 10),
                      strtoll(v[1].c_str(), nullptr, 10)});
  }

  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;
    auto v = strtoll(input.c_str(), nullptr, 10);
    if (fresh(ranges, v))
      score++;
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}