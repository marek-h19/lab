#include <iostream>
#include <vector>

static bool recursionA(std::size_t i, long long remaining, const std::vector<int>& A,
                       std::vector<int>& chosen, long long& calls) {
    ++calls; // pocet rekurzivnych volani

    if (i == A.size())             
        return remaining == 0;

    chosen.push_back(A[i]);      
    if (recursionA(i + 1, remaining - A[i], A, chosen, calls))
        return true;
    chosen.pop_back(); // backtrack

    return recursionA(i + 1, remaining, A, chosen, calls);   // preskoc A[i]
}

bool solveBaseline(const std::vector<int>& A, long long S,
                   std::vector<int>& chosen, long long& calls) {
    calls = 0;
    chosen.clear();
    return recursionA(0, S, A, chosen, calls);
}

static void demo(const std::vector<int>& A, long long S) {
    std::vector<int> chosen;
    long long calls = 0;
    if (solveBaseline(A, S, chosen, calls)) {
        std::cout << "solution:";
        for (int x : chosen) std::cout << ' ' << x;
    } else {
        std::cout << "no solution";
    }
    std::cout << ", calls = " << calls << '\n';
}

int main() {
    demo({3, 7, 11, 15, 19}, 30);   // ma riesenie
    demo({4, 8, 12, 16, 20}, 30);   // nema riesenie 
    return 0;
}