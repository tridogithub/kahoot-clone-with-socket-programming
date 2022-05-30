
typedef struct
{
    char questionSuiteId[50];
    char username[50];
    int totalPoint; //initial is 0
} Room;

int assignNewUserToRoom(char *username, int initialPoint); //append to file txt
Room getRoomByRoomCode(int roomCode); //get data from file "roomCode.txt"

