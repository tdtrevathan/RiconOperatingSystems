#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
//void* extractCodes(void * decodeVariable){
//
//    DecodeVariable* variable = (DecodeVariable*) decodeVariable;
//
//    if (!variable->root)
//        return (void *)nullptr;
//
//    if(variable->code->compressionCode == variable->nodeCode){
//        variable->code->compressionCode = variable->root->data;
//        decodeMessage(variable->message, variable->code);
//    }
//
//    MinHeapNode* tempRoot = variable->root;
//    string tempCode = variable->nodeCode;
//
//    variable->root = variable->root->left;
//    variable->nodeCode += "0";
//
//    extractCodes((void *)variable);
//
//    variable->root = tempRoot->right;
//    variable->nodeCode = tempCode + "1";
//
//    extractCodes((void *)variable);
//}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
MinHeapNode* HuffmanCodes(priority_queue<MinHeapNode*, vector<MinHeapNode*>, compareMinHeap>& minHeap, vector<Pair> pairs)
{
    struct MinHeapNode *left, *right, *top;

    for (int i = 0; i < pairs.size(); ++i){
        minHeap.push(new MinHeapNode(pairs.at(i)));
    }

    //created an arbirtary count value to regulate comparison issue with standard library priority queue
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

        //set the sequence integer to the arbitrary count
        top->seq = count;
        //icriment count so each node is distiguishable by order entered
        count++;

        top->left = left;
        top->right = right;
 
        minHeap.push(top);
    }
 
    // Print Huffman codes using
    // the Huffman tree built above
    printCodes(minHeap.top(), "");

    return minHeap.top();
}
///End of Geeks for Geeks huffman code

//save input into a vector of character frequency pairs
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

//save input into a vector of character frequency pairs
void addPair(string line, vector<Pair> &pairs){

        Pair pair;

        char character = line.at(0);

        string frequency = "";
        frequency += line.at(2);

        pair.character = character;
        pair.frequency = stoi(frequency);
        pairs.push_back(pair);
}



int main(int argc, char* argv[]){

    //server stuff
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256];
    sockaddr_in serv_addr, cli_addr;
    hostent *server;

    //thread stuff

    //ifstream firstFile;
    //ifstream secondFile;
    //string secondFileName;


    //tree stuff
    vector<Pair> pairs;

    string line;
    

    while (getline(cin, line))
    {
        addPair(line, pairs);
    }
    // int count = 0;
    // getPairs(firstFile, pairs);
    // cin >> secondFileName;
    // secondFile.open(secondFileName);
    // getLocations(secondFile, codes);
    
    priority_queue<MinHeapNode *, vector<MinHeapNode *>, compareMinHeap> minHeap;
    MinHeapNode *root;


    root = HuffmanCodes(minHeap, pairs);

  
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            cout << "ERROR opening socket";
            return 1;
        }

        bzero((char *)&serv_addr, sizeof(serv_addr));

        portno = atoi(argv[1]);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            cout << "ERROR on binding";
            return 1;
        }

        listen(sockfd, 52);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

    for(int i = 0; i < pairs.size(); i++)
    {
        if (fork() == 0)
        {
            if (newsockfd < 0)
            {
                cout << "ERROR on accept";
                return 1;
            }

            bzero(buffer, 256);
            int sizeOfMessage;

            n = read(newsockfd, &sizeOfMessage, sizeof(int));

            if (n < 0)
            {
                cout << "ERROR reading from socket";
                return 1;
            }

            n = write(newsockfd, "I got your message", 18);

            string value;
            n = read(newsockfd, &value, sizeOfMessage + 1);

            cout << "Value = " << value << endl;

            n = write(newsockfd, "I got your message", 18);

            if (n < 0)
            {
                cout << "ERROR writing to socket";
                return 1;
            }

            close(newsockfd);
            _exit(0);
        }
    }
    wait(0);
    close(sockfd);
    
}