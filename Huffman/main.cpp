#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct Pair{
    char character;
    int freqeuency;
};

struct Code{
   int compressionCode;
   vector<int> locations; 
};

struct priority {
    bool operator()(Pair* p, Pair* p2){
        if(p->freqeuency == p2->freqeuency){
            return (p->character < p2->character);
        }
        else{
            return p->freqeuency > p2->freqeuency;
        }
    }
};

// A Huffman tree node
struct MinHeapNode {
 
    // One of the input characters
    char data;
 
    // Frequency of the character
    unsigned freq;
 
    // Left and right child
    MinHeapNode *left, *right;
 
    MinHeapNode(char data, unsigned freq)
 
    {
 
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};
 
// For comparison of
// two heap nodes (needed in min heap)
struct compare {
 
    bool operator()(MinHeapNode* l, MinHeapNode* r)
 
    {
        return (l->freq > r->freq);
    }
};
 
// Prints huffman codes from
// the root of Huffman Tree.
void printCodes(struct MinHeapNode* root, string str)
{
 
    if (!root)
        return;
 
    if (root->data != '$')
        cout << root->data << ": " << str << "\n";
 
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}
 
 ///Grabbed Geeks for Geeks Huffman Code algorithm
 ///https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size, std::priority_queue<Pair*,vector<Pair*>)
{
    struct MinHeapNode *left, *right, *top;
 
    // Create a min heap & inserts all characters of data[]
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
 
    for (int i = 0; i < size; ++i)
        minHeap.push(new MinHeapNode(data[i], freq[i]));
 
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
        top = new MinHeapNode('$', left->freq + right->freq);
 
        top->left = left;
        top->right = right;
 
        minHeap.push(top);
    }
 
    // Print Huffman codes using
    // the Huffman tree built above
    printCodes(minHeap.top(), "");
}

///End of Geeks for Geeks huffman code

/*
struct HuffmanTree{
    struct node{
        int frequency;
        int character;
        node* left;
        node* right;
    };

    node* root;

    HuffmanTree(){
        root == nullptr;
    }

    node* addNode(node* current, node* newNode){
        if(root == nullptr){
            root = newNode;
            return root;
        }

        if(newNode->frequency < )
    }
};

*/

void getPairs(ifstream &infile, vector<Pair> &pairs){

    string temp;

    while(!infile.eof()){
        Pair pair;
        string temp;
        infile >> temp;
        char character = temp.at(0);
        pair.character = character;
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

std::priority_queue<Pair*,vector<Pair*>, priority>  createPriorityQueue(vector<Pair> &pairs){

    std::priority_queue<Pair*,vector<Pair*>, priority> q;
    
    for(int i = 0; i < pairs.size(); i++){
        Pair* p = &pairs[i];
        q.push(p);
    }

    return q;
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
        cout << endl << endl;
    }

    std::priority_queue<Pair*,vector<Pair*>, priority> q = createPriorityQueue(pairs);

    char* characters = new char[q.size()];
    int* frequencies = new int[q.size()];
    int count = 0;

    while (!q.empty()) {
		Pair* p = q.top();
		characters[count] = p->character;
        frequencies[count] = p->freqeuency;
		q.pop();
        count++;
	}

    HuffmanCodes(characters, frequencies, count);
}