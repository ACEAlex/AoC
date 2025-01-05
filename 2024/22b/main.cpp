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

using Sequence = std::tuple<int, int, int, int>;
using MonkeyId = int;
using Bananas = int64_t;

using MonkeyMap = std::map<Sequence, std::map<MonkeyId, Bananas>>;

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

int64_t find_highest_score(const MonkeyMap &mm) {
  int64_t best = 0;
  for (const auto &i : mm) {
    int64_t sum = 0;
    for (const auto &s : i.second) {
      sum += s.second;
    }
    if (sum > best)
      best = sum;
  }

  return best;
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

  MonkeyMap mm;

  MonkeyId monkey_nbr;

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    auto init_nbr = atol(line.c_str());
    int64_t secret_history[4] = {init_nbr, 0, 0, 0};
    secret_history[1] = evolve(secret_history[0]);
    secret_history[2] = evolve(secret_history[1]);
    secret_history[3] = evolve(secret_history[2]);
    monkey_nbr++;

    int64_t last_value[4] = {secret_history[0] % 10, secret_history[1] % 10,
                             secret_history[2] % 10, secret_history[3] % 10};
    int64_t delta[4] = {0, last_value[1] - last_value[0],
                        last_value[2] - last_value[1],
                        last_value[3] - last_value[2]};

    for (int i = 4; i <= 2000; ++i) {
      secret_history[i % 4] = evolve(secret_history[(i - 1) % 4]);
      last_value[i % 4] = secret_history[i % 4] % 10;
      delta[i % 4] = last_value[i % 4] - last_value[(i + 3) % 4];
      Sequence seq{delta[(i - 3) % 4], delta[(i - 2) % 4], delta[(i - 1) % 4],
                   delta[(i - 0) % 4]};
      auto &seqres = mm[seq];
      auto it = seqres.find(monkey_nbr);
      if (it == seqres.end())
        seqres.insert({monkey_nbr, last_value[i % 4]});
    }

    std::cout << init_nbr << ": " << secret_history[2000 % 4] << std::endl;
    sum += secret_history[2000 % 4];
  }

  auto sum2 = find_highest_score(mm);

  std::cout << "Total score " << sum2 << std::endl;

  return 0;
}