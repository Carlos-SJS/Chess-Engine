#include<fstream>
#include<iostream>
#include<string>
#include<set>
using namespace std;

int main(){
    ifstream f("openings(games).txt");

    string l;
    set<string> s;

    int games = 0;

    while(!f.eof()){
        getline(f, l);
        if(s.find(l) == s.end()){
            s.insert(l);
            games++;
        }
    }

    f.close();

    ofstream o("openings(games)_c.txt");
    
    for(auto it = s.begin(); it!=s.end(); it++) o << *it << '\n';

    o.close();
    
    cout << games << '\n';
}