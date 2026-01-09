#include "../src/repositories/UserRepository.h"
#include "../src/services/LibraryService.h"
#include "catch.hpp"
#include <fstream>

using namespace Helpers;
// Hàm hỗ trợ tạo file dữ liệu giả
void createDummyUserFile(const std::string &path, const std::string &content) {
  std::ofstream ofs(path);
  ofs << content;
  ofs.close();
}

TEST_CASE("FileHelper identify file types correctly", "[FileHelper]") {
  // Kịch bản 1: File chuẩn
  SECTION("Standard extensions") {
    REQUIRE(FileHelper::getFileType("data.txt") == FileType::TXT);
    REQUIRE(FileHelper::getFileType("user.csv") == FileType::CSV);
    REQUIRE(FileHelper::getFileType("config.json") == FileType::JSON);
  }

  // Kịch bản 2: File chữ hoa chữ thường lộn xộn
  SECTION("Case insensitivity") {
    REQUIRE(FileHelper::getFileType("DATA.TXT") == FileType::TXT);
    REQUIRE(FileHelper::getFileType("User.Csv") == FileType::CSV);
  }

  // Kịch bản 3: File lạ hoặc không có đuôi
  SECTION("Unknown or no extension") {
    REQUIRE(FileHelper::getFileType("image.png") == FileType::UNKNOWN);
    REQUIRE(FileHelper::getFileType("README") == FileType::UNKNOWN);
    REQUIRE(FileHelper::getFileType("archive.tar.gz") ==
            FileType::UNKNOWN); // Giả sử chưa hỗ trợ
  }
  // Kịch bản 4: Phân loại dữ liệu (b* -> BOOKS, u* -> USERS)
  SECTION("Identify Data Category") {
    REQUIRE(FileHelper::identifyDataCategory("books.csv") == DataCategory::BOOKS);
    REQUIRE(FileHelper::identifyDataCategory("Users.json") == DataCategory::USERS);
    REQUIRE(FileHelper::identifyDataCategory("other.txt") == DataCategory::UNKNOWN);
    REQUIRE(FileHelper::identifyDataCategory("") == DataCategory::UNKNOWN);
  }
}

TEST_CASE("FileHelper get files in directory", "[FileHelper]") {
  std::string testDir = "test_temp_dir";
  std::filesystem::create_directory(testDir);
  std::ofstream(testDir + "/file1.txt").close();
  std::ofstream(testDir + "/file2.csv").close();
  std::ofstream(testDir + "/file3.json").close();

  SECTION("Retrieve files from directory") {
    // hàm chính cần kiểm tra
    std::vector<std::string> files = FileHelper::getFilesInDirectory(testDir);

    REQUIRE(files.size() == 3);
    REQUIRE(std::find(files.begin(), files.end(), "file1.txt") != files.end());
    REQUIRE(std::find(files.begin(), files.end(), "file2.csv") != files.end());
    REQUIRE(std::find(files.begin(), files.end(), "file3.json") != files.end());
  }

  std::filesystem::remove_all(testDir);
}

TEST_CASE("User & LoanTicket Parsing Logic", "[UserRepository]") {
  SECTION("Parse CSV Format") {
    std::string testPath = "./test_users.csv";
    std::string csvContent = "id,name,borrowed\n"
                             "U1,Alice,[1|2023-01-01|7; 2|2023-01-05|14]\n"
                             "U2,Bob,[]";
    createDummyUserFile(testPath, csvContent);

    UserRepository repo(testPath);
    REQUIRE(repo.loadData() == true);

    User *u1 = repo.findUserById("U1");
    REQUIRE(u1 != nullptr);
    REQUIRE(u1->getName() == "Alice");
    REQUIRE(u1->getLoans().size() == 2);
    REQUIRE(u1->getLoans()[0].getBookId() == "1");
    REQUIRE(u1->getLoans()[0].getLoanDay() == 7);

    User *u2 = repo.findUserById("U2");
    REQUIRE(u2 != nullptr);
    REQUIRE(u2->getLoans().empty());

    std::filesystem::remove(testPath);
  }

  SECTION("Parse TXT Format") {
    std::string testPath = "./test_users.txt";
    std::string txtContent = "id=U1, name=Alice, borrowed=[1|2023-01-01|7; 2|2023-01-05|14]\n"
                             "id=U2, name=Bob, borrowed=[]";
    createDummyUserFile(testPath, txtContent);

    UserRepository repo(testPath);
    REQUIRE(repo.loadData() == true);

    User *u1 = repo.findUserById("U1");
    REQUIRE(u1 != nullptr);
    REQUIRE(u1->getName() == "Alice");
    REQUIRE(u1->getLoans().size() == 2);
    REQUIRE(u1->getLoans()[0].getBookId() == "1");

    User *u2 = repo.findUserById("U2");
    REQUIRE(u2 != nullptr);
    REQUIRE(u2->getLoans().empty());

    std::filesystem::remove(testPath);
  }

  SECTION("Parse JSON Format") {
    std::string testPath = "./test_users.json";
    std::string jsonContent = R"([
      {"id": "U1", "name": "Alice", "borrowed": ["1|2023-01-01|7", "2|2023-01-05|14"]},
      {"id": "U2", "name": "Bob", "borrowed": []}
    ])";
    createDummyUserFile(testPath, jsonContent);

    UserRepository repo(testPath);
    REQUIRE(repo.loadData() == true);

    User *u1 = repo.findUserById("U1");
    REQUIRE(u1 != nullptr);
    REQUIRE(u1->getName() == "Alice");
    REQUIRE(u1->getLoans().size() == 2);
    REQUIRE(u1->getLoans()[0].getBookId() == "1");

    User *u2 = repo.findUserById("U2");
    REQUIRE(u2 != nullptr);
    REQUIRE(u2->getLoans().empty());

    std::filesystem::remove(testPath);
  }

  SECTION("Parse XML Format") {
    std::string testPath = "./test_users.xml";
    std::string xmlContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<Users>
  <User>
    <id>U1</id>
    <name>Alice</name>
    <borrowed>[1|2023-01-01|7; 2|2023-01-05|14]</borrowed>
  </User>
  <User>
    <id>U2</id>
    <name>Bob</name>
    <borrowed>[]</borrowed>
  </User>
</Users>)";
    createDummyUserFile(testPath, xmlContent);

    UserRepository repo(testPath);
    REQUIRE(repo.loadData() == true);

    User *u1 = repo.findUserById("U1");
    REQUIRE(u1 != nullptr);
    REQUIRE(u1->getName() == "Alice");
    REQUIRE(u1->getLoans().size() == 2);
    REQUIRE(u1->getLoans()[0].getBookId() == "1");

    User *u2 = repo.findUserById("U2");
    REQUIRE(u2 != nullptr);
    REQUIRE(u2->getLoans().empty());

    std::filesystem::remove(testPath);
  }
}

