package cn.com.zte.coach;

public final class StringUtils {
    private StringUtils() {
    }

    public static int compare(String s1, String s2) {
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
}
