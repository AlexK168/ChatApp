#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <mutex>
#include "Session.h"

#define SERVER_PORT 12345
#define CLIENT_PORT 12346
#define BUF_SIZE 100

void fatal(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

void setUpUDP(int &s, int port) {
    struct sockaddr_in channel;
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0) fatal("failed to create udp socket");

    memset(&channel, 0, sizeof(channel));
    channel.sin_family = AF_INET; // IPv4
    channel.sin_addr.s_addr = htons(INADDR_ANY);
    channel.sin_port = htons(port);

    bind(s, (struct sockaddr*)&channel, sizeof(channel));
}

void connect(char * userName, sockaddr_in &channel, sockaddr_in &opponentAddr, int &socketUDP, char *opponentName) {
    int s ,c;
    char ip[15];
    struct hostent *h;
    std::cout << "Enter IP address to connect to" << std::endl;
    scanf("%s", ip);
    h = gethostbyname(ip);
    if (!h) fatal("Invalid ip address");

    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0) fatal("Can't create a socket");

    memset(&channel, 0, sizeof(channel));
    bcopy(h->h_addr, &channel.sin_addr, h->h_length);
    channel.sin_family = AF_INET;
    channel.sin_port = htons(SERVER_PORT);
    memcpy(&opponentAddr, &channel, sizeof(channel));
    c = connect(s, (struct  sockaddr *) &channel, sizeof(channel));
    if(c < 0) fatal("Can't connect");

    printf("%s\n", "Connected");
    write(s, userName, strlen(userName) + 1);
    read(s, opponentName, BUF_SIZE);
    close(s);

    setUpUDP(socketUDP, CLIENT_PORT);
}

void createConnection(char * userName, sockaddr_in &channel, sockaddr_in &opponentAddr, int &socketUDP, char *opponentName) {
    int on = 1;
    memset(&channel, 0, sizeof(channel));
    channel.sin_family = AF_INET; // it's always AF_INET
    channel.sin_addr.s_addr = htonl(INADDR_ANY); // set IP address. htonl is used to eliminate big/little-endian issues
    channel.sin_port = htons(SERVER_PORT); // set port to SERVER_PORT - 12345

    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET - address format, SOCK_STREAM - reliable byte stream, IPPROTO_TCP - TCP protocol
    if (s < 0) fatal("Can't create a socket");

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *) &on, sizeof(on));
    int b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
    if (b < 0) fatal("Binding failed");

    int l = listen(s, 5);
    if (l < 0) fatal("listen() has failed");

    int c = sizeof(struct sockaddr_in);
    int sa = accept(s, (struct sockaddr *) &opponentAddr, (socklen_t*)&c);
    printf("%s\n", "Connected");
    opponentAddr.sin_port = htons(CLIENT_PORT);
    if (sa < 0) fatal("Can't accept");
    read(sa, opponentName, BUF_SIZE);
    write(sa, userName, strlen(userName) + 1);
    close(sa);

    setUpUDP(socketUDP, SERVER_PORT);
}

int main() {
    struct sockaddr_in opponentAddr, channel;
    int s;
    char opponentName[30], myName[30];
    printf("Enter your nickname(that's what other user will see):\n");
    scanf("%s", myName);
    printf("Press \n1 - to create connection\n2 - to connect to existing one\n3 - to quit\n");
    int option;
    bool flag = true;
    while (flag) {
        scanf("%d", &option);
        switch (option) {
            case 1:
                std::cout << "Waiting for connections...\n";
                createConnection(myName, channel, opponentAddr, s, opponentName);
                flag = false;
                break;
            case 2:
                connect(myName, channel, opponentAddr, s, opponentName);
                flag = false;
                break;
            case 3:
                exit(1);
                break;
            default:
                std::cout << "Enter valid option!" << std::endl;
        }
    }

    Session session(s, std::string(myName), std::string(opponentName), opponentAddr, BUF_SIZE);
    session.startChatting();

    return 0;
}