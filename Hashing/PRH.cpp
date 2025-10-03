#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;

static inline uint64_t prh64(const string& s) {
    const uint64_t B = 131ULL; 
    uint64_t h = 0ULL;
    for (unsigned char c : s) {
        h = h * B + (static_cast<uint64_t>(c) + 1ULL);
    }
    return h;
}

string random_string(int minLen = 5, int maxLen = 20) {
    static const string alphabet =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> lenDist(minLen, maxLen);
    uniform_int_distribution<int> charDist(0, (int)alphabet.size() - 1);

    int len = lenDist(rng);
    string s; s.reserve(len);
    for (int i = 0; i < len; ++i) {
        s.push_back(alphabet[charDist(rng)]);
    }
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 20000; 
    vector<string> v; v.reserve(n);
    for (int i = 0; i < n; ++i) {
        v.push_back(random_string());
    }

    vector<uint64_t> hashes; hashes.reserve(n);
    unordered_map<uint64_t, string> seen;
    seen.reserve(static_cast<size_t>(n * 1.3));
    seen.max_load_factor(0.7f);

    auto t0 = chrono::high_resolution_clock::now();

    size_t collisions = 0;
    for (const auto& s : v) {
        uint64_t h = prh64(s);
        hashes.push_back(h);

        auto it = seen.find(h);
        if (it == seen.end()) {
            seen.emplace(h, s);
        } else {
            if (it->second != s) ++collisions; 
        }
    }

    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();

    cout << "Metodo: PRH\n";
    cout << "Total strings: " << n << "\n";
    cout << "Tiempo (ms): " << fixed << setprecision(3) << ms << "\n";
    cout << "Hashes distintos: " << seen.size() << "\n";
    cout << "Colisiones reales: " << collisions << "\n";

    cout << "\nEjemplos (10 primeros):\n";
    for (int i = 0; i < min(n, 10); ++i) {
        cout << v[i] << " -> " << hashes[i] << "\n";
    }

    return 0;
}