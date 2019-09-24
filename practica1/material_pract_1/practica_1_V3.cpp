//*****************************************************************
// File:   practica_1_V3.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2019
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ practica_1_V3.cpp -o practica_1_V3 -std=c++11 -pthread
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

//-----------------------------------------------------
// Parte de especificación: variables y funciones
// de los objetos de esta clase.
// Normalmente iría en el fichero "Saludador.hpp"
// Dejamos todo público, para centrarnos en los conceptos
// de la concurrencia. Lo adecuado es distinguir entre
// parte pública y privada, con operadores para acceder
// a los atributos, por ejemplo. Lo usaremos en prácticas
// futuras

class Saludador {
public:
    Saludador(string mens,  //constructor suministrando datos
              int retardo,
              int veces);
    Saludador(); //constructor por defecto
    void run();
    //más funciones, si las hubiera
    string mens;
    int retardo, veces;
};
//-----------------------------------------------------
// Implementación (las funciones de los objetos de la clase)
// Normalmente iría en el fichero "Saludador.cpp"

Saludador::Saludador(string mens, int retardo, int veces) {
    this->mens = mens; //mens: parámetro de la función
                       //this->mens: variable propia del objeto
    this->retardo = retardo;
    this->veces = veces;
};

Saludador::Saludador() {
    mens = ""; //mens: parámetro de la función
               //this->mens: variable propia
    retardo = 0;
    veces = 0;
};

void Saludador::run() {
    for(int i=1; i<=veces; i++) {
        cout << mens +"\n";
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
};
//-----------------------------------------------------
int main() {

    //creación de los objetos: constructor con datos
    Saludador s1("Soy 1", 100, 10),
              s2("\tSoy 2", 150, 15),
              s3("\t\tSoy 3", 10, 40);
    //creación de los objetos: constructor por defecto         
    Saludador s4;

    //le damos datos a s4
    s4 = {"\t\t\tSoy 4", 2, 12};
    //equivalente a 
    // s4.mens = "\t\t\tSoy 4"; s4.retardo = 2; ...
    cout << "veces: " << s4.veces << endl;
    //puesta en ejecución
    thread th_1 = thread(&Saludador::run, s1);
    thread th_2 = thread(&Saludador::run, s2);
    thread th_3 = thread(&Saludador::run, s3);
    thread th_4 = thread(&Saludador::run, s4);
    //esperar a que vayan acabando
    th_1.join();
    th_2.join();
    th_3.join();
    th_4.join();

    return 0;
}