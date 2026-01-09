#pragma once
#include "../../core/Enums/Enum.h"
#include "./CsvFormatter.h"
#include "./IDataFormatter.h"
#include "./JsonFormatter.h"
#include "./TxtFormatter.h"
#include "./XmlFormatter.h"
#include <iostream>

class FormatterFactory {
public:
  static IDataFormatter *create(const FileType &);
};
