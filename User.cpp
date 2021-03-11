#include "User.h"

User::User(std::string name) {
    displayName = name;
}

std::string User::getName() const {return displayName;}

std::ostream& operator<<(std::ostream& stream, const User& user) {
    stream << user.getName();
    return stream;
}