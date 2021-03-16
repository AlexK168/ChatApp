#include "Session.h"

Session::Session(int socket, std::string myName, std::string opponentsName, sockaddr_in &opponentAddr, int bufferSize) {
    counter = 0;
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
        sendto(socket, buffer, BUF_SIZE, 0,
               (struct sockaddr*)&opponentAddr, sizeof(opponentAddr));
        queue->push(c, std::string(b), myNickname);
    }
}

void Session::receive() {
    for (;;) {
        char buffer[BUF_SIZE];
        recvfrom(socket, buffer, BUF_SIZE, 0, (struct sockaddr*)NULL, NULL);
        int c;
        memcpy(&c, buffer, sizeof(int));
        char * b = buffer;
        b += sizeof(int);
        queue->push(c, std::string(b), opponentsNickname);
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
