#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

enum class UserRole
{
    User,
    Owner
};

class User
{
public:
    int id;
    string username;
    string password;
    UserRole role;
    double workTime;

    User() : id(0), username(""), password(""), role(UserRole::User), workTime(0.0) {}
    User(string u, string p, UserRole r = UserRole::User, double workTime = 0);
    User(int id, string u, string p, UserRole r = UserRole::User, double workTime = 0);
};

#endif
