#include "utils.hpp"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>

namespace hepp {
void showProgressBar(const int &i, const int &total, const char *name) {
  static auto start = std::chrono::steady_clock::now();

  if (i == 0)
    start = std::chrono::steady_clock::now();

  if (i > total)
    return;

  auto now = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
  int eta = 0;

  if (i > 0) {
    eta = (elapsed.count() / (double)i) * (total - i);
  }

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

  std::cout << " | Elapsed: " << elapsed.count() << " seconds";

  std::cout << std::setprecision(0);

  std::cout << " | ETA: " << eta / (double)60 << " minutes";

  std::cout << (i == total ? "\r" : "\r");

  std::cout.flush();
}

/* NOTE: Pythia needs the PYTHIA8DATA, but that variable is not set when Pythia
 * is installed with Conda. This function sets it. */
void ensurePythiaData() {

  // User set it.
  if (const char *env = std::getenv("PYTHIA8DATA")) {
    if (std::filesystem::exists(env)) {
      return;
    } else {
      std::cerr << "[ERROR] PYTHIA8DATA is set but invalid: " << env << "\n";
      std::exit(1);
    }
  }

  // Setting it from CONDA_PREIX
  const char *conda = std::getenv("CONDA_PREFIX");
  if (conda) {
    std::string path = std::string(conda) + "/share/Pythia8/xmldoc";

    if (std::filesystem::exists(path)) {
      setenv("PYTHIA8DATA", path.c_str(), 1);

      std::cout << "[INFO] PYTHIA8DATA auto-set to: " << path << std::endl;
      return;
    }
  }

  std::cerr << "[ERROR] PYTHIA8DATA not set and could not be inferred.\n"
               "Expected location:\n"
               "  $CONDA_PREFIX/share/Pythia8/xmldoc\n\n"
               "Fix:\n"
               "  conda activate your-env\n"
               "  export PYTHIA8DATA=$CONDA_PREFIX/share/Pythia8/xmldoc\n";

  std::exit(1);
}

} // namespace hepp
