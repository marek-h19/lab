// Kompilacia (baseline.cpp a improved.cpp sa NEMENIA, ich main sa iba premenuje):
//   g++ -std=c++17 -O2 -c baseline.cpp -Dmain=baseline_main
//   g++ -std=c++17 -O2 -c improved.cpp -Dmain=improved_main
//   g++ -std=c++17 -O2 experiments.cpp baseline.o improved.o -o experiments
//
// Spustenie:
//   ./experiments                    vygeneruje testy zo SEED a ulozi ich do instances.txt
//   ./experiments instances.txt      nacita ulozene testy (presne zopakovanie)

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// z baseline.cpp a improved.cpp
bool solveBaseline(const std::vector<int>& A, long long S, std::vector<int>& chosen, long long& calls);
bool solveImproved(const std::vector<int>& A, long long S, std::vector<int>& chosen, long long& calls);

// nastavenia 
const std::uint64_t SEED = 2026;                 
const std::vector<int> SIZES = {10, 15, 20, 25};
const int MAX_VALUE = 1000;// hodnoty su z <1, MAX_VALUE>
const double TIME_LIMIT_MS = 10000; // 10 s na jeden beh
const int REPEATS = 3; 

struct Instance {
    std::string name; 
    long long S;
    std::vector<int> A;
};

// generovanie
std::mt19937_64 rng(SEED);   // mt19937_64 dava pri rovnakom seede vsade rovnake cisla

long long randomBetween(long long lo, long long hi) {
    return lo + (long long)(rng() % (std::uint64_t)(hi - lo + 1));
}

std::vector<Instance> generateInstances() {
    std::vector<Instance> list;
    for (int n : SIZES) {
        std::string prefix = "n" + std::to_string(n) + "_";

        // 1) solvable: S = sucet nahodne vybranej podmnoziny, riesenie urcite existuje
        Instance solvable{prefix + "solvable", 0, {}};
        for (int k = 0; k < n; ++k) {
            solvable.A.push_back((int)randomBetween(1, MAX_VALUE));
            if (randomBetween(0, 1) == 1) solvable.S += solvable.A[k];
        }
        if (solvable.S == 0) solvable.S = solvable.A[0];
        list.push_back(solvable);

        // 2) random: nahodne S z [1, sucet], nevieme vopred, ci riesenie existuje
        Instance random{prefix + "random", 0, {}};
        long long total = 0;
        for (int k = 0; k < n; ++k) {
            random.A.push_back((int)randomBetween(1, MAX_VALUE));
            total += random.A[k];
        }
        random.S = randomBetween(1, total);
        list.push_back(random);

        // 3) unsat: vsetko delitelne 4, S = 2 (mod 4), riesenie neexistuje
        Instance unsat{prefix + "unsat", 0, {}};
        total = 0;
        for (int k = 0; k < n; ++k) {
            unsat.A.push_back(4 * (int)randomBetween(1, MAX_VALUE / 4));
            total += unsat.A[k];
        }
        long long half = total / 2;          // S okolo polovice suctu, aby to bolo tazke
        unsat.S = half - half % 4 + 2;
        list.push_back(unsat);
    }
    return list;
}

// ulozenie / nacitanie 
// riadok v subore:  meno  S  n  a1 a2 ... an
void saveInstances(const std::string& file, const std::vector<Instance>& list) {
    std::ofstream out(file);
    for (const Instance& in : list) {
        out << in.name << ' ' << in.S << ' ' << in.A.size();
        for (int x : in.A) out << ' ' << x;
        out << '\n';
    }
}

std::vector<Instance> loadInstances(const std::string& file) {
    std::ifstream inFile(file);
    std::vector<Instance> list;
    Instance in;
    std::size_t n;
    while (inFile >> in.name >> in.S >> n) {
        in.A.assign(n, 0);
        for (std::size_t k = 0; k < n; ++k) inFile >> in.A[k];
        list.push_back(in);
    }
    return list;
}

// meranie
struct Result {
    bool found = false;
    bool timeout = false;
    long long calls = 0;
    double ms = 0;               // median casu
    std::vector<int> subset;
};

using Solver = bool (*)(const std::vector<int>&, long long, std::vector<int>&, long long&);

Result measure(Solver solve, const Instance& in) {
    Result r;
    std::vector<double> times;
    for (int rep = 0; rep < REPEATS; ++rep) {
        auto start = std::chrono::steady_clock::now();
        r.found = solve(in.A, in.S, r.subset, r.calls);
        auto end = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();

        if (ms > TIME_LIMIT_MS) {    // prekrocil limit -> TIMEOUT
            r.timeout = true;
            r.ms = ms;
            return r;
        }
        times.push_back(ms);
    }
    std::sort(times.begin(), times.end());
    r.ms = times[REPEATS / 2];       // median
    return r;
}

long long sum(const std::vector<int>& v) {
    long long s = 0;
    for (int x : v) s += x;
    return s;
}

std::string foundText(const Result& r) {
    if (r.timeout) return "TIMEOUT";
    return r.found ? "yes" : "no";
}

int main(int argc, char** argv) {
    std::vector<Instance> list;
    if (argc > 1) {
        list = loadInstances(argv[1]);
        std::cout << "Loaded " << list.size() << " instances from " << argv[1] << "\n";
    } else {
        list = generateInstances();
        saveInstances("instances.txt", list);
        std::cout << "Seed " << SEED << ", " << list.size() << " instances saved to instances.txt\n";
    }

    std::ofstream csv("results.csv");
    csv << "instance,n,S,A_found,A_calls,A_ms,B_found,B_calls,B_ms\n";

    std::cout << std::left << std::setw(15) << "instance" << std::right
              << std::setw(4) << "n" << std::setw(8) << "S"
              << std::setw(9) << "A found" << std::setw(12) << "A calls" << std::setw(11) << "A ms"
              << std::setw(9) << "B found" << std::setw(12) << "B calls" << std::setw(11) << "B ms"
              << '\n';

    for (const Instance& in : list) {
        Result a = measure(solveBaseline, in);
        Result b = measure(solveImproved, in);

        std::cout << std::left << std::setw(15) << in.name << std::right
                  << std::setw(4) << in.A.size() << std::setw(8) << in.S << std::fixed << std::setprecision(4)
                  << std::setw(9) << foundText(a) << std::setw(12) << a.calls << std::setw(11) << a.ms
                  << std::setw(9) << foundText(b) << std::setw(12) << b.calls << std::setw(11) << b.ms
                  << '\n';

        csv << in.name << ',' << in.A.size() << ',' << in.S << ','
            << foundText(a) << ',' << a.calls << ',' << a.ms << ','
            << foundText(b) << ',' << b.calls << ',' << b.ms << '\n';

        // kontrola spravnosti
        if (a.found && sum(a.subset) != in.S) std::cout << "  !! A: subset does not sum to S\n";
        if (b.found && sum(b.subset) != in.S) std::cout << "  !! B: subset does not sum to S\n";
        if (!a.timeout && !b.timeout && a.found != b.found) std::cout << "  !! A and B disagree\n";
    }
    std::cout << "Results saved to results.csv\n";
    return 0;
}