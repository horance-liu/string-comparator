#include "internal/TokenBuilder.h"
#include "internal/Token.h"
#include <string>

namespace {
  struct StringTokenBuilder : TokenBuilder {
    void append(char c) override {
      str += c;
    }

    Token* build() override {
      return Token::newStringToken(str);
    }

    std::string str;
  };

  struct NumericTokenBuilder : TokenBuilder {
    NumericTokenBuilder() : length(0), value(0) {
    }

  private:
    void append(char c) override {
      length++;
      value = 10 * value + intValue(c);
    }

    Token* build() override {
      return Token::newNumericToken(length, value);
    }

  private:
    static int intValue(char c) {
      return std::isdigit(c) ? (c - '0') : c;
    }

  private:
    int length;
    int value;
  };
}

TokenBuilder* TokenBuilder::newStringTokenBuilder() {
  return new StringTokenBuilder();
}

TokenBuilder* TokenBuilder::newNumericTokenBuilder() {
  return new NumericTokenBuilder();
}
