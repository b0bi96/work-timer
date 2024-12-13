#include "../include/AuthManager.h"
#include "../include/DatabaseManager.h"
#include "../include/Toast.h"
#include "../include/AuthFrame.h"
#include "../include/User.h"
#include "utils/Exceptions.h"

AuthManager::AuthManager(MyApp *mainApp) : mainApp(mainApp) {}

std::string encryptPassword(const std::string &password)
{
    // XOR encryption
    const char key = 'K';
    std::string encrypted = password;
    for (char &c : encrypted)
    {
        c ^= key;
    }
    return encrypted;
}

bool AuthManager::verifyPassword(const string &inputPassword, const string &storedPassword)
{
    return inputPassword == encryptPassword(storedPassword);
}

User AuthManager::registerUser(const string &username, const string &password, UserRole role)
{
    DatabaseManager *db = mainApp->getDatabaseManager();
    vector<User> users = db->getAllUsers();

    for (const auto &user : users)
        if (user.username == username)
            throw ClientException("This username already exists!");

    string hashPassword = encryptPassword(password);
    User newUser(username, hashPassword, role, 0);

    db->saveUser(newUser);
    cout << "User \"" << username << "\" registered successfully." << endl;
    return newUser;
}

User AuthManager::loginUser(const string &username, const string &password)
{
    DatabaseManager *db = mainApp->getDatabaseManager();
    vector<User> users = db->getAllUsers();

    for (const auto &user : users)
    {
        if (user.username == username)
        {
            if (verifyPassword(password, user.password))
            {
                cout << "Login successful for user \"" << username << "\"." << endl;
                mainApp->setCurrentUser(user);
                return user;
            }
            else
            {
                cout << "Error: Incorrect password for user \"" << username << "\"." << endl;
                throw ClientException("Incorrect username or password!");
            }
        }
    }

    cout << "Error: Incorrect username for user \"" << username << "\"." << endl;
    throw ClientException("Incorrect username or password!");
}

void AuthManager::logOut()
{
    mainApp->setCurrentUser(User());
}
