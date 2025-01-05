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

using Map = std::vector<int64_t>;

std::string to_string(const Map &m) {
  std::stringstream ss;

  for (const auto &i : m) {
    if (i == -1)
      ss << '.';
    else if (i == -2) {
      break;
    } else
      ss << i;
  }
  return ss.str();
}

void parse_input(Map &m, const std::string &input) {
  int64_t id{0};
  bool is_file{true};
  for (const auto &c : input) {
    auto n = c - '0';
    while (n) {
      if (is_file)
        m.push_back(id);
      else
        m.push_back(-1);
      n--;
    }
    if (is_file)
      id++;
    is_file = !is_file;
  }
}

void defrag(Map &m) {
  int i1 = 0;
  int i2 = m.size() - 1;
  while (i1 < i2) {
    if (m[i1] != -1) {
      i1++;
      continue;
    }

    if (m[i2] == -1) {
      i2--;
      continue;
    }

    // Empty + we got value at the end
    m[i1] = m[i2];
    i1++;
    m[i2] = -2;
    i2--;
  }
}

int64_t score(const Map &m) {
  int64_t r = 0;

  for (int64_t i = 0; i < m.size(); ++i) {
    if (m[i] < 0)
      break;

    r += m[i] * i;
  }

  return r;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    Map tmp;
    parse_input(tmp, "12345");
    auto str = to_string(tmp);
    assert(str == "0..111....22222");
  }
  {
    Map tmp;
    parse_input(tmp, "90909");
    auto str = to_string(tmp);
    assert(str == "000000000111111111222222222");
  }

  Map map;

  while (std::cin) {
    std::string line;
    std::getline(std::cin, line);
    if (line.empty())
      break;

    parse_input(map, line);
  }

  std::cout << "Before defrag\n" << to_string(map) << std::endl;
  defrag(map);
  std::cout << "After defrag\n" << to_string(map) << std::endl;

  sum = score(map);

  std::cout << "Score " << sum << std::endl;

  return 0;
}