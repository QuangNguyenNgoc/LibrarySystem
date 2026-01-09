#pragma once
#include "../core/Book.h"
#include "../core/User.h"
#include "../repositories/BookRepository.h"
#include "../repositories/UserRepository.h"
#include <iostream>
#include <string>
#include <vector>

class LibraryService {
private:
  BookRepository *_bookRepo;
  UserRepository *_userRepo;

  User *_currentUser;
  bool _isAdminMode;

public:
  // constructor:khởi tạo repo
  LibraryService(std::string bookPath, std::string userPath) {
    _bookRepo = new BookRepository(bookPath);
    _userRepo = new UserRepository(userPath);
    _currentUser = nullptr;
    _isAdminMode = false;

    // load data ngay khi khởi tạo
    _bookRepo->loadData();
    _userRepo->loadData();
  }

  // destructor
  ~LibraryService() {
    delete _bookRepo;
    delete _userRepo;
  }

  // --- NHÓM 1: CHUNG (Shared) ---
  bool login(std::string userId);
  void logout();
  bool isAdmin() const;
  User *getCurrentUser();
  std::vector<Book> getAllBooks();
  std::vector<Book> searchBooks(std::string keyword);
  Book *findBookById(std::string bookId);

  // --- NHÓM 2: ADMIN ONLY ---
  bool addNewBook(Book book);
  bool removeBook(std::string bookId);
  bool updateBookInfo(std::string bookId, std::string key, std::string value);
  
  bool registerUser(std::string name); // Tự sinh ID
  bool removeUser(std::string userId);
  std::vector<User> getAllUsers();
  User *findUserById(std::string userId);
  std::vector<LoanTicket> getUserHistory(std::string userId);

  // --- NHÓM 3: USER ONLY ---
  bool borrowBook(std::string bookId, int days = 14);
  bool returnBook(std::string bookId);
  std::vector<LoanTicket> getMyLoans();

  // Helper để lấy repo (cho UI nếu cần thiết, nhưng nên hạn chế)
  BookRepository &getBookRepo() { return *_bookRepo; }
  UserRepository &getUserRepo() { return *_userRepo; }
  
  // Reload data khi đổi file
  void reload(std::string bookPath, std::string userPath);
};