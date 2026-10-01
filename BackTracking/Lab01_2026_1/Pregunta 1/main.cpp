#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

// Función recursiva de backtracking
// indice: posición actual en el conjunto
// suma_actual: suma del subconjunto S1 en la rama actual
// suma_total: suma de todos los elementos del conjunto S
// min_dif: referencia a la diferencia mínima global encontrada
void particionBacktracking(const vector<int>& S, int indice, int suma_actual, int suma_total, int& min_dif) {
    // Calculamos la suma del otro subconjunto S2
    int suma_S2 = suma_total - suma_actual;

    // Calculamos la diferencia actual absoluta
    int diferencia_actual = abs(suma_actual - suma_S2);

    // Actualizamos la diferencia mínima global si encontramos una mejor
    if (diferencia_actual < min_dif) {
        min_dif = diferencia_actual;
    }

    // Poda (Pruning): Si la diferencia es 0, es una partición perfecta (T es par)
    // No necesitamos seguir explorando esta rama.
    if (min_dif == 0) {
        return;
    }

    // Caso base: si llegamos al final del conjunto, terminamos esta rama
    if (indice == S.size()) {
        return;
    }

    // EXPLORACIÓN (Backtracking)

    // Opción 1: Incluir el elemento actual en el subconjunto S1
    // Solo lo incluimos si no superamos la mitad de la suma total para optimizar
    if (suma_actual + S[indice] <= suma_total / 2 + 1) {
        particionBacktracking(S, indice + 1, suma_actual + S[indice], suma_total, min_dif);
    }

    // Opción 2: No incluir el elemento actual en S1 (va a S2)
    particionBacktracking(S, indice + 1, suma_actual, suma_total, min_dif);
}

int main() {
    // Conjunto de ejemplo del enunciado
    vector<int> S = {3, 1, 4, 2, 5, 1};

    int suma_total = 0;
    for (int num : S) {
        suma_total += num;
    }

    // Se recomienda ordenar el arreglo para optimizar el backtracking
    // Lo ordenamos de forma descendente usando iteradores reversos
    sort(S.rbegin(), S.rend());

    int min_dif = INT_MAX;

    // Iniciamos el backtracking desde el índice 0 y suma actual 0
    particionBacktracking(S, 0, 0, suma_total, min_dif);

    cout << "La diferencia minima es: " << min_dif << endl;

    if (min_dif == 0) {
        cout << "Se logro una particion perfecta." << endl;
    } else {
        cout << "No es posible una particion perfecta." << endl;
    }

    return 0;
}