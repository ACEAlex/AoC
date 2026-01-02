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

int64_t part_1(const std::string &str) {
  std::vector<int> bank;
  for (auto i : str) {
    bank.push_back(i - '0');
  }

  int ret = 0;
  for (auto it = bank.begin(); it != bank.end(); ++it) {
    for (auto it2 = it + 1; it2 != bank.end(); ++it2) {
      int tmp = *it * 10 + *it2;
      if (tmp > ret)
        ret = tmp;
    }
  }

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

    score += part_1(input);
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}