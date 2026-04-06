#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CMDS 4 //Up to 3 pipes so only 4 commands max
#define MAX_ARGS 10 //more than enough for command arguments, can be any number
#define PORT 8080

//Function to trim spaces in command 
char *trim(char *s) {
    if (!s) return NULL;
    while (*s == ' ') s++;
    if (*s == '\0') return NULL;
    char *end = s + strlen(s);
    while (end > s && end[-1] == ' ') end--;
    *end = '\0';
    return s;
}

void myls(char **args){ //runs ls
    execvp("ls", args);
    perror("ls"); 
    exit(1); 
}

void mygrep(char **args) {// runs grep
    execvp("grep", args);
    perror("grep");
    exit(1);
}

void mypwd(char **args) {// runs pwd
    execvp("pwd", args);
    perror("pwd");
    exit(1);
}

void mymkdir(char **args) {// runs mkdir
    execvp("mkdir", args);
    perror("mkdir");
    exit(1);
}

void myrmdir(char **args) {// runs rmdir
    execvp("rmdir", args);
    perror("rmdir");
    exit(1);
}

void myrm(char **args) {// runs rm
    execvp("rm", args);
    perror("rm");
    exit(1);
}

void mycat(char **args) {// runs cat
    execvp("cat", args);
    perror("cat");
    exit(1);
}

void mycp(char **args) {// runs cp
    execvp("cp", args);
    perror("cp");
    exit(1);
}

void mymv(char **args) {// runs mv
    execvp("mv", args);
    perror("mv");
    exit(1);
}

void myecho(char **args) {// runs echo
    execvp("echo", args);
    perror("echo");
    exit(1);
}

void mydate(char **args) {// runs date
    execvp("date", args);
    perror("date");
    exit(1);
}

void mywhoami(char **args) {// runs whoami
    execvp("whoami", args);
    perror("whoami");
    exit(1);
}

void myhead(char **args) {// runs head
    execvp("head", args);
    perror("head");
    exit(1);
}

void mytail(char **args) {// runs tail
    execvp("tail", args);
    perror("tail");
    exit(1);
}

void myps(char **args) {// runs ps
    execvp("ps", args);
    perror("ps");
    exit(1);
}

void mynano(char **args) {// runs nano
    execvp("nano", args);
    perror("nano");
    exit(1);
}

void mytouch(char **args) { //runs touch
    execvp("touch", args); 
    perror("touch"); 
    exit(1); 
}

void mywho(char **args) { //runs who
    execvp("who", args); 
    perror("who"); 
    exit(1); 
}

void mychmod(char **args) { //runs chmod
    execvp("chmod", args); 
    perror("chmod"); 
    exit(1); 
}

void mychown(char **args) { //mychown
    execvp("chown", args); 
    perror("chown"); 
    exit(1); 
}

void mywc(char **args) { //runs wc
    execvp("wc", args); 
    perror("wc"); 
    exit(1); 
}

void Run(char *command) {
    char *argv[MAX_ARGS]; //to get command and additional arugment (eg. myls -l /home)
    int argc = 0; // argument counter
    char *token = strtok(command, " "); //using strtok to split commands by spaces
    while (token && argc < MAX_ARGS - 1) { 
        argv[argc++] = token; // stores each command in argv array, main command followed by its arguments
        token = strtok(NULL, " "); //until no more tokens
    }
    argv[argc] = NULL; // last command is NULL for execvp
    if (argc == 0) exit(0); // no command to run 

    char *cmd = argv[0]; // main element from argv array
if (strcmp(cmd, "myls") == 0){ //calling the functions based on command
        myls(argv);
    }

    else if (strcmp(cmd, "mygrep") == 0){
        mygrep(argv);
    }

    else if (strcmp(cmd, "mypwd") == 0){
        mypwd(argv);
    }

    else if (strcmp(cmd, "mymkdir") == 0){
        mymkdir(argv);
    }

    else if (strcmp(cmd, "myrmdir") == 0){
        myrmdir(argv);
    }

    else if (strcmp(cmd, "myrm") == 0){
        myrm(argv);
    }

    else if (strcmp(cmd, "mycat") == 0){
        mycat(argv);
    }

    else if (strcmp(cmd, "mycp") == 0){
        mycp(argv);
    }

    else if (strcmp(cmd, "mymv") == 0){
        mymv(argv);
    }

    else if (strcmp(cmd, "myecho") == 0){
        myecho(argv);
    }

    else if (strcmp(cmd, "mydate") == 0){
        mydate(argv);
    }

    else if (strcmp(cmd, "mywhoami") == 0){
        mywhoami(argv);
    }

    else if (strcmp(cmd, "myhead") == 0){
        myhead(argv);
    }

    else if (strcmp(cmd, "mytail") == 0){
        mytail(argv);
    }

    else if (strcmp(cmd, "myps") == 0){
        myps(argv);
    }
    else if (strcmp(cmd, "mynano") == 0){
        mynano(argv);
    }
    else if (strcmp(cmd, "mytouch") == 0){
        mytouch(argv);
    }
    else if (strcmp(cmd, "mychmod") == 0){
        mychmod(argv);
    }
    else if (strcmp(cmd, "mychown") == 0){
        mychown(argv);
    }
    else if (strcmp(cmd, "mywho") == 0){
        mywho(argv);
    }
    else if (strcmp(cmd, "mywc") == 0){
        mywc(argv);
    }
    else {
        printf("%s: command not found\n", cmd);
        exit(1);
    }
}


