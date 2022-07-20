#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

#include "room.h"
#include "historyAnswer.h"

#define MAXLINE 4096

int checkValidRoomCode(char *roomCode)
{
    DIR *dir;
    struct dirent *ent;
    int status = 0;
    if ((dir = opendir("../room")) != NULL)
    {
        printf("\nDir: ");
        roomCode[strlen(roomCode) - 2] = '\0';
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL)
        {
            // char filename[256];
            // strcpy(filename, ent->d_name);
            // ent->d_name[strlen(ent->d_name) - 1] = '\0';
            printf("\n%s-%d", ent->d_name, strcmp(roomCode, ent->d_name));
            if (strcmp(roomCode, ent->d_name) == 0)
            {
                printf("\nMatched");
                status = 1;
            }
        }
        closedir(dir);
    }
    return status;
}

char *assignNewUserToRoom(char *username, int initialPoint, char *roomCode) // append to file txt
{
    // roomCode[strlen(roomCode)-2] = '\0';
    // strcat(roomCode, ".txt");
    // printf("\n%s", roomCode);
    if (!checkValidRoomCode(roomCode))
    {
        return "roomNotFound";
    }

    char *location = (char *)malloc(sizeof(char) * MAXLINE);
    strcpy(location, "../room/");
    strcat(location, roomCode);
    FILE *fp = fopen(location, "a");
    fprintf(fp, "%s\n", username);
    fclose(fp);
    return "success";
}
Room getRoomByRoomCode(char *roomCode) // get data from file "roomCode.txt"
{
    char *dir = (char *)malloc(sizeof(char) * MAXLINE);
    char *results = (char *)malloc(sizeof(char) * MAXLINE);
    strcat(dir, "../room/");
    strcat(dir, roomCode);
    Room room;
    FILE *f = fopen(dir, "r");
    fscanf(f, "%s", room.questionSuiteId);
    int index = 0;
    while (fscanf(f, "%s", room.userAndPoint[index].username) != EOF)
    {
        room.userAndPoint[index].totalPoint = 0;
        index++;
    }
    room.numberOfUser = index;
    fclose(f);
    return room;
}

void CreateRoom(char *roomCode, char *questionSuiteID)
{
    char *dir = (char *)malloc(sizeof(char) * MAXLINE);
    strcat(dir, "../room/");
    strcat(dir, roomCode);
    FILE *f = fopen(dir, "w");
    fprintf(f, "%s", questionSuiteID);
    fclose(f);
}

void rand_room_code(int length, char *result)
{
    time_t t;
    srand((unsigned)time(&t));
    int i, rand_int;
    char char_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&quot";

    for (i = 0; i < length; i++)
    {
        result[i] = char_set[rand() % sizeof(char_set)];
    }
    result[length] = 0;
    printf("\nGenerated room code: %s", result);
}

char *rankOfRoom(char *roomCode)
{
    char *dir = (char *)malloc(sizeof(char) * MAXLINE);
    char *results = (char *)malloc(sizeof(char) * MAXLINE);
    strcat(dir, "../room/");
    strcat(dir, roomCode);
    FILE *f = fopen(dir, "r+");

    strcat(results, "getRankTable_");
    char *QSID;
    fscanf(f, "%s", QSID);
    while ((!feof(f)))
    {
        char *line;
        fscanf(f, "%s", line);
        strcat(results, line);
        strcat(results, "_");
    }
    fclose(f);
    return results;
}

char *getRankTable(char *roomCode)
{
    Room room = getRoomByRoomCode(roomCode);
    for (int i = 0; i < room.numberOfUser; i++)
    {
        int currentPoint = getCurrentPoint(room.userAndPoint[i].username);
        room.userAndPoint[i].totalPoint = currentPoint;
    }

    // sorting
    for (int i = 0; i < room.numberOfUser - 1; i++)
    {
        for (int j = i + 1; j < room.numberOfUser; j++)
        {
            if (room.userAndPoint[i].totalPoint < room.userAndPoint[j].totalPoint ||
                (room.userAndPoint[i].totalPoint == room.userAndPoint[j].totalPoint && strcmp(room.userAndPoint[i].username, room.userAndPoint[j].username) > 0))
            {
                UserPoint tmp = room.userAndPoint[i];
                room.userAndPoint[i] = room.userAndPoint[j];
                room.userAndPoint[j] = tmp;
            }
        }
    }

    char *results = (char *)malloc(sizeof(char) * 500);
    memset(results, 0, 500);
    strcat(results, "getRankTable");
    for (int i = 0; i < room.numberOfUser; i++)
    {
        char *tmp = (char *)malloc(sizeof(char) * 100);
        sprintf(tmp, "_%s:%d", room.userAndPoint[i].username, room.userAndPoint[i].totalPoint);
        strcat(results, tmp);
    }
    return results;
}