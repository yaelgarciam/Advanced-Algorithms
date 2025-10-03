Yael García
yael.garciam
En línea

Aquí empieza el canal #coding. 
Joaquin — 17:39
Imagen
inzunzita — 17:41
-----------------------------------------------------------------------------------
#include <bits/stdc++.h>
using namespace std;

// Polynomial Rolling Hash 64-bit (overflow natural)
static inline uint64_t prh64(const string& s) {
    const uint64_t B = 131ULL; // base típica
    uint64_t h = 0ULL;
    for (unsigned char c : s) {
        h = h * B + (static_cast<uint64_t>(c) + 1ULL);
    }
    return h;
}

// Genera un string aleatorio de longitud [minLen, maxLen]
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

    int n = 20000; // número de strings
    vector<string> v; v.reserve(n);
    for (int i = 0; i < n; ++i) {
        v.push_back(random_string());
    }

    vector<uint64_t> hashes; hashes.reserve(n);
// hash -> string (para detectar colisiones reales)
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
            if (it->second != s) ++collisions; // colisión real
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
Joaquin — 17:50
Imagen
inzunzita — 17:52
#include <bits/stdc++.h>
#include <openssl/sha.h>
using namespace std;

// ---------- Polynomial Rolling Hash 64-bit (overflow natural) ----------
static inline uint64_t prh64(const string& s) {
    const uint64_t B = 131ULL; // base típica
    uint64_t h = 0ULL;
    for (unsigned char c : s) {
        h = h * B + (static_cast<uint64_t>(c) + 1ULL);
    }
    return h;
}

// ---------- SHA-1 (hex) con OpenSSL ----------
static inline string sha1_hex(const string& s) {
    unsigned char digest[SHA_DIGEST_LENGTH]; // 20 bytes
    SHA1(reinterpret_cast<const unsigned char*>(s.data()), s.size(), digest);
    static const char* hex = "0123456789abcdef";
    string out; out.resize(40);
    for (int i = 0; i < 20; ++i) {
        out[2*i]   = hex[(digest[i] >> 4) & 0xF];
        out[2*i+1] = hex[digest[i] & 0xF];
    }
    return out;
}

