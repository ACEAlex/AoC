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

bool is_digit(char c) { return c >= '0' && c <= '9'; }

int64_t calc_mul(const char *str) {
  // check that there is enough in str. Smallest possible is "mul(a,b)" = 8
  if (strlen(str) < 8)
    return -1;
  // check if str starts with mul(
  if (strncmp("mul(", str, 4) != 0)
    return -1;

  str += 4;
  int64_t d1 = 0, d2 = 0;
  {
    int digits = 0;

    while (is_digit(str[digits]))
      ++digits;
    if (digits < 1 || digits > 3 || str[digits] != ',')
      return -1;
    char tmp[4];
    memset(tmp, 0, 4);
    memcpy(tmp, str, digits);
    d1 = atoi(tmp);
    str += digits + 1; // digits + ,
  }
  {
    int digits = 0;

    while (is_digit(str[digits]))
      ++digits;
    if (digits < 1 || digits > 3 || str[digits] != ')')
      return -1;
    char tmp[4];
    memset(tmp, 0, 4);
    memcpy(tmp, str, digits);
    d2 = atoi(tmp);
    str += digits + 1; // digits + ,
  }

  return d1 * d2;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  {
    assert(is_digit('0'));
    assert(is_digit('1'));
    assert(is_digit('8'));
    assert(is_digit('9'));
    assert(!is_digit('a'));
    assert(!is_digit('!'));
  }

  {
    auto str = "mul(123,4)";
    assert(calc_mul(str) == 123 * 4);
  }

  {
    auto str = "mul(44,46)";
    assert(calc_mul(str) == 44 * 46);
  }

  {
    auto str = "mul(4*";
    assert(calc_mul(str) == -1);
  }

  {
    auto str = "mul(6,9!";
    assert(calc_mul(str) == -1);
  }

  {
    auto str = "?(12,34)";
    assert(calc_mul(str) == -1);
  }

  {
    auto str = "mul(2,4)";
    assert(calc_mul(str) == 2 * 4);
  }

  int64_t sum = 0;

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    auto str = input.c_str();
    while (*str != '\0') {
      // std::cout << *str;
      auto i = calc_mul(str);
      if (i != -1) {
        std::cout << "Found a value: " << i << std::endl;
        sum += i;
      }
      str++;
    }
  }

  std::cout << "\n";

  std::cout << "Score " << sum << std::endl;

  return 0;
}