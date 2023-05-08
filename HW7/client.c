#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
//Pranav Yalamala
//I pledge my honor that I have abdied by the Stevens Honor System

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    float message;
    float server_reply;
    int port = atoi(argv[1]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
	while(1)
	{
		printf("Enter float number: ");
		scanf("%f" , &message);
		
		if( send(sock , &message , sizeof(float) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		
		if( recv(sock , &server_reply , sizeof(float), 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		puts("Server Float Sum:");
		printf("%f\n", server_reply);
	}
	
	close(sock);
	return 0;
}