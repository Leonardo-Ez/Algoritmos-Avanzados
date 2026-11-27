#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include <algorithm>
#define ITERACIONES 10
#define NUMIND 15
#define TSELECCION 0.5
#define PCASAMIENTO 0.5
#define TMUTACION 0.1

using namespace  std;


struct Caracteristica {
    int indice;
    double pesoPredictivo;
    string descripcion;
};

struct ParRedundante {
    int indice1;
    int indice2;
    double reduccion;
};

double sumasRedundantes(const vector<int> &cromo,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias,
    vector<bool> &reducidos) {
    double sumasRedundantes=0;
    for (int pos=0;pos<cromo.size();pos++) {
        for (int i=0;i<redundancias.size();i++) {
            if (redundancias[i].indice1==pos+1
                and cromo[redundancias[i].indice2-1]
                and !reducidos[redundancias[i].indice1-1] and !reducidos[redundancias[i].indice2-1] ) {
                //caso si es que es el indice 1 y no su usó ni a su par
                double sumaBase=caracteristicas[redundancias[i].indice1-1].pesoPredictivo+caracteristicas[redundancias[i].indice2-1].pesoPredictivo;
                sumasRedundantes+= sumaBase*(1-redundancias[i].reduccion)-sumaBase;
                reducidos[redundancias[i].indice1-1]=true;
                reducidos[redundancias[i].indice2-1]=true;

                }
            else if (redundancias[i].indice2==pos+1
                and cromo[redundancias[i].indice1-1]
                and !reducidos[redundancias[i].indice2-1] and !reducidos[redundancias[i].indice1-1]) {
                //caso en el que es el indice 2
                double sumaBase=caracteristicas[redundancias[i].indice2-1].pesoPredictivo+caracteristicas[redundancias[i].indice1-1].pesoPredictivo;
                sumasRedundantes+= sumaBase*(1-redundancias[i].reduccion)-sumaBase;
                    reducidos[redundancias[i].indice1-1]=true;
                    reducidos[redundancias[i].indice2-1]=true;
                }
        }
    }

    return sumasRedundantes;
}


double calculafitness(const vector<int> &cromo,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias) {
    double sumap=0;
    int cantidad=0;
    vector<bool> reducidos;
    for (int i=0;i<cromo.size();i++) {
        reducidos.push_back(false);
    }

    for (int i=0;i<cromo.size();i++) {
        sumap+=cromo[i]*caracteristicas[i].pesoPredictivo;
        cantidad+=cromo[i];
    }
    sumap+=sumasRedundantes(cromo,caracteristicas,redundancias,reducidos);
    if (cantidad==0) return 0;
    return  sumap/cantidad;
}

bool aberracion(vector<int>ind,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias
    ) {
    return calculafitness(ind,caracteristicas,redundancias)<=0;
}

void muestrapoblacion(vector<vector<int>>poblacion,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias) {
    for(int i=0;i<poblacion.size();i++) {
        for(int j=0;j<poblacion[i].size();j++) {
            cout<<poblacion[i][j]<<" ";
        }
        cout<<" fo="<< calculafitness(poblacion[i],caracteristicas,redundancias)<<endl;
    }

}

int decimal(vector<int>ind) {
    int num=0;
    for (int i=0;i<ind.size();i++)
        num+=ind[i]*pow(2,i);
    return num;
}

void mataclon(vector<vector<int>> &poblacion) {
    map<int,vector<int>> indunicos;

    for(int i=0;i<poblacion.size();i++) {
        int dec=decimal(poblacion[i]);
        indunicos[dec]=poblacion[i];
    }
    poblacion.clear();
    for(map<int,vector<int>>::iterator it=indunicos.begin();
        it!=indunicos.end();it++) {
        poblacion.push_back(it->second);
    }
}


void generapoblacion(vector<vector<int>> &poblacion,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &incompatibilidades) {
    int cont=0;
    srand(time(NULL));
    while (cont<NUMIND) {
        vector<int> individuo;
        // aqui también cambiar
        for (int i=0; i<caracteristicas.size(); i++)
            individuo.push_back(rand()%2);
        if (!aberracion(individuo,caracteristicas,incompatibilidades)) {
            poblacion.push_back(individuo);
            cont++;
        }
    }
}

void calculasupervivencia(vector<vector<int>>poblacion,
    vector<double> &supervivencia,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias) {
    double sumafitness=0;

    for(int i=0;i<poblacion.size();i++)
        sumafitness+=calculafitness(poblacion[i],caracteristicas,redundancias);
    for (int i=0;i<poblacion.size();i++) {
        double superfit;
        superfit=round(100*(double)calculafitness(poblacion[i],caracteristicas,redundancias)/sumafitness);
        supervivencia.push_back(superfit);
    }
}
void cargaruleta(vector<double>supervivencia,int *ruleta) {
    int cont=0;
    for (int i=0;i<supervivencia.size();i++)
        for (int j=0;j<supervivencia[i];j++) {
            ruleta[cont++]=i;
        }
}



