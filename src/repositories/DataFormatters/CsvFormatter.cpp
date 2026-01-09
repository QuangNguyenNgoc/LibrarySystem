#include "CsvFormatter.h"

std::vector<std::map<std::string, std::string>>
CsvFormatter::parse(const std::vector<std::string> &rawLines) const {
  // chuyển hóa dữ liệu dòng thô thành map giá trị.
  std::vector<std::map<std::string, std::string>> result;

  if (rawLines.empty())
    return result;

  // lấy dòng đầu tiên là kiểu dữ liệu (keys)
  std::vector<std::string> keys =
      Helpers::StringHelper::split(rawLines[0], ",");

  // duyệt các dòng dữ liệu tiếp theo (values)
  for (size_t i = 1; i < rawLines.size(); i++) {
    std::string line = rawLines[i];
    std::vector<std::string> values = Helpers::StringHelper::split(line, ",");

    std::map<std::string, std::string> keyData;
    for (size_t j = 0; j < keys.size(); j++) {
      std::string key = keys[j];
      std::string value = (j < values.size()) ? values[j] : "";
      keyData[key] = value;
    }
    result.push_back(keyData);
  }
  return result;
}

std::vector<std::string> CsvFormatter::format(
    const std::vector<std::map<std::string, std::string>> &data,
    const std::vector<std::string> &orderedKeys) const {
  std::vector<std::string> result;

  if (data.empty())
    return result;

  // Tạo header
  std::string header;
  for (const auto &key : orderedKeys) {
    header += key;
    if (key != orderedKeys.back()) {
      header += ",";
    }
  }
  result.push_back(header);

  // Tạo các dòng dữ liệu
  for (const auto &mapData : data) {
    std::string line;
    for (const auto &key : orderedKeys) {
      if (mapData.find(key) != mapData.end()) {
        line += mapData.at(key);
      }
      if (key != orderedKeys.back()) {
        line += ",";
      }
    }
    result.push_back(line);
  }

  return result;
}

std::string CsvFormatter::toType() const { return "CSV"; }