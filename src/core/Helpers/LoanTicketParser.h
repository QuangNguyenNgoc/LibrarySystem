#pragma once
#include "../../repositories/Helpers/StringHelper.h"
#include "../LoanTicket.h"
#include <string>
#include <vector>

namespace Helpers {
namespace LoanTicketParser {
inline std::vector<LoanTicket> parseCompact(const std::string &borrowedStr) {
  std::vector<LoanTicket> loans;

  // Kiểm tra chuỗi rỗng hoặc chỉ có []
  if (borrowedStr.empty())
    return loans;

  std::string content = borrowedStr;

  // Loại bỏ dấu [ và ]
  if (content.size() >= 2 && content.front() == '[' && content.back() == ']') {
    content = content.substr(1, content.size() - 2);
  }

  content = StringHelper::trim(content);
  if (content.empty())
    return loans;

  // Tách từng ticket theo dấu ;
  std::vector<std::string> tickets = StringHelper::split(content, ";");

  for (const auto &ticketStr : tickets) {
    if (ticketStr.empty())
      continue;

    // Tách các thành phần theo dấu |
    std::vector<std::string> parts = StringHelper::split(ticketStr, "|");

    if (parts.size() >= 3) {
      try {
        std::string bookId = parts[0];
        std::string borrowDate = parts[1];
        int loanDay = std::stoi(parts[2]);
        loans.emplace_back(bookId, borrowDate, loanDay);
      } catch (const std::exception &) {
        // Bỏ qua ticket lỗi
        continue;
      }
    }
  }

  return loans;
}

inline std::string formatCompact(const std::vector<LoanTicket> &loans) {
  std::string result = "[";

  for (size_t i = 0; i < loans.size(); ++i) {
    const auto &loan = loans[i];
    result += loan.getBookId() + "|" + loan.getBorrowDate() + "|" +
              std::to_string(loan.getLoanDay());

    if (i + 1 < loans.size()) {
      result += "; ";
    }
  }

  result += "]";
  return result;
}
} // namespace LoanTicketParser
} // namespace Helpers
