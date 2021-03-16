#ifndef CONSOLECHAT_SESSION_H
#define CONSOLECHAT_SESSION_H
#include <string>
#include <netinet/in.h>
#include "MsgQueue.h"
#include <thread>
#include <cstring>

class Session {
private:
    int counter;
    std::string myNickname, opponentsNickname;
    int socket;
    sockaddr_in opponentAddr{};
    int BUF_SIZE;
    MsgQueue *queue;
    void send();
    void receive();
    std::mutex mtx;
public:
    Session(int, std::string, std::string, sockaddr_in&, int);
    void startChatting();
};


#endif //CONSOLECHAT_SESSION_H
