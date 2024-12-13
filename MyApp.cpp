#include "./include/MyApp.h"

#include "./include/DatabaseManager.h"
#include "./include/AuthManager.h"

#include "./include/AdminPanelFrame.h"
#include "./include/AuthFrame.h"
#include "./include/UserFrame.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    try
    {
        databaseManager = new DatabaseManager("users.txt");
        authManager = new AuthManager(this);

        userFrame = new UserFrame("User", this);
        adminPanelFrame = new AdminPanelFrame("Admin Panel", this);
        authFrame = new AuthFrame("Login", this);

        authFrame->Show();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return true;
    }

    return true;
}

MyApp::~MyApp()
{
    delete databaseManager;
    delete authManager;
    delete adminPanelFrame;
    delete userFrame;
}
