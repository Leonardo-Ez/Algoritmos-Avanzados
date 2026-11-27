#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
#define ALFA 0.2
#define ITERACIONES 1000
#define NUM_CAMIONES 3
#define NUM_ZONAS 2

struct Pedido {
    char id;
    int peso;
    int zona;
    int usado;
};

struct Costo {
    int costo1;
    int costo2;
};

struct Camion {
    vector<Pedido> pedidos;
    int capacidad;
    vector<int> costos;
};

struct RCL {
    int camion;
    Pedido pedido;
    int costo;
};

void completarDatosCamiones(vector<Camion> &camiones,
    const vector<Costo> &costos,const int *capacidades) {
    for(int i=0;i<NUM_CAMIONES;i++) {
        Camion camion{};
        camion.capacidad=capacidades[i];
        camion.costos.push_back(costos[i].costo1);
        camion.costos.push_back(costos[i].costo2);
        camiones.push_back(camion);
    }
}

vector<RCL> PrepararRCL(vector<Camion> &solucionActual,
    vector<Pedido> &pedidos,int numPedidos) {
    vector<RCL> rcl{};
    for(int i=0;i<NUM_CAMIONES;i++) {
        for(int j=0;j<numPedidos;j++) {
            if (!pedidos[j].usado and
                solucionActual[i].capacidad>=pedidos[j].peso) {
                RCL posible{};
                posible.pedido=pedidos[j];
                posible.camion=i;
                posible.costo=solucionActual[i].costos[pedidos[j].zona-1];//-1 por que los indices van desde 0
                rcl.push_back(posible);
            }
        }
    }
    return rcl;
}

bool compararDatosRCL(const RCL &elemento1,const RCL &elemento2) {
    return elemento1.costo<elemento2.costo;
}

vector<RCL> tomarSolucionesSobreUmbral(const vector<RCL> &rcl,
    double umbral) {
    vector<RCL> sobreUmbral{};
    for(const RCL &solucion:rcl) {
        if (solucion.costo<=umbral) {
            sobreUmbral.push_back(solucion);
        }
    }
    return sobreUmbral;
}

void marcarUsado(vector<Pedido> &pedidosCopia, vector<RCL> sobreUmbral, int pos) {
    for(Pedido &pedido:pedidosCopia) {
        if (pedido.id==sobreUmbral[pos].pedido.id) {
            pedido.usado=true;
            break;
        }
    }
}

void construirGrasp(int *capacidades,vector<Pedido> &pedidos,
                    vector<Costo> &costos,int numPedidos,int iteraciones) {

    //centralizamos todos los datos en un solo struct
    vector<Camion> camiones{};
    completarDatosCamiones(camiones,costos,capacidades);
    double menorCosto=INT_MAX;
    vector<Camion> mejorSolucion{};
    while (iteraciones) {
        vector<Pedido> pedidosCopia=pedidos;
        vector<Camion> solucionActual=camiones;
        double costoTotal=0;
        int numPedidosCopia=numPedidos;
        while (numPedidosCopia > 0) {
            vector<RCL> rcl=PrepararRCL(solucionActual,pedidosCopia,numPedidos);
            sort(rcl.begin(),rcl.end(),compararDatosRCL);
            double beta=rcl.front().costo;
            double tao=rcl.back().costo;

            double umbral=beta+ALFA*(tao-beta);

            //sobreUmbral:
            vector<RCL> sobreUmbral=tomarSolucionesSobreUmbral(rcl,umbral);
            int pos=rand()%sobreUmbral.size();
            solucionActual[sobreUmbral[pos].camion].pedidos.push_back(sobreUmbral[pos].pedido);
            marcarUsado(pedidosCopia, sobreUmbral, pos);
            costoTotal+=sobreUmbral[pos].costo;
            solucionActual[sobreUmbral[pos].camion].capacidad-=sobreUmbral[pos].pedido.peso;
            numPedidosCopia--;
        }
        if (menorCosto>costoTotal) {
            mejorSolucion=solucionActual;
            menorCosto=costoTotal;
        }
        iteraciones--;
    }
    int i=1;
    for (const auto &camion:mejorSolucion) {
        cout<<"Camion "<<i<<" :";
        for (auto pedido:camion.pedidos) {
            cout<<pedido.id<<' ';
        }
        cout<<" - "<<capacidades[i-1]-camion.capacidad<<"/"<<capacidades[i-1]<<"Kg"<<endl;
        i++;
    }
    cout<<"Costo total: "<<menorCosto<<endl;

}

int main() {
    srand(time(NULL));
    int capacidades[NUM_CAMIONES]{40,35,50};
    vector<Pedido> pedidos{
        {'A',15,1},
        {'B',10,2},
        {'C',20,1},
        {'D',12,2},
        {'E',8,1},
        {'F',18,2},
    };
    vector<Costo> costos{
        {4,9},
        {7,3},
        {5,6},
    };

    construirGrasp(capacidades,pedidos,costos,
        pedidos.size(),10000);

}
