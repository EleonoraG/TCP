/*
 ============================================================================
 Name        : EClient.c
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
#define PROTOPORT 27015 // Numero di porta di default
#define FLAGS 0

typedef struct {
	char operation;
	int a;
	int b;
} message;

int main(void) {

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

	int CSocket = createSocket();
	struct sockaddr_in cad = initSockAddr(AF_INET, "127.0.0.1", port);
	clientConnect(CSocket, cad);

	char buf[BUFFERSIZE];
	printf("Received: \n"); //setup to print the echoed string
	receiving(CSocket, BUFFERSIZE, FLAGS, buf);
	printf("%s \n", buf);

// Inviare una struct

	message inputString;

	printf("Scegliere un'operazione aritmetica (A, S, M, D): ");
	fflush(stdout);
	fgets(buf, BUFFERSIZE, stdin);
	inputString.operation = buf[0];

	printf("Scegliere il primo operatore intero: ");
	fgets(buf, BUFFERSIZE, stdin);
	inputString.a = strtol(buf, NULL, 10);

	printf("Scegliere il secondo operatore intero: ");
	fgets(buf, BUFFERSIZE, stdin);
	inputString.b = strtol(buf, NULL, 10);

	sendingStruct(CSocket, inputString, BUFFERSIZE, FLAGS);

// Ricevere il risultato

	char buf2[BUFFERSIZE];
	printf("Received: \n"); //setup to print the echoed string
	receiving(CSocket, BUFFERSIZE, FLAGS, buf2);

	if (buf2[0] == 'T') {
		printf("Chiusura connessione \n");
	} else {
		printf("%s \n", buf2);
	}

	system("pause");
	closesocket(CSocket);
	ClearWinSock();

}
