package coach2018;

interface TokenBuilder {
    void  append(char c);
    Token build();
}

class StringTokenBuilder implements TokenBuilder {
    private StringBuilder buff = new StringBuilder();

    @Override
    public void append(char c) {
        buff.append(c);
    }

    @Override
    public Token build() {
        return new StringToken(buff.toString());
    }
}

class NumericTokenBuilder implements TokenBuilder {
    private int length;
    private int value;

    @Override
    public void append(char c) {
        length++;
        value = 10 * value + intValue(c);
    }

    private static int intValue(char c) {
        return Character.isDigit(c) ? Character.getNumericValue(c) : c;
    }

    @Override
    public Token build() {
        return new NumericToken(length, value);
    }
}
