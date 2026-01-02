#include "boost/algorithm/string/constants.hpp"
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
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

using Row = std::string;
using Map = std::vector<Row>;

int64_t getNum(const Map &map, int col) {
  int64_t ret;
  std::stringstream ss;
  for (auto &row : map) {
    if (row[col] != ' ')
      ss << row[col];
  }

  auto str = ss.str();
  boost::trim(str);
  return strtoll(str.c_str(), nullptr, 10);
}

const char *findColEnd(const char *ptr) {
  while (++ptr) {
    if (*ptr == '\0' or *ptr != ' ') {
      ptr--;
      return ptr;
    }
  }
  assert(false);
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t score = 0;

  Map map;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    map.push_back(input);
  }

  // Rotate input
  Map rmap;
  char op = ' ';
  for (int col = map[0].size() - 1; col >= 0; --col) {
    std::stringstream ss;
    for (int row = 0; row < map.size() - 1; ++row) {
      ss << map[row][col];
    }
    auto str = ss.str();
    boost::trim(str);
    if (str.empty())
      continue;
    rmap.push_back(str);
    op = map.back()[col];
    if (op == '*') {
      int64_t sum = 1;
      for (auto i : rmap) {
        sum *= atoi(i.c_str());
      }

      std::cout << "Sum* " << sum << std::endl;
      score += sum;
      rmap.clear();
    } else if (op == '+') {
      int64_t sum = 0;
      for (auto i : rmap) {
        sum += atoi(i.c_str());
      }
      std::cout << "Sum+ " << sum << std::endl;
      score += sum;
      rmap.clear();
    }
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}