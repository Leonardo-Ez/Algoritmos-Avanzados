#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

/*
 * EJERCICIO 02 - GRASP: fase de construcción (ruta del repartidor)
 *
 * Autoría:
 *   - Enunciado e instancias de prueba (5 y 15 ciudades): redactados por
 *     Claude (Anthropic) como ejercicio de práctica.
 *   - Implementación: Hector Leonardo Bonifacio Espinoza :).
 *
 * Enunciado:
 *   Un repartidor sale de la ciudad A y debe visitar B, C, D y E una vez
 *   cada una, en cualquier orden, sin volver a A. Se quiere MINIMIZAR la
 *   distancia total recorrida.
 *
 *   Ciudades: A=0, B=1, C=2, D=3, E=4 (la matriz es simétrica).
 *
 *   Implementa la fase de construcción de GRASP:
 *     - Candidatos: las ciudades aún no visitadas.
 *     - Mérito c(x): distancia desde la ciudad actual a x (se MINIMIZA).
 *     - En cada paso:
 *           beta = menor distancia    tau = mayor distancia
 *           RCL  = { x : c(x) <= beta + alfa * (tau - beta) }
 *       y se elige UNA ciudad al azar de la RCL.
 *
 *   Repite la construcción ITERACIONES veces y guarda la mejor ruta.
 *   Imprime la mejor ruta (con letras) y su distancia total.
 *
 * Comprobación (lo que hiciste a mano):
 *   - Con alfa = 0 debe salir A-B-D-E-C = 18 (voraz).
 *   - Con alfa = 0.5 debería encontrar A-C-B-D-E = 16.
 *   - ¿Qué pasa con alfa = 0.2? ¿Y con alfa = 0.25? (piensa en el paso 1)
 */
#define NUM_CIUDADES 15
#define INICIO 0
//#define ITERACIONES 1000000 ejecutar bajo tu propio riesgo XDD
#define ITERACIONES 20
#define ALPHA 0.5

struct Solucion {
    vector <char> solucion;
    int distancia;
};

bool estaEnlaSolucion(const vector<char>& solucion,char car) {
    for (auto caracter : solucion) {
        if (caracter == car) return true;
    }
    return false;
}

int buscarMenor(const int *distancias,const vector<char> solucion) {
    int menor=INT_MAX;
    for(int i=0;i<NUM_CIUDADES;i++) {
        if(distancias[i]<menor and !estaEnlaSolucion(solucion,'A'+i)) {
            menor=distancias[i];
        }
    }
    return menor;
}


int encontrarMayor(const int *distancias, const vector<char> &solucion) {
    int mayor=0;
    for(int i=0;i<NUM_CIUDADES;i++) {
        if (distancias[i]>mayor and !estaEnlaSolucion(solucion,'A'+i)) {
            mayor=distancias[i];
        }
    }
    return mayor;
}


vector <int> encontrarCumplenUmbral(const int *distancias,double umbral,const vector<char> solucion) {
    vector <int> cumplenUmbral{};
    for(int i=0;i<NUM_CIUDADES;i++) {
        if (distancias[i]<=umbral and !estaEnlaSolucion(solucion,'A'+i)) cumplenUmbral.push_back(i);
    }
    return cumplenUmbral;
}

Solucion encontrarMejorRuta(int distancias[][NUM_CIUDADES],int inicio) {
    int numSolucion=0;
    vector <char> mejorSolucion{};
    int mejorDistancia=INT_MAX;
    while (numSolucion<ITERACIONES) {
        int i=1;//ya se insertó el inicial
        vector<char> solucion;
        int posActual=inicio;
        int distancia=0;
        solucion.push_back('A'+inicio);
        //salimos cuando los hayamos visitado todos
        while (i<NUM_CIUDADES) {
            int beta=buscarMenor(distancias[posActual],solucion);
            int tau=encontrarMayor(distancias[posActual],solucion);
            //como queremos minimizar:
            double umbral=beta+ALPHA*(tau-beta);
            //encontramos todas las sub Soluciones que cumplan el umbral
            vector<int> subSoluciones=encontrarCumplenUmbral(distancias[posActual],umbral,solucion);
            //sacamos un valor aleatorio de todas las subSoluciones
            int pos=rand()%subSoluciones.size();
            //almacenamos la subSolucion
            solucion.push_back('A'+subSoluciones[pos]);
            //agregamos la distancia actual
            distancia+=distancias[posActual][subSoluciones[pos]];
            //actualizamos la posición actual
            posActual=subSoluciones[pos];
            //incrementamos el número de nodos visitados
            i++;
        }
        if (distancia<mejorDistancia) {
            mejorDistancia=distancia;
            mejorSolucion=solucion;
        }
        numSolucion++;
    }

    return {mejorSolucion,mejorDistancia};

};



