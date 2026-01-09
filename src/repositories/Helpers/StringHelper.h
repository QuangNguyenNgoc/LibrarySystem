#pragma once
#include <iostream>
#include <vector>

namespace Helpers {
namespace StringHelper {
inline std::string toUpper(const std::string &str) {
  std::string result = str;
  for (char &c : result) {
    c = toupper(c);
  }
  return result;
}

inline std::string toLower(const std::string &str) {
  std::string result = str;
  for (char &c : result) {
    c = tolower(c);
  }
  return result;
}

// hàm loại bỏ khoảng trắng thừa xung quanh data
inline std::string trim(const std::string &str) {
  size_t first = str.find_first_not_of(" \t\n\r");

  // kiểm tra chuỗi ko có space, hoặc toàn space
  if (first == std::string::npos) {
    return str;
  }

  size_t end = str.find_last_not_of(" \t\n\r");
  return str.substr(first, (end - first + 1));
}

// hàm cắt chuỗi theo needle
inline std::vector<std::string> split(const std::string &haystack,
                                      const std::string needle) {
  std::vector<std::string> result;
  int startPos = 0;
  size_t foundPos = haystack.find(needle, startPos);

  while (foundPos != std::string::npos) {
    int count = foundPos - startPos;
    std::string token = trim(haystack.substr(startPos, count));

    result.push_back(token);
    startPos = foundPos + needle.length();
    foundPos = haystack.find(needle, startPos);
  }

  std::string token =
      trim(haystack.substr(startPos, haystack.length() - startPos));
  result.push_back(token);
  return result;
}
} // namespace StringHelper
} // namespace Helpers