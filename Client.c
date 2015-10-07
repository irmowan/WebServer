//
// Created by irmo on 15/10/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define PROCESS_SIZE 5
#define MESSAGE_SIZE 10
void error(const char *msg) {
    perror(msg);
    exit(1);
}

char messages[MESSAGE_SIZE][32] =
        {"Hello!", "Nice to see you!", "Wonderful!", "OK.",
         "This is a simple client.", "Welcome!", "Thank you!",
        "It is great.", "Cool!", "Great~"};

int main(int argc, char *argv[]) {
    int sockfd, portno, n, pid, count;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    clock_t  start = clock();
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    for (int i = 0; i < PROCESS_SIZE && (pid = fork()) != 0; ++i);
    if (pid < 0)
        error("ERROR on fork");
    if (pid > 0) {
        while (wait(NULL) != -1);
        exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    bzero(buffer, 256);
    //fgets(buffer, 255, stdin);
    clock_t finish = clock();
    srand((unsigned int) (((finish - start)% 1001)));
    int index = rand() % MESSAGE_SIZE;
    strcpy(buffer, messages[index]);
    printf("The message is \"%s\".\n", buffer);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n", buffer);
    close(sockfd);
    return 0;
}