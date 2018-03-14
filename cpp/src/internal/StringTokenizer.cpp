#include "internal/StringTokenizer.h"
#include "internal/TokenScanner.h"

StringTokenizer::StringTokenizer(const std::string& str) :
    str(str), current(0) {
}

bool StringTokenizer::hasMoreTokens() const {
  return str.isInRange(current);
}

inline void StringTokenizer::assertValidLetter(int pos) const {
  auto isValid = str.startWithLetter(pos) || str.startWithDigit(pos);
  if (!isValid)
    throw std::invalid_argument(std::string("contains invalid char: ") + str[pos]);
}

inline void StringTokenizer::assertValidContents(int pos) const {
  while (str.isInRange(pos)) {
    assertValidLetter(pos);
    pos++;
  }
}

bool StringTokenizer::hasRemained() const {
  assertValidContents(current);
  return str.isInRange(current);
}

inline TokenScanner* StringTokenizer::newScanner() const {
  return TokenScanner::newScanner(str, str.startWithDigit(current));
}

Token* StringTokenizer::nextToken() {
  assertValidLetter(current);
  std::unique_ptr<TokenScanner> scanner(newScanner());
  return scanner->nextToken(current);
}
