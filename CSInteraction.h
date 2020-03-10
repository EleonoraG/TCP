/*
 * CSInteraction.h
 *
 *  Created on: 25 ott 2019
 *      Author: Eleonora
 */

#ifndef CSINTERACTION_H_
#define CSINTERACTION_H_

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

typedef struct message Message;

void ErrorHandler();
void ClearWinSock();
int createPort();
int createSocket();
struct sockaddr_in initSockAddr();
void assignSockIP ();
void serverListen();
int serverAccept();
void clientConnect();
void sending();
void sendingStruct();
void receiving();
void receivingStruct();


#endif /* CSINTERACTION_H_ */
