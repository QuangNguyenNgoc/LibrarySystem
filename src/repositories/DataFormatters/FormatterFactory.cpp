#include "FormatterFactory.h"

IDataFormatter *FormatterFactory::create(const FileType &type) {
  switch (type) {
  case FileType::TXT:
    return new TxtFormatter();
  case FileType::CSV:
    return new CsvFormatter();
  case FileType::JSON:
    return new JsonFormatter();
  case FileType::XML:
    return new XmlFormatter();
  default:
    return nullptr;
  }
}