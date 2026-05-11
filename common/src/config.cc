#include "config.hh"

namespace hepp {

// No need for the pointer initialization here if we use the local static
// approach Config *Config::config_ = nullptr;

toml::parse_result *Config::GetConfig() {
  // This is thread-safe in C++11 and later
  static Config instance;
  return &instance.result_;
}

} // namespace hepp
