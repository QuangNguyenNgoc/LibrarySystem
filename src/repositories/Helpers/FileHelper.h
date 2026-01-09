#pragma once
#include "../../Core/Enums/Enum.h"
#include "./StringHelper.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Helpers {
namespace FileHelper {

// Lấy các file trong thư mục
inline std::vector<std::string>
getFilesInDirectory(const std::string &directoryPath) {
  std::vector<std::string> fileList;

  // Kiểm tra thư mục tồn tại
  if (!std::filesystem::exists(directoryPath) ||
      !std::filesystem::is_directory(directoryPath)) {
    return fileList; // trả về rỗng nếu không tồn tại
  }

  for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
    // Chỉ duyệt file
    if (entry.is_regular_file()) {
      fileList.push_back(entry.path().filename().string());
    }
  }
  return fileList;
}

// Hàm lấy format file từ path
inline std::string getFileExtension(const std::string &filePath) {
  // Lấy định dạng file (lấy từ dấu . cuối cùng)
  size_t dotPos = filePath.find_last_of(".");
  if (dotPos != std::string::npos) {
    return filePath.substr(dotPos);
  }
  return "";
}

// Hàm lấy tên file từ path
inline std::string getFileName(const std::string &filePath) {
  return std::filesystem::path(filePath).filename().string();
}

// Hàm trả về kiểu enum FileType từ path
inline FileType getFileType(const std::string &filePath) {
  std::string ext = getFileExtension(filePath);
  ext = StringHelper::toLower(ext);

  if (ext == ".txt")
    return FileType::TXT;
  if (ext == ".csv")
    return FileType::CSV;
  if (ext == ".json")
    return FileType::JSON;
  if (ext == ".xml")
    return FileType::XML;

  return FileType::UNKNOWN;
}

// Kiểm tra format file được hỗ trợ
inline bool isSupported(const FileType &type) {
  return type != FileType::UNKNOWN;
}

// Phân loại dữ liệu dựa trên tên file (b* -> BOOKS, u* -> USERS)
inline DataCategory identifyDataCategory(const std::string &filename) {
  if (filename.empty()) return DataCategory::UNKNOWN;
  
  char firstChar = std::tolower(filename[0]);
  if (firstChar == 'b') return DataCategory::BOOKS;
  if (firstChar == 'u') return DataCategory::USERS;
  
  return DataCategory::UNKNOWN;
}

// Xóa nội dung toàn bộ file
inline void clearFile(const std::string &filePath) {
  std::ofstream file(filePath, std::ios::trunc);
  file.close();
}

// * Đọc nội dung file, trả về rawDatas dưới dạng mảng string
inline std::vector<std::string> readFileLines(const std::string &filePath) {
  std::vector<std::string> lines;
  std::ifstream file(filePath);

  if (!file.is_open()) {
    return lines; // trả về rỗng nếu không mở được file
  }

  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  file.close();
  return lines;
}

// * Ghi đè dữ liệu dạng mảng string vào file
inline void writeFileLines(const std::string &filePath,
                           const std::vector<std::string> &lines) {
  std::ofstream file(filePath, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    return;
  }

  for (size_t i = 0; i < lines.size(); ++i) {
    file << lines[i];
    if (i + 1 < lines.size()) {
      file << '\n';
    }
  }
  file.close();
}
} // namespace FileHelper
} // namespace Helpers