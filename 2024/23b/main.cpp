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

using Arcs = std::set<std::string>;
using Computers = std::map<std::string, Arcs>;

std::pair<std::string, std::string> parse_pair(const std::string &str) {
  std::stringstream ss(str);
  std::pair<std::string, std::string> ret;
  std::getline(ss, ret.first, '-');
  std::getline(ss, ret.second, '-');
  return ret;
}

using Loop = std::set<std::string>;
using Loops = std::set<Loop>;

int64_t solution_1(const Computers &computers) {
  int64_t sum = 0;
  Loops loops;

  for (const auto &c : computers) {
    if (c.first.starts_with("t")) {
      std::cout << "Testing: " << c.first << std::endl;

      for (const auto &a1 : c.second) {
        auto it1 = computers.find(a1);
        for (const auto &a2 : it1->second) {
          auto it2 = computers.find(a2);
          for (const auto &a3 : it2->second) {
            if (a3 == c.first) {
              Loop loop;
              loop.insert(c.first);
              loop.insert(a1);
              loop.insert(a2);
              loops.insert(loop);
            }
          }
        }
      }
    }
  }
  return loops.size();
}

struct QueueItem {
  int64_t cost{0};
  std::string start_computer;
  std::string next_computer;
  Loop history;

  bool operator<(const QueueItem &other) const {
    // < = largest first
    return cost < other.cost;
    // return pos.distance(finish) > other.pos.distance(finish);
  }
};

int64_t solution_2(const Computers &computers) {
  std::priority_queue<QueueItem> queue;

  for (const auto &c : computers) {
    QueueItem item;
    item.start_computer = c.first;
    item.cost = 1;
    for (const auto &n : c.second) {
      item.next_computer = n;
      item.history.insert(c.first);
      queue.push(item);
    }
  }

  Loop largest_loop;
  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();

    // Found loop
    if (current.start_computer == current.next_computer) {
      if (current.cost > largest_loop.size()) {
        largest_loop = current.history;
      }
      continue;
    }

    current.cost++;
    current.history.insert(current.next_computer);
    const auto &arcs = computers.find(current.next_computer)->second;
    for (const auto &arc : arcs) {
      if (current.history.find(arc) == current.history.end()) {
        current.next_computer = arc;
        queue.push(current);
      }
    }
  }

  return largest_loop.size();
}

using Set = std::set<std::string>;
using Sets = std::set<Set>;

void brute(const Computers &comps, Sets &sets, const std::string &current,
           const Set &must_have) {
  if (sets.find(must_have) != sets.end())
    return;

  sets.insert(must_have);
  for (const auto &neighbor : comps.find(current)->second) {
    if (must_have.find(neighbor) !=
        must_have.end()) // Avoid going back to where we ware
      continue;
    const auto &neighbor_outbound = comps.find(neighbor)->second;
    if (std::includes(neighbor_outbound.begin(), neighbor_outbound.end(),
                      must_have.begin(), must_have.end())) {
      auto must_have_next = must_have;
      must_have_next.insert(neighbor);
      brute(comps, sets, neighbor, must_have_next);
    }
  }
}

int64_t solution2b(const Computers &comps) {
  Sets sets;

  for (const auto &c : comps) {
    Set must_have;
    must_have.insert(c.first);
    brute(comps, sets, c.first, must_have);
  }
  auto largest = std::max_element(
      sets.begin(), sets.end(),
      [](const auto &a, const auto &b) { return a.size() < b.size(); });

  for (const auto &i : *largest) {
    std::cout << i << ",";
  }
  std::cout << std::endl;

  return largest->size();
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t sum = 0;

  {
    auto r = parse_pair("kh-tc");
    assert(r.first == "kh");
    assert(r.second == "tc");
  }

  // std::ifstream is("basic.input");
  std::ifstream is("basic.input");

  Computers computers;

  // Parse map
  while (std::cin) {
    std::string line;
    std::getline(is, line);
    if (line.empty())
      break;

    auto pair = parse_pair(line);

    computers[pair.first].insert(pair.second);
    computers[pair.second].insert(pair.first);
  }

  sum = solution2b(computers);

  std::cout << "Total score " << sum << std::endl;

  return 0;
}