void *client_handler(void *arg) {// function to handle each client (called by each thread)
    int client_sock = *(int *)arg; // socket descriptor
    free(arg); // free allocated memory for socket
    char input[1024], output[1024]; // buffers for input and output

    printf("Client connected with ID=%lu\n", pthread_self()); //different ID for each client

    while (1) { // main loop to handle client commands
        ssize_t nread = read(client_sock, input, sizeof(input) - 1); // read command from client descriptor 
        if (nread <= 0) break; // client closed or error
        input[nread] = '\0'; // null terminate the string
        input[strcspn(input, "\n")] = '\0'; // remove newline
        if (strcmp(input, "exit") == 0) break; // exit condition

        char *trimmed_input = trim(input); // Trim input first
        if (!trimmed_input || *trimmed_input == '\0') // if empty command
            continue;

        size_t len = strlen(trimmed_input); // get length of trimmed input
        if (trimmed_input[0] == '|' || trimmed_input[len - 1] == '|') { //Reject inputs like "| cmd" or "cmd |"
            char *err = "syntax error near unexpected token '|'\n"; 
            send(client_sock, err, strlen(err), 0); // send error to client
            continue;
        }

        // Split commands by '|'
        char *cmds[MAX_CMDS];
        int ncmds = 0;
        char *token = strtok(trimmed_input, "|");
        bool empty = false; // used to check if input between pipes is empty
        while (token && ncmds < MAX_CMDS) { // max commands check
            char *tcmd = trim(token);
            if (!tcmd) { empty = true; break; }
            cmds[ncmds++] = tcmd; // store trimmed command
            token = strtok(NULL, "|");
        }
        if (empty || ncmds == 0) continue; // if any command between pipes is empty or no commands exit for pipes

        int pipes[MAX_CMDS - 1][2]; //pipe = #commands -1, 4 proccesses has 3 pipes
        for (int i = 0; i < ncmds - 1; i++) // create pipes
            if (pipe(pipes[i]) == -1) { perror("pipe error"); exit(1); }

        int last[2];
        pipe(last);
        // Fork for each command
        for (int i = 0; i < ncmds; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                if (i > 0) // if not the first command
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                if (i < ncmds - 1)
                    dup2(pipes[i][1], STDOUT_FILENO); // if not the last command
                else
                    dup2(last[1], STDOUT_FILENO); // last command

                dup2(STDOUT_FILENO, STDERR_FILENO); // Redirect stderr to stdout so errors are sent to client to prevent hanging

                for (int j = 0; j < ncmds - 1; j++) { // Close all unused pipe ends in child
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                close(last[0]);
                close(last[1]);

                Run(cmds[i]);// execute command

            } else if (pid > 0) {// Close previous pipe ends
                if (i > 0) {
                    close(pipes[i - 1][0]);
                    close(pipes[i - 1][1]);
                }
            } else {
                perror("fork error");
                exit(1);
            }
        }

        for (int i = 0; i < ncmds - 1; i++) { // Close all pipes in parent (double checking)
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        close(last[1]);

        ssize_t bytes;
        int sent_any = 0; //to prevent empty output hanging
        while ((bytes = read(last[0], output, sizeof(output) - 1)) > 0) {
            output[bytes] = '\0';
            send(client_sock, output, bytes, 0);
            sent_any = 1;
        }
        close(last[0]);

        for (int i = 0; i < ncmds; i++)
            wait(NULL);

        // If the pipeline produced no output at all, decide what to send.
        if (!sent_any)
            send(client_sock, "\n", 1, 0); // send an empty line
    }

    printf("Client disconnected ID=%lu\n", pthread_self()); // client disconnected
    close(client_sock); // close socket
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // create socket
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // set socket options

    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    address.sin_port = htons(PORT); // Port number

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { // bind socket to the port
        perror("binding error");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) { // listen for connections
        perror("listening error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening ...\n"); // server is ready

    while (1) { // main loop to accept clients
        int *new_sock = malloc(sizeof(int)); // allocate memory for client socket
        if ((*new_sock = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) { // accept a connection
            perror("acception error"); //error accepting client
            free(new_sock);
            continue;
        }

        // Create a thread to handle this client
        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, new_sock) != 0) { // create thread
            perror("pthread create error"); // error creating thread
            close(*new_sock); // close client socket
            free(new_sock);// free allocated memory
            continue;
        }

        pthread_detach(tid); // detach the thread to free resources when done
    }

    close(server_fd); // close server socket
    return 0;
}
