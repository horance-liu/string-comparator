#ifndef H12323951_26BF_4DCA_9414_119C2CECD1D1
#define H12323951_26BF_4DCA_9414_119C2CECD1D1

#include <string>

struct Token {
  virtual ~Token() {}
  int compareTo(const Token& other) const;

  static Token* newStringToken(const std::string& str);
  static Token* newNumericToken(int length, int value);

private:
  virtual int safeCompareTo(const Token& other) const = 0;
};

#endif
