//
// Created by irmo on 15/10/7.
//

/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void dostuff(int, int);

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, pid, count;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("Welcome to the server! The port is %d.\n", portno);
    printf("The server is running...\n");
    // Infinite loop
    count = 0;
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0) {
            // The child process call the dostuff() to handle messages.
            close(sockfd);
            dostuff(newsockfd, count);
            exit(0);
        } else
            close(newsockfd);
        count++;
    }
    // Never get here.
    close(sockfd);
    return 0;
}

void dostuff(int sock, int count) {
    int n;
    int len;
    char buffer[256];
    char reply[32] = "The length of the message is";
    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    len = strlen(buffer);
    sprintf(reply + strlen(reply), " %d" , len);
    printf("No.%d: Here is the message: %s\n", count, buffer);
    n = write(sock, reply, 255);
    if (n < 0) error("ERROR writing to socket");
}