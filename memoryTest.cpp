#include <iostream>
#include <pthread.h>

using namespace std;

struct Thing{
    int value;
    char character;
};

void* importantFunction(void* parameter){
    Thing* thing = (Thing*) parameter;
    thing->character = '*';
    thing->value += 10;
}

int main(){

    const int NTHREADS = 10;
    pthread_t tid[NTHREADS];
    static Thing things[NTHREADS];

    for(int i = 0; i < NTHREADS; i++){

        Thing thing;
        thing.character = '%';
        thing.value = i;
        things[i] = thing;

        if(pthread_create(&tid[i], NULL, importantFunction, &things[i])){
            cout << "FAILURE" << endl;
            return 1;
        }
    }

    for(int i = 0; i < NTHREADS; i++){
        cout << things[i].character << " " << things[i].value << endl;
    }
}