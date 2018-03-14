#ifndef H7CC5C448_A9A2_4D68_BD1E_5B6B13E99A30
#define H7CC5C448_A9A2_4D68_BD1E_5B6B13E99A30

#include <string>

struct StringBuffer {
  explicit StringBuffer(const std::string&);

  bool startWithLetter(int pos) const;
  bool startWithDigit(int pos) const;

  bool isInRange(int pos) const;
  bool isSlash(int pos) const;
  bool isLetter(int pos) const;
  bool isDigit(int pos) const;

  char operator[](int pos) const;

private:
  const std::string& buff;
};

#endif
