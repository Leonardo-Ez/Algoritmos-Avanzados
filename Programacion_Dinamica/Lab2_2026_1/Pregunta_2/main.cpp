#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Función para ejecutar las validaciones de un envío de la ONCE usando la matriz DP
void evaluarEnvio(int numeroEnvio, int lima, int extranjero) {
    // 8 Regiones electorales
    const int N = 8;
    // La suma objetivo total máxima es 95,000 actas
    const int MAX_ACTAS = 95000;

    // Regiones y sus cantidades fijas o propuestas
    // 1: Costa Sur, 2: Costa Norte, 3: Sierra Centro, 4: Sierra Norte,
    // 5: Sierra Sur, 6: Oriente, 7: Lima, 8: Extranjero
    int actas[N + 1] = {0, 6000, 12000, 8000, 12000, 15000, 4500, lima, extranjero};

    // Matriz de Programación Dinámica (Se utiliza una única matriz para todas las validaciones)
    // DP[i][j] almacenará el valor máximo de actas alcanzable utilizando un subconjunto
    // de las primeras 'i' regiones con un límite/capacidad de 'j' actas.
    static int DP[N + 1][MAX_ACTAS + 1];

    /*
     * FORMA DE SOLUCIÓN (Comentario obligatorio para el examen):
     * Estrategia: Programación Dinámica - Suma de Subconjuntos / Mochila 0/1 (Bottom-Up).
     * Estado: DP[i][j] guarda la suma de actas exacta alcanzable considerando las primeras 'i' regiones.
     * Transición: Para cada región 'i' con peso actas[i]:
     *   - Si actas[i] > j: DP[i][j] = DP[i-1][j] (No se incluye la región)
     *   - Si actas[i] <= j: DP[i][j] = max(DP[i-1][j], actas[i] + DP[i-1][j - actas[i]])
     * Una vez llenada la matriz, se efectúan las 3 validaciones requeridas consultando las celdas directas.
     */

    // Llenado iterativo de la matriz de soluciones (Bottom-Up)
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= MAX_ACTAS; j++) {
            if (actas[i] > j) {
                DP[i][j] = DP[i - 1][j];
            } else {
                DP[i][j] = max(DP[i - 1][j], actas[i] + DP[i - 1][j - actas[i]]);
            }
        }
    }


    // --- VALIDACIONES MEDIANTE LA MATRIZ DE SOLUCIONES ---

    // 1. Validación: Total de actas sea 95,000
    // Se verifica si la celda DP[8][95000] alcanza exactamente 95,000
    bool v1 = (DP[N][MAX_ACTAS] == 95000);

    // 2. Validación: Suma de Oriente (4,500) y Extranjero sea 7,000
    int sumaOrienteExtranjero = actas[6] + actas[8];
    bool v2 = (sumaOrienteExtranjero == 7000);

    // 3. Validación: Diferencia de 3,000 actas entre los dos grupos
    // Grupo A: Extranjero + Oriente + Costa Sur + Sierra Centro
    int grupoA = actas[8] + actas[6] + actas[1] + actas[3];
    // Grupo B: Costa Norte + Sierra Norte
    int grupoB = actas[2] + actas[4];
    bool v3 = (abs(grupoA - grupoB) == 3000);

    // --- IMPRESIÓN DE RESULTADOS ---
    cout << "==========================================" << endl;
    cout << "          ENVIO " << numeroEnvio << " DE LA ONCE" << endl;
    cout << "==========================================" << endl;
    cout << "Declaracion: Lima = " << lima << " | Extranjero = " << extranjero << endl << endl;

    cout << "Resultados de las Validaciones del JNV:" << endl;
    cout << "- Cantidad total de actas: " << (v1 ? "CORRECTA" : "INCORRECTA") << endl;
    cout << "- Suma actas Oriente y Extranjero: " << (v2 ? "CORRECTA" : "INCORRECTA") << endl;
    cout << "- Diferencia de actas entre grupos: " << (v3 ? "CORRECTA" : "INCORRECTA") << endl;

    cout << "\nRESULTADO FINAL: ";
    if (v1 && v2 && v3) {
        cout << "Cantidades CORRECTAS (Aprobado por el JNV)" << endl;
    } else {
        cout << "Cantidades INCORRECTAS (Rechazado por el JNV)" << endl;
    }
    cout << endl;
}

int main() {
    // Primer envío de la ONCE (Lima: 35,200 | Extranjero: 2,300)
    evaluarEnvio(1, 35200, 2300);

    // Segundo envío de la ONCE (Lima: 35,000 | Extranjero: 2,500)
    evaluarEnvio(2, 35000, 2500);

    return 0;
}