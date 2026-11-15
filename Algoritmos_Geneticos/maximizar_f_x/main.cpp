#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

/*
 * EJERCICIO 01 - Algoritmo Genetico: maximizar f(x) = x^2
 *
 * Autoria:
 *   - Enunciado: redactado por Claude (Anthropic) como ejercicio de practica,
 *     siguiendo el ejemplo del capitulo de Algoritmos Geneticos.
 *   - Implementacion: Hector Leonardo Bonifacio Espinoza :), con partes
 *     implementadas por Claude (Anthropic) con fines academicos a pedido
 *     del usuario (mutar, esAberracion, intentarMutar, obtenerMejor,
 *     imprimirIndividuo y el seguimiento del mejor historico).
 *
 * Enunciado:
 *   Se desea MAXIMIZAR f(x) = x^2 para x en [0, 63], usando un Algoritmo
 *   Genetico con poblacion inicial P0 = {1, 12, 20, 61}.
 *
 *   Etapas implementadas:
 *     1) Codificacion cromosomatica: cada individuo x se representa con un
 *        cromosoma de N_BITS = 6 bits (binario puro, bit mas significativo
 *        en cromosoma[0]). Ej: 2 -> 000010, 63 -> 111111.
 *     2) Evaluacion (fitness): fitness(x) = x^2.
 *     3) Seleccion por ruleta: probabilidad(i) = fitness(i) / suma total.
 *     4) Cruce: tasa de casamiento BITS_CASAMIENTO = 3 genes, el hijo 1
 *        hereda los primeros 3 genes del padre 1 y los ultimos 3 del
 *        padre 2 (y viceversa para el hijo 2).
 *     5) Mutacion: cada GEN (no solo uno fijo) tiene una probabilidad
 *        TASA_MUTACION independiente de voltearse, en cada uno de los 4
 *        individuos resultantes del cruce (2 padres + 2 hijos). Si el
 *        resultado es un clon de algun individuo de la poblacion previa,
 *        o tiene peor fitness que sus dos padres (aberracion), se
 *        descarta el intento completo y se conserva el original.
 *
 *     Nota: la primera version de este ejercicio mutaba solo el gen menos
 *     significativo (fijo). Con la poblacion inicial P0 dada, ningun
 *     individuo tiene un 1 en la posicion 4, y como el cruce solo
 *     recombina bits ya existentes (nunca inventa un 1 nuevo), esa
 *     posicion quedaba congelada en 0 para siempre: el optimo alcanzable
 *     era 61 (111101), no 63 (111111). Mutar CUALQUIER gen con una
 *     probabilidad (en vez de solo uno fijo) es lo que le da al AG la
 *     posibilidad real de alcanzar el optimo global.
 *
 *   Cada generacion reemplaza a la poblacion completa por esos 4
 *   individuos (2 padres ya procesados + 2 hijos ya procesados), y se
 *   repite durante ITERACIONES generaciones, llevando un registro del
 *   mejor individuo visto en toda la corrida (no solo en la ultima
 *   generacion, para no perderlo si no vuelve a ser seleccionado).
 *
 * Antes de programar (a mano):
 *   - Codifica 20 y 61 en binario de 6 bits.
 *   - Calcula el fitness de 1, 12, 20 y 61.
 *   - Calcula la probabilidad de seleccion de cada uno (fitness / suma).
 *   - Si los seleccionados por ruleta son 12 y 61, haz el cruce a mano
 *     con tasa de casamiento = 3. ¿Que hijos obtienes?
 *
 * Comprobacion (valores del ejemplo visto en clase):
 *   - Fitness: 1 -> 1, 12 -> 144, 20 -> 400, 61 -> 3721
 *   - Seleccion: 1 -> 0.0002, 12 -> 0.0338, 20 -> 0.0938, 61 -> 0.8722
 *   - Cruce de 12 (001100) x 61 (111101) con tasa 3 -> hijos 13 (001101)
 *     y 60 (111100)
 *   - Mutacion del bit menos significativo: 12<->13, 60<->61
 *
 * Preguntas:
 *   1. ¿Por que el individuo 61 tiene tanta ventaja en la ruleta frente
 *      a los demas?
 *   2. ¿Que pasaria con la diversidad de la poblacion si nunca se
 *      aplicara mutacion?
 *   3. En este problema en particular, el optimo (63) es el cromosoma
 *      de todo 1s. ¿Por que eso NO es cierto para cualquier AG?
 *   4. Si el problema fuera MINIMIZAR en vez de maximizar, ¿que partes
 *      del codigo cambiarias y cuales no bastaria con solo invertirles
 *      el signo de la comparacion?
 */
