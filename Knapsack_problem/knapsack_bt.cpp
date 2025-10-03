/*
  Backtracking solution 
*/

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

//Generador n items aleatorios (v,w) en rangos dados
vector<pair<int,int>> make_random_items(int n, int w_min, int w_max, int v_min, int v_max, uint32_t seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> Wd(w_min, w_max), Vd(v_min, v_max);
    vector<pair<int,int>> s; s.reserve(n);
    for (int i = 0; i < n; ++i) s.emplace_back(Vd(rng), Wd(rng));
    return s;
}

//Función recursiva backtracking
static void knapsack_bt_rec(const vector<pair<int,int>>& s, int i, int W,
                            int currW, long long currV,
                            vector<int>& node,
                            vector<vector<int>>& sol, bool collect_all,
                            long long& bestVal, vector<int>& bestSet) {
    int n = (int)s.size();

    //Registro de soluciones si cabe
    if (i == n) {
        if (currW <= W) {
            if (collect_all) sol.push_back(node);
            if (currV > bestVal) { bestVal = currV; bestSet = node; }
        }
        return;
    }

    //Opcion de exclusión o inclusión del objeto i
    knapsack_bt_rec(s, i+1, W, currW, currV, node, sol, collect_all, bestVal, bestSet);

    //Opcion de incluir el objeto i si cabe
    int vi = s[i].first, wi = s[i].second;
    if (currW + wi <= W) {
        node.push_back(i);                       
        knapsack_bt_rec(s, i+1, W, currW+wi, currV+vi, node, sol, collect_all, bestVal, bestSet);
        node.pop_back();                           
    }
}

//Función "driver" 
struct BTResult {
    long long bestValue = 0;
    vector<int> bestSet;               
    vector<vector<int>> allSolutions;
};

BTResult knapsack_bt(const vector<pair<int,int>>& s, int W, bool collect_all) {
    vector<int> node;
    vector<vector<int>> sol;
    long long bestVal = 0;
    vector<int> bestSet;
    knapsack_bt_rec(s, 0, W, 0, 0, node, sol, collect_all, bestVal, bestSet);
    return BTResult{bestVal, bestSet, sol};
}

//Tomar el tiempo de ejecución promedio en microsegundos
struct Timed { long long value; long long us; vector<int> pick; size_t count_all; };

template<class F>
Timed time_call_avg_us(F f, int reps = 5) {
    using Clock = std::chrono::high_resolution_clock;
    BTResult last;
    auto t0 = Clock::now();
    for (int r = 0; r < reps; ++r) last = f();
    auto t1 = Clock::now();
    long long total_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    return Timed{ last.bestValue, total_us / reps, last.bestSet, last.allSolutions.size() };
}

//Imprimir los ítems elegidos
void print_pick(const vector<int>& pick, const vector<pair<int,int>>& items) {
    cout << "Ítems elegidos";
    for (int id : pick) cout << " " << id;
    cout << "\nDetalle (v,w):";
    for (int id : pick) cout << " (" << items[id].first << "," << items[id].second << ")";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto header = [](){
        cout << left
             << setw(16) << "Algoritmo"
             << setw(20) << "Caso"
             << setw(12) << "Valor"
             << setw(14) << "Tiempo (us)"
             << setw(14) << "#Soluciones"
             << "\n" << string(76, '-') << "\n";
    };
    header();

    // === CASO Chico ===
    {
        vector<pair<int,int>> s = {{140,20},{60,10},{50,25}}; // (v,w)
        int W = 30;
        //Guardar soluciones en sol
        auto r = time_call_avg_us([&]{ return knapsack_bt(s, W, true); }, 80);
        cout << left
             << setw(16) << "Backtracking"
             << setw(20) << "Chico"
             << setw(12) << r.value
             << setw(14) << r.us
             << setw(14) << r.count_all << "\n";
        print_pick(r.pick, s);
        cout << "\n";
    }

    // === CASO Mediano ===
    {
        int n = 22, W = 110; 
        auto s = make_random_items(n, 1, 20, 5, 50, 12345);
        auto r = time_call_avg_us([&]{ return knapsack_bt(s, W, /*collect_all=*/false); }, 3);
        cout << left
             << setw(16) << "Backtracking"
             << setw(20) << "Mediano"
             << setw(12) << r.value
             << setw(14) << r.us
             << setw(14) << "-" << "\n";
        cout << "\n";
    }

    // === CASO Grande ===
    {
        int n = 30, W = 300; 
        auto s = make_random_items(n, 1, 20, 5, 50, 2025);
        auto r = time_call_avg_us([&]{ return knapsack_bt(s, W, false); }, 1);
        cout << left
             << setw(16) << "Backtracking"
             << setw(20) << "Grande"
             << setw(12) << r.value
             << setw(14) << r.us
             << setw(14) << "-" << "\n";
        cout << "\n";
    }
    return 0;
}
