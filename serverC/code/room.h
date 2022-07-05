
typedef struct 
{
    char username[50];
    int totalPoint; //initial is 0
} UserPoint;

typedef struct
{
    char questionSuiteId[50];
    UserPoint userAndPoint[100]; //temporary limitation to 100 users
} Room;

void assignNewUserToRoom(char *username, int initialPoint, char *token); //append to file txt
Room getRoomByRoomCode(int roomCode); //get data from file "roomCode.txt"
void rand_room_code(int length, char *result);
void CreateRoom(char* roomCode);
char* rankOfRoom(char* roomCode);
