#include <algorithm>
#include <iomanip>
#include <vector>
#include <iostream>
using namespace std;
#define NUM_OBJETOS 6
#define MAX_PESO 10
#define NUM_INDIVIDUOS 4
#define TASA_MUTACION 0.1

struct Individuo {
    vector<int> objetos;
    int cromosoma[NUM_OBJETOS];
    double fitness;
    double probabilidad;
};

Individuo decodificar(const int *cromosoma) {
    Individuo individuo{};
    for (int i = 0; i < NUM_OBJETOS; i++) {
        individuo.cromosoma[i] = cromosoma[i];
        if (cromosoma[i]) {
            individuo.objetos.push_back(i);
        }
    }
    return individuo;
}

vector<Individuo> decodificarInicial(int poblacionInicial[][NUM_OBJETOS]) {
    vector<Individuo> decodificado{};
    for (int i = 0; i < NUM_INDIVIDUOS; i++) {
        decodificado.push_back(decodificar(poblacionInicial[i]));
    }
    return decodificado;
}

double calcularFitness(vector<Individuo> &poblacion,const int *peso,
    const int *valor) {
    double fitnessTotal = 0;
    for (Individuo &individuo:poblacion) {
        int valorTotal = 0;
        for (const int &objeto:individuo.objetos) {
            valorTotal += valor[objeto];
        }
        individuo.fitness = valorTotal;
        fitnessTotal += individuo.fitness;
    }
    return fitnessTotal;
}

void calcularProbabilidad(vector<Individuo> &poblacion,
    double fitnessTotal) {
    for (Individuo &individuo:poblacion) {
        individuo.probabilidad=individuo.fitness/fitnessTotal;
    }
}

int seleccionarRuleta(const vector<Individuo> &poblacion) {
    int i=-1;
    double posRand=rand()/(double)RAND_MAX;
    double acumulador=0;
    for (const Individuo &individuo:poblacion) {
        acumulador+=individuo.probabilidad;
        i++;
        if (acumulador>=posRand) break;
    }
    return i;//devuelve valido siempre y cuando el vector no esté vacío
}

void cazar(const Individuo &padre1,const Individuo &padre2,
    Individuo &hijo1) {
    for (int i=0;i<NUM_OBJETOS/2;i++) {
        hijo1.cromosoma[i]=padre1.cromosoma[i];
    }
    for (int i=NUM_OBJETOS/2;i<NUM_OBJETOS;i++) {
        hijo1.cromosoma[i]=padre2.cromosoma[i];
    }
}

Individuo cruce(const Individuo &padre1,const Individuo &padre2) {
    Individuo hijo1{};
    cazar(padre1,padre2,hijo1);
    hijo1=decodificar(hijo1.cromosoma);
    return hijo1;
}

void mutacion(Individuo &hijo,const int *peso) {
    //ojo, a este punto no sabemos si el hijo es valido o no
    Individuo copiaHijo=hijo;
    for (int i=0;i<NUM_OBJETOS;i++) {
        double numRand=rand()/(double)RAND_MAX;
        if (numRand<TASA_MUTACION) hijo.cromosoma[i]=!hijo.cromosoma[i];
    }
    //confirmamos si sale algo válido, si es que sí, entonces procedemos con el mutado
    //si es que no, descartamos
    double pesoTotal=0;
    for (int i=0;i<NUM_OBJETOS;i++) {
        if (hijo.cromosoma[i]) {
            pesoTotal+=peso[i];
        }
    }
    hijo=decodificar(hijo.cromosoma);
    if (pesoTotal>MAX_PESO) {
        hijo=copiaHijo;
    }

}

bool iguales(const Individuo &individuo,
    const Individuo &hijo) {
    for (int i=0;i<NUM_OBJETOS;i++) {
        if (individuo.cromosoma[i]!=hijo.cromosoma[i]) return false;
    }
    return true;
}

