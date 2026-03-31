#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

int sockfd = -1;

  
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void handle_sigint(int sig)
{
    printf("Caught signal %d\n", sig);
    if (sockfd != -1) {
        close(sockfd);
        printf("Socket closed\n");
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;        // define ints
    struct sockaddr_in serv_addr; // define structs
    struct hostent *server;       // defecines host address struct
    char buffer[256];             // define buffer

    signal(SIGINT, handle_sigint);

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0); // exit main() function
    }
    portno = atoi(argv[2]);                   // get port number
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket
    if (sockfd < 0)
        error("ERROR opening socket"); // if socket creation failed
    server = gethostbyname(argv[1]);   // get server address
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n"); // if server address is not proper
        exit(0);                                  // exit main() function
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // define address family (IPv4)
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);            // copy byte data
    serv_addr.sin_port = htons(portno); // assign port
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");             // if connection failed
    // printf("Please enter the message: ");      //
    // bzero(buffer, 256);                        // fill buffer with zeros
    // fgets(buffer, 255, stdin);                 // read into buffer
    // n = write(sockfd, buffer, strlen(buffer)); // write the msg to socket
    // if (n < 0)
    //     error("ERROR writing to socket"); // writing failed
    bzero(buffer, 256);  
    while(1) {                 // if writing succeed, zero the buffer
    n = read(sockfd, buffer, 255);        // read from socket into buffer
    if (n < 0)
        error("ERROR reading from socket"); // if reading failed
    printf("%s\n", buffer);
    }
                     // print buffer
    close(sockfd);                          // close socket
    return 0;                               // exit main() function
}
