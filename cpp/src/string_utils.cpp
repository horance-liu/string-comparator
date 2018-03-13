#include "string_utils.h"
#include <map>
#include <unique_ptr>
#include <stdlib.h>

namespace 
{
  inline int compare(int x, int y) 
  {
  	return (x < y) ? -1 : ((x == y) ? 0 : 1);
  }

  struct Token
  {
  	virtual ~Token() {}

    int compareTo(const Token& token) const 
    {
      auto priority = compare(
          priorities[typeid(this).name()],
          priorities[typeid(token).name()]);
      return priority == 0 ? safeCompareTo(other) : priority;
    }

  private:
    std::map<std::string, int> priorities;

  private:
    virtual int safeCompareTo() const = 0;
  };

  struct StringToken : Token
  {
  	explicit StringToken(const std::string& str) : str(str) 
  	{}

  private:
    int safeCompareTo(const Token& token) const override
    {
      auto& other = dynamic_cast<const StringToken&>(token);
      return compare(str.compare(other.str), 0);
    }

  private:
    std::string str;
  };

  struct NumericToken : Token
  {
    NumericToken(int length, int value)
      : length(length), value(value)
    {}

  private:
    int safeCompareTo(const Token& token) const override
    {
      auto& other = dynamic_cast<const NumericToken&>(token);
      int result = compare(value, other.value);
      return (result == 0) ? compare(length, other.length) : result;
    }

  private:
    int length;
    int value;
  };

  std::map<std::string, int> Token::priorities =
  {
    { typeid(NumericToken).name(), 1 },
    { typeid(StringToken).name(),  2 },
  };

  struct StringTokenizer
  {
    explicit StringTokenizer(const std::string& str) : str(str)
    {}

    bool hasMoreTokens() const 
    {
      return isInRange(current);
    }

    bool hasRemained() const 
    {
      assertValidContents(current);
      return isInRange(current);
    }

    std::unique_ptr<Token> nextToken() 
    {
      assertValidLetter(current);
      auto scanner = std::make_unique(newTokenScanner());
      return std::make_unique(scanner->nextToken());
    }

  private:
    void assertValidContents(int start) const
    {
	  while (isInRange(start)) 
	  {
	    assertValidLetter(start);
	    start++;
	  }
    }

    void assertValidLetter(int pos) const 
    {
      bool isValid = beginWithLetter(pos) || beginWithDigit(pos);
      if (!isValid) 
        throw std::invalid_argument("contains invalid char: " + str[pos]);
    }

    bool beginWithLetter(int pos) const 
    {
      return isLetter(pos) || (isSlash(pos) && isDigit(pos + 1));
    }

    bool beginWithDigit(int pos) const 
    {
      return isDigit(pos) || (isSlash(pos) && isLetter(pos + 1));
    }

    TokenScanner* newTokenScanner() const 
    {
      return beginWithLetter(current) ?
          new StringTokenScanner(str) : new NumericTokenScanner(str);
    }

    bool isInRange(int pos) const 
    {
      return pos < str.size();
    }

    bool isSlash(int pos) const 
    {
      return isInRange(pos) && str[pos] == '\\';
    }

    bool isLetter(int pos) const 
    {
      return isInRange(pos) && std::isalpha(str[pos]);
    }

    bool isDigit(int pos) const
    {
      return isInRange(pos) && std::isdigit(str[pos]);
    }

  private:
    const std::string& str;
    int current;
  };

  struct TokenScanner
  {
  	TokenScanner(TokenBuilder* buff, const String& str)
  	  : buff(buff), str(str)
  	{}

  	~TokenScanner() 
  	{
  	  delete buff;
  	}

  	Token* nextToken() const {
      scanToken();
      return buff->build();
    }

  private:
    void scanToken() const
    {
      int pos = current;
      while (shouldForward(pos)) 
      {
        pos = scanValidChars(pos);
        pos = scanEscapeChars(pos);
      }
      current = pos;
    }

    int scanValidChars(int start) const
    {
      int pos = start;
      if (isValid(pos)) 
      {
        buff->append(str[pos]);
        pos++;
      }
      return pos;
    }

    int scanEscapeChars(int pos) const
    {
        if (containsEscape(pos)) 
        {
            pos = escape(pos + 1);
        }
        return pos;
    }

    bool shouldForward(int pos) const
    {
        return isValid(pos) || containsEscape(pos);
    }

    bool containsEscape(int pos) const
    {
        return isSlash(pos) && isEscape(pos + 1);
    }

    bool isSlash(int pos) const
    {
        return isInRange(pos) && str[pos] == '\\';
    }

  private:
    virtual bool isValid(int pos) const = 0;
    virtual bool isEscape(int pos) const = 0;
    virtual int escape(int pos) const = 0;

  protected:
    TokenBuilder* buff;
    const String& str;
  };

  struct StringTokenScanner extends TokenScanner {
    StringTokenScanner(const std::string& str)
      : TokenScanner(new StringTokenBuilder(), str)
    {}

  private:
    bool isValid(int pos) const override 
    {
        return isInRange(pos) && isLetter(pos);
    }

    bool isEscape(int pos) const override
    {
        return isInRange(pos) && isDigit(pos);
    }

    enum { MAX_NUM_LENGTH = 3 };

    int escape(int pos) const override
    {
        int i = 0;
        while (i < MAX_NUM_LENGTH && isEscape(pos + i)) {
            buff->append(str.charAt(pos + i));
            i++;
        }
        return pos + i;
    }
  };

  struct NumericTokenScanner : TokenScanner 
  {
    NumericTokenScanner(const std::string& str)
     : TokenScanner(new NumericTokenBuilder(), str)
    {}

  private:
    bool isValid(int pos) const override
    {
        return isInRange(pos) && isDigit(pos);
    }

    bool isEscape(int pos) const override
    {
        return isInRange(pos) && isLetter(pos);
    }

    int escape(int pos) const override
    {
        buff->append(str.charAt(pos));
        return pos + 1;
    }
  };

  struct TokenBuilder
  {
	virtual ~TokenBuilder() {}
	virtual void  append(char c) = 0;
	virtual Token* build() = 0;
  };

  struct StringTokenBuilder : TokenBuilder
  {
    void  append(char c) override
    {
      str += c;
    }

    Token* build() override
    {
      return new StringToken(str);
    }

    std::string str;
  };

  struct NumericTokenBuilder : TokenBuilder
  {
     NumericTokenBuilder() : length(0), value(0)
     {}

  private:
    void append(char c) override
    {
        length++;
        value = 10 * value + intValue(c);
    }

    static int intValue(char c) {
        return std::isdigit(c) ? c - '0' : c;
    }

    Token* build() override
    {
        return new NumericToken(length, value);
    }

    int length;
    int value;
  };
}

int compare(const std::string& s1, const std::string& s2) 
{
  StringTokenizer st1(s1), st2(s2);
  
  while (st1.hasMoreTokens() && st2.hasMoreTokens()) 
  {
  	auto result = st1.nextToken()->compareTo(*st1.nextToken());
    if (result != 0) 
      return result;
  }

  if (st1.hasRemained()) return 1;
  if (st2.hasRemained()) return -1;
  return 0;
}