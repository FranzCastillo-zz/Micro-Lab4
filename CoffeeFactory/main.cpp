using namespace std;

#include <iostream>
#include <unistd.h>

pthread_mutex_t mutexCoffee;
pthread_cond_t canBalerStart;

// INITIAL VALUES FOR THE SIMULATION
const int maxBags = 20;
int warehouseBags = 0;
int siloBags = 0;

/**
 * Prints on to the console a visual separator for printed states of other functions
 */
void separator(){
    cout << "================================================" << endl;
}

/**
 * Thread function that emulates the working process of a toaster
 * @param arg the Toaster ID (int)
 * @return NULL
 */
void* toaster(void* arg){
    // Gets the toaster ID
    int iToaster = *(int*) arg;

    // Produces the bags
    for(int i = 0; i < maxBags / 2; i++){
        pthread_mutex_lock(&mutexCoffee);
        siloBags++;
        cout << "Tostadora " << iToaster << " produjo: 1 lb de cafe tostado" << endl;
        pthread_mutex_unlock(&mutexCoffee);
        // Signals that a bag has been produced for the baler to check
        pthread_cond_signal(&canBalerStart);
        sleep(1);
    }

    // Deallocate the memory of the parameter
    free(arg);

    //Exits the thread
    pthread_exit(nullptr);
}

/**
 * Thread function that emulates the working process of a baler
 * @param arg nullptr
 * @return NULL
 */
void* baler(void* arg){
    pthread_mutex_lock(&mutexCoffee);
    // Keeps checking that the toasters have produced at least 5 coffee bags
    while(siloBags < 5){
        separator();
        cout << "No se puede empacar todavia. lb: " << siloBags << endl;
        separator();
        pthread_cond_wait(&canBalerStart, &mutexCoffee);
    }
    pthread_mutex_unlock(&mutexCoffee);

    // Can start baling
    cout << "La empacadora ha comenzado a empacar" << endl;
    while(warehouseBags <= maxBags){
        pthread_mutex_lock(&mutexCoffee);
        separator();
        cout << "Lbs de cafe en silo: " << siloBags-- << endl;
        cout << "La empacadora produjo: 1 bolsa de 1 lb de cafe" << endl;
        cout << "Lbs de cafe en bodega: " << warehouseBags++ << endl;
        separator();
        pthread_mutex_unlock(&mutexCoffee);
        sleep(1);
    }
    // Exits the thread
    pthread_exit(nullptr);
}

int main() {
    // Initializes mutex and condition variable
    pthread_mutex_init(&mutexCoffee, nullptr);
    pthread_cond_init(&canBalerStart, nullptr);
    pthread_t threads[3];

    for(int i = 0; i < 3; i++){
        // On the last iteration creates the baler
        if(i == 2){
            pthread_create(&threads[i], nullptr, &baler, nullptr);
        }else{ // Creates the Toasters
            int* iToaster = (int*)malloc(sizeof(int));
            *iToaster = i + 1;
            pthread_create(&threads[i], nullptr, &toaster, iToaster);
        }
    }
    // Joins the threads
    for(auto thread : threads){
        pthread_join(thread, nullptr);
    }       // Deallocates memory
    pthread_mutex_destroy(&mutexCoffee);
    pthread_cond_destroy(&canBalerStart);
    return 0;
}
