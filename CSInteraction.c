/*
 ============================================================================
 Name        : CSInteraction.c
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


typedef struct {
	char operation;
	int a;
	int b;
} message;

///////////////////////////////

void ErrorHandler(char *errorMessage) {
	printf ("%s", errorMessage);
}

///////////////////////////////

void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	#endif
}

///////////////////////////////

int createPort(int portNumber) {

	if (portNumber < 0) {
		ErrorHandler("bad port number\n");
		return 0;
	} else {
		printf("Port: %d OK\n", portNumber);
		return portNumber;
	}
}

///////////////////////////////

int createSocket() {

	int mySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mySocket < 0) {
		ErrorHandler("Socket creation failed.\n");
		return -1;
	} else {
		printf("Socket creation succeeded!\n");
		return mySocket;
	}
}

///////////////////////////////

struct sockaddr_in initSockAddr(int family, char* addr, int port){

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = family;
	sad.sin_addr.s_addr = inet_addr(addr);
	sad.sin_port = htons(port);
	return sad;
}

///////////////////////////////

void assignSockIP(int mySocket, struct sockaddr_in sad) {

	if(bind(mySocket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		ErrorHandler("bind() failed.\n");
		closesocket(mySocket);
	} else {
		printf("bind() succeeded! \n");
	}
}

///////////////////////////////

void serverListen(int mySocket, int qlen) {

	if(listen(mySocket, qlen) < 0) {
		ErrorHandler("listen() failed.\n");
		closesocket(mySocket);
		ClearWinSock();
	} else {
		printf("Server is listening! \n");
	}
}

///////////////////////////////

int serverAccept(int mySocket) {

	struct sockaddr_in cad; //structure for the client address
	int clientSocket; //socket descriptor for the client
	int clientLen; //the size of the client address
	clientLen = sizeof(cad);

	if ((clientSocket = accept(mySocket, (struct sockaddr *)&cad, &clientLen)) < 0) {
		ErrorHandler("accept() failed.\n");
		closesocket(mySocket);
		ClearWinSock();
		return -1;
	} else {
		// clientSocket is connected to a client
		// inet_ntoa() converte un numero a 32 bit un numero in notazione puntata
		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
		return clientSocket;
	}

}

///////////////////////////////

void clientConnect(int mySocket, struct sockaddr_in sad) {

	if (connect(mySocket,(struct sockaddr *) &sad, sizeof(sad)) < 0) {
		ErrorHandler( "Failed to connect.\n" );
		closesocket(mySocket);
		ClearWinSock();
	} else {
		printf("Client connected successfully!\n");
		fflush(stdout);
	}
}

///////////////////////////////

void sending(int connectedSocket, char buf[BUFFERSIZE], int flags) {

	int stringLen = strlen(buf);
	if(send(connectedSocket, buf, stringLen, flags) != stringLen) {
		ErrorHandler("send() sent a different number of bytes than expected.\n");
		closesocket(connectedSocket);

	}
}

///////////////////////////////

void sendingStruct(int connectedSocket, message buf, int size, int flags) {

	if(send(connectedSocket, &buf, size, flags) != size) {
		ErrorHandler("send() sent a different number of bytes than expected.\n");
		closesocket(connectedSocket);
		ClearWinSock();
	}
}

///////////////////////////////

void receiving(int connectedSocket, int size, int flags, char buf[BUFFERSIZE]) {

	int bytesRcvd;
	int totalBytesRcvd = 0;

	while(totalBytesRcvd == 0) {
		if((bytesRcvd = recv(connectedSocket, buf, BUFFERSIZE - 1, flags)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely.\n");
			closesocket(connectedSocket);
		}
		totalBytesRcvd += bytesRcvd; //keep tally of total bytes
		buf[bytesRcvd] = '\0'; //add /0 so printf knows where to stop
	}

}

///////////////////////////////

void receivingStruct(int connectedSocket, int size, int flags, message *buf) {
	message recvBuf;
	int bytesRcvd;
	int totalBytesRcvd = 0;

	while(totalBytesRcvd == 0) {

		if((bytesRcvd = recv(connectedSocket, buf, size - 1, flags)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely.\n");
			closesocket(connectedSocket);
		}

		totalBytesRcvd += bytesRcvd; //keep tally of total bytes
	}
}
