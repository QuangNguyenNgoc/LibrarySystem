#pragma once
#include "../services/LibraryService.h"
#include "../core/User.h"
#include "AdminMenu.h"
#include "BaseMenu.h"
#include "UserMenu.h"

class App : public BaseMenu {
private:
  std::string _bookPath;
  std::string _userPath;
  LibraryService *_service;

  void loginAsAdmin() {
    if (_service->login("admin")) {
      AdminMenu adminMenu(*_service);
      adminMenu.run();
      _service->logout();
    }
  }

  void loginAsUser() {
    clearScreen();
    drawBox("DANG NHAP NGUOI DUNG");

    std::string userId;
    std::cout << "\n  Nhap User ID: ";
    std::cin >> userId;

    if (_service->login(userId)) {
      UserMenu userMenu(*_service);
      userMenu.run();
      _service->logout();
    } else {
      std::cout << "  [LOI] Khong tim thay nguoi dung!\n";
      pause();
    }
  }

  void selectDataFormat() {
    clearScreen();
    drawBox("CHON FILE SACH (BOOKS)");
    std::cout << "\n  [Luu y] Nen su dung 2 file cung ten va cung dinh dang (VD: books.csv & users.csv)\n";
    std::cout << "  + Hien tai: " << Helpers::FileHelper::getFileName(_bookPath) << " | " << Helpers::FileHelper::getFileName(_userPath) << "\n\n";

    std::string dataDir = "./data";
    std::vector<std::string> allFiles =
        Helpers::FileHelper::getFilesInDirectory(dataDir);

    // 1. CHỌN FILE SACH
    std::vector<std::string> bookFiles;
    for (const auto &file : allFiles) {
      if (Helpers::FileHelper::identifyDataCategory(file) ==
          DataCategory::BOOKS) {
        if (Helpers::FileHelper::isSupported(
                Helpers::FileHelper::getFileType(file))) {
          bookFiles.push_back(file);
        }
      }
    }

    if (bookFiles.empty()) {
      std::cout << "  [LOI] Khong tim thay file sach nao trong " << dataDir
                << "!\n";
      pause();
      return;
    }

    for (size_t i = 0; i < bookFiles.size(); ++i) {
      std::cout << "  [" << (i + 1) << "] " << bookFiles[i] << "\n";
    }
    std::cout << "  [0] Huy\n";
    drawLine();

    int bChoice = getInput(0, bookFiles.size());
    if (bChoice == 0 || bChoice == -1)
      return;

    std::string bookFile = bookFiles[bChoice - 1];
    std::string bookExt = Helpers::FileHelper::getFileExtension(bookFile);

    // 2. CHỌN FILE NGUOI DUNG (Cùng định dạng)
    clearScreen();
    drawBox("CHON FILE NGUOI DUNG (USERS)");
    std::cout << "\n  (Yeu cau cung dinh dang: " << bookExt << ")\n\n";

    std::vector<std::string> userFiles;
    for (const auto &file : allFiles) {
      if (Helpers::FileHelper::identifyDataCategory(file) ==
              DataCategory::USERS &&
          Helpers::FileHelper::getFileExtension(file) == bookExt) {
        userFiles.push_back(file);
      }
    }

    if (userFiles.empty()) {
      std::cout << "  [LOI] Khong tim thay file nguoi dung hop le!\n";
      pause();
      return;
    }

    for (size_t i = 0; i < userFiles.size(); ++i) {
      std::cout << "  [" << (i + 1) << "] " << userFiles[i] << "\n";
    }
    std::cout << "  [0] Huy\n";
    drawLine();

    int uChoice = getInput(0, userFiles.size());
    if (uChoice == 0 || uChoice == -1)
      return;

    std::string userFile = userFiles[uChoice - 1];

    _bookPath = dataDir + "/" + bookFile;
    _userPath = dataDir + "/" + userFile;

    _service->reload(_bookPath, _userPath);

    std::cout << "\n  [OK] Da cau hinh du lieu moi:\n";
    std::cout << "       - Sach: " << bookFile << "\n";
    std::cout << "       - Users: " << userFile << "\n";
    pause();
  }

public:
  App(const std::string &bookPath, const std::string &userPath)
      : BaseMenu("HE THONG QUAN LY THU VIEN"), _bookPath(bookPath),
        _userPath(userPath) {
    _service = new LibraryService(bookPath, userPath);

    _options = {"Dang nhap Admin", "Dang nhap Nguoi dung", "Cau hinh du lieu"};
  }

  ~App() { delete _service; }

  void run() override {
    while (_running) {
      display();
      int choice = getInput(0, _options.size());

      switch (choice) {
      case 1:
        loginAsAdmin();
        break;
      case 2:
        loginAsUser();
        break;
      case 3:
        selectDataFormat();
        break;
      case 0:
        _running = false;
        std::cout << "\n  Tam biet! Hen gap lai.\n\n";
        break;
      default:
        std::cout << "  [LOI] Lua chon khong hop le!\n";
        pause();
      }
    }
  }
};
