#include "boost/algorithm/string/split.hpp"
#include <algorithm>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
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

struct Node {
  std::string name;
  std::vector<std::string> lines;
  bool visited{false};

  bool operator<(const Node &other) const { return name < other.name; }
};

struct CacheKey {
  std::string name;
  bool dac;
  bool fft;
  bool operator<(const CacheKey &other) const {
    return std::tie(name, dac, fft) <
           std::tie(other.name, other.dac, other.fft);
  }
};

using Nodes = std::map<std::string, Node>;
using Cache = std::map<CacheKey, int64_t>;

Node parse_node(const std::string &str) {
  Node ret;
  ret.name = str.substr(0, 3);

  auto connections_str = str.substr(5);
  boost::split(ret.lines, connections_str, boost::is_any_of(" "),
               boost::token_compress_on);

  return ret;
}

int64_t part_2(Nodes &nodes, const std::string &current, bool dac, bool fft,
               Cache &cache) {
  if (current == "out") {
    if (dac && fft) {
      static int64_t count = 0;
      std::cout << "Wahoo " << count++ << std::endl;
      return 1;
    } else
      return 0;
  }

  if (auto it = cache.find({current, dac, fft}); it != cache.end()) {
    return it->second;
  }

  if (current == "dac")
    dac = true;

  if (current == "fft")
    fft = true;

  auto &current_node = nodes.find(current)->second;

  // if (current_node.visited)
  //  return 0;

  // current_node.visited = true;

  int64_t ret = 0;
  for (auto &n : current_node.lines) {
    ret += part_2(nodes, n, dac, fft, cache);
  }
  // current_node.visited = false;
  cache.insert({{current, dac, fft}, ret});
  return ret;
}

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t score = 0;

  {
    auto str = "ccc: ddd eee fff";
    auto n = parse_node(str);
    // std::cout << "l = " << l << std::endl;
    assert(n.name == "ccc");
    assert(n.lines.size() == 3);
    assert(n.lines[0] == "ddd");
    assert(n.lines[1] == "eee");
    assert(n.lines[2] == "fff");
  }

  Nodes nodes;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    if (input.empty())
      break;

    auto node = parse_node(input);
    nodes.insert({node.name, node});
  }

  Cache cache;

  score = part_2(nodes, "svr", false, false, cache);

  std::cout << "Score " << score << std::endl;

  return 0;
}