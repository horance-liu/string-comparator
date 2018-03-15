package coach2018

import spock.lang.Specification

import static StringUtils.compare

class StringUtilsSpec extends Specification {
    def "empty string should be equal"() {
        expect:
        compare("", "") == 0
    }

    def "empty string compare to non empty numeric string"() {
        expect:
        compare("", "123") == -1
        compare("123", "") == 1
    }

    def "empty string compare to non empty alphabetic string"() {
        expect:
        compare("", "abc") == -1
        compare("abc", "") == 1
    }

    def "alphabetic fully string comparation"() {
        expect:
        compare("abc", "abc") == 0
        compare("abc", "ab")  == 1
        compare("ab",  "abc") == -1
    }

    def "numeric fully string comparation"() {
        expect:
        compare("123", "123") == 0
        compare("123", "12")  == 1
        compare("12",  "123") == -1
    }

    def "alphabetic && numeric: start with common string head"() {
        expect:
        compare("String123", "String123") == 0
        compare("String123", "String78") == 1
        compare("String78",  "String128") == -1
    }

    def "alphabetic | numeric: with different string head"() {
        expect:
        compare("abcd123", "abce123") == -1
        compare("abce123", "abcd123") == 1
    }

    def "numeric | alphabetic: start with common number head"() {
        expect:
        compare("123String", "123String") == 0
        compare("123String", "123Zoo") == -1
        compare("123Zoo", "123String") == 1
    }

    def "numeric | alphabetic: with different numeric head"() {
        expect:
        compare("123String", "124String") == -1
        compare("124String", "123String") == 1
    }

    def "numeric | alphabetic | number"() {
        expect:
        compare("123String123", "123String123") == 0
        compare("123String123", "123String78") == 1
        compare("123String78", "123String123") == -1
    }

    def "alphabetic string: start with letter and end with letter"() {
        expect:
        compare("a\\2a", "a2a") == 1
        compare("a\\23a", "a23a") == 1
        compare("a\\234a", "a234a") == 1
    }

    def "alphabetic string: start with letter and end with escape digit"() {
        expect:
        compare("a\\2", "a2123") == 1
        compare("a\\23\\23", "a2323") == 1
        compare("a\\234\\234", "a234234") == 1
    }

    def "alphabetic string: start with escape digit and end with letter"() {
        expect:
        compare("\\2\\2a", "22a") == 1
        compare("\\23\\23a", "2323a") == 1
        compare("\\234\\234a", "234234a") == 1
    }

    def "alphabetic string: start with escape digit and end with escape digit"() {
        expect:
        compare("\\2\\2", "22") == 1
        compare("\\23\\23", "2323") == 1
        compare("\\234\\234", "234234") == 1
    }

    def "numeric string: start with digit and end with escape letters"() {
        expect:
        compare("2\\A\\A", "950") == -1
        compare("2\\A\\A", "915") == 0
        compare("2\\A\\A", "900") == 1
    }

    def "numeric string: start with digit and end with digit"() {
        expect:
        compare("2\\A\\A2", "9500") == -1
        compare("2\\A\\A2", "9152") == 0  // same value and length
        compare("2\\A\\A2", "9000") == 1
    }

    def "numeric string: start with escape letter and end with escape letter"() {
        expect:
        compare("\\A22\\A", "65286") == -1
        compare("\\A22\\A", "65285") == 1  // same value, but len(4) VS. len(5)
        compare("\\A22\\A", "65284") == 1
    }

    def "numeric string: same value but different length"() {
        expect:
        // 8 * 100 + 65 * 10 + 65 = 1515(len=3) VS. 1515(len=4)
        compare("8\\A\\A", "1515") == 1
    }

    def "numeric | alphabetic | numeric"() {
        expect:
        // 5 * 100 + 65 * 10 + 66 = 1216(len=3) | "2333" | 5 * 10 + 65 = 115(len=2)
        // 5 * 100 + 65 * 10 + 66 = 1216(len=3) | "2333" | 115(len=3)
        compare("5\\A\\B\\2\\3335\\A", "5\\A\\B\\2\\333115") == 1
    }

    def "contains invalid chars"() {
        expect:
        compare("555*ddd", "555*ddd") == -2
    }

    def "invalid escape digits"() {
        expect:
        compare("a\\*a", "a\\*a") == -2
    }

    def "invalid escape letters"() {
        expect:
        compare("5\\*5", "5\\*5") == -2
    }

    def "same head, but tail contains invalid chars"() {
        expect:
        compare("aaa123aaa123", "aaa123aaa123***") == -2
    }
}
