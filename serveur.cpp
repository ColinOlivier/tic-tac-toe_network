#include<stdio.h>
#include<iostream>
#include<WS2tcpip.h>
#include <string>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define N 1000

void main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	SOCKET server;

	SOCKET sock1;
	SOCKET sock2;


	SOCKADDR_IN sinserv;
	SOCKADDR_IN sin;

	int port;
	//initialisation et saisi des informations

	cout << "\nPort : ";
	cin >> port;

	sinserv.sin_family = AF_INET;
	sinserv.sin_addr.s_addr = INADDR_ANY;
	sinserv.sin_port = htons(port);

	//creation de la socket
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//la socket est configur� pour �couter l'adresse et le port saisi
	bind(server, (SOCKADDR*)&sinserv, sizeof(sinserv));

	//pas de file d'attente
	listen(server, 0);

	string buffer;
	int sinsize;
	int err1 = 0;
	int err2 = 0;

	while (1)
	{
		sinsize = sizeof(sin);
		sock1 = accept(server, (SOCKADDR*)&sin, &sinsize);
		sock2 = accept(server, (SOCKADDR*)&sin, &sinsize);
		if (sock1 != INVALID_SOCKET && sock2 != INVALID_SOCKET)
		{
			buffer[0] = '1';
			err1 = send(sock1, (char*)&buffer, sizeof(buffer), 0);
			buffer[0] = '2';
			err2 = send(sock2, (char*)&buffer, sizeof(buffer), 0);

			while (err1 > -1 && err2 > -1)
			{
				recv(sock1, (char*)&buffer, sizeof(buffer), 0);
				cout << "USER_1 : " << buffer << "\n";
				err2 = send(sock2, (char*)&buffer, sizeof(buffer), 0);
				recv(sock2, (char*)&buffer, sizeof(buffer), 0);
				cout << "USER_2 : " << buffer << "\n";
				err1 = send(sock1, (char*)&buffer, sizeof(buffer), 0);
			}
			closesocket(sock1);
			closesocket(sock2);
		}
	}
	closesocket(server);
	WSACleanup();

}