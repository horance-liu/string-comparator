#ifndef H4E0982D3_00D7_4C77_A7EC_06215CE0F0BA
#define H4E0982D3_00D7_4C77_A7EC_06215CE0F0BA

#include <memory>
#include <string>

#include "internal/StringBuffer.h"

struct Token;
struct TokenScanner;

struct StringTokenizer {
  explicit StringTokenizer(const std::string& str);

  bool hasMoreTokens() const;
  bool hasRemained() const;

  Token* nextToken();

private:
  void assertValidLetter(int pos) const;
  void assertValidContents(int pos) const;
  TokenScanner* newScanner() const;

private:
  StringBuffer str;
  int current;
};

#endif
