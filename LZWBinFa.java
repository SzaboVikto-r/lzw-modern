import java.io.PrintWriter;

class LZWBinFa {
    private final Node root = new Node('/');
    private Node cur = root;

    void write(char b) {
        if (b == '0') {
            if (cur.left == null) { cur.left = new Node('0'); cur = root; }
            else cur = cur.left;
        } else {
            if (cur.right == null) { cur.right = new Node('1'); cur = root; }
            else cur = cur.right;
        }
    }

    void print(PrintWriter pw) { print(root, pw, 0); }

    private void print(Node n, PrintWriter pw, int d) {
        if (n == null) return;
        print(n.right, pw, d + 1);
        for (int i = 0; i <= d; i++) pw.print("---");
        pw.println(n.val + "(" + d + ")");
        print(n.left, pw, d + 1);
    }

    int getMelyseg() { return maxD(root, 0) - 1; }

    private int maxD(Node n, int d) {
        if (n == null) return d;
        return Math.max(maxD(n.left, d + 1), maxD(n.right, d + 1));
    }

    double getAtlag() {
        int[] s = {0, 0};
        leafSum(root, 0, s);
        return (double) s[0] / s[1];
    }

    private void leafSum(Node n, int d, int[] s) {
        if (n == null) return;
        if (n.left == null && n.right == null) { s[0] += d; s[1]++; return; }
        leafSum(n.left, d + 1, s);
        leafSum(n.right, d + 1, s);
    }

    double getSzoras() {
        double m = getAtlag();
        double[] ss = {0};
        int[] cnt = {0};
        leafVar(root, 0, m, ss, cnt);
        return cnt[0] > 1 ? Math.sqrt(ss[0] / (cnt[0] - 1)) : Math.sqrt(ss[0]);
    }

    private void leafVar(Node n, int d, double m, double[] ss, int[] cnt) {
        if (n == null) return;
        if (n.left == null && n.right == null) {
            ss[0] += (d - m) * (d - m); cnt[0]++; return;
        }
        leafVar(n.left, d + 1, m, ss, cnt);
        leafVar(n.right, d + 1, m, ss, cnt);
    }
}
