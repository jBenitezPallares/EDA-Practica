# Letra de práctica: 10 ejercicios de EDA2

Es una letra de práctica armada por mí con el mismo estilo del obligatorio. No es el enunciado original.

**Reglas generales**
- Lenguaje: C++11. Entrada por `cin` y salida por `cout`.
- Cada archivo es `ejercicioN.cpp` con su `main`.
- Documentá cada función con PRE y POS.
- No usar la STL (`vector`, `map`, `sort`, etc.). Se pueden usar arreglos y los TADs que armes vos.
- Liberá toda la memoria dinámica (destructores).
- Respetá las complejidades pedidas.

---

## Ejercicio 1: Registro de jugadores (AVL por id)

**Estructura obligatoria:** un único árbol AVL ordenado por `id`. Cada nodo guarda `id`, `nombre`, el factor de balance `fVal` y `cantSubarbol` (cantidad de nodos de su subárbol).

**Entrada:** una línea con `N` (cantidad de acciones) y luego `N` acciones, una por línea:

| Acción | Efecto | Salida |
|---|---|---|
| `ADD id nombre` | Agrega el jugador. Si el `id` ya existe, actualiza su nombre. | nada |
| `FIND id` | Busca por id. | `nombre`, o `jugador_no_encontrado` |
| `RANK id` | Cuenta los jugadores con id estrictamente menor a `id`. | un entero |
| `COUNT` | Cantidad de jugadores. | un entero |

**Complejidad:** `ADD`, `FIND` y `RANK` en O(log n). `COUNT` en O(1).

**Ejemplo**
```
6
ADD 10 ana
ADD 5 beto
ADD 20 carla
FIND 5
RANK 20
COUNT
```
Salida:
```
beto
2
3
```

---

## Ejercicios 2 y 3: Tabla de recursos web

Los dos ejercicios tienen **el mismo problema, la misma estructura de dos niveles y la misma entrada y salida**. La única diferencia es cómo resuelven las colisiones en las tablas hash.

**Estructura común**
- Una tabla hash de dominios. Cada dominio tiene su propia tabla hash de paths.
- Cada dominio guarda `cantElementos` y una **lista doblemente enlazada** (`sig` y `ant`) con sus recursos **ordenados por `tiempo` descendente**. Si hay empate, el recurso insertado después queda después.
- La tabla principal guarda la cantidad total de recursos.
- Función de Horner como hash principal.
- Tamaño de cada tabla: el primer primo mayor al doble de su tamaño inicial.
- Cada recurso tiene `titulo` y `tiempo`.
- `cantElementos` cuenta solo los recursos vivos (los borrados lógicamente no cuentan).

**Acciones**

| Acción | Efecto | Salida |
|---|---|---|
| `PUT dom path titulo tiempo` | Inserta. Si el recurso ya existe, reemplaza `titulo` y `tiempo` y lo reubica en la lista del dominio. | nada |
| `GET dom path` | Busca el recurso. | `titulo tiempo`, o `recurso_no_encontrado` |
| `REMOVE dom path` | Borra el recurso. | nada |
| `CONTAINS dom path` | Verifica si existe. | `true` o `false` |
| `COUNT_DOMAIN dom` | Cantidad de recursos del dominio. | un entero |
| `LIST_DOMAIN dom` | Lista los paths del dominio por `tiempo` descendente. | los paths en una línea separados por espacios, o `sin_informacion` si no hay |
| `CLEAR_DOMAIN dom` | Borra todos los recursos del dominio. | nada |
| `SIZE` | Total de recursos. | un entero |
| `CLEAR` | Vacía la tabla. | nada |

**Complejidad (promedio)**

| Operación | Complejidad |
|---|---|
| `GET`, `CONTAINS`, `REMOVE`, `COUNT_DOMAIN`, `SIZE` | O(1) |
| `PUT` | O(k), porque inserta en la lista ordenada |
| `LIST_DOMAIN` | O(k) |
| `CLEAR_DOMAIN` | O(tamaño de la tabla de paths del dominio) en el ejercicio 2; O(k) en el 3 |

