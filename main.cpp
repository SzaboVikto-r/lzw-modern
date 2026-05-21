#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <string_view>

class LZWBinFa {
    struct Csom {
        char c;
        std::unique_ptr<Csom> nulla, egy;
        explicit Csom(char c) noexcept : c(c) {}
    };

    Csom gyoker{'/'};
    Csom* aktualis{&gyoker};
    mutable int melyseg{};

    void kiirRek(const Csom* n, std::ostream& os, int m) const {
        if (!n) return;
        ++melyseg;
        kiirRek(n->egy.get(), os, m + 1);
        for (int i = 0; i < melyseg; ++i) os << "---";
        os << n->c << '(' << melyseg - 1 << ")\n";
        kiirRek(n->nulla.get(), os, m + 1);
        --melyseg;
    }

    void rMelyseg(const Csom* n, int& max, int m) const {
        if (!n) return;
        if (m > max) max = m;
        rMelyseg(n->egy.get(),   max, m + 1);
        rMelyseg(n->nulla.get(), max, m + 1);
    }

    void rAtlag(const Csom* n, int& ossz, int& db, int m) const {
        if (!n) return;
        if (!n->egy && !n->nulla) { ossz += m; ++db; return; }
        rAtlag(n->egy.get(),   ossz, db, m + 1);
        rAtlag(n->nulla.get(), ossz, db, m + 1);
    }

    void rSzoras(const Csom* n, double atl, double& ossz, int& db, int m) const {
        if (!n) return;
        if (!n->egy && !n->nulla) {
            ossz += (m - atl) * (m - atl); ++db; return;
        }
        rSzoras(n->egy.get(),   atl, ossz, db, m + 1);
        rSzoras(n->nulla.get(), atl, ossz, db, m + 1);
    }

public:
    void operator<<(char b) {
        auto& gyermek = (b == '0') ? aktualis->nulla : aktualis->egy;
        if (!gyermek) {
            gyermek = std::make_unique<Csom>(b);
            aktualis = &gyoker;
        } else {
            aktualis = gyermek.get();
        }
    }

    void kiir(std::ostream& os) const {
        melyseg = 0;
        kiirRek(&gyoker, os, 0);
    }

    int getMelyseg() const {
        int m = 0;
        rMelyseg(&gyoker, m, 0);
        return m - 1;
    }

    double getAtlag() const {
        int ossz = 0, db = 0;
        rAtlag(&gyoker, ossz, db, 0);
        return db ? static_cast<double>(ossz) / db : 0.0;
    }

    double getSzoras() const {
        double atl = getAtlag();
        double ossz = 0.0; int db = 0;
        rSzoras(&gyoker, atl, ossz, db, 0);
        return db > 1 ? std::sqrt(ossz / (db - 1)) : std::sqrt(ossz);
    }

    friend std::ostream& operator<<(std::ostream& os, const LZWBinFa& bf) {
        bf.kiir(os); return os;
    }
};

static void usage(std::string_view prog) {
    std::cout << "Hasznalat: " << prog << " bemenet -o kimenet\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) { usage(argv[0]); return -1; }

    std::string_view inNev    = argv[1];
    std::string_view kapcsolo = argv[2];
    std::string_view outNev   = argv[3];

    if (kapcsolo != "-o") { usage(argv[0]); return -2; }

    std::ifstream be(inNev.data(), std::ios::binary);
    if (!be) { std::cerr << inNev << " nem letezik\n"; return -3; }
    std::ofstream ki(outNev.data());

    for (unsigned char b; be.read(reinterpret_cast<char*>(&b), 1) && b != 0x0a;) {}

    LZWBinFa fa;
    bool komment = false;
    unsigned char b;

    while (be.read(reinterpret_cast<char*>(&b), 1)) {
        if (b == '>') { komment = true;  continue; }
        if (b == '\n') { komment = false; continue; }
        if (komment || b == 'N') continue;
        for (int i = 7; i >= 0; --i)
            fa << ((b >> i & 1) ? '1' : '0');
    }

    ki << fa;
    ki << "depth = " << fa.getMelyseg() << '\n';
    ki << "mean = "  << fa.getAtlag()   << '\n';
    ki << "var = "   << fa.getSzoras()  << '\n';
}
