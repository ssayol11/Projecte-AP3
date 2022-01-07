#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

string fitxer_sortida;
int num_pel;
int num_res;
int num_sal;
vector<string> pel;
map<string, int> pel_num;
vector<vector<int>> res;
vector<string> sal;

double now()
{
    return clock() / double(CLOCKS_PER_SEC);
}

void llegir(const string& dades)
{
    ifstream in(dades);
    if (in.fail()) {
        cout << "Error: Any instance has been found" << endl;
        exit(1);
    }
    in >> num_pel;
    pel = vector<string>(num_pel);
    map<string, int> pel_num;
    for (int i = 0; i < num_pel; ++i) {
        in >> pel[i];
        pel_num.insert({ pel[i], i });
    }
    in >> num_res;
    pair<string, string> restric;
    res = vector<vector<int>>(num_pel);
    for (int i = 0; i < num_res; ++i) {
        in >> restric.first >> restric.second;
        res[pel_num.find(restric.first)->second].push_back(pel_num.find(restric.second)->second);
        res[pel_num.find(restric.second)->second].push_back(pel_num.find(restric.first)->second);
    }
    in >> num_sal;
    sal = vector<string>(num_sal);
    for (int i = 0; i < num_sal; ++i) {
        in >> sal[i];
    }
}

int main(int argc, char** argv)
{
    double inici = now();
    if (argc != 3) {
        cout << "Syntax: "
             << "exh.exe "
             << "instance.inp "
             << "solution.txt" << endl;
        exit(1);
    }
    llegir(argv[1]);
    vector<vector<int>> plan(num_sal);
    int num_rep = 0;
    vector<bool> rep(num_pel, true);
    while (num_rep != num_pel) {
        int i = 0;
        int num_pel_dia = 0;
        vector<bool> rep_dia = rep;
        while (not rep_dia[i]) {
            ++i;
        }
        while ((num_pel_dia < num_sal) and (i < num_pel)) {
            plan[num_pel_dia].push_back(i);
            ++num_pel_dia;
            rep[i] = false;
            rep_dia[i] = false;
            ++num_rep;
            for (int o = 0; o < res[i].size(); ++o) {
                rep_dia[res[i][o]] = false;
            }
            while (i < num_pel and not rep_dia[i]) {
                ++i;
            }
        }
        while (num_pel_dia != num_sal) {
            plan[num_pel_dia].push_back(-1);
            ++num_pel_dia;
        }
    }

    double final = now();
    fitxer_sortida = argv[2];
    ofstream fitxer;
    fitxer.setf(ios::fixed);
    fitxer.precision(1);
    fitxer.open(fitxer_sortida);
    fitxer << final - inici << "\n";
    int num_dies = plan[0].size();
    fitxer << num_dies << "\n";
    for (int o = 0; o < num_dies; ++o) {
        for (int i = 0; i < num_sal; ++i) {
            if (plan[i][o] != -1) {
                fitxer << pel[plan[i][o]] << ' ' << o + 1 << ' ' << sal[i] << "\n";
            }
        }
    }
    fitxer.close();
}
