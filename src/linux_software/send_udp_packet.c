#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DEST_IP   "192.168.1.5"
#define DEST_PORT 8080
#define MESSAGE   "Hello, World!"

int main(void) {
    int sockfd;
    struct sockaddr_in dest_addr;

    // 1. Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return EXIT_FAILURE;
    }

    // 2. Fill in destination address struct
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port   = htons(DEST_PORT);
    if (inet_pton(AF_INET, DEST_IP, &dest_addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid address: %s\n", DEST_IP);
        close(sockfd);
        return EXIT_FAILURE;
    }

    // 3. Send the message
    ssize_t sent_bytes = sendto(
        sockfd,
        MESSAGE,
        strlen(MESSAGE),
        0,
        (struct sockaddr*)&dest_addr,
        sizeof(dest_addr)
    );
    if (sent_bytes < 0) {
        perror("sendto failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Sent %zd bytes to %s:%d\n", sent_bytes, DEST_IP, DEST_PORT);

    // 4. Clean up
    close(sockfd);
    return EXIT_SUCCESS;
}
