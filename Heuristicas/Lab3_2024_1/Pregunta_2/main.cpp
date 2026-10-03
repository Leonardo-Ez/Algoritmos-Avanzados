#include <iomanip>
#include <iostream>
#include <cstdlib>
using namespace std;
#define NUM_PAQUETES 5
struct Paquete {
    int ganancia;
    int peso;
    double relacion;
};

void caluclarRelacion(Paquete *paquetes) {
    for (int i = 0; i < NUM_PAQUETES; i++) {
        paquetes[i].relacion=(double)paquetes[i].ganancia/paquetes[i].peso;
    }
}

int compare(const void* a, const void* b) {
    const Paquete paqueteA = *(const Paquete *)a;
    const Paquete paqueteB = *(const Paquete *)b;

    if (paqueteA.relacion < paqueteB.relacion) return 1;
    if (paqueteA.relacion > paqueteB.relacion) return -1;
    return 0;
}



void seleccionarPaquete(Paquete *paquetes, int pesoMax) {

    caluclarRelacion(paquetes);

    qsort(paquetes, NUM_PAQUETES, sizeof(Paquete), compare);

    int gananciaTotal = 0;
    int pesoTotal=0;
    for (int i = 0; i < NUM_PAQUETES; i++) {
        if (paquetes[i].peso <= pesoMax-pesoTotal) {
            pesoTotal+=paquetes[i].peso;
            gananciaTotal+=paquetes[i].ganancia;
        }
    }
    cout<<"Solución empleando un algoritmo heurístico voraz:"<<endl;
    cout<<"Peso sobrante en el contenedor: "<<pesoMax-pesoTotal<<" Tn "<<endl;
    cout<<"Ganancia de la exportación: "<<gananciaTotal<<endl;
}

int main() {


    Paquete paquetes[]{
        {10,2},
        {15,3},
        {10,5},
        {24,12},
        {8,2}
    };

    seleccionarPaquete(paquetes, 16);


    return 0;
}