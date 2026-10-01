# Detalle de ejercicios

Este documento explica, para cada ejercicio del repositorio, qué problema resuelve, qué estrategia algorítmica se usó y cuál es su complejidad aproximada. Está pensado como guía de repaso para el curso.

---

## BackTracking

### 1. [`BackTracking/8_Rreinas`](BackTracking/8_Rreinas/main.cpp) — Problema de las 8 reinas

**Problema:** colocar 8 reinas en un tablero de 8x8 de modo que ninguna ataque a otra (misma fila, columna o diagonal).

**Estrategia:** backtracking columna por columna.
- `solve(tablero, col)` intenta colocar una reina en cada fila de la columna `col`.
- `esSeguro(tablero, fila, col)` valida que no haya otra reina en la misma fila a la izquierda, ni en las diagonales hacia la izquierda (superior e inferior). Como se avanza columna por columna, no hace falta revisar a la derecha porque ahí todavía no hay reinas colocadas.
- Si una colocación no lleva a solución, se deshace (`tablero[fila][col] = 0`) y se prueba la siguiente fila — el "retroceso" característico del backtracking.
- Caso base: `col >= MAX_REINAS` (se colocaron las 8 reinas).

**Complejidad:** en el peor caso exponencial (`O(N!)` aproximado para N-reinas), acotada en la práctica por la poda de `esSeguro`.

**Estado:** completo y funcional. Imprime el tablero antes (vacío) y después de resolver.

> Nota de detalle: `esSeguro` revisa las diagonales iterando con `tablero[fila][i]` en vez de `tablero[i][j]`; funciona porque al recorrer columna por columna solo puede haber una reina por fila, pero si se quisiera reutilizar esta función para otro recorrido conviene revisarla con cuidado.

---

### 2. [`BackTracking/Lab01_2024_2/Pregunta_2`](BackTracking/Lab01_2024_2/Pregunta_2/main.cpp) — Ciclo Hamiltoniano

**Problema:** dado un grafo representado como matriz de adyacencia, determinar si existe un ciclo Hamiltoniano (un recorrido que visita todos los vértices exactamente una vez y regresa al vértice inicial).

**Estrategia:** backtracking clásico sobre el camino (`path`).
- Se fija el vértice 0 como inicio (`path[0] = 0`).
- `backtrackingHamiltoniano` intenta agregar cada vértice `v` en la posición `pos` del camino.
- `esValido(v, pos, path, grafo)` comprueba dos cosas: que exista arista entre el vértice anterior del camino y `v`, y que `v` no haya sido visitado ya.
- Caso base: cuando `pos == V` (se visitaron todos los vértices), se verifica que el último vértice tenga arista de regreso al vértice inicial (`grafo[path[pos-1]][path[0]]`), cerrando el ciclo.
- Si una rama falla, se revierte la asignación (`path[pos] = -1`) y se prueba el siguiente vértice.

**Complejidad:** `O(V!)` en el peor caso (se exploran permutaciones de vértices), típico de este problema NP-difícil resuelto por fuerza bruta con poda.

**Estado:** completo. El `main` prueba dos grafos de ejemplo: uno con ciclo Hamiltoniano y otro sin él, imprimiendo el resultado en ambos casos.

---

### 3. [`BackTracking/Lab01_2026_1/Pregunta 1`](BackTracking/Lab01_2026_1/Pregunta%201/main.cpp) — Partición en dos subconjuntos con mínima diferencia

**Problema:** dado un conjunto de enteros `S`, dividirlo en dos subconjuntos `S1` y `S2` tales que la diferencia absoluta entre sus sumas sea mínima (idealmente 0, si el total es par y existe una partición exacta).

**Estrategia:** backtracking con poda, decidiendo para cada elemento si entra a `S1` o a `S2`.
- `particionBacktracking(S, indice, suma_actual, suma_total, min_dif)` explora, para cada elemento, las dos ramas: incluirlo en `S1` o dejarlo para `S2` (implícito, ya que `S2 = total - S1`).
- En cada llamada se actualiza `min_dif` si la diferencia actual (`|suma_actual - suma_S2|`) es mejor que la mejor encontrada.
- **Poda 1:** si `min_dif` llega a 0, se corta la recursión (ya no se puede mejorar una partición perfecta).
- **Poda 2:** solo se explora la rama "incluir" si `suma_actual + S[indice]` no se pasa de la mitad de la suma total (evita construir sumas inútilmente grandes).
- El arreglo se ordena de mayor a menor antes de empezar, lo que ayuda a que la poda por mitad de suma actúe más temprano.

**Complejidad:** en el peor caso `O(2^N)` (se decide incluir/no incluir cada elemento), reducido en la práctica por las podas.

**Estado:** completo y probado con el conjunto `{3, 1, 4, 2, 5, 1}` del enunciado.

---

### 4. [`BackTracking/Mochila_0_1`](BackTracking/Mochila_0_1/main.cpp) — Mochila 0/1 vía backtracking

