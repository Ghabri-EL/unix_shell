/* Name: Gabriel Gherasim
** Email: gherasimg15@gmail.com
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h> 
#include<ctype.h>
#include"functions.h"

int main(void){
    size_t characters;
    size_t size = 40;
    char* line;
    char** tokens;
    int token_count;
    int arrow_index = 0;
    int sentinel = 1;
    int terminalOutput = 0;
    
    line = malloc(size * sizeof(char));
    
    printf("\n[]=================== SHELL ===================[]\n");
    if (signal(SIGINT, sig_handler) == SIG_ERR){
        printf("\nCannnot catch SIGINT\n");
    }
    
    while(sentinel){
        token_count = 0;
        char* tokenPtr = NULL;
        tokens = malloc(sizeof(char*));

        printf("%s# ", timeInfo());
        if((characters = getline(&line, &size, stdin)) != EOF){

            if((line == NULL) || (strlen(line) < 2)){
                continue;// skip iteration
            }

            removeTrailingNL(line);
            tokenPtr = strtok(line, " ");
                    
            // splitting tokens 
            while(tokenPtr != NULL){

                tokens[token_count] = tokenPtr;
                token_count++;
                tokens = realloc(tokens, (token_count + 1) * sizeof(char*));
                if(tokens == NULL){
                    fprintf(stderr, "\nCannot allocate memory. Exiting\n");
                    return EXIT_FAILURE;
                }
                tokenPtr = strtok(NULL, " ");
            }
            tokens[token_count] = tokenPtr;
            //end split
            lowercase(tokens[0]);//tokens[0]/command to lowercase

            //start of commands execution
            if(strcmp("cd", tokens[0]) == 0){
                if(token_count > 1){                    
                    cdcmd(tokens[1]);
                }
                else{
                    cdcmd("home");
                }
                free(tokens);
                continue;// skip iteration
            }
            
            // redirect returns the index of >
            // we resize the tokens accordingly
            arrow_index = redirect(tokens, &terminalOutput);
            if(arrow_index != 0){
                tokens[arrow_index] = NULL;
                tokens = realloc(tokens, (arrow_index + 1) * sizeof(char*));
                token_count = arrow_index - 1; 
            }                      
                        
            if(strcmp("ls", tokens[0]) == 0){//run ls command
                runcmd("ls",tokens);
            }
            else if(strcmp("mkdir", tokens[0]) == 0){//run mkdir command
                runcmd("mkdir",tokens);
            }
            else if(strcmp("pwd", tokens[0]) == 0){//run pwd command
                runcmd("pwd",tokens);
            }
            else if(strcmp("echo", tokens[0]) == 0){//run echo command
                runcmd("echo",tokens);
            }
            else if(strcmp("sleep", tokens[0]) == 0){//run sleep command
                runcmd("sleep",tokens);
            }
            else if(strcmp("rmdir", tokens[0]) == 0){//run rmdir command
                runcmd("rmdir",tokens);
            }           
            else if(strcmp("rm", tokens[0]) == 0){//run rm command
                runcmd("rm",tokens);
            }
            else if(strcmp("man", tokens[0]) == 0){//run man command
                runcmd("man",tokens);
            }
            else if(strcmp("cat", tokens[0]) == 0){//run cat command
                runcmd("cat",tokens);
                printf("\n");
            }            
            else if(strcmp("whoami", tokens[0]) == 0){//run whoami command
                whoami();
            }
            else if(strcmp("clear", tokens[0]) == 0){//run clear command
                runcmd("clear", tokens);
            }
            else if(strcmp("exit", tokens[0]) == 0){//run exit command
                exit(0);
            }
            else if(strcmp("args", tokens[0]) == 0){//run args command
                print(tokens);
            }
            else if(strcmp("help", tokens[0]) == 0){//run help command
                help();
            }else
            {
                printf("\n >> Invalid command << \n");
            }
                                        
            //redirect saves the normal stdout to terminalOutput before it redirect output to file
            //after the command is executed this return output to terminal
            if(terminalOutput != 0){
                dup2(terminalOutput, 1);
                close(terminalOutput);
            }            
        } 
        else{
            printf("^D\n[> Exit <]\n");
            sentinel = 0;
        }
        free(tokens);        
    }
    
    free(line);
    return EXIT_SUCCESS;
}