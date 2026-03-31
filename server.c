#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// define function that deals with errors
void error(const char *msg)
{
    perror(msg); // print error msg
    exit(1);     // exit the main() function
}
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;          // defines ints
    socklen_t clilen;                       // size of address
    char buffer[256];                       // buffer
    struct sockaddr_in serv_addr, cli_addr; // defines structures
    int n;
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n"); // deal with wrong port
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // makes a socket
    if (sockfd < 0)
        error("ERROR opening socket"); // if socket creation failed
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);                 // get port number
    serv_addr.sin_family = AF_INET;         // specify address family (IPv4)
    serv_addr.sin_addr.s_addr = INADDR_ANY; // bind IP addresses
    serv_addr.sin_port = htons(portno);     // set port number
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding"); // error while binding socket
    listen(sockfd, 5);             // listen on created socket
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *)&cli_addr,
                       &clilen); // creates new socket for first incoming connection
    if (newsockfd < 0)
        error("ERROR on accept"); // error while creating socket
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255); // read from connection socket
    if (n < 0)
        error("ERROR reading from socket");      // if error
    printf("Here is the message: %s\n", buffer); // show the message

    // Remove newline character if present
    buffer[strcspn(buffer, "\r\n")] = 0;

    // Check if the buffer contains the string "red"
    if (strcmp(buffer, "red") == 0)
    {
        // Call the Python script to turn on the LED
        if (system("python3 /home/student/led_on.py > /home/student/led_on_output.log 2>&1") == -1)
        {
            perror("Error executing system command");
        }
        else
        {
            printf("Warunek spełniony\n");
        }
    }
    else
    {
        printf("Warunek nie jest spełniony\n");
    }

    n = write(newsockfd, "I got your message", 18);

    if (n < 0)
        error("ERROR writing to socket"); //
    close(newsockfd);                     // close connection socket
    close(sockfd);                        // close listening socket
    return 0;
}

