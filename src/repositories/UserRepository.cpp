#include "UserRepository.h"
#include "../core/Helpers/LoanTicketParser.h"

UserRepository::UserRepository(const std::string &filePath)
    : _filePath(filePath) {
  FileType type = Helpers::FileHelper::getFileType(filePath);
  _formatter = FormatterFactory::create(type);
}

User *UserRepository::findUserById(const std::string &Id) {
  for (auto &user : _users) {
    if (Id == user.getUserId())
      return &user;
  }
  return nullptr;
}

bool UserRepository::loadData() {
  if (!_formatter)
    return false;

  // Phân tích dữ liệu từ file
  auto rawLines = Helpers::FileHelper::readFileLines(_filePath);
  auto dataMaps = _formatter->parse(rawLines);

  _users.clear();

  for (const auto &map : dataMaps) {
    try {
      // Tạo user từ map dữ liệu
      std::string id = map.count("id") ? map.at("id") : "0";
      std::string name = map.count("name") ? map.at("name") : "No Name";

      // Parse borrowed field
      std::vector<LoanTicket> loans;
      if (map.count("borrowed")) {
        std::string borrowedStr = map.at("borrowed");
        loans = Helpers::LoanTicketParser::parseCompact(borrowedStr);
      }

      User user(id, name, loans);
      _users.push_back(user);
    } catch (const std::exception &e) {
      std::cerr << "Error loading user data: " << e.what() << std::endl;
      continue;
    }
  }

  return true;
}

bool UserRepository::addUser(User user) {
  _users.push_back(user);

  updateChange();
  return true;
}

bool UserRepository::removeUser(const std::string &Id) {
  // duyệt tìm user theo Id và xóa
  for (auto it = _users.begin(); it != _users.end(); ++it) {
    if (it->getUserId() == Id) {
      _users.erase(it);

      updateChange();
      return true;
    }
  }

  return false;
}

bool UserRepository::configureUser(const std::string &Id,
                                   const std::string &key,
                                   const std::string &value) {
  User *user = findUserById(Id);
  if (!user) {
    return false;
  }

  // Cập nhật giá trị cho user tương ứng
  if (key == "name") {
    user->setName(value);
  } else {
    return false;
  }

  updateChange();
  return true;
}

bool UserRepository::updateChange() {
  // Xóa dữ liệu cũ
  Helpers::FileHelper::clearFile(_filePath);

  // Ghi lại dữ liệu mới từ đầu
  std::vector<std::map<std::string, std::string>> dataMaps;
  for (const auto &user : _users) {
    // Nén dữ liệu vào map
    std::map<std::string, std::string> mapData;
    mapData["id"] = user.getUserId();
    mapData["name"] = user.getName();
    mapData["borrowed"] =
        Helpers::LoanTicketParser::formatCompact(user.getLoans());

    dataMaps.push_back(mapData);
  }

  // Chuyển đổi mapDatas thành chuỗi rồi ghi lại vào file
  // Quy định thứ tự header (QUAN TRỌNG: phải có 'borrowed')
  std::vector<std::string> headers = {"id", "name", "borrowed"};
  std::vector<std::string> lines = _formatter->format(dataMaps, headers);
  Helpers::FileHelper::writeFileLines(_filePath, lines);

  return true;
}

// NOTE: tương tự như file BookRepo
std::string UserRepository::generateId() const {
  int maxId = 0;
  for (const auto &user : _users) {
    try {
      int idNum = std::stoi(user.getUserId());
      if (idNum > maxId) {
        maxId = idNum;
      }
    } catch (const std::exception &e) {
      continue;
    }
  }
  return "U" + std::to_string(maxId + 1);
}

std::vector<User> UserRepository::getUsers() const { return _users; }

UserRepository::~UserRepository() {
  if (_formatter) {
    delete _formatter;
    _formatter = nullptr;
  }
}