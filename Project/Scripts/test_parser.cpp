#include<fstream>
#include<iostream>
using namespace std;

int main(){
    ifstream data_file("many_games.pgn");
    ofstream out("curated_games.txt");
    int game_count = 0;
    string l;
    while(!data_file.eof()){
        getline(data_file, l);
        if(l.size() > 1 && l[0] == '1'){
            out << l << '\n';
            game_count++;
        }
    }

    data_file.close();
    out.close();

    cout << "Total games: " << game_count;
    
}