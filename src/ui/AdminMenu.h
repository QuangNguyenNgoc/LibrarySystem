#pragma once
#include "../services/LibraryService.h"
#include "../core/User.h"
#include "BaseMenu.h"
#include "UIHelper.h"

class AdminMenu : public BaseMenu {
private:
  LibraryService &_service;

  void showAllBooks(bool showPause = true) {
    clearScreen();
    drawBox("DANH SACH SACH");
    std::cout << "\n";

    auto books = _service.getAllBooks();
    if (books.empty()) {
      std::cout << "  (Khong co sach nao)\n";
    } else {
      std::cout << "  ID   | Ten sach                        | SL | Con\n";
      UI::UIHelper::drawTableLine(55);
      for (const auto &b : books) {
        UI::UIHelper::drawTableRow(b.getId(), b.getTitle(), b.getQuantity(), b.getAvailable());
      }
    }
    if (showPause) pause();
  }

  void showAllUsers(bool showPause = true) {
    clearScreen();
    drawBox("DANH SACH NGUOI DUNG");
    std::cout << "\n";

    auto users = _service.getAllUsers();
    if (users.empty()) {
      std::cout << "  (Khong co nguoi dung nao)\n";
    } else {
      std::cout << "  ID   | Ten                    | So sach muon\n";
      drawLine();
      for (const auto &u : users) {
        printf("  %-4s | %-22s | %zu\n", u.getUserId().c_str(),
               u.getName().c_str(), u.getLoans().size());
      }
    }
    if (showPause) pause();
  }

  void addBook() {
    clearScreen();
    drawBox("THEM SACH MOI");
    std::cout << "\n";

    std::string title, author;
    int year, quantity;

    std::cin.ignore(10000, '\n');
    std::cout << "  Ten sach: ";
    std::getline(std::cin, title);
    std::cout << "  Tac gia: ";
    std::getline(std::cin, author);
    std::cout << "  Nam xuat ban: ";
    std::cin >> year;
    std::cout << "  So luong: ";
    std::cin >> quantity;

    Book newBook("", title, year, author, quantity, quantity);
    if (_service.addNewBook(newBook)) {
      std::cout << "\n  [OK] Da them sach thanh cong!\n";
    } else {
      std::cout << "\n  [LOI] Khong the them sach!\n";
      pause();
    }
  }

  void addUser() {
    clearScreen();
    drawBox("THEM NGUOI DUNG MOI");
    std::cout << "\n";

    std::string name;
    std::cin.ignore(10000, '\n');
    std::cout << "  Ten nguoi dung: ";
    std::getline(std::cin, name);

    if (_service.registerUser(name)) {
      auto users = _service.getAllUsers();
      std::cout << "\n  [OK] Da them nguoi dung: " << name;
      if (!users.empty()) std::cout << " (ID: " << users.back().getUserId() << ")";
      std::cout << "\n";
    } else {
      std::cout << "\n  [LOI] Khong the dang ky nguoi dung!\n";
      pause();
    }
  }

  void removeBook() {
    clearScreen();
    drawBox("XOA SACH");
    
    std::string id;
    std::cout << "\n  Nhap ID sach can xoa: ";
    std::cin >> id;

    if (_service.removeBook(id)) {
      std::cout << "  [OK] Da xoa sach!\n";
    } else {
      std::cout << "  [LOI] Khong tim thay sach!\n";
      pause();
    }
  }

  void removeUser() {
    clearScreen();
    drawBox("XOA NGUOI DUNG");

    std::string id;
    std::cout << "\n  Nhap ID nguoi dung can xoa: ";
    std::cin >> id;

    if (_service.removeUser(id)) {
      std::cout << "  [OK] Da xoa nguoi dung (sach da duoc thu hoi)!\n";
    } else {
      std::cout << "  [LOI] Khong tim thay nguoi dung!\n";
      pause();
    }
  }

  void userDetail() {
    clearScreen();
    drawBox("CHI TIET NGUOI DUNG");

    std::string id;
    std::cout << "\n  Nhap ID nguoi dung: ";
    std::cin >> id;

    User *user = _service.findUserById(id);
    if (!user) {
      std::cout << "  [LOI] Khong tim thay nguoi dung!\n";
    } else {
      std::cout << "\n  ID: " << user->getUserId() << "\n";
      std::cout << "  Ten: " << user->getName() << "\n";
      
      auto loans = user->getLoans();
      std::cout << "  So sach dang muon: " << loans.size() << "\n";

      if (!loans.empty()) {
        std::cout << "\n  Danh sach sach muon:\n";
        drawLine();
        for (const auto &loan : loans) {
          std::cout << "    - Book ID: " << loan.getBookId()
                    << " | Ngay: " << loan.getBorrowDate()
                    << " | " << loan.getLoanDay() << " ngay\n";
        }
      }
    }
    pause();
  }

public:
  AdminMenu(LibraryService &service)
      : BaseMenu("QUAN TRI THU VIEN"), _service(service) {
    _options = {"Xem danh sach sach",   "Xem danh sach nguoi dung",
                "Them sach moi",        "Them nguoi dung",
                "Xoa sach",             "Xoa nguoi dung",
                "Chi tiet nguoi dung"};
  }

  void run() override {
    while (_running) {
      display();
      int choice = getInput(0, _options.size());

      switch (choice) {
      case 1: showAllBooks(); break;
      case 2: showAllUsers(); break;
      case 3: addBook(); break;
      case 4: addUser(); break;
      case 5: removeBook(); break;
      case 6: removeUser(); break;
      case 7: userDetail(); break;
      case 0: _running = false; break;
      default:
        std::cout << "  [LOI] Lua chon khong hop le!\n";
        pause();
      }
    }
  }
};
