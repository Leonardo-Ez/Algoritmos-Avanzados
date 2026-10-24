#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <climits>
using namespace std;

/*
 * EJERCICIO 03 - GRASP: fase de construcción (estaciones de bomberos)
 *
 * Autoría:
 *   - Enunciado e instancia de prueba: redactados por Claude (Anthropic)
 *     como ejercicio de práctica.
 *   - Implementación: Hector Leonardo Bonifacio Espinoza :).
 *
 * Enunciado:
 *   Una municipalidad quiere que sus 6 distritos (A, B, C, D, E, F) queden
 *   protegidos por al menos una estación de bomberos. Hay 5 lugares posibles
 *   para construir estaciones; cada uno tiene un costo (millones) y cubre
 *   ciertos distritos. Se quiere MINIMIZAR el costo total de modo que TODOS
 *   los distritos queden cubiertos.
 *
 *     #  Estación      Costo  Distritos que cubre
 *     0  Centro          8    A B C D
 *     1  Norte           4    A B C
 *     2  Sur             5    D E F
 *     3  Playa           3    E F
 *     4  Industrial      9    C D E F
 *
 *   Implementa la fase de construcción de GRASP:
 *     - Candidatos: estaciones aún no construidas que cubren AL MENOS UN
 *       distrito que todavía está sin cubrir.
 *     - Mérito c(x) = costo / (cantidad de distritos SIN CUBRIR que cubre x)
 *       (se MINIMIZA: el mejor es el menor, es "cuánto pago por distrito nuevo").
 *     - En cada paso:
 *           beta = menor c(x)      tau = mayor c(x)
 *           RCL  = { x : c(x) <= beta + alfa * (tau - beta) }
 *       y se elige UNA estación al azar de la RCL.
 *     - La construcción termina cuando todos los distritos están cubiertos.
 *
 *   Repite la construcción ITERACIONES veces y guarda la mejor solución.
 *   Imprime las estaciones elegidas y el costo total.
 *
 * NOVEDAD respecto a la mochila:
 *   En la mochila el mérito valor/peso era FIJO. Aquí el mérito CAMBIA en cada
 *   paso: cuando construyes una estación, los distritos que cubre dejan de
 *   contar como "nuevos" para las demás. Hay que recalcular c(x) en cada paso
 *   (es la "A" de Adaptive en GRASP).
 *
 * Antes de programar (a mano, con alfa = 0.5):
 *   Haz una tabla por paso:  sin cubrir | c(x) de cada candidata | beta | tau
 *                            | umbral | RCL | elegida
 *   - Construcción 1: de la RCL sale siempre la estación de # MÁS PEQUEÑO.
 *   - Construcción 2: de la RCL sale siempre la estación de # MÁS GRANDE.
 *   - Haz también el voraz (alfa = 0).
 *
 * Preguntas:
 *   1. ¿Cuánto cuesta la solución del voraz? ¿Alguna de sus estaciones sobra?
 *   2. ¿Cuál de las dos construcciones es mejor? ¿Qué decisión hizo la diferencia?
 *   3. ¿Por qué una estación que cubre 0 distritos nuevos NO puede ser candidata?
 *      (pista: mira la fórmula del mérito)
 */
#define NUM_ESTACIONES 5
#define NUM_DISTRITOS 6
#define ITERACIONES 50
#define ALFA 0.5

struct Estacion {
    int numero;
    int costo;
    vector<char> cubre;
    double merito;
};

void listarCubre(Estacion &estacion,int *cubre) {
    for(int i=0;i<NUM_DISTRITOS;i++) {
        if (cubre[i]==1) {
            estacion.cubre.push_back('A'+i);
        }
    }
}

vector<Estacion> inicializarDistritos(int cubre[][NUM_DISTRITOS],
    int *costo) {
    vector<Estacion> estaciones;
    for (int i = 0; i < NUM_ESTACIONES; i++) {
        Estacion estacion;
        estacion.numero = i;
        estacion.costo = costo[i];
        listarCubre(estacion,cubre[i]);
        estaciones.push_back(estacion);
    }
    return estaciones;
}

bool seEncuentraCubierto(char distrito,const vector<char> &distritoCubiertos) {
    for (auto distritoCubierto: distritoCubiertos) {
        if (distritoCubierto==distrito) {
            return true;
        }
    }
    return false;
}

vector<Estacion> calcularMerito(vector<Estacion> &estaciones,
    const vector<char> &distritosCubiertos) {
    vector<Estacion> estacionesAgregan;

    for (auto estacion : estaciones) {
        int numAgregadas=0;
        for (auto distrito:estacion.cubre) {
            if (!seEncuentraCubierto(distrito,distritosCubiertos)) {
                numAgregadas++;
            }
        }
        if (numAgregadas) {
            estacion.merito=(double)estacion.costo/numAgregadas;
            estacionesAgregan.push_back(estacion);
            //nunca se llega a agregar estaciones ya dispuestas en la solución
            //por que ya su numAgregadas es 0
        }
    }
    return estacionesAgregan;
}