double calcularPesoTotal(const Individuo &hijo,
    const int *peso) {
    double pesoTotal=0;
    for (int i=0;i<NUM_OBJETOS;i++) {
        if (hijo.cromosoma[i]) {
            pesoTotal+=peso[i];
        }
    }
    return pesoTotal;
}

bool aberracion_clon(const Individuo &hijo,
    const vector<Individuo> &poblacion,const int *peso) {
    //verificamos si es clon de alguno
    for (const Individuo &individuo:poblacion) {
        if (iguales(individuo,hijo)) return true;
    }
    double pesoTotal=calcularPesoTotal(hijo,peso);
    if (pesoTotal>MAX_PESO) return true;
    return false;

}

bool compararPoblaciones(const Individuo &individuo1,const Individuo &individuo2) {
    return individuo1.fitness>individuo2.fitness;
}

void seleccionarIndividuos(vector<Individuo> &poblacion) {
    vector<Individuo> seleccionados{};
    sort(poblacion.begin(),poblacion.end(),compararPoblaciones);
    for (int i=0;i<NUM_INDIVIDUOS;i++) {
        seleccionados.push_back(poblacion[i]);
    }
    poblacion=seleccionados;
}

double calcularValorTotal(const Individuo &individuo,const int *valor) {
    double valorTotal=0;
    for (int i=0;i<NUM_OBJETOS;i++) {
        if (individuo.cromosoma[i]) {
            valorTotal+=valor[i];
        }
    }
    return valorTotal;
}

void algoritmoGenetico(const vector<Individuo> &poblacion_Inicial,
                       const int *peso,const int *valor, int iteraciones) {

    vector<Individuo>poblacion=poblacion_Inicial;

    while (iteraciones){
        double fitnessTotal=calcularFitness(poblacion,peso,valor);

        calcularProbabilidad(poblacion,fitnessTotal);

        Individuo padre1=poblacion[seleccionarRuleta(poblacion)];
        Individuo padre2=poblacion[seleccionarRuleta(poblacion)];

        Individuo hijo1=cruce(padre1,padre2);
        Individuo hijo2=cruce(padre2,padre1);

        mutacion(hijo1,peso);
        mutacion(hijo2,peso);

        if(!aberracion_clon(hijo1,poblacion,peso)) poblacion.push_back(hijo1);
        if(!aberracion_clon(hijo2,poblacion,peso)) poblacion.push_back(hijo2);
        //se re calcula el fitnes para los nuevos hijos
        calcularFitness(poblacion,peso,valor);
        seleccionarIndividuos(poblacion);
        iteraciones--;
    }
    //dado que en seleccionar se ordena la población, entonces solo sacamos el primero
    Individuo mejor=poblacion.front();
    cout<<"Peso total:"<<calcularPesoTotal(mejor,peso)<<endl;
    cout<<"Valor total:"<<calcularValorTotal(mejor,valor)<<endl;
    cout<<"Objetos: ";
    for (int &objeto:mejor.objetos) {
        cout<<objeto<<' ';
    }
    cout<<endl;
}


int main() {
    srand(time(NULL));
    int peso[NUM_OBJETOS]{3,4,2,5,1,6};
    int valor[NUM_OBJETOS]{4,5,3,8,3,7};
    int poblacionInicial[NUM_INDIVIDUOS][NUM_OBJETOS] = {
        {1,0,1,0,0,0},  // A: linterna + cuerda       -> 5 kg, valor 7
        {0,1,0,0,1,0},  // B: botiquin + mapa         -> 5 kg, valor 8
        {0,0,1,0,0,1},  // C: cuerda + cocina         -> 8 kg, valor 10
        {1,0,0,1,1,0}   // D: linterna + carpa + mapa -> 9 kg, valor 15
    };
    vector<Individuo>poblacion_Inicial=decodificarInicial(poblacionInicial);
    algoritmoGenetico(poblacion_Inicial,peso,valor,500);
}