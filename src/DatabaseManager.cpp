#include "../include/DatabaseManager.h"
#include "utils/Exceptions.h"

DatabaseManager::DatabaseManager(const string &dbFilePath) : dbFilePath(dbFilePath) {}

int DatabaseManager::getNextId()
{
    ifstream infile(dbFilePath);
    int maxId = 0;
    string line;

    if (!infile.is_open())
        throw runtime_error("Unable to open database file for reading.");

    while (getline(infile, line))
    {
        stringstream ss(line);
        int id;
        ss >> id;
        if (id > maxId)
            maxId = id;
    }

    infile.close();
    return maxId + 1;
}

void DatabaseManager::saveUser(User &user)
{
    ofstream outfile(dbFilePath, ios::app);

    if (!outfile.is_open())
        throw runtime_error("Unable to open database file for appending.");

    int newId = getNextId();
    user.id = newId;
    outfile << newId << " " << user.username << " " << user.password << " "
            << static_cast<int>(user.role) << " " << user.workTime << endl;

    outfile.close();
}

User DatabaseManager::getUserById(int id)
{
    ifstream infile(dbFilePath);
    string line;

    if (!infile.is_open())
        throw runtime_error("Unable to open database file for reading.");

    while (getline(infile, line))
    {
        stringstream ss(line);
        int userId;
        string username, password;
        int role;
        double workTime;
        ss >> userId >> username >> password >> role >> workTime;

        if (userId == id)
        {
            infile.close();
            return User(userId, username, password, static_cast<UserRole>(role), workTime);
        }
    }

    infile.close();
    throw ClientException("User with ID " + std::to_string(id) + " not found.");
}

void DatabaseManager::updateUserById(const int &id, const string &newUsername, const string &newPassword, UserRole *newRole, const double *workTime)
{
    vector<User> users = getAllUsers();
    bool updated = false;
    ofstream outfile("temp.txt");

    if (!outfile.is_open())
        throw runtime_error("Unable to open temporary file for writing.");

    for (auto &user : users)
    {
        if (user.id == id)
        {
            outfile << id << " "
                    << (newUsername.empty() ? user.username : newUsername) << " "
                    << (newPassword.empty() ? user.password : newPassword) << " "
                    << static_cast<int>(newRole ? *newRole : user.role) << " "
                    << (workTime ? *workTime : user.workTime)
                    << endl;
            updated = true;
        }
        else
        {
            outfile << user.id << " " << user.username << " " << user.password << " " << static_cast<int>(user.role) << " " << user.workTime << endl;
        }
    }

    outfile.close();

    if (!updated)
        throw ClientException("User with ID " + std::to_string(id) + " not found.");

    remove(dbFilePath.c_str());
    rename("temp.txt", dbFilePath.c_str());
}

vector<User> DatabaseManager::getAllUsers()
{
    ifstream infile(dbFilePath);
    vector<User> users;
    string line;

    if (!infile.is_open())
        throw runtime_error("Unable to open database file for reading.");

    while (getline(infile, line))
    {
        stringstream ss(line);
        int userId;
        string username, password;
        int role;
        double workTime;
        ss >> userId >> username >> password >> role >> workTime;
        users.emplace_back(userId, username, password, static_cast<UserRole>(role), workTime);
    }

    infile.close();
    return users;
}

void DatabaseManager::removeUserById(int id)
{
    vector<User> users = getAllUsers();
    bool removed = false;
    ofstream outfile("temp.txt");

    if (!outfile.is_open())
        throw runtime_error("Unable to open temporary file for writing.");

    for (const auto &user : users)
    {
        if (user.id != id)
            outfile << user.id << " " << user.username << " " << user.password << " " << static_cast<int>(user.role) << " " << user.workTime << endl;
        else
            removed = true;
    }

    outfile.close();

    if (!removed)
        throw ClientException("User with ID " + std::to_string(id) + " not found.");

    remove(dbFilePath.c_str());
    rename("temp.txt", dbFilePath.c_str());
}

void DatabaseManager::updateWorkTime(int id, const double &workTime)
{
    updateUserById(id, "", "", nullptr, &workTime);
}