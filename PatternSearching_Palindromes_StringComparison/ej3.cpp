
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

// String generator
static inline string random_string(int minLen = 4, int maxLen = 8) {
    static const string alphabet =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static mt19937_64 rng(
        static_cast<uint64_t>(chrono::steady_clock::now().time_since_epoch().count())
    );
    uniform_int_distribution<int> lenDist(minLen, maxLen);
    uniform_int_distribution<int> charDist(0, static_cast<int>(alphabet.size()) - 1);

    int len = lenDist(rng);
    string s; s.resize(len);
    for (int i = 0; i < len; ++i) s[i] = alphabet[charDist(rng)];
    return s;
}

// Polynomial Rolling Hash Function
static inline long long prhf(string_view s, long long p = 53, long long m = 1000000009LL) {
    long long value = 0;
    long long pwr   = 1;
    for (unsigned char c : s) {
        value = (value + (static_cast<long long>(c) * pwr) % m) % m;
        pwr   = (pwr * p) % m;
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // String generator
    const int N = 200000;
    unordered_set<string> uniq;
    uniq.reserve(N * 2);
    while ((int)uniq.size() < N) {
        uniq.insert(random_string(4, 8));
    }
    vector<string> words; words.reserve(N);
    for (const auto& s : uniq) words.push_back(s);

    // Hashing and timing
    unordered_map<long long, size_t> buckets; 
    buckets.reserve(N * 2);

    auto t0 = chrono::high_resolution_clock::now();

    for (const auto& s : words) {
        long long h = prhf(s); 
        ++buckets[h];
    }

    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();

    // Collision analysis
    size_t buckets_con_colision = 0;
    size_t colisiones_eventos   = 0;
    size_t pares_en_conflicto   = 0;

    for (const auto& [h, cnt] : buckets) {
        if (cnt >= 2) {
            ++buckets_con_colision;
            colisiones_eventos += (cnt - 1);
            pares_en_conflicto += (cnt * (cnt - 1)) / 2;
        }
    }

    // Output results
    cout << "Metodo: Polynomial Rolling Hash (PRH)\n";
    cout << "Total de strings distintos: " << words.size() << "\n";
    cout << fixed << setprecision(3)
         << "Tiempo total de hashing (ms): " << ms << "\n\n";

    cout << "[PRH]\n";
    cout << "  Hashes distintos: " << buckets.size() << "\n";
    cout << "  Buckets con colision: " << buckets_con_colision << "\n";
    cout << "  Colisiones (eventos): " << colisiones_eventos << "\n";
    cout << "  Pares en conflicto: " << pares_en_conflicto << "\n\n";
    cout << "Ejemplo 10 primeros:\n";
    for (int i = 0; i < min((int)words.size(), 10); ++i) {
        cout << words[i] << " -> PRH: " << prhf(words[i]) << "\n";
    }

    return 0;
}
