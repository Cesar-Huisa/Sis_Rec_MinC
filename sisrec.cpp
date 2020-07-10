#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <map>
#include <algorithm>

using namespace std;

template <typename T>
float minimos_cuadrados(vector<T> x,vector<T> y,int n) {
    float k=0,tk=0,w=0,k2=0,m;
    for(int i=0;i<n;i++,x++,y++) {
        k+=x[i];
        tk+=y[i];
        w+= (x[i])*(y[i]);
        k2+=(x[i])*(x[i]);
    }
    m=(n*w-k*tk)/(n*k2-k*k);
    return m;
}

class Rect
{
private:
    unsigned int limit;
    unsigned short grado;
    vector<float> caract;
    float a;
    float b;
    float m;
public:
    Rect() {
    }

    Rect (map<int,float> mapa) {
        get_rect(mapa,6);
    }

    vector<float> get_rect(map<int,float> mapa,unsigned short grado) {
        int i,j,k,n,N;

        N=mapa.size();
        n=grado;
        double x[N],y[N];

        map<int, float>::iterator itr;
        i=0;
        for (itr = mapa.begin(); itr != mapa.end(); ++itr) {
            x[i]=itr->first;
            y[i]=itr->second;
            i++;
        }

        double X[2*n+1];
        for (i=0;i<2*n+1;i++)
        {
            X[i]=0;
            for (j=0;j<N;j++)
                X[i]=X[i]+pow(x[j],i);
        }
        double B[n+1][n+2],a[n+1];
        for (i=0;i<=n;i++)
            for (j=0;j<=n;j++)
                B[i][j]=X[i+j];
        double Y[n+1];
        for (i=0;i<n+1;i++)
        {
            Y[i]=0;
            for (j=0;j<N;j++)
            Y[i]=Y[i]+pow(x[j],i)*y[j];
        }
        for (i=0;i<=n;i++)
            B[i][n+1]=Y[i];
        n=n+1;

        for (i=0;i<n;i++)
            for (k=i+1;k<n;k++)
                if (B[i][i]<B[k][i])
                    for (j=0;j<=n;j++)
                    {
                        double temp=B[i][j];
                        B[i][j]=B[k][j];
                        B[k][j]=temp;
                    }

        for (i=0;i<n-1;i++)
            for (k=i+1;k<n;k++)
                {
                    double t=B[k][i]/B[i][i];
                    for (j=0;j<=n;j++)
                        B[k][j]=B[k][j]-t*B[i][j];
                }
        for (i=n-1;i>=0;i--)
        {
            a[i]=B[i][n];
            for (j=0;j<n;j++)
                if (j!=i)
                    a[i]=a[i]-B[i][j]*a[j];
            a[i]=a[i]/B[i][i];
        }
        vector<float> o;

        for(i=0;i<n+1;i++) {
            o.push_back((float)a[i]);
        }
        return o;
    }


    float get_y(int x) {
        float y=0;
        for(int i=0;i<caract.size();i++) {
            y+=(caract[i])*pow(x,i);
        }
        return y;
    }

    void add_point(int x,float y,float pres) {
        float salt=100*pres;
        map<int,float> neww;
        for(int i=0;i<limit;i+=salt) {
            neww[i]=get_y(i);
        }


    }
    string get_str() {
        string o="";
        for(int j=0;j<caract.size();j++) {
            o+=to_string(caract[j]);
            o+=',';
        }
        o.erase(o.end()-1,o.end());
        return o;
    }
    ~Rect() {
        caract.clear();
        limit=0;
        grado=0;
    }
};

class Record
{
private:
    unsigned int limit;
    bool state;
    unsigned int len_rect;
    string user;
    unsigned int n;

    map<int,float> vals;
    Rect recta;

public:
    Record() {
    }
    Record(string usernam) {
        user=usernam;
        state=false;
        n=0;
    }

    Record(string usernam, unsigned int lim) {
        user=usernam;
        state=false;
        limit=lim;
        n=0;
    }

