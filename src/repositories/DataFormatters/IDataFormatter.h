#pragma once
#include <iostream>
#include <map>
#include <vector>
class IDataFormatter {
public:
  // Chuyển dữ liệu thô thành map giá trị
  virtual std::vector<std::map<std::string, std::string>>
  parse(const std::vector<std::string> &) const = 0;

  // Chuyển dữ liệu thành chuỗi định dạng
  virtual std::vector<std::string>
  format(const std::vector<std::map<std::string, std::string>> &,
         const std::vector<std::string> &) const = 0;

  virtual std::string toType() const = 0;

  virtual ~IDataFormatter() = default;
};
