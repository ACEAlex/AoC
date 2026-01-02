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
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

using Row = std::vector<int64_t>;
using Map = std::vector<Row>;

std::vector<std::string> getVector(const std::string &str) {
  std::vector<std::string> v;
  boost::split(v, str, boost::is_any_of(" "), boost::token_compress_on);

  std::vector<std::string> ret;
  for (auto i : v) {
    boost::trim(i);
    if (i != "") {
      std::cout << i << std::endl;
      ret.push_back(i);
    }
  }

  return ret;
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

    auto v = getVector(input);
    if (v[0] == "+" or v[0] == "*") {
      for (int col = 0; col < v.size(); ++col) {
        int64_t colsum = map[0][col];
        for (int row = 1; row < map[row].size(); ++row) {
          if (v[col] == "*")
            colsum *= map[row][col];
          else if (v[col] == "+")
            colsum += map[row][col];
          else
            assert(false);
        }
        score += colsum;
      }

      break;
    }
    Row r;
    for (auto i : v) {
      r.push_back(strtoll(i.c_str(), nullptr, 10));
    }
    map.push_back(r);
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}