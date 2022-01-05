#include <ctime>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main()
{
    cout.setf(ios::fixed);
    cout.precision(4);
    double now();

    int num_pel;
    cin >> num_pel;
    vector<string> pel(num_pel);
    map<string, int> pel_num;
    for (int i = 0; i < num_pel; ++i) {
        cin >> pel[i];
        pel_num.insert({ pel[i], i });
    }

    int num_res;
    cin >> num_res;
    pair<string, string> restric;
    vector<vector<int>> res(num_pel);
    for (int i = 0; i < num_res; ++i) {
        cin >> restric.first >> restric.second;
        res[pel_num.find(restric.first)->second].push_back(pel_num.find(restric.second)->second);
        res[pel_num.find(restric.second)->second].push_back(pel_num.find(restric.first)->second);
    }

    int num_sal;
    cin >> num_sal;
    vector<string> sal(num_sal);
    for (int i = 0; i < num_sal; ++i) {
        cin >> sal[i];
    }

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

    cout << clock() / double(CLOCKS_PER_SEC) << endl;
    int num_dies = plan[0].size();
    cout << num_dies << endl;
    for (int o = 0; o < num_dies; ++o) {
        for (int i = 0; i < num_sal; ++i) {
            if (plan[i][o] != -1) {
                cout << pel[plan[i][o]] << ' ' << o + 1 << ' ' << sal[i] << endl;
            }
        }
    }
}
