#include "JsonFormatter.h"
#include "../../libs/json.hpp"
#include <sstream>

using json = nlohmann::json;

std::vector<std::map<std::string, std::string>>
JsonFormatter::parse(const std::vector<std::string> &rawLines) const {
  std::vector<std::map<std::string, std::string>> result;

  if (rawLines.empty())
    return result;

  // Nối chuối thành một dòng duy nhất
  std::ostringstream oss;
  for (const auto &line : rawLines) {
    oss << line;
  }
  std::string fullContent = oss.str();

  try {
    // parse chuỗi
    json jData = json::parse(fullContent);

    // labmda để chuyển 1 item Json thành map
    auto convertJsonToMap = [](const json &jItem) {
      std::map<std::string, std::string> record;
      for (auto &it : jItem.items()) {
        std::string key = it.key();
        auto value = it.value();

        if (value.is_string()) {
          record[key] = value.get<std::string>();
        } else if (value.is_array()) {
          // Xử lý array (đặc biệt cho borrowed)
          // Chuyển ["a", "b"] -> "[a; b]"
          std::string arrayStr = "[";
          for (size_t i = 0; i < value.size(); ++i) {
            if (value[i].is_string()) {
              arrayStr += value[i].get<std::string>();
            } else {
              arrayStr += value[i].dump();
            }
            if (i + 1 < value.size()) {
              arrayStr += "; ";
            }
          }
          arrayStr += "]";
          record[key] = arrayStr;
        } else {
          // Chuyển số thành chuỗi
          if (value.is_number_integer()) {
            record[key] = std::to_string(value.get<int>());
          } else if (value.is_number_float()) {
            record[key] = std::to_string(value.get<double>());
          } else {
            // dump các kiểu khác thành chuỗi
            record[key] = value.dump();
          }
        }
      }
      return record;
    };

    // Xử lý dữ liệu Json
    if (jData.is_array()) {
      for (const auto &jItem : jData) {
        if (jItem.is_object()) {
          result.push_back(convertJsonToMap(jItem));
        }
      }
    } else if (jData.is_object()) {
      result.push_back(convertJsonToMap(jData));
    }

  } catch (json::parse_error &e) {
    std::cerr << "[JsonFormatter] Loi cu phap: " << e.what() << std::endl;
  }

  return result;
}

std::vector<std::string> JsonFormatter::format(
    const std::vector<std::map<std::string, std::string>> &data,
    const std::vector<std::string> &orderedKeys) const {

  if (data.empty())
    return std::vector<std::string>();

  // Chuyển dữ liệu thành chuỗi định dạng Json
  json jArray = json::array();
  for (const auto &record : data) {
    json jRecord = json::object();

    // sắp xếp theo key đã cho
    for (const auto &key : orderedKeys) {
      if (record.find(key) != record.end()) {
        jRecord[key] = record.at(key);
      }
    }
    jArray.push_back(jRecord);
  }

  // tạo chuỗi cho vào kết quả
  std::vector<std::string> result;
  result.push_back(jArray.dump(2));

  return result;
}

std::string JsonFormatter::toType() const { return "JSON"; }