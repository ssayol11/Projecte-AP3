#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<string> mode = { "easy-", "med-", "hard-" };
    for (int o = 0; o < 3; ++o) {
        for (int i = 0; i < 10; ++i) {
            string fitxer_entrada = (mode[o] + to_string(i + 1) + ".inp").c_str();
            string fitxer_sortida = (mode[o] + to_string(i + 1) + ".out").c_str();
            system(("./2 <" + fitxer_entrada + " >" + fitxer_sortida).c_str());
            system(("./checker " + fitxer_entrada + " " + fitxer_sortida).c_str());
        }
    }
}
