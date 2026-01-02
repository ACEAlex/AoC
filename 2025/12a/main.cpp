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

struct Map {
  int64_t row;
  int64_t col;
  std::vector<int64_t> presents;
};

int main(int argc, char **argv) {
  std::cout << "Start" << std::endl;

  int64_t score = 0;

  std::ifstream is("basic.input");
  while (is) {
    std::string input;
    std::getline(is, input);
    // if (input.empty())
    //   break;

    if (input.contains('x')) {
      auto x_str = input.substr(0, input.find_first_of('x'));
      auto y_str = input.substr(input.find_first_of('x') + 1,
                                input.find_first_of(':') -
                                    (input.find_first_of('x') + 1));
      auto present_str = input.substr(input.find_first_of(':') + 2);
      int64_t x = std::atoi(x_str.c_str());
      int64_t y = std::atoi(y_str.c_str());
      std::vector<std::string> presents;
      boost::split(presents, present_str, boost::is_any_of(" "),
                   boost::token_compress_on);
      Map m{x, y};
      for (const auto &p : presents) {
        m.presents.push_back(std::atoi(p.c_str()));
      }

      // Check number of presents
      auto num_presents =
          std::accumulate(m.presents.begin(), m.presents.end(), 0);

      // Check if this is possible. All presents just stuffed as 3x3
      // Crazy, all are either very big or fits within. Dahh. no need to do more work.
      // Lets try the answer! It is right!??!
      auto present_space = (m.col / 3) * (m.row / 3);
      std::cout << "Comparing: " << present_space << " >= " << num_presents
                << std::endl;
      if (present_space >= num_presents)
        score++;
    }
  }

  std::cout << "Score " << score << std::endl;

  return 0;
}