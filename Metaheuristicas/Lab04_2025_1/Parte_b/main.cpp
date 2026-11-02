#include <algorithm>
#include <float.h>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

/*
 * LABORATORIO 4 (2025-1) - Pregunta 3, parte b
 * Algoritmos Avanzados - PUCP
 *
 * Autoría:
 *   - Enunciado: Laboratorio 4 del curso (2025-1).
 *   - Implementación: Hector Leonardo Bonifacio Espinoza :).
 *
 * Criterio de selección:
 *   - Primero la prioridad (se toma como mayor prioridad el número más alto:
 *     3 > 2 > 1) y, entre órdenes de la misma prioridad, la mayor cantidad
 *     de botellas.
 *   - La RCL se arma solo con las órdenes de la prioridad actual, usando la
 *     cantidad de botellas como mérito (se maximiza). Cuando ya no quedan
 *     órdenes de esa prioridad, se pasa a la siguiente.
 *
 * Mejores resultados posibles:
 *   - Óptimo global del reparto: 2688 minutos (cota 448 botellas * 6 min, ver
 *     la parte a). Esta construcción no lo alcanza, porque atender primero
 *     toda una prioridad le quita libertad para equilibrar las líneas.
 *   - Lo mejor que logra esta construcción (100000 corridas por alfa):
 *       alfa = 0 (voraz): 2826    alfa = 0.3: 2754
 *       alfa = 0.5:       2724    alfa = 1:   2706
 *   - Con alfa = 0.3 y 200 iteraciones, el programa da 2754 o 2760.
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
    return (orden1.prioridad > orden2.prioridad) or
    (orden1.prioridad == orden2.prioridad and orden1.cantidasBotellas > orden2.cantidasBotellas);
}


vector<Orden> encontrarOrdenesNoProcesadasPrioridad(const vector<Orden> &ordenes,int prioridad) {
    vector<Orden> result;
    for (auto orden: ordenes) {
        if (!orden.usado and prioridad==orden.prioridad) {
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
    const Orden &seleccionado,double tiempoLinea) {
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
        int prioridadActual=ordenes.front().prioridad;//tomamos como mayor prioridad el de mayor valor
        while (prioridadActual) {
            vector<Orden> ordenesCopia=encontrarOrdenesNoProcesadasPrioridad(ordenes,prioridadActual);
            if (!ordenesCopia.empty()) {
                //cuando ya no queden más ordenes que procesar, se sale de la iteración
                //a este punto el mayor queda delante y el menor queda detrás
                int beta=ordenesCopia.front().cantidasBotellas;
                int tau=ordenesCopia.back().cantidasBotellas;

                double umbral=beta-ALFA*(beta-tau);

                vector<Orden> sobreUmbral=encontrarOrdenesSobreUmbral(ordenesCopia,umbral);
                int posOrden=rand()%sobreUmbral.size();
                asignarMenosCargado(lineas,sobreUmbral[posOrden],tiempoLinea);
                cambiarUsado(ordenes, sobreUmbral[posOrden].id);
            }
            else {
                prioridadActual--;
            }

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
