#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include <netinet/in.h>
#include <sys/time.h>
int sv_socket_desc;

int main(int argc , char *argv[]){
	int new_socket , c;
	struct sockaddr_in server , client;     
	//Create socket
	sv_socket_desc = socket(AF_INET , SOCK_STREAM , 0); 
	//printf("sv_socket_desc = %d\n", sv_socket_desc);
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	unsigned short portnum;
	sscanf(argv[1], "%hu", &portnum);
	//printf("portnum = %hu\n", portnum);
	server.sin_port = htons(portnum);
	//server.sin_port = htons( 8888 );
	//printf("server.sin_port = %hu\n", server.sin_port);
	//Bind
	bind(sv_socket_desc,(struct sockaddr *)&server , sizeof(server));
	//Listen
	//printf("server.sin_port = %hu\n", server.sin_port);
	listen(sv_socket_desc , 3);
	//struct sockaddr_in sin;
	/*socklen_t len = sizeof(server);
	if (getsockname(sv_socket_desc, (struct sockaddr *)&server, &len) == -1)
    		perror("getsockname");
	else
    		printf("port number %d\n", ntohs(server.sin_port));*/     
	//Accept and incoming connection
	c = sizeof(client);
	while(1){
		new_socket = accept(sv_socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		//puts("Connection accepted");
		/*socklen_t len00 = sizeof(client);
		if (getsockname(new_socket, (struct sockaddr *)&client, &len00) == -1)
    			perror("getsockname");
		else
    			printf("port number %d\n", ntohs(client.sin_port));*/
		//Reply to the client
		//struct sockaddr_in newclient;
		//start thread
		ssize_t read_size = 0L;
		ssize_t write_size = 0L;
		char client_message[200];
		memset(client_message, 0, sizeof(client_message));
		//printf("newclient.sin_port = %hu\n", client.sin_port);
		unsigned short client_port_num = ntohs(client.sin_port);
		//printf("client_port_num = %hu\n", client_port_num);
		char client_addr[2000];
		//inet_ntop(AF_INET6, &(newclient.sin_addr.s_addr), client_addr, INET6_ADDRSTRLEN);
		char *clientIP = inet_ntoa(client.sin_addr);
		if(read_size = read(new_socket, client_message, sizeof(client_message)) > 0){
			printf("recv from %s:%hu, seq = %s\n", clientIP, client_port_num, client_message);
		}
		while(read_size > 0){
			//(read_size = read(sv_socket_desc, client_message, sizeof(client_message))) >= 0
			//printf("readbef\n");
			//fflush(stdin);
			//printf("read_size = %ld\n", read_size);		
			//printf("client_message = %s\n", client_message);
			//fflush(stdout);
			//printf("client_message = %s\n", client_message);
			write_size = write(new_socket, client_message, sizeof(client_message));
			//printf("client_message = %s\n", client_message);			
			//printf("write_size = %ld\n", write_size);
			if(read_size = read(new_socket, client_message, sizeof(client_message)) > 0){
				printf("recv from %s:%hu, seq = %s\n", clientIP, client_port_num, client_message);
			}
			fflush(stdout);
		}
		fflush(stdout);
		//Free the socket pointer
		close(new_socket);
		sleep(1);
	}
	return 0;
}