Acá `k` es la cantidad de recursos vivos del dominio. Para que `REMOVE` sea O(1), el nodo se desengancha de la lista del dominio usando `ant` y `sig`, sin recorrerla.

---

### Ejercicio 2: hash cerrado

- Las dos tablas (dominios y paths) usan **direccionamiento abierto con exploración cuadrática** (`fHashColisiones`).
- El borrado es **lógico**: el nodo queda en la tabla de paths marcado como `estaBorrado`, pero se saca de la lista del dominio.
- `GET`, `CONTAINS` y la búsqueda de `PUT` no deben cortar la exploración en un nodo borrado. Solo un hueco real (`NULL`) la corta.
- `PUT` de un path que estaba borrado **reutiliza ese mismo nodo**: lo marca como vivo, actualiza `titulo` y `tiempo`, lo vuelve a insertar en la lista ordenada y suma los dos contadores.
- `CLEAR` y `CLEAR_DOMAIN` liberan todos los nodos de la tabla, incluidos los borrados, y dejan las posiciones en `NULL`.

**Ejemplo**
```
11
PUT a.com /x T1 10
PUT a.com /y T2 20
PUT b.com /x T3 30
GET a.com /y
GET c.com /x
REMOVE a.com /x
CONTAINS a.com /x
COUNT_DOMAIN a.com
LIST_DOMAIN a.com
LIST_DOMAIN c.com
SIZE
```
Salida:
```
T2 20
recurso_no_encontrado
false
1
/y
sin_informacion
2
```

---

### Ejercicio 3: hash abierto (encadenado)

- Las dos tablas (dominios y paths) resuelven las colisiones **encadenando** nodos en una lista por posición (`sig` entre dominios, `sigPath` entre paths).
- El borrado es **físico**: se saca el nodo de la lista del bucket y también de la lista ordenada del dominio, y se libera.
- Cada `nodoPath` está en **dos listas a la vez**: la del bucket y la del dominio. Al borrar o cambiar el `tiempo` hay que actualizar las dos.

**Ejemplo**
```
7
PUT a.com /x T1 10
PUT a.com /y T2 20
PUT a.com /z T3 15
LIST_DOMAIN a.com
CLEAR_DOMAIN a.com
LIST_DOMAIN a.com
SIZE
```
Salida:
```
/y /z /x
sin_informacion
0
```

---

## Ejercicio 4: Mezcla de K listas ordenadas (heap)

**Estructura obligatoria:** min-heap implementado por vos, con arreglo. Cada nodo guarda valor, lista de origen y posición en esa lista.

**Entrada:**
- Una línea con `K`.
- Luego `K` líneas: `tamaño e1 e2 ... etam`. Cada lista viene ordenada ascendente y puede ser vacía.

**Salida:** todos los elementos en orden ascendente, uno por línea.

**Complejidad:** O(N log K), con N el total de elementos. El heap nunca supera K elementos.

**Ejemplo**
```
3
3 1 4 7
2 2 5
4 0 3 6 9
```
Salida: `0 1 2 3 4 5 6 7 9`, un número por línea.

---

## Ejercicio 5: Árbol de cubrimiento mínimo (Kruskal o Prim)

**Entrada:**
- Una línea con `V A` (vértices numerados de 0 a V-1, y aristas).
- Luego `A` líneas `u v costo`, grafo no dirigido.

**Salida:**
- El costo total del árbol de cubrimiento mínimo.
- Si el grafo no es conexo, `no_conexo`.

**Requisitos:**
- Representá el grafo con listas de adyacencia.
- Con Kruskal: heap de aristas y Union-Find con compresión de caminos y unión por tamaño.
- Con Prim: heap de aristas.
- Para practicar, implementá ambos y compará los resultados.

**Ejemplo**
```
4 5
0 1 1
1 2 2
2 3 3
0 3 4
0 2 5
```
Salida: `6`

---

## Ejercicio 6: Contar inversiones (divide and conquer)

