// Please note this is a C program
// It compiles without warnings with gcc
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

void error(char *msg)
{
    perror(msg);
    _exit(0);
}

struct Code{
   string compressionCode;
   vector<int> locations; 
};


//struct containing all necessary information to decode the message in a multithreaded process
struct DecodeVariable{

    string nodeCode;
    Code* code;
    string* message;
    hostent* server;
    int portnumber;

    DecodeVariable(){
        nodeCode = "";
        code = NULL;
        message = nullptr;
        server = NULL;
        portnumber = 0;
    }

    DecodeVariable( string nCode, Code* c, string* m, hostent* s, int p){
        nodeCode = nCode;
        code = c;
        message = m;
        server = s;
        portnumber = p;
    }
};

void decodeMessage(string* message, Code* code){

    for(int k = 0; k < code->locations.size(); k++){
        int location = code->locations.at(k);
        while(message->size() < location+1){
            *message += " ";
        }
        message->at(location) = code->compressionCode.at(0);
    }
}


void* sendCodeToServer(void* decodeVariable){

    int sockfd, n;
    struct sockaddr_in serv_addr;
    hostent* server;
    
    DecodeVariable* variable = (DecodeVariable*) decodeVariable;

    server = variable->server;

    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        cout << "ERROR opening socket";
    }


    if (variable->server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *)variable->server->h_addr, (char *)&serv_addr.sin_addr.s_addr,variable->server->h_length);

    serv_addr.sin_port = htons(variable->portnumber);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        cout << "ERROR connecting";
    }

    bzero(buffer,256);

    string temp = variable->code->compressionCode;
    char* char_array = new char[temp.size()+1];


    int lengthOfString = temp.size()+1;

    n = write(sockfd, &lengthOfString, sizeof(int));

    if (n < 0) {
         cout << "ERROR writing to socket" ;
    }

    n = read(sockfd,buffer,255);
    strcpy(buffer, temp.c_str());

    if (n < 0) {
        cout << "ERROR reading from socket";
    }

    n = write(sockfd,&buffer, 255);

    if (n < 0) {
        cout << "ERROR writing to socket";
    }

    char result = '\0';

    n = read(sockfd,&result,sizeof(char));

    if (n < 0) {
        cout << "ERROR reading from socket";
    }
    
    string resultString = "";
    resultString += result;

    variable->code->compressionCode = resultString;
    
    decodeMessage(variable->message, variable->code);

    close(sockfd);
    return 0;
}

//save input into vector of huffman codes with their corresponding locations
void getLocations(string line, vector<Code> &codes){

    Code code;
    string compressionCode;
    vector<int> locations;

    //set compressionCode equal to the information up to the first space
    string first = line.substr(0,line.find(' '));

    //set the line equal to the rest of the information
    line = line.substr(line.find(' ')+1);
    compressionCode = first;
    
    //iterate through the remaining lines to save the locations
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

    //add info to compression code
    code.compressionCode = compressionCode;
    code.locations = locations;
    codes.push_back(code);

}

int main(int argc, char *argv[])
{

    const int NTHREADS = 26;

    static string message = "";
    string *messagePtr = &message;
    static DecodeVariable variables[NTHREADS];
    pthread_t tid[NTHREADS];

    int portno;
    portno = atoi(argv[2]);

    struct hostent *server;

    server = gethostbyname(argv[1]);

    string line;
    vector<Code> codes;

    while(getline(cin,line)){
        getLocations(line,codes);
    }

    for (int i = 0; i < codes.size(); i++)
    {

        //create a thread

        //the thread will create a socket

        //then it will send the the binary code to the server
        
        //wait for the response

        //write it to message


        //Code *code = &codes.at(0);
        DecodeVariable var("", &codes.at(i), messagePtr, server, portno);
        variables[i] = var;

        //sendCodeToServer(&variables[0]);

        if (pthread_create(&tid[i], NULL, sendCodeToServer, &variables[i]))
        {
            fprintf(stderr, "Error could not create thread");
            return 1;
        }
    }
    for (int i = 0; i < codes.size(); i++)
    {
        pthread_join(tid[i], NULL);
    }

    cout << "Original message: " << message;
}
