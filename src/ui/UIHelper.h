#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

namespace UI {
namespace UIHelper {

// Chia nhỏ văn bản thành nhiều dòng nếu quá dài
inline std::vector<std::string> splitTextIntoLines(const std::string &text, size_t maxLength) {
    std::vector<std::string> lines;
    if (text.empty()) {
        lines.push_back("");
        return lines;
    }

    for (size_t i = 0; i < text.length(); i += maxLength) {
        lines.push_back(text.substr(i, maxLength));
    }
    return lines;
}

// In một dòng trong bảng (Đầy đủ: ID | Ten | SL | Con)
inline void drawTableRow(const std::string &id, const std::string &title, int quantity, int available, 
                        int idWidth = 6, int titleWidth = 33, int qtyWidth = 4) {
    auto titleLines = splitTextIntoLines(title, titleWidth - 2);
    
    printf("  %-*s | %-*s | %*d | %*d\n", 
           idWidth - 2, id.c_str(), 
           titleWidth - 2, titleLines[0].c_str(), 
           qtyWidth - 1, quantity, 
           qtyWidth - 1, available);
           
    for (size_t i = 1; i < titleLines.size(); ++i) {
        printf("  %-*s | %-*s | %*s | %*s\n", 
               idWidth - 2, "", 
               titleWidth - 2, titleLines[i].c_str(), 
               qtyWidth - 1, "", 
               qtyWidth - 1, "");
    }
}

// In một dòng trong bảng (Rút gọn: ID | Ten | Con lai) - Dành cho User
inline void drawSimpleTableRow(const std::string &id, const std::string &title, int available, 
                              int idWidth = 6, int titleWidth = 35) {
    auto titleLines = splitTextIntoLines(title, titleWidth - 2);
    
    printf("  %-*s | %-*s | %d\n", 
           idWidth - 2, id.c_str(), 
           titleWidth - 2, titleLines[0].c_str(), 
           available);
           
    for (size_t i = 1; i < titleLines.size(); ++i) {
        printf("  %-*s | %-*s |\n", 
               idWidth - 2, "", 
               titleWidth - 2, titleLines[i].c_str());
    }
}

// In dòng kẻ ngang cho bảng
inline void drawTableLine(int totalWidth = 55) {
    std::cout << "  " << std::string(totalWidth, '-') << "\n";
}

} // namespace UIHelper
} // namespace UI