void seleccion(vector<vector<int>>poblacion,
    vector<vector<int>> &padres,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias ) {
    int ruleta[100]{-1};
    vector<double>supervivencia;
    calculasupervivencia(poblacion,supervivencia,caracteristicas,redundancias);
    cargaruleta(supervivencia,ruleta);
    int npadres=round(poblacion.size()*TSELECCION);

    for (int i=0;i<npadres;i++) {
        int ticket=rand()%100;
        if (ruleta[ticket]!=-1)
            padres.push_back(poblacion[ruleta[ticket]]);
    }
    //muestrapoblacion(padres,paq);
}

void creahijo(vector<int>padre,vector<int>madre,
    vector<int>&hijo) {
    int posi=round(padre.size()*PCASAMIENTO);

    for (int i=0;i<posi;i++)
        hijo.push_back(padre[i]);
    for (int i=posi;i<madre.size();i++)
        hijo.push_back(madre[i]);
}

void casamiento(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias) {

    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++) {
            if (i!=j) {
                vector<int>hijo;
                creahijo(padres[i],padres[j],hijo);
                if (!aberracion(hijo,caracteristicas,redundancias))
                    poblacion.push_back(hijo);
            }


        }
}


void regenerapoblacion(vector<vector<int>> &poblacion,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias){
    mataclon(poblacion);
    // se evita emplear variables globales
    sort(poblacion.begin(), poblacion.end(),
         [caracteristicas,redundancias](const vector<int>& a, const vector<int>& b) {
            double suma=0, sumb=0;
            for(int i=0;i<a.size();i++)
                suma += calculafitness(a,caracteristicas,redundancias);
            for(int i=0;i<b.size();i++)
                sumb += calculafitness(b, caracteristicas,redundancias);

            return suma > sumb;
         });
    if (poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,poblacion.end());

}

void mutacion(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias) {
    int nmutaciones=round(padres[0].size()*TMUTACION);
    for (int i=0;i<padres.size();i++) {
        int cont=0;
        while (cont<nmutaciones) {
            int gen=rand()%padres[i].size();
            //aqui hay que cambiar si son numeros enteros
            if (padres[i][gen]==0) padres[i][gen]=1;
            else padres[i][gen]=0;
            cont++;
        }
        if (!aberracion(padres[i],caracteristicas,redundancias))
            poblacion.push_back(padres[i]);
    }
}


double muestramejor(vector<vector<int>> poblacion,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias){
    int mejor=0;
    // puede mejorarse ya que esta muestra mejor lo busca en toda la poblaciÃ³n
    // desde luego se puede asumir que es la posiciÃ³n 0 para nuestro caso
    for(int i=0;i<poblacion.size();i++)
        if(calculafitness(poblacion[mejor],caracteristicas,redundancias)<calculafitness(poblacion[i],caracteristicas,redundancias))
            mejor=i;

    cout << endl<<"La mejor solucion es:" << calculafitness(poblacion[mejor],caracteristicas,redundancias)<<endl;
    for(int i=0;i<poblacion[mejor].size();i++)
        cout << poblacion[mejor][i] << "  ";

    cout << endl;
    return calculafitness(poblacion[mejor],caracteristicas,redundancias);
}

void mochilaAG(const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &redundancias) {
    vector<vector<int>> poblacion;
    generapoblacion(poblacion,caracteristicas,redundancias);

    for(int i=0;i<ITERACIONES;i++) {
        vector<vector<int>>padres;
        muestrapoblacion(poblacion,caracteristicas,redundancias);
        cout << endl;
        seleccion(poblacion,padres,caracteristicas,redundancias);
        casamiento(poblacion,padres,caracteristicas,redundancias);
        mataclon(poblacion);
        mutacion((poblacion),padres,caracteristicas,redundancias);

        regenerapoblacion(poblacion,caracteristicas,redundancias);
        muestrapoblacion(poblacion,caracteristicas,redundancias);
        muestramejor(poblacion,caracteristicas,redundancias);
    }
}

int main() {
    vector<Caracteristica> caracteristicas{
        {1,0.85,"Edad del paciente"},
        {2,0.72,"Indice de masa corporal"},
        {3,0.91,"Nivel de glucosa"},
        {4,0.60,"Presion sistólica"},
        {5,0.78,"Colesterol total"},
        {6,0.45,"Horas de ejercicio"},
        {7,0.88,"Antecedentes familiares"},
        {8,0.55,"Horas de sueño"},
    };
    vector<ParRedundante> incompatibilidades{
        {1,3,0.30},
        {1,7,0.25},
        {3,7,0.20},
        {2,5,0.35},
        {4,8,0.15},
    };

    mochilaAG(caracteristicas,incompatibilidades);

    return 0;
}