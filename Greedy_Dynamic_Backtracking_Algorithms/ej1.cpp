//=================================================================================================================
/**
 *  Implementation of Greedy Algorithm.
 *  - Joaquín Hiroki Campos Kishi A01639134
 *  - Yael García Morelos "The Gopher" A01352461 
 */
//=================================================================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <fstream>

float distance(std::vector<int> a, std::vector<int> b){
    return sqrt(pow(b[0]-a[0], 2) + pow(b[1]-a[1], 2));
}

std::vector<int> shortestPath(std::vector<std::vector<int>> a){
    std::vector<bool> visited(a.size(), false);
    std::vector<int> path;
    int current = 0;
    visited[current] = true;
    path.push_back(current);

    while (path.size() < a.size()) {
        int next = -1;
        float minDist = std::numeric_limits<float>::max();

        for (int i = 0; i < a.size(); i++) {
            if (!visited[i]) {
                float dist = distance(a[current], a[i]);
                if (dist < minDist) {
                    minDist = dist;
                    next = i;
                }
            }
        }

        if (next != -1) {
            visited[next] = true;
            path.push_back(next);
            current = next;
        }
    }

    return path;
}

void guardarVector(const std::string &filename, const std::vector<std::vector<int>> &vec){
    std::ofstream file(filename);
    for (auto &p : vec){
        file << p[0] << " " << p[1] << "\n";
    }
    file.close();
}

void guardarCamino(const std::string &filename, const std::vector<std::vector<int>> &vec, const std::vector<int> &path){
    std::ofstream file(filename);
    for (int idx : path){
        file << vec[idx][0] << " " << vec[idx][1] << "\n";
    }
    file.close();
}

int main(){
    std::vector<std::vector<int>> a = {
        {20, 20}, {20, 40}, {20, 160}, {30, 120}, {40, 140}, {40, 150}, 
        {50, 20}, {60, 40}, {60, 80}, {60, 200}, {70, 200}, {80, 150}, 
        {90, 170}, {90, 170}, {100, 50}, {100, 40}, {100, 130}, {100, 150}, 
        {110, 10}, {110, 70}, {120, 80}, {130, 70}, {130, 170}, {140, 140}, 
        {140, 180}, {150, 50}, {160, 20}, {170, 100}, {180, 70}, {180, 200}, 
        {200, 30}, {200, 70}, {200, 100}
    };

    std::vector<std::vector<int>> b = {
        {87, 15}, {230, 90}, {45, 200}, {170, 130}, {12, 65},
        {300, 40}, {95, 175}, {210, 25}, {60, 80}, {145, 220},
        {33, 140}, {250, 55}, {110, 190}, {5, 30}, {275, 160},
        {190, 70}, {135, 15}, {80, 240}, {50, 125}, {215, 185},
        {40, 60}, {260, 110}, {20, 170}, {155, 95}, {225, 45},
        {130, 200}, {10, 100}, {205, 140}, {300, 190}, {55, 15},
        {240, 75}, {180, 220}, {65, 160}, {15, 210}, {120, 55},
        {270, 135}, {35, 95}, {195, 35}, {85, 130}, {150, 180},
        {140, 65}, {100, 225}, {280, 20}, {175, 105}, {245, 200},
        {90, 45}, {160, 150}, {30, 10}, {220, 170}, {70, 205}
    }; 

    guardarVector("inicio1.txt", a);
    guardarVector("inicio2.txt", b);

    auto path = shortestPath(a);
    auto path2 = shortestPath(b);

    guardarCamino("final1.txt", a, path);
    guardarCamino("final2.txt", b, path2);

    return 0;
}