#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "User.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdio>

using namespace std;

class DatabaseManager
{
private:
    string dbFilePath;

    int getNextId();

public:
    DatabaseManager(const string &dbFilePath);

    void saveUser(User &user);
    User getUserById(int id);
    void updateUserById(const int &id, const string &newUsername, const string &newPassword, UserRole *newRole, const double *workTime);
    vector<User> getAllUsers();
    void removeUserById(int id);
    void updateWorkTime(int id, const double &workTime);
};

#endif
