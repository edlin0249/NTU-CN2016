#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fd;
void build_connection();
int fsize(FILE *fptr);
int check_tag(char *tag);

void build_connection(){
	fd = socket(AF_INET , SOCK_STREAM , 0);
	fprintf(stderr, "fd = %d\n", fd);
	if(fd < 0){
		fprintf(stderr, "socket fail\n");
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(23);  //server's port
	//convert ptt.cc to its ip_addr
	struct hostent *he;
    struct in_addr **addr_list;
    char hostname[] = "ptt.cc";
    char ip[100];
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        fprintf(stderr, "gethostbyname");
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(int i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
    }
    server.sin_addr.s_addr = inet_addr(ip);

    //Connect to remote server
    if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        fprintf(stderr, "connect failed. Error");
    }
    return;
}

int fsize(FILE *fptr){
	fseek(fptr, 0, SEEK_END);
	int sz_file = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
	return sz_file;
}

int check_tag(char *tag){
	if(!strcmp(tag, "<") || !strcmp(tag, "<I") || !strcmp(tag, "<ID") || !strcmp(tag, "<ID>")|| !strcmp(tag, "<P") 
		|| !strcmp(tag, "<PA") || !strcmp(tag, "<PAS") || !strcmp(tag, "<PASS") || !strcmp(tag, "<PASS>")
		|| !strcmp(tag, "<B") || !strcmp(tag, "<BO") || !strcmp(tag, "<BOA") || !strcmp(tag, "<BOAR") || !strcmp(tag, "<BOARD")
		|| !strcmp(tag, "<BOARD>") || !strcmp(tag, "<P") || !strcmp(tag, "<P>") || !strcmp(tag, "<C") || !strcmp(tag, "<CO")
		|| !strcmp(tag, "<CON") || !strcmp(tag, "<CONT") || !strcmp(tag, "<CONTE") || !strcmp(tag, "<CONTEN") || !strcmp(tag, "<CONTENT")
		|| !strcmp(tag, "<CONTENT>") || !strcmp(tag, "<E") || !strcmp(tag, "<EX") || !strcmp(tag, "<EXI") || !strcmp(tag, "<EXIT")
		|| !strcmp(tag, "<EXIT>")){
		return 1;
	}
	else{
		return 0;
	}

}

