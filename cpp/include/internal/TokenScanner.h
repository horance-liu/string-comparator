#ifndef HBC83B4B1_AEC4_4E0A_B075_C48FFF31228D
#define HBC83B4B1_AEC4_4E0A_B075_C48FFF31228D

#include <string>

struct Token;
struct TokenBuilder;
struct StringBuffer;

struct TokenScanner {
  TokenScanner(TokenBuilder* buff, const StringBuffer& str);
  ~TokenScanner();

  Token* nextToken(int& pos) const;

  static TokenScanner* newScanner(const StringBuffer& str, bool numeric);

private:
  virtual bool isValid(int pos) const = 0;
  virtual bool isEscape(int pos) const = 0;
  virtual int escape(int pos) const = 0;

private:
  void scanToken(int& pos) const;
  void scanValidChars(int& pos) const;
  void scanEscapeChars(int& pos) const;

  bool shouldForward(int pos) const;
  bool containsEscape(int pos) const;
  bool isSlash(int pos) const;

protected:
  TokenBuilder* buff;
  const StringBuffer& str;
};

#endif
