#include <iomanip>
#include <iostream>
using namespace std;
#define NUM_TAREAS 4

struct Tarea {
    char tarea;
    int tiempo;
    int peso;
    double ratio;
};

void calcularRatios(Tarea *tareas) {
    for(int i=0;i<NUM_TAREAS;i++) {
        tareas[i].ratio=(double)tareas[i].peso/tareas[i].tiempo;
    }

}

int comparar_por_ratio(const void* a, const void* b) {
    const Tarea* ta = (Tarea *)a;
    const Tarea* tb = (Tarea *)b;

    //orden descendente: el mayor ratio va primero

    if (ta->ratio > tb->ratio) return -1;
    if (ta->ratio < tb->ratio) return 1;
    return 0;
}

void imprimirNVecesCar(char car,int num) {
    for(int i=0;i<num;i++) {
        cout<<car;
    }
    cout<<endl;
}

void imprimirResultados(Tarea *tareas) {
    cout<<fixed<<setprecision(2);
    imprimirNVecesCar('=',40);
    cout<<"ORDENAMIENTO FINAL SEGUN REGLA DE SMITH"<<endl;
    imprimirNVecesCar('=',40);
    double costoTotal = 0;
    double cTime=0;
    for(int i=0;i<NUM_TAREAS;i++) {
        cTime+=tareas[i].tiempo;
        double costoP=cTime*tareas[i].peso;
        costoTotal+=costoP;
        cout<<"Tarea:"<<tareas[i].tarea<<endl;
        cout<<"Tiempo de procesamiento:"<<tareas[i].tiempo<<endl;
        cout<<"Peso:"<<tareas[i].peso<<endl;
        cout<<"Ratio w/p:"<<tareas[i].ratio<<endl;
        cout<<"Completion Time:"<<cTime<<endl;
        cout<<"Costo ponderado:"<<costoP<<endl;
        imprimirNVecesCar('-',40);
    }
    cout<<"COSTO TOTAL PONDERADO: "<<costoTotal<<endl;
}


void schedulingTareas(struct Tarea *tareas) {
    /*
     * Calculamos los ratios de las tareas
     * En el problema se menciona que una tarea es rentable si tiene mucho peso
     * y consume poco tiempo
     * entonces su relación w/p debe ser de mayor a menor, por eso
     * calculamos los ratios y luego ordenamos por ese campo para finalmente imprimirlo
     *
     */
    calcularRatios(tareas);

    qsort(tareas,NUM_TAREAS,sizeof(struct Tarea),comparar_por_ratio);

    imprimirResultados(tareas);

}

int main() {

    struct Tarea tareas[NUM_TAREAS] {
        {'A',4,20},
        {'B',2,10},
        {'C',5,15},
        {'D',3,18},
    };

    schedulingTareas(tareas);





    return 0;
}