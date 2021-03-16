#ifndef CONSOLECHAT_MSGQUEUE_H
#define CONSOLECHAT_MSGQUEUE_H
#include <string>
#include <queue>
#include <iostream>
#include <algorithm>
#include <term.h>
#include <zconf.h>
#include <mutex>

class MsgQueue {
public:
    explicit MsgQueue(int s = 10);
    void push(int, std::string, std::string);
private:
    struct Item {
        int id;
        std::string message;
        std::string holder;

        bool operator< (const Item& item1) const;
    };
    std::mutex mtx;
    int size;
    std::deque<Item> queue;
    void output();
    void clearScreen();
};

#endif //CONSOLECHAT_MSGQUEUE_H
