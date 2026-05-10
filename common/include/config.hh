#pragma once

#include <toml.hpp>
namespace hepp {

class Config {
protected:
  Config() { result_ = toml::parse_file("resources/configuration.toml"); }

  static Config *config_;
  toml::parse_result result_;

public:
  Config(Config &other) = delete;
  void operator=(const Config &) = delete;
  static toml::parse_result *GetConfig();
  toml::parse_result config() const { return result_; }
};

} // namespace hepp
