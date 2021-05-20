#include<stdio.h>	
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define N 1000

void main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0),&wsa);

	SOCKET server;
	SOCKET sock;

	SOCKADDR_IN sinserv;	
	SOCKADDR_IN sin;		

	int port;
	//initialisation et saisi des informations
	printf("Port : ");
	scanf("%i",&port);

	sinserv.sin_family=AF_INET;	
	sinserv.sin_addr.s_addr=INADDR_ANY;
	sinserv.sin_port=htons(port);

	//creation de la socket
	server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	//la socket est configuré pour écouter l'adresse et le port saisi
	bind(server,(SOCKADDR*)&sinserv,sizeof(sinserv));

	//pas de file d'attente
	listen(server,0);

	char buffer[N];
	int sinsize;
	int err=0;

	while(1)
	{
		sinsize=sizeof(sin);
		//acceptation ou non des appels
		if((sock=accept(server,(SOCKADDR*)&sin,&sinsize))!=INVALID_SOCKET)
		{
			while(err>-1)
			{
				memset(buffer,0,sizeof(buffer));
				getline((char*)buffer, N);
				err=send(sock,buffer,sizeof(buffer),0);

				memset(buffer,0,sizeof(buffer));
				recv(sock,buffer,sizeof(buffer),0);
				printf("%s\n",buffer);
			}
			closesocket(sock);
		}
	}
	closesocket(server);
	WSACleanup();

}
