#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <fstream>
#include <cstdlib>
using namespace std;

int num_pelicules, num_restriccions, num_sales;
map<string, int> pelicules_to_int;
map<int, string> int_to_pelicules;
map<string, int> sales_to_int;
map<int, string> int_to_sales;
vector<vector<bool>> restriccions;
int millor_solucio_actual = 1000000;
int solucio_optima;

double now(){
    return clock() / double(CLOCKS_PER_SEC);
}

void llegir(){
    cin >> num_pelicules;
    for (int i = 0; i < num_pelicules; ++i){
        string pelicula;
        cin >> pelicula;
        pelicules_to_int.insert({pelicula, i});
        int_to_pelicules.insert({i, pelicula});
    }
    cin >> num_restriccions;
    restriccions = vector<vector<bool>>(num_pelicules, vector<bool>(num_pelicules, false));
    for (int i = 0; i < num_restriccions; ++i){
        pair<string, string> parella;
        cin >> parella.first >> parella.second;
        auto x = *pelicules_to_int.find(parella.first);
        auto y = *pelicules_to_int.find(parella.second);
        restriccions[x.second][y.second] = true;
        restriccions[y.second][x.second] = true;
    }
    cin >> num_sales;
    for (int i = 0; i < num_sales; ++i){
        string sala;
        cin >> sala;
        sales_to_int.insert({sala, i});
        int_to_sales.insert({i, sala});
    }
    solucio_optima = num_pelicules / num_sales;
}

bool compatible(vector<vector<int>>& festival, int dia_actual, int candidat){
    for (int i = 0; i < num_sales; ++i){
        if (festival[dia_actual][i] == -1) return true;
        else if (restriccions[candidat][festival[dia_actual][i]]) return false;
    }
    return true;
}

void escriure(const vector<vector<int>>& festival, float inici){
    double final = now();
    double t = inici - final;
    cout << t << endl;
    cout << millor_solucio_actual + 1 << endl;
    for (int i = 0; i < festival.size(); ++i){
        for (int j = 0; j < festival[0].size(); ++j){
            if (festival[i][j] != -1){
                auto peli = *int_to_pelicules.find(festival[i][j]);
                auto sala = *int_to_sales.find(j);
                cout << peli.second << ' ' << i + 1 << ' ' << sala.second << endl;
            }
        }
    }
}

void organitzar_festival(vector<vector<int>>& festival, vector<bool>& utilitzades, int pelis_utilitzades, int pelis_utilitzades_dia, int dia_actual, float inici){
    if (pelis_utilitzades == num_pelicules){
        if (dia_actual < millor_solucio_actual){
            millor_solucio_actual = dia_actual;
            escriure(festival, inici);
        }
    } else if (millor_solucio_actual == solucio_optima){
        return;
    } else if (pelis_utilitzades_dia < num_sales){
        for (int candidat = 0; candidat < num_pelicules; ++candidat){
            if (not utilitzades[candidat]){
                if (compatible(festival, dia_actual, candidat)){
                    festival[dia_actual][pelis_utilitzades_dia] = candidat;
                    utilitzades[candidat] = true;
                    organitzar_festival(festival, utilitzades, pelis_utilitzades + 1, pelis_utilitzades_dia + 1, dia_actual, inici);
                    utilitzades[candidat] = false;
                }
            }
        }
    } else {
        organitzar_festival(festival, utilitzades, pelis_utilitzades, 0, dia_actual + 1, inici);
    }
}

int main(){
    cout.setf(ios::fixed);
    cout.precision(1);
    double inici = now();
    llegir();
    vector<bool> utilitzades(num_pelicules, false);
    vector<vector<int>> festival(num_pelicules, vector<int>(num_sales, -1));
    cout << festival.size() << ' ' << festival[0].size() << endl;
    int pelis_utilitzades = 0;
    int pelis_utilitzades_dia = 0;
    int dia_actual = 0;
    organitzar_festival(festival, utilitzades, pelis_utilitzades, pelis_utilitzades_dia, dia_actual, inici);
}
