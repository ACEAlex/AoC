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

int64_t calc_value(int64_t value, int64_t repeat) {
  int64_t ret = 0;
  auto digits = GetNumberOfDigits(value);
  while (repeat-- > 0) {
    ret = ret * power(10, digits) + value;
  }
  return ret;
}

int64_t part_2(const std::string &str) {
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

    std::set<int64_t> matches;

    for (auto i = digits.start; i <= digits.end; ++i) {
      // if (i % 2 == 1)
      //   continue;
      // Search number of parts 123456 is possible to search with 1,2,3
      // Search number of parts 12345 is possible to search with 1
      // Search number of parts 12345678 is possible to search with 1,2,4

      for (int64_t digit_part = 1; digit_part <= i / 2; ++digit_part) {
        if (i % digit_part != 0)
          continue;
        //   std::cout << " Checking digits " << i << std::endl;

        // 1025
        auto part = power(10, (digit_part - 1));
        auto stop = part * 10;

        while (true) {
          auto current = calc_value(part, i / digit_part);
          if (part >= stop)
            break;
          if (current > range.end)
            break;

          if (current < range.start) {
            part++;
            continue;
          }
          // auto match = part + part * power(10,i/2);
          std::cout << "   Found match " << current;
          auto [it, r] = matches.insert(current);
          if (r)
            std::cout << " new";
          std::cout << std::endl;
          //          ret += current;
          part++;
        }
      }
    }
    for (auto i : matches)
      ret += i;
  }
  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  assert(GetNumberOfDigits(9) == 1);
  assert(GetNumberOfDigits(10) == 2);
  assert(GetNumberOfDigits(168) == 3);
  assert(calc_value(42, 3) == 424242);

  uint64_t score = 0;

  while (std::cin) {
    std::ifstream is("basic.input");
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    score = part_2(input);
    break;
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}