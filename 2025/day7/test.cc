#include "test.h"

void Day7::run() {
  std::string text;
  std::fstream file(input);

  std::unordered_map<int, unsigned> timelines;

  // get the start
  getline(file, text);
  timelines[text.find('S')] = 1;

  // while in the manifold
  while (getline(file, text)) {
    std::unordered_map<int, unsigned> newTimelines;

    // check all the timelines
    for (std::pair<int, unsigned> const& tl : timelines) {
      // if they hit a splitter, add current timelines to left and right of
      // current.
      if (text[tl.first] == '^') {
        result1++;  // count how many times the timelines get split.
        newTimelines[tl.first - 1] += tl.second;
        newTimelines[tl.first + 1] += tl.second;
      } else  // no splitter, current timelines continue on.
      {
        newTimelines[tl.first] += tl.second;
      }
    }
    timelines = newTimelines;
  };
  file.close();

  // count how many timelines there are.

  // accumulate returns the type of its initial value, (b, e, initial value,
  // [](){}); so just a plain 0 will return int. 0ULL returns an unsigned long
  // long. 0.0 for double
  result2 = std::accumulate(
      timelines.begin(), timelines.end(), 0ULL,
      [](unsigned current_sum, std::pair<int, unsigned> const& tl) {
        return current_sum + tl.second;
      });

  /*
  //vs this easy for loop.
  for ( std::pair<int, unsigned __int64> const& tl : timelines) {
          result2 += tl.second;
  }
  */
  std::cout << "Day 7: " << result1 << " " << result2 << std::endl;
}

int main() {
  Day7 d;
  d.run();

  return 0;
}
