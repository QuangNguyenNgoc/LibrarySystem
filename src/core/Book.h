#pragma once
#include "Object.h"
#include <iostream>

class Book : public Object {
private:
  std::string _id;
  std::string _title;
  int _year;
  std::string _author;

private:
  int _quantity;
  int _available;

public:
  // Constructors
  Book(const std::string &id, const std::string &title, int year,
       const std::string &author)
      : _id(id), _title(title), _year(year), _author(author), _quantity(1),
        _available(1) {}

  Book(const std::string &id, const std::string &title, int year,
       const std::string &author, int quantity)
      : _id(id), _title(title), _year(year), _author(author),
        _quantity(quantity), _available(quantity) {}

  Book(const std::string &id, const std::string &title, int year,
       const std::string &author, int quantity, int available)
      : _id(id), _title(title), _year(year), _author(author),
        _quantity(quantity), _available(available) {}

  // Getters
  std::string getId() const { return _id; }

  std::string getTitle() const { return _title; }

  int getYear() const { return _year; }

  std::string getAuthor() const { return _author; }

  std::string toString() const { return "Book"; }

  // Setters
  int getQuantity() const { return _quantity; }

  int getAvailable() const { return _available; }

  void setId(const std::string &id) { _id = id; }

  void setTitle(const std::string &title) { _title = title; }

  void setYear(int year) { _year = year; }

  void setAuthor(const std::string &author) { _author = author; }

  void setQuantity(int quantity) { _quantity = quantity; }

  void setAvailable(int available) { _available = available; }
};