bool compararEstaciones(const Estacion &a, const Estacion &b) {
    return a.merito<b.merito;
}

vector<Estacion> seleccionarSobreUmbral(double umbral,
    const vector<Estacion>& agreganNuevos) {
    vector<Estacion> estaciones;
    for (auto estacion : agreganNuevos) {
        if (estacion.merito<=umbral) {
            estaciones.push_back(estacion);
        }
    }
    return estaciones;
}


void actualizarDistritosCubiertos(int &numDistritos, vector<char> &distritosCubiertos,
    const vector<Estacion> &sobreUmbral,
    int posEstacion) {
    for (auto distrito:sobreUmbral[posEstacion].cubre) {
        if (!seEncuentraCubierto(distrito,distritosCubiertos)) {
            distritosCubiertos.push_back(distrito);
            numDistritos--;
        }
    }
}

vector<Estacion>minimizarCostos(int *costo,int cubre[][NUM_DISTRITOS],
                                   int numDistritos,int iteraciones) {

    vector<Estacion> estaciones=inicializarDistritos(cubre,costo);
    vector<Estacion> mejorSolucion{};
    double menorCosto=INT_MAX;

    while (iteraciones>0) {
        vector<char> distritosCubiertos;//contiene los distritos ya seleccionados
        vector<Estacion> solucionActual{};
        double costoActual=0;
        int numDistritosActual=numDistritos;
        while (numDistritosActual>0) {
            vector<Estacion> agreganNuevos=calcularMerito(estaciones,distritosCubiertos);
            /*
             * en la linea anterior solo se filtran las estaciones que agreguen por lo menos
             * una nueva cobertura más
             */
            if (!agreganNuevos.empty()) { //caso en que ya no se agregan nuevos distritos
                sort(agreganNuevos.begin(),agreganNuevos.end(),compararEstaciones);
                double beta=agreganNuevos.front().merito;
                double tau=agreganNuevos.back().merito;

                double umbral=beta+ALFA*(tau-beta);

                vector<Estacion> sobreUmbral=seleccionarSobreUmbral(umbral,agreganNuevos);
                int posEstacion=rand()%sobreUmbral.size();
                solucionActual.push_back(sobreUmbral[posEstacion]);
                costoActual+=sobreUmbral[posEstacion].costo;
                actualizarDistritosCubiertos(numDistritosActual, distritosCubiertos,
                    sobreUmbral, posEstacion);
            }
            else break;
        }
        if (numDistritosActual==0 and costoActual<menorCosto) {
            menorCosto=costoActual;
            mejorSolucion=solucionActual;
        }
        iteraciones--;
    }
    return mejorSolucion;
}

void imprimirSolucion(const vector<Estacion> &solucion,
    const char *textoExtra) {
    int costo=0;
    cout<<textoExtra<<endl;
    cout<<"Numero"<<
        setw(7)<<"Costo"<<setw(8)<<"Cubre"<<endl;
    for (auto estacion : solucion) {
        cout<<setw(6)<<estacion.numero;
        cout<<setw(6)<<estacion.costo<<"    ";
        for (auto distrito : estacion.cubre) {
            cout<<setw(2)<<distrito;
        }
        cout<<endl;
        costo+=estacion.costo;
    }
    cout<<"Costo total: "<<costo<<endl;
}

int main() {
    srand(time(nullptr));
    int costo[NUM_ESTACIONES] = {8, 4, 5, 3, 9};
    //cubre[e][d] = 1 si la estación e cubre al distrito d (A=0, B=1, ..., F=5)
    int cubre[NUM_ESTACIONES][NUM_DISTRITOS] = {
        //A  B  C  D  E  F
        { 1, 1, 1, 1, 0, 0},  //0 Centro
        { 1, 1, 1, 0, 0, 0},  //1 Norte
        { 0, 0, 0, 1, 1, 1},  //2 Sur
        { 0, 0, 0, 0, 1, 1},  //3 Playa
        { 0, 0, 1, 1, 1, 1}   //4 Industrial
    };

    //TODO: tu solución
    vector<Estacion>solucion=minimizarCostos(costo, cubre, NUM_DISTRITOS,ITERACIONES);

    if (solucion.empty()) {
        cout<<"No se encontró solución que cubra todas los distritos"<<endl;
    }
    else {
        imprimirSolucion(solucion,"La solución con menor costo es:");
    }

    return 0;
}