// ---------- Strings aleatorios [4, 8] ----------
static inline string random_string(int minLen = 4, int maxLen = 8) {
    static const string alphabet =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static thread_local mt19937_64 rng(
        (uint64_t)chrono::steady_clock::now().time_since_epoch().count() ^
        (uint64_t)(uintptr_t)&rng
    );
    uniform_int_distribution<int> lenDist(minLen, maxLen);
    uniform_int_distribution<int> charDist(0, (int)alphabet.size() - 1);

    int len = lenDist(rng);
    string s; s.reserve(len);
    for (int i = 0; i < len; ++i) s.push_back(alphabet[charDist(rng)]);
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int n = 20000;

    vector<string> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(random_string(4, 8));

    vector<uint64_t> prh_vals; prh_vals.reserve(n);
    vector<string>  sha1_vals; sha1_vals.reserve(n);

    // Buckets para conteo correcto de colisiones
    unordered_map<uint64_t, unordered_set<string>> prh_bucket;
    prh_bucket.reserve(size_t(n * 1.3));
    prh_bucket.max_load_factor(0.7f);

    unordered_map<string, unordered_set<string>> sha1_bucket;
    sha1_bucket.reserve(size_t(n * 1.3));
    sha1_bucket.max_load_factor(0.7f);

    // Bucket combinado PRH+SHA1 (clave: prh||sha1)
    struct PairHash {
        size_t operator()(const pair<uint64_t,string>& p) const noexcept {
            // Mezcla simple: hash de prh (64b) con hash de string
            size_t h1 = std::hash<uint64_t>{}(p.first);
            size_t h2 = std::hash<string>{}(p.second);
            // xorshift mezcla
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
        }
    };
    unordered_map<pair<uint64_t,string>, unordered_set<string>, PairHash> both_bucket;
    both_bucket.reserve(size_t(n * 1.3));
    both_bucket.max_load_factor(0.7f);

    auto t0 = chrono::high_resolution_clock::now();

    for (const auto& s : v) {
        uint64_t h1 = prh64(s);
        string  h2  = sha1_hex(s);

        prh_vals.push_back(h1);
        sha1_vals.push_back(h2);

        prh_bucket[h1].insert(s);
        sha1_bucket[h2].insert(s);
        both_bucket[ {h1, h2} ].insert(s);
    }

    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();

    // ---- Métricas de colisiones (eventos) ----
    auto collision_events = [](auto& bucket_map) {
        size_t events = 0, buckets = 0, pairs = 0;
        for (const auto& kv : bucket_map) {
... (44 líneas restantes)
Contraer
message.txt
6 KB
Joaquin — 17:54
rhc.cpp:24:26: error: use of undeclared identifier 'SHA_DIGEST_LENGTH'
    unsigned char digest[SHA_DIGEST_LENGTH]; // 20 bytes
                         ^
rhc.cpp:65:29: error: use of undeclared identifier 'unordered_set'
    unordered_map<uint64_t, unordered_set<string>> prh_bucket;
                            ^
rhc.cpp:69:27: error: use of undeclared identifier 'unordered_set'
    unordered_map<string, unordered_set<string>> sha1_bucket;
                          ^
rhc.cpp:83:42: error: use of undeclared identifier 'unordered_set'
    unordered_map<pair<uint64_t,string>, unordered_set<string>, PairHash> both_bucket;
                                         ^
4 errors generated.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <random>
Expandir
message.txt
6 KB
inzunzita — 17:55
bla
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
Expandir
message.txt
6 KB
Joaquin — 17:56
╭─    ~ ─────────────────────────────────────────────────────────────── ✔  17:56:16  ─╮
╰─ g++ -std=c++17 -O2 rhc.cpp -o rhc && ./rhc                                             ─╯

Undefined symbols for architecture x86_64:
  "_SHA1", referenced from:
      _main in rhc-74b20a.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
inzunzita — 17:57
bla 2
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <random>
#include <iomanip>
#include <tuple>
#include <utility>
#include <cstdint>
#include <CommonCrypto/CommonDigest.h>  // Usamos CommonCrypto para SHA-1 en macOS

using namespace std;

// -------- Polynomial Rolling Hash 64-bit (overflow natural) --------
static inline uint64_t prh64(const string& s) {
    const uint64_t B = 131ULL;
    uint64_t h = 0ULL;
    for (unsigned char c : s) {
        h = h * B + (static_cast<uint64_t>(c) + 1ULL);
    }
    return h;
}

// -------- SHA-1 (hex) con CommonCrypto --------
static inline string sha1_hex(const string& s) {
    unsigned char digest[CC_SHA1_DIGEST_LENGTH]; // 20 bytes
    CC_SHA1(reinterpret_cast<const unsigned char*>(s.data()),
            static_cast<CC_LONG>(s.size()), digest);

    static const char* hex = "0123456789abcdef";
    string out; out.resize(40);
    for (int i = 0; i < CC_SHA1_DIGEST_LENGTH; ++i) {
        out[2*i]   = hex[(digest[i] >> 4) & 0xF];
        out[2*i+1] = hex[digest[i] & 0xF];
    }
    return out;
}

// -------- Strings aleatorios [4, 8] --------
static inline string random_string(int minLen = 4, int maxLen = 8) {
    static const string alphabet =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static mt19937_64 rng(
        static_cast<uint64_t>(chrono::steady_clock::now().time_since_epoch().count())
    );
    uniform_int_distribution<int> lenDist(minLen, maxLen);
    uniform_int_distribution<int> charDist(0, static_cast<int>(alphabet.size()) - 1);

    int len = lenDist(rng);
    string s; s.reserve(len);
    for (int i = 0; i < len; ++i) s.push_back(alphabet[charDist(rng)]);
    return s;
}

// Hash para pair<uint64_t, string> (para el bucket combinado)
struct PairHash {
    size_t operator()(const pair<uint64_t,string>& p) const noexcept {
        size_t h1 = std::hash<uint64_t>{}(p.first);
        size_t h2 = std::hash<string>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int n = 20000;

    // Generar strings
    vector<string> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(random_string(4, 8));

    vector<uint64_t> prh_vals; prh_vals.reserve(n);
    vector<string>  sha1_vals; sha1_vals.reserve(n);

    // Buckets para conteo correcto de colisiones
    unordered_map<uint64_t, unordered_set<string>> prh_bucket;
    unordered_map<string, unordered_set<string>> sha1_bucket;
    unordered_map<pair<uint64_t,string>, unordered_set<string>, PairHash> both_bucket;

    auto t0 = chrono::high_resolution_clock::now();

    for (const auto& s : v) {
        uint64_t h1 = prh64(s);
        string  h2  = sha1_hex(s);

        prh_vals.push_back(h1);
        sha1_vals.push_back(h2);

        prh_bucket[h1].insert(s);
        sha1_bucket[h2].insert(s);
        both_bucket[{h1, h2}].insert(s);
    }

    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();

... (49 líneas restantes)
Contraer
message.txt
6 KB
﻿
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <random>
#include <iomanip>
#include <tuple>
#include <utility>
#include <cstdint>
#include <CommonCrypto/CommonDigest.h>  // Usamos CommonCrypto para SHA-1 en macOS

using namespace std;

// -------- Polynomial Rolling Hash 64-bit (overflow natural) --------
static inline uint64_t prh64(const string& s) {
    const uint64_t B = 131ULL;
    uint64_t h = 0ULL;
    for (unsigned char c : s) {
        h = h * B + (static_cast<uint64_t>(c) + 1ULL);
    }
    return h;
}

// -------- SHA-1 (hex) con CommonCrypto --------
static inline string sha1_hex(const string& s) {
    unsigned char digest[CC_SHA1_DIGEST_LENGTH]; // 20 bytes
    CC_SHA1(reinterpret_cast<const unsigned char*>(s.data()),
            static_cast<CC_LONG>(s.size()), digest);

    static const char* hex = "0123456789abcdef";
    string out; out.resize(40);
    for (int i = 0; i < CC_SHA1_DIGEST_LENGTH; ++i) {
        out[2*i]   = hex[(digest[i] >> 4) & 0xF];
        out[2*i+1] = hex[digest[i] & 0xF];
    }
    return out;
}

// -------- Strings aleatorios [4, 8] --------
static inline string random_string(int minLen = 4, int maxLen = 8) {
    static const string alphabet =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static mt19937_64 rng(
        static_cast<uint64_t>(chrono::steady_clock::now().time_since_epoch().count())
    );
    uniform_int_distribution<int> lenDist(minLen, maxLen);
    uniform_int_distribution<int> charDist(0, static_cast<int>(alphabet.size()) - 1);

    int len = lenDist(rng);
    string s; s.reserve(len);
    for (int i = 0; i < len; ++i) s.push_back(alphabet[charDist(rng)]);
    return s;
}

// Hash para pair<uint64_t, string> (para el bucket combinado)
struct PairHash {
    size_t operator()(const pair<uint64_t,string>& p) const noexcept {
        size_t h1 = std::hash<uint64_t>{}(p.first);
        size_t h2 = std::hash<string>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int n = 20000;

    // Generar strings
    vector<string> v; v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(random_string(4, 8));

    vector<uint64_t> prh_vals; prh_vals.reserve(n);
    vector<string>  sha1_vals; sha1_vals.reserve(n);

    // Buckets para conteo correcto de colisiones
    unordered_map<uint64_t, unordered_set<string>> prh_bucket;
    unordered_map<string, unordered_set<string>> sha1_bucket;
    unordered_map<pair<uint64_t,string>, unordered_set<string>, PairHash> both_bucket;

    auto t0 = chrono::high_resolution_clock::now();

    for (const auto& s : v) {
        uint64_t h1 = prh64(s);
        string  h2  = sha1_hex(s);

        prh_vals.push_back(h1);
        sha1_vals.push_back(h2);

        prh_bucket[h1].insert(s);
        sha1_bucket[h2].insert(s);
        both_bucket[{h1, h2}].insert(s);
    }

    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();

    // ---- MÃ©tricas de colisiones ----
    auto collision_metrics = [](auto& bucket_map) {
        size_t buckets = 0, events = 0, pairs = 0;
        for (const auto& kv : bucket_map) {
            size_t u = kv.second.size();
            if (u >= 2) {
                ++buckets;
                events += (u - 1);
                pairs  += (u * (u - 1)) / 2;
            }
        }
        return tuple<size_t,size_t,size_t>(buckets, events, pairs);
    };

    auto [prh_cb, prh_events, prh_pairs]    = collision_metrics(prh_bucket);
    auto [sha1_cb, sha1_events, sha1_pairs] = collision_metrics(sha1_bucket);
    auto [both_cb, both_events, both_pairs] = collision_metrics(both_bucket);

    // ---- Reporte ----
    cout << "Metodo: PRH + SHA-1\n";
    cout << "Total strings: " << n << "\n";
    cout << "Tiempo (ms): " << fixed << setprecision(3) << ms << "\n\n";

    cout << "[PRH]\n";
    cout << "  Hashes distintos: " << prh_bucket.size() << "\n";
    cout << "  Buckets con colision: " << prh_cb << "\n";
    cout << "  Colisiones (eventos): " << prh_events << "\n";
    cout << "  Pares en conflicto: " << prh_pairs << "\n\n";

    cout << "[SHA-1]\n";
    cout << "  Hashes distintos: " << sha1_bucket.size() << "\n";
    cout << "  Buckets con colision: " << sha1_cb << "\n";
    cout << "  Colisiones (eventos): " << sha1_events << "\n";
    cout << "  Pares en conflicto: " << sha1_pairs << "\n\n";

    cout << "[PRH + SHA-1 simultaneo]\n";
    cout << "  Buckets con colision: " << both_cb << "\n";
    cout << "  Colisiones (eventos): " << both_events << "\n";
    cout << "  Pares en conflicto: " << both_pairs << "\n\n";

    cout << "Ejemplos (10 primeros):\n";
    for (int i = 0; i < min(n, 10); ++i) {
        cout << v[i] << " -> PRH:" << prh_vals[i]
             << "  SHA1:" << sha1_vals[i] << "\n";
    }

    return 0;
}