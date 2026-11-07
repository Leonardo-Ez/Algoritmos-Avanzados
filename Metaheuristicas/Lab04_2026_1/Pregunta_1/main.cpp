#include <algorithm>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

/*
 * LABORATORIO 4 (2026-1) - Pregunta 1
 * Algoritmos Avanzados - PUCP
 *
 * Autoría:
 *   - Enunciado: Laboratorio 4 del curso (2026-1).
 *   - Implementación: Hector Leonardo Bonifacio Espinoza :).
 *
 * Problema: selección de proyectos (TUPIA CONSTRUYE S.A.) con presupuesto
 * P = 400 y predecesores; se busca maximizar la ganancia total.
 *
 * Estrategia: GRASP construcción (alfa = 0.3, 1000 iteraciones).
 *   - Candidatos: proyectos no elegidos, que caben en el presupuesto y que ya
 *     tienen todos sus predecesores elegidos (tip del enunciado). Se arranca
 *     con los independientes y los dependientes entran al desbloquearse.
 *   - Mérito (se maximiza):
 *       c(x) = (ganancia de x + ganancia de los proyectos que desbloquea) / costo de x
 *     Con el mérito simple ganancia/costo, alfa = 0.3 siempre da 4 5 7 1 2 = 1350,
 *     porque el proyecto 1 vale poco solo, aunque desbloquea al 2 y al 3.
 *
 * Resultado: 4 1 2 6 5 = 1900, igual al ejemplo del enunciado.
 * Es el óptimo global (verificado por fuerza bruta: {1, 2, 4, 5, 6}, costo 400).
 */
#define N 7
#define P 400
#define ALFA 0.3


struct Proyecto {
    int id;
    double costo;
    double ganancia;
    vector<int> predecesores;
    double beneficio;
};

bool compararProyectos(Proyecto &a, Proyecto &b) {
    return a.beneficio > b.beneficio;
}



vector<Proyecto> elegirProyectosDependientes(const vector<Proyecto> &proyectos) {
    vector<Proyecto> dependientes;
    for (const auto &proyecto : proyectos) {
        if (!proyecto.predecesores.empty()) {
            dependientes.push_back(proyecto);
        }
    }
    return dependientes;
}

vector<Proyecto> elegirProyectosIndependientes(const vector<Proyecto> &proyectos) {
    vector<Proyecto> independientes;
    for (const auto &proyecto : proyectos) {
        if (proyecto.predecesores.empty()) {
            independientes.push_back(proyecto);
        }
    }
    return independientes;
}

void calcularBeneficio(vector<Proyecto> &proyectos) {
    /*
     * el beneficio de un proyecto considera su ganancia más la ganancia de los
     * proyectos que desbloquea (los que lo tienen como predecesor), dividido
     * entre su costo; así un proyecto que sirve de base para otros buenos
     * no queda relegado (con esto se reproduce el ejemplo: 4 1 2 6 5 = 1900)
     */
    //fue la única forma que diera el resultado del enunciado, de otro modo, el
    //1 no entra en la rcl y la solución sale otra
    for (auto &proyecto : proyectos) {
        double gananciaTotal=proyecto.ganancia;
        for (auto &otro : proyectos) {
            for (auto &predecesor : otro.predecesores) {
                if (predecesor==proyecto.id) {
                    gananciaTotal+=otro.ganancia;
                    break;
                }
            }
        }
        proyecto.beneficio=gananciaTotal/proyecto.costo;
    }
}


vector<Proyecto> elegirProyectosUmbral(const vector<Proyecto> &proyectosAEvaluar,
    double umbral) {
    vector<Proyecto> proyectosSeleccionados;
    for (auto &proyecto : proyectosAEvaluar) {
        if (proyecto.beneficio>=umbral) {
            proyectosSeleccionados.push_back(proyecto);
        }
    }
    return proyectosSeleccionados;
}

bool sePuedeAgregar(Proyecto dependiente,
    vector<Proyecto> &proyectosAEvaluar) {
    bool resultado = false;
    for (auto & id: dependiente.predecesores) {
        for (auto & proyecto : proyectosAEvaluar) {
            if (proyecto.id == id) {
                resultado = true;
                break;
            }
        }
        if (!resultado)return false;
        resultado=false;
    }
    /*
     * la única forma en que el resultado sea true es que todos los id de los que depende
     * se encuentren en los que se van a evaluar
     */
    return true;
}

