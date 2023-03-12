#include <iostream>
#include <vector>
#include <pthread.h>
#include <iomanip>

using namespace std;

const int NTHREADS = 4;

struct Values{
    int value1;
    int value2;
    int index;
};

void* evaluateValues(void* values){

    Values* vals = (Values*) values;

    if(vals->index == 0){
        cout << vals->value1 << " + " << vals->value2 << " = " << (double)(vals->value1 + vals->value2) << endl;
    }
    else if(vals->index == 1){
        cout << vals->value1 << " - " << vals->value2 << " = " << (double)(vals->value1 - vals->value2) << endl;
    }
    else if(vals->index == 2){
        cout << vals->value1 << " * " << vals->value2 << " = " << (double)(vals->value1 * vals->value2) << endl;
    }
    else {
        // if idividing by zero, ensure result is zero
        if(vals->value2 == 0){
            cout << vals->value1 << " / " << vals->value2 << " = 0.00" << endl;
        }
        else{
            cout << vals->value1 << " / " << vals->value2 << " = " << (double)(vals->value1) / vals->value2 << endl;
        }
    }
}

int main(){
    pthread_t tid[NTHREADS];

    static Values values[NTHREADS];

    cout << fixed << setprecision(2);

    for(int i = 0; i < 4; i++){

        int value1, value2;
        cin >> value1;
        cin >> value2;

        Values vals;
        vals.index = i;
        vals.value1 = value1;
        vals.value2 = value2;

        values[i] = vals;

        
    }

    for(int i = 0; i < 4; i++){
        if(pthread_create(&tid[i], NULL, evaluateValues, (void*) &values[i])){
            cout << "could not create thread" << endl;
            return 1;
        }
    }

    for(int i = 0; i < 4; i++){
        pthread_join(tid[i], NULL);
    }
}