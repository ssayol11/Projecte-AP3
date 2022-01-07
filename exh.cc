#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

int num_pelicules, num_restriccions, num_sales;
//creem diccionaris que ens permeten permutar entre pelis/sales i els seus numeros corresponents en l'execució del programa
map<string, int> pelicules_to_int;
map<int, string> int_to_pelicules;
map<string, int> sales_to_int;
map<int, string> int_to_sales;
//matriu que ens indica amb un True si entre les pelis i, j hi ha alguna incompatibilitat
vector<vector<bool>> restriccions;
int millor_solucio_actual;
int solucio_optima;
string fitxer_sortida;

//funció que retorna el temps actual (ens servira per calcular el temps d'execucio del programa)
double now()
{
    return clock() / double(CLOCKS_PER_SEC);
}

//inicialitzem el nostre inici de programa
const double inici = now();

//funció per llegir les dades del nostre fitxer de text amb la que inicialitzem totes les variables
void llegir(const string& dades)
{
    ifstream in(dades);
    if (in.fail()) {
        cout << "Error: Any instance has been found" << endl;
        exit(1);
    }
    in >> num_pelicules;
    millor_solucio_actual = num_pelicules; //la millor solució inicialment es la que posa, a cada dia, una pelicula diferent (evitant aixi incompatibilitats)
    for (int i = 0; i < num_pelicules; ++i) {
        string pelicula;
        in >> pelicula;
        pelicules_to_int.insert({ pelicula, i });
        int_to_pelicules.insert({ i, pelicula });
    }
    in >> num_restriccions;
    restriccions = vector<vector<bool>>(num_pelicules, vector<bool>(num_pelicules, false));
    for (int i = 0; i < num_restriccions; ++i) {
        pair<string, string> parella;
        in >> parella.first >> parella.second;
        auto x = *pelicules_to_int.find(parella.first);
        auto y = *pelicules_to_int.find(parella.second);
        restriccions[x.second][y.second] = true;
        restriccions[y.second][x.second] = true;
    }
    in >> num_sales;
    for (int i = 0; i < num_sales; ++i) {
        string sala;
        in >> sala;
        sales_to_int.insert({ sala, i });
        int_to_sales.insert({ i, sala });
    }
    //considerem que la solució optima es aquella on posa a cada dia tantes pelicules com sales hi ha
    solucio_optima = num_pelicules / num_sales;
    if (num_pelicules % num_sales == 0) {
        --solucio_optima;
    }
    in.close();
}

//funció que retorna si una pelicula és compatible en una posició segons les pelicules anteriors que es reprodueixen en aquell dia
bool compatible(vector<vector<int>>& festival, int dia_actual, int candidat)
{
    for (int i = 0; i < num_sales; ++i) {
        if (festival[dia_actual][i] == -1)
            return true;
        else if (restriccions[candidat][festival[dia_actual][i]])
            return false;
    }
    return true;
}

//funció per escriure la nostra sortida en un fitxer de text
void escriure(const vector<vector<int>>& festival, float inici)
{
    ofstream fitxer;
    fitxer.setf(ios::fixed);
    fitxer.precision(1);
    fitxer.open(fitxer_sortida);
    double final = now();
    double t = final - inici;
    fitxer << t << "\n";
    fitxer << millor_solucio_actual + 1 << "\n";
    for (int i = 0; i < festival.size(); ++i) {
        for (int j = 0; j < festival[0].size(); ++j) {
            if (festival[i][j] != -1) {
                auto peli = *int_to_pelicules.find(festival[i][j]);
                auto sala = *int_to_sales.find(j);
                fitxer << peli.second << ' ' << i + 1 << ' ' << sala.second << "\n";
            }
        }
    }
    fitxer.close();
}

//funció recursiva que per a totes les pelis i per a tots els dies va colocant una per una (cada peli) i torna a cridar-se a si mateixa
//quan la solució trobada és igual a la solució optima es para l'execució amb un return
void organitzar_festival(vector<vector<int>>& festival, vector<bool>& utilitzades, int pelis_utilitzades, int pelis_utilitzades_dia, int dia_actual, float inici)
{
    if (pelis_utilitzades == num_pelicules) {
        if (dia_actual < millor_solucio_actual) {
            millor_solucio_actual = dia_actual;
            escriure(festival, inici);
        }
    } else if (millor_solucio_actual == solucio_optima) {
        return;
    } else {
        if (pelis_utilitzades_dia == num_sales) {
            organitzar_festival(festival, utilitzades, pelis_utilitzades, 0, dia_actual + 1, inici);
        } else {
            bool possible = false;
            for (int candidat = 0; candidat < num_pelicules; ++candidat) {
                if (not utilitzades[candidat]) {
                    if (compatible(festival, dia_actual, candidat)) {
                        possible = true;
                        festival[dia_actual][pelis_utilitzades_dia] = candidat;
                        utilitzades[candidat] = true;
                        organitzar_festival(festival, utilitzades, pelis_utilitzades + 1, pelis_utilitzades_dia + 1, dia_actual, inici);
                        utilitzades[candidat] = false;
                    }
                }
            }
            if (pelis_utilitzades_dia != 0) {
                organitzar_festival(festival, utilitzades, pelis_utilitzades, 0, dia_actual + 1, inici);
            }
        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "Syntax: "
             << "exh.exe "
             << "instance.inp "
             << "solution.txt" << endl;
        exit(1);
    }
    llegir(argv[1]);
    fitxer_sortida = argv[2];
    vector<bool> utilitzades(num_pelicules, false);
    vector<vector<int>> festival(num_pelicules, vector<int>(num_sales, -1));
    int pelis_utilitzades = 0;
    int pelis_utilitzades_dia = 0;
    int dia_actual = 0;
    organitzar_festival(festival, utilitzades, pelis_utilitzades, pelis_utilitzades_dia, dia_actual, inici);
}
