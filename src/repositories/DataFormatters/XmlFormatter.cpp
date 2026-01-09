#include "XmlFormatter.h"
#include <sstream>

using namespace tinyxml2;

std::vector<std::map<std::string, std::string>>
XmlFormatter::parse(const std::vector<std::string> &rawLines) const {
  std::vector<std::map<std::string, std::string>> result;

  if (rawLines.empty())
    return result;

  // nối chuỗi thành một dòng duy nhất
  std::ostringstream oss;
  for (const auto &line : rawLines)
    oss << line;
  std::string xmlContent = oss.str();

  // parse chuỗi XML
  XMLDocument doc;
  XMLError e = doc.Parse(xmlContent.c_str());
  if (e != XML_SUCCESS) {
    std::cerr << "XML Parse Error ID: " << e << std::endl;
    std::cerr << "Error Name: " << doc.ErrorName() << std::endl;
    return result;
  }

  // duyệt các phần tử XML
  // Giả định cấu trúc: <Rows> <Row>...</Row> <Row>...</Row> </Rows>
  XMLElement *root = doc.RootElement();
  if (!root) {
    return result;
  }

  // Duyệt từng phần tử con của Root (tương ứng với từng Record/Book)
  for (XMLElement *elem = root->FirstChildElement(); elem != nullptr;
       elem = elem->NextSiblingElement()) {
    std::map<std::string, std::string> record;

    // Duyệt từng trường dữ liệu bên trong (id, title, author...)
    for (XMLElement *child = elem->FirstChildElement(); child != nullptr;
         child = child->NextSiblingElement()) {
      const char *key = child->Name();
      const char *val = child->GetText();

      // GetText() có thể trả về null nếu thẻ rỗng <title></title>
      record[key] = (val) ? val : "";
    }

    if (!record.empty()) {
      result.push_back(record);
    }
  }

  return result;
}

std::vector<std::string> XmlFormatter::format(
    const std::vector<std::map<std::string, std::string>> &data,
    const std::vector<std::string> &orderedKeys) const {

  if (data.empty())
    return {};

  XMLDocument doc;

  // thêm encoding declaration
  XMLDeclaration *decl = doc.NewDeclaration();
  doc.InsertFirstChild(decl);

  // tạo thẻ chứa gốc
  XMLElement *root = doc.NewElement("Books");
  doc.InsertEndChild(root);

  // duyệt dữ liệu để tạo các thẻ con
  for (const auto &record : data) {
    XMLElement *row = doc.NewElement("Book");

    // duyệt theo orderedKeys nếu có, ngược lại duyệt toàn bộ key trong map
    if (!orderedKeys.empty()) {
      for (const auto &key : orderedKeys) {
        if (record.count(key)) {
          XMLElement *field = doc.NewElement(key.c_str());
          field->SetText(record.at(key).c_str());
          row->InsertEndChild(field);
        }
      }
    } else {
      for (const auto &[key, val] : record) {
        XMLElement *field = doc.NewElement(key.c_str());
        field->SetText(val.c_str());
        row->InsertEndChild(field);
      }
    }
    root->InsertEndChild(row);
  }

  // in ra chuỗi XML
  XMLPrinter printer;
  doc.Print(&printer);

  // TinyXML trả về toàn bộ XML dưới dạng một chuỗi, chia thành các dòng
  std::string fullXml = printer.CStr();
  std::vector<std::string> lines;
  std::stringstream ss(fullXml);
  std::string line;
  while (std::getline(ss, line)) {
    lines.push_back(line);
  }

  return lines;
}

std::string XmlFormatter::toType() const { return "XML"; }