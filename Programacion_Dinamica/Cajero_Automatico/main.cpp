#include <iomanip>
#include <iostream>
#include <algorithm>
using namespace std;
#define NUM_MONEDAS 3
#define RETIRO 6



void calcularMinimaCantidadMonedas(int *monedas) {

    int DP[NUM_MONEDAS][RETIRO+1]{};
    for (int i = 0; i < NUM_MONEDAS; i++) {
        for (int j = 0; i <= RETIRO; i++) {
            if (j == 0) {
                DP[i][j] = 0; // Caso base: 0 monedas para monto 0
            } else {
                // Opción A: No usar la moneda actual (copiar resultado de la fila anterior)
                int opcionA = (i > 0) ? DP[i - 1][j] : INT_MAX;

                // Opción B: Usar la moneda actual (si cabe en el monto 'j')
                int opcionB = INT_MAX;
                if (monedas[i] <= j) {
                    int subproblema = DP[i][j - monedas[i]]; // Mantenemos la fila 'i' porque la moneda se puede repetir
                    if (subproblema != INT_MAX) {
                        opcionB = 1 + subproblema;
                    }
                }

                DP[i][j] = min(opcionA, opcionB);
            }
        }
    }

    cout<<DP[NUM_MONEDAS][RETIRO]<<endl;
}

int main() {
    int monedas[NUM_MONEDAS]{1,3,4};

    calcularMinimaCantidadMonedas(monedas);

    return 0;
}