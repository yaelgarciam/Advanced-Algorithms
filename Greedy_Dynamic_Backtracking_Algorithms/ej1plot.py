#
# Implementation of Greedy Algorithm. (Graphical Representation)
# Joaquín Hiroki Campos Kishi A01639134
# Yael García Morelos "The Gopher" A01352461
#

import matplotlib.pyplot as plt

def plot_camino(filename, title):
    coords = []
    with open(filename) as f:
        for line in f:
            x, y = map(int, line.split())
            coords.append((x, y))

    xs, ys = zip(*coords)
    plt.figure(figsize=(7,7))
    plt.scatter(xs, ys, c="blue")
    plt.plot(xs, ys, c="red")

    for i, (x, y) in enumerate(coords):
        plt.text(x+3, y+3, str(i), fontsize=8)

    plt.title(title)
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.show()

# Graficar los 4 archivos
plot_camino("inicio1.txt", "Ejemplo Canvas - Inicial")
plot_camino("inicio2.txt", "Datos Random - Inicial")
plot_camino("final1.txt", "Ejemplo Canvas - Camino Greedy")
plot_camino("final2.txt", "Datos Random - Camino Greedy")
