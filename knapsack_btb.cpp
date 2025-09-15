/*
  Backtracking + Bound 
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

struct Item {
    int value;
    int weight;
    double ratio;
};

double calculateUpperBound(const std::vector<Item> &items, int capacity, int currentWeight, int currentValue, int index) {
    double upperBound = currentValue;
    int remainingCapacity = capacity - currentWeight;

    for (int i = index; i < static_cast<int>(items.size()) && remainingCapacity > 0; i++) {
        if (items[i].weight <= remainingCapacity) {
            upperBound += items[i].value;
            remainingCapacity -= items[i].weight;
        } else {
            upperBound += static_cast<double>(items[i].value) *
                          (static_cast<double>(remainingCapacity) / items[i].weight);
            remainingCapacity = 0;
        }
    }
    return upperBound;
}

// --- Backtracking con poda ---
void knapsackBTB(const std::vector<Item> &items, int capacity, int &bestValue,
                 int currentValue, int currentWeight, double upperBound, int index,
                 std::vector<int> &currentSolution, std::vector<int> &bestSolution) {

    if (currentValue > bestValue && currentWeight <= capacity) {
        bestValue = currentValue;
        bestSolution = currentSolution;
    }

    index++;
    if (index >= 0 && index < static_cast<int>(items.size()) && currentWeight <= capacity) {
        
        // Caso 1: Tomar el ítem
        currentSolution.push_back(index);
        knapsackBTB(items, capacity, bestValue,
                    currentValue + items[index].value,
                    currentWeight + items[index].weight,
                    upperBound, index,
                    currentSolution, bestSolution);
        currentSolution.pop_back();

        // Caso 2: No tomar el ítem
        double newUpperBound = calculateUpperBound(items, capacity, currentWeight, currentValue, index + 1);
        if (newUpperBound > bestValue) {
            knapsackBTB(items, capacity, bestValue,
                        currentValue, currentWeight,
                        newUpperBound, index,
                        currentSolution, bestSolution);
        }
    }
}

// --- Wrapper para iniciar el backtracking ---
std::pair<int, std::vector<int>> knapsack_btb(std::vector<Item> items, int capacity) {
    for (auto &item : items) item.ratio = static_cast<double>(item.value) / item.weight;
    std::sort(items.begin(), items.end(), [](const Item &a, const Item &b) {
        return a.ratio > b.ratio;
    });

    int bestValue = 0;
    double upperBound = calculateUpperBound(items, capacity, 0, 0, 0);
    std::vector<int> currentSolution, bestSolution;

    knapsackBTB(items, capacity, bestValue, 0, 0, upperBound, -1, currentSolution, bestSolution);
    return { bestValue, bestSolution };
}

// --- Función para ejecutar un caso ---
void ejecutarCaso(const std::string& nombreCaso, std::vector<Item> items, int W) {
    for (auto& item : items) {
        item.ratio = (double)item.value / item.weight;
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto [bestValue, bestSolution] = knapsack_btb(items, W);
    auto end = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << std::left << std::setw(25) << "Backtracking with Bound"
              << std::setw(20) << nombreCaso
              << std::setw(12) << bestValue
              << std::setw(12) << duracion
              << std::endl;

    std::cout << "Objetos seleccionados (índices después del ordenamiento por ratio): ";
    for (int idx : bestSolution) {
        std::cout << idx << " ";
    }
    std::cout << "\n------------------------------------------------------------\n";
}

int main() {
    std::cout << "Algoritmo                Caso                Valor       Tiempo (us)\n";
    std::cout << "------------------------------------------------------------------\n";

    // --- Caso pequeño ---
    std::vector<Item> itemsChico = {
        {60, 10, 0},
        {100, 20, 0},
        {120, 30, 0}
    };
    ejecutarCaso("Chico", itemsChico, 50);

    // --- Caso mediano ---
    std::vector<Item> itemsMediano = {
        {100, 10, 0}, {280, 40, 0}, {120, 20, 0}, {120, 24, 0},
        {150, 30, 0}, {200, 35, 0}, {300, 50, 0}, {90, 15, 0}
    };
    ejecutarCaso("Mediano", itemsMediano, 100);

    return 0;
}
