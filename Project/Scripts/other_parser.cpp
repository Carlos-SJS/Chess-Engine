#include<fstream>
#include<iostream>
#include<string>
using namespace std;

int main(){
    ifstream f("curated_games.txt");
    ofstream o("openings.txt");
    int move_count = 1;
    int game_count = 0;

    string num, w, b;

    string l;
    while(!f.eof()){
        game_count ++;
        while(move_count <= 5){
            f >> num;
            if(num == to_string(move_count)+"."){
                f >> w;
                if(w[0] < 'A' || (w[0]>'Z' && w[0]<'a') || w[0] > 'z') goto ignore;
                f >> b;
                if(b[0] < 'A' || (b[0]>'Z' && b[0]<'a') || b[0] > 'z') goto ignore;

                o << num << " " << w << " " << b << " ";
            }

            move_count++;
        }
        ignore:;
        o << '\n';
        getline(f,num);

        move_count=1;
    }
    
    f.close();
    o.close();

    cout << "Total games: " << game_count;
    
}