int main(void){
	//build_connection();
	FILE* fptr = fopen("P_input.txt", "r");
	char buff[1];
	//analyse
	//int sz_file = fsize(fptr);
	//char *buff = (char*)malloc(sizeof(char)*sz_file);
	//if(fread(buff, sizeof(char), sz_file, fptr) < 0){
	//	fprintf(stderr, "fread error\n");
	//}
	char tag[100];
	memset(tag, 0, sizeof(tag));
	int idx = 0;
	int flag = 1;
	int test = 0;
	while(fread(buff, sizeof(char), 1, fptr) > 0){
		if(flag){
			build_connection();
			flag = 0;
		}
		tag[idx] = buff[0];
		if(!strcmp(tag, "\n")){
			test++;
			fprintf(stderr, "test = %d\n", test);
		}
		fprintf(stderr, "tag = %s\n", tag);
		if(!strcmp(tag, "<ID>")){   //post ID
			fprintf(stderr, "enter ID\n");
			//dprintf(fd, "\b\b\b\b\b\b\b\b\b\b");
			char data[100];
			memset(data, 0, sizeof(data));
			int idx_data = 0;
			while(fread(buff, sizeof(char), 1, fptr) > 0){
				if(idx_data == 5){
					fprintf(stderr, "idx_data == 5, data = %s\n", data);
					//if(!strcmp(data, "</ID>")){
					//	fprintf(stderr, "finish ID\n");
						//char tmp[] = "\r\n";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
					//	write(fd, "\r\n\0", 3);
					//	sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
					//	break;
					//}
					//else{
					write(fd, &data[0], 1);
					sleep(1);
					for(int i = 1; i < idx_data; i++){
						data[i-1] = data[i];
					}
					data[idx_data-1] = buff[0];
					if(!strcmp(data, "</ID>")){
						fprintf(stderr, "finish ID\n");
						//char tmp[] = "\r\n";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
						write(fd, "\r\n\0", 3);
						sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
						break;
					}
					//}
				}
				else{
					data[idx_data] = buff[0];
					idx_data++;
				}
			}
			memset(tag, 0, sizeof(tag));
			idx = 0;
		}
		else if(!strcmp(tag, "<PASS>")){  //post password
			fprintf(stderr, "enter PASS\n");
			char data[100];
			memset(data, 0, sizeof(data));
			int idx_data = 0;
			while(fread(buff, sizeof(char), 1, fptr) > 0){
				if(idx_data == 7){
					fprintf(stderr, "idx_data == 7, data = %s\n", data);
					//if(!strcmp(data, "</PASS>")){
					//	fprintf(stderr, "finish pass\n");
						//char tmp[] = "\r\n\r\n";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
					//	write(fd, "\r\n\0", 3);
					//	write(fd, "\r\n\0", 3);
					//	sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
					//	break;
					//}
					//else{
					write(fd, &data[0], 1);
					sleep(1);
					for(int i = 1; i < idx_data; i++){
						data[i-1] = data[i];
					}
					data[idx_data-1] = buff[0];
					if(!strcmp(data, "</PASS>")){
						fprintf(stderr, "finish pass\n");
						//char tmp[] = "\r\n\r\n";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
						write(fd, "\r\n\0", 3);
						write(fd, "\r\n\0", 3);
						sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
						break;
					}
					//}
				}
				else{
					data[idx_data] = buff[0];
					idx_data++;
				}
			}
			//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
			memset(tag, 0, sizeof(tag));
			idx = 0;
		}
		else if(!strcmp(tag, "<BOARD>")){   //enter board
			//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
			fprintf(stderr, "enter BOARD\n");
			//char enter_board[] = "s";
			write(fd, "s", 1);
			sleep(1);			
			char data[100];
			memset(data, 0, sizeof(data));
			int idx_data = 0;
			while(fread(buff, sizeof(char), 1, fptr) > 0){
				if(idx_data == 8){
					fprintf(stderr, "idx_data == 8, data = %s\n", data);
					//if(!strcmp(data, "</BOARD>")){
					//	fprintf(stderr, "finish board\n");
						//char tmp[] = "\r\r";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
					//	write(fd, "\r\n\0", 3);
					//	write(fd, "\r\n\0", 3);
					//	sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
					//	break;
					//}
					//else{
					write(fd, &data[0], 1);
					sleep(1);
					for(int i = 1; i < idx_data; i++){
						data[i-1] = data[i];
					}
					data[idx_data-1] = buff[0];
					if(!strcmp(data, "</BOARD>")){
						fprintf(stderr, "finish board\n");
						//char tmp[] = "\r\r";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
						write(fd, "\r\n\0", 3);
						write(fd, "\r\n\0", 3);
						sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
						break;
					}
					//}
				}
				else{
					data[idx_data] = buff[0];
					idx_data++;
				}
			}
			//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
			memset(tag, 0, sizeof(tag));
			idx = 0;
		}
		else if(!strcmp(tag, "<P>")){   //post title
			//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
			fprintf(stderr, "enter P\n");
			char post[1] = {16};
			write(fd, post, 1);
			write(fd, "\r\n\0", 3);
			sleep(1);			
			char data[100];
			memset(data, 0, sizeof(data));
			int idx_data = 0;
			while(fread(buff, sizeof(char), 1, fptr) > 0){
				if(idx_data == 4){
					fprintf(stderr, "idx_data == 4, data = %s\n", data);
					//if(!strcmp(data, "</P>")){
					//	fprintf(stderr, "finish post\n");
						//char tmp[] = "\r";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
					//	write(fd, "\r\n\0", 3);
					//	sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
					//	break;
					//}
					//else{
					write(fd, &data[0], 1);
					sleep(1);
					for(int i = 1; i < idx_data; i++){
						data[i-1] = data[i];
					}
					data[idx_data-1] = buff[0];
					if(!strcmp(data, "</P>")){
						fprintf(stderr, "finish post\n");
						//char tmp[] = "\r";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
						write(fd, "\r\n\0", 3);
						sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
						break;
					}
					//}
				}
				else{
					data[idx_data] = buff[0];
					idx_data++;
				}
			}
			memset(tag, 0, sizeof(tag));
			idx = 0;
		}
		else if(!strcmp(tag, "<CONTENT>")){		//post content
			fprintf(stderr, "enter content\n");	
			char data[100];
			memset(data, 0, sizeof(data));
			int idx_data = 0;
			while(fread(buff, sizeof(char), 1, fptr) > 0){
				if(idx_data == 10){
					fprintf(stderr, "idx_data == 10, data = %s\n", data);
					//if(!strcmp(data, "</CONTENT>")){
					//	fprintf(stderr, "finish content\n");
						//char tmp[] = "^xs\r";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
					//	char ctrlX[1] = {24};
					//	write(fd, ctrlX, 1);
					//	write(fd, "s", 1);
					//	write(fd, "\r\n\0", 3);
					//	write(fd, "\r\n\0", 3);
					//	sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
					//	break;
					//}
					//else{
					write(fd, &data[0], 1);
					sleep(1);
					for(int i = 1; i < idx_data; i++){
						data[i-1] = data[i];
					}
					data[idx_data-1] = buff[0];
					if(!strcmp(data, "</CONTENT>")){
						fprintf(stderr, "finish content\n");
						//char tmp[] = "^xs\r";
						//fprintf(stderr, "strlen(tmp) = %d\n", strlen(tmp));
						char ctrlX[1] = {24};
						write(fd, ctrlX, 1);
						write(fd, "s", 1);
						write(fd, "\r\n\0", 3);
						write(fd, "\r\n\0", 3);
						sleep(1);
						//dprintf(fd, "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03");
						//sleep(1);
						break;
					}
					//}
				}
				else{
					data[idx_data] = buff[0];
					idx_data++;
				}
			}
			memset(tag, 0, sizeof(tag));
			idx = 0;
		}
		else if(!strcmp(tag, "<EXIT>")){   //exit
			//char exit[] = "qg\ry\r";
			fprintf(stderr, "logout fd = %d\n", fd);
			write(fd, "q", 1);
			write(fd, "g", 1);
			write(fd, "\r\n\0", 3);
			sleep(1);
			write(fd, "y", 1);
			write(fd, "\r\n\0", 3);
			sleep(1);
			memset(tag, 0, sizeof(tag));
			idx = 0;
			flag = 1;
			close(fd);
			sleep(1);
		}
		if(check_tag(tag)){
			idx++;
		}
		else{
			memset(tag, 0, sizeof(tag));
			idx = 0;
		}
	}
	fprintf(stderr, "fd = %d\n", fd);
	//close(fd);
	fclose(fptr);

	return 0;
}
