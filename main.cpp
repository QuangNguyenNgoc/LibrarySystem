#include "src/ui/App.h"

int main() {
  // Khởi tạo ứng dụng với đường dẫn mặc định (CSV)
  App app("./data/books.csv", "./data/users.csv");

  // Chạy ứng dụng
  app.run();

  return 0;
}