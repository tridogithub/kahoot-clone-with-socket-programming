
typedef struct
{
    char questionContent[1000]; // question: ...?
    char answer1[100];
    char answer2[100];
    char answer3[100];
    char answer4[100];
    char correctAns[100];
} Question;

typedef struct
{
    char questionSuiteId[50];  // E.g. NetPro1, DataStruc,...
    char briefDescription[50]; // something like "Question suite about Network Programming"
    char Question[50];         // limit to 50 questions per suite for simplicity
    int status;                // 1-CON; 2-FINAL. Data type can be flexibly modified, depended on DEV
} QuestionSuite;

typedef struct
{
    char listQS[20][256];
} ListQS;

QuestionSuite getQuestionSuiteByQsID(char *qsID);
int saveNewQuestion(char *questionSuiteID, Question question); //append to file
ListQS ReturnQS();
