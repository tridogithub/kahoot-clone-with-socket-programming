#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "questionSuite.h"

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
