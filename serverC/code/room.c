#include <stdio.h>
#include <string.h>
#include "room.h"

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