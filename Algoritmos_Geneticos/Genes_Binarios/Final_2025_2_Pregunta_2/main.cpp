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
#define TMUTACION 0.10

using namespace  std;

struct Caracteristica {
    double pesoPredictivo;
    string descripcion;
};

struct ParRedundante {
    int caracteristica1;
    int caracteristica2;
    double reduccion;
};

int tieneParRedundante(const vector<ParRedundante> &paresRedundantes,
    int posCromo,const vector<int> &cromo) {
    for (int i=0;i<paresRedundantes.size();i++) {
        if (paresRedundantes[i].caracteristica1-1==posCromo
            and cromo[paresRedundantes[i].caracteristica2-1]) {
            return i;
        }
        else if (paresRedundantes[i].caracteristica2-1==posCromo
            and cromo[paresRedundantes[i].caracteristica1-1]) {
            return i;
        }
    }
    return -1;
}

double calcularSumaCaracteristicas(vector<int> cromo,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes) {
    double sumap=0;
    for(int i=0;i<cromo.size();i++) {
        sumap+=cromo[i]*caracteristicas[i].pesoPredictivo;
    }
    //aplicamos las reducciones:
    for (int i=0;i<cromo.size();i++) {
        if (cromo[i]) {
            //aplicamos reducción solo a la posición actual, el otro par
            //redundante aplicará su propia reducción
            int posRedundante=tieneParRedundante(paresRedundantes,i,cromo);
            if (posRedundante!=-1) {
                //le quitamos el desfase que generó al sumar el cromo sin considerar la
                sumap+=((1-paresRedundantes[posRedundante].reduccion)*caracteristicas[i].pesoPredictivo)-
                        caracteristicas[i].pesoPredictivo;
                //reducción
            }
        }
    }
    return sumap;
}

double calculafitness(vector<int>cromo,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes) {
    double sumap=calcularSumaCaracteristicas(cromo,caracteristicas,paresRedundantes);
    int cantidad=0;
    for (int i=0;i<cromo.size();i++) {
        if (cromo[i]) cantidad++;
    }
    if (cantidad==1 or cantidad==0) return 0;
    return sumap/cantidad;
}

bool aberracion(vector<int>ind,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes) {
    return calculafitness(ind,caracteristicas,paresRedundantes)<=0;
}

void muestrapoblacion(vector<vector<int>>poblacion,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes) {
    for(int i=0;i<poblacion.size();i++) {
        for(int j=0;j<poblacion[i].size();j++) {
            cout<<poblacion[i][j]<<" ";
        }
        cout<<"Peso neto: "<<calcularSumaCaracteristicas(poblacion[i],caracteristicas,paresRedundantes)<<" / ";
        cout<<" fo="<< calculafitness(poblacion[i],caracteristicas,paresRedundantes)<<endl;
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
    const vector<ParRedundante> &paresRedundantes) {
    int cont=0;
    srand(time(NULL));
    while (cont<NUMIND) {
        vector<int> individuo;
        // aqui también cambiar
        for (int i=0; i<caracteristicas.size(); i++)
            individuo.push_back(rand()%2);
        if (!aberracion(individuo,caracteristicas,paresRedundantes)) {
            poblacion.push_back(individuo);
            cont++;
        }
    }
}

void calculasupervivencia(vector<vector<int>>poblacion,
    vector<double> &supervivencia,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes) {
    double sumafitness=0;

    for(int i=0;i<poblacion.size();i++)
        sumafitness+=calculafitness(poblacion[i],caracteristicas,paresRedundantes);
    for (int i=0;i<poblacion.size();i++) {
        double superfit;
        superfit=round(100*(double)calculafitness(poblacion[i],caracteristicas,paresRedundantes)/sumafitness);
        supervivencia.push_back(superfit);
    }
}
void cargaruleta(vector<double >supervivencia,int *ruleta) {
    int cont=0;
    for (int i=0;i<supervivencia.size();i++)
        for (int j=0;j<supervivencia[i];j++) {
            ruleta[cont++]=i;
        }
}



void seleccion(vector<vector<int>>poblacion,vector<vector<int>> &padres,
    const vector<Caracteristica> &caracteristicas,
     const vector<ParRedundante> &paresRedundantes) {
    int ruleta[100]{-1};
    vector<double>supervivencia;
    calculasupervivencia(poblacion,supervivencia,caracteristicas,paresRedundantes);
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
    const vector<ParRedundante> &paresRedundantes) {

    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++) {
            if (i!=j) {
                vector<int>hijo;
                creahijo(padres[i],padres[j],hijo);
                if (!aberracion(hijo,caracteristicas,paresRedundantes))
                    poblacion.push_back(hijo);
            }


        }
}