#define N_BITS 6
#define NUM_INDIVIDUOS 4
#define TASA_MUTACION 0.1
#define BITS_CASAMIENTO 3

struct Individuo {
    int valor;                  // x, el numero que representa al individuo
    int cromosoma[N_BITS];      // los genes (0/1), uno por bit
    int fitness;                // f(x) = x^2
    double probabilidadSeleccion; // fitness / suma total (para la ruleta)
};

void convertirBinario(const int valorInicial, int *individuo) {
    int copiaValor=valorInicial;
    int i=N_BITS-1;
    while (copiaValor != 0 and i>=0) {
        int bit=copiaValor%2;
        copiaValor/=2;
        individuo[i]=bit;
        i--;
    }
}

void codificar(const int *valoresIniciales,
    vector<Individuo> &individuos) {
    for (int i = 0; i < NUM_INDIVIDUOS; i++) {
        Individuo individuo{};
        individuo.valor = valoresIniciales[i];
        convertirBinario(valoresIniciales[i], individuo.cromosoma);
        individuos.push_back(individuo);
    }
}

int calcularFitness(vector<Individuo> &individuos) {
    int suma=0;
    for (auto &individuo : individuos) {
        individuo.fitness=individuo.valor*individuo.valor;
        suma+=individuo.fitness;
    }
    return suma;
}

void calcularProbabilidad(vector<Individuo> &individuos,const int sumaFitness) {
    for (auto &individuo : individuos) {
        individuo.probabilidadSeleccion=(double)individuo.fitness/sumaFitness;
    }
}

int seleccionarRuleta(const vector<Individuo> &individuos) {
    double posAleatorio=rand()/(double)RAND_MAX;
    double acumulado=0;
    int i=-1;
    for (auto &individuo : individuos) {
        acumulado+=individuo.probabilidadSeleccion;
        i++;
        if (acumulado>=posAleatorio) break;
    }
    return i;
}

void copiarPrimerosBits(Individuo &hijo,const Individuo &padre) {
    for (int i=0;i<BITS_CASAMIENTO;i++) {
        hijo.cromosoma[i]=padre.cromosoma[i];
    }
}

void copiarUltimosBits(Individuo &hijo,const Individuo &padre) {
    for (int i=BITS_CASAMIENTO;i<N_BITS;i++) {
        hijo.cromosoma[i]=padre.cromosoma[i];
    }
}

int decodificarI(const int *cromosoma) {
    int num=0;
    int multiplicar=1<<(N_BITS-1);
    for (int i=0;i<N_BITS;i++) {
        num+=cromosoma[i]*multiplicar;
        multiplicar/=2;
    }
    return num;
}

void decodificar(vector<Individuo> &individuos) {
    for (auto &individuo : individuos) {
        individuo.valor=decodificarI(individuo.cromosoma);
    }
}

vector<Individuo> cruzar(const Individuo &padre1,const Individuo &padre2) {
    vector<Individuo> hijos;
    Individuo hijo1{};
    copiarPrimerosBits(hijo1,padre1);
    copiarUltimosBits(hijo1,padre2);
    Individuo hijo2{};
    copiarPrimerosBits(hijo2,padre2);
    copiarUltimosBits(hijo2,padre1);

    hijos.push_back(hijo1);
    hijos.push_back(hijo2);

    decodificar(hijos);

    calcularFitness(hijos);

    return hijos;
};

void mutar(Individuo &individuo) {
    for (int gen = 0; gen < N_BITS; gen++) {
        double r = rand() / (double)RAND_MAX;
        if (r < TASA_MUTACION) {
            individuo.cromosoma[gen] = !individuo.cromosoma[gen];
        }
    }
    individuo.valor = decodificarI(individuo.cromosoma);
    individuo.fitness = individuo.valor * individuo.valor;
}

