#pragma once
#include "IDataFormatter.h"
#include <iostream>
#include <map>
#include <vector>
class JsonFormatter : public IDataFormatter {
public:
  std::vector<std::map<std::string, std::string>>
  parse(const std::vector<std::string> &) const override;

  std::vector<std::string>
  format(const std::vector<std::map<std::string, std::string>> &,
         const std::vector<std::string> &) const override;

  std::string toType() const override;
};
