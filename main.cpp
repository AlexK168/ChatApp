#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include "User.h"

#define SERVER_PORT 12345
#define BUF_SIZE 1024

void connect(char * userName) {
    int s ,c;
    char buffer[1024] = {};
    char ip[15] = {};
    struct hostent *h;
    struct sockaddr_in channel;
    std::cout << "Enter IP address to connect to" << std::endl;
    scanf("%s", ip);
    h = gethostbyname(ip);
    if (!h) return;
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0) return;
    memset(&channel, 0, sizeof(channel));
    channel.sin_family=AF_INET;
    memcpy(&channel.sin_addr.s_addr, h->h_addr_list, h->h_length);
    channel.sin_port = htons(SERVER_PORT);

    c = connect(s, (struct  sockaddr *) &channel, sizeof(channel));
    if(c < 0) return;
    write(s, userName, strlen(userName) + 1);
}

void createConnection() {
    int on = 1;
    struct sockaddr_in channel;
    char buffer[1024] = {};

    // initialization of socket address
    memset(&channel, 0, sizeof(channel));
    channel.sin_family = AF_INET; // it's always AF_INET
    channel.sin_addr.s_addr = htonl(INADDR_ANY); // set IP address. htonl is used to eliminate big/little-endian issues
    channel.sin_port = htons(SERVER_PORT); // set port to SERVER_PORT - 12345

    // step 1 - create socket
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET - address format, SOCK_STREAM - reliable byte stream, IPPROTO_TCP - TCP protocol
    if (s < 0) return;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

    // step 2 - bind to address
    int b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
    if (b < 0) return;

    // step 3 - listen to connections
    int l = listen(s, 10);
    if (l < 0) return;

    // step 4 - block thread and wait for connection
    int sa = accept(s, 0, 0);
    if (sa < 0) return;

    read(sa, buffer, BUF_SIZE);
    for (const auto c : buffer)
        std::cout << c;
    std::cout << std::endl;
}

int main() {
    printf("Enter your nickname(that's what other user will see):\n");
    char name[30]{};
    scanf("%s", name);
    // User u(name);
    // std::cout << "Welcome, " << u << "!" << std::endl;
    std::cout << "Press \n1 - to create connection\n2 - to connect to existing one\n3 - to quit" << std::endl;
    int option;
    bool flag = true;
    while (flag) {
        std::cin >> option;
        switch (option) {
            case 1:
                std::cout << "Creating connection...";
                createConnection();
                flag = false;
                break;
            case 2:
                std::cout << "Connecting...";
                connect(name);
                flag = false;
                break;
            case 3:
                flag = false;
                break;
            default:
                std::cout << "Enter valid option!" << std::endl;
        }
    }

    return 0;
}
