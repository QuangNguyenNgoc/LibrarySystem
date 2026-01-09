#pragma once
#include <iostream>
#include <string>
#include <vector>

class BaseMenu {
protected:
  std::string _title;
  std::vector<std::string> _options;
  bool _running;

  void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
  }

  void drawBox(const std::string &text, int width = 50) {
    std::string border(width, '=');
    int padding = (width - 2 - text.length()) / 2;
    std::string paddedText =
        "|" + std::string(padding, ' ') + text +
        std::string(width - 2 - padding - text.length(), ' ') + "|";

    std::cout << border << "\n";
    std::cout << paddedText << "\n";
    std::cout << border << "\n";
  }

  void drawLine(int width = 50) {
    std::cout << std::string(width, '-') << "\n";
  }

  int getInput(int min, int max) {
    int choice;
    std::cout << "\n>> Nhap lua chon: ";
    std::cin >> choice;

    if (std::cin.fail() || choice < min || choice > max) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      return -1;
    }
    return choice;
  }

  void pause() {
    std::cout << "\nNhan Enter de tiep tuc...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
  }

public:
  BaseMenu(const std::string &title) : _title(title), _running(true) {}
  virtual ~BaseMenu() = default;

  virtual void display() {
    clearScreen();
    drawBox(_title);
    std::cout << "\n";

    for (size_t i = 0; i < _options.size(); i++) {
      std::cout << "  [" << (i + 1) << "] " << _options[i] << "\n";
    }
    std::cout << "  [0] Thoat\n";
    drawLine();
  }

  virtual void run() = 0;
};
