#pragma once
#include "../core/User.h"
#include "./DataFormatters/FormatterFactory.h"
#include "./Helpers/FileHelper.h"
#include <iostream>
#include <map>
#include <vector>

class UserRepository {
private:
  std::string _filePath;
  IDataFormatter *_formatter;
  std::vector<User> _users;

public:
  // constructor
  UserRepository(const std::string &);

  // find/get user
  User *findUserById(const std::string &);

  // data operations
  bool loadData();

  bool addUser(User);

  bool removeUser(const std::string &);

  bool configureUser(const std::string &, const std::string &,
                     const std::string &);

  // save changes to file
  bool updateChange();

  // utils
  std::string generateId() const;

  std::vector<User> getUsers() const;

  //  destructor
  ~UserRepository();
};