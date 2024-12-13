#ifndef MYAPP_H
#define MYAPP_H

#include <wx/wx.h>
#include "User.h"

class AuthManager;
class DatabaseManager;
class UserManager;
class AdminPanelFrame;
class AuthFrame;
class UserFrame;

class MyApp : public wxApp
{
private:
    DatabaseManager *databaseManager;
    AuthManager *authManager;
    UserManager *userManager;
    AdminPanelFrame *adminPanelFrame;
    AuthFrame *authFrame;
    UserFrame *userFrame;

    User currentUser;

public:
    virtual ~MyApp();

    virtual bool OnInit() override;

    DatabaseManager *getDatabaseManager() const { return databaseManager; }
    AuthManager *getAuthManager() const { return authManager; }
    UserManager *getUserManager() const { return userManager; }
    AdminPanelFrame *getAdminPanelFrame() const { return adminPanelFrame; }
    AuthFrame *getAuthFrame() const { return authFrame; }
    UserFrame *getUserFrame() const { return userFrame; }
    const User &getCurrentUser() const { return currentUser; }
    void setCurrentUser(const User &user) { currentUser = user; }
};

#endif
