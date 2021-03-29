#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
 *
 */

#ifdef HISTORY_LIST
#define HISTORY_LIST
typedef struct Node Node;
struct Node {
    char command[MAX_INPUT_SIZE];
    Node * Next;
    Node * Prev;
};


/*This function inserts new command to the history list*/
void add2history(Node * Position, char * line) {
    Node * temp = (Node * ) malloc(sizeof(Node));
    if (temp == NULL) {
        perror("Out of memory!!!");
        exit(0);
    }
    Position -> Next = temp;
    strcpy(temp -> command, line);
    temp -> Prev = Position;
    temp -> Next = NULL;
}

/*This function writes all the previous commands to a file named "history.txt"*/
void writeHistory(Node * List) {
    FILE * commandWritter; // File pointer
    Node * cell = List -> Next; // pointing to next cell because head cell is empty
    commandWritter = fopen("history.txt", "w");
    while (cell -> Next) {
        fprintf(commandWritter, "%s", cell -> command);
        cell = cell -> Next;
    }
    fclose(commandWritter);
}
#endif // HISTORY_LIST

char ** tokenize(char * line) {
    char ** tokens = (char ** ) malloc(MAX_NUM_TOKENS * sizeof(char * ));
    char * token = (char * ) malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for (i = 0; i < strlen(line); i++) {
        char readChar = line[i];
        if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0) {
                tokens[tokenNo] = (char * ) malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        }
        else
            token[tokenIndex++] = readChar;
    }
    free(token);
    tokens[tokenNo] = NULL;
    return tokens;
}
/* Splits the string by & and returns the array of tokens
 *
 */