    Record(string usern,map<int, float> maps) {
        user=usern;
        map<int, float>::iterator itr;
        for (itr = maps.begin(); itr != maps.end(); ++itr) {
            add_point(itr->first,itr->second);
        }
    }

    void transfor_rect() {
        state=true;
        recta=Rect(vals);
        vals.clear();
    }

    void change_limit(unsigned int ne) {
        limit=ne;
        if(n>limit) {
            transfor_rect();
        }
    }

    string get_usr() {
        return user;
    }

    void add_point(int x,float y) {
        if(!state) {
            vals[x]=y;
            n++;
            if (n>limit) {
                transfor_rect();
            }
        }
        else {
            recta.add_point(x,y,0.5);
        }
    }

    float get_val(int x) {
        if(!state) {

            return vals[x];
        }
        else {
            return recta.get_y(x);
        }

    }

    string get_str() {
        string out="";
        out+=(user+',');
        if(state) {
            out+="T,";
            out+=recta.get_str();
            out+="\n";
        }
        else {
            out+="F,";
            map<int, float>::iterator itr;
            for (itr = vals.begin(); itr != vals.end(); ++itr) {
                out+=to_string(itr->first);
                out+=";";
                out+=to_string(itr->second);
                out+=",";
            }
            out.erase(out.end()-1,out.end());
        }
        return out;
    }

    bool get_state() {
        return state;
    }
};

class BDLect
{
private:
    map<string,map<string,float> > mapa;
    map<string,Record> usr;
    vector<string> obj;
    char type;
    bool chunk;
public:
    BDLect(const char* name, string typed,int ind1=0,int ind2=0,int indval=0) {
        if (typed=="tupla") {
            type='a';
            readdbtypea(mapa,name,ind1,ind2,indval);
        }
        if (typed=="matriz") {
            type='b';
            readdbtypeb(mapa,name,ind1,ind2,indval);
        }
    }

    void readdbtypea(map<string,map<string,float> > &maps,const char* name,int id1,int id2,int idval) {
        maps.clear();
        //mapa.clear();
        string tm;
        fstream arch(name,std::ios::in);
        int cont;
        if(!arch) {
            cout<<"Error al abrir archivo"<<endl;
            return;
        }
        while(getline(arch, tm)) {
            cont=0;
            string delimiter = ",";
            size_t pos = 0;
            string token;
            string tmpa,tmpb;
            float tmpc;

            while ((pos = tm.find(delimiter)) != string::npos) {
                token = tm.substr(0, pos);
                if (cont==id1) {
                    tmpa=token;
                }
                if (cont==id2) {
                    tmpb=token;
                }
                if (cont==idval) {
                    tmpc=atof(token.c_str());
                }
                tm.erase(0, pos + delimiter.length());
                cont++;
            }

            int tmpbi;
            bool found=false;
            for(int i=0;i<obj.size();i++) {
                if(obj[i]==tmpb) {
                    found=true;
                    tmpbi=i;
                }
            }
            if(!found) {
                obj.push_back(tmpb);
                tmpbi=obj.size()-1;
            }

            //maps[tmpa][tmpb]=tmpc;

            map<string, Record>::iterator itr;
            found=false;
            for (itr = usr.begin(); itr != usr.end(); ++itr) {
                if(itr->first==tmpa) {
                    found=true;
                    itr->second.add_point(tmpbi,tmpc);
                }
            }
        }
        arch.close();
    }

    void readdbtypeb(map<string,map<string,float> > &maps,const char* name,int id1,int id2,int idval) {
        maps.clear();
        string tm;
        fstream arch(name,std::ios::in);
        int cont;
        if(!arch) {
            cout<<"Error al abrir archivo"<<endl;
        }
        while(getline(arch, tm)) {
            cont=0;
            string delimiter = " ";
            size_t pos = 0;
            string token;
            string tmpa,tmpb;
            float tmpc;

            while ((pos = tm.find(delimiter)) != string::npos) {
                token = tm.substr(0, pos);
                if (cont==id1) {
                    tmpa=token;
                }
                if (cont==id2) {
                    tmpb=token;
                }
                if (cont==idval) {
                    tmpc=atof(token.c_str());
                }
                tm.erase(0, pos + delimiter.length());
                cont++;
            }
            maps[tmpa][tmpb]=tmpc;

        }
        arch.close();
    }

