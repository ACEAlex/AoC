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

struct Range {
  int64_t start;
  int64_t end;
};

int64_t GetNumberOfDigits(int64_t i) {
  return i > 0 ? (int64_t)log10((double)i) + 1 : 1;
}

int64_t power(int64_t value, int64_t power) {
  int64_t ret = 1;
  while (power) {
    ret *= value;
    power--;
  }

  return ret;
}

int64_t part_1(const std::string &str) {
  int64_t ret = 0;

  std::vector<std::string> inputs;
  boost::split(inputs, str, boost::is_any_of(","));

  for (const auto &i : inputs) {
    std::cout << "Testing: " << i << std::endl;
    std::vector<std::string> values;
    boost::split(values, i, boost::is_any_of("-"));
    assert(values.size() == 2);
    Range range{strtoll(values[0].c_str(), nullptr, 10),
                strtoll(values[1].c_str(), nullptr, 10)};
    std::cout << " R: " << range.start << " " << range.end << std::endl;

    // The digits to search 123-123456
    Range digits{GetNumberOfDigits(range.start), GetNumberOfDigits(range.end)};
    for (auto i = digits.start; i <= digits.end; ++i) {
      if (i % 2 == 1)
        continue;

      std::cout << " Checking digits " << i << std::endl;

      // 1025
      auto part = power(10, (i - 1) / 2);
      auto stop = part * 10;

      while (true) {
        auto current = (part * power(10, i / 2)) + part;
        if (part >= stop)
          break;
        if (current > range.end)
          break;

        if (current < range.start) {
          part++;
          continue;
        }
        auto match = part + part * power(10, i / 2);
        std::cout << "   Found match " << match << std::endl;
        ret += match;
        part++;
      }
    }
  }
  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  assert(GetNumberOfDigits(9) == 1);
  assert(GetNumberOfDigits(10) == 2);
  assert(GetNumberOfDigits(168) == 3);

  uint64_t score = 0;

  while (std::cin) {
    std::ifstream is("basic.input");
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    score = part_1(input);
    break;
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}