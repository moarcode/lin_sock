// Klient2.cpp : Defines the entry point for the console application.
//
 
#include "stdafx.h"
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<iostream>
 
 
 
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
 
 
 
 
#define DEFAULT_PORT 2007
#define DEFAULT_PROTO SOCK_STREAM
 
 
 
 
int main(int argc, char **argv)
{
        char Buffer[128];
        char *server_name = "localhost"; // adres mozna zmienic w linii komend
        unsigned short port = DEFAULT_PORT;
        int retval, loopflag = 0;
        int i, loopcount, maxloop = -1;
        unsigned int addr;
        int socket_type = DEFAULT_PROTO;
        struct sockaddr_in server;
        struct hostent *hp;
        WSADATA wsaData;
        SOCKET  conn_socket;
 
        // zmienne linii komend
        if (argc >1)
        {
                for (i = 1; i<argc; i++)
                {
                        if ((argv[i][0] == '-') || (argv[i][0] == '/'))
                        {
                                switch (tolower(argv[i][1]))
                                {
                                case 'p': //TCP czy UDP
                                        if (!_stricmp(argv[i + 1], "TCP"))
                                                socket_type = SOCK_STREAM;
                                        else if (!_stricmp(argv[i + 1], "UDP"))
                                                socket_type = SOCK_DGRAM;
                                        i++;
                                        break;
                                case 'n':
                                        server_name = argv[++i];
                                        break;
                                case 'e':
                                        port = atoi(argv[++i]);
                                        break;
                                case 'l':
                                        loopflag = 1;
                                        if (argv[i + 1]) {
                                                if (argv[i + 1][0] != '-')
                                                        maxloop = atoi(argv[i + 1]);
                                        }
                                        else
                                                maxloop = -1;
                                        i++;
                                        break;
                                }
                        }
                }
        }
 
        //sprawdzanie bledow
        if ((retval = WSAStartup(0x202, &wsaData)) != 0)
        {
                fprintf(stderr, "Klient: WSAStartup() failed with error %d\n", retval);
                WSACleanup();
                return -1;
        }
        else
                printf("Klient: WSAStartup() is OK.\n");
       
        if (isalpha(server_name[0]))
        {  
                hp = gethostbyname(server_name);
        }
        else
        {
                addr = inet_addr(server_name);
                hp = gethostbyaddr((char *)&addr, 4, AF_INET);
        }
        if (hp == NULL)
        {
                fprintf(stderr, "Klient: Cannot resolve address \"%s\": Error %d\n", server_name, WSAGetLastError());
                WSACleanup();
                exit(1);
        }
        else
                printf("Klient: gethostbyaddr() is OK.\n");
       
        memset(&server, 0, sizeof(server));
        memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
        server.sin_family = hp->h_addrtype;
        server.sin_port = htons(port);
 
        conn_socket = socket(AF_INET, socket_type, 0); /* Open a socket */
        if (conn_socket <0)
        {
                fprintf(stderr, "Klient: Error Opening socket: Error %d\n", WSAGetLastError());
                WSACleanup();
                return -1;
        }
        else
                printf("Klient: socket() is OK.\n");
 
        printf("Klient: Klient connecting to: %s.\n", hp->h_name);
        if (connect(conn_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        {
                fprintf(stderr, "Klient: connect() failed: %d\n", WSAGetLastError());
                WSACleanup();
                return -1;
        }
 
        else
                printf("Klient: connect() is OK.\n");
 
        //wysylanie stringa
        loopcount = 0;
        while (1)
        {
                wsprintf(Buffer,"portup", loopcount++);
                retval = send(conn_socket, Buffer, sizeof(Buffer), 0);
                if (retval == SOCKET_ERROR)
                {
                        fprintf(stderr, "Klient: send() failed: error %d.\n", WSAGetLastError());
                        WSACleanup();
                        return -1;
                }
                else
                        printf("Klient: send() is OK.\n");
                printf("Klient: Sent data \"%s\"\n", Buffer);
               
                system("\client2.exe -n 213.92.205.101 -e 8889");
 
                retval = recv(conn_socket, Buffer, sizeof(Buffer), 0);
                if (retval == SOCKET_ERROR)
                {
                        fprintf(stderr, "Klient: recv() failed: error %d.\n", WSAGetLastError());
                        closesocket(conn_socket);
                        WSACleanup();
                        return -1;
                }
                else
                        printf("Klient: recv() is OK.\n");
 
 
                if (retval == 0)
                {
                        printf("Klient: Server closed connection.\n");
                        closesocket(conn_socket);
                        WSACleanup();
                        return -1;
                }
 
                printf("Klient: Received %d bytes, data \"%s\" from server. port %d\n", retval, Buffer,port);
                if (!loopflag)
                {
                        printf("Klient: Terminating connection...\n");
                        break;
                }
                else
                {
                        if ((loopcount >= maxloop) && (maxloop >0))
                                break;
                }
        }
        closesocket(conn_socket);
        WSACleanup();
        gets(Buffer);
        return 0;
}
