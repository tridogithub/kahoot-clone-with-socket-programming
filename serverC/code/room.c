#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "room.h"

#define MAXLINE 4096

void assignNewUserToRoom(char *username, int initialPoint, char *token) // append to file txt
{
    strcat(token, ".txt");
    FILE *fp = fopen(token, "a");
    char buf[1024];
    fprintf(fp, "%s_%d", username, initialPoint);
    fclose(fp);
}
Room getRoomByRoomCode(int roomCode) // get data from file "roomCode.txt"
{
}

void CreateRoom(char* roomCode){
    char* dir = (char *)malloc(sizeof(char) * MAXLINE);
    strcat(dir,"/home/phankhanh658/kahoot-clone-with-socket-programming/serverC/room/");
    strcat(dir,roomCode);
    FILE* f = fopen(dir,"w+");
    fclose(dir);
}


void rand_room_code(int length, char *result) {
    int i, rand_int;
    char char_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&quot";
 
    for (i = 0; i <length; i++) {
        result[i] = char_set[rand() % sizeof(char_set)];
    }
    result[length] = 0;
}

char* rankOfRoom(char* roomCode){
    char* dir = (char *)malloc(sizeof(char) * MAXLINE);
    char* results = (char *)malloc(sizeof(char) * MAXLINE);
    strcat(dir,"/home/phankhanh658/kahoot-clone-with-socket-programming/serverC/room/");
    strcat(dir,roomCode);
    FILE* f = fopen(dir,"r+");

    strcat(results,"getRankTable_");
    char* QSID;
    fscanf(f,"%s",QSID);
    while ((!feof(f))){
        char* line;
        fscanf(f,"%s",line);
        strcat(results,line);
        strcat(results,"_");
    }
    return results;
    
}