/*
 ============================================================================
 Name        : Server.c
 Author      : Eleonora Ghizzota
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h> // for atoi()
#include <ctype.h>
#include <string.h>
#include "CSInteraction.h"
#define BUFFERSIZE 512
#define PROTOPORT 27015 // default protocol port number
#define QLEN 6 // size of request queue
#define FLAGS 0

typedef struct {
	char operation;
	int a;
	int b;
} message;

int main() {

	int port;
	port = createPort(PROTOPORT);

#if defined WIN32

	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);

	if(iResult != NO_ERROR) {
		printf("Error at WSASturtup\n");
		printf("A usable WinSock DLL cannot be found");
		return -1;
	}

#endif

	int SSocket = createSocket();
	struct sockaddr_in sad = initSockAddr(AF_INET, "127.0.0.1", port);
	assignSockIP(SSocket, sad);
	serverListen(SSocket, QLEN);

	int serviceSocket;
	char inputString[BUFFERSIZE] = "Connessione avvenuta";
	char answer[BUFFERSIZE];
	message buf;
	int result;

	while (1) {
		printf(" \n");
		printf( "Waiting for a client to connect...\n");

		serviceSocket = serverAccept(SSocket); //socket di servizio

		sending(serviceSocket, inputString, FLAGS);

// Ricevere la struct

		receivingStruct(serviceSocket, BUFFERSIZE, FLAGS, &buf);

		switch(buf.operation) {
		case 'A':
		case 'a':
				result = buf.a + buf.b;
				sprintf(answer, "Risultato dell'addizione: %d \n", result);
				break;

		case 'S':
		case 's':
				result = buf.a - buf.b;
				sprintf(answer, "Risultato della sottrazione: %d \n", result);
				break;

		case 'M':
		case 'm':
				result = buf.a * buf.b;
				sprintf(answer, "Risultato della moltiplicazione: %d \n", result);
				break;

		case 'D':
		case 'd':
				result = buf.a / buf.b;
				sprintf(answer, "Risultato della divisione: %d \n", result);
				break;

		default:
			sprintf(answer, "TERMINE PROCESSO CLIENT \n");
			break;
		}

		sending(serviceSocket, answer, FLAGS);
	}

}
