#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;
#define N_OBJETOS 3
#define CAPACIDAD_MAXIMA 9

/*
 * Problema: mochila 0/1 clásica. Dado un conjunto de objetos con peso
 * y valor, maximizar el valor total sin exceder una capacidad máxima,
 * pudiendo tomar cada objeto como máximo una vez.
 *
 * Estrategia: DP tabulada bottom-up en una matriz DP[objeto][capacidad].
 * Para cada objeto i y capacidad j: si el objeto cabe, se compara no
 * incluirlo (DP[i-1][j]) contra incluirlo
 * (objetos[i].valor + DP[i-1][j - objetos[i].peso]), tomando el
 * máximo; si no cabe, se copia el valor de la fila anterior.
 *
 * Complejidad: O(N_OBJETOS x CAPACIDAD_MAXIMA) tiempo y espacio.
 */
struct Objeto {
    int peso;
    int valor;
};

void imprimirDP(int DP[][CAPACIDAD_MAXIMA+1],int filas,int columnas) {
    for (int i=0;i<filas;i++) {
        for (int j=0;j<columnas;j++) {
            cout<<setw(4)<<DP[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
}

void calcularObjetosOptimo(struct Objeto *objetos) {
    //la matriz ya se encuentra inicializada en 0
    //para todos sus valores
    int DP[N_OBJETOS+1][CAPACIDAD_MAXIMA+1]{};

    //el recorrido de DP será por columnas, pues más adelante necesitamos que
    //las columnas anteriores estén completas

    // Recorrido estándar: Filas (Objetos) -> Columnas (Capacidades)
    for (int i = 1; i <= N_OBJETOS; i++) {
        for (int pesoActual = 1; pesoActual <= CAPACIDAD_MAXIMA; pesoActual++) {
            if (objetos[i].peso <= pesoActual) {
                // Opción 1: No incluir el objeto i -> DP[i-1][pesoActual]
                // Opción 2: Incluir el objeto i -> valores[i] + DP[i-1][pesoActual - pesos[i]]
                int no_incluir = DP[i - 1][pesoActual];
                int incluir = objetos[i].valor + DP[i - 1][pesoActual - objetos[i].peso];

                DP[i][pesoActual] = max(no_incluir, incluir);
            } else {
                // El objeto pesa más que la capacidad actual, se copia el valor de arriba
                DP[i][pesoActual] = DP[i - 1][pesoActual];
            }
        }
    }

    imprimirDP(DP,N_OBJETOS+1,CAPACIDAD_MAXIMA+1);

    cout<<"El valor maximo obtenido será "<<DP[N_OBJETOS][CAPACIDAD_MAXIMA]<<endl;

}


int main() {
    struct Objeto objetos[N_OBJETOS+1] {
        {0,0},
        {2,3},
        {3,4},
        {4,5}
    };

    calcularObjetosOptimo(objetos);

    return 0;
}