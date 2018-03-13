package cn.com.zte.coach;

class StringTokenizer {
    private String str;
    private int current;

    public StringTokenizer(String str) {
        this.str = str;
        this.current = 0;
    }

    public boolean hasMoreTokens() {
        return isInRange(current);
    }

    public boolean hasRemained() {
        assertValidContents(current);
        return isInRange(current);
    }

    public Token nextToken() {
        assertValidLetter(current);
        TokenScanner scanner = newTokenScanner();
        return scanner.nextToken();
    }

    private void assertValidContents(int start) {
        while (isInRange(start)) {
            assertValidLetter(start);
            start++;
        }
    }

    private void assertValidLetter(int pos) {
        boolean isValid = beginWithLetter(pos) || beginWithDigit(pos);
        if (!isValid)
            throw new IllegalArgumentException("contains invalid char: " + str.charAt(pos));
    }

    private boolean beginWithLetter(int pos) {
        return isLetter(pos) || (isSlash(pos) && isDigit(pos + 1));
    }

    private boolean beginWithDigit(int pos) {
        return isDigit(pos) || (isSlash(pos) && isLetter(pos + 1));
    }

    private TokenScanner newTokenScanner() {
        return beginWithLetter(current) ?
                new StringTokenScanner() : new NumericTokenScanner();
    }

    private boolean isInRange(int pos) {
        return pos < str.length();
    }

    private boolean isSlash(int pos) {
        return isInRange(pos) && str.charAt(pos) == '\\';
    }

    private boolean isLetter(int pos) {
        return isInRange(pos) && Character.isLetter(str.charAt(pos));
    }

    private boolean isDigit(int pos) {
        return isInRange(pos) && Character.isDigit(str.charAt(pos));
    }

    abstract class TokenScanner {
        protected TokenBuilder buff;

        protected TokenScanner(TokenBuilder buff) {
            this.buff = buff;
        }

        public Token nextToken() {
            scanToken();
            return buff.build();
        }

        private void scanToken() {
            int pos = current;
            while (shouldForward(pos)) {
                pos = scanValidChars(pos);
                pos = scanEscapeChars(pos);
            }
            current = pos;
        }

        private int scanValidChars(int start) {
            int pos = start;
            if (isValid(pos)) {
                buff.append(str.charAt(pos));
                pos++;
            }
            return pos;
        }

        private int scanEscapeChars(int pos) {
            if (containsEscape(pos)) {
                pos = escape(pos + 1);
            }
            return pos;
        }

        private boolean shouldForward(int pos) {
            return isValid(pos) || containsEscape(pos);
        }

        private boolean containsEscape(int pos) {
            return isSlash(pos) && isEscape(pos + 1);
        }

        private boolean isSlash(int pos) {
            return isInRange(pos) && str.charAt(pos) == '\\';
        }

        abstract protected boolean isValid(int pos);
        abstract protected boolean isEscape(int pos);
        abstract protected int escape(int pos);
    }

    class StringTokenScanner extends TokenScanner {
        public StringTokenScanner() {
            super(new StringTokenBuilder());
        }

        @Override
        protected boolean isValid(int pos) {
            return isInRange(pos) && isLetter(pos);
        }

        @Override
        protected boolean isEscape(int pos) {
            return isInRange(pos) && isDigit(pos);
        }

        private final static int MAX_NUM_LENGTH = 3;

        @Override
        protected int escape(int pos) {
            int i = 0;
            while (i < MAX_NUM_LENGTH && isEscape(pos + i)) {
                buff.append(str.charAt(pos + i));
                i++;
            }
            return pos + i;
        }
    }

    class NumericTokenScanner extends TokenScanner {
        public NumericTokenScanner() {
            super(new NumericTokenBuilder());
        }

        @Override
        protected boolean isValid(int pos) {
            return isInRange(pos) && isDigit(pos);
        }

        @Override
        protected boolean isEscape(int pos) {
            return isInRange(pos) && isLetter(pos);
        }

        @Override
        protected int escape(int pos) {
            buff.append(str.charAt(pos));
            return pos + 1;
        }
    }
}
