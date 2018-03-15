#include "StringUtils.h"
#include "internal/StringTokenizer.h"
#include "internal/Token.h"

namespace {
  /**
   * @return the value 0 if s1 == s2; -1 if s1 < s2; and 1 if s1 > s2.
   * @throw std::invalid_argument if contains invalid characters.
   */
  int doCompare(const std::string& s1, const std::string& s2) {
    StringTokenizer st1(s1), st2(s2);

    while (st1.hasMoreTokens() && st2.hasMoreTokens()) {
      std::unique_ptr<Token> t1(st1.nextToken()), t2(st2.nextToken());
      if (auto result = t1->compareTo(*t2)) {
        return result;
      }
    }

    if (st1.hasRemained()) return 1;
    if (st2.hasRemained()) return -1;
    return 0;
  }
}

int compare(const std::string& s1, const std::string& s2) {
  try {
    return doCompare(s1, s2);
  } catch (const std::invalid_argument&) {
    return -2;
  }
}