**Problema:** resolver el problema de la mochila 0/1 (maximizar valor sin exceder una capacidad de peso) explorando combinaciones por backtracking.

**Estado:** ⚠️ **pendiente / sin implementar.** Por ahora el `main` solo declara el arreglo `objetos[MAX_OBJETOS]{4,5}` y no contiene la lógica recursiva de backtracking (no hay función de decisión incluir/no incluir, ni poda, ni cálculo de valor máximo). Queda como siguiente paso implementar la recursión análoga a la de `Lab01_2026_1/Pregunta 1`, pero evaluando `valor` en vez de solo la suma, y respetando `MAX_PESO_MOCHILA` como cota de la capacidad.

---

## Programación Dinámica

### 5. [`Programacion_Dinamica/Cajero_Automatico`](Programacion_Dinamica/Cajero_Automatico/main.cpp) — Mínima cantidad de monedas (coin change)

**Problema:** dado un conjunto de denominaciones de moneda (`{1, 3, 4}`) y un monto a retirar (`RETIRO = 6`), hallar la mínima cantidad de monedas necesarias para formar ese monto (monedas con repetición ilimitada).

**Estrategia:** programación dinámica tabulada, estilo "mochila ilimitada" en 2D.
- `DP[i][j]` representa el mínimo número de monedas para formar el monto `j` usando las monedas `0..i`.
- **Opción A** (no usar la moneda `i`): `DP[i-1][j]`.
- **Opción B** (usar la moneda `i`, que puede repetirse): `1 + DP[i][j - monedas[i]]` — nótese que se mantiene la fila `i` porque la misma moneda puede reutilizarse.
- El resultado final es el mínimo de ambas opciones.

**Complejidad:** `O(NUM_MONEDAS × RETIRO)` en tiempo y espacio.

**Estado:** ✅ completo. (Se corrigió un bug en el bucle interno, que antes usaba `i` en la condición y el incremento en vez de `j`, y en el acceso final a la matriz, que ahora lee `DP[NUM_MONEDAS - 1][RETIRO]`.) Probado con monedas `{1,3,4}` y retiro 6, da como resultado 2 monedas (`3+3`).

---

### 6. [`Programacion_Dinamica/Lab2_2026_1/Pregunta_1`](Programacion_Dinamica/Lab2_2026_1/Pregunta_1/main.cpp) — Scheduling de eventos con bono por continuidad

**Problema:** variante del clásico *weighted interval scheduling*. Se tiene una lista de eventos con hora de inicio, hora de fin y pago. Se busca maximizar la ganancia total elegible, con la regla adicional de que si el evento elegido anterior terminó exactamente 1 hora antes de que empiece el actual, se obtiene un bono de 15.

**Estrategia:** programación dinámica 1D sobre eventos ya ordenados por hora de fin.
- `DP[i]` = máxima ganancia considerando los primeros `i` eventos.
- **Opción A:** no incluir el evento `i` → `DP[i-1]`.
- Se busca `j`, el evento compatible más reciente (cuyo fin es al menos 1 hora antes del inicio del evento `i`), retrocediendo desde `i-1`.
- Se detecta si aplica el bono (el evento `j` termina *exactamente* 1 hora antes de que empiece `i`).
- **Opción B:** incluir el evento `i` → `pago[i] + bono + DP[j]`.
- `DP[i] = max(Opción A, Opción B)`.

**Complejidad:** `O(M^2)` en el peor caso por la búsqueda retroactiva de `j` dentro del bucle principal (podría optimizarse a `O(M log M)` con búsqueda binaria, ya que los eventos están ordenados por fin).

**Estado:** completo. Se prueba con 6 eventos de ejemplo y se imprime la ganancia máxima.

---

### 7. [`Programacion_Dinamica/Lab2_2026_1/Pregunta_2`](Programacion_Dinamica/Lab2_2026_1/Pregunta_2/main.cpp) — Validación de actas (caso ONCE) con subset-sum

**Problema:** contextualizado como una validación electoral: dadas cantidades fijas de actas por región más dos valores declarados (Lima y Extranjero), verificar tres condiciones sobre sumas de subconjuntos de regiones (total exacto de 95,000; suma de dos regiones específicas; diferencia exacta de 3,000 entre dos grupos).

**Estrategia:** mochila 0/1 / subset-sum tabulado.
- `DP[i][j]` = máxima suma de actas alcanzable usando un subconjunto de las primeras `i` regiones sin exceder la capacidad `j`.
- Recurrencia estándar de mochila 0/1: si el peso de la región `i` excede `j`, `DP[i][j] = DP[i-1][j]`; si no, `DP[i][j] = max(DP[i-1][j], actas[i] + DP[i-1][j-actas[i]])`.
- Las tres validaciones se responden consultando celdas de la matriz ya llena (`DP[N][MAX_ACTAS]`) y comparando sumas directas de subconjuntos de regiones.

