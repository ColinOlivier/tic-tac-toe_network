/////////////////////////////////////////////////////////////////////////////
// Creation 09/12/2003                                               
// 
// 
//                         WSAEVENSELECT.C
//                         ---------------
// 
// 
// Sylvain MARECHAL - sylvain.marechal1@libertysurf.fr
/////////////////////////////////////////////////////////////////////////////
// 
// Ce programme créé 3 sockets : 
//  La première socket écoute le premier port de libre, la 2° se connecte
//  dessus, ce qui crée donc une 3° socket.
//  Après avoir échangé un paquet de données, les connexions sont fermées
// 
/////////////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#define	SOCKET_ERRNO	WSAGetLastError()


/////////////////////////////////////////////////////////////////////////////
//
//        LISTENFIRSTFREEPORT
/////////////////////////////////////////////////////////////////////////////
//
//  DESCRIPTION
//       --ListenFirstFreePort--
//
//	Creation de la premiere socket qui ecoute le premier port
//	de libre
//
//  ARGUMENTS
//        Argument1: int * pnPort
//  RETOUR/RESULTAT
//        static SOCKET 
//  REMARQUE
//  Rev 09/12/2003 
//////////////////////////////////////////////////////////////////////////////
static SOCKET ListenFirstFreePort( int * pnPort )
{
    struct sockaddr_in addr;
    int len = sizeof(addr);    
    SOCKET hSocket;

    // Premiere socket
    hSocket = socket( PF_INET, SOCK_STREAM, 0 );
    if( hSocket == INVALID_SOCKET )
    {
	printf( "socket() error %d\n", SOCKET_ERRNO );
	exit(1);
    }

    // On ecoute le premier port de libre
    addr.sin_family = AF_INET ;
    addr.sin_addr.s_addr = htonl (INADDR_ANY);
    addr.sin_port = htons ((unsigned short)*pnPort ); // 0 = le premier de libre
    if ( bind( hSocket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
    {
	printf( "bind() error %d\n", SOCKET_ERRNO );
	exit(1);
    }
    if ( listen( hSocket, 100) == SOCKET_ERROR )
    {
	printf( "listen() error %d\n", SOCKET_ERRNO );
	exit(1);
    }

    // On recupere la valeur du port
    if( getsockname( hSocket, (struct sockaddr *)&addr, &len) == SOCKET_ERROR )
    {
	printf( "listen() error %d\n", SOCKET_ERRNO );
	exit(1);
    }
    *pnPort = ntohs( addr.sin_port );

    return hSocket;
}



/////////////////////////////////////////////////////////////////////////////
//
//        CONNECTTOPORT
/////////////////////////////////////////////////////////////////////////////
//
//  DESCRIPTION
//       --ConnectToPort--
//
//	Creation de la 2° socket qui va se connecter sur la premiere
//
//  ARGUMENTS
//        Argument1: int nPort
//  RETOUR/RESULTAT
//        SOCKET 
//  REMARQUE
//  Rev 09/12/2003 
//////////////////////////////////////////////////////////////////////////////
SOCKET ConnectToPort( int nPort )
{
    struct sockaddr_in addr;
    SOCKET hSocket;
    u_long arg; int err;

    // Deuxieme socket
    hSocket = socket( PF_INET, SOCK_STREAM, 0 );
    if( hSocket == INVALID_SOCKET )
    {
	printf( "socket() error %d\n", SOCKET_ERRNO );
	exit(1);
    }

    // On passe en mode non bloquant
    arg = 1;
    err = ioctlsocket( hSocket, FIONBIO, &arg );
    if( err == SOCKET_ERROR )
    {
	printf( "ioctlsocket() : Error n %d\n", SOCKET_ERRNO );
	exit(1);
    }

    // On se connecte en local
    addr.sin_family = AF_INET ;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons ((unsigned short)nPort );
    if( connect( hSocket, (struct sockaddr *)&addr, sizeof(addr) ) == SOCKET_ERROR )
    {
	// En mode non bloquant, on aura jours l'erreur
	// WSAEWOULDBLOCK qui n'en est pas une
	if( SOCKET_ERRNO != WSAEWOULDBLOCK )
	{
	    printf( "connect() error (%d)\n", SOCKET_ERRNO );
	    return -1;
	}
    }

    return hSocket;
}



void main()
{
    WSADATA stack_info;
    SOCKET ahSocket[3];
    WSAEVENT ahEvents[3];
    int rc;
    int nListenPort = 0;
    
    // On initialise Winsock
    WSAStartup(MAKEWORD(2,0), &stack_info) ;
    
    // 3 evenements pour les 3 socket
    ahEvents[0] = WSACreateEvent();
    ahEvents[1] = WSACreateEvent();
    ahEvents[2] = WSACreateEvent();
    
    // Premiere socket qui ecoute
    ahSocket[0] = ListenFirstFreePort( &nListenPort );
    rc = WSAEventSelect(ahSocket[0], ahEvents[0], FD_ACCEPT );
    if( rc == SOCKET_ERROR )
    {
	printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO );
	exit(1);
    }
    
    // 2° socket qui se connecte sur la premiere
    ahSocket[1]= ConnectToPort(nListenPort);
    rc = WSAEventSelect(ahSocket[1], ahEvents[1], FD_CONNECT );
    if( rc == SOCKET_ERROR )
    {
	printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO );
	exit(1);
    }
    
    
    // On boucle 
    while (TRUE) 
    { 
        DWORD dwEvent;
	WSANETWORKEVENTS NetworkEvents;
	
	// On attend que quelquechose arrive.
	// Typiquement, on va commencer par recevoir la connexion
	// puis ensuite, on sera notifie pour lire les donnees
        dwEvent = WSAWaitForMultipleEvents( 3, ahEvents, FALSE, WSA_INFINITE, FALSE); 
        switch (dwEvent) 
	{ 
	case WSA_WAIT_FAILED: 
	    printf("WSAEventSelect: %d\n", WSAGetLastError()); 
	    break; 
	case WAIT_IO_COMPLETION: 
	case WSA_WAIT_TIMEOUT: 
	    break; 
	    
	default: 
	    // On deduit la socket du numero d'evenement
	    dwEvent -= WSA_WAIT_EVENT_0; 
	    // => hSocket = ahSocket[dwEvent];
	    
	    // On recherche 
	    if (SOCKET_ERROR == WSAEnumNetworkEvents(ahSocket[dwEvent], ahEvents[dwEvent], &NetworkEvents)) 
	    { 
		printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n",  
		    WSAGetLastError(), NetworkEvents.lNetworkEvents); 
		NetworkEvents.lNetworkEvents = 0; 
	    } 
	    else  
	    { 
		
		if (FD_CONNECT & NetworkEvents.lNetworkEvents) 
		{
		    // La connexion est OK
		    printf( "FD_CONNECT ok (dwEvent=%d)\n", dwEvent );
		    
		    // On va recevoir un paquet de donnee
		    rc = WSAEventSelect(ahSocket[dwEvent], ahEvents[dwEvent], FD_CLOSE | FD_READ );
		    if( rc == SOCKET_ERROR )
		    {
			printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO );
			exit(1);
		    }
		}
		if (FD_CLOSE   & NetworkEvents.lNetworkEvents) 
		{
		    // Seule la socket 2 va recevoir la notification de fermeture
		    printf( "FD_CLOSE ok (dwEvent=%d)\n", dwEvent );

		    printf( "press a key to exit\n" );
		    getch();
		    
		    // On pourrait faire plus propre :-)
		    exit(0);
		}
		if (FD_READ & NetworkEvents.lNetworkEvents) 
		{
		    char szBuffer[256]; int cbRecv;

		    // Seule la socket 1 s'attend a recevoir un paquet de donnees
		    printf( "FD_READ ok (dwEvent=%d)\n", dwEvent );

		    // On lit le paquet
		    cbRecv = recv( ahSocket[dwEvent], szBuffer, sizeof(szBuffer) - 1, 0 );
		    if( cbRecv <= 0 )
		    {
			printf( "recv() error %d\n", SOCKET_ERRNO );
			exit(1);
		    }

		    // On ecrit ce paquet (On remet le 0 au cas ou le paquet
		    // ait ete coupe en 2 - je sais, ca n'arrivera jamais en local)
		    szBuffer[cbRecv] = 0;
		    printf( "socket %d : '%s'\n", dwEvent, szBuffer );

		    // Maintenant on ferme
		    closesocket( ahSocket[dwEvent] );
		}
		if (FD_WRITE & NetworkEvents.lNetworkEvents) 
		{
		    char szBuffer[256]; int cbBuffer, cbSend;
		    // il doit s'agir de la 2° socket, puisque c'est la seule 
		    // qui demande quand elle pourra ecrire.
		    // Comme on est la, c'est qu'on doit pouvoir ecrire
		    printf( "FD_WRITE ok (dwEvent=%d)\n", dwEvent );

		    // On envoie la chaine avec le 0 de fin
		    cbBuffer = sprintf( szBuffer, "Coucou depuis la socket %d\n", dwEvent );
		    cbSend = send( ahSocket[dwEvent], szBuffer, cbBuffer + 1, 0 );

		    // En socket non bloquante, il est possible que cbSend < cbBuffer
		    // en particulier en charge. Il faut donc verifier cela et buffeuriser
		    // au cas ou. Ici, c'est un exemple donc on ne le fait pas
		    if( cbSend != cbBuffer + 1 )
		    {
			printf( "send() error %d\n", SOCKET_ERRNO );
			exit(1);
		    }


		}
		if (FD_ACCEPT & NetworkEvents.lNetworkEvents) 
		{
		    struct sockaddr_in addr;
		    int len;
		    
		    // On doit avoir dwEvent=0
		    printf( "accept ok (dwEvent=%d)\n", dwEvent );
		    len = sizeof( addr );
		    
		    // On accepte la connexion (3° socket => ahSocket[2] )
		    ahSocket[2] = accept(ahSocket[dwEvent], (struct sockaddr *)&addr, &len);
		    
		    // On demande à etre prevenu qd on pourra ecrire de la
		    // donnee sur cette nouvelle socket
		    rc = WSAEventSelect(ahSocket[2], ahEvents[2], FD_CLOSE | FD_WRITE );
		    if( rc == SOCKET_ERROR )
		    {
			printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO );
			exit(1);
		    }
		}
	    }
	} 
    }
    WSACloseEvent( ahEvents[0] );
    WSACloseEvent( ahEvents[1] );
    WSACloseEvent( ahEvents[2] );
}
