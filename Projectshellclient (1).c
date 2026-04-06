#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sock=0;
    struct sockaddr_in serv_addr;

    if ((sock = socket (AF_INET, SOCK_STREAM,0)) < 0 )// create socket
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;// IPv4
    serv_addr.sin_port = htons(PORT);// Port number

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)//
    {
        printf("\n Invalied address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))< 0)// connect to server
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    while(1)
    {
        char input[1024];
        char output[1024] = {0};
        char pwd[256];
        if (getcwd(pwd, sizeof(pwd)) == NULL)
            strcpy(pwd, "?");
        printf(">myshell:%s$ ",pwd);  // Shell prompt
        if (!fgets(input, sizeof(input), stdin))
            break;

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Ignore empty input (just reprompt)
        char *trimmed = input;
        while (*trimmed == ' ') trimmed++;  // skip leading spaces

        // Check if input is empty or just spaces
        if (strlen(trimmed) == 0)
            continue;

        if (strcmp(input, "exit") == 0)
            break;

        send(sock, input, strlen(input), 0);// send input to server


        ssize_t bytes;
        while ((bytes = read(sock, output, sizeof(output) - 1)) > 0) {// read output from server
            output[bytes] = '\0';
            printf("%s", output);

    
            if (bytes < sizeof(output) - 1)// if less than buffer size, done reading
                break;
        }

        if (bytes == 0) {
            printf("Server closed connection.\n");
            break;
        } else if (bytes < 0) {
            perror("read");
            break;
        }


    
    }
    close(sock);// close socket
    printf("logout\n\n");
    return 0;
}