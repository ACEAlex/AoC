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
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

int64_t mix(int64_t secret, int64_t nbr) { return secret ^ nbr; }

int64_t prune(int64_t secret) { return secret % 16777216; }

int64_t evolve(int64_t secret) {
  {
    auto a = secret * 64;
    secret = mix(secret, a);
    secret = prune(secret);
  }
  {
    auto b = secret / 32;
    secret = mix(secret, b);
    secret = prune(secret);
  }
  {
    auto c = secret * 2048;
    secret = mix(secret, c);
    secret = prune(secret);
  }
  return secret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    auto n = mix(42, 15);
    assert(n == 37);
  }

  {
    auto n = prune(100000000);
    assert(n == 16113920);
  }

  {
    auto a = evolve(123);
    assert(a == 15887950);
    a = evolve(a);
    assert(a == 16495136);
    a = evolve(a);
    assert(a == 527345);
    a = evolve(a);
    assert(a == 704524);
    a = evolve(a);
    assert(a == 1553684);
    a = evolve(a);
    assert(a == 12683156);
    a = evolve(a);
    assert(a == 11100544);
    a = evolve(a);
    assert(a == 12249484);
    a = evolve(a);
    assert(a == 7753432);
    a = evolve(a);
    assert(a == 5908254);
  }

  // std::ifstream is("basic.input");
  std::ifstream is("basic.input");

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    auto init_nbr = atol(line.c_str());
    auto secret = init_nbr;
    for (int i = 0; i < 2000; ++i) {
      secret = evolve(secret);
    }
    std::cout << init_nbr << ": " << secret << std::endl;
    sum += secret;
  }

  std::cout << "Total score " << sum << std::endl;

  return 0;
}