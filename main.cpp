#include <iostream>
#include <sisrec.cpp>
#include <ctime>

using namespace std;

int main()
{
    clock_t t = clock();
    BDLect base("./BD/movielens_l.csv","tupla",0,1,2);
    t=clock()-t;
    printf("Tiempo total de carga: %f segundos",((float)t)/CLOCKS_PER_SEC);

    t=clock();
    cout<<base.get_dist("1","100","manhattan")<<endl;
    t=clock()-t;
    printf("Tiempo total de consulta: %f segundos",((float)t)/CLOCKS_PER_SEC);
    //base.mos_cos_aj();
}
