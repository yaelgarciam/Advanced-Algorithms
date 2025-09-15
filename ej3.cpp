//=================================================================================================================
/**
 *  Implementation of Backtracking for the Map Coloring problem.
 *  - Joaquín Hiroki Campos Kishi A01639134
 *  - Yael García Morelos "The Gopher" A01352461
 *  
 *  
 */
//=================================================================================================================

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>

//Region structure
struct Region {
    std::string name;
    std::vector<int> neighbors; 
};

bool is_consistent(const std::vector<int>& node, int regionIndex, int color,
                   const std::vector<Region>& regions) {
    for (int neighbor : regions[regionIndex].neighbors) {
        if (node[neighbor] == color) {
            return false;
        }
    }
    return true;
}

//Backtracking using recursion
std::vector<std::vector<int>> backtrack(std::vector<std::vector<int>>& sol,
                                        std::vector<int>& node,
                                        const std::vector<Region>& regions,
                                        int num_colors) {
    int nextRegion = -1;

    for (size_t i = 0; i < node.size(); i++) {
        if (node[i] == -1) {
            nextRegion = static_cast<int>(i);
            break;
        }
    }

    if (nextRegion == -1) {
        sol.push_back(node);
        return sol;
    }

    for (int color = 0; color < num_colors; ++color) {
        if (is_consistent(node, nextRegion, color, regions)) {
            node[nextRegion] = color;
            backtrack(sol, node, regions, num_colors);
            node[nextRegion] = -1;
        }
    }

    return sol;
}

//Wrapper
std::vector<std::vector<int>> backtrack(const std::vector<Region>& regions, int num_colors) {
    std::vector<std::vector<int>> sol;
    std::vector<int> node(regions.size(), -1);
    return backtrack(sol, node, regions, num_colors);
}

//Show one solution
void show_solution(const std::vector<int>& solution,
                   const std::vector<Region>& regions,
                   const std::vector<std::string>& color_names) {
    std::cout << "\nColoring:\n";
    for (size_t i = 0; i < solution.size(); ++i) {
        std::cout << "  " << std::left << std::setw(10) << regions[i].name << " -> " << color_names[solution[i]] << std::endl;
    }
}

int main() {
    std::vector<std::string> color_names = {"Rojo", "Verde", "Azul", "Gris"};
    int num_colors = static_cast<int>(color_names.size());

    //Neighbors list for each region
    std::vector<Region> regions = {
        {"Mark",     {1, 3}},         
        {"Julia",    {0, 2, 3}},     
        {"Brian",    {1, 4}},         
        {"Steve",    {0, 4, 5}},      
        {"Amanda",   {1, 3, 4, 5, 6}},
        {"Derek",    {2, 4, 6}},      
        {"Kelly",    {2, 5, 9}},      
        {"Allan",    {3, 8}},         
        {"Michelle", {7, 4, 9}},      
        {"Joanne",   {6, 8, 10}},     
        {"Chris",    {9}}             
    };

    auto solutions = backtrack(regions, num_colors);

    std::cout << "Backtracking for Custom Map Coloring\n";
    std::cout << "Regions: " << regions.size() << "\n";
    std::cout << "Colors:  " << num_colors << "\n";
    std::cout << "Valid colorings found: " << solutions.size() << "\n";

    int limit = 10;
    for (size_t i = 0; i < std::min(limit, static_cast<int>(solutions.size())); ++i) {
        std::cout << "\n-----------------------------";
        show_solution(solutions[i], regions, color_names);
    }

    return 0;
}
