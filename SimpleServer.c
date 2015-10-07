//
// Created by irmo on 15/10/7.
//

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    char buffer[256];
    // sockaddr_in is a structure containing an internet address.
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    // socket() creates a new socket.
    // AF_INET is the address domain of the socket, which means the Internet domain for two hosts.
    // SOCK_STREAM is a type of socket, as a stream.
    // The third argument is the protocol, zero let the operating system choose the most appropriate protocol.
    // Here it will choose TCP for stream sockets.
    // The socket call returns a entry into the file descriptor table.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // If the socket call fails, it returns -1.
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    // htons() converts a port number in host byte order to in network byte order.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // bind() binds a socket to an address.
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    // listen() applies to stream sockets.
    // The second argument is the size of backlog queue,
    // 5 is the maximum size permitted by most systems.
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    // accept() causes the process to block until a client connects to the server.
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n", buffer);
    n = write(newsockfd, "I got your message", 18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);
    return 0;
}