Una **inversión** es un par de posiciones `i < j` con `a[i] > a[j]`.

**Entrada:** una línea con `N` y luego `N` enteros.

**Salida:** la cantidad de inversiones.

**Requisitos:**
- Modificá merge sort para contar las inversiones durante el `merge`.
- Complejidad O(n log n).
- Usá un contador de tipo `long long`.

**Ejemplo**
```
5
2 4 1 3 5
```
Salida: `3`. Las inversiones son (2,1), (4,1) y (4,3).

---

## Ejercicio 7: Camino mínimo (Dijkstra)

**Entrada:**
- Una línea con `V A` (grafo dirigido, vértices de 0 a V-1).
- Luego `A` líneas `u v peso`, con pesos positivos.
- Una última línea `origen destino`.

**Salida:**
- Primera línea: el costo mínimo, o `inalcanzable`.
- Si hay camino, una segunda línea con los vértices del camino separados por espacios.

**Requisitos:**
- Lista de adyacencia.
- Min-heap de pares (distancia, vértice).
- Arreglo de padres para reconstruir el camino.
- Complejidad O((V + A) log V).

**Ejemplo**
```
5 6
0 1 4
0 2 1
2 1 2
1 3 1
2 3 5
3 4 3
0 4
```
Salida:
```
7
0 2 1 3 4
```

---

## Ejercicio 8: N reinas (backtracking)

**Entrada:** un entero `N`.

**Salida:**
- La cantidad de formas de ubicar `N` reinas en un tablero `N x N` sin que se ataquen.
- Después, la primera solución en orden lexicográfico, como `N` enteros (la columna de la reina de cada fila). Si no hay solución, imprimí `sin_solucion` en lugar de la segunda línea.

**Requisitos:**
- Backtracking con poda: chequeá columna y diagonales antes de recursar.
- Usá arreglos de booleanos para columnas y diagonales, para chequear en O(1).

**Ejemplo**
```
6
```
Salida:
```
4
1 3 5 0 2 4
```

---

## Ejercicio 9: Selección de actividades (greedy)

**Entrada:**
- Una línea con `N`.
- Luego `N` líneas `inicio fin`, con `inicio < fin`.

**Salida:**
- Primera línea: la máxima cantidad de actividades que se pueden hacer sin solaparse. Una actividad puede empezar justo cuando termina otra.
- Siguientes líneas: las actividades elegidas, ordenadas por inicio, en formato `inicio fin`.

**Requisitos:**
- Criterio voraz: ordenar por hora de fin y elegir la primera compatible.
- Ordená con tu propio merge sort o heap sort.
- Complejidad O(n log n).
- Si hay empate de fin, elegí la de menor inicio.

**Ejemplo**
```
11
1 4
3 5
0 6
5 7
3 9
5 9
6 10
8 11
8 12
2 14
12 16
```
Salida:
```
4
1 4
5 7
8 11
12 16
```

---

## Ejercicio 10: Mochila 0/1 (programación dinámica con tabla)

**Entrada:**
- Una línea con `N W` (cantidad de objetos y capacidad).
- Luego `N` líneas `peso valor`.

**Salida:**
- Primera línea: el valor máximo.
- Segunda línea: los índices (desde 1, ascendentes) de los objetos elegidos, separados por espacio.

**Requisitos:**
- Método bottom-up con tabla `T[i][w]`.
- Reconstruí la solución recorriendo la tabla hacia atrás.
- Complejidad O(N·W) en tiempo y espacio.
- Escribí en un comentario la fórmula de recurrencia que usás.

**Ejemplo**
```
4 7
1 1
3 4
4 5
5 7
```
Salida:
```
9
2 3
```

---

## Cómo practicar

1. Hacé primero los de teoría conocida (1 a 5) sin mirar tu solución anterior.
2. Armá casos propios más grandes (100 o más acciones) y compará contra una versión ingenua.
3. Probá con valgrind o `-fsanitize=address` para verificar que no haya leaks.
4. Si querés, pedime casos de prueba extra o una solución de referencia de cualquier ejercicio.