TEST_CASE("UserRepository Operations (CRUD)", "[UserRepository]") {
  std::string testPath = "./test_users_crud.csv";
  // Reset file rỗng có header
  createDummyUserFile(testPath, "id,name,borrowed\n");

  UserRepository repo(testPath);
  repo.loadData();

  SECTION("Add User and Add Loan Ticket") {
    // 1. Thêm User mới
    User newUser("U99", "Tester");
    repo.addUser(newUser);

    User *savedUser = repo.findUserById("U99");
    REQUIRE(savedUser != nullptr);
    REQUIRE(savedUser->getLoans().empty());

    // 2. Thêm vé mượn cho User này (Giả lập logic mượn sách)
    LoanTicket ticket("B01", "2024-01-01", 30);
    savedUser->getLoans().push_back(ticket);

    // Lưu thay đổi xuống file
    repo.updateChange();

    // 3. Reload lại từ file để xem dữ liệu có còn đó không
    UserRepository repo2(testPath); // Repo mới đọc file cũ
    repo2.loadData();

    User *reloadedUser = repo2.findUserById("U99");
    REQUIRE(reloadedUser != nullptr);
    REQUIRE(reloadedUser->getLoans().size() == 1);
    REQUIRE(reloadedUser->getLoans()[0].getBookId() == "B01");
  }

  SECTION("Remove User") {
    User u("DEL", "Delete Me");
    repo.addUser(u);
    repo.updateChange();

    REQUIRE(repo.findUserById("DEL") != nullptr);

    repo.removeUser("DEL");
    REQUIRE(repo.findUserById("DEL") == nullptr);

    // Reload để chắc chắn đã xóa trong file
    UserRepository repo3(testPath);
    repo3.loadData();
    REQUIRE(repo3.findUserById("DEL") == nullptr);
  }

  // Cleanup: xóa file test sau khi hoàn thành
  std::filesystem::remove(testPath);
}

TEST_CASE("LibraryService Business Logic", "[LibraryService]") {
  std::string bookPath = "./test_svc_books.csv";
  std::string userPath = "./test_svc_users.csv";

  // Init dummy data: 1 book with 5 available, 1 user with 1 loan
  createDummyUserFile(bookPath, "id,title,year,author,quantity,available\nB1,Test Book,2023,Author,5,4\n");
  createDummyUserFile(userPath, "id,name,borrowed\nU1,User, [B1|2024-01-01|7]\n");

  LibraryService service(bookPath, userPath);
  service.login("admin"); // Cần quyền admin để xóa user

  SECTION("Remove user should return books to inventory") {
    // Trước khi xóa: B1 có 4 cuốn khả dụng
    Book *b = service.findBookById("B1");
    REQUIRE(b != nullptr);
    REQUIRE(b->getAvailable() == 4);

    // Xóa user U1
    bool success = service.removeUser("U1");
    REQUIRE(success == true);

    // Sau khi xóa: B1 phải có 5 cuốn khả dụng (4 + 1 trả lại)
    REQUIRE(b->getAvailable() == 5);

    // Kiểm tra đã lưu xuống file chưa bằng cách reload
    service.reload(bookPath, userPath);
    Book *b2 = service.findBookById("B1");
    REQUIRE(b2->getAvailable() == 5);
    REQUIRE(service.findUserById("U1") == nullptr);
  }

  std::filesystem::remove(bookPath);
  std::filesystem::remove(userPath);
}
