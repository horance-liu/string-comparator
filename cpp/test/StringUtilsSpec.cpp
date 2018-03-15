#include "StringUtils.h"
#include <cut/cut.hpp>

USING_CUM_NS
USING_CUT_NS

FIXTURE(StringUtilsSpec)
{
  TEST("empty string should be equal") {
    ASSERT_THAT(compare("", ""), eq(0));
  }

  TEST("empty string compare to non empty numeric string") {
    ASSERT_THAT(compare("", "123"), eq(-1));
    ASSERT_THAT(compare("123", ""), eq(1));
  }

  TEST("empty string compare to non empty alphabetic string") {
    ASSERT_THAT(compare("", "abc"), eq(-1));
    ASSERT_THAT(compare("abc", ""), eq(1));
  }

  TEST("alphabetic fully string comparation") {
    ASSERT_THAT(compare("abc", "abc"), eq(0));
    ASSERT_THAT(compare("abc", "ab"), eq(1));
    ASSERT_THAT(compare("ab", "abc"), eq(-1));
  }

  TEST("numeric fully string comparation") {
    ASSERT_THAT(compare("123", "123"), eq(0));
    ASSERT_THAT(compare("123", "12"), eq(1));
    ASSERT_THAT(compare("12", "123"), eq(-1));
  }

  TEST("alphabetic && numeric: start with common string head") {
    ASSERT_THAT(compare("String123", "String123"), eq(0));
    ASSERT_THAT(compare("String123", "String78"), eq(1));
    ASSERT_THAT(compare("String78", "String128"), eq(-1));
  }

  TEST("alphabetic | numeric: with different string head") {
    ASSERT_THAT(compare("abcd123", "abce123"), eq(-1));
    ASSERT_THAT(compare("abce123", "abcd123"), eq(1));
  }

  TEST("numeric | alphabetic: start with common number head") {
    ASSERT_THAT(compare("123String", "123String"), eq(0));
    ASSERT_THAT(compare("123String", "123Zoo"), eq(-1));
    ASSERT_THAT(compare("123Zoo", "123String"), eq(1));
  }

  TEST("numeric | alphabetic: with different numeric head") {
    ASSERT_THAT(compare("123String", "124String"), eq(-1));
    ASSERT_THAT(compare("124String", "123String"), eq(1));
  }

  TEST("numeric | alphabetic | number") {
    ASSERT_THAT(compare("123String123", "123String123"), eq(0));
    ASSERT_THAT(compare("123String123", "123String78"), eq(1));
    ASSERT_THAT(compare("123String78", "123String123"), eq(-1));
  }

  TEST("alphabetic string: start with letter and end with letter") {
    ASSERT_THAT(compare("a\\2a", "a2a"), eq(1));
    ASSERT_THAT(compare("a\\23a", "a23a"), eq(1));
    ASSERT_THAT(compare("a\\234a", "a234a"), eq(1));
  }

  TEST("alphabetic string: start with letter and end with escape digit") {
    ASSERT_THAT(compare("a\\2", "a2123"), eq(1));
    ASSERT_THAT(compare("a\\23\\23", "a2323"), eq(1));
    ASSERT_THAT(compare("a\\234\\234", "a234234"), eq(1));
  }

  TEST("alphabetic string: start with escape digit and end with letter") {
    ASSERT_THAT(compare("\\2\\2a", "22a"), eq(1));
    ASSERT_THAT(compare("\\23\\23a", "2323a"), eq(1));
    ASSERT_THAT(compare("\\234\\234a", "234234a"), eq(1));
  }

  TEST("alphabetic string: start with escape digit and end with escape digit") {
    ASSERT_THAT(compare("\\2\\2", "22"), eq(1));
    ASSERT_THAT(compare("\\23\\23", "2323"), eq(1));
    ASSERT_THAT(compare("\\234\\234", "234234"), eq(1));
  }

  TEST("numeric string: start with digit and end with escape letters") {
    ASSERT_THAT(compare("2\\A\\A", "950"), eq(-1));
    ASSERT_THAT(compare("2\\A\\A", "915"), eq(0));
    ASSERT_THAT(compare("2\\A\\A", "900"), eq(1));
  }

  TEST("numeric string: start with digit and end with digit") {
    ASSERT_THAT(compare("2\\A\\A2", "9500"), eq(-1));
    ASSERT_THAT(compare("2\\A\\A2", "9152"), eq(0));  // same length(4)
    ASSERT_THAT(compare("2\\A\\A2", "9000"), eq(1));
  }

  TEST("numeric string: start with escape letter and end with escape letter") {
    ASSERT_THAT(compare("\\A22\\A", "65286"), eq(-1));
    ASSERT_THAT(compare("\\A22\\A", "65285"), eq(1)); // len(4) VS. len(5)
    ASSERT_THAT(compare("\\A22\\A", "65284"), eq(1));
  }

  TEST("numeric string: same value but different length") {
    // 8 * 100 + 65 * 10 + 65 = 1515(len=3) VS. 1515(len=4)
    ASSERT_THAT(compare("8\\A\\A", "1515"), eq(1));
  }

  TEST("numeric | alphabetic | numeric") {
    // 5 * 100 + 65 * 10 + 66 = 1216(len=3) | "2333" | 5 * 10 + 65 = 115(len=2)
    // 5 * 100 + 65 * 10 + 66 = 1216(len=3) | "2333" | 115(len=3)
    ASSERT_THAT(compare("5\\A\\B\\2\\3335\\A", "5\\A\\B\\2\\333115"), eq(1));
  }

  TEST("contains invalid chars") {
    ASSERT_THAT(compare("555*ddd", "555*ddd"), eq(-2));
  }

  TEST("invalid escape digits") {
    ASSERT_THAT(compare("a\\*a", "a\\*a"), eq(-2));
  }

  TEST("invalid escape letters") {
    ASSERT_THAT(compare("5\\*5", "5\\*5"), eq(-2));
  }

  TEST("same head but tail contains invalid chars") {
    ASSERT_THAT(compare("aaa123aaa123", "aaa123aaa123***"), eq(-2));
  }
};
