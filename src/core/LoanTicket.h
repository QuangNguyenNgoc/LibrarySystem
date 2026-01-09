#pragma once
#include "./Helpers/DataHelper.h"
#include "./Object.h"
#include <iostream>

class LoanTicket : public Object {
private:
  std::string _bookId;
  std::string _borrowDate;
  int _loanDay;

public:
  // Constructor: lấy ngày hiện tại làm ngày mượn
  LoanTicket(const std::string &bookId, int loanDay)
      : _bookId(bookId),
        _borrowDate(Helpers::DateHelper::getCurrentDate("%Y-%m-%d")),
        _loanDay(loanDay) {}

  // Constructor: sử dụng ngày đã có sẵn
  LoanTicket(const std::string &bookId, const std::string &borrowDate,
             int loanDay)
      : _bookId(bookId), _borrowDate(borrowDate), _loanDay(loanDay) {}

  // getters
  std::string getBookId() const { return _bookId; }

  std::string getBorrowDate() const { return _borrowDate; }

  int getLoanDay() const { return _loanDay; }

  // setters
  void setBorrowDate(const std::string &borrowDate) {
    _borrowDate = borrowDate;
  }

  void setLoanDay(int loanDay) { _loanDay = loanDay; }

  std::string toString() const { return "LoanTicket"; }
};
