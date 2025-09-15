/*
    Branch and Bound solution
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <iomanip> 

// Estructura para representar un objeto
struct Item {
    int value;
    int weight;
    double ratio;
    int index;    
};

// Nodo en el árbol de ramificación
struct Node {
    int level;
    int value;
    int weight;
    double bound;
    std::vector<int> taken;
};

// Función para calcular la cota superior (bound)
double calculateBound(const Node& node, int n, int W, const std::vector<Item>& items) {
    if (node.weight >= W) return 0;

    double profitBound = node.value;
    int totalWeight = node.weight;
    int i = node.level;

    while (i < n && totalWeight + items[i].weight <= W) {
        totalWeight += items[i].weight;
        profitBound += items[i].value;
        i++;
    }

    if (i < n) {
        profitBound += (W - totalWeight) * items[i].ratio;
    }

    return profitBound;
}


struct Compare {
    bool operator()(const Node& a, const Node& b) {
        return a.bound < b.bound;
    }
};

// Branch and Bound algorithm
Node knapsackBB(std::vector<Item>& items, int W) {
    
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.ratio > b.ratio;
    });

    std::priority_queue<Node, std::vector<Node>, Compare> frontier;
    int n = items.size();
    Node best = {-1, 0, 0, 0, {}};

    Node root = {0, 0, 0, 0, {}};
    root.bound = calculateBound(root, n, W, items);
    frontier.push(root);

    while (!frontier.empty()) {
        Node node = frontier.top();
        frontier.pop();

        if (node.bound <= best.value || node.level >= n) continue;

        // Caso 1: Tomamos el objeto actual
        Node withItem;
        withItem.level = node.level + 1;
        withItem.weight = node.weight + items[node.level].weight;
        withItem.value = node.value + items[node.level].value;
        withItem.taken = node.taken;
        withItem.taken.push_back(items[node.level].index);

        if (withItem.weight <= W && withItem.value > best.value) {
            best = withItem;
        }

        withItem.bound = calculateBound(withItem, n, W, items);
        if (withItem.bound > best.value) {
            frontier.push(withItem);
        }

        // Caso 2: No tomamos el objeto actual
        Node withoutItem;
        withoutItem.level = node.level + 1;
        withoutItem.weight = node.weight;
        withoutItem.value = node.value;
        withoutItem.taken = node.taken;
        withoutItem.bound = calculateBound(withoutItem, n, W, items);

        if (withoutItem.bound > best.value) {
            frontier.push(withoutItem);
        }
    }

    return best;
}

void ejecutarCaso(const std::string& nombreCaso, std::vector<Item> items, int W) {
    for (auto& item : items) {
        item.ratio = (double)item.value / item.weight;
    }

    auto start = std::chrono::high_resolution_clock::now();
    Node best = knapsackBB(items, W);
    auto end = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << std::left << std::setw(20) << "Branch & Bound"
              << std::setw(20) << nombreCaso
              << std::setw(12) << best.value
              << std::setw(12) << duracion
              << std::endl;

    std::cout << "Objetos seleccionados (índices originales): ";
    for (int idx : best.taken) {
        std::cout << idx << " ";
    }
    std::cout << "\n------------------------------------------------------------\n";
}

int main() {
    std::cout << "Algoritmo           Caso                Valor       Tiempo (us)\n";
    std::cout << "------------------------------------------------------------------\n";

    // --- Caso pequeño ---
    std::vector<Item> itemsChico = {
        {60, 10, 0, 0},
        {100, 20, 0, 1},
        {120, 30, 0, 2}
    };
    ejecutarCaso("Chico", itemsChico, 50);

    // --- Caso mediano ---
    std::vector<Item> itemsMediano = {
        {100, 10, 0, 0}, {280, 40, 0, 1}, {120, 20, 0, 2}, {120, 24, 0, 3},
        {150, 30, 0, 4}, {200, 35, 0, 5}, {300, 50, 0, 6}, {90, 15, 0, 7}
    };
    ejecutarCaso("Mediano", itemsMediano, 100);

    return 0;
}
