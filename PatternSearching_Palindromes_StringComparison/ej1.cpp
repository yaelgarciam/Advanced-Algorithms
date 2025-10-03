#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <chrono>   
#include <cctype>   
#include <algorithm>

// algoritmo Z
std::vector<int> getZ(const std::string &s) {
    int n = s.size();
    std::vector<int> z(n);
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

// donde w es la palabra y b el libro
std::vector<int> zFun(const std::string &w, const std::string &b){
    const char sep = '\x1F';
    std::string concat;
    concat.reserve(w.size() + 1 + b.size());
    concat = w;
    concat.push_back(sep);
    concat += b;

    std::vector<int> z = getZ(concat);
    std::vector<int> occurrences;
    int m = (int)w.size();
    for (int i = m + 1; i < (int)z.size(); ++i) {
        if (z[i] == m) {
            occurrences.push_back(i - m - 1);
        }
    }
    return occurrences;
}

// algoritmo KMP
std::vector<int> lps (const std::string &w) {
    int m = w.size();
    std::vector<int> lps(m);
    if (m == 0) return lps;
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < m) {
        if (w[i] == w[len]) {
            ++len;
            lps[i] = len;
            ++i;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                ++i;
            }
        }
    }
    return lps;
}

std::vector<int> kmp(const std::string &w, const std::string &b) {
    int m = w.size();
    int n = b.size();
    std::vector<int> lpsd = lps(w);
    std::vector<int> occurrences;
    int i = 0;
    int j = 0;
    while (i < n) {
        if (w[j] == b[i]) {
            ++i;
            ++j;
        }
        if (j == m) {
            occurrences.push_back(i - j);
            j = lpsd[j - 1];
        } else if (i < n && w[j] != b[i]) {
            if (j != 0) {
                j = lpsd[j - 1];
            } else {
                ++i;
            }
        }
    }
    return occurrences;
}

// leer .txt y regresa string
std::string readFile(const std::string& filename) {
    std::ifstream file (filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << filename << std::endl;
        exit(1);
    }
    std::string book ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return book;
}

// formato de salida
void printOcurrences(const std::string& methodName, const std::string& word, const std::vector<int>& occurrences, double elapsed, const std::string& book) {
    std::cout << "Método: " << methodName << " | Total de ocurrencias: " << occurrences.size() << " | Tiempo (ms): " << elapsed << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;
    for (size_t i = 0; i < occurrences.size(); ++i) {
        int pos = occurrences[i];
        if (pos < 0 || pos >= static_cast<int>(book.size())) continue;

        size_t maxlen = 50;
        size_t start = static_cast<size_t>(pos);
        size_t len = std::min(maxlen, book.size() - start);
        std::string fragment = book.substr(start, len);

        for (auto& c : fragment) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (c == '\n' || c == '\r' || c == '\t' || !std::isprint(uc)) c = ' ';
        }

        std::cout << "No. " << (i + 1) << std::endl;
        std::cout << "Palabra: " << word << std::endl;
        std::cout << "Fragmento: " << fragment << std::endl << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::string book = readFile("Metamorphosis.txt");
    std::vector<std::string> words = {"window" , "father", "mother", "Gregor", "Samsa", "room", "life", "hand", "head", "door"};

    for (const auto& word : words) {
        // Z algorithm
        auto start_z = std::chrono::high_resolution_clock::now();
        std::vector<int> occurrences = zFun(word, book);
        auto end_z = std::chrono::high_resolution_clock::now();
        double elapsed_z = std::chrono::duration<double, std::milli>(end_z - start_z).count();

        // KMP algorithm
        auto start_kmp = std::chrono::high_resolution_clock::now();
        std::vector<int> occurrences_kmp = kmp(word, book);
        auto end_kmp = std::chrono::high_resolution_clock::now();
        double elapsed_kmp = std::chrono::duration<double, std::milli>(end_kmp - start_kmp).count();

        printOcurrences("Z algorithm", word, occurrences, elapsed_z, book);
        printOcurrences("KMP algorithm", word, occurrences_kmp, elapsed_kmp, book);
    }

    return 0;
}
