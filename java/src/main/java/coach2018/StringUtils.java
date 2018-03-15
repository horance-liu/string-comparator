package coach2018;

public final class StringUtils {
    public static int compare(String s1, String s2) {
        try {
            return doCompare(s1, s2);
        } catch (IllegalArgumentException e) {
            return -2;
        }
    }

    private static int doCompare(String s1, String s2) {
        StringTokenizer st1 = new StringTokenizer(s1);
        StringTokenizer st2 = new StringTokenizer(s2);

        while (st1.hasMoreTokens() && st2.hasMoreTokens()) {
            int result = st1.nextToken().compareTo(st2.nextToken());
            if (result != 0)
                return result;
        }

        if (st1.hasRemained()) return 1;
        if (st2.hasRemained()) return -1;
        return 0;
    }

    private StringUtils() {
    }
}
