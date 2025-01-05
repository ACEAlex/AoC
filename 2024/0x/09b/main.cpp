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

int find_leftmost_empty(const Map &m, int size) {
  int r = -1;
  for (int i = 0; i < m.size(); ++i) {
    // Start of space
    if (m[i] != -1) {
      continue;
    }
    int space = 1;
    int start_space = i;
    while (i < m.size() && m[i + space] == -1) {
      space++;
    }
    if (space >= size) {
      r = start_space;
      break;
    } else
      i += space;
  }

  return r;
}

void defrag(Map &m) {
  int i2 = m.size() - 1;
  while (0 < i2) {

    if (m[i2] == -1) {
      i2--;
      continue;
    }

    int chunk_size = 1;
    while (i2 > 0 && m[i2 - chunk_size] == m[i2]) {
      chunk_size++;
    }

    // Find leftmost empty space
    auto lms = find_leftmost_empty(m, chunk_size);

    // Did not find any for this space --> skip over
    if (lms == -1 || lms >= i2) {
      i2 -= chunk_size;
      continue;
    }

    // We found a space. Lets copy
    for (int a = 0; a < chunk_size; ++a) {
      m[lms + a] = m[i2 - a];
      m[i2 - a] = -1;
    }
    // Move over
    i2 -= chunk_size;
  }
}

int64_t score(const Map &m) {
  int64_t r = 0;

  for (int64_t i = 0; i < m.size(); ++i) {
    if (m[i] < 0)
      continue;

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

  {
    Map tmp;
    parse_input(tmp, "112233114");
    auto str = to_string(tmp);
    assert(str == "0.11..222...3.4444");
    auto p1 = find_leftmost_empty(tmp, 3);
    assert(p1 == 9);
    auto p2 = find_leftmost_empty(tmp, 4);
    assert(p2 == -1);
  }

  {
    Map tmp;
    parse_input(tmp, "2333133121414131402");
    auto str = to_string(tmp);
    assert(str == "00...111...2...333.44.5555.6666.777.888899");
    defrag(tmp);
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