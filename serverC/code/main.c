#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#include "historyAnswer.h"
#include "questionSuite.h"
#include "room.h"
#include "userAccount.h"
#include "constant.h"

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections*/

// method definition
void sig_cld();
char *getResults(char *receivedString);

// global variable
char *currentUserId;

int main(int argc, char **argv)

{

    currentUserId = (char *)malloc(sizeof(char) * 20);
    // TODO: read question suite datas

    int listenfd, connfd, n;
    pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;
    char results[MAXLINE];

    // Create a socket for the soclet
    // If sockfd<0 there was an error in the creation of the socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    // preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    // bind the socket
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // listen to the socket by creating a connection queue, then wait for clients
    listen(listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;)
    {
        clilen = sizeof(cliaddr);
        // accept a connection
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        printf("%s%d-%d\n", "Received request... ", connfd, cliaddr.sin_port);

        if ((childpid = fork()) == 0)
        { // if it’s 0, it’s child process
            printf("%s\n", "Child created for dealing with client requests");
            // close listening socket
            close(listenfd);
            while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
            {
                printf("%s", "\nString received from the client:");
                puts(buf);
                strcpy(results, getResults(buf));
                printf("\nResults: %s", results);
                send(connfd, results, sizeof(results), 0);
                memset(buf, 0, MAXLINE);
                memset(results, 0, MAXLINE);
            }
            if (n < 0)
                printf("%s\n", "Read error");
            exit(0);
        }
        signal(SIGCHLD, sig_chld);
        // close socket of the server
        close(connfd);
    }
}

void sig_chld()
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("child %d terminated\n", pid);
    }
    return;
}

char *getResults(char *receivedString)
{
    printf("\nReceived string: %s", receivedString);

    char *results = (char *)malloc(sizeof(char) * MAXLINE);

    char *token = strtok(receivedString, "_");

    if (strcmp(token, LOGIN) == 0)
    {
        // TODO implement new method for login function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, REGISTER) == 0)
    {
        // TODO implement new method for register function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, REQUESTQS) == 0)
    {
        // TODO implement new method for request Question Suite function
        //  and return string to client saved to Variable results
        char *listQS = ReturnQS();
        int num = sizeof listQS / sizeof *listQS;
        char *resultsQS = (char *)malloc(sizeof(char) * MAXLINE);
        strcat(resultsQS,"getQS_")
        for(int i = 0; i<num-1; i++){
            strcat(resultsQS,listQS[i]);
            if(i!=num-1){
                strcat(resultsQS,":");
            }
        }
        strcpy(results,resultsQS);
        
    }
    else if (strcmp(token, CREATEROOM) == 0)
    {
        // TODO implement new method for Create Room function
        //  and return string to client saved to Variable results
        token = strtok(NULL,"_");
        char* roomCode = (char *)malloc(sizeof(char) * MAXLINE);
        rand_room_code(7,roomCode);
        CreateRoom(roomCode);
        char* ResultsCR = (char *)malloc(sizeof(char) * MAXLINE);
        strcat(ResultsCR,"getRoomCode_");
        strcat(ResultsCR,roomCode);
        strcpy(results,ResultsCR);
    }
    else if (strcmp(token, JOINROOM) == 0)
    {
        // TODO implement new method for Join Room function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, REQUESTQ) == 0)
    {
        // TODO implement new method for Request Question function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, SENDANS) == 0)
    {
        // TODO implement new method for Send Answer function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, REQUESTPOINT) == 0)
    {
        // TODO implement new method for Request Total Point function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, REQUESTRANKTABLE) == 0)
    {
        // TODO implement new method for Rank Table function
        //  and return string to client saved to Variable results
        token = strtok(NULL,"_");
        char* roomCode = (char *)malloc(sizeof(char) * MAXLINE);
        strcpy(roomCode,token);
        strcpy(results,rankOfRoom(roomCode));
    }
    else
    {
        // TODO return error message
    }

    return results;
}