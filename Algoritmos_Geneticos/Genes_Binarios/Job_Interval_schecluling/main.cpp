#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
using namespace std;
#define NUM_TAREAS 8
#define TAM_POBLACION 4
#define GENERACIONES 10
#define TASA_SELECCION 0.25
#define PROB_MUTACION 0.10

struct Tarea {
    int id;
    int inicio;
    int fin;
    int beneficio;
};

bool finDentroDeIntervalo(const Tarea &tarea,const Tarea &nuevaTarea) {
    return tarea.inicio<nuevaTarea.fin and
            tarea.fin>nuevaTarea.fin;
}

bool inicioDentroDeIntervalo(const Tarea &tarea,const Tarea &nuevaTarea) {
    return tarea.inicio<nuevaTarea.inicio and
            tarea.fin>nuevaTarea.inicio;
}

bool seSolapan(const vector<Tarea> &tareas,int posTarea,vector<int> individuo) {
    for (int i=0;i<tareas.size();i++) {
        if (individuo[i] and i!=posTarea and
            (inicioDentroDeIntervalo(tareas[i],tareas[posTarea])
            or finDentroDeIntervalo(tareas[i],tareas[posTarea]))) return true;
    }
    return false;
}

bool esAberracion(const vector<Tarea> &tareas,vector<int> &individuo) {
    for (int i=0;i<individuo.size();i++) {
        if (individuo[i] and seSolapan(tareas,i,individuo)) return true;
    }
    return false;
}

void filtrarClon(vector<vector<int>>& poblacion) {
    map<vector<int>, bool> existe;
    for (const auto& individuo : poblacion) {
        existe[individuo] = true;
    }
    poblacion.clear();

    for (const auto& elemento : existe) {
        poblacion.push_back(elemento.first);
    }
}

vector<vector<int>> generarPoblacion(const vector<Tarea> &tareas) {
    vector<vector<int>> poblacion;
    while (poblacion.size() < TAM_POBLACION) {
        vector<int> individuo(tareas.size());
        for (int i = 0; i < tareas.size(); i++) {
            individuo[i] = rand() % 2;
        }
        if (!esAberracion(tareas, individuo)) {
            poblacion.push_back(individuo);
            filtrarClon(poblacion);
        }
    }
    return poblacion;
}

vector<int> generarFitnessAcumulados(const vector<vector<int>> &poblacion,
    const vector<Tarea> &tareas) {
    //para este caso queremos tener la mayor cantidad de beneficios
    vector<int> beneficios;
    for (const auto& individuo : poblacion) {
        int beneficioTotal=0;
        for (int i=0;i<NUM_TAREAS;i++) {
            if (individuo[i]) beneficioTotal+=tareas[i].beneficio;
        }
        beneficios.push_back(beneficioTotal);
    }
    return beneficios;
}
vector<vector<int>> seleccionarPadres(const vector<vector<int>>& poblacion,
                                      const vector<int>& fitnessAcumulados) {
    vector<vector<int>> padres{};
    int cantPadres = (int)round(poblacion.size() * TASA_SELECCION);

    for (int i = 0; i < cantPadres; i++) {
        int ticket = rand() % fitnessAcumulados.back();
        int indice = 0;

        while (ticket >= fitnessAcumulados[indice]) {
            indice++;
        }
        padres.push_back(poblacion[indice]);
    }

    return padres;
}

void mutar(vector<int>& individuo) {
    if (rand() % 100 < PROB_MUTACION * 100) {
        // Seleccionamos un gen al azar
        int indiceGen = rand() % individuo.size();
        individuo[indiceGen] = !individuo[indiceGen];
    }
}

vector<int> generarHijo(const vector<int>& padre,
                        const vector<int>& madre) {
    vector<int> hijo(padre.size());
    for (int i = 0; i < padre.size(); i++) {
        hijo[i] = rand() % 2 ? padre[i] : madre[i];
    }
    return hijo;
}


void casamiento(const vector<Tarea>& tareas,
                vector<vector<int>>& poblacion,
                const vector<vector<int>>& padres) {
    for (int i = 0; i < padres.size(); i++) {
        for (int j = 0; j < padres.size(); j++) {
            if (i != j) {
                vector<int> hijo = generarHijo(padres[i], padres[j]);
                mutar(hijo);
                if (!esAberracion(tareas, hijo)) {
                    poblacion.push_back(hijo);
                }
            }
        }
    }
}


int calculaFitness(const vector<Tarea> &tareas,const vector<int> &a) {
    int fitness = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i]) fitness += tareas[i].beneficio;
    }
    return fitness;
}

void regenerarPoblacion(const vector<Tarea>& tareas,
                        vector<vector<int>>& poblacion) {
    filtrarClon(poblacion);

    sort(poblacion.begin(), poblacion.end(),
         [&tareas](const vector<int>& a, const vector<int>& b) {
             int fitness_a=calculaFitness(tareas,a);
             int fitness_b=calculaFitness(tareas,b);
             return fitness_a > fitness_b;
    });

    if (poblacion.size() > TAM_POBLACION) {
        poblacion.erase(poblacion.begin() + TAM_POBLACION, poblacion.end());
    }
}


void imprimirPortafolio(const vector<Tarea>& tareas,
                        const vector<int>& individuo) {
    //Tarea totales = calcularTotales(tareas, individuo);
    cout << "Mejor cromosoma encontrado: ";
    for (int gen : individuo) cout << gen << " ";
    cout << "\nTareas seleccionadas: ";
    for (int i = 0; i < individuo.size(); i++)
        if (individuo[i]) cout << i + 1 << " ";

    // cout << "\n\nPresupuesto: " << PRESUPUESTO << endl;
    // cout << "Costo actual: " << totales.precioActual << endl;
    // cout << "Valor optimista: " << totales.precioOptimista << endl;
    // cout << "Valor pesimista: " << totales.precioPesimista << endl;
    // cout << "Ganancia potencial: " << totales.precioOptimista - totales.precioActual << endl;
    // cout << "Perdida potencial: " << totales.precioActual - totales.precioPesimista << endl;
    // cout << "Fitness: " << calcularFitness(activos, individuo) << endl;
    // cout << "Presupuesto restante: " << PRESUPUESTO - totales.precioActual << endl;
}

void programarTareas(const vector<Tarea> &tareas) {
    vector<vector<int>>poblacion = generarPoblacion(tareas);
    for (int i = 0; i < GENERACIONES; i++) {
        vector<int> fitnessAcumulados = generarFitnessAcumulados(poblacion,tareas);
        vector<vector<int>> padres = seleccionarPadres(poblacion, fitnessAcumulados);
        casamiento(tareas, poblacion, padres);
        regenerarPoblacion(tareas, poblacion);
    }

    imprimirPortafolio(tareas, poblacion.front());
}

int main() {
    vector<Tarea> tareas = {
        {1,0,3,35},
        {2,1,4,50},
        {3,3,6,40},
        {4,2,5,45},
        {5,5,8,60},
        {6,6,9,70},
        {7,7,10,55},
        {8,9,12,30}
    };

    //maximizar una máquina
    //ejecutar la mayor cantidad de tareas que no se solapen
    //"que los intervalos de tareas no tengan intersección"

    programarTareas(tareas);
}