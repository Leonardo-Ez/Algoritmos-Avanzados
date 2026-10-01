#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;
#define N_OBJETOS 3
#define CAPACIDAD_MAXIMA 9

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