bool seEncuentraEnSolucion(vector<Proyecto> &solucionActual,
    Proyecto &proyecto) {
    for (auto &solucion : solucionActual) {
        if (proyecto.id==solucion.id) return true;
    }
    return false;
}

void agregarDependientes(vector<Proyecto> &dependientes,
    vector<Proyecto> &solucionActual,vector<Proyecto> &proyectosAEvaluar,
    double presupuestoActual) {

    vector<Proyecto> proyectosRestantes;
    //quitamos de proyectos a evaluar todos los que tengan un coste superior al presupuesto

    for (auto &proyecto : proyectosAEvaluar) {
        if (!seEncuentraEnSolucion(solucionActual,proyecto) and proyecto.costo<=presupuestoActual) {
            proyectosRestantes.push_back(proyecto);
        }
    }

    bool ordenar=false;
    //ahora agregamos los proyectos que ya se pueden programar por su dependencia
    for (auto &proyecto :dependientes) {
        if (!seEncuentraEnSolucion(solucionActual,proyecto) and proyecto.costo<=presupuestoActual
            and sePuedeAgregar(proyecto,solucionActual)) {
            proyectosRestantes.push_back(proyecto);
            ordenar=true;
            }
    }
    if (ordenar) {
        sort(proyectosRestantes.begin(),proyectosRestantes.end(),compararProyectos);
    }
    proyectosAEvaluar=proyectosRestantes;
}


void obtenerMaxBeneficio(vector<Proyecto> proyectos,
                         int iteraciones=10) {

    //para esta sección vamos a separar los proyectos
    //en 2 vectores, uno de independientes y otro de dependientes
    calcularBeneficio(proyectos);
    sort(proyectos.begin(), proyectos.end(),compararProyectos);

    double mayorGanancia=0;
    vector<Proyecto> mejorSolucion;
    while (iteraciones) {

        vector<Proyecto> dependientes=elegirProyectosDependientes(proyectos);
        vector<Proyecto> proyectosAEvaluar=elegirProyectosIndependientes(proyectos);

        /*dado que tenemos separado los independientes, comenzaremos con los independientes
            para este punto ambos están ordenados así que podemos lanzar la selección
           como condición de parada usaremos cuando ya no queden proyectos a evaluar
        */
        double presupuestoActual=P;
        double gananciaActual=0;
        vector<Proyecto> solucionActual;
        while (!proyectosAEvaluar.empty()) {
            double beta=proyectosAEvaluar.front().beneficio;
            double tau=proyectosAEvaluar.back().beneficio;

            double umbral=beta-ALFA*(beta-tau);

            vector<Proyecto> sobreUmbral=elegirProyectosUmbral(proyectosAEvaluar,umbral);

            int pos=rand()%sobreUmbral.size();

            //este if solo servirá para la primera iteración
            if (sobreUmbral[pos].costo<=presupuestoActual) {
                presupuestoActual-=sobreUmbral[pos].costo;
                gananciaActual+=sobreUmbral[pos].ganancia;
                solucionActual.push_back(sobreUmbral[pos]);
                //a este punto ya solo tengo los que puedo agregar si o si

                agregarDependientes(dependientes,solucionActual,proyectosAEvaluar,
                    presupuestoActual);
            }

        }

        if (mayorGanancia<gananciaActual) {
            mayorGanancia=gananciaActual;
            mejorSolucion=solucionActual;
        }
        iteraciones--;
    }


    for (auto &proyecto : mejorSolucion) {
        cout<<proyecto.id<<' ';
    }
    cout<<endl;
    cout<<mayorGanancia<<endl;

}


int main() {
    //inicializamos el rand
    srand(time(NULL));
    vector<Proyecto> proyectos{
        {1,100,200},
        {2,50,300, {1}},
        {3,150,300,{1}},
        {4,50,400},
        {5,50,200,{4}},
        {6,150,800,{2,4}},
        {7,100,250}
    };

    obtenerMaxBeneficio(proyectos,1000);






    return 0;
}