#include "LibraryService.h"
#include "../core/Helpers/DataHelper.h"
#include <iostream>

// --- NHÓM 1: CHUNG (Shared) ---

bool LibraryService::login(std::string userId) {
  if (userId == "admin") {
    _isAdminMode = true;
    _currentUser = nullptr;
    return true;
  }

  User *user = _userRepo->findUserById(userId);
  if (user) {
    _currentUser = user;
    _isAdminMode = false;
    return true;
  }
  return false;
}

void LibraryService::logout() {
  _currentUser = nullptr;
  _isAdminMode = false;
}

bool LibraryService::isAdmin() const { return _isAdminMode; }

User *LibraryService::getCurrentUser() { return _currentUser; }

std::vector<Book> LibraryService::getAllBooks() {
  return _bookRepo->getBooks();
}

std::vector<Book> LibraryService::searchBooks(std::string keyword) {
  if (keyword.empty()) {
    return _bookRepo->getBooks();
  }
  return _bookRepo->findBooksByKeywordTitle(keyword);
}

Book *LibraryService::findBookById(std::string bookId) {
  return _bookRepo->findBookById(bookId);
}

// --- NHÓM 2: ADMIN ONLY ---

bool LibraryService::addNewBook(Book book) {
  if (!_isAdminMode) return false;
  bool success = _bookRepo->addBook(book);
  if (success) _bookRepo->updateChange();
  return success;
}

bool LibraryService::removeBook(std::string bookId) {
  if (!_isAdminMode) return false;
  bool success = _bookRepo->removeBook(bookId);
  if (success) _bookRepo->updateChange();
  return success;
}

bool LibraryService::updateBookInfo(std::string bookId, std::string key, std::string value) {
  if (!_isAdminMode) return false;
  Book *book = _bookRepo->findBookById(bookId);
  if (!book) return false;

  // Tạm thời support update basic fields
  if (key == "title") book->setTitle(value);
  else if (key == "author") book->setAuthor(value);
  else if (key == "year") book->setYear(std::stoi(value));
  else if (key == "quantity") {
    int oldQty = book->getQuantity();
    int newQty = std::stoi(value);
    int diff = newQty - oldQty;
    book->setQuantity(newQty);
    book->setAvailable(book->getAvailable() + diff);
  } else return false;

  _bookRepo->updateChange();
  return true;
}

bool LibraryService::registerUser(std::string name) {
  if (!_isAdminMode) return false;
  std::string newId = _userRepo->generateId();
  User newUser(newId, name);
  bool success = _userRepo->addUser(newUser);
  if (success) _userRepo->updateChange();
  return success;
}

bool LibraryService::removeUser(std::string userId) {
  if (!_isAdminMode) return false;
  
  User *user = _userRepo->findUserById(userId);
  if (!user) return false;

  // Trả lại toàn bộ sách đang mượn vào kho trước khi xóa user
  auto loans = user->getLoans();
  for (const auto &loan : loans) {
    Book *book = _bookRepo->findBookById(loan.getBookId());
    if (book) {
      book->setAvailable(book->getAvailable() + 1);
    }
  }

  bool success = _userRepo->removeUser(userId);
  if (success) {
    _userRepo->updateChange();
    _bookRepo->updateChange();
  }
  return success;
}

std::vector<User> LibraryService::getAllUsers() {
  if (!_isAdminMode) return {};
  return _userRepo->getUsers();
}

User *LibraryService::findUserById(std::string userId) {
  return _userRepo->findUserById(userId);
}

std::vector<LoanTicket> LibraryService::getUserHistory(std::string userId) {
  User *user = _userRepo->findUserById(userId);
  if (user) return user->getLoans();
  return {};
}

// --- NHÓM 3: USER ONLY ---

bool LibraryService::borrowBook(std::string bookId, int days) {
  if (!_currentUser || _isAdminMode) return false;

  Book *book = _bookRepo->findBookById(bookId);
  if (!book || book->getAvailable() <= 0) return false;

  // Giới hạn mượn 5 cuốn
  if (_currentUser->getLoans().size() >= 5) {
    std::cout << "Ban da muon qua gioi han (5 cuon)!" << std::endl;
    return false;
  }

  // Check xem đã mượn chưa
  for (const auto &ticket : _currentUser->getLoans()) {
    if (ticket.getBookId() == bookId) {
      std::cout << "Ban dang muon cuon nay roi!" << std::endl;
      return false;
    }
  }

  // Thực hiện mượn
  book->setAvailable(book->getAvailable() - 1);
  
  LoanTicket newTicket(bookId, days);
  auto loans = _currentUser->getLoans();
  loans.push_back(newTicket);
  _currentUser->setLoans(loans);

  _bookRepo->updateChange();
  _userRepo->updateChange();

  return true;
}

bool LibraryService::returnBook(std::string bookId) {
  if (!_currentUser || _isAdminMode) return false;

  Book *book = _bookRepo->findBookById(bookId);
  if (!book) return false;

  auto loans = _currentUser->getLoans();
  bool found = false;
  for (auto it = loans.begin(); it != loans.end(); ++it) {
    if (it->getBookId() == bookId) {
      loans.erase(it);
      found = true;
      break;
    }
  }

  if (found) {
    _currentUser->setLoans(loans);
    book->setAvailable(book->getAvailable() + 1);

    _bookRepo->updateChange();
    _userRepo->updateChange();
    return true;
  }

  return false;
}

std::vector<LoanTicket> LibraryService::getMyLoans() {
  if (_currentUser) return _currentUser->getLoans();
  return {};
}

void LibraryService::reload(std::string bookPath, std::string userPath) {
  delete _bookRepo;
  delete _userRepo;
  _bookRepo = new BookRepository(bookPath);
  _userRepo = new UserRepository(userPath);
  _bookRepo->loadData();
  _userRepo->loadData();
}