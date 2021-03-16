#include "Session.h"

Session::Session(int socket, std::string myName, std::string opponentsName, sockaddr_in &opponentAddr, int bufferSize) {
    this->socket = socket;
    myNickname = std::move(myName);
    opponentsNickname = std::move(opponentsName);
    this->opponentAddr = opponentAddr;
    BUF_SIZE = bufferSize;
    queue = new MsgQueue();
}

void Session::send() {
    for (;;) {
        size_t bufsize = BUF_SIZE;
        char buffer[bufsize];
        char *b = buffer;
        getline(&b, &bufsize, stdin);
        if (buffer[0] == '\n') {
            continue;
        }
        sendto(socket, buffer, sizeof(buffer), 0,
               (struct sockaddr*)&opponentAddr, sizeof(opponentAddr));
        queue->push(1, std::string(buffer), myNickname);
    }
}

void Session::receive() {
    for (;;) {
        char buffer[BUF_SIZE];
        recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
        queue->push(1, std::string(buffer), opponentsNickname);
    }
}

void Session::startChatting() {
    std::thread t1(&Session::receive, this);
    std::thread t2(&Session::send, this);
    t1.join();
    t2.join();
}
