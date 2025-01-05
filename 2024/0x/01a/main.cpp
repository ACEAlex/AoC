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

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t diff = 0;
  std::vector<int64_t> v1, v2;

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    int a = 0;
    int b = 0;
    auto r = sscanf(input.c_str(), "%d %d", &a, &b);
    assert(r == 2);
    v1.emplace_back(a);
    v2.emplace_back(b);
  }
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());

  auto i1 = v1.begin();
  auto i2 = v2.begin();
  while (i1 != v1.end() && i2 != v2.end()) {
    auto delta = *i1 - *i2;
    diff += std::abs(delta);
    ++i1;
    ++i2;
    // std::cout << i.first << " " << i.second << " " << (i.first-i.second) << "
    // " << diff << std::endl;
  }

  std::cout << "Score " << diff << std::endl;

  return 0;
}