**Complejidad:** `O(N × MAX_ACTAS)` tiempo y espacio (con `N=8` y `MAX_ACTAS=95000`, usa una matriz estática bastante grande).

**Estado:** completo. Se prueban dos envíos de ejemplo (`evaluarEnvio`) y se imprime si cada validación es correcta o incorrecta.

---

### 8. [`Programacion_Dinamica/Mochila_1_0`](Programacion_Dinamica/Mochila_1_0/main.cpp) — Mochila 0/1 clásica

**Problema:** el problema clásico de la mochila 0/1: dado un conjunto de objetos con peso y valor, maximizar el valor total sin exceder una capacidad máxima, pudiendo tomar cada objeto como máximo una vez.

**Estrategia:** programación dinámica bottom-up en una matriz `DP[objeto][capacidad]`.
- Para cada objeto `i` y cada capacidad `pesoActual`: si el objeto cabe (`objetos[i].peso <= pesoActual`), se compara no incluirlo (`DP[i-1][pesoActual]`) contra incluirlo (`objetos[i].valor + DP[i-1][pesoActual - objetos[i].peso]`), tomando el máximo.
- Si no cabe, se copia directamente el valor de la fila anterior.
- Incluye `imprimirDP` para visualizar la matriz completa, útil para seguir el proceso paso a paso.

**Complejidad:** `O(N_OBJETOS × CAPACIDAD_MAXIMA)` tiempo y espacio.

**Estado:** completo. Prueba con 3 objetos y capacidad máxima 9, imprimiendo la matriz DP y el valor máximo (`DP[N_OBJETOS][CAPACIDAD_MAXIMA]`).

---

### 9. [`Programacion_Dinamica/Subiendo_Escaleras`](Programacion_Dinamica/Subiendo_Escaleras/main.cpp) — Climbing stairs

**Problema:** contar de cuántas formas distintas se puede subir una escalera de `n` escalones, pudiendo avanzar de 1 o 2 escalones a la vez.

**Estrategia:** programación dinámica 1D bottom-up, equivalente a la secuencia de Fibonacci.
- `dp[i] = dp[i-1] + dp[i-2]`: para llegar al escalón `i`, se viene del escalón `i-1` (paso de 1) o del escalón `i-2` (paso de 2).
- Casos base: `dp[0] = 1`, `dp[1] = 1`.
- Se imprime la tabla completa de soluciones para los `n` escalones, además del resultado final.

**Complejidad:** `O(n)` tiempo y espacio.

**Estado:** completo. Ejecuta con `n = 4` como ejemplo (el valor está fijo en el código, no se lee por input).

---

### 10. [`Programacion_Dinamica/Varilla`](Programacion_Dinamica/Varilla/main.cpp) — Rod cutting

**Problema:** el problema clásico de corte de varilla: dada una varilla de longitud `L` y una tabla de precios según la longitud de cada corte, encontrar la forma de cortarla (en partes enteras) que maximice el ingreso total.

**Estrategia:** se implementan **ambos enfoques** de programación dinámica para comparar:

1. **`cortarVarillaRecursivo`** (top-down con memorización): función recursiva que, para cada longitud, prueba todos los primeros cortes posibles (`precios[i] + resultado_óptimo(longitud - i)`) y memoriza el resultado en `memoria[longitud_varilla]` para no recalcularlo.
2. **`cortarVarillaIterativo`** (bottom-up con tabulación): construye la tabla `tabla[]` de menor a mayor longitud, reutilizando los resultados ya calculados de longitudes menores.

Ambas resuelven la misma recurrencia: `mejorValor(n) = max sobre i en [1,n] de (precios[i] + mejorValor(n - i))`.

**Complejidad:** `O(L^2)` tiempo (por cada longitud se prueban todos los cortes posibles), `O(L)` espacio, en ambas versiones.

**Estado:** completo. El `main` ejecuta las dos versiones con `L = 4` y la tabla de precios del enunciado, e imprime ambos resultados (deben coincidir).

---

## Resumen de estado

| Ejercicio | Técnica | Estado |
|---|---|---|
| 8 Reinas | Backtracking | ✅ Completo |
| Ciclo Hamiltoniano | Backtracking | ✅ Completo |
| Partición mínima diferencia | Backtracking + poda | ✅ Completo |
| Mochila 0/1 (backtracking) | Backtracking | ⚠️ Pendiente de implementar |
| Cajero Automático | DP tabulada 2D | ✅ Completo |
| Scheduling con bono | DP 1D | ✅ Completo |
| Validación de actas (ONCE) | DP (subset-sum) | ✅ Completo |
| Mochila 1/0 | DP tabulada 2D | ✅ Completo |
| Subiendo escaleras | DP 1D (Fibonacci) | ✅ Completo |
| Varilla (rod cutting) | DP memorizada + iterativa | ✅ Completo |
