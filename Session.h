#ifndef CONSOLECHAT_SESSION_H
#define CONSOLECHAT_SESSION_H
#define SERVER_PORT 12345
#define CLIENT_PORT 12346
#include <string>
#include <netinet/in.h>
#include "MsgQueue.h"
#include <thread>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <mutex>

class Session {
private:
    char myName[30]{}, opponentName[30]{};
    sockaddr_in opponentAddr{} ,channel{};
    int counter;
    int connect_socket{};
    int BUF_SIZE;
    MsgQueue *queue;
    std::mutex mtx;

    void send();
    void receive();
    void connectToServer();
    void createConnection();
    void setupUDP(int &, int);
    static void fatal(const std::string&);
public:
    enum option{
        CLIENT,
        SERVER,
    };
    explicit Session(int);
    void startChatting();
    void establishConnection(option);
    void setUserName();
};


#endif //CONSOLECHAT_SESSION_H
