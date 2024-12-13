#include "../include/User.h"

User::User(int id, string u, string p, UserRole r, double workTime)
    : id(id), username(u), password(p), role(r), workTime(workTime)
{
}

User::User(string u, string p, UserRole r, double workTime)
    : id(0), username(u), password(p), role(r), workTime(workTime) {}
