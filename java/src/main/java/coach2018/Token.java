package coach2018;

import java.util.HashMap;
import java.util.Map;

import static java.lang.Integer.compare;

abstract class Token implements Comparable<Token> {
    private static Map<Class<?>, Integer> priorities = new HashMap<Class<?>, Integer>() {{
        put(NumericToken.class, 1);
        put(StringToken.class,  2);
    }};

    @Override
    public int compareTo(Token other) {
        int priority = compare(
                priorities.get(getClass()),
                priorities.get(other.getClass()));
        return priority == 0 ? safeCompareTo(other) : priority;
    }

    abstract protected int safeCompareTo(Token other);
}

class StringToken extends Token {
    private String str;

    StringToken(String str) {
        this.str = str;
    }

    @Override
    protected int safeCompareTo(Token token) {
        StringToken other = (StringToken)token;
        return compare(str.compareTo(other.str), 0);
    }
}

class NumericToken extends Token {
    private int length;
    private int value;

    public NumericToken(int length, int value) {
        this.length = length;
        this.value = value;
    }

    @Override
    protected int safeCompareTo(Token token) {
        NumericToken other = (NumericToken)token;
        int result = compare(value, other.value);
        return result == 0 ? compare(other.length, length) : result;
    }
}
