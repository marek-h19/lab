#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

static bool recursion(std::size_t i, long long remaining,
                      const std::vector<int>& vals,
                      const std::vector<long long>& suffix,
                      std::vector<int>& chosen, long long& calls) {
    
    ++calls;
    if (remaining < 0)         return false;  // orezanie 1: prestrelili sme S
    if (remaining == 0)        return true;   // nájdené
    if (i == vals.size())      return false;  // došli prvky
    if (remaining > suffix[i]) return false;  // orezanie 2: zvyšné prvky nestačia

    chosen.push_back(vals[i]);
    if (recursion(i + 1, remaining - vals[i], vals, suffix, chosen, calls))
        return true;
    chosen.pop_back();
    return recursion(i + 1, remaining, vals, suffix, chosen, calls);
}

bool solveImproved(const std::vector<int>& A, long long S,
                   std::vector<int>& chosen, long long& calls) {
    std::vector<int> vals = A;                                  // kópia, A ostane nezmenené
    std::sort(vals.begin(), vals.end(), std::greater<int>());   // zostupne, raz

    std::vector<long long> suffix(vals.size() + 1, 0);          // suffix[n] = 0
    for (std::size_t k = vals.size(); k-- > 0; )
        suffix[k] = suffix[k + 1] + vals[k];

    calls = 0;
    chosen.clear();
    return recursion(0, S, vals, suffix, chosen, calls);
}

int main() {
    std::vector<int> A = {3, 7, 11, 15, 19};
    long long S = 30, calls = 0;
    std::vector<int> chosen;

    if (solveImproved(A, S, chosen, calls)) {
        for (int x : chosen) std::cout << x << ' ';
        std::cout << '\n';
    } else {
        std::cout << "no solution\n";
    }
    std::cout << "Number of calls: " << calls << '\n';
}