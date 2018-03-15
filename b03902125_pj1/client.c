#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent
#include <pthread.h>
#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define NAMEMAX 4096
int socket_desc;
int packet_num;
struct timeval timeout;

int main(int argc , char *argv[]){
     	sscanf(argv[2], "%d", &packet_num);
	//printf("packet_num = %d\n", packet_num);
	timeout.tv_sec = 0L;
	sscanf(argv[4], "%ld", &timeout.tv_usec);
	//printf("timeout.tv_sec = %ld, timeout.tv_usec = %ld\n", timeout.tv_sec, timeout.tv_usec);
    	//Create socket
    	//socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	//printf("socket_desc = %d\n", socket_desc);
	int j = 5;
	int t;
	int isip = 1;
	while(j < argc){
		socket_desc = socket(AF_INET , SOCK_STREAM , 0);
		//int new_socket
		struct sockaddr_in server;
		//server_object server;
		char hostname[NAMEMAX] = {0};
		unsigned short portnum = 0;
		int meet = 0;
		for(int k = 0; k < strlen(argv[j]); k++){
			if(argv[j][k] == ':'){
				//portnum = argv[j]+(k+1);
				meet = 1;
				t = (k+1);
				continue;
			}
			if(meet == 0){
				hostname[k] = argv[j][k];
				if(isdigit(argv[j][k]) || argv[j][k] == '.'){
					isip = 1;
				}
				else{
					isip = 0;
				}
			}
			if(meet == 1){
				portnum = portnum*10 + (argv[j][k] - '0');
			}
		}
    		char ip[100];
		memset(ip, 0, sizeof(ip));
    		struct hostent *he;
    		struct in_addr **addr_list;
		struct in_addr hipaddr;
         	if(isip){
			inet_aton(hostname, &hipaddr);
			he = gethostbyaddr(&hipaddr, 4, AF_INET);
		}
		else{
    			he = gethostbyname( hostname );
		}
		//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
		addr_list = (struct in_addr **) he->h_addr_list;
    		for(int i = 0; addr_list[i] != NULL; i++){
    			//Return the first one;
        		strcpy(ip , inet_ntoa(*addr_list[i]) );
    		}

    		//printf("%s resolved to : %s\n" , hostname , ip);
		//strcpy(server.server_ip, ip);
		//printf("port = %u\n", portnum);
		//server.server_portportnum;
    		server.sin_addr.s_addr = inet_addr(ip);	
    		server.sin_family = AF_INET;
    		server.sin_port = htons(portnum);
		//thread start
		//pthread_t ntid;
		//pthread_create(&ntid, NULL, request, &server);
		struct timeval start;
		struct timeval end;
		struct timeval thistimeout = timeout;
		int num = 0;
		//Connect to remote server
    		int test_con = connect(socket_desc , (struct sockaddr *)&server , sizeof(server));
		/*socklen_t len = sizeof(server);
		if (getsockname(socket_desc, (struct sockaddr *)&server, &len) == -1)
    			perror("getsockname");
		else
    			printf("port number %d\n", ntohs(server.sin_port));*/
		//printf("test_con = %d\n", test_con);
		//printf("test_con = %d\n", test_con);	
    		//puts("Connected\n");
    		//Send some data
		//while-loop begin
		//int overtime = 0;
		while(((num < packet_num && packet_num != 0) || (packet_num == 0)) && thistimeout.tv_usec >= 0){
			char msg[200];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%d", num);
			long diff;
        		gettimeofday(&start,NULL);
			ssize_t write_s;
			write_s = write(socket_desc, msg, sizeof(msg));
        		//puts("write failed");
			assert(num <= INT_MAX);
			//Receive a reply from the server
			char buf[2000];
			ssize_t read_s;
			read_s = read(socket_desc, buf, sizeof(buf));
			//printf("buf = %s\n", buf);
			gettimeofday(&end,NULL);
			diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
			thistimeout.tv_usec -= diff;
			//printf("thistimeout.tv_usec = %ld\n", thistimeout.tv_usec);
			//printf("thedifference is %ldmsec\n",diff);
			//printf("buf = %s\n", buf);
    			//puts("Reply received\n");
			printf("recv from %s:%hu, seq = %s, RTT = %ld msec\n", ip, portnum, buf, diff);		
			fflush(stdout);
			num++;
		}
		if(thistimeout.tv_usec < 0){
			printf("timeout when connect to %s:%hu, seq = %d\n", ip, portnum, num);
			fflush(stdout);
		}
		close(socket_desc);
		sleep(1);
		j++;
     	}
    	return 0;
}
