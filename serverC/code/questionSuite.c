#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "questionSuite.h"

#define location "../question-suite/"

QuestionSuite getQuestionSuiteByQsID(char *qsID)
{
    QuestionSuite newQSSuite;
    strcpy(newQSSuite.questionSuiteId, qsID);
    FILE *f = fopen(qsID, "r");

    Question listQ[50];
    int i = 0;
    char *brief;
    fscanf(f, "%s", brief);
    strcpy(newQSSuite.briefDescription, brief);
    while (!(feof(f)))
    {
        char *inputQ;
        fscanf(f, "%s", inputQ);
        char *questionID = strtok(inputQ, "_");
        strcpy(listQ[i].questionContent, strtok(NULL, ":"));
        strcpy(listQ[i].answer1, strtok(NULL, ":"));
        strcpy(listQ[i].answer2, strtok(NULL, ":"));
        strcpy(listQ[i].answer3, strtok(NULL, ":"));
        strcpy(listQ[i].answer4, strtok(NULL, ":"));
        strcpy(listQ[i].correctAns, strtok(NULL, ":"));
        i++;
    }
    memcpy(newQSSuite.Question, listQ, sizeof(listQ));
    return newQSSuite;
}

ListQS ReturnQS(int *count)
{
    DIR *dir;
    struct dirent *ent;
    ListQS listQS;
    if ((dir = opendir("../question-suite")) != NULL)
    {
        printf("\nDir: ");
        /* print all the files and directories within directory */
        int i = 0;
        while ((ent = readdir(dir)) != NULL)
        {
            // char filename[256];
            // strcpy(filename, ent->d_name);
            if (ent->d_name[0] != 46)
            {
                strcpy(listQS.listQS[i], ent->d_name);
                i++;
                printf("\n%d", i);
            }
        }
        *count = i;
        closedir(dir);
    }

    return listQS;
}

int saveNewQuestion(char *questionSuiteID, Question question) // append to file
{
}

char *getQuestionByQID(char *quesitonSuiteID, int questionID)
{

    char filePath[100] = "../question-suite/";
    strcat(filePath, quesitonSuiteID);

    FILE *fp = fopen(filePath, "r");
    char *result = (char *)malloc(sizeof(char) * 4096);
    int count = 0;
    char line[500];
    while (fgets(line, sizeof line, fp) != NULL) /* read a line */
    {
        if (count == questionID)
        {
            // printf("\n%s", line);
            strcpy(result, line);
            break;
        }
        else
        {
            count++;
        }
    }
    fclose(fp);
    return result;
}

char *getQuestionSuiteIDByRoomCode(char *roomCode)
{
    printf("\n...");
    char filePath[100] = "../room/";
    strcat(filePath, roomCode);

    FILE *fp = fopen(filePath, "r");
    char *result = (char *)malloc(sizeof(char) * 50);
    // char line[50];
    fscanf(fp, "%s", result); /* read a line */
    printf("\n%s---", result);
    // strcat(result, line);
    // result[strlen(result) - 2] = '\0';
    fclose(fp);
    return result;
}
