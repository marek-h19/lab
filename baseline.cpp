#include <iostream> 
#include <vector>

/*
calls ← 0

SOLVE(i, currentSum, chosen):
    calls ← calls + 1                  // základná operácia = 1 rekurzívne volanie
    if i = n:                          // rozhodli sme o všetkých prvkoch (list stromu)
        return remaining = 0 
    // vetva 1: zober A[i]
    chosen.push(i)
    if SOLVE(i+1, currentSum - A[i], chosen): return true
    chosen.pop()                       // backtrack, vrátime sa do pôvodného stavu
    // vetva 2: preskoč A[i]
    return SOLVE(i+1, currentSum, chosen)

Volanie: SOLVE(0, 0, []) → ak true, v chosen je riešenie, inak riešenie neexistuje
*/

bool solve(std::size_t i, long long remaining, 
    std::vector<int>& chosen, const std::vector<int>& Arr, long long& calls) {
    ++calls; 

    if (i == Arr.size()){
        return remaining == 0; 
    } 
    chosen.push_back(Arr[i]);
    if (solve(i+1, remaining - Arr[i], chosen, Arr, calls)){
        return true; 
    }
    chosen.pop_back();
    return solve(i + 1, remaining, chosen, Arr, calls);   
}

int main (void){
    std::vector<int> Arr = {3, 7, 11, 15, 19}; 
    int sum = 30; 
    long long calls = 0; 

    std::vector<int> chosen;
    solve(0, sum, chosen, Arr, calls);

    for (int i = 0; i < chosen.size(); i++) {
        std::cout << chosen[i] << " "; 
    }
    std::cout << "\nNumber of calls: " << calls << std::endl;
    return 0; 
}