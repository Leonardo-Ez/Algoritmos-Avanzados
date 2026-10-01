#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;
#define M 6
#define N 4
#define FIN 2
#define INICIO 1
#define PAGO 3


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
