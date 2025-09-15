/*
  Divide and Conquer solution 
*/

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>   
using namespace std;

//Recursión Divide & Vencerás
// Complejidad: O(2^n)
static long long knapsack_dc_rec(const vector<pair<int,int>>& s, int i, int W) {
    if (i == 0 || W == 0) return 0;

    int v = s[i-1].first;
    int w = s[i-1].second;

    if (w > W) {
        //Si el peso del objeto es mayor que la capacidad, no se puede incluir
        return knapsack_dc_rec(s, i-1, W);
    }
    //Exclusión o inclusión 
    long long excluir = knapsack_dc_rec(s, i-1, W);
    long long incluir = v + knapsack_dc_rec(s, i-1, W - w);
    return max(excluir, incluir);
}

long long knapsack_dc(const vector<pair<int,int>>& s, int W) {
    return knapsack_dc_rec(s, (int)s.size(), W);
}

//Tomar el tiempo de ejecución promedio en microsegundos
struct Timed {
    long long value;   
    long long us;      
};

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
        auto r = time_call_avg_us([&]{ return knapsack_dc(s, W); }, 50);
        cout << left
             << setw(20) << "Divide y venceras"
             << setw(20) << "Chico"
             << setw(12) << r.value
             << setw(14) << r.us << "\n";
    }

    // --- Caso Mediano ---
    {
        
        int n = 21, W = 100; 
        auto s = make_random_items(n, 1, 20, 5, 50, 12345);
        auto r = time_call_avg_us([&]{ return knapsack_dc(s, W); }, 3);
        cout << left
             << setw(20) << "Divide y venceras"
             << setw(20) << "Mediano"
             << setw(12) << r.value
             << setw(14) << r.us << "\n";
    }

    return 0;
}
