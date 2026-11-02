#include <algorithm>
#include <float.h>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

/*
 * LABORATORIO 4 (2025-1) - Pregunta 3, parte a
 * Algoritmos Avanzados - PUCP
 *
 * Autoría:
 *   - Enunciado: Laboratorio 4 del curso (2025-1).
 *   - Implementación: Hector Leonardo Bonifacio Espinoza :).
 *
 * Mejor opción posible (óptimo global): makespan = 2688 minutos
 *   - Cada botella pasa por las 4 máquinas: 2 + 1 + 1.5 + 1.5 = 6 minutos.
 *   - Las 20 órdenes suman 2236 botellas; repartidas en 5 líneas son
 *     2236 / 5 = 447.2 botellas por línea en el mejor caso.
 *   - Como no se pueden partir botellas, alguna línea carga al menos 448:
 *     448 * 6 = 2688 minutos es una cota inferior (nadie puede bajar de ahí).
 *   - Esta implementación (alfa = 0.3, 50 iteraciones) llega a 2688 en algunas
 *     corridas (en otras da 2694 o 2700, por el azar), así que 2688 es el
 *     óptimo. El 2724 del enunciado es una buena solución, pero no la mejor.
 */
#define M 5
#define N 20
#define ALFA 0.3

struct Orden {
    int id;
    int cantidasBotellas;
    int prioridad;
    bool usado;
};

struct Maquina {
    string nombre;
    double tiempoDemora;
};

struct Linea {
    vector<int> id;
    double tiempoDemora;
};

bool compararOrdenes(const Orden &orden1,const Orden &orden2) {
    return orden1.cantidasBotellas > orden2.cantidasBotellas;
}


vector<Orden> encontrarOrdenesNoProcesadas(const vector<Orden> &ordenes) {
    vector<Orden> result;
    for (auto orden: ordenes) {
        if (!orden.usado) {
            result.push_back(orden);
        }
    }
    return result;
}

vector<Orden> encontrarOrdenesSobreUmbral(const vector<Orden> &ordenesCopia,double umbral) {
    vector<Orden> result;
    for (auto orden: ordenesCopia) {
        if (orden.cantidasBotellas>=umbral) {
            result.push_back(orden);
        }
    }
    return result;
}

void asignarMenosCargado(Linea *lineas,
    Orden &seleccionado,double tiempoLinea) {
    double menor=DBL_MAX;
    int posMenor=-1;
    for (int i=0;i<M;i++) {
        if (lineas[i].tiempoDemora<menor) {
            menor=lineas[i].tiempoDemora;
            posMenor=i;
        }

    }
    if (posMenor!=-1) {
        lineas[posMenor].id.push_back(seleccionado.id);
        lineas[posMenor].tiempoDemora+=tiempoLinea*seleccionado.cantidasBotellas;
    }

}

double calcularTiempo(const vector<Maquina> &maquinas) {
    double tiempo=0;
    for (const auto& maquina: maquinas) {
        tiempo+=maquina.tiempoDemora;
    }
    return tiempo;
}

void resetearUsados(vector<Orden> &ordenes) {
    for (auto &orden: ordenes) {
        orden.usado=false;
    }
}

void imprimirSolucion(Linea *mejorSolucion, double numMinutosSol) {
    cout<<"Mejor Makespan: "<<numMinutosSol<<" minutos"<<endl;
    for (int i=0;i<M;i++) {
        cout<<"Linea "<<i+1<<": ";
        for (auto orden: mejorSolucion[i].id) {
            cout<<orden<<" ";
        }
        cout<<endl;
    }
}

void cambiarUsado(vector<Orden> &ordenes,int id) {
    for (auto &orden:ordenes) {
        if (id==orden.id) {
            orden.usado=true;
        }
    }
}

void generarMakespan(const vector<Maquina> &maquinas,
                     vector<Orden> ordenes,int iteraciones) {


    double tiempoLinea=calcularTiempo(maquinas);

    Linea mejorSolucion[M]{};
    double numMinutosSol=0;
    sort(ordenes.begin(), ordenes.end(),compararOrdenes);
    while (iteraciones){
        Linea lineas[M]{};
        //copiamos el vector de las ordenes, con las ordenes no procesadas
        resetearUsados(ordenes);
        while (true) {
            vector<Orden> ordenesCopia=encontrarOrdenesNoProcesadas(ordenes);
            if (ordenesCopia.empty()) break;//cuando ya no queden más ordenes que procesar, se sale de la iteración
            //a este punto el mayor queda delante y el menor queda detrás
            int beta=ordenesCopia.front().cantidasBotellas;
            int tau=ordenesCopia.back().cantidasBotellas;

            double umbral=beta-ALFA*(beta-tau);

            vector<Orden> sobreUmbral=encontrarOrdenesSobreUmbral(ordenesCopia,umbral);
            int posOrden=rand()%sobreUmbral.size();
            asignarMenosCargado(lineas,sobreUmbral[posOrden],tiempoLinea);
            cambiarUsado(ordenes, sobreUmbral[posOrden].id);
        }
        double makespan = 0;
        for (int i = 0; i < M; i++)
            makespan = max(makespan, lineas[i].tiempoDemora);

        if (numMinutosSol == 0 || makespan < numMinutosSol) {
            for (int i = 0; i < M; i++) mejorSolucion[i] = lineas[i];
            numMinutosSol = makespan;
        }

        iteraciones--;
    }

    imprimirSolucion(mejorSolucion, numMinutosSol);


}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    vector<Maquina> maquinas{
        {"Llenadora de liquido",2},
        {"Gasificadora",1},
        {"Tapadora",1.5},
        {"Etiquetadora",1.5}
    };

    vector<Orden> ordenes{
        {1 ,103, 2},
        {2 ,58 ,1},
        {3 ,88 ,3},
        {4 ,126 ,1},
        {5 ,195, 2},
        {6 ,90 ,2},
        {7 ,54 ,3},
        {8 ,195, 1},
        {9 ,124 ,2},
        {10, 113 ,3},
        {11 ,193, 2},
        {12 ,55 ,1},
        {13, 97 ,1},
        {14 ,97 ,2},
        {15 ,169 ,2},
        {16, 50 ,2},
        {17 ,167 ,3},
        {18 ,74 ,1},
        {19 ,79 ,2},
        {20,109,1}
    };


    generarMakespan(maquinas, ordenes,200);


    return 0;
}
