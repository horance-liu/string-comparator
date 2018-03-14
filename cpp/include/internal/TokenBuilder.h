#ifndef H9684E545_48FE_483D_99BA_4034E58677AD
#define H9684E545_48FE_483D_99BA_4034E58677AD

struct Token;

struct TokenBuilder {
  virtual ~TokenBuilder() {}
  virtual void append(char c) = 0;
  virtual Token* build() = 0;

  static TokenBuilder* newStringTokenBuilder();
  static TokenBuilder* newNumericTokenBuilder();
};

#endif
