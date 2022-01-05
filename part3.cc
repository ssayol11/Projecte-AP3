#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int num_pel;
vector<string> pel;
map<string, int> pel_num;
vector<vector<int>> res;
int num_sal;
vector<string> sal;
string nom_fitxer;
vector<vector<int>> plan;

void read(const string& data)
{
    ifstream in(data);
    in >> num_pel;
    pel = vector<string>(num_pel);
    for (int i = 0; i < num_pel; ++i) {
        in >> pel[i];
        pel_num.insert({ pel[i], i });
    }

    int num_res;
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
    in.close();
}

void escriure_sol()
{
    ofstream myfile;
    myfile.setf(ios::fixed);
    myfile.precision(1);
    myfile.open(nom_fitxer);
    myfile << clock() / double(CLOCKS_PER_SEC) << "\n";
    int num_dies = plan[0].size();
    myfile << num_dies << "\n";
    for (int o = 0; o < num_dies; ++o) {
        for (int i = 0; i < num_sal; ++i) {
            if (plan[i][o] != -1) {
                myfile << pel[plan[i][o]] << ' ' << o + 1 << ' ' << sal[i] << "\n";
            }
        }
    }
    myfile.close();
}

int dies_gready()
{
    plan = vector<vector<int>>(num_sal);
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
    escriure_sol();
    return plan[0].size();
}

int main(int argc, char** argv)
{
    double now();
    read(argv[1]);
    nom_fitxer = argv[2];
    int dies_best_solucio = dies_gready();
}
