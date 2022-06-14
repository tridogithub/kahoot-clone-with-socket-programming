
typedef struct
{
    char username[50];
    char password[50];
} User;

int checkAuthenticated(char *username, char *password); //check in file user-account.txt
User *getAllUser();
int registryNewUser(User newUser); //append to file
