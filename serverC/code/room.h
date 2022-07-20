
typedef struct
{
    char username[50];
    int totalPoint; // initial is 0
} UserPoint;

typedef struct
{
    char questionSuiteId[50];
    int numberOfUser;
    UserPoint userAndPoint[100]; // temporary limitation to 100 users
} Room;

char *assignNewUserToRoom(char *username, int initialPoint, char *roomCode); // append to file txt
Room getRoomByRoomCode(char *roomCode);                                      // get data from file "roomCode.txt"
void rand_room_code(int length, char *result);
void CreateRoom(char *roomCode, char *questionSuiteID);
char *rankOfRoom(char *roomCode);
char *getRankTable(char *roomCode);
