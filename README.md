# EDA-Practica

Repositorio de práctica para Estructura de Datos y Algoritmos 2 (C++).
La letra de los 10 ejercicios está en [letra.md](letra.md).

## Estructura

- `ejercicio1.cpp` a `ejercicio10.cpp`: un archivo por ejercicio, con su `main` vacío.
- `tads/`: TADs propios (listas, heap, grafo, etc.).
- `funciones/`: funciones auxiliares (`enteros.cpp`).
- `.vscode/`: tareas de compilación y depuración.

## Ejercicios

| Ej. | Tema |
|---|---|
| 1 | AVL |
| 2 | Hash cerrado |
| 3 | Hash de dos niveles |
| 4 | Heap (merge de K listas) |
| 5 | Kruskal / Prim |
| 6 | Divide and conquer |
| 7 | Dijkstra |
| 8 | Backtracking |
| 9 | Greedy |
| 10 | Programación dinámica (tabla) |

## Compilar y ejecutar

```bash
g++ -std=c++11 -Wall -g -fsanitize=address -o ejercicioN ejercicioN.cpp
./ejercicioN < entrada.txt
```
