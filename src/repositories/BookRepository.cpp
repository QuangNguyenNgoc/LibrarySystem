#include "BookRepository.h"

BookRepository::BookRepository(const std::string &filePath)
    : _filePath(filePath), _formatter(nullptr) {
  FileType type = Helpers::FileHelper::getFileType(filePath);
  _formatter = FormatterFactory::create(type);
}

Book *BookRepository::findBookById(const std::string &Id) {
  for (auto &book : _books) {
    if (Id == book.getId())
      return &book;
  }
  return nullptr;
}

inline Book *BookRepository::findBookByTitle(const std::string &title) {
  std::string lowerTitle = Helpers::StringHelper::toLower(title);

  for (auto &book : _books) {
    if (Helpers::StringHelper::toLower(book.getTitle()) == lowerTitle) {
      return &book;
    }
  }

  return nullptr;
}

std::vector<Book>
BookRepository::findBooksByKeywordTitle(const std::string &title) {
  std::vector<Book> result;

  std::string lowerTitle = Helpers::StringHelper::toLower(title);

  for (const auto &book : _books) {
    if (Helpers::StringHelper::toLower(book.getTitle()).find(lowerTitle) !=
        std::string::npos) {
      result.push_back(book);
    }
  }
  return result;
}

bool BookRepository::loadData() {
  if (!_formatter)
    return false;

  // Phân tích dữ liệu từ file
  auto rawLines = Helpers::FileHelper::readFileLines(_filePath);
  auto dataMaps = _formatter->parse(rawLines);

  _books.clear();

  for (const auto &map : dataMaps) {
    try {
      // tạo book từ map dữ liệu
      std::string id = map.count("id") ? map.at("id") : "0";
      std::string title = map.count("title") ? map.at("title") : "No Title";
      std::string author = map.count("author") ? map.at("author") : "Unknown";
      int year = map.count("year") ? std::stoi(map.at("year")) : 0;
      int quantity = map.count("quantity") ? std::stoi(map.at("quantity")) : 0;
      int available =
          map.count("available") ? std::stoi(map.at("available")) : 0;

      Book newBook(id, title, year, author, quantity, available);
      _books.push_back(newBook);
    } catch (const std::exception &e) {
      std::cerr << "error when add this book to library!" << std::endl;
      continue;
    }
  }
  return true;
}

bool BookRepository::addBook(Book book) {
  Book *existingBook = findBookByTitle(book.getTitle());

  // * Sách tồn tại -> cập nhật số lượng
  if (existingBook) {
    // nếu sách đã tồn tại, cập nhật tăng số lượng sách đã có
    existingBook->setQuantity(existingBook->getQuantity() + book.getQuantity());
    existingBook->setAvailable(existingBook->getAvailable() +
                               book.getAvailable());

    updateChange();
    return true;
  }

  // * Sách chưa tồn tại -> thêm sách mới
  std::string newId = generateId();
  book.setId(newId);

  _books.push_back(book);

  updateChange();
  return true;
}

bool BookRepository::removeBook(const std::string &Id) {
  // duyệt tìm sách theo Id và xóa
  for (auto it = _books.begin(); it != _books.end(); ++it) {
    if (it->getId() == Id) {
      _books.erase(it);

      updateChange();
      return true;
    }
  }

  return false;
}

bool BookRepository::configureBook(const std::string &Id,
                                   const std::string &key,
                                   const std::string &value) {
  Book *book = findBookById(Id);
  if (!book) {
    return false;
  }

  // Cập nhật giá trị cho key tương ứng
  if (key == "title") {
    book->setTitle(value);
  } else if (key == "author") {
    book->setAuthor(value);
  } else if (key == "year") {
    book->setYear(stoi(value));
  } else if (key == "quantity") {
    int newQuantity = stoi(value);
    int delta = newQuantity - book->getQuantity();
    book->setQuantity(newQuantity);
    book->setAvailable(book->getAvailable() + delta);
  } else if (key == "available") {
    book->setAvailable(stoi(value));
  } else {
    return false;
  }

  updateChange();
  return true;
}

bool BookRepository::updateChange() {
  // xóa dữ liệu cũ
  Helpers::FileHelper::clearFile(_filePath);

  // ghi lại dữ liệu mới từ đầu
  std::vector<std::map<std::string, std::string>> dataMaps;
  for (const auto &book : _books) {
    // nén dữ liệu vào map
    std::map<std::string, std::string> mapData;
    mapData["id"] = book.getId();
    mapData["title"] = book.getTitle();
    mapData["year"] = std::to_string(book.getYear());
    mapData["author"] = book.getAuthor();
    mapData["quantity"] = std::to_string(book.getQuantity());
    mapData["available"] = std::to_string(book.getAvailable());

    dataMaps.push_back(mapData);
  }

  // chuyển đổi mapDatas thành chuỗi rồi ghi lại vào file.
  // Quy đinh thứ tự header
  std::vector<std::string> headers = {"id",   "title",    "author",
                                      "year", "quantity", "available"};
  std::vector<std::string> lines = _formatter->format(dataMaps, headers);
  Helpers::FileHelper::writeFileLines(_filePath, lines);

  return true;
}

// NOTE: hàm này đang lấy ID dạng số, có thể sửa đổi để lấy dạng khác
std::string BookRepository::generateId() const {
  int maxId = 0;
  for (const auto &book : _books) {
    int bookId = stoi(book.getId());
    if (bookId > maxId) {
      maxId = bookId;
    }
  }
  return std::to_string(maxId + 1);
}

std::vector<Book> BookRepository::getBooks() const { return _books; }

BookRepository::~BookRepository() {
  if (_formatter) {
    delete _formatter;
    _formatter = nullptr;
  }
}
