#include "StringUtils.h"
#include "internal/StringTokenizer.h"
#include "internal/Token.h"
#include <iostream>

int compare(const std::string& s1, const std::string& s2) {
  StringTokenizer st1(s1);
  StringTokenizer st2(s2);

  while (st1.hasMoreTokens() && st2.hasMoreTokens()) {
    std::unique_ptr<Token> t1(st1.nextToken());
    std::unique_ptr<Token> t2(st2.nextToken());

    if (auto result = t1->compareTo(*t2)) {
      return result;
    }
  }

  if (st1.hasRemained()) return 1;
  if (st2.hasRemained()) return -1;
  return 0;
}
