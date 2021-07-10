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

int local_socket = 0, remote_socket = 0, remote_length = 0, message_length = 0;
char message[BUFFER_SIZE];
struct sockaddr_in local_address,remote_address;
WSADATA wsa_data;

void msg_err_exit(char *msg) {
    fprintf(stderr, msg);
    system("PAUSE");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    //start Winsock 2.0 (DLL), Only for Windows
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
    	msg_err_exit("WSAStartup() failed\n");
    	
    //creating localhost socket
    local_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (local_socket == INVALID_SOCKET) {
        WSACleanup();
        msg_err_exit("socket() failed\n");
    }
    
    fflush(stdin);
    
    memset(&local_address, 0, sizeof (local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(port);
    local_address.sin_addr.s_addr = inet_addr(ip);
    
    //connecting socket with localhost
    if (bind(local_socket, (struct sockaddr *) &local_address, sizeof (local_address)) == SOCKET_ERROR) {
        WSACleanup();
        closesocket(local_socket);
        msg_err_exit("bind() failed\n");
    }
    
    //listening connections
    if (listen(local_socket, BACKLOG_MAX) == SOCKET_ERROR) {
        WSACleanup();
        closesocket(local_socket);
        msg_err_exit("listen() failed\n");
    }

    remote_length = sizeof (remote_address);
    printf("waiting some connection...\n");
    
    remote_socket = accept(local_socket, (struct sockaddr *) &remote_address, &remote_length);
    if (remote_socket == INVALID_SOCKET) {
        WSACleanup();
        closesocket(local_socket);
        msg_err_exit("accept() failed\n");
    }
    printf("connected with %s\n", inet_ntoa(remote_address.sin_addr));
    printf("waiting messages...\n");
    do {
        memset(&message, 0, BUFFER_SIZE);
        //receive message
        message_length = recv(remote_socket, message, BUFFER_SIZE, 0);
        if (message_length == SOCKET_ERROR)
            msg_err_exit("recv() failed\n");
        //show message
        printf("%s: %s\n", inet_ntoa(remote_address.sin_addr), message);
    } while (strcmp(message, EXIT_CALL_STRING));
    
    printf("closed\n");
    WSACleanup();
    closesocket(local_socket);
    closesocket(remote_socket);
    system("PAUSE");
    return 0;
}