bool esAberracion(const Individuo &mutante, const Individuo &padre1,
    const Individuo &padre2, const vector<Individuo> &poblacionActual) {
    if (mutante.fitness < padre1.fitness and mutante.fitness < padre2.fitness) {
        return true;
    }
    for (auto &individuo : poblacionActual) {
        if (individuo.valor == mutante.valor) {
            return true;
        }
    }
    return false;
}

void intentarMutar(Individuo &individuo, const Individuo &padre1,
    const Individuo &padre2, const vector<Individuo> &poblacionActual) {
    Individuo original = individuo;
    mutar(individuo);
    if (esAberracion(individuo, padre1, padre2, poblacionActual)) {
        individuo = original;
    }
}

Individuo obtenerMejor(const vector<Individuo> &individuos) {
    Individuo mejor = individuos[0];
    for (auto &individuo : individuos) {
        if (individuo.fitness > mejor.fitness) {
            mejor = individuo;
        }
    }
    return mejor;
}

void imprimirIndividuo(const Individuo &individuo, const char *titulo) {
    cout << titulo << endl;
    cout << "Valor: " << individuo.valor << "  Cromosoma: ";
    for (int b = 0; b < N_BITS; b++) cout << individuo.cromosoma[b];
    cout << "  Fitness: " << individuo.fitness << endl;
}

Individuo algoritmoGenetico(int *valoresIniciales,int iteraciones) {
    vector<Individuo> individuos;
    //codificamos los individuos y inicializamos el resto de datos en 0
    codificar(valoresIniciales, individuos);

    Individuo mejorHistorico{};
    mejorHistorico.fitness = -1; //asi la primera comparacion siempre actualiza

    while (iteraciones) {
        //ahora comenzamos:
        //calculamos los fitness
        int sumaFitness=calcularFitness(individuos);

        //nos quedamos con el mejor visto hasta ahora, antes de que esta
        //generacion se descarte al reemplazar "individuos" mas abajo
        Individuo mejorActual = obtenerMejor(individuos);
        if (mejorActual.fitness > mejorHistorico.fitness) {
            mejorHistorico = mejorActual;
        }

        //calculamos las probabilidades
        calcularProbabilidad(individuos,sumaFitness);
        //simulamos la ruleta
        int posPadre1=seleccionarRuleta(individuos);
        int posPadre2=seleccionarRuleta(individuos);
        vector<Individuo>hijos=cruzar(individuos[posPadre1],individuos[posPadre2]);

        //poblacion de referencia para detectar clones (antes de mutar a nadie)
        vector<Individuo> poblacionActual = individuos;
        poblacionActual.insert(poblacionActual.end(), hijos.begin(), hijos.end());

        const Individuo padre1 = individuos[posPadre1];
        const Individuo padre2 = individuos[posPadre2];

        //copias independientes: aunque posPadre1 == posPadre2, cada una
        //se muta por su cuenta, sin compartir memoria con la otra
        Individuo hijoPadre1 = padre1;
        Individuo hijoPadre2 = padre2;
        intentarMutar(hijoPadre1, padre1, padre2, poblacionActual);
        intentarMutar(hijoPadre2, padre1, padre2, poblacionActual);
        intentarMutar(hijos[0], padre1, padre2, poblacionActual);
        intentarMutar(hijos[1], padre1, padre2, poblacionActual);
        vector<Individuo> nuevaPoblacion;
        nuevaPoblacion.push_back(hijoPadre1);
        nuevaPoblacion.push_back(hijoPadre2);
        nuevaPoblacion.push_back(hijos[0]);
        nuevaPoblacion.push_back(hijos[1]);

        individuos=nuevaPoblacion;
        iteraciones--;
    }

    return mejorHistorico;
}

int main() {
    srand(time(nullptr));

    int valoresIniciales[NUM_INDIVIDUOS] = {1, 12, 20, 61};
    Individuo mejor = algoritmoGenetico(valoresIniciales,200);
    imprimirIndividuo(mejor, "Mejor individuo encontrado:");

    return 0;
}
