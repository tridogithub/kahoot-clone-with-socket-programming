#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "historyAnswer.h"

void getHistoryAnswerByUsername(char *username, HistoryAnswer *historyAnswer)
{
    strcpy(historyAnswer->username, username);
    char filePath[100] = "../history-answer/";
    strcat(filePath, username);
    FILE *fp = fopen(filePath, "r");
    fscanf(fp, "%d", &historyAnswer->currentPoint);
    int index = 0;
    while (fscanf(fp, "%d:%d:%d", &historyAnswer->answer[index].questionId, &historyAnswer->answer[index].answer, &historyAnswer->answer[index].isCorrect) != EOF)
    {
        index++;
    }
    historyAnswer->numberOfAnswer = index;
    fclose(fp);
    // return historyAnswer;
}

int saveNewHistoryAnswerToFile(char *username, HistoryAnswer newHistoryAnswer)
{
    char filePath[100] = "../history-answer/";
    strcat(filePath, username);
    FILE *fp = fopen(filePath, "w");
    int index = 0;
    fprintf(fp, "%d\n", newHistoryAnswer.currentPoint);
    for (int i = 0; i < newHistoryAnswer.numberOfAnswer; i++)
    {
        fprintf(fp, "%d:%d:%d\n", newHistoryAnswer.answer[i].questionId, newHistoryAnswer.answer[i].answer, newHistoryAnswer.answer[i].isCorrect);
    }
    fclose(fp);
    return 1;
}

void saveHistoryAns(char *username, Answer answer)
{
    printf("\nJoined...");
    HistoryAnswer *currentHistoryAnswer = malloc(sizeof(HistoryAnswer));
    getHistoryAnswerByUsername(username, currentHistoryAnswer);
    // printf("")
    if (answer.isCorrect)
    {
        currentHistoryAnswer->currentPoint += 10;
    }
    currentHistoryAnswer->answer[currentHistoryAnswer->numberOfAnswer] = answer;
    // printf("\nQuestion ID: %d", currentHistoryAnswer->answer[currentHistoryAnswer->numberOfAnswer].questionId);
    currentHistoryAnswer->numberOfAnswer += 1;
    saveNewHistoryAnswerToFile(username, *currentHistoryAnswer);
}

int getCurrentPoint(char *username)
{
    char filePath[100] = "../history-answer/";
    printf("\n%s", username);
    strcat(filePath, username);
    FILE *fp = fopen(filePath, "r");
    int currentPoint = 0;
    fscanf(fp, "%d", &currentPoint);
    fclose(fp);
    return currentPoint;
}

void createUserHistoryFile(char *username)
{
    char filePath[100] = "../history-answer/";
    printf("\n%s", username);
    strcat(filePath, username);
    FILE *fp = fopen(filePath, "w");
    fprintf(fp, "%d\n", 0);
    fclose(fp);
}

int getAnswerOfQuestionByQuestionID(char *username, int questionID)
{
    char filePath[100] = "../history-answer/";
    printf("\n%s", username);
    strcat(filePath, username);
    FILE *fp = fopen(filePath, "r");
    char line[256];
    int count = 0;
    int answer = 0;
    while (fgets(line, sizeof line, fp) != NULL)
    {
        if(count == questionID) {
            int questionID;
            int status;
            sscanf(line, "%d:%d:%d", &questionID, &answer, &status);
            printf("\n%d:%d:%d", questionID, answer, status);
            break;
        }
        count++;
    }
    
    fclose(fp);
    return answer;
}
