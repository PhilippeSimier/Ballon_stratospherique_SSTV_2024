/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 *
 * Created on 21 mars 2024, 08:21
 * ajouter l'option de compilation -pthread lors de la compilation, 
 */

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace std;

// Fonction exécutée par le premier thread
void threadFunction1() {
    for (int i = 0; i < 5; ++i) {
        cout << "Thread 1 executing..." << endl;
        this_thread::sleep_for(chrono::milliseconds(500)); // Attendre 500 millisecondes
    }
}

// Fonction exécutée par le deuxième thread
void threadFunction2() {
    for (int i = 0; i < 5; ++i) {
        cout << "Thread 2 executing..." << std::endl;
        this_thread::sleep_for(chrono::milliseconds(700)); // Attendre 700 millisecondes
    }
}


int main(int argc, char** argv) {
    
    // Création des deux threads
    thread t1(threadFunction1);
    thread t2(threadFunction2);

    // Attente de la fin des threads
    t1.join();
    t2.join();
    
    
    
    return 0;
}

