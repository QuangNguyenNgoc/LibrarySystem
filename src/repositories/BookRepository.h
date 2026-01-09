#pragma once
#include "../core/Book.h"
#include "./DataFormatters/FormatterFactory.h"
#include "./Helpers/FileHelper.h"
#include <iostream>
#include <map>
#include <vector>

class BookRepository {
private:
  std::string _filePath;
  IDataFormatter *_formatter;
  std::vector<Book> _books;

public:
  // constructor
  BookRepository(const std::string &);

  // find/get book
  Book *findBookById(const std::string &);

  Book *findBookByTitle(const std::string &);

  std::vector<Book> findBooksByKeywordTitle(const std::string &);

  std::vector<Book> getBooks() const;

  // data operations
  bool loadData();

  bool addBook(Book);

  bool removeBook(const std::string &);

  bool configureBook(const std::string &, const std::string &,
                     const std::string &);

  // save changes to file
  bool updateChange();

  // utils
  std::string generateId() const;

  //  destructor
  ~BookRepository();
};
