#include<fstream>
#include<iostream>
#include<string>
#include<set>
#include<map>
using namespace std;

int main(){
    ifstream f("openings(games)_c.txt");

    string n, w, b,ac;
    map<string, set<string>> opb;

    int games = 0;
    while(!f.eof()){
        f >> n >> w >> b;
        if(n == "1.") ac = "";
        ac += n + " " + w;
        if(opb[ac].find(b) == opb[ac].end()) opb[ac].insert(b);
        ac += " " + b + " ";
    }

    f.close();

    ofstream o("opening_book.txt");
    for(auto it = opb.begin(); it!=opb.end(); it++){
        o << "< " << it->first << " > " << it->second.size();
        for(auto it2 = it->second.begin(); it2!=it->second.end(); it2++){
            o << " " << *it2;
        }
        o << '\n';
    }

    o.close();
}