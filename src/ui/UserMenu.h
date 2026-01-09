#pragma once
#include "../services/LibraryService.h"
#include "../core/LoanTicket.h"
#include "BaseMenu.h"
#include "UIHelper.h"

class UserMenu : public BaseMenu {
private:
  LibraryService &_service;

  void showAvailableBooks(bool showPause = true) {
    clearScreen();
    drawBox("SACH CO THE MUON");
    std::cout << "\n";

    auto books = _service.getAllBooks();
    std::cout << "  ID   | Ten sach                          | Con lai\n";
    UI::UIHelper::drawTableLine(55);
    for (const auto &b : books) {
      if (b.getAvailable() > 0) {
        UI::UIHelper::drawSimpleTableRow(b.getId(), b.getTitle(), b.getAvailable());
      }
    }
    if (showPause) pause();
  }

  void showMyLoans(bool showPause = true) {
    clearScreen();
    drawBox("SACH DANG MUON");
    std::cout << "\n";

    auto loans = _service.getMyLoans();
    if (loans.empty()) {
      std::cout << "  (Ban chua muon sach nao)\n";
    } else {
      std::cout << "  Book ID | Ngay muon     | So ngay\n";
      drawLine();
      for (const auto &loan : loans) {
        printf("  %-7s | %-13s | %d\n", loan.getBookId().c_str(),
               loan.getBorrowDate().c_str(), loan.getLoanDay());
      }
    }
    if (showPause) pause();
  }

  void borrowBook() {
    // Không hiện pause ở đây
    showAvailableBooks(false);

    std::string bookId;
    int days;
    std::cout << "\n  Nhap ID sach muon muon: ";
    std::cin >> bookId;
    std::cout << "  So ngay muon: ";
    std::cin >> days;

    if (_service.borrowBook(bookId, days)) {
      std::cout << "\n  [OK] Da muon sach thanh cong!\n";
    } else {
      std::cout << "\n  [LOI] Khong the muon sach! Vui long kiem tra lai ID va so luong.\n";
      pause();
    }
  }

  void returnBook() {
    // Không hiện pause ở đây
    showMyLoans(false);

    auto loans = _service.getMyLoans();
    if (loans.empty()) {
      pause();
      return;
    }

    std::string bookId;
    std::cout << "\n  Nhap ID sach can tra: ";
    std::cin >> bookId;

    if (_service.returnBook(bookId)) {
      std::cout << "\n  [OK] Da tra sach thanh cong!\n";
    } else {
      std::cout << "\n  [LOI] Khong tim thay ban ghi mượn sách này!\n";
      pause();
    }
  }

public:
  UserMenu(LibraryService &service)
      : BaseMenu("MENU NGUOI DUNG"), _service(service) {
    _options = {"Xem sach co the muon", "Xem sach dang muon", "Muon sach",
                "Tra sach"};
  }

  void run() override {
    while (_running) {
      clearScreen();
      drawBox(_title);
      
      User* currentUser = _service.getCurrentUser();
      if (currentUser) {
        std::cout << "\n  Xin chao: " << currentUser->getName() << " (ID: " << currentUser->getUserId() << ")\n\n";
      }

      for (size_t i = 0; i < _options.size(); i++) {
        std::cout << "  [" << (i + 1) << "] " << _options[i] << "\n";
      }
      std::cout << "  [0] Dang xuat\n";
      drawLine();

      int choice = getInput(0, _options.size());

      switch (choice) {
      case 1: showAvailableBooks(); break;
      case 2: showMyLoans(); break;
      case 3: borrowBook(); break;
      case 4: returnBook(); break;
      case 0: _running = false; break;
      default:
        std::cout << "  [LOI] Lua chon khong hop le!\n";
        pause();
      }
    }
  }
};
