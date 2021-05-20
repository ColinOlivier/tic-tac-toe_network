#include<stdio.h>
#include<iostream>
#include<WS2tcpip.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define N 1000

void main()
{
	printf("\t\t\t\tBienvenue sur \"T\'Chat\".\n\n");

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	SOCKET sock;
	SOCKADDR_IN sin;
	char ip[15];
	int port;

	//initialisation et saisi des informations
	cout << "Ip : ";	// 192.168.1.140
	cin >> ip;

	cout << "\nPort : ";
	cin >> port;

	sin.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &sin.sin_addr.s_addr);
	sin.sin_port = htons(port);

	//creation de la socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//connexion a la socket
	if (connect(sock, (SOCKADDR*)&sin, sizeof(sin)))
	{
		cout << "La connection a echoue\n";

		system("PAUSE");
		exit(0);
	}

	printf("\t\t\t\tOuverture du \"T\'Chat\".\n\n");


	char buffer[N];
	memset(buffer, 0, sizeof(buffer));

	int err = 0;

	while (err > -1)
	{
		cout << "A envoyer : ";
		cin >> buffer;
		err = send(sock, buffer, sizeof(buffer), 0);

		cout<< "Recu :";
		err = recv(sock, buffer, sizeof(buffer), 0);
		cout << buffer << "\n";
	}
	closesocket(sock);

	cout << "Connection terminée\n\n";

	WSACleanup();

}