    float get_promedio(map<string,float> mapa) {
        map<string, float>::iterator itr;
        float cc=0;
        float den=0;
        for (itr = mapa.begin(); itr != mapa.end(); ++itr) {
            den+=itr->second;
            cc++;
        }
        if(cc>0) {
            return (den/cc);
        }
        else {
            return -1;
        }
    }

    float manhattanTA(map<string,float> usr1,map<string,float> usr2) {
        map<string, float>::iterator itr;
        map<string, float>::iterator itr2;
        float res=0.;
        for (itr = usr1.begin(); itr != usr1.end(); ++itr) {
            for (itr2 = usr2.begin(); itr2 != usr2.end(); ++itr2) {
                if(itr->first==itr2->first) {
                    float tmp=itr->second - itr2->second;
                    if(tmp<0) {
                        tmp=tmp*(-1);
                    }
                    res+=tmp;
                }
            }
        }
        return res;
    }

    float euclidianaTA(map<string,float> usr1,map<string,float> usr2) {
        map<string, float>::iterator itr;
        map<string, float>::iterator itr2;
        float res=0.;
        for (itr = usr1.begin(); itr != usr1.end(); ++itr) {
            for (itr2 = usr2.begin(); itr2 != usr2.end(); ++itr2) {
                if(itr->first==itr2->first) {
                    float tmp=itr->second - itr2->second;
                    tmp=pow(tmp,2);
                    res+=tmp;
                }
            }
        }
        res=sqrt(res);
        return res;
    }

    float pearsonTA(map<string,float> usr1,map<string,float> usr2) {
        map<string, float>::iterator itr;
        map<string, float>::iterator itr2;
        float res=0.;
        float sx,sy,sxy,sx2,sy2;
        float n;
        sx=sy=sxy=sx2=sy2=0;
        for (itr = usr1.begin(); itr != usr1.end(); ++itr) {
            for (itr2 = usr2.begin(); itr2 != usr2.end(); ++itr2) {
                if(itr->first==itr2->first) {
                    sx+=itr->second;
                    sy+=itr2->second;
                    sx2+=pow(itr->second,2);
                    sy2+=pow(itr2->second,2);
                    sxy+=(itr->second*itr2->second);
                    n+=1;
                }
            }
        }
        res=sxy-((sx*sy)/n);
        res=res/((sqrt(sx2-(pow(sx,2)/n)))*(sqrt(sy2-(pow(sy,2)/n))));
        return res;
    }

    float simcosTA(map<string,float> usr1,map<string,float> usr2) {
        map<string, float>::iterator itr;
        map<string, float>::iterator itr2;
        float res=0.;
        float sxy,sx2,sy2;
        float n;
        sxy=sx2=sy2=0;
        for (itr = usr1.begin(); itr != usr1.end(); ++itr) {
            for (itr2 = usr2.begin(); itr2 != usr2.end(); ++itr2) {
                if(itr->first==itr2->first) {
                    sx2+=pow(itr->second,2);
                    sy2+=pow(itr2->second,2);
                    sxy+=(itr->second*itr2->second);
                    n+=1;
                    break;
                }
            }
        }
        sx2=sqrt(sx2);
        sy2=sqrt(sy2);
        res=sxy/(sx2*sy2);
        return res;
    }

    float jaccardTA(map<string,float> usr1,map<string,float> usr2) {
        map<string, float>::iterator itr;
        map<string, float>::iterator itr2;
        float res=0.;
        float inter,unio;
        inter=unio=0;
        bool in;

        for (itr = usr1.begin(); itr != usr1.end(); ++itr) {
            in=false;
            for (itr2 = usr2.begin(); itr2 != usr2.end(); ++itr2) {
                if(itr->first==itr2->first) {
                    in=true;
                }
            }
            if(in) {
                inter+=1;
                unio+=1;
            }
            else
                unio+=1;

        }

        res=1-(inter/unio);
        return res;
    }

