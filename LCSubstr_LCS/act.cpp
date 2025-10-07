#include <iostream>
#include <string>
#include <vector>
#include <fstream>

//Longest Common Substring
std::string lcsubstr(const std::string& s1, const std::string& s2) {
    int m = s1.size();
    int n = s2.size();
    std::vector<std::vector<int>> LCSTR(m + 1, std::vector<int>(n + 1, 0));

    int length = 0; 
    int end = 0;    

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                LCSTR[i][j] = LCSTR[i-1][j-1] + 1;
                if (LCSTR[i][j] > length) {
                    length = LCSTR[i][j];
                    end = i - 1;
                }
            } else {
                LCSTR[i][j] = 0;
            }
        }
    }

    return s1.substr(end - length + 1, length);
}

//Longest Common Subsequence
std::string lcsubsq (const std::string& s1, const std::string& s2) {
    int m = s1.size();
    int n = s2.size();
    std::vector<std::vector<int>> LCS(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                LCS[i][j] = LCS[i-1][j-1] + 1;
            } else {
                LCS[i][j] = std::max(LCS[i-1][j], LCS[i][j-1]);
            }
        }
    }

    std::string result;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (s1[i-1] == s2[j-1]) {
            result = s1[i-1] + result;
            i--;
            j--;
        } else if (LCS[i-1][j] > LCS[i][j-1]) {
            i--;
        } else {
            j--;
        }
    }

    return result;
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
    if (book.size() > 50000) {
        return book.substr(0, 50000);
    }
    return book;
}

int main(){
    std::vector <std::string> books = {"Frankenstein.txt", "Metamorphosis.txt"};
    std::string book1 = readFile(books[0]);
    std::string book2 = readFile(books[1]);
    std::string result = lcsubstr(book1, book2);
    std::string result2 = lcsubsq(book1, book2);

    std::cout << "Longest Common Substring length: " << result.size() << std::endl;
    std::cout << "Longest Common Substring: '" << result << "'" << std::endl;
    std::cout << "Longest Common Subsequence length: " << result2.size() << std::endl;
    std::cout << "Longest Common Subsequence: '" << result2 << "'" << std::endl;

    return 0;
}