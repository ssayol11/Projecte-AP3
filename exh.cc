#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
using namespace std;

int num_pelicules, num_restriccions, num_sales;
map<string, int> pelicules;
map<string, int> sales;
vector<vector<bool>> restriccions;
int dies_minims = 100000000;
int menors_dies;

double now(){
    return clock() / double(CLOCKS_PER_SEC);
}

void llegir(){
    cin >> num_pelicules;
    for (int i = 0; i < num_pelicules; ++i){
        string pelicula;
        cin >> pelicula;
        pelicules.insert({pelicula, i});
    }
    cin >> num_restriccions;
    restriccions = vector<vector<bool>>(num_pelicules, vector<bool>(num_pelicules, false));
    for (int i = 0; i < num_restriccions; ++i){
        pair<string, string> parella;
        cin >> parella.first >> parella.second;
        auto x = *pelicules.find(parella.first);
        auto y = *pelicules.find(parella.second);
        restriccions[x.second][y.second] = true;
        restriccions[y.second][x.second] = true;
    }
    cin >> num_sales;
    for (int i = 0; i < num_sales; ++i){
        string sala;
        cin >> sala;
        sales.insert({sala, i});
    }
    if (num_pelicules % num_sales == 0){
        menors_dies = num_pelicules / num_sales;
    } else {
        menors_dies = (num_pelicules / num_sales);
    }
}

bool compatible(vector<vector<int>>& festival, int dia_actual, int candidat){
    for (int i = 0; i < num_sales; ++i){
        if (festival[dia_actual][i] == -1){
            return true;
        } else {
            if (restriccions[candidat][festival[dia_actual][i]]){
                return false;
            }
        }
    }
    return true;
}

void escriure(const vector<vector<int>>& festival){
    cout << "   d1 d2 d3 d4 d5 d6" << endl;
    for (int i = 0; i < festival[0].size(); ++i){
        cout << 's' << i + 1 << ' ';
        for (int j = 0; j < festival.size(); ++j){
            if (festival[j][i] != -1){
                cout << 0 << festival[j][i] + 1 << ' ';
            } else {
                cout << "-- ";
            }
        }
        cout << endl;
    }
    cout << "--------------" << endl;
}

void organitzar_festival(vector<vector<int>>& festival, vector<bool>& utilitzades, int pelis_utilitzades, int pelis_utilitzades_dia, int dia_actual){
    if (dies_minims == menors_dies){
        return;
    } else if (pelis_utilitzades == num_pelicules){
        if (dia_actual < dies_minims){
            dies_minims = dia_actual;
            cout << dies_minims << endl;
            cout << menors_dies << endl;
            escriure(festival);
        }
    } else if (pelis_utilitzades_dia < num_sales){
        for (int candidat = 0; candidat < num_pelicules; ++candidat){
            if (not utilitzades[candidat]){
                if (compatible(festival, dia_actual, candidat)){
                    festival[dia_actual][pelis_utilitzades_dia] = candidat;
                    utilitzades[candidat] = true;
                    organitzar_festival(festival, utilitzades, pelis_utilitzades + 1, pelis_utilitzades_dia + 1, dia_actual);
                    utilitzades[candidat] = false;
                }
            }
        }
    } else {
        organitzar_festival(festival, utilitzades, pelis_utilitzades, 0, dia_actual + 1);
    }
}

int main(){
    double inici = now();
    llegir();
    vector<bool> utilitzades(num_pelicules, false);
    vector<vector<int>> festival(num_pelicules, vector<int>(num_sales, -1));
    int pelis_utilitzades = 0;
    int pelis_utilitzades_dia = 0;
    int dia_actual = 0;
    organitzar_festival(festival, utilitzades, pelis_utilitzades, pelis_utilitzades_dia, dia_actual);
    double final = now();
    double t = inici - final;
    cout << t << endl;
}
