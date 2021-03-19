#include <iostream>
#include "Session.h"
#define BUF_SIZE 100

int main() {
    Session session(BUF_SIZE);
    session.setUserName();
    std::cout << "Press \n1 - to create connection\n2 - to connect to existing one\n3 - to quit\n";
    int option;
    bool flag = true;
    while (flag) {
        std::cin >> option;
        switch (option) {
            case 1:
                std::cout << "Waiting for connections...\n";
                session.establishConnection(Session::SERVER);
                flag = false;
                break;
            case 2:
                session.establishConnection(Session::CLIENT);
                flag = false;
                break;
            case 3:
                exit(1);
            default:
                std::cout << "Enter valid option!" << std::endl;
        }
    }
    session.startChatting();

    return 0;
}