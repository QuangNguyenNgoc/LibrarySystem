#pragma once
#include "../../libs/tinyxml2.h"
#include "IDataFormatter.h"
#include <iostream>
#include <map>
#include <vector>

class XmlFormatter : public IDataFormatter {
public:
  std::vector<std::map<std::string, std::string>>
  parse(const std::vector<std::string> &) const override;

  std::vector<std::string>
  format(const std::vector<std::map<std::string, std::string>> &,
         const std::vector<std::string> &) const override;

  std::string toType() const override;
};