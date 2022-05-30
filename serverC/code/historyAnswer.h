
typedef struct
{
    char username[50]; // correspond to a unique filename txt
    int questionId;    //[1,2,...,50]
    int answer;        //[1,2,3,4]
    int isCorrect;
} HistoryAnswer;

HistoryAnswer *getHistoryAnswerByUsername(char *username);
int saveNewHistoryAnswerToFile(char *username, HistoryAnswer newHistoryAnswer);
