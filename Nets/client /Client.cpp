//
//  Client.cpp
//  Nets
//
//  Created by Ivan on 15.04.2021.
//

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

#define Family AF_INET
//#define Port htons(4000)
#define Addr htonl(INADDR_LOOPBACK)
int sock;
char nick[16];
int Port;

#define _GNU_SOURCE

#define Family AF_INET
//#define Port htons(4000)
#define Addr htonl(INADDR_LOOPBACK)
#define SIZE 1024


void sigHndlr(int s)
{

    shutdown(sock, 2);
    close(sock);
    exit(0);
}


int main(int argc, char **argv)
{
    int len, len2;
    char buf[SIZE];
    //char buf1[]  =  {"{\nAB\nBC\nCD\n}\n{\nA:10\nB:5\nC:4\nD:9\n}\n\0"};
    //std::cout << buf1;
    struct sockaddr_in server_addr;
    server_addr.sin_family = Family;
    
    //char *msg;
    struct addrinfo hints;
   struct addrinfo *result, *rp;
   int sfd, s, j;
    
    //if (argc < 2)
    //{
     //  printf("There is no enough arguments (port)\n");
     //  return 0;
    //}
    int tmp = 3434;
    Port = htons(tmp);

    server_addr.sin_port = Port;
    server_addr.sin_addr.s_addr = Addr;


    signal(SIGINT, sigHndlr);
    signal(SIGTSTP, sigHndlr);

    memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
   hints.ai_socktype = SOCK_STREAM; /* Stream socket */
   hints.ai_flags = 0;
   hints.ai_protocol = 0;          /* Any protocol */

   s = getaddrinfo("127.0.0.1", "3434", &hints, &result);
   if (s != 0) {
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
       exit(EXIT_FAILURE);
   }

   for (rp = result; rp != NULL; rp = rp->ai_next) {
       sock = socket(rp->ai_family, rp->ai_socktype,
                    0);
       if (sock == -1)
           continue;

       if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1)
           break;                  /* Success */

       //close(sfd);
   }

   if (rp == NULL) {               /* No address succeeded */
       fprintf(stderr, "Could not connect\n");
       exit(EXIT_FAILURE);
   }

   
        if(fork() == 0)
        {
            
            //char *text;
            while(1)
            {
                //memset(buf1,0,strlen(buf1));
                int idx;
                std::cout << "INPUT NUMBER OF TEST(1,2,3):\n";
                std::cin >> idx;
                if (idx == 1)
                {
                    write(sock, "{\n\nA,B\nB,C\nC,D\n}\n{\nA:10\nB:5\nC:4\nD:9\n}\n\0\0\0", strlen("{\n\nA,B\nB,C\nC,D\n}\n{\nA:10\nB:5\nC:4\nD:9\n}\n\0\0\0"));
                    //write(sock, buf1, SIZE);
                }
                if (idx == 2)
                {
                    write(sock, "{\n\nA,B\nB,C\nC,A\n}\n{\nA:10\nB:5\nC:4\n}\n\0\0\0", strlen("{\n\nA,B\nB,C\nC,A\n}\n{\nA:10\nB:5\nC:4\n}\n\0\0\0"));
                    //write(sock, buf1, SIZE);
                }
                if (idx == 3)
                {
                    write(sock, "{\n\nA,B\nB,C\nC,D\nD,E\n}\n{\nA:10\nB:5\nC:4\nD:3\nE:23\n}\n\0\0\0", strlen("{\n\nA,B\nB,C\nC,D\nD,E\n}\n{\nA:10\nB:5\nC:4\nD:3\nE:23\n}\n\0\0\0"));
                    //write(sock, buf1, SIZE);
                }
                
                

            }
        }
        else
            while(1)
            {
                if((len = read(sock, buf, SIZE)) > 0)
                {
                    buf[len] = '\0';
                    if(strcmp(buf,"\n"))
                    {
                        printf("%s\n", buf);
                    }
                }
                else
                {
                    printf("You have left\n");
                    sigHndlr(0);
                }
            }
                
    return 0;
}
