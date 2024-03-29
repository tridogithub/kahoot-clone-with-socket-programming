#include <stdio.h>
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
void sig_chld();
char *getResults(char *receivedString);

// global variable
char *currentUserId;
char *currentRoom;

int main(int argc, char **argv)

{

    currentUserId = (char *)malloc(sizeof(char) * 20);
    currentRoom = (char *)malloc(sizeof(char) * 20);
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
        token = strtok(NULL, "_");
        char *username = strtok(token, ":");
        char *password = strtok(NULL, ":");
        login(username, password, results);
        if (strcmp(results, "authenticated") == 0)
        {
            strcpy(currentUserId, username);
        }
    }
    else if (strcmp(token, REGISTER) == 0)
    {
        token = strtok(NULL, "_");
        char *username = strtok(token, ":");
        char *password = strtok(NULL, ":");
        User *newUser = (User *)malloc(sizeof(User));
        strcpy(newUser->username, username);
        strcpy(newUser->password, password);
        registryNewUser(newUser);
        strcpy(results, "success");
        // TODO implement new method for register function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, REQUESTQS) == 0)
    {
        printf("\n...");
        // TODO implement new method for request Question Suite function
        //  and return string to client saved to Variable results
        int numberOfQS = 0;
        ListQS listQS = ReturnQS(&numberOfQS);
        // printf("\n%s", listQS.listQS[0]);
        int num = numberOfQS;
        // printf("\nNum: %d", num);
        char *resultsQS = (char *)malloc(sizeof(char) * MAXLINE);
        memset(results, 0, MAXLINE);
        strcat(resultsQS, "getQS_");
        for (int i = 0; i <= num - 1; i++)
        {
            strcat(resultsQS, listQS.listQS[i]);
            strcat(resultsQS, "-");
            strcat(resultsQS, getDescriptionByQuestionSuiteID(listQS.listQS[i]));
            if (i != num - 1)
            {
                strcat(resultsQS, ":");
            }
        }
        strcpy(results, resultsQS);
    }
    else if (strcmp(token, CREATEROOM) == 0)
    {
        // TODO implement new method for Create Room function
        //  and return string to client saved to Variable results
        token = strtok(NULL, "_");
        char *roomCode = (char *)malloc(sizeof(char) * MAXLINE);
        char *ResultsCR = (char *)malloc(sizeof(char) * MAXLINE);
        strcat(ResultsCR, "getRoomCode_");
        rand_room_code(7, roomCode);
        strcpy(currentRoom, roomCode);
        strcat(ResultsCR, roomCode);
        strcpy(results, ResultsCR);

        CreateRoom(roomCode, token);
    }
    else if (strcmp(token, JOINROOM) == 0)
    {
        token = strtok(NULL, "_");
        char *status = assignNewUserToRoom(currentUserId, 0, token);
        createUserHistoryFile(currentUserId);
        strcpy(results, status);
        // TODO implement new method for Join Room function
        //  and return string to client saved to Variable results
    }
    else if (strcmp(token, REQUESTQ) == 0)
    {
        // TODO implement new method for Request Question function
        //  and return string to client saved to Variable results
        char *roomCode = strtok(NULL, "_");
        char *qsID = getQuestionSuiteIDByRoomCode(roomCode);
        char *qIDString = strtok(NULL, "_");

        qIDString[strlen(qIDString) - 2] = '\0';

        strcpy(results, "getQ_");
        strcat(results, qIDString);
        strcat(results, "_");
        printf("\n...");

        int qID = atoi(qIDString);
        printf("\n%s-%d", qsID, qID);
        char *question = (char *)malloc(sizeof(char) * 2048);
        question = getQuestionByQID(qsID, qID);
        strcat(results, question);
    }
    else if (strcmp(token, SENDANS) == 0)
    {
        // TODO implement new method for Send Answer function
        //  and return string to client saved to Variable results
        Answer answer;
        int statusInt = 0;
        char *roomCode = strtok(NULL, "_");
        // printf("\n...");
        char *content = strtok(NULL, "_");
        // printf("\n...");
        char *qID = strtok(content, ":");
        int qIDInt = atoi(qID);
        // printf("\n%s-%d", qID, qIDInt);
        char *ans = strtok(NULL, ":");
        int ansNum = atoi(ans);
        // printf("\n%s-%d", ans, ansNum);
        char *status = strtok(NULL, ":");
        // printf("\n%s", status);
        strtok(NULL, ":");
        status[strlen(status) - 2] = '\0';

        strcpy(results, "wrong");
        if (strcmp(status, "TRUE") == 0)
        {
            statusInt = 1;
            printf("\nTRUE");
            strcpy(results, "correct");
        }

        answer.questionId = qIDInt;
        answer.answer = ansNum;
        answer.isCorrect = statusInt;
        saveHistoryAns(currentUserId, answer);
    }
    else if (strcmp(token, REQUESTPOINT) == 0)
    {
        // TODO implement new method for Request Total Point function
        //  and return string to client saved to Variable results
        char *username = strtok(NULL, "_");
        char *roomCode = strtok(NULL, "_");

        int currentPoint = getCurrentPoint(username);
        sprintf(results, "%s_%d", "getTotalPoint", currentPoint);
    }
    else if (strcmp(token, REQUESTRANKTABLE) == 0)
    {
        // TODO implement new method for Rank Table function
        //  and return string to client saved to Variable results
        token = strtok(NULL, "_");
        token[strlen(token) - 2] = '\0';
        // char *roomCode = (char *)malloc(sizeof(char) * MAXLINE);
        // strcpy(roomCode, token);
        // strcpy(results, rankOfRoom(roomCode));
        strcpy(results, getRankTable(token));
    }
    else if (strcmp(token, REQUESTANSDETAIL) == 0)
    {
        // TODO implement new method for Get Ans detail function
        //  and return string to client saved to Variable results
        // token = strtok(NULL, "_");
        char *roomCode = (char *)malloc(sizeof(char) * 10);
        roomCode = strtok(NULL, "_");
        token = strtok(NULL, "_");
        int questionID = atoi(token);
        strcpy(results, getAnsDetail(roomCode, questionID));
    }
    else if (strcmp(token, REQUESTHISTORYANSWER) == 0)
    {
        char *username = strtok(NULL, "_");
        HistoryAnswer *historyAnswer = malloc(sizeof(HistoryAnswer));
        getHistoryAnswerByUsername(currentUserId, historyAnswer);
        strcat(results, "getHis");
        for (int i = 0; i < historyAnswer->numberOfAnswer; i++)
        {
            char *tmp = (char *)malloc(sizeof(char) * 100);
            sprintf(tmp, "_%d:%d:%d", historyAnswer->answer[i].questionId, historyAnswer->answer[i].answer, historyAnswer->answer[i].isCorrect);
            strcat(results, tmp);
        }
    }
    else if (strcmp(token, CREATEFILE) == 0)
    {
        char *fileName = strtok(NULL, "_");
        fileName[strlen(fileName) - 2] = '\0';
        char filePath[100] = "../question-suite/";
        strcat(filePath, fileName);
        FILE *fp = fopen(filePath, "w");
        fclose(fp);
        memset(results, 0, MAXLINE);
        strcpy(results, "success");
    }
    else if (strcmp(token, APPENDFILE) == 0)
    {
        token = strtok(NULL, "_");
        char *fileName = strtok(token, "-");
        // printf("\nFile Name: %s", fileName);
        char *content = strtok(NULL, "-");
        // printf("\nContent: %s", content);
        content[strlen(content) - 2] = '\0';

        char filePath[100] = "../question-suite/";
        strcat(filePath, fileName);
        FILE *fp = fopen(filePath, "a");
        fprintf(fp, "%s\n", content);
        fclose(fp);
        memset(results, 0, MAXLINE);
        strcpy(results, "success");
    }
    else
    {
        strcpy(results, "CODE_NOT_MATCH_ERROR");
        // TODO return error message
    }
    strcat(results, "\n");
    return results;
}