void regenerapoblacion(vector<vector<int>> &poblacion,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes){
    mataclon(poblacion);
    // se evita emplear variables globales
    sort(poblacion.begin(), poblacion.end(),
         [caracteristicas,paresRedundantes](const vector<int>& a, const vector<int>& b) {
            double suma=0, sumb=0;
            for(int i=0;i<a.size();i++)
                suma += calculafitness(a, caracteristicas,paresRedundantes);
            for(int i=0;i<b.size();i++)
                sumb += calculafitness(b, caracteristicas,paresRedundantes);

            return suma > sumb;
         });
    if (poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,poblacion.end());

}

void mutacion(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes) {
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
        if (!aberracion(padres[i],caracteristicas,paresRedundantes))
            poblacion.push_back(padres[i]);
    }
}

// void inversion(vector<vector<int>> &poblacion,vector<vector<int>>padres,
//     const vector<Caracteristica> &caracteristicas,
//     const vector<ParRedundante> &paresRedundantes) {
//
//     for (int i=0;i<padres.size();i++) {
//         for (int j=0;j<padres[i].size();j++) {
//             //cambiar para numeros enteros
//             if (padres[i][j]==0) padres[i][j]=1;
//             else padres[i][j]=0;
//         }
//         if (!aberracion(padres[i],caracteristicas,paresRedundantes))
//             poblacion.push_back(padres[i]);
//     }
// }

double muestramejor(vector<vector<int>> poblacion,
    const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes){
    int mejor=0;
    // puede mejorarse ya que esta muestra mejor lo busca en toda la poblaciÃ³n
    // desde luego se puede asumir que es la posiciÃ³n 0 para nuestro caso
    for(int i=0;i<poblacion.size();i++)
        if(calculafitness(poblacion[mejor],caracteristicas,paresRedundantes)<calculafitness(poblacion[i],caracteristicas,paresRedundantes))
            mejor=i;

    cout << endl<<"La mejor solucion es:" <<
        calculafitness(poblacion[mejor],caracteristicas,paresRedundantes)<<
        " Con peso neto: "<<calcularSumaCaracteristicas(poblacion[mejor],caracteristicas,paresRedundantes)<<endl;
    for(int i=0;i<poblacion[mejor].size();i++)
        cout << poblacion[mejor][i] << "  ";

    cout << endl;
    return calculafitness(poblacion[mejor],caracteristicas,paresRedundantes);
}

void mochilaAG(const vector<Caracteristica> &caracteristicas,
    const vector<ParRedundante> &paresRedundantes) {
    vector<vector<int>> poblacion;
    generapoblacion(poblacion,caracteristicas,paresRedundantes);

    for(int i=0;i<ITERACIONES;i++) {
        vector<vector<int>>padres;
        muestrapoblacion(poblacion,caracteristicas,paresRedundantes);
        cout << endl;
        seleccion(poblacion,padres,caracteristicas,paresRedundantes);
        casamiento(poblacion,padres,caracteristicas,paresRedundantes);
        mataclon(poblacion);
        mutacion((poblacion),padres,caracteristicas,paresRedundantes);
        //inversion(poblacion,padres,caracteristicas,paresRedundantes);

        regenerapoblacion(poblacion,caracteristicas,paresRedundantes);
        muestrapoblacion(poblacion,caracteristicas,paresRedundantes);
        muestramejor(poblacion,caracteristicas,paresRedundantes);

    }
}

int main() {

    vector<Caracteristica> caracteristicas{
        {0.85},
        {0.72},
        {0.91},
        {0.6},
        {0.78},
        {0.45},
        {0.88},
        {0.55},
    };
    vector<ParRedundante> paresRedundantes{
        {1,3,0.30},
        {1,7,0.25},
        {3,7,0.20},
        {2,5,0.35},
        {4,8,0.15},
    };

    mochilaAG(caracteristicas,paresRedundantes);

    return 0;
}