char ** tokenize_and(char * line) {
    char ** tokens_and = (char ** ) malloc(MAX_NUM_TOKENS * sizeof(char * ));
    char * token_and = (char * ) malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;
    for (i = 0; i < strlen(line); i++) {
        char readChar = line[i];
        if (readChar == '&' || readChar == '\n') {
            if (readChar == '\n') {
                token_and[tokenIndex++] = '\n';
                token_and[tokenIndex] = '\0';
            }
            else if (readChar == '&') {
                token_and[tokenIndex++] = ' ';
                token_and[tokenIndex++] = '&';
                token_and[tokenIndex] = '\0';
            }
            else
                token_and[tokenIndex] = '\0';
            if (tokenIndex != 0) {
                tokens_and[tokenNo] = (char * ) malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens_and[tokenNo++], token_and);
                tokenIndex = 0;
            }
        }
        else
            token_and[tokenIndex++] = readChar;
    }

    free(token_and);
    tokens_and[tokenNo] = NULL;
    return tokens_and;
}
// function to print current directory
void printDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execCmd(char ** tokens) {
    int i = 0;
    int file;
    char* filename;
    pid_t pid;
    pid = fork();

    // Forking a child
    if (pid == -1) {
        printf("\nShell: Failed forking child");
        return;
    }
    else if (pid == 0) {
        while (tokens[++i]) { // not NULL
            if (!strcmp(tokens[i], ">")) {
                if (tokens[i+2]) {  // should be NULL
                    printf("\nShell: Wrong filename!!!");
                    return;
                }
                else if (tokens[i+1]) {  // filename
                    file = open(tokens[i+1], O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
                    if(file == -1) {
                        printf("\nShell: Could not create/edit file :(");
                        return;
                    }
                    dup2(file, STDOUT_FILENO);
                    close(file);
                    tokens[i] = NULL;   // make ">" = NULL because execvp cannot handle it
                    break;  // break the loop because we got what we wanted ;)
                }
                else
                    printf("\nShell: Missing filename");
            }
            else if (!strcmp(tokens[i], "<")) {
                if (tokens[i+2]) {  // should be NULL
                    printf("\nShell: Wrong filename!!!");
                    return;
                }
                else if (tokens[i+1]) {  // filename
                    file = open(tokens[i+1], O_RDONLY);
                    if(file == -1) {
                        printf("\nShell: File not found :(");
                        return;
                    }
                    dup2(file, STDIN_FILENO);
                    close(file);
                    tokens[i] = NULL;   // make "<" = NULL because execvp cannot handle it
                    break;  // break the loop because we got what we wanted ;)
                }
                else
                    printf("\nShell: Missing filename");
            }
        }
        if (execvp(tokens[0], tokens) < 0) {
            printf("\nShell: Incorrect command\n");
        }
        exit(0);
    }
    else { // waiting for child to terminate
        wait(NULL);
        return;
    }
}


// function to execute command in background and print child process id
void execCmd_and(char ** tokens) {

    pid_t pid;
    pid = fork();
    // Forking a child
    if (pid == -1) {
        printf("\nShell: Failed forking child");
        return;
    }
    else if (pid == 0) {

        if (execvp(tokens[0], tokens) < 0) {
            printf("\nShell: Incorrect command\n");
        }
        exit(0);
    }
    else { // waiting for child to terminate
        printf("pid: %d", pid);
        //wait(NULL);
        return;
    }
}

void execute_cmd(char line[]){
    char ** tokens_and;
    char ** tokens;
    int i;
    if (line[strlen(line) - 2] == '&') { // if a command ends with & pass to execCmd_and to exceute in background
        line[strlen(line) - 2] = '\n';
        line[strlen(line) - 1] = '\0';
        execCmd_and(tokenize(line));
        return; // return shell
    }
    tokens_and = tokenize_and(line); // tokenizing sentence based on &
    for (i = 0; tokens_and[i] != NULL; i++) { // executing each token
        if (strlen(tokens_and[i]) == 1) // if no command is entered return shell
            return;
        else {
            tokens = tokenize(tokens_and[i]); // tokenizing based on space
            if (tokens[0][0] == 'c' && tokens[0][1] == 'd' & tokens[0][2] == '\0') { // if cd command
                if (chdir(tokens[1]) != 0)
                    // so chdir will return -1
                    perror("cd failed");
            }
            else // built-in commands
                execCmd(tokens);
        }
    }
    // freeing memory allocated
    for (int j = 0; tokens[j] != NULL; j++) {
        free(tokens[j]);
    }
    free(tokens);

    for (i = 0; tokens_and[i] != NULL; i++) {
        free(tokens_and[i]);
    }
    free(tokens_and);
}

int main(int argc, char * argv[]) {
    char line[MAX_INPUT_SIZE], history[MAX_INPUT_SIZE]="";
    FILE * fp;

#ifdef HISTORY_LIST
    Node * commandList, * last, *front; // will store history
    commandList = (Node * ) malloc(sizeof(Node)); // history list
    if (commandList == NULL) {
        perror("Out of memory!!!");
        exit(0);
    }
    else {
        commandList -> Next = NULL;
        commandList -> Prev = NULL;
        last = commandList;
        front = last;
    }
#endif // HISTORY_LIST

    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            puts("File doesn't exists.");
            return -1;
        }
        while(fgets(line, MAX_INPUT_SIZE, fp)){
            execute_cmd(line);
        }
        return 0;
    }

    while (1) {
        /* BEGIN: TAKING INPUT */
        printDir();
        bzero(line, sizeof(line));
        if (argc == 2) { // batch mode
            if (fgets(line, sizeof(line), fp) == NULL) { // file reading finished
                break;
            }
            line[strlen(line) - 1] = '\0';
        }
        else { // interactive mode
            printf("$ ");
            scanf("%[^\n]", line);
            getchar();
        }

        /* END: TAKING INPUT */

        if(!strcmp(line, "!!")) {
            if(history[0]=='\0') {
                printf("No commands in history");
                continue;
            }
            strcpy(line, history);
            printf("%s\n", line);
        }
        line[strlen(line)] = '\n'; //terminate with new line
        strcpy(history, line);

#ifdef HISTORY_LIST
        add2history(last, line); // add command to the list
        last = last -> Next; // last inlast of the latest command
        if (!strcmp(line, "history\n")) writeHistory(commandList); // write commands to a file
#endif // HISTORY_LIST

        if (strlen(line) == 1) // if no command is entered return shell
            continue;
        execute_cmd(line);
    }
    return 0;
}

