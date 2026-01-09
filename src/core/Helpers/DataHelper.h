#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace Helpers {
namespace DateHelper {
// Lấy ngày hiện tại theo định dạng tùy chọn (Mặc định: dd/mm/yyyy)
inline std::string getCurrentDate(const std::string &format = "%d/%m/%Y") {
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);

  // chuyển thành chuỗi theo định dạng
  std::ostringstream oss;
  oss << std::put_time(&tm, format.c_str());

  return oss.str();
}
}; // namespace DateHelper
} // namespace Helpers