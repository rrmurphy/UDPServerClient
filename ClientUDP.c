#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

#define MAXDATASIZE 256 // max number of bytes we can get at once
#define MAXSTRLEN 253

#define DISEMVOWELING (80)
#define CLENGTH (5)
#define UPPERCASING (10)

#define BILLION (1000000000L)

int main(int argc, char *argv[]) {
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
    char buffer[MAXDATASIZE];
    char string[MAXSTRLEN];
    int opCode = atoi(argv[3]);
    uint64_t diff;
	struct timeval tvalBefore, tvalAfter;

	if (argc != 5) {
        printf("%d\n", argc);
		fprintf(stderr,"usage: client hostname port operation message\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

    // Create buffer
    buffer[0] = strlen(argv[4]) + 3;
    buffer[1] = 1; // requestId
    buffer[2] = opCode;

    int i = 0;
    for (i = 0; i < strlen(argv[4]); i++) {
        buffer[i + 3] = argv[4][i];
    }

    gettimeofday(&tvalBefore, NULL);
	if ((numbytes = sendto(sockfd, buffer, buffer[0], 0,
            p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
    
    if ((numbytes = recvfrom(sockfd, buffer, buffer[0], 0,
            p->ai_addr, &(p->ai_addrlen))) == -1) {
        perror("error: recvfrom");
        exit(1);
    }
    gettimeofday(&tvalAfter, NULL);

	diff = (BILLION * (tvalAfter.tv_sec - tvalBefore.tv_sec)) 
		+ (1000L * (tvalAfter.tv_usec - tvalBefore.tv_usec));
    
    if (opCode == 80 || opCode == 10) {
        int length = buffer[0];
        for (i = 0; i < length; i++) {
            string[i] = buffer[i + 2];
        }
        string[i] = 0;
        printf("Request ID: %d\nResponse: %s\nRound-Trip Time: %llu nanoseconds\n", 
                buffer[1], string, (long long unsigned int) diff);
    } else {
        printf("Request ID: %d\nResponse: %d\nRound-Trip Time: %llu nanoseconds\n", 
                buffer[1], buffer[2], (long long unsigned int) diff);
    }

	freeaddrinfo(servinfo);
	close(sockfd);

	return 0;
}
