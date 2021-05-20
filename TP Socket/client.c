#include<stdio.h>	
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define N 1000

void main()
{
	printf("\t\t\t\tBienvenue sur \"T\'Chat\".\n\n");

	WSADATA wsa; 
	WSAStartup(MAKEWORD(2,0),&wsa);	

	SOCKET sock;	
	SOCKADDR_IN sin;
	char ip[15];
	int port;

	//initialisation et saisi des informations
	printf("Ip : ");
	scanf("%s",ip);	

	printf("%s","\nPort : ");
	scanf("%i",&port);

	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=inet_addr(ip);
	sin.sin_port=htons(port);

	//creation de la socket
	sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	//connexion a la socket
	if(connect(sock,(SOCKADDR*)&sin,sizeof(sin)))
	{
		printf("La connection a echoue\n");

		system("PAUSE");
		exit(0);
	}

	char buffer[N];
	memset(buffer,0,sizeof(buffer));

	int err=0;

	while(err>-1)
	{
		getline((char*)buffer, N);
		err=send(sock,buffer,sizeof(buffer),0);
		memset(buffer,0,sizeof(buffer));

		err=recv(sock,buffer,sizeof(buffer),0);
		printf("%s\n",buffer);
		memset(buffer,0,sizeof(buffer));
	}
	closesocket(sock);

	printf("Connection terminée\n\n");

	WSACleanup();

}
