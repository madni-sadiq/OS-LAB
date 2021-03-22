#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


char **tokenize_and(char *line)
{
  char **tokens_and = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token_and = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;
  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == '&' || readChar == '\n'){
    	if ( readChar == '\n' ){
    		token_and[tokenIndex++] = '\n';
    		token_and[tokenIndex] = '\0';
    	}
    	else
      		token_and[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens_and[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens_and[tokenNo++], token_and);
	tokenIndex = 0; 
      }
    } else {
      token_and[tokenIndex++] = readChar; 
    }
  }
 
  free(token_and);
  tokens_and[tokenNo] = NULL ;
  return tokens_and;
}

void printDir() 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\nDir: %s", cwd); 
}

// Function where the system command is executed
void execCmd(char** tokens)
{

	pid_t pid;
	pid = fork();
		
		// Forking a child
		if (pid == -1){
			printf("\nShell: Failed forking child");
			return;
		}
		else if (pid == 0){
			if (execvp(tokens[0], tokens) < 0){
				printf("\nShell: Incorrect command");
			}
			exit(0);
		}
		else {// waiting for child to terminate
			wait(NULL);
			return;
		}
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE]; 
	char **tokens_and;           
	char  **tokens;              
	int i;

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		/* BEGIN: TAKING INPUT */
		printDir();
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		
		}
		
		/* END: TAKING INPUT */
		
		line[strlen(line)] = '\n'; //terminate with new line
		if (strlen(line) == 1)
			continue;
		tokens_and = tokenize_and(line);
		
		for(i = 0; tokens_and[i] != NULL;i++){
			if (strlen(tokens_and[i]) == 1)
				continue;
			else{	
			tokens = tokenize(tokens_and[i]);
			
       		if (tokens[0][0] == 'c' && tokens[0][1] == 'd' & tokens[0][2] == '\0')
				chdir(tokens[1]);	
			else
				execCmd(tokens);
				
			}
			}
		
		for(int j=0;tokens[j]!=NULL;j++){
			free(tokens[j]);
		}
		free(tokens);
		
		for(i=0;tokens_and[i]!=NULL;i++){
			free(tokens_and[i]);
		}
		free(tokens_and);

	}
	return 0;
}
