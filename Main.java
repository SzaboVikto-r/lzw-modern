import java.io.*;

class Main {
    public static void main(String[] args) throws IOException {
        if (args.length != 3) {
            System.err.println("Usage: lzwtree in_file -o out_file");
            System.exit(-1);
        }
        if (!args[1].equals("-o")) {
            System.err.println("Usage: lzwtree in_file -o out_file");
            System.exit(-2);
        }

        LZWBinFa tree = new LZWBinFa();
        boolean comment = false;
        int b;

        try (InputStream in = new FileInputStream(args[0])) {
            while ((b = in.read()) != -1 && b != '\n') {}
            while ((b = in.read()) != -1) {
                if (b == '>') { comment = true; continue; }
                if (b == '\n') { comment = false; continue; }
                if (comment || b == 'N') continue;
                for (int mask = 0x80; mask > 0; mask >>= 1)
                    tree.write((b & mask) != 0 ? '1' : '0');
            }
        }

        try (PrintWriter out = new PrintWriter(new FileWriter(args[2]))) {
            tree.print(out);
            out.println("depth = " + tree.getMelyseg());
            out.println("mean = " + tree.getAtlag());
            out.println("var = " + tree.getSzoras());
        }
    }
}
