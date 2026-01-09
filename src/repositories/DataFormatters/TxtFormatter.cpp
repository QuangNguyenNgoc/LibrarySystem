#include "TxtFormatter.h"

std::vector<std::map<std::string, std::string>>
TxtFormatter::parse(const std::vector<std::string> &rawLines) const {
  // chuyển hóa dữ liệu dòng thô thành map giá trị.
  std::vector<std::map<std::string, std::string>> result;

  if (rawLines.empty()) {
    return result;
  }

  for (const auto &line : rawLines) {
    std::vector<std::string> datas = Helpers::StringHelper::split(line, ",");
    std::map<std::string, std::string> keyData;

    for (const auto &data : datas) {
      std::vector<std::string> keyValue =
          Helpers::StringHelper::split(data, "=");

      std::string key = keyValue[0];
      std::string value = (keyValue.size() > 1) ? keyValue[1] : "";
      keyData[key] = value;
    }
    result.push_back(keyData);
  }

  return result;
}

std::vector<std::string> TxtFormatter::format(
    const std::vector<std::map<std::string, std::string>> &data,
    const std::vector<std::string> &orderedKeys) const {

  std::vector<std::string> result;

  if (data.empty())
    return result;

  // Tạo các dòng dữ liệu
  for (const auto &mapData : data) {
    std::string line;
    for (const auto &key : orderedKeys) {
      line += key + "=";
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

std::string TxtFormatter::toType() const { return "TXT"; }