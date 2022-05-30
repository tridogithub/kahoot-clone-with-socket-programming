#include <stdlib.h>

#include <stdio.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <sys/wait.h>

#include <signal.h>

#include <string.h>

#define MAXLINE 4096 /*max text line length*/

#define SERV_PORT 3000 /*port*/

#define LISTENQ 8 /*maximum number of client connections*/

typedef struct Course

{

    char class[100];

    char courseId[100];

    char courseName[100];

    char description[500];

    char weekDay[10];

    char amOrPm[10];

    int startTime;

    int endTime;

    char week[50];

    char room[20];

} Course;

typedef struct Registration

{

    char studentId[100];

    char class[100];

} Registration;

Course courses[100];

int courseIndex = 0;

Registration registrations[100];

int registrationIndex = 0;

char *currentStudentId;

int authenticated = 0;

char roomSchedule[12][5][20];

int checkAuthentication(char *username, char *password)

{

    printf("\n%s", username);

    printf("\n%s", password);

    FILE *fp = fopen("user_account.txt", "r");

    char buf[1024];

    while (fgets(buf, sizeof(buf), fp) != NULL)

    {

        char *token = strtok(buf, " ");

        if (strcmp(username, token) == 0)

        {

            token = strtok(NULL, " ");

            token[strlen(token) - 1] = '\0';

            printf("\n%d", strcmp(password, token));

            if (strcmp(password, token) == 0)

            {

                printf("\nOK");

                fclose(fp);

                return 1;
            }
        }
    }

    fclose(fp);

    return 0;
}

void login(char *username, char *password, char *results)

{

    if (checkAuthentication(username, password) == 1)

    {

        printf("\nAuthenticated!");

        authenticated = 1;

        strcpy(currentStudentId, username);

        strcpy(results, "Authenticated!");
    }

    else

    {

        printf("\nUnauthenticated!");

        authenticated = 0;

        strcpy(results, "Unauthenticated!");
    }
}

// read daily schedule

char *getWeekDay(int dayOfWeek)

{

    switch (dayOfWeek)

    {

    case 2:

        return "Monday";

    case 3:

        return "Tuesday";

    case 4:

        return "Wednesday";

    case 5:

        return "Thursday";

    case 6:

        return "Friday";

    default:

        return "NULL";
    }
}

char *getTimeOfDay(int timeOfDay)

{

    if (timeOfDay == 1)

    {

        return "Morning";
    }

    else if (timeOfDay == 2)

    {

        return "Afternoon";
    }

    else

    {

        return "NULL";
    }
}

char *getStringFromInteger(int integer)

{

    char *result = (char *)malloc(sizeof(char) * 10);

    sprintf(result, "%d", integer);

    return result;
}

void getArrayOfTokens(char *description, char *weekDay, char *amOrPm,

                      int *start, int *end, char *week, char *room)

{

    char stringTokens[20][100];

    char index = 0;

    char *delim = ",";

    char *token = strtok(description, delim);

    while (token != NULL)

    {

        strcpy(stringTokens[index], token);

        index++;

        token = strtok(NULL, delim);
    }

    int startTime = atoi(stringTokens[0]);

    // printf("\nStartTime: %d", startTime);

    strcpy(weekDay, getWeekDay(startTime / 100));

    strcpy(amOrPm, getTimeOfDay((startTime / 10) % 10));

    int finishTime = atoi(stringTokens[1]);

    *start = startTime % 10;

    *end = finishTime % 10;

    strcpy(week, "");

    for (int i = 0; i < index - 1; i++)

    {

        if (i != 0 && i != 1)

        {

            strcat(week, stringTokens[i]);

            if (i != index - 2)

            {

                strcat(week, ",");
            }
        }
    }

    strcpy(room, stringTokens[index - 1]);

    room[strlen(room) - 1] = '\0';
}

void readCourseData()

{

    FILE *fp = fopen("course_schedule.txt", "r");

    char buf[1024];

    char *delim = " ";

    while (fgets(buf, sizeof(buf), fp) != NULL)

    {

        Course newCourse;

        // sscanf(buf, "%s\t%s\t%s\t%s", newCourse.class, newCourse.courseId, newCourse.courseName, newCourse.description);

        char *token = strtok(buf, delim);

        strcpy(newCourse.class, token);

        token = strtok(NULL, delim);

        strcpy(newCourse.courseId, token);

        token = strtok(NULL, delim);

        strcpy(newCourse.courseName, token);

        token = strtok(NULL, delim);

        token[strlen(token) - 1] = '\0';

        strcpy(newCourse.description, token);

        getArrayOfTokens(newCourse.description, newCourse.weekDay, newCourse.amOrPm, &newCourse.startTime, &newCourse.endTime, newCourse.week, newCourse.room);

        courses[courseIndex] = newCourse;

        courseIndex++;
    }

    fclose(fp);
}

void readRegistrationData()

{

    FILE *fp = fopen("student_registration.txt", "r");

    char buf[1024];

    char *delim = " ";

    while (fgets(buf, sizeof(buf), fp) != NULL)

    {

        Registration newRegistration;

        char *token = strtok(buf, delim);

        strcpy(newRegistration.studentId, token);

        token = strtok(NULL, delim);

        token[strlen(token) - 1] = '\0';

        strcpy(newRegistration.class, token);

        registrations[registrationIndex] = newRegistration;

        registrationIndex++;
    }

    fclose(fp);
}

void printDailySchedule(char *weekDay, char *classId, char *results)

