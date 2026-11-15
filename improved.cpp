#include <iostream> 
#include <vector>

/*

*/

bool solve(std::size_t i, long long remaining, 
    std::vector<int>& chosen, const std::vector<int>& Arr, long long& calls) {
    ++calls; 
    
    // improvements
    if (remaining < 0) return false;

    long long sumRemaining = 0;
    for (std::size_t j = i; j < Arr.size(); ++j){
        sumRemaining += Arr[j]; 
    }
    if (sumRemaining < remaining){
        return false; 
    }
    
    if (remaining == 0){
        return true; 
    }else if (i == Arr.size()){
        return false; 
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