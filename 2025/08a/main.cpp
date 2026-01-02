#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
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

using Map = std::vector<std::string>;

struct Circuit;

struct Pos {
  int x;
  int y;
  int z;
  bool operator<(const Pos &other) const {
    return std::tie(x, y, z) < std::tie(x, y, z);
  }

  Circuit *inCircuit = nullptr;
};

std::ostream &operator<<(std::ostream &os, const Pos &p) {
  os << p.x << "," << p.y << "," << p.z;
  return os;
}

using PositionPair = std::pair<Pos *, Pos *>;

struct Circuit {
  std::vector<Pos *> connection;
};

double distance(Pos a, Pos b) {
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  double dz = a.z - b.z;
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

std::unique_ptr<Pos> parse_pos(const std::string &str) {
  std::vector<std::string> v;
  boost::split(v, str, boost::is_any_of(","), boost::token_compress_on);
  return std::make_unique<Pos>(atoi(v[0].c_str()), atoi(v[1].c_str()),
                               atoi(v[2].c_str()));
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  uint64_t score = 0;

  {
    auto p = parse_pos("123,456,789");
    assert(p->x == 123);
    assert(p->y == 456);
    assert(p->z == 789);
  }

  std::vector<std::unique_ptr<Pos>> positions; // Heap = memory stable
  std::vector<PositionPair> pospairs;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    auto pos = parse_pos(input);

    // Make it a pair of every already read postions
    for (const auto &i : positions) {
      pospairs.push_back({pos.get(), i.get()});
    }
    positions.push_back(std::move(pos));
  }

  std::cout << "Number of pospairs " << pospairs.size() << std::endl;

  // Sort positions just to get everything in size sort
  std::sort(pospairs.begin(), pospairs.end(), [](const auto &a, const auto &b) {
    return distance(*a.first, *a.second) < distance(*b.first, *b.second);
  });

  for (const auto &i : pospairs) {
    std::cout << "Dist: " << distance(*i.first, *i.second) << " " << *i.first
              << " " << *i.second << " " << std::endl;
  }

  std::vector<std::unique_ptr<Circuit>> circuits;

  // Pospairs is sorted by distance. So we just grab the number of pairs needed
  int i = 0;
  for (auto &[a, b] : pospairs) {

    // Both are unconnected
    if (a->inCircuit == nullptr && b->inCircuit == nullptr) {
      auto circ = std::make_unique<Circuit>();
      a->inCircuit = circ.get();
      b->inCircuit = circ.get();
      circ->connection.push_back(a);
      circ->connection.push_back(b);
      circuits.push_back(std::move(circ));
    }

    // first is connected but not second
    else if (a->inCircuit && !b->inCircuit) {
      b->inCircuit = a->inCircuit;
      a->inCircuit->connection.push_back(b);
    }

    // Second is connected but not first
    else if (!a->inCircuit && b->inCircuit) {
      // Register second in first
      a->inCircuit = b->inCircuit;
      b->inCircuit->connection.push_back(a);
    }

    // Both are connected. Join circut
    else if (a->inCircuit && b->inCircuit && a->inCircuit != b->inCircuit) {
      auto to_empty = a->inCircuit;
      auto into = b->inCircuit;
      for (auto &p : to_empty->connection) {
        into->connection.push_back(p);
        p->inCircuit = into;
      }
      to_empty->connection.clear();
    }
    ++i;
    if (i == 1000)
      break;
  }

  // Calculate score
  std::sort(circuits.begin(), circuits.end(), [](const auto &a, const auto &b) {
    return a->connection.size() > b->connection.size();
  });

  score = 1;
  for (int i = 0; i < 3; ++i) {
    std::cout << "S: " << circuits[i]->connection.size() << std::endl;
    score *= circuits[i]->connection.size();
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}