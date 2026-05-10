#include "config.hh"
#include "toml.hpp"

namespace hepp {

Config *Config::config_ = nullptr;

toml::parse_result *Config::GetConfig() {
  if (config_ == nullptr) {
    config_ = new Config();
  }

  return &config_->result_;
}

} // namespace hepp