    float get_dist(string name1,string name2,string type) {
        if(type=="manhattan") {
            return manhattanTA(mapa[name1],mapa[name2]);
        }
        if(type=="euclidiana") {
            return euclidianaTA(mapa[name1],mapa[name2]);
        }
        if(type=="pearson") {
            return pearsonTA(mapa[name1],mapa[name2]);
        }
        if(type=="jaccard") {
            return jaccardTA(mapa[name1],mapa[name2]);
        }
        if(type=="sim_coseno") {
            return simcosTA(mapa[name1],mapa[name2]);
        }
        printf("Opcion incorrecta");
        return 0;
    }

    map<string,map<string,float> > get_cos_as() {
        map<string,map<string,float> > fn;
        //vector<vector<float> > res;
        vector<float> tmp;
        string tmp1,tmp2;
        tmp1=tmp2="";
        for(int i=0;i<obj.size();i++) {
            for(int j=0;j<obj.size();j++) {
                float res=0;
                if(obj[i]!=obj[j]) {
                    map<string,map<string,float> >::iterator itr;
                    map<string, float>::iterator itr2;
                    for (itr = mapa.begin(); itr != mapa.end(); ++itr) {
                        if((itr->second)[obj[i]]!=0 &&(itr->second)[obj[j]]!=0) {
                            float prom=get_promedio(itr->second);
                            res+=( ((itr->second)[obj[i]]-prom)*((itr->second)[obj[j]]-prom) );
                        }
                    }
                }

                fn[obj[i]][obj[j]]=res;

            }
        }
        return fn;
    }

    void mos_cos_aj() {
        map<string,map<string,float> > mms=get_cos_as();
        for(int i =obj.size();i>=0;i-=1) {
            for(int j=0;j<i;j++) {
                cout<<mms[obj[i]][obj[j]]<<",";
            }
            cout<<endl;
        }
    }

    map<string,map<string,float> > get_slop_one() {
        map<string,map<string,float> > fn;
        float carnada;
        //vector<vector<float> > res;
        vector<float> tmp;
        string tmp1,tmp2;
        tmp1=tmp2="";
        for(int i=0;i<obj.size();i++) {
            for(int j=0;j<obj.size();j++) {
                float res=0;
                if(obj[i]!=obj[j]) {
                    map<string,Record>::iterator itr;
                    map<string, float>::iterator itr2;
                    for (itr = usr.begin(); itr != usr.end(); ++itr) {
                        if((itr->second.get_val(i)!=0) &&(itr->second.get_val(j)!=0)) {
                            res+=itr->second.get_val(i)-itr->second.get_val(j)+res;
                        }
                    }
                }
                fn[obj[i]][obj[j]]=res;

            }
        }
        return fn;
    }

    void ins_user(string usrn) {
        map<string, Record>::iterator itr;
        bool found=false;
        for (itr = usr.begin(); itr != usr.end(); ++itr) {
            if(itr->first==usrn) {
                found=true;
                return;
            }
        }
        if(!found) {
            usr[usrn]=Record(usrn);
        }
    }

    void ins_val(string usrn,string objs,int cal) {
        int tmpbi;
        bool found=false;
        for(int i=0;i<obj.size();i++) {
            if(obj[i]==objs) {
                found=true;
                tmpbi=i;
            }
        }
        if(!found) {
            obj.push_back(objs);
            tmpbi=obj.size()-1;
        }



        map<string, Record>::iterator itr;
        found=false;
        for (itr = usr.begin(); itr != usr.end(); ++itr) {
            if(itr->first==usrn) {
                found=true;
                itr->second.add_point(tmpbi,cal);
            }
        }
    }


};
