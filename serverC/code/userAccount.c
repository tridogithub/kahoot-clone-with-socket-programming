#include <stdio.h>

#include <userAccount.h>

int checkAuthenticated(char *username, char *password); // check in file user-account.txt
User *getAllUser();
int registryNewUser(User newUser); // append to file