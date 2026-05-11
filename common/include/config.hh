#pragma once
#include <toml.hpp>

namespace hepp {

class Config {
private:
  Config() { result_ = toml::parse_file("resources/configuration.toml"); }

  toml::parse_result result_;

public:
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  static toml::parse_result *GetConfig();
};

} // namespace hepp
