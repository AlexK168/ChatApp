#ifndef CONSOLECHAT_USER_H
#define CONSOLECHAT_USER_H
#include <string>
#include <ostream>

class User {
private:
    std::string displayName;
public:
    User(std::string name);
    std::string getName() const;
};

std::ostream& operator<<(std::ostream& stream, const User& user);

#endif //CONSOLECHAT_USER_H
