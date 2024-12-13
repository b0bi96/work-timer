#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include "User.h"
#include "MyApp.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class AuthManager
{
private:
    MyApp *mainApp;

    bool verifyPassword(const string &inputPassword, const string &storedPassword);

public:
    AuthManager(MyApp *mainApp);

    User registerUser(const string &username, const string &password, UserRole role);
    User loginUser(const string &username, const string &password);
    void logOut();
};

#endif
