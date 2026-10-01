#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Número de escalones
    int n = 4;

    // Caso base directo
    if (n == 0 || n == 1) {
        cout << "Formas de subir " << n << " escalones: 1" << endl;
        return 0;
    }

    // Arreglo de programación dinámica para almacenar las soluciones
    // dp[i] almacenará el número de formas distintas de llegar al escalón i
    vector<int> dp(n + 1, 0);

    // Formas de solución / Explicación del enfoque:
    // Utilizaremos Programación Dinámica con enfoque Bottom-Up.
    // Estado: dp[i] es la cantidad de formas únicas de alcanzar el escalón i.
    // Transición: Para llegar al escalón i, solo se puede provenir de:
    //   1. Del escalón (i - 1) dando un paso de 1 escalón.
    //   2. Del escalón (i - 2) dando un paso de 2 escalones.
    // Por lo tanto, la ecuación de recurrencia es: dp[i] = dp[i-1] + dp[i-2].

    // Casos Base
    dp[0] = 1; // 1 forma de estar en la base (no dar pasos)
    dp[1] = 1; // 1 forma de llegar al primer escalón (dar 1 paso de 1)

    // Llenado iterativo de la tabla (Bottom-Up)
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    // Muestreo del contenido del arreglo de soluciones
    cout << "--- Arreglo de Soluciones (DP) ---" << endl;
    for (int i = 0; i <= n; i++) {
        cout << "Escalon " << i << ": " << dp[i] << " formas" << endl;
    }

    cout << "\nEl número total de formas de subir " << n << " escalones es: " << dp[n] << endl;

    return 0;
}