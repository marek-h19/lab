#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

static bool recursionB(std::size_t i, long long remaining, const std::vector<int>& vals,
                       const std::vector<long long>& suffix,
                       std::vector<int>& chosen, long long& calls) {
    ++calls;                                  

    if (remaining < 0)         return false; // orezanie 1
    if (remaining == 0)        return true;  // riesenie najdene
    if (i == vals.size())      return false; 
    if (remaining > suffix[i]) return false;  // orezanie 2

    chosen.push_back(vals[i]);
    if (recursionB(i + 1, remaining - vals[i], vals, suffix, chosen, calls))
        return true;
    chosen.pop_back();
    return recursionB(i + 1, remaining, vals, suffix, chosen, calls);
}

bool solveImproved(const std::vector<int>& A, long long S,
                   std::vector<int>& chosen, long long& calls) {
    std::vector<int> vals = A; // kopia
    std::sort(vals.begin(), vals.end(), std::greater<int>());   

    std::vector<long long> suffix(vals.size() + 1, 0); // suffix[i] = vals[i] + ... + vals[n-1]
    for (std::size_t k = vals.size(); k-- > 0;)
        suffix[k] = suffix[k + 1] + vals[k];

    calls = 0;
    chosen.clear();
    return recursionB(0, S, vals, suffix, chosen, calls);
}

static void demo(const std::vector<int>& A, long long S) {
    std::vector<int> chosen;
    long long calls = 0;
    if (solveImproved(A, S, chosen, calls)) {
        std::cout << "solution:";
        for (int x : chosen) std::cout << ' ' << x;
    } else {
        std::cout << "no solution";
    }
    std::cout << ", calls = " << calls << '\n';
}

int main() {
    demo({3, 7, 11, 15, 19}, 30);
    demo({4, 8, 12, 16, 20}, 30);
    return 0;
}