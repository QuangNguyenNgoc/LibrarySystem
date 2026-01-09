#pragma once
#include <iostream>

class Object {
private:
  /* data */
public:
  virtual std::string toString() const = 0;

  virtual ~Object() = default;
};
