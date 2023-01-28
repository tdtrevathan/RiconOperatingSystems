#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

using namespace std;

#define NTHREADS 26
 
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

struct DecodeVariable{

    struct MinHeapNode* root;
    string nodeCode;
    Code* code;

    DecodeVariable(){
        root = nullptr;
        nodeCode = "";
        code = NULL;
    }

    DecodeVariable( MinHeapNode* r, string nCode, Code* c){
        root = r;
        nodeCode = nCode;
        code = c;
    }
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

    unsigned seq = 0;
 
    // Left and right child
    MinHeapNode *left, *right;
 
    MinHeapNode(Pair p){
        left = right = NULL;
        this->data = p.character;
        this->freq = p.frequency;
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
// For comparison of
// two heap nodes (needed in min heap)
struct compareMinHeap {
    
    bool operator()(MinHeapNode* l, MinHeapNode* r){

        if(l->freq == r->freq){
            if(l->data == r->data){
                return (l->seq < r->seq);
            }
            else{
                return (l->data > r->data);
            }
        }
        else{
            return (l->freq > r->freq);
        }
    }
};

//Go through the tree and extract characters from code positions
void* extractCodes(void * decodeVariable){

    DecodeVariable* variable = (DecodeVariable*) decodeVariable;

    if (!variable->root)
        return (void *)nullptr;

    if(variable->code->compressionCode == variable->nodeCode){
        variable->code->compressionCode = variable->root->data;
    }

    MinHeapNode* tempRoot = variable->root;
    string tempCode = variable->nodeCode;

    variable->root = variable->root->left;
    variable->nodeCode += "0";

    extractCodes((void *)variable);

    variable->root = tempRoot->right;
    variable->nodeCode = tempCode + "1";

    extractCodes((void *)variable);
}

void printQueue(priority_queue<MinHeapNode*, vector<MinHeapNode*>, compareMinHeap> queue){
    
    while(!queue.empty()){
        MinHeapNode* temp = queue.top();
        queue.pop();

        if( temp->data == '\0'){
            string left = "";
            left += temp->left->data;
            string right = "";
            right += temp->right->data;
            if(temp->left->data  == '\0'){
                left = "null";
            }
            else if (temp->left->data  == ' '){
                left = "space";
            }
            if(temp->right->data  == '\0'){
                right = "null";
            }
            else if (temp->right->data  == ' '){
                right = "space";
            }
            cout << "root values: " << left << " " << right << " ";
        }
        cout << "char: " << temp->data << " freq: " << temp->freq << endl;
    }
    cout << endl;
}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
MinHeapNode* HuffmanCodes(priority_queue<MinHeapNode*, vector<MinHeapNode*>, compareMinHeap>& minHeap, vector<Pair> pairs, vector<Code> &codes)
{
    struct MinHeapNode *left, *right, *top;

    for (int i = 0; i < pairs.size(); ++i){
        minHeap.push(new MinHeapNode(pairs.at(i)));
    }

    //printQueue(minHeap);

    int count = 0;
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
        // to the min heap '\0' is a special value
        // for internal nodes, not used
        Pair temp('\0',left->freq + right->freq);


        top = new MinHeapNode(temp);
        top->seq = count;
        count++;
        top->left = left;
        top->right = right;
 
        minHeap.push(top);
        //printQueue(minHeap);
        //cout << endl << endl;
        //printCodes(minHeap.top(), "");
        //cout << endl << endl;
    }
 
    // Print Huffman codes using
    // the Huffman tree built above
    printCodes(minHeap.top(), "");

    return minHeap.top();
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

void bubbleSort(vector<Pair> &pairs){
    int i, j;
    for (i = 0; i < pairs.size() - 1; i++){
        // Last i elements are already in place
        for (j = 0; j < pairs.size() - i - 1; j++)
            if (pairs.at(j).frequency > pairs.at(j + 1).frequency){
                Pair temp = pairs.at(j);
                pairs.at(j) = pairs.at(j+1);
                pairs.at(j+1) = temp;
            }
            else if(pairs.at(j).frequency == pairs.at(j + 1).frequency && (pairs.at(j).character > pairs.at(j + 1).character)){
                Pair temp = pairs.at(j);
                pairs.at(j) = pairs.at(j+1);
                pairs.at(j+1) = temp;
            }
    }
}

int main(){

    ifstream firstFile;
    ifstream secondFile;

    string firstFileName;
    string secondFileName;

    pthread_t tid[NTHREADS];
    static DecodeVariable variables[NTHREADS];

    cin >> firstFileName;

    firstFile.open(firstFileName);

    if(!firstFile.is_open()){
        return -1;
    }

    vector<Pair> pairs;
    vector<Code> codes;

    int count = 0;

    getPairs(firstFile, pairs);
    bubbleSort(pairs);
    
    cin >> secondFileName;
    secondFile.open(secondFileName);

    getLocations(secondFile, codes);

    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compareMinHeap> minHeap;

    MinHeapNode* root;
    root = HuffmanCodes(minHeap, pairs, codes);

    for(int i = 0; i < codes.size(); i++){

        Code* code = &codes.at(i);
        DecodeVariable var(root, "", code);
        variables[i] = var;

        //extractCodes((void*) &variables[i]);
        if (pthread_create(&tid[i], NULL, extractCodes, (void *) &variables[i])){
            fprintf(stderr, "Error could not create thread");
            return 1;
        }

        codes.at(i).compressionCode = variables[i].code->compressionCode;
    }

    for(int i = 0; i < codes.size(); i++){
        pthread_join(tid[i], NULL);
    }

    string message = "";

    message = decodeMessage(message, codes);

    cout << "Original message: " << message;
}