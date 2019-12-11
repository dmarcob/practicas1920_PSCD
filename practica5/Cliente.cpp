//*****************************************************************
// File:   ClienteAvion.cpp
// Author: Diego Marco 755233
// Date:   noviembre 2018
// Coms:   cliente servicio de aviones
//
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

int main() {

    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = "127.0.0.1";//"localhost";
    int SERVER_PORT = 2000;
    int contador = 0;
	string buffer; //buffer para recibir respuesta la mensaje inicial
	string str1 ("VUELO COMPLETO"); //Mensaje a recibir si no se puede reservar mas;
  string str2("RESERVADA");
	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 10;
	int count = 0;
	int socket_fd;
	do {
		// Conexión con el servidor.
    	socket_fd = socket.Connect();
    	count++;

    	// Si error --> esperamos 1 segundo para reconectar
    	if(socket_fd == -1){
    	    this_thread::sleep_for(chrono::seconds(1));
    	}
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1){
    	return socket_fd;
    }
        //Enviamos mensaje inicial
        string nombre;
        string peticion;
		cout << "INICIO COMPRA:";
		cin >> nombre;
		cout << endl;
		string mensaje = "INICIO COMPRA:" + nombre;
	    int send_bytes = socket.Send(socket_fd, mensaje);
	    if(send_bytes == -1){
			cerr << "Error al enviar mensaje de inicio: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
		//Recibimos la respuesta del servidor al mensaje inicial
		int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
		if(read_bytes == -1){
			cerr << "Error al recibir la respuesta al mensaje de inicio: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
	    //Sacamos por pantalla el mensaje
		cout << buffer << endl;

	while(1) {
		//Peticion de reserva de plaza de avion
		cout << "RESERVAR:";
		cin>>peticion;
		mensaje = "RESERVAR:" + peticion;
	    send_bytes = socket.Send(socket_fd, mensaje);
	    if(send_bytes == -1){
			cerr << "Error al enviar peticion de reserva: " << nombre << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
		//Recibimos respuesta del servidor
		int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
		if(read_bytes == -1){
			cerr << "Error al recibir la respuesta al mensaje de inicio: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
		// Caso en que avion esta lleno y manda mensaje VUELO COMPLETO
		else if (str1.compare(buffer) == 0) {
				socket.Close(socket_fd);
        cout << buffer << endl;
        cout <<"Plazas reservadas: " << contador << endl;
				exit(1);
		}
		else {
				cout << buffer << endl;
        buffer.resize(9);
        if (str2.compare(buffer) == 0){
            contador++;
        }
		}
	}

    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    // Despedida
	cout << "Bye bye" << endl;

    return error_code;
}
