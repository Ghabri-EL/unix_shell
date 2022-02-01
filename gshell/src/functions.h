/* Name: Gabriel Gherasim
** Email: gherasimg15@gmail.com
*/
//environment variable
extern char** environ;
//errno variable
extern int errno;

//function to remove the new line char at the end of a string
void removeTrailingNL(char st[]);

//signal handler
void sig_handler(int signo);

//function to run commands
void runcmd(char *s, char *args[]);

//function to print tokens
void print(char **strs);

//function to set chars to lowercase
void lowercase(char* s);

//cd feture to change directiories
void cdcmd(char* str);

//returns the name of the user
void whoami();

//funtion to return the time as header of new line, returns string with required time data
char* timeInfo();

//to redirect output to entered file and return the index of the '>' if found and if it has a command before it. Otherwise returns 0
//takes a pointer to an int ant saves the usual terminal output in that var and is used in code to restore output to terminal
int redirect(char **tkns, int *terminalOut);

//print some helpful information
void help();