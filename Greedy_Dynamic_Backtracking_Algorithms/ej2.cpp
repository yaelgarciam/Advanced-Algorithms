//=================================================================================================================
/**
 *  Implementation of Dynamic Programming.
 *  - Joaquín Hiroki Campos Kishi A01639134
 *  - Yael García Morelos "The Gopher" A01352461
 *  
 *  
 */
//=================================================================================================================


#include <iostream>
#include <vector>

int recursiveCut (std::vector<int> price, int n) {
    if (n <= 0 || n > 10) return 0;
    int max_val = INT_MIN;
    for (int i = 0; i < n; i++) {
        max_val = std::max(max_val, price[i] + recursiveCut(price, n - i - 1));
    }
    return max_val;
}

int topDownCut (std::vector<int> price, int n, std::vector<int> &memo) {
    if (n <= 0 || n > 10) return 0;
    if (memo[n] >= 0) return memo[n];
    int max_val = INT_MIN;
    for (int i = 0; i < n; i++) {
        max_val = std::max(max_val, price[i] + topDownCut(price, n - i - 1, memo));
    }
    memo[n] = max_val;
    return max_val;
}

int bottomUpCut (std::vector<int> price, int n) {
    if (n <= 0 || n > 10) return 0;
    std::vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        int max_val = INT_MIN;
        for (int j = 0; j < i; j++) {
            max_val = std::max(max_val, price[j] + dp[i - j - 1]);
        }
        dp[i] = max_val;
    }
    return dp[n];
}

int main(){
    std::vector<int> price = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    std::vector<int> n = {7, 9, 10, 34, -1, 6};
    for (int i : n) {
        std::cout << "Length: " << i << "\n";
        std::cout << "Recursive: " << recursiveCut(price, i) << "\n";
        std::vector<int> memo(i + 1, -1);
        std::cout << "Top-Down: " << topDownCut(price, i, memo) << "\n";
        std::cout << "Bottom-Up: " << bottomUpCut(price, i) << "\n";
    }
    return 0;
}