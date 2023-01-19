#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Pair{
    int character;
    int freqeuency;
};

struct Code{
   int compressionCode;
   vector<int> locations; 
};

void getPairs(ifstream &infile, vector<Pair> &pairs){

    string temp;

    while(!infile.eof()){
        Pair pair;
        string temp;
        infile >> temp;
        char character = temp.at(0);
        pair.character = (int)character;
        infile >> temp;
        pair.freqeuency = stoi(temp);
        pairs.push_back(pair);
    }
}

void getLocations(ifstream &infile, vector<Code> &codes){

    string line;
    int code;

    while(!infile.eof()){

        Code code;
        int compressionCode;
        vector<int> locations;

        getline(infile, line);
        string first = line.substr(0,line.find(' '));
        line = line.substr(line.find(' ')+1);
        cout << line;
        compressionCode = stoi(first);


        while(line != ""){
            
            string location;
            
            if(line.find(' ') != string::npos){
                location = line.substr(0,line.find(' '));
            }
            else{
                location = line;
                line = "";
            }

            line = line.substr(line.find(' ')+1);
            cout << line;
            locations.push_back(stoi(location));

            if(location == ""){
                locations.push_back(stoi(line));
            }
        }

        code.compressionCode = compressionCode;
        code.locations = locations;
        codes.push_back(code);
    }
}

int main(){

    ifstream firstFile;
    ifstream secondFile;

    string firstFileName;
    string secondFileName;

    cin >> firstFileName;

    firstFile.open(firstFileName);

    if(!firstFile.is_open()){
        return -1;
    }

    vector<Pair> pairs;
    vector<Code> codes;

    int count = 0;

    getPairs(firstFile,pairs);

    for(int i = 0; i < pairs.size(); i++){
        cout << pairs.at(i).character << " " << pairs.at(i).freqeuency << endl;
    }

    cout << endl;

    cin >> secondFileName;
    secondFile.open(secondFileName);

    getLocations(secondFile, codes);

    cout << endl << endl;
    for(int i = 0; i < codes.size(); i++){
        cout << codes.at(i).compressionCode << " ";
        for(int k = 0; k < codes.at(i).locations.size(); k++){
            cout << codes.at(i).locations.at(k) << " ";
        } 
        cout << endl;
    }
}