#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

/*
 * EJERCICIO 01 - GRASP: fase de construcción (mochila 0/1)
 *
 * Enunciado:
 *   Se tienen N = 8 objetos, cada uno con un peso y un valor, y una mochila
 *   de capacidad 20. Se quiere elegir objetos (cada uno a lo más una vez)
 *   que maximicen el valor total sin exceder la capacidad.
 *
 *   Implementa SOLO la fase de construcción de GRASP:
 *     - Función voraz de mérito: c(x) = valor / peso   (se MAXIMIZA)
 *     - En cada paso, considerando solo los objetos no usados que aún caben:
 *           beta = mejor c(x)      tau = peor c(x)
 *           RCL  = { x : c(x) >= beta - alfa * (beta - tau) }
 *       y elige UN objeto al azar de la RCL para agregarlo a la solución.
 *     - Termina cuando ya no quepa ningún objeto.
 *
 *   Repite la construcción ITERACIONES veces y quédate con la mejor solución.
 *   Ejecuta para alfa = 0.0, 0.3, 0.5 y 1.0 e imprime, para cada alfa,
 *   los objetos elegidos, el peso usado y el valor total.
 *
 * Preguntas:
 *   1. ¿Qué solución da alfa = 0 (voraz puro)? ¿Es la óptima?
 *   2. ¿Algún alfa intermedio encuentra algo mejor? ¿Por qué?
 *   3. ¿Qué pasa si pones ITERACIONES = 1?
 */
#define N 8
#define CAPACIDAD 20
#define ITERACIONES 50

struct Objeto {
    int peso;
    int valor;
};

int main() {
    srand(time(nullptr));
    Objeto objetos[N] = {
        {10, 60}, {6, 30}, {5, 28}, {4, 20},
        {9, 40},  {3, 13}, {7, 31}, {2, 7}
    };

    //TODO: tu solución

    return 0;
}
