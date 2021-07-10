#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <winsock.h>
#include <string.h>
#define BACKLOG_MAX 5
#define BUFFER_SIZE 128
#define EXIT_CALL_STRING "#quit"

char* ip="192.168.0.110";
unsigned short port = 8080;

int local_socket = 0,remote_socket = 0,remote_length = 0, message_length = 0;
char message_client[BUFFER_SIZE];
struct sockaddr_in local_address,remote_address;
WSADATA wsa_data;

void msg_err_exit(char *msg) {
    fprintf(stderr, msg);
    system("PAUSE");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        msg_err_exit("WSAStartup() failed\n");
        
    remote_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (remote_socket == INVALID_SOCKET) {
        WSACleanup();
        msg_err_exit("socket() failed\n");
    }
    
    memset(&remote_address, 0, sizeof (remote_address));
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr(ip);
    remote_address.sin_port = htons(port);
    printf("connecting with server %s...\n", ip);
    
    if (connect(remote_socket, (struct sockaddr *) &remote_address, sizeof (remote_address)) == SOCKET_ERROR) {
        WSACleanup();
        msg_err_exit("connect() failed\n");
    }
    
    printf("type your messages\n");
    
    do {
        memset(&message_client, 0, BUFFER_SIZE);
        printf("msg: ");
        gets(message_client);
        fflush(stdin);    
        message_length = strlen(message_client);
        //send message to server
        if (send(remote_socket, message_client, message_length, 0) == SOCKET_ERROR) {
            WSACleanup();
            closesocket(remote_socket);
            msg_err_exit("send() failed\n");
        }
	} while(strcmp(message_client, EXIT_CALL_STRING));	
	
    printf("closed\n");
    WSACleanup();
    closesocket(remote_socket);
    system("PAUSE");
    return 0;
}
