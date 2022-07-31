#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

char *login()
{
    char *buf = (char *)malloc(sizeof(char) * 50);
    char username[100];
    char password[100];
    printf("\nUsername: ");
    scanf("%s", username);
    printf("\nPassword: ");
    scanf("%s", password);

    sprintf(buf, "1-%s:%s", username, password);
    return buf;
}

char *registry()
{
    char *buf = (char *)malloc(sizeof(char) * 50);
    char username[100];
    char password[100];
    printf("\nUsername: ");
    scanf("%s", username);
    printf("\nPassword: ");
    scanf("%s", password);

    sprintf(buf, "3-%s:%s", username, password);
    return buf;
}

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

char *readSchedule(char *weekDay)
{
    char *request = (char *)malloc(sizeof(char) * MAXLINE);
    sprintf(request, "2-%s", weekDay);
    return request;
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
        int dayOfWeek = 2;
        sendline[0] = '\0';
        recvline[0] = '\0';
        printf("\n-----Menu-----");
        printf("\n1. Login");
        printf("\n2. Read schedule");
        printf("\n3. Registry");
        printf("\n4. Exit.");
        printf("\n-----Enter option:");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            strcpy(sendline, login());
            sendAndReceiveMessages(sendline, sockfd, recvline);
            memset(sendline, 0, strlen(sendline));
            memset(recvline, 0, strlen(recvline));
            break;
        case 2:
            printf("\nEnter the interested day of week(nothing for quit): ");
            fflush(stdin);
            scanf("%s", sendline);
            printf("\nSend line: %s", sendline);
            if (strcmp(sendline, "\n") == 0)
            {
                break;
            }
            strcpy(sendline, readSchedule(sendline));

            sendAndReceiveMessages(sendline, sockfd, recvline);
            memset(sendline, 0, strlen(sendline));
            memset(recvline, 0, strlen(recvline));
            break;
        case 3:
            strcpy(sendline, registry());
            sendAndReceiveMessages(sendline, sockfd, recvline);
            memset(sendline, 0, strlen(sendline));
            memset(recvline, 0, strlen(recvline));
            break;
        case 4:
            break;
        default:
            break;
        }
        if (option == 4)
        {
            break;
        }
    }

    exit(0);
}