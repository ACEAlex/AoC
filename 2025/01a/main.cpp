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

int rotate(int pos, std::string op) {
  auto v = std::atoi(op.c_str() + 1);
  switch (op[0]) {

  case 'L':
    pos = (pos - v) % 100;
    if (pos < 0)
      pos += 100;
    return pos;
  case 'R':
    return (pos + v) % 100;
  default:
    assert(false);
  }
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int position = 50;

  assert(rotate(50, "L2") == 48);
  assert(rotate(50, "L50") == 0);
  assert(rotate(50, "L51") == 99);
  assert(rotate(50, "R2") == 52);
  assert(rotate(50, "R100") == 50);

  int score = 0;
  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    position = rotate(position, input);
    if (position == 0)
      score++;
  }

  std::cout << "Position " << position << std::endl;
  std::cout << "Score " << score << std::endl;

  return 0;
}