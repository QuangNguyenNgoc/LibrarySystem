#pragma once
#include "./LoanTicket.h"
#include "./Object.h"
#include <iostream>
#include <vector>

class User : public Object {
private:
  std::string _userId;
  std::string _name;
  std::vector<LoanTicket> _loans;

public:
  // constructors: khởi tạo thêm user mới
  User(const std::string &userId, const std::string &name)
      : _userId(userId), _name(name) {}

  // constructor: khởi tạo user từ dữ liệu có sẵn
  User(const std::string &userId, const std::string &name,
       const std::vector<LoanTicket> &loans)
      : _userId(userId), _name(name), _loans(loans) {}

  // getters
  std::string getUserId() const { return _userId; }

  std::string getName() const { return _name; }

  const std::vector<LoanTicket> &getLoans() const { return _loans; }

  std::vector<LoanTicket> &getLoans() { return _loans; }

  // setters
  void setName(const std::string &name) { _name = name; }

  void setLoans(const std::vector<LoanTicket> &loans) { _loans = loans; }

  std::string toString() const { return "User"; }
};
