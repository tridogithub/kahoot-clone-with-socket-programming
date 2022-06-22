#include <stdio.h>

#include <questionSuite.h>

QuestionSuite getQuestionSuiteByQsID(char *qsID)
{
    QuestionSuite newQSSuite;
    strcpy(newQSSuite.questionSuiteId, qsID);
    FILE *f = fopen(&qsID, "r");

    Question listQ[50];
    int i = 0;
    char* brief;
    fscanf(f,"%s",brief);
    strcpy(newQSSuite.briefDescription,brief)
    while(!(feof(f))){
        char* inputQ;
        fscanf(f,"%s",inputQ);
        char* questionID = strtok(inputQ,"_");
        strcpy(listQ[i].questionContent,strtok(NULL,":"));
        strcpy(listQ[i].answer1,strtok(NULL,":"));
        strcpy(listQ[i].answer2,strtok(NULL,":"));
        strcpy(listQ[i].answer3,strtok(NULL,":"));
        strcpy(listQ[i].answer4,strtok(NULL,":"));
        strcpy(listQ[i].correctAns,strtok(NULL,":"));
        i++;
    }
    newQSSuite.Question = listQ;
    return newQSSuite;

}

char *ReturnQS(){
    DIR *dir;
    struct dirent *ent;
    char *listQS[20];
    if ((dir = opendir ("/home/phankhanh658/kahoot-clone-with-socket-programming/serverC/question-suite")) != NULL) {
    /* print all the files and directories within directory */
    int i=0;
    while ((ent = readdir (dir)) != NULL) {
        strcpy(listQS[i],ent->d_name);
        i++;
    }
    closedir (dir);
    return listQS;
    } else {
    return 0;
    }
}

int saveNewQuestion(char *questionSuiteID, Question question) // append to file
{
}
