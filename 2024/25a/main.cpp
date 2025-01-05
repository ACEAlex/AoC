#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <limits>
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

enum class DType { Key, Lock };

using DataStore = std::vector<int>;

struct Item {
  DType type;
  DataStore data;
  Item() : data(5, 0) {}
};

using KeyMap = std::vector<std::string>;
using ItemCol = std::vector<Item>;

Item parse_item(const KeyMap &km) {
  Item ret;
  assert(km.size() == 7);
  if (km[0] == "#####") {
    ret.type = DType::Lock;
  } else if (km[6] == "#####") {
    ret.type = DType::Key;
  } else
    assert(false);

  for (int row = 1; row < km.size() - 1; ++row) {
    for (int col = 0; col < 5; ++col) {
      auto c = km[row][col];
      if (c == '#') {
        ret.data[col]++;
      } else {
        assert(c == '.');
      }
    }
  }

  return ret;
}

bool fit(const Item &key, const Item &lock) {
  assert(key.type == DType::Key);
  assert(lock.type == DType::Lock);

  for (int i = 0; i < 5; ++i) {
    if (key.data[i] + lock.data[i] > 5)
      return false;
  }
  return true;
}

int64_t solution_1(const ItemCol &ic) {
  int64_t ret = 0;

  for (int lock = 0; lock < ic.size(); ++lock) {
    for (int key = 0; key < ic.size(); ++key) {
      if (ic[key].type != DType::Key)
        continue;
      if (ic[lock].type != DType::Lock)
        continue;
      if (fit(ic[key], ic[lock]))
        ret++;
    }
  }

  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    KeyMap km = {"#####", ".####", ".####", ".####", ".#.#.", ".#...", "....."};
    auto item = parse_item(km);
    assert(item.type == DType::Lock);
    DataStore foo{0, 5, 3, 4, 3};
    assert(item.data == foo);
  }
  {
    KeyMap km = {".....", "#....", "#....", "#...#", "#.#.#", "#.###", "#####"};
    auto item = parse_item(km);
    assert(item.type == DType::Key);
    DataStore foo{5, 0, 2, 1, 3};
    assert(item.data == foo);
  }
  {
    Item key;
    key.type = DType::Key;
    key.data = {4, 3, 4, 0, 2};
    Item lock;
    lock.type = DType::Lock;
    lock.data = {0, 5, 3, 4, 3};
    assert(!fit(key, lock));
  }
  {
    Item key;
    key.type = DType::Key;
    key.data = {3, 0, 2, 0, 1};
    Item lock;
    lock.type = DType::Lock;
    lock.data = {0, 5, 3, 4, 3};
    assert(fit(key, lock));
  }

  int64_t sum = 0;

  std::ifstream is("basic.input");

  ItemCol ic;

  // Parse parse init registers
  while (std::cin) {
    KeyMap km;
    while (std::cin) {
      std::string line;
      std::getline(is, line);
      if (line.empty())
        break;

      km.push_back(line);
    }
    if (km.empty())
      break;
    auto item = parse_item(km);
    ic.push_back(item);
  }

  sum = solution_1(ic);

  std::cout << "Total score " << sum << std::endl;

  return 0;
}