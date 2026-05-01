#include "utils.hpp"
#include <iostream>

namespace hepp {
void showProgressBar(const int &i, const int &total, const char *name) {

  if (i > total)
    return;

  double progress = i / (double)total;

  int barWidth = 40;

  std::cout << name << " [";

  int pos = static_cast<int>(barWidth * progress);

  for (int i = 0; i < barWidth; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }

  std::cout << "] " << i << "/" << total;

  std::cout << (i == total ? "\n" : "\r");

  std::cout.flush();
}

} // namespace hepp
