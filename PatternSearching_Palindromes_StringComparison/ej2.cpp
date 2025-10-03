#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <cctype>

bool hasValidChar(const std::string &str) {
    for (char ch : str) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            return true;
        }
    }
    return false;
}

std::string manacher(const std::string &s){
    int c = 1, r = 0;
    std::string t = "^";
    for (char ch : s) {
        t += "#" + std::string(1, ch);
    }
    t += "#$";
    int n = t.size();
    std::vector<int> p(n, 0);

    for (int i = 1; i < n - 1; ++i) {
        int m = 2 * c - i;
        if (i < r) {
            p[i] = std::min(r - i, p[m]);
        }
        while (t[i + p[i] + 1] == t[i - p[i] - 1]) {
            p[i]++;
        }
        if (i + p[i] > r) {
            c = i;
            r = i + p[i];
        }
    }

    std::string best = "";
    for (int i = 1; i < n - 1; i++) {
        int len = p[i];
        std::string candidate = s.substr((i - len) / 2, len);
        if (candidate.size() > best.size() && hasValidChar(candidate)) {
            best = candidate;
        }
    }

    return best;
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << filename << std::endl;
        exit(1);
    }
    std::string book((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return book;
}

int main () {
    std::vector<std::string> books = {
        "Metamorphosis.txt", 
        "Frankenstein.txt", 
        "Middlemarch.txt", 
        "The_picture_of_dorian_gray.txt", 
        "War_and_peace.txt"
    };
    
    for (const auto& book : books) {
        std::string content = readFile(book);
        std::string processed = manacher(content);

        if (processed.empty() || processed.length() < 3) {
            std::cout << "No se encontro palindromo en " << book << std::endl;
        } else {
            std::cout << "Palindromo mas largo en " << book << ": " << processed << "." << std::endl;
        }
    }
    return 0;
}
