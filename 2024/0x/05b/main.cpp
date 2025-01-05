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

std::pair<int, int> parse_first(const std::string &str) {
  std::pair<int, int> a;
  auto r = sscanf(str.c_str(), "%d|%d", &a.first, &a.second);
  assert(r == 2);
  return a;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;
  int64_t sum2 = 0;

  {
    auto p = parse_first("22|24");
    assert(p.first == 22);
    assert(p.second == 24);
  }

  std::map<int, std::set<int>> rules;

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    auto t = parse_first(input);

    rules[t.first].insert(t.second);
  }

  while (std::cin) {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
      break;

    std::stringstream ss(input);
    std::string str;
    std::vector<int> update;
    while (getline(ss, str, ',')) {
      if (str.empty())
        break;
      int a = atoi(str.c_str());
      update.push_back(a);
    }

    bool correct = true;
    for (auto it = update.begin(); it != update.end(); ++it) {
      // foreach update
      // find the rule --> check if everything in the update is registered as
      // following std::cout << "Testing: " << *it << "\n";
      auto rit = rules.find(*it);
      if (rit == rules.end()) {
        if (it + 1 == update.end())
          continue;
        else {
          // Breaking rules
          correct = false;
          break;
        }
      }
      assert(rit != rules.end());
      auto nit = it;
      auto good = true;
      while (++nit != update.end()) {
        if (rit->second.find(*nit) == rit->second.end()) {
          good = false;
          break;
        }
      }
      if (!good) {
        correct = false;
        break;
      }
    }

    if (correct) {
      // std::cout << "Is correct: " << input << std::endl;
      sum += update[update.size() / 2];
    } else {
      std::cout << "Is not correct: " << input << std::endl;
      std::map<int, int> aftermap; // first = number,numbers after
      for (auto i : update) {
        auto afters = std::count_if(
            rules.begin(), rules.end(), [&i, &update](const auto &in) {
              if (std::find(update.begin(), update.end(), in.first) ==
                  update.end())
                return false;
              return in.second.find(i) != in.second.end();
            });
        aftermap[afters] = i;
      }
      std::vector<int> in_v;
      std::cout << "  Should be: ";
      for (auto i : aftermap) {
        in_v.push_back(i.second);
        std::cout << i.second << " ";
      }
      auto score2 = in_v[in_v.size() / 2];
      std::cout << " score: " << score2 << std::endl;
      sum2 += score2;
    }
  }

  std::cout << "Score " << sum2 << std::endl;

  return 0;
}