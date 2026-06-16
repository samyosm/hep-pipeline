#include "config.hh"
#include <iostream>

int main(int argc, char **argv) {
  auto &config = *hepp::Config::GetConfig();
  int threadCount = config["threads"].value_or(1);

  std::cout << "Thread count: " << threadCount << std::endl;

  return 0;
}
