#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

/*
 * EJERCICIO 01 - GRASP: fase de construcción (mochila del excursionista)
 *
 * Autoría:
 *   - Enunciado e instancia de prueba: redactados por Claude (Anthropic)
 *     como ejercicio de práctica.
 *   - Implementación: Hector Leonardo Bonifacio Espinoza :).
 *
 * Enunciado:
 *   Un excursionista va a acampar y su mochila soporta como máximo 20 kg.
 *   Tiene 8 objetos que podría llevar; cada uno tiene un peso (kg) y una
 *   utilidad (puntos). Cada objeto se lleva entero o no se lleva (no se puede
 *   llevar medio objeto ni dos veces el mismo). Se quiere MAXIMIZAR la
 *   utilidad total sin pasarse de los 20 kg.
 *
 *     #  Objeto              Peso  Utilidad
 *     0  Carpa                10      60
 *     1  Bolsa de dormir       6      30
 *     2  Cocina portátil       5      28
 *     3  Botiquín              4      20
 *     4  Comida                9      40
 *     5  Linterna              3      13
 *     6  Agua                  7      31
 *     7  Cuerda                2       7
 *
 *   Implementa la fase de construcción de GRASP:
 *     - Candidatos: los objetos que aún no están en la mochila y que CABEN
 *       en la capacidad que queda libre.
 *     - Mérito c(x) = utilidad / peso   (se MAXIMIZA: el mejor es el mayor).
 *     - En cada paso:
 *           beta = mayor c(x)      tau = menor c(x)
 *           RCL  = { x : c(x) >= beta - alfa * (beta - tau) }
 *       y se elige UN objeto al azar de la RCL para meterlo en la mochila.
 *     - La construcción termina cuando ya no cabe ningún objeto.
 *
 *   Repite la construcción ITERACIONES veces y guarda la mejor mochila.
 *   Ejecuta para alfa = 0.0, 0.3, 0.5 y 1.0 e imprime, para cada alfa,
 *   los objetos elegidos, el peso usado y la utilidad total.
 *
 * Antes de programar (a mano, con alfa = 0.3):
 *   - Calcula c(x) de los 8 objetos.
 *   - Paso 1: ¿cuánto valen beta, tau y el umbral? ¿Qué objetos forman la RCL?
 *   - Supón que sale la Carpa. Paso 2: ¿cuánta capacidad queda? ¿Quiénes son
 *     ahora los candidatos? Recalcula beta, tau, umbral y RCL.
 *
 * Comprobación:
 *   - Con alfa = 0 (voraz) debe salir una mochila de utilidad 108.
 *   - El óptimo de este problema es 110 (verificado por fuerza bruta).
 *
 * Preguntas:
 *   1. ¿Qué objetos elige el voraz? ¿Por qué no llega a 110?
 *   2. ¿Algún alfa intermedio encuentra el 110? ¿Qué decisión distinta toma?
 *   3. ¿Qué pasa si pones ITERACIONES = 1? ¿Y con alfa = 1?
 */
#define N 8
#define CAPACIDAD 20
#define ITERACIONES 50
#define ALFA 0.3
struct Objeto {
    int peso;
    int valor;
    double relacion;
};


int comparar(const void* a, const void* b) {
    Objeto* obj1 = (Objeto*) a;
    Objeto* obj2 = (Objeto*) b;

    if(obj1->relacion>obj2->relacion) return -1;
    else if(obj1->relacion<obj2->relacion) return 1;
    else return 0;
}

void calcularRelaciones(Objeto *objetos) {
    for(int i=0;i<N;i++) {
        objetos[i].relacion=double(objetos[i].valor)/objetos[i].peso;
    }
}

bool estaEnSolucion(const vector<Objeto> &solucion,Objeto objeto) {
    for(auto & i : solucion) {
        if(i.peso==objeto.peso and
            i.valor==objeto.valor) {
            return true;
        }
    }
    return false;
}

