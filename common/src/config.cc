#include "config.hh"

namespace hepp {

Config *Config::config_ = nullptr;

Config *Config::GetConfig() {
  if (config_ == nullptr) {
    config_ = new Config();
  }

  return config_;
}

} // namespace hepp
