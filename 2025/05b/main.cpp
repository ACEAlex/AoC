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

int64_t fresh(const Ranges &ranges) {
  int64_t ret = 0;
  int64_t current = -1;

  for (auto it = ranges.begin(); it != ranges.end(); ++it) {
    auto start = it->first;
    auto end = it->second;
    if (current >= start) {
      start = current + 1;
    }

    if (start <= end) {
      ret += (end - start) + 1;
    }
    std::cout << "Foo: " << ret << std::endl;
    current = std::max(current, end);
  }
  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t score = 0;

  Ranges ranges;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    std::vector<std::string> v;
    boost::split(v, input, boost::is_any_of("-"));
    // std::cout << strtoll(v[0].c_str(), nullptr,10) << " " <<
    // strtoll(v[1].c_str(), nullptr, 10) << std::endl;
    ranges.push_back({strtoll(v[0].c_str(), nullptr, 10),
                      strtoll(v[1].c_str(), nullptr, 10)});
  }

  std::sort(ranges.begin(), ranges.end());
  score = fresh(ranges);

  std::cout << "Score " << score << std::endl;

  return 0;
}