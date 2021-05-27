#include<stdio.h>
#include<iostream>
#include<WS2tcpip.h>
#include<string>
#include"Morpion.h"
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define N 1000

void main()
{

	printf("\t\t\t\tBienvenue sur Tic-Tac-Toe.\n\n");

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	SOCKET sock;
	SOCKADDR_IN sin;
	char ip[15];
	int port;

	//initialisation et saisi des informations
	cout << "Ip : ";
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

	system("cls");

	printf("\t\t\t\tOuverture du Tic-Tac-Toe.\n\n");


	char user;
	CMorpion morpion;

	memset(&user, 0, sizeof(user));

	int err = 0;

	err = recv(sock,&user, sizeof(user), 0);

	cout << "Vous etes le joueur numero " << user << endl;

	memset((char*)&morpion, 0, sizeof(&morpion));

	if (user == '1') {
		while (err > -1 && morpion.IsVainqueur() == 0)
		{
			system("cls");
			morpion.getMove(1);
			err = send(sock, (char*)&morpion, sizeof(morpion), 0);
			err = recv(sock, (char*)&morpion, sizeof(morpion), 0);
		}
	}
	else {
		while (err > -1 && morpion.IsVainqueur() == 0)
		{
			system("cls");
			morpion.print();
			err = recv(sock, (char*)&morpion, sizeof(morpion), 0);
			system("cls");
			morpion.getMove(2);
			err = send(sock, (char*)&morpion, sizeof(morpion), 0);
		}
	}

	system("cls");

	if (user == '1' && morpion.IsVainqueur() == 1 || user == '2' && morpion.IsVainqueur() == 2) {
		morpion.win();
	}
	else {
		morpion.loss();
	}
	
	closesocket(sock);

	cout << "\n\nConnection terminee\n\n";

	WSACleanup();

}