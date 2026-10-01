#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;
#define M 6
#define N 4
#define FIN 2
#define INICIO 1
#define PAGO 3

/*
 * Problema: variante del "weighted interval scheduling". Se tiene una
 * lista de eventos (inicio, fin, pago) ya ordenados por hora de fin.
 * Se busca maximizar la ganancia total elegible, con la regla
 * adicional de que si el evento elegido anterior terminó EXACTAMENTE
 * 1 hora antes de que empiece el actual, se obtiene un bono de 15.
 *
 * Estrategia: DP 1D sobre eventos ordenados por fin. DP[i] = máxima
 * ganancia considerando los primeros i eventos.
 *   - Opción A (no incluir el evento i): DP[i-1]
 *   - Se busca j = el evento compatible más reciente (fin + 1 <= inicio
 *     del evento i), retrocediendo desde i-1.
 *   - Opción B (incluir el evento i): pago[i] + bono + DP[j], donde el
 *     bono (15) se aplica solo si el evento j termina exactamente 1
 *     hora antes de que empiece el evento i.
 *   - DP[i] = max(Opción A, Opción B)
 *
 * Complejidad: O(M^2) en el peor caso por la búsqueda retroactiva de j
 * (podría bajarse a O(M log M) con búsqueda binaria).
 */
int maximizarGanancia(int eventos[][N]) {
    //consideramos que los eventos ya vienen ordenados
    //por su hora de fin de menor a mayor

    //tenemos los registros
    int DP[M+1]{}; //generamos los registros inicializados en 0


    for (int i = 1; i <= M; i++) {//recorremos todas las cantidades de eventos posibles
        int opcion_A=DP[i-1];//la opcion A es que el evento actual no se incluya

        int j=i-1;//buscaremos el último evento que agregamos y cumpla con las condiciones
        while (j>0 and eventos[j][FIN]+1>eventos[i][INICIO]) {
            /*
             * siempre que estemos dentro de "eventos" y que el evento verificado -> j sea
             * por lo menos 1 hora después del evento actual -> i
             */
            j--;//decrementamos j para recorrer todos los eventos anteriores
        }

        //verificamos si se aplica el bono
        int bono=0; //inicializamos
        if (j>0 and eventos[j][FIN]+1==eventos[i][INICIO]) {
            /*
             * El evento verificado es mayo a 0? (caso base)
             * y si el evento j anterior es exactamente 1 hora antes del evento actual
             */
            bono=15; //aplicamos el bono
        }

        /*
         * Seleccionamos el evento actual, consideramos su pago base,
         * sumamos el bono (0 o 15)
         * Sumamos el maximo para el evento verificado y que guardamos en los registros(j)
         */
        int opcion_B=eventos[i][PAGO]+bono+DP[j];

        //actualizamos el valor maximo para el evento actual en el registro
        DP[i]=max(opcion_A,opcion_B);

    }


    return DP[M];//retornamos el valor mas alto para todas las cantidades de eventos
}


int main() {
    //Num Evento Inicio Fin Pago
    int datos[M+1][N] {
        {0,0,0,0},//consideramos el caso base
        {1,1,3,30},
        {2,4,5,10},
        {3,6,8,60},
        {4,6,8,20},
        {5,5,9,50},
        {6,8,12,40}
    };

    cout<<"La ganancia máxima final es S/. "<<maximizarGanancia(datos);

    return 0;
}
