#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct Pair{
    char character;
    int frequency;

    Pair(){
        character = -1;
        frequency = -1;
    }
    Pair(char c, int f){
        character = c;
        frequency = f;
    }
};

struct Code{
   string compressionCode;
   vector<int> locations; 
};


///Grabbed Geeks for Geeks Huffman Code algorithm
///https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
///with some alterations

// A Huffman tree node
struct MinHeapNode {
 
    // One of the input characters
    char data;
 
    // Frequency of the character
    unsigned freq;
 
    // Left and right child
    MinHeapNode *left, *right;
 
    MinHeapNode(Pair p){
        left = right = NULL;
        this->data = p.character;
        this->freq = p.frequency;
    }
};
 
// For comparison of
// two heap nodes (needed in min heap)
struct compareMinHeap {
 
    bool operator()(MinHeapNode* l, MinHeapNode* r){
        if(l->freq == r->freq){
            return (l->data > r->data);
        }
        else{
            return (l->freq > r->freq);
        }
    }
};

// Prints huffman codes from
// the root of Huffman Tree.
void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;
 
    if (root->data != '\0'){
        cout << "Symbol: " << root->data << ", Frequency: " << root->freq << ", Code: " << str << "\n";
    }
 
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

//Go through the tree and extract characters from code positions
void extractCodes(struct MinHeapNode* root, string code, vector<Code> &codes)
{
    if (!root)
        return;
 
    if (root->data != '\0'){
        for(int i = 0; i < codes.size(); i++){
            if(codes.at(i).compressionCode == code){
                codes.at(i).compressionCode = root->data;
            }
        }
    }
 
    extractCodes(root->left, code + "0", codes);
    extractCodes(root->right, code + "1", codes);
}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
void HuffmanCodes(vector<Pair> pairs, vector<Code> &codes)
{
    struct MinHeapNode *left, *right, *top;
 
    // Create a min heap & inserts all characters of data[]
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compareMinHeap> minHeap;
    
    for (int i = 0; i < pairs.size(); ++i)
        minHeap.push(new MinHeapNode(pairs.at(i)));
 
    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1) {
 
        // Extract the two minimum
        // freq items from min heap
        left = minHeap.top();
        minHeap.pop();
 
        right = minHeap.top();
        minHeap.pop();
 
        // Create a new internal node with
        // frequency equal to the sum of the
        // two nodes frequencies. Make the
        // two extracted node as left and right children
        // of this new node. Add this node
        // to the min heap '$' is a special value
        // for internal nodes, not used
        Pair temp('\0',left->freq + right->freq);
        top = new MinHeapNode(temp);
 
        top->left = left;
        top->right = right;
 
        minHeap.push(top);
    }
 
    // Print Huffman codes using
    // the Huffman tree built above
    printCodes(minHeap.top(), "");
    extractCodes(minHeap.top(), "", codes);
}
///End of Geeks for Geeks huffman code

void getPairs(ifstream &infile, vector<Pair> &pairs){

    string temp;

    while(!infile.eof()){
        Pair pair;
        getline(infile, temp);

        char character = temp.at(0);

        string frequency = "";
        frequency += temp.at(2);

        pair.character = character;
        pair.frequency = stoi(frequency);
        pairs.push_back(pair);
    }
}

void getLocations(ifstream &infile, vector<Code> &codes){

    string line;
    int code;

    while(!infile.eof()){

        Code code;
        string compressionCode;
        vector<int> locations;

        getline(infile, line);
        string first = line.substr(0,line.find(' '));
        line = line.substr(line.find(' ')+1);
        compressionCode = first;

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

string decodeMessage(string message, vector<Code> &codes){

    for(int i = 0; i < codes.size(); i++){
        for(int k = 0; k < codes.at(i).locations.size(); k++){

            int location = codes.at(i).locations.at(k);

            while(message.size() < location+1){
                message += " ";
            }

            message.at(location) = codes.at(i).compressionCode.at(0);
        }
    }

    return message;
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

    getPairs(firstFile, pairs);

    /*
    int i, j;
    for (i = 0; i < pairs.size() - 1; i++){
        // Last i elements are already in place
        for (j = 0; j < pairs.size() - i - 1; j++)
            if (pairs.at(j).frequency > pairs.at(j + 1).frequency){
                Pair temp = pairs.at(j);
                pairs.at(j) = pairs.at(j+1);
                pairs.at(j+1) = temp;
            }
    }

    
        for(int i = 0; i < pairs.size(); i++){
            cout << pairs.at(i).character << " " << pairs.at(i).frequency << endl;
        }
        cout << endl;
    */

    cin >> secondFileName;
    secondFile.open(secondFileName);

    getLocations(secondFile, codes);

    //for(int i = 0; i < codes.size(); i++){
    //    cout << codes.at(i).compressionCode << " ";
    //    for(int k = 0; k < codes.at(i).locations.size(); k++){
    //        cout << codes.at(i).locations.at(k) << " ";
    //    } 
    //    cout << endl << endl;
    //}

    HuffmanCodes(pairs, codes);

    string message = "";

    message = decodeMessage(message, codes);

    cout << "Original message: " << message;
}