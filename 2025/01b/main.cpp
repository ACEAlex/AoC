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

inline int positive_mod(int v, int m) {
    return (v % m + m) % m;
}

int rotate(int pos, std::string op, int& score)
{
  auto v = std::atoi(op.c_str()+1);
  int inc = 0;

  if (op[0] == 'L')
    inc = -1;
  else if (op[0] == 'R')
    inc = 1;
  else
    assert(false);

    while (v > 0)
    {
      pos += inc;
      pos = positive_mod(pos, 100);
      if (pos == 0)
        score++;
      v--;
    }
    return pos;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int position = 50;
  int d = 0;

  std::cout << rotate(50, "L50", d)  << " d " << d << std::endl; 
  assert(rotate(50, "L50", d) == 0);
  assert(rotate(50, "L51", d) == 99);
  assert(rotate(50, "R2", d) == 52);
  assert(rotate(50, "R100", d) == 50);

  int score = 0;
  int score2 = 0;
  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    position = rotate(position, input, score2);
    //std::cout << input << "** " << position << " " << score2 << std::endl;
    if (position == 0)
      score++;
  }

  std::cout << "Position " << position << std::endl;
  std::cout << "Score " << score << std::endl;
  std::cout << "Score2 " << score2 << std::endl;

  return 0;
}