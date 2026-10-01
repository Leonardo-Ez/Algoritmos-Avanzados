#include <iostream>
#include <iomanip>
using namespace std;

/*
 * Problema: mochila 0/1 vía backtracking. Dado un conjunto de
 * objetos (peso y valor) y una capacidad máxima, maximizar el valor
 * total sin exceder la capacidad, decidiendo para cada objeto si se
 * incluye o no (cada objeto se puede tomar como máximo una vez).
 *
 * Estrategia prevista: backtracking análogo al de
 * BackTracking/Lab01_2026_1/Pregunta 1 (incluir/no incluir cada
 * objeto), pero maximizando "valor" en vez de acercarse a una suma,
 * y podando las ramas que excedan MAX_PESO_MOCHILA.
 *
 * Estado: PENDIENTE. Todavía no se implementó la función recursiva
 * de backtracking ni el cálculo del valor máximo.
 */
#define MAX_PESO_MOCHILA 6
#define MAX_OBJETOS 10

int main() {

    int objetos[MAX_OBJETOS]{4,5};

    return 0;
}