{

    char tmp[MAXLINE];

    for (int i = 0; i < courseIndex; i++)

    {

        if (strcmp(courses[i].class, classId) == 0 && strcmp(weekDay, courses[i].weekDay) == 0)

        {

            printf("\n%-10s%-30s%-10s%-10s%-d-%-8d%-25s%-s", courses[i].courseId, courses[i].courseName, weekDay, courses[i].amOrPm, courses[i].startTime, courses[i].endTime,

                   courses[i].week, courses[i].room);

            sprintf(tmp, "\n%-10s%-30s%-10s%-10s%-d-%-8d%-25s%-s", courses[i].courseId, courses[i].courseName, weekDay, courses[i].amOrPm, courses[i].startTime, courses[i].endTime,

                    courses[i].week, courses[i].room);

            strcat(results, tmp);
        }
    }
}

void readDailySchedule(char *weekDay, char *results)

{

    if (!authenticated)

    {

        printf("\nYou are not logged in!");

        sprintf(results, "\nYou are not logged in!");
    }

    else

    {

        sprintf(results, "\n%-10s%-30s%-10s%-10s%-10s%-25s%-s", "Code", "|Course", "|Week Day", "|AM/PM", "|Period", "|Week", "|Room");

        printf("\n%-10s%-30s%-10s%-10s%-10s%-25s%-s", "Code", "|Course", "|Week Day", "|AM/PM", "|Period", "|Week", "|Room");

        for (int i = 0; i < registrationIndex; i++)

        {

            if (strcmp(registrations[i].studentId, currentStudentId) == 0)

            {

                printDailySchedule(weekDay, registrations[i].class, results);
            }
        }
    }
}

// read weekly schedule

int getDayOfWeek(char *weekDay)

{

    if (strcmp(weekDay, "Monday") == 0)

    {

        return 2;
    }

    else if (strcmp(weekDay, "Tuesday") == 0)

    {

        return 3;
    }

    else if (strcmp(weekDay, "Wednesday") == 0)

    {

        return 4;
    }

    else if (strcmp(weekDay, "Thursday") == 0)

    {

        return 5;
    }

    else if (strcmp(weekDay, "Friday") == 0)

    {

        return 6;
    }

    else

    {

        return 0;
    }
}

void readWeeklySchedule(char *results)

{

    if (!authenticated)

    {

        printf("\nYou are not logged in!");

        sprintf(results, "\nYou are not logged in!");
    }

    else

    {

        for (int i = 0; i < registrationIndex; i++)

        {

            if (strcmp(registrations[i].studentId, currentStudentId) == 0)

            {

                for (int j = 0; j < courseIndex; j++)

                {

                    if (strcmp(courses[j].class, registrations[i].class) == 0)

                    {

                        printf("Student: %s - COurse: %s", currentStudentId, courses[j].class);

                        int dayOfWeek = getDayOfWeek(courses[j].weekDay);

                        for (int k = courses[j].startTime; k <= courses[j].endTime; k++)

                        {

                            if (strcmp(courses[j].amOrPm, "Afternoon") == 0)

                            {

                                strcpy(roomSchedule[k + 6][dayOfWeek - 2], courses[j].room);
                            }

                            else

                            {

                                strcpy(roomSchedule[k][dayOfWeek - 2], courses[j].room);
                            }
                        }
                    }
                }
            }
        }

        printf("\n--%-11s%-11s%-11s%-11s%-11s", "|Monday", "|Tuesday", "|Wednesday", "|Thursday", "|Friday");

        printf("\n");

        sprintf(results, "\n--%-11s%-11s%-11s%-11s%-11s", "|Monday", "|Tuesday", "|Wednesday", "|Thursday", "|Friday");

        strcat(results, "\n");

        char tmp[MAXLINE];

        for (int i = 0; i < 12; i++)

        {

            printf("%-2d", i);

            sprintf(tmp, "%-2d", i);

            strcat(results, tmp);

            for (int j = 0; j < 5; j++)

            {

                printf("|%-10s", roomSchedule[i][j]);

                sprintf(tmp, "|%-10s", roomSchedule[i][j]);

                strcat(results, tmp);
            }

            printf("\n");

            strcat(results, "\n");
        }
    }
}

char *getResults(char *receivedString)

{

    // buf format: number-data

    //  number = 1 -> Login function

    //  number = 2 -> Read schedule

    // number =3 -> registry new user

    printf("\nReceived string: %s", receivedString);

    char *results = (char *)malloc(sizeof(char) * MAXLINE);

    char *token = strtok(receivedString, "-");

    if (strcmp(token, "1") == 0)

    {

        token = strtok(NULL, "-");

        char *username = strtok(token, ":");

        char *password = strtok(NULL, "");

        login(username, password, results);

        strtok(NULL, " ");
    }

    else if (strcmp(token, "2") == 0)

    {

        token = strtok(NULL, "-");

        if (strcmp(token, "All") == 0)

        {

            readWeeklySchedule(results);
        }

        else

        {

            readDailySchedule(token, results);
        }
    }
    else if (strcmp(token, "3") == 0)
    {
        token = strtok(NULL, "-");

        char *username = strtok(token, ":");

        char *password = strtok(NULL, "");
        // write new account to file
        FILE *fp = fopen("user_account.txt", "a");
        fprintf(fp, "%s %s\n", username, password);
        fclose(fp);
        strcpy(results, "Successful registration");
    }
    else

    {

        strcpy(results, " ");
    }

    return results;
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

int main(int argc, char **argv)

{

    currentStudentId = (char *)malloc(sizeof(char) * 20);

    readCourseData();

    readRegistrationData();

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