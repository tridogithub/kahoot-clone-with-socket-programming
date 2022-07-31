#ifndef _HISTORY_ANSWER
#define _HISTORY_ANSWER
typedef struct
{
    int questionId; //[1,2,...,50]
    int answer;     //[1,2,3,4]
    int isCorrect;
} Answer;

typedef struct
{
    char username[50]; // correspond to a unique filename txt
    Answer answer[50];
    int numberOfAnswer;
    int currentPoint;
} HistoryAnswer;

void getHistoryAnswerByUsername(char *username, HistoryAnswer *historyAnswer);
int saveNewHistoryAnswerToFile(char *username, HistoryAnswer newHistoryAnswer);
void saveHistoryAns(char *username, Answer answer);
void createUserHistoryFile(char *username);
int getCurrentPoint(char *username);
int getAnswerOfQuestionByQuestionID(char *username, int questionID);
#endif