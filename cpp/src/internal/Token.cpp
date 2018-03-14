#include "internal/Token.h"
#include <map>
#include <iostream>

namespace {
  inline int compare(int x, int y) {
    return (x < y) ? -1 : ((x == y) ? 0 : 1);
  }

  struct StringToken : Token {
    explicit StringToken(const std::string& str) : str(str) {
    }

  private:
    int safeCompareTo(const Token& token) const override {
      auto& other = dynamic_cast<const StringToken&>(token);
      return compare(str.compare(other.str), 0);
    }

  private:
    std::string str;
  };

  struct NumericToken : Token {
    NumericToken(int length, int value) : length(length), value(value) {
    }

  private:
    int safeCompareTo(const Token& token) const override {
      auto& other = dynamic_cast<const NumericToken&>(token);
      auto result = compare(value, other.value);
      return result == 0 ? compare(length, other.length) : result;
    }

  private:
    int length;
    int value;
  };

  std::map<std::string, int> priorities = {
      { typeid(NumericToken).name(), 1 },
      { typeid(StringToken).name(),  2 },
  };
}

int Token::compareTo(const Token& other) const {
  auto priority = compare(
      priorities[typeid(*this).name()],
      priorities[typeid(other).name()]);
  return priority == 0 ? safeCompareTo(other) : priority;
}

Token* Token::newStringToken(const std::string& str) {
  return new StringToken(str);
}

Token* Token::newNumericToken(int length, int value) {
  return new NumericToken(length, value);
}
