#include "internal/TokenScanner.h"
#include "internal/TokenBuilder.h"
#include "internal/StringBuffer.h"

inline void TokenScanner::scanValidChars(int& pos) const {
  if (isValid(pos)) {
    buff->append(str[pos]);
    pos++;
  }
}

inline void TokenScanner::scanEscapeChars(int& pos) const {
  if (containsEscape(pos)) {
    pos = escape(pos + 1);
  }
}

inline bool TokenScanner::isSlash(int pos) const {
  return str.isInRange(pos) && str[pos] == '\\';
}

inline bool TokenScanner::containsEscape(int pos) const {
  return isSlash(pos) && isEscape(pos + 1);
}

inline bool TokenScanner::shouldForward(int pos) const {
  return isValid(pos) || containsEscape(pos);
}

inline void TokenScanner::scanToken(int& pos) const {
  while (shouldForward(pos)) {
    scanValidChars(pos);
    scanEscapeChars(pos);
  }
}

TokenScanner::TokenScanner(TokenBuilder* buff, const StringBuffer& str)
  : buff(buff), str(str) {
}

TokenScanner::~TokenScanner() {
  delete buff;
}

Token* TokenScanner::nextToken(int& pos) const {
  scanToken(pos);
  return buff->build();
}

namespace {
  struct StringTokenScanner : TokenScanner {
    explicit StringTokenScanner(const StringBuffer& str)
      : TokenScanner(TokenBuilder::newStringTokenBuilder(), str) {
    }

  private:
    bool isValid(int pos) const override {
      return str.isInRange(pos) && str.isLetter(pos);
    }

    bool isEscape(int pos) const override {
      return str.isInRange(pos) && str.isDigit(pos);
    }

    enum { MAX_NUM_LENGTH = 3 };

    int escape(int pos) const override {
      auto i = 0;
      while (i < MAX_NUM_LENGTH && isEscape(pos + i)) {
        buff->append(str[pos + i]);
        i++;
      }
      return pos + i;
    }
  };

  struct NumericTokenScanner: TokenScanner {
    explicit NumericTokenScanner(const StringBuffer& str) :
        TokenScanner(TokenBuilder::newNumericTokenBuilder(), str) {
    }

  private:
    bool isValid(int pos) const override {
      return str.isInRange(pos) && str.isDigit(pos);
    }

    bool isEscape(int pos) const override {
      return str.isInRange(pos) && str.isLetter(pos);
    }

    int escape(int pos) const override {
      buff->append(str[pos]);
      return pos + 1;
    }
  };
}

TokenScanner* TokenScanner::newScanner(const StringBuffer& str, bool startWithDigit) {
  if (startWithDigit) {
    return new NumericTokenScanner(str);
  } else {
    return new StringTokenScanner(str);
  }
}