int main() {
    srand(time(nullptr));
    /* Instancia de 5 ciudades (NUM_CIUDADES 5): óptimo A-C-B-D-E = 16
    int distancias[NUM_CIUDADES][NUM_CIUDADES] = {
        //A  B  C  D  E
        { 0, 2, 3, 5, 6},  //A
        { 2, 0, 6, 4, 9},  //B
        { 3, 6, 0, 8, 9},  //C
        { 5, 4, 8, 0, 3},  //D
        { 6, 9, 9, 3, 0}   //E
    };
    */

    /* Instancia de 15 ciudades (NUM_CIUDADES 15)
     *   Óptimo exacto (programación dinámica): 131
     *   A-C-F-I-G-B-M-L-N-K-O-E-J-D-H
     *   Voraz (alfa = 0): 160 o 161 (hay empates de distancia en la RCL) */
    int distancias[NUM_CIUDADES][NUM_CIUDADES] = {
        // A   B   C   D   E   F   G   H   I   J   K   L   M   N   O
        { 0, 20,  5, 21, 28,  5, 14, 19, 11, 24, 42, 37, 36, 44, 28},  //A
        {20,  0, 15, 28, 20, 15,  9, 32, 13, 13, 34, 22, 15, 34, 20},  //B
        { 5, 15,  0, 21, 25,  1,  9, 21,  8, 20, 39, 33, 31, 41, 25},  //C
        {21, 28, 21,  0, 17, 22, 28,  8, 28, 19, 26, 30, 40, 31, 17},  //D
        {28, 20, 25, 17,  0, 25, 25, 25, 28,  7, 15, 13, 26, 16,  1},  //E
        { 5, 15,  1, 22, 25,  0,  9, 22,  7, 20, 40, 33, 30, 42, 25},  //F
        {14,  9,  9, 28, 25,  9,  0, 30,  4, 19, 40, 30, 23, 41, 25},  //G
        {19, 32, 21,  8, 25, 22, 30,  0, 29, 26, 34, 38, 45, 39, 25},  //H
        {11, 13,  8, 28, 28,  7,  4, 29,  0, 22, 43, 34, 27, 44, 28},  //I
        {24, 13, 20, 19,  7, 20, 19, 26, 22,  0, 22, 13, 21, 22,  7},  //J
        {42, 34, 39, 26, 15, 40, 40, 34, 43, 22,  0, 18, 36,  7, 15},  //K
        {37, 22, 33, 30, 13, 33, 30, 38, 34, 13, 18,  0, 19, 14, 13},  //L
        {36, 15, 31, 40, 26, 30, 23, 45, 27, 21, 36, 19,  0, 33, 26},  //M
        {44, 34, 41, 31, 16, 42, 41, 39, 44, 22,  7, 14, 33,  0, 16},  //N
        {28, 20, 25, 17,  1, 25, 25, 25, 28,  7, 15, 13, 26, 16,  0}   //O
    };

    //TODO: tu solución
    //Hola:) asjasjajs
    Solucion solucion=encontrarMejorRuta(distancias,INICIO);
    if (solucion.solucion.empty()) {
        cout<<"No se encontró solución"<<endl;
    }
    else {
        cout<<"mejor solucion encontrada:"<<endl;
        int i=0;
        for (auto pos:solucion.solucion) {
            if (i>0) cout<<'-';
            cout<<pos;
            i++;
        }
        cout<<endl;
        cout<<"Distancia: "<<solucion.distancia<<endl;
        cout<<endl;
    }


    return 0;
}
