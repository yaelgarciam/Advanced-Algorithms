/*
  Dynamic Programming solution
*/

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>   
using namespace std;

long long knapsack_dp(const vector<pair<int,int>>& s, int W) {
    int n = (int)s.size();
    vector<vector<long long>> V(n+1, vector<long long>(W+1, 0));

    for (int i = 1; i <= n; ++i) {
        int vi = s[i-1].first;
        int wi = s[i-1].second;
        for (int j = 1; j <= W; ++j) {
            if (wi > j) {
                V[i][j] = V[i-1][j];
            } else {
                V[i][j] = max(V[i-1][j], (long long)vi + V[i-1][j - wi]);
            }
        }
    }
    return V[n][W];
}

//Reconstrucción de objetos 
vector<int> reconstruct_items(const vector<pair<int,int>>& s, int W) {
    int n = (int)s.size();
    vector<vector<long long>> V(n+1, vector<long long>(W+1, 0));
    for (int i = 1; i <= n; ++i) {
        int vi = s[i-1].first, wi = s[i-1].second;
        for (int j = 1; j <= W; ++j) {
            if (wi > j) V[i][j] = V[i-1][j];
            else V[i][j] = max(V[i-1][j], (long long)vi + V[i-1][j - wi]);
        }
    }
    vector<int> chosen;
    int i = n, j = W;
    while (i > 0 && j > 0) {
        if (V[i][j] != V[i-1][j]) { 
            chosen.push_back(i-1);
            j -= s[i-1].second;
        }
        i--;
    }
    reverse(chosen.begin(), chosen.end());
    return chosen;
}

//Tomar el tiempo de ejecución promedio en microsegundos
struct Timed { long long value; long long us; };
template<class F>
Timed time_call_avg_us(F f, int reps = 5) {
    using Clock = std::chrono::high_resolution_clock;
    long long last = 0;
    auto t0 = Clock::now();
    for (int r = 0; r < reps; ++r) last = f();
    auto t1 = Clock::now();
    long long total_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    return { last, total_us / reps };
}

vector<pair<int,int>> make_random_items(int n, int w_min, int w_max, int v_min, int v_max, uint32_t seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> Wd(w_min, w_max), Vd(v_min, v_max);
    vector<pair<int,int>> s; s.reserve(n);
    for (int i = 0; i < n; ++i) s.emplace_back(Vd(rng), Wd(rng));
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto print_header = [](){
        cout << left
             << setw(20) << "Algoritmo"
             << setw(20) << "Caso"
             << setw(12) << "Valor"
             << setw(14) << "Tiempo (us)"
             << "\n" << string(66, '-') << "\n";
    };
    print_header();

    // --- Caso Chico ---
    {
        vector<pair<int,int>> s = {{140,20}, {60,10}, {50,25}}; // (v,w)
        int W = 30;
        auto r = time_call_avg_us([&]{ return knapsack_dp(s, W); }, 50);
        cout << left
             << setw(20) << "DP"
             << setw(20) << "Chico"
             << setw(12) << r.value
             << setw(14) << r.us << "\n";

        auto pick = reconstruct_items(s, W);
        cout << "DP objetos elegidos";
        for (int id : pick) cout << " " << id;
        cout << "\n";
    }

    // --- Caso Mediano ---
    {
        int n = 21, W = 100; 
        auto s = make_random_items(n, 1, 20, 5, 50, 12345);
        auto r = time_call_avg_us([&]{ return knapsack_dp(s, W); }, 10);
        cout << left
             << setw(20) << "DP"
             << setw(20) << "Mediano"
             << setw(12) << r.value
             << setw(14) << r.us << "\n";
    }

    // --- Caso Grande---
    {
        int n = 300, W = 5000; 
        auto s = make_random_items(n, 1, 20, 5, 50, 2025);
        auto r = time_call_avg_us([&]{ return knapsack_dp(s, W); }, 1);
        cout << left
             << setw(20) << "DP"
             << setw(20) << "Grande"
             << setw(12) << r.value
             << setw(14) << r.us << "\n";
    }

    return 0;
}