double encontrarMayor(const vector<Objeto> &objetos,const vector<Objeto> &solucion) {
    double mayor=0;
    for(int i=0;i<objetos.size();i++) {
        if(objetos[i].relacion>mayor and
            !estaEnSolucion(solucion,objetos[i])) {
            mayor=objetos[i].relacion;
        }
    }
    return mayor;
}

vector<Objeto> seleccionarObjetosQueCaben(Objeto *objetos,const vector<Objeto> &solucion,
 int capacidad) {
    vector<Objeto> caben;
    for(int i=0;i<N;i++) {
        if(objetos[i].peso<=capacidad and !estaEnSolucion(solucion,objetos[i])) {
            caben.push_back(objetos[i]);
        }
    }
    return caben;
}

double encontrarMenor(const vector<Objeto> &losQueCaben,const vector<Objeto> &solucion) {
    double menor=INT_MAX;
    for (auto objeto : losQueCaben) {
        if(objeto.relacion<menor and !estaEnSolucion(solucion,objeto)) {
            menor=objeto.relacion;
        }
    }
    return menor;
}

vector<Objeto> encontrarObjetosSobreUmbral(const vector<Objeto> &losQueCaben,
    double umbral) {
    vector<Objeto> sobreUmbral;
    for (auto objeto : losQueCaben) {
        if(objeto.relacion>=umbral) {
            sobreUmbral.push_back(objeto);
        }
    }
    return sobreUmbral;
}


vector <Objeto> maximizarUtilidad(Objeto *objetos,
    int capacidad,int iteraciones) {

    calcularRelaciones(objetos);

    qsort(objetos,N,sizeof(Objeto),comparar);

    vector <Objeto> mejorSolucion{};
    int mayorValor=0;

    while (iteraciones>0) {
        vector <Objeto> solucionActual{};
        int capacidadActual=capacidad;
        int valorActual=0;
        while(capacidadActual>0) {
            vector<Objeto>losQueCaben=seleccionarObjetosQueCaben(objetos,solucionActual,capacidadActual);
            if (!losQueCaben.empty()) {
                double beta=losQueCaben.front().relacion;
                double tau=losQueCaben.back().relacion;

                double umbral=beta-ALFA*(beta-tau);

                vector<Objeto> sobreUmbral=encontrarObjetosSobreUmbral(losQueCaben,umbral);

                int posObjeto=rand()%sobreUmbral.size();
                Objeto objeto=sobreUmbral[posObjeto];
                solucionActual.push_back(objeto);
                capacidadActual-=objeto.peso;
                valorActual+=objeto.valor;
            }
            else break;
        }
        if (mayorValor<valorActual) {
            mayorValor=valorActual;
            mejorSolucion=solucionActual;
        }
        iteraciones--;
    }
    return mejorSolucion;
}

void imprimirSolucion(const vector <Objeto> &mejorSolucion,
    const char *textoExtra) {
    cout<<textoExtra<<endl;
    int valor=0;
    cout<<setw(6)<<"Peso"<<setw(8)<<"Valor"<<endl;
    for (auto objeto : mejorSolucion) {
        cout<<setw(6)<<objeto.peso<<' ';
        cout<<setw(6)<<objeto.valor<<endl;
        valor+=objeto.valor;
    }
    cout<<"Con un valor maximo de: "<<valor<<endl;
}



int main() {
    srand(time(nullptr));
    Objeto objetos[N] = {
        {10, 60}, {6, 30}, {5, 28}, {4, 20},
        {9, 40},  {3, 13}, {7, 31}, {2, 7}
    };

    //TODO: tu solución

    vector<Objeto>mejorSolucion=maximizarUtilidad(objetos,CAPACIDAD,ITERACIONES);
    if(mejorSolucion.empty()) {
        cout<<"Solucion nao encontrada."<<endl;
    }
    else {
        imprimirSolucion(mejorSolucion,"La mejor solucion es: ");
    }

    return 0;
}
