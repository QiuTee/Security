/*ArucerProbe.c
An IP-based TCP client
HIT3321
This client works with the Arucer.dll keylogger located on 
the Red Hat Linux 7.3 VMWare Image version 1.5 and later. 
The server is in the home directory of the student user. 
Author: James Hamlyn-Harris
Last updated: 21-08-2013*/

/*link with
-lws2_32
*/
#include <ctype.h>
#include <sys/types.h>
#include <winsock2.h> /*networking*/
#include <stdio.h>
#include <windows.h>  /*threads*/

/*macros*/
#define SIZE sizeof(struct sockaddr_in)
#define PORT 7777

#define IP "127.0.0.1" 

/*function prototypes*/
void listener(void *);
void client(struct sockaddr_in);

/*main function for managing command-line options IP, port*/
int main(void)
{
	char address[16] = {'\0'};
	char* iaddress = address;
	struct sockaddr_in serverc = {AF_INET, htons(PORT)}; //holds address of server

	WSADATA wsaData; /*winsock*/
	serverc.sin_addr.s_addr = inet_addr(IP);
	WSAStartup(MAKEWORD(1,0), &wsaData); /*start winsock*/

	/*just for feedback to the user*/	     
    strncpy(iaddress, (char*)inet_ntoa(serverc.sin_addr), strlen((char*)inet_ntoa(serverc.sin_addr)));
	printf("Contacting server at %s on port %i\n", iaddress, ntohs(serverc.sin_port));

	client(serverc);
	
	WSACleanup();
	printf("\nPress ENTER key to quit: ");
	getchar();
	
	return 0;
}



/*Connect to the server and set up a 2-directional TCP client
* @param serverData - a struct containing the protocol, IP and port to be used
*/
void client(struct sockaddr_in serverData)
{
	HANDLE Handle_Of_Listener = 0;
	int sockfd; /*socket pointer*/
	unsigned long pid2; /*pid of listener thread*/
	int i;
	
	printf("Sending Probe to port %i... \n", PORT);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("%i", WSAGetLastError()); //10093 = WSA not started.
		exit(1);
	}
	
	if(-1 == connect((unsigned)sockfd, (struct sockaddr *)&serverData, sizeof(serverData)))
	{
		printf("%i", WSAGetLastError()); //10061 = connection refused-no service running
		printf("\tConnection refused. No service running (10061)\n");
		return;
	}
	
	/*create thread to run listener*/
	Handle_Of_Listener = CreateThread(NULL, 0, // security, stack size
	             (LPTHREAD_START_ROUTINE)listener, // start
	             (void *)sockfd, 0, &pid2); // param, creation flags, id

	if ( Handle_Of_Listener == NULL)
	{
	    printf("listener handle lost\n");
		ExitProcess((unsigned)sockfd);
	}

/*talker*/
	char areYouArucer[] ="{E2AC5089-3820-43fe-8A4D-A7028FAD8C28}" ;
	char nll = '\x00';
	char esc = '\x27';
	char mess[44] = {'\0'};
	
	//encode
	i = 0;
	mess[i++] = esc ^ 0xE5; /* 27h is the length of the command string*/
	mess[i++] = nll ^ 0xE5;
	mess[i++] = nll ^ 0xE5;
	mess[i++] = nll ^ 0xE5;

	for (i = 4; i < 44 ; i++)
	{
		mess[i] = areYouArucer[i-4] ^ 0xE5;
    }
	mess[i] = nll ^ 0xE5;

	printf("Are you Arucer?...\n");
	for (i = 0 ; i < 44 ; i++)
	{
		send((unsigned)sockfd, &mess[i], 1, 0);
		//printf("%c", mess[i-1] ^ 0xE5);
		printf(" \b"); /*add a little delay to allow the listener thread to work*/
	}

   
	return;
}


/* Create a listening thread using the same socket as the talking thread
* @param: mySocket - the socket pointer
*/
void listener(void * mySocket)
{
	/*listener*/
	char rc = '\0';
			
	while (1)
	{
		if (recv((unsigned)mySocket, &rc, 1, 0) > 0)
		{
			if (rc == '\n')
				printf("\n");
			else
				printf("%c", rc ^ 0xE5);
		}
	}
	return;
}

