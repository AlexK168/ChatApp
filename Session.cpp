#include "Session.h"

Session::Session(int bufferSize) {
    BUF_SIZE = bufferSize;
    queue = new MsgQueue(15);
    counter = 0;
}

void Session::send() {
    for (;;) {
        size_t bufsize = BUF_SIZE;
        char buffer[bufsize + sizeof(int)];
        char *b = buffer + sizeof(int);
        getline(&b, &bufsize, stdin);
        if (*b == '\n') {
            continue;
        }
        int c;
        mtx.lock();
        counter++;
        c = counter;
        mtx.unlock();
        memcpy(buffer, &c, sizeof(int));
        sendto(connect_socket, buffer, BUF_SIZE, 0,
               (struct sockaddr*)&opponentAddr, sizeof(opponentAddr));
        queue->push(c, std::string(b), std::string(myName));
    }
}

void Session::receive() {
    for (;;) {
        char buffer[BUF_SIZE];
        recvfrom(connect_socket, buffer, BUF_SIZE, 0, (struct sockaddr*)NULL, NULL);
        int c;
        memcpy(&c, buffer, sizeof(int));
        char * b = buffer;
        b += sizeof(int);
        queue->push(c, std::string(b), std::string(opponentName));
        mtx.lock();
        counter = c + 1;
        mtx.unlock();
    }
}

void Session::startChatting() {
    std::thread t1(&Session::receive, this);
    std::thread t2(&Session::send, this);
    t1.join();
    t2.join();
}

void Session::establishConnection(Session::option opt) {
    if (opt == Session::CLIENT) {
        connectToServer();
    } else {
        createConnection();
    }
}

void Session::createConnection() {
    int on = 1;
    memset(&channel, 0, sizeof(channel));
    channel.sin_family = AF_INET;
    channel.sin_addr.s_addr = htonl(INADDR_ANY);
    channel.sin_port = htons(SERVER_PORT);

    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    write(sa, myName, strlen(myName) + 1);
    close(sa);

    setupUDP(connect_socket, SERVER_PORT);
}

void Session::connectToServer() {
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
    write(s, myName, strlen(myName) + 1);
    read(s, opponentName, BUF_SIZE);
    close(s);

    setupUDP(connect_socket, CLIENT_PORT);
}

void Session::setUserName() {
    printf("Enter your nickname(that's what other user will see):\n");
    scanf("%s", myName);
}

void Session::fatal(const std::string& msg) {
    std::cout << msg << std::endl;
    exit(1);
}

void Session::setupUDP(int &s, int port) {
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0) fatal("failed to create udp socket");

    memset(&channel, 0, sizeof(channel));
    channel.sin_family = AF_INET; // IPv4
    channel.sin_addr.s_addr = htons(INADDR_ANY);
    channel.sin_port = htons(port);

    bind(s, (struct sockaddr*)&channel, sizeof(channel));
}



