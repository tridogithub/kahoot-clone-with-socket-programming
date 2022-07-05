#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

void sendAndReceiveMessages(char *sendline, int sockfd, char *recvline)
{
    send(sockfd, sendline, strlen(sendline), 0);

    if (recv(sockfd, recvline, MAXLINE, 0) == 0)
    {
        // error: server terminated prematurely
        perror("\nThe server terminated prematurely");
        exit(4);
    }
    printf("\n%s", "String received from the server: ");
    fputs(recvline, stdout);
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    // basic check of the arguments
    // additional checks can be inserted
    if (argc != 2)
    {
        perror("Usage: TCPClient <IP address of the server");
        exit(1);
    }

    // Create a socket for the client
    // If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    // Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT); // convert to big-endian order

    // Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(3);
    }
    // if (recv(sockfd, recvline, MAXLINE, 0) == 0)
    // {
    //     // error: server terminated prematurely
    //     perror("The server terminated prematurely");
    //     exit(4);
    // }
    // printf("\n%s", "String received from the server: ");
    // fputs(recvline, stdout);
    // memset(recvline, 0, strlen(recvline));

    int option;
    while (1)
    {

        printf("\nEnter message with predefined format sending to server: ");
        fflush(stdin);
        scanf("%s", sendline);
        if (strcmp(sendline, "\n") == 0)
        {
            break;
        }

        sendAndReceiveMessages(sendline, sockfd, recvline);
        memset(sendline, 0, strlen(sendline));
        memset(recvline, 0, strlen(recvline));
        break;
    }

    exit(0);
}