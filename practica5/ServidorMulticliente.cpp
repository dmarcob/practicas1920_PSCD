//*****************************************************************
// File:   ServidorMulticliente.cpp
// Author: Diego Marco 755232
// Date:   diciembre 2019
// Coms:   Servidor multicliente para la reserva de plazas de un avion
//
//*****************************************************************


#include "Socket.hpp"
#include <iostream>
#include <thread>
#include <cstring> //manejo de cadenas tipo C
#include "GestorReserva/GestorReserva.hpp"

using namespace std;

//Servidor: 	int socket_fd =socket.Bind();
//						client_fd[i] = socket.Accept();
//						int rcv_bytes = soc.Recv(int client_fd,char* buffer,int length);
//						int send_bytes = soc.Send(int client_fd, string s);


void servCliente(Socket& soc, int client_fd, GestorReserva &GR) {
	//Declaracion de variables
	const int MAX_LENGTH = 100; //Max tamano de mensaje recibido
	char respuesta_inicial[] = "BIENVENIDO AL SERVICIO"; 	//Respuesta incial
	char respuesta_reserva[MAX_LENGTH];	//Respuesta a la reserva del cliente
	char buffer[MAX_LENGTH];	// Buffer para recibir el mensaje
	bool reservar;
	int fila, columna;	//Fila y columna correspondientes a una plaza de avion

	//Recepcion de peticion de inicio de compra de cliente
	int rcv_bytes = soc.Recv(client_fd, buffer, MAX_LENGTH);
		if (rcv_bytes == -1) {
			cerr << "Error datos al recibir peticion inicial: " + string(strerror(errno)) + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}

	cout << "Mensaje recibido: " + string(buffer) + "\n";

	//Respuesta inicial a la peticion de compra
	int send_bytes = soc.Send(client_fd, respuesta_inicial);
		if(send_bytes == -1) {
			cerr << "Error datos al enviar respuesta inicial: " + string(strerror(errno)) + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
			exit(1);
		}

	bool out = false; // Inicialmente no salir del bucle
	while(!out) {
		//Recepcion de plaza a reservar por cliente
		int rcv_bytes = soc.Recv(client_fd, buffer, MAX_LENGTH);
			if (rcv_bytes == -1) {
				cerr << "Error datos al recibir reserva: " + string(strerror(errno)) + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
			}
		cout << "Mensaje recibido: " + string(buffer) + "\n";
		//fila y columna de la plaza que se quiere reservar, rango [1, 4]
		fila = buffer[9] - '0';
		columna = buffer[10] - '@';
		if ((fila < 1 || fila > 4) || (columna < 1 || columna > 4) ) {
			cerr << "Columna o fila errónea. Fila: " << fila << ", Columna: " << columna << endl;
		}
		//Intento reservar la plaza. (Gestion por monitor)
		GR.reservar_plaza(reservar, fila, columna);
		if (reservar) {
			//He conseguido reservar
			memset(respuesta_reserva, 0, MAX_LENGTH);
			string mensaje(respuesta_reserva);
			mensaje.append("RESERVADA:"+to_string (fila)+(char)(columna +'@'));
			strcpy(respuesta_reserva, mensaje.c_str());
		} else {
			//No he conseguido reservar. (Gestion por monitor)
			GR.plaza_ocupada(respuesta_reserva, MAX_LENGTH, out);
		}
		//Send respuesta
		int send_bytes = soc.Send(client_fd, respuesta_reserva);
			if(send_bytes == -1) {
				cerr << "Error datos al enviar respuesta inicial: " + string(strerror(errno)) + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
				exit(1);
			}
	}
	soc.Close(client_fd);
}
//-------------------------------------------------------------
int main(int argc, char* argv[]) {
	if(argc != 2) {
		cerr <<"Parametros incorrectos" << endl;
		exit(1);
	}
	const int N = 10;
	GestorReserva GR;
	// Puerto donde escucha el proceso servidor
    int SERVER_PORT = atoi(argv[1]);
    thread cliente[N];
    int client_fd[N];

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);

	// Bind
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
    	cerr << "Error en el bind: " + string(strerror(errno)) + "\n";
		exit(1);
	}

	// Listen
  int max_connections = N;
	int error_code = socket.Listen(max_connections);
	if (error_code == -1) {
		cerr << "Error en el listen: " + string(strerror(errno)) + "\n";
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	for (int i=0; i<max_connections; i++) {
		// Accept
		client_fd[i] = socket.Accept();

		if(client_fd[i] == -1) {
			cerr << "Error en el accept: " + string(strerror(errno)) + "\n";
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		cliente[i] = thread(&servCliente, ref(socket), client_fd[i], ref(GR));
		cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
	}

	//¿Qué pasa si algún thread acaba inesperadamente?
	for (int i=0; i<max_connections; i++) {
		cliente[i].join();
	}

    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1) {
    	cerr << "Error cerrando el socket del servidor: " + string(strerror(errno)) + " aceptado" + "\n";
	}

	// Despedida
	cout << "Bye bye" << endl;

    return error_code;
}
//-------------------------------------------------------------
