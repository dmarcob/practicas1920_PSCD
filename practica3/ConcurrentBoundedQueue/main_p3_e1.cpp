//*****************************************************************
// File:   main_p3_e1.cpp (esqueleto)
// Author: PSCD-Unizar
// Date:   octubre 2018
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante el makefile asociado
//         Invocación: main_p3_e1
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "ConcurrentBoundedQueue.hpp"

using namespace std;
#define LONG_ALFABETO 26 //Número de letras del alfabeto
#define ASCII_INIT_MINUSCULAS 97 //Carácter ASCII de 'a'
#define MOD(i,n) ((i % n + n) % n) //Módulo positivo
//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"? Modo main: declara la variable "logger"
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger logger("_log_.log");
#else
    #ifndef ADD_EVENT //para evitar redefiniciones
        #define ADD_EVENT(e)  // nada
    #endif
#endif
//-----------------------------------------------------

//-----------------------------------------------------
//Pre:  <entrada> es un string no vacío de caracteres ascii minúsculas.
//Post: Devuelve un string resultante de aplicar el Cifrado Cesar con
//      desplazamiento <desplazamiento> al string <entrada>
string cifrar(const string entrada, const int desplazamiento) {
   const string minusculas = "abcdefghijklmnopqrstuvwxyz";
   string salida; int i = 0;
   while (i != entrada.length()) {
     if (!isalpha(entrada[i])) {
       //caso no es una letra
       salida += entrada[i];}
     else {
       //caso es una letra
       salida += minusculas[ MOD(entrada[i] - ASCII_INIT_MINUSCULAS + desplazamiento, LONG_ALFABETO)];
     }
     i++;
   }
   return salida;
}


//-----------------------------------------------------
//Pre:  <entrada> es un string no vacío de caracteres ascii minúsculas.
//Post: Devuelve un string, resultado de descifrar el Cifrado Cesar con desplazamiento
//      <desplazamiento> al string de entrada <entrada>
string descifrar(const string entrada, const int desplazamiento) {
   const string minusculas = "abcdefghijklmnopqrstuvwxyz";
   string salida; int i = 0;
   while (i != entrada.length()) {
     if (!isalpha(entrada[i])) {
       //caso no es una letra
       salida += entrada[i];}
     else {
       //caso es una letra
       salida += minusculas[ MOD(entrada[i] - ASCII_INIT_MINUSCULAS - desplazamiento, LONG_ALFABETO)];
     }
     i++;
   }
   return salida;
}

template <class T>
void insertar(ConcurrentBoundedQueue<T> &cbq, const int nD){
    const string mensajes[] = {
      "a leopard never changes its spots",
      "a friend in need is a friend indeed",
      "kill two birds with one stone",
      "every cloud has a silver lining",
      "things often happen when you least expect them to",
      "he who laughs last laughs longest",
      "a cat in gloves catches no mice",
      "actions speak louder than words"
    };
    for(int i=0; i<nD; i++){
    	int num = rand() % 100;
        cbq.enqueue(cifrar(mensajes[i], 3));
        this_thread::sleep_for(chrono::milliseconds(num));
    }
}

template <class T>
void extraer(ConcurrentBoundedQueue<T> &cbq, const int nD){
    for(int i=0; i<nD; i++){
    	T v;
        cbq.firstR(v);
        cout <<  "id_" << this_thread::get_id() <<", " << descifrar(v, 3) << endl;
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));
    }
}

int main(int argc, char* argv[]) {
    const int N = 10; //tamaño de la cola
    const int N_DAT_ESC = 8; //Número de datos a insertar por el escritor
    const int N_DAT_LEC = 6; //Número de datos a leer por el lector
    const int N_ESC = 4;  //Número de procesos escritores
    const int N_LEC = 5;  //Número de procesos lectores

    ADD_EVENT("main, BEGIN_FUNC_PROC, 0")
    ConcurrentBoundedQueue<string> cbq(N);
    thread pIns[N_ESC];
    thread pExt[N_LEC];

    for (int i=0; i<N_ESC; i++){
        pIns[i] = thread (&insertar<string>, ref(cbq), N_DAT_ESC);
    }

    for (int i=0; i<N_LEC; i++){
        pExt[i] = thread (&extraer<string>, ref(cbq), N_DAT_LEC);
    }

    for (int i=0; i<N_ESC; i++){
        pIns[i].join();
    }

    for (int i=0; i<N_LEC; i++){
        pExt[i].join();
    }

    cout << "------------------------------------------" << endl;
    cout << "Contenido final de la cola: ";
    cbq.print();
    cout << endl;
    cout << "------------------------------------------" << endl;
    return 0;
    ADD_EVENT("main,END_FUNC_PROC,0");
}
