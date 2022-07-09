#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

#include "room.h"

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

char* assignNewUserToRoom(char *username, int initialPoint, char *roomCode) // append to file txt
{
    // roomCode[strlen(roomCode)-2] = '\0';
    // strcat(roomCode, ".txt");
    // printf("\n%s", roomCode);
    if(!checkValidRoomCode(roomCode)) {
        return "roomNotFound";
    }
    
    char* location = (char *)malloc(sizeof(char)*MAXLINE);
    strcpy(location, "../room/");
    strcat(location, roomCode);
    FILE *fp = fopen(location, "a");
    fprintf(fp, "%s_%d", username, initialPoint);
    fclose(fp);
    return "success";
}
Room getRoomByRoomCode(int roomCode) // get data from file "roomCode.txt"
{